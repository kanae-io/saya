#ifndef SAYA_LOGGER_LOGGER_HPP_
#define SAYA_LOGGER_LOGGER_HPP_

#include "saya/logger_fwd.hpp"
#include "saya/logger/logger_env.hpp"
#include "saya/logger/level.hpp"
#include "saya/logger/traits.hpp"
#include "saya/logger/stream.hpp"
// #include "saya/logger/access.hpp"

#include <deque>
#include <memory>
#include <mutex>

namespace saya {

template<class CharT, class Traits>
class basic_logger
{
public:
    template<class>
    friend class detail::logger_access;

    using char_type = CharT;
    using traits_type = Traits;
    using env_type = basic_logger_env<char_type, traits_type>;
    using self_type = basic_logger<char_type, traits_type>;
    using string_type = typename traits_type::string_type;
    using ostream_type = typename traits_type::ostream_type;
    using streambuf_type = typename traits_type::streambuf_type;
    using ofstream_type = std::basic_ofstream<char_type>;
    using stream_object_type = typename traits_type::stream_object_type;

    template<class Level>
    using logger_stream_type = basic_logger_stream<self_type, Level>;

    using mutex_type = std::mutex;
    using lock_type = std::lock_guard<mutex_type>;
    using note_type = logger_stream_type<logger_level::NOTE>;
    using notes_type = std::deque<note_type>;

    explicit basic_logger(string_type const& prompt = {})
        : prompt_(prompt)
        , env_()
    {}

    explicit basic_logger(
        env_type env,
        string_type const& prompt = {}
    )
        : prompt_(prompt)
        , env_(std::move(env))
    {}

    explicit basic_logger(basic_logger const& other, string_type const& prompt = {})
        : basic_logger(other.env_, prompt)
    {}


    //basic_logger(basic_logger const&) = delete;
    basic_logger(basic_logger&&) = delete;

    basic_logger& operator=(basic_logger const&) = delete;
    basic_logger& operator=(basic_logger&&) = delete;

    ~basic_logger() = default;


    logger_stream_type<logger_level::INFO>
    info() const { return {const_cast<self_type*>(this)}; }

    logger_stream_type<logger_level::WARN>
    warn() const { return {const_cast<self_type*>(this)}; }

    logger_stream_type<logger_level::ERROR>
    error() const { return {const_cast<self_type*>(this)}; }

    // 'extra' output for in addition to all types above
    note_type&
    note() const
    {
        lock_type lock(note_mtx_);
        notes_.emplace_back();
        return const_cast<note_type&>(notes_.back());
    }

    void prompt(string_type const& name) const
    {
        lock_type lock(prompt_mtx_);
        prompt_ = name;
    }

    void prompt_color(char const* color_code) const
    {
        lock_type lock(prompt_mtx_);
        prompt_color_ = color_code;
    }

    bool color() const
    {
        lock_type lock(prompt_mtx_);
        return env_.need_color;
    }

    void color(bool flag) const
    {
        lock_type lock(prompt_mtx_);
        env_.need_color = flag;
    }

    env_type const& env() const { return env_; }

protected:
    mutable mutex_type prompt_mtx_;
    mutable mutex_type note_mtx_;
    mutable string_type prompt_{};
    mutable char const* prompt_color_{traits_type::color_type::fg::BLUE()};

    mutable notes_type notes_;

    mutable env_type env_;
};

} // saya

#endif
