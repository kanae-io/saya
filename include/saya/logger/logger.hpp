#ifndef SAYA_LOGGER_LOGGER_HPP_
#define SAYA_LOGGER_LOGGER_HPP_

#include "saya/logger_fwd.hpp"
#include "saya/logger/level.hpp"
#include "saya/logger/traits.hpp"
#include "saya/logger/stream.hpp"
// #include "saya/logger/access.hpp"

#include <deque>
#include <fstream>
#include <memory>
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

    using mutex_type = std::mutex;
    using lock_type = std::lock_guard<mutex_type>;
    using note_type = logger_stream_type<logger_level::NOTE>;
    using notes_type = std::deque<std::unique_ptr<note_type>>;

    explicit basic_logger(string_type const& prompt = {})
        : color_(true)
        , prompt_(prompt)
        , out_(stream_object_type::cout().rdbuf())
        , err_(stream_object_type::cerr().rdbuf())
    {}

    explicit basic_logger(
        string_type const& prompt,
        ostream_type& out,
        ostream_type& err
    )
        : color_(true)
        , prompt_(prompt)
        , out_(out.rdbuf())
        , err_(err.rdbuf())
    {}

    explicit basic_logger(
        string_type const& prompt,
        ofstream_type& out,
        ofstream_type& err
    )
        : color_(false)
        , prompt_(prompt)
        , out_(out.rdbuf())
        , err_(err.rdbuf())
    {}

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
        notes_.emplace_back(std::make_unique<note_type>(const_cast<self_type*>(this)));
        return *const_cast<note_type*>(notes_.back().get());
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

    void color(bool flag) const { color_ = flag; }

protected:
    mutable std::atomic<bool> color_;
    mutable mutex_type prompt_mtx_;
    mutable mutex_type note_mtx_;
    mutable string_type prompt_{};
    mutable char const* prompt_color_{traits_type::color_type::fg::BLUE()};

    mutable notes_type notes_;

    streambuf_type* out_;
    streambuf_type* err_;
};

} // saya

#endif
