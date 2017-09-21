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
#include <functional>


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
    using envs_type = basic_logger_env_set<env_type>;
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
    using note_type = logger_stream_type<logger_level::note>;
    using notes_type = std::deque<note_type>;

    using prompt_fetcher_type = std::function<string_type()>;

    explicit basic_logger(string_type const& prompt = {})
        : prompt_(prompt)
        , pf_([this] () -> string_type { return this->prompt_; })
    {}

    explicit basic_logger(
        env_type env,
        string_type const& prompt = {}
    )
        : prompt_(prompt)
        , pf_([this] () -> string_type { return this->prompt_; })
    {
        envs_.emplace_back(std::move(env));
    }

    explicit basic_logger(
        envs_type envs,
        string_type const& prompt = {}
    )
        : prompt_(prompt)
        , envs_(std::move(envs))
        , pf_([this] () -> string_type { return this->prompt_; })
    {}

    explicit basic_logger(basic_logger const& other, string_type const& prompt = {})
        : basic_logger(other.envs_, prompt)
    {}


    //basic_logger(basic_logger const&) = delete;
    basic_logger(basic_logger&&) = delete;

    basic_logger& operator=(basic_logger const&) = delete;
    basic_logger& operator=(basic_logger&&) = delete;

    ~basic_logger() = default;


    logger_stream_type<logger_level::info>
    info() const { return {const_cast<self_type*>(this)}; }

    logger_stream_type<logger_level::warn>
    warn() const { return {const_cast<self_type*>(this)}; }

    logger_stream_type<logger_level::error>
    error() const { return {const_cast<self_type*>(this)}; }

    // 'extra' output for in addition to all types above
    note_type&
    note() const
    {
        lock_type lock(note_mtx_);
        notes_.emplace_back();
        return const_cast<note_type&>(notes_.back());
    }

    void tee(env_type another_env)
    {
        envs_.emplace_back(std::move(another_env));
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

    envs_type const& envs() const { return envs_; }

    void set_prompt_fetcher(prompt_fetcher_type pf) { pf_ = std::move(pf); }

protected:
    mutable mutex_type prompt_mtx_;
    mutable mutex_type note_mtx_;
    mutable string_type prompt_{};
    mutable char const* prompt_color_{traits_type::color_type::fg::BLUE()};

    mutable notes_type notes_;

    mutable envs_type envs_;

    string_type pf() const { return pf_(); }

private:
    prompt_fetcher_type pf_;
};

} // saya

#endif
