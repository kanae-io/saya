#ifndef SAYA_IKA_COMPILER_SOURCE_DATA_HPP
#define SAYA_IKA_COMPILER_SOURCE_DATA_HPP

#include <boost/filesystem/path.hpp>
#include <boost/functional/hash.hpp>

#include <iostream>
#include <type_traits>


namespace saya { namespace ika {

enum class SourceKind : unsigned
{
    MEMORY,
    FILE,
};

// ---------------------------------------------------------

struct source_id;

namespace detail {

struct source_id_access
{
protected:
    static void mark_as_std(source_id& id);
};

} // detail

// ---------------------------------------------------------

struct source_id
{
    friend struct detail::source_id_access;

    SourceKind kind;
    std::string id;

    static source_id init(SourceKind k, std::string const& loc, std::string const& buf);
    bool is_std() const noexcept { return is_std_; }

    inline bool operator==(source_id const& rhs) const noexcept
    {
        return is_std_ == rhs.is_std_ && kind == rhs.kind && id == rhs.id;
    }

private:
    void is_std(bool flag) { is_std_ = flag; }
    bool is_std_{false};
};
std::ostream& operator<<(std::ostream& os, source_id const& sid);

// ---------------------------------------------------------

inline std::size_t hash_value(source_id const& sid)
{
    std::size_t seed = 0;
    boost::hash_combine(seed, sid.is_std());
    boost::hash_combine(seed, static_cast<std::underlying_type_t<SourceKind>>(sid.kind));
    boost::hash_combine(seed, sid.id);
    return seed;
}

struct source_data
{
    std::string location;
    std::string buf;

    std::size_t size() const noexcept { return buf.size(); }
};

std::ostream& operator<<(std::ostream& os, source_data const& data);

}} // saya


namespace std {

template<>
class hash<saya::ika::source_id>
{
public:
    size_t operator()(saya::ika::source_id const& sid) const
    {
        return saya::ika::hash_value(sid);
    }
};

} // std

#endif
