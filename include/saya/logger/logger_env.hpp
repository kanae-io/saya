#ifndef SAYA_LOGGER_LOGGER_ENV_HPP
#define SAYA_LOGGER_LOGGER_ENV_HPP

#include "saya/logger/traits.hpp"

#include <iostream>
#include <fstream>


namespace saya {

template<class CharT, class Traits = basic_logger_traits<CharT>>
struct basic_logger_env
{
    using streambuf_type = typename Traits::streambuf_type;

    streambuf_type* out{std::cerr.rdbuf()};
    bool need_color{true};

    basic_logger_env() = default;
    basic_logger_env(basic_logger_env const&) = default;
    basic_logger_env(basic_logger_env&&) = default;

    explicit basic_logger_env(std::streambuf* out_, bool need_color = true)
        : out(out_)
        , need_color(need_color)
    {}

    ~basic_logger_env() noexcept = default;

    basic_logger_env& operator=(basic_logger_env const&) = default;
    basic_logger_env& operator=(basic_logger_env&&) = default;
};

using logger_env = basic_logger_env<char>;
using wlogger_env = basic_logger_env<wchar_t>;

} // saya

#endif
