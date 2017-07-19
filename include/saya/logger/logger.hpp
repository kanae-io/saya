#ifndef SAYA_LOGGER_LOGGER_HPP_
#define SAYA_LOGGER_LOGGER_HPP_

#include "saya/logger_fwd.hpp"
#include "saya/logger/level.hpp"
#include "saya/logger/traits.hpp"
#include "saya/logger/stream.hpp"
// #include "saya/logger/access.hpp"

#include <fstream>
#include <mutex>
#include <atomic>

namespace saya {

template<class CharT, class Traits>
class basic_logger
{
public:
    template<class>
    friend class detail::logger_access;

    using char_type = CharT;
    using traits_type = Traits;
    using self_type = basic_logger<char_type, traits_type>;
    using string_type = typename traits_type::string_type;
    using ostream_type = typename traits_type::ostream_type;
    using streambuf_type = typename traits_type::streambuf_type;
    using ofstream_type = std::basic_ofstream<char_type>;
    using stream_object_type = typename traits_type::stream_object_type;

    template<class Level>
    using logger_stream_type = basic_logger_stream<self_type, Level>;

    explicit basic_logger(string_type const& prompt = {})
        : prompt_(prompt)
        , out_(stream_object_type::cout().rdbuf())
        , err_(stream_object_type::cerr().rdbuf())
    {
        color_ = true;
    }

    explicit basic_logger(
        string_type const& prompt,
        ostream_type& out,
        ostream_type& err
    )
        : prompt_(prompt)
        , out_(out.rdbuf())
        , err_(err.rdbuf())
    {
        color_ = true;
    }

    explicit basic_logger(
        string_type const& prompt,
        ofstream_type& out,
        ofstream_type& err
    )
        : prompt_(prompt)
        , out_(out.rdbuf())
        , err_(err.rdbuf())
    {
        color_ = false;
    }

    logger_stream_type<logger_level::INFO>
    info() const { return {const_cast<self_type*>(this)}; }

    logger_stream_type<logger_level::WARN>
    warn() const { return {const_cast<self_type*>(this)}; }

    logger_stream_type<logger_level::ERROR>
    error() const { return {const_cast<self_type*>(this)}; }

    void prompt(string_type const& name) const
    {
        std::lock_guard<mutex_type> lock(mtx_);
        prompt_ = name;
    }

    void color(bool flag) const { color_ = flag; }

protected:
    using mutex_type = std::mutex;
    mutable mutex_type mtx_;
    mutable string_type prompt_{};
    mutable std::atomic<bool> color_;

    streambuf_type* out_;
    streambuf_type* err_;
};

} // saya

#endif
