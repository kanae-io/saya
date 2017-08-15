#ifndef SAYA_FLYWEIGHT_HPP
#define SAYA_FLYWEIGHT_HPP

#include <boost/core/ignore_unused.hpp>

#include <iostream>
#include <unordered_map>
#include <functional>
#include <memory>
#include <mutex>
#include <type_traits>


namespace saya { namespace flyweights {

template<class T, class FactoryTag>
struct Init
{
    Init()
    {
        boost::ignore_unused(m());
        // boost::ignore_unused(storage());
    }

    static auto& m()
    {
        static std::mutex i;
        return i;
    }

    static auto& storage()
    {
        static std::unordered_map<T, std::shared_ptr<T>> i;
        return i;
    }
};

template<class T, class FactoryTag, class SelfTag = void>
class flyweight
{
public:
    static_assert(std::is_nothrow_move_constructible<T>::value, "T must be nothrow-move-constructible");
    static_assert(std::is_nothrow_move_assignable<T>::value, "T must be nothrow-move-assignable");

    using init_type = Init<T, FactoryTag>;
    using mutex_type = std::mutex;
    using lock_type = std::lock_guard<mutex_type>;
    using base_type = T;

    flyweight() noexcept = default;
    flyweight(flyweight&&) noexcept = default;
    flyweight(flyweight const&) = default;

    flyweight& operator=(flyweight&&) noexcept = default;
    flyweight& operator=(flyweight const&) = default;

    explicit flyweight(T&& v) noexcept
    {
        lock_type lock(init_type::m());
        auto& target = init_type::storage()[v];

        if (!target) {
            hold_ = target = std::make_shared<T>(std::move(v));
        } else {
            hold_ = target;
        }
    }

    explicit flyweight(T const& v)
    {
        lock_type lock(init_type::m());
        auto& target = init_type::storage()[v];

        if (!target) {
            hold_ = target = std::make_shared<T>(v);
        } else {
            hold_ = target;
        }
    }

    ~flyweight()
    {
        if (!hold_) return;

        lock_type lock(init_type::m());

        auto& stor = init_type::storage();
        auto it = stor.find(*hold_);

        if (it->second.use_count() <= 2) {
            stor.erase(it);
        }

        // hold_.reset();
    }

    inline bool operator==(flyweight const& rhs) const noexcept
    {
        return hold_ && rhs.hold_ && (*hold_ == *rhs.hold_);
    }

    inline bool operator==(T const& rhs) const noexcept
    {
        return hold_ && (*hold_ == rhs);
    }

    void swap(flyweight& rhs) noexcept
    {
        using std::swap;

        if (this != std::addressof(rhs)) {
            swap(hold_, rhs.hold_);
        }
    }

    explicit operator bool() const noexcept { return static_cast<bool>(hold_); }

    T const* get() const noexcept { return hold_.get(); }
    T* get() noexcept { return hold_.get(); }

private:
    // init_type init_;
    std::shared_ptr<T> hold_;
};

template<class T, class FactoryTag, class SelfTag>
inline bool operator==(T const& lhs, flyweight<T, FactoryTag, SelfTag> const& rhs) noexcept
{
    return rhs.operator==(lhs);
}

template<class T, class FactoryTag, class SelfTag>
inline std::ostream& operator<<(std::ostream& os, flyweight<T, FactoryTag, SelfTag> const& rhs)
{
    if (static_cast<bool>(rhs)) {
        return os << *rhs.get();
    } else {
        return os << "[invalid_flyweight]";
    }
}

template<class T, class FactoryTag, class SelfTag>
inline std::size_t
hash_value(flyweight<T, FactoryTag, SelfTag> const& fw)
{
    return std::hash<T>()(*fw.get());
}

struct extractor
{
    template<class T, class FactoryTag, class SelfTag>
    inline T const& operator()(flyweight<T, FactoryTag, SelfTag> const& fw) const
    {
        return *fw.get();
    }
};

}} // saya

#endif
