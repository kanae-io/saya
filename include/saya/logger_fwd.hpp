#ifndef SAYA_LOGGER_FWD_HPP_
#define SAYA_LOGGER_FWD_HPP_

#include "saya/console_fwd.hpp"

namespace saya {

namespace detail {

template<class Logger>
class logger_access;

} // detail

namespace logger_level {

struct INFO;
struct WARN;
struct ERROR;

} // logger_level

template<class CharT, class Color = console::basic_color<CharT>>
struct basic_logger_traits;

template<class CharT, class Level>
struct basic_logger_level_traits;

template<class Logger, class Level>
class basic_logger_stream;

template<class CharT, class Traits = basic_logger_traits<CharT>>
class basic_logger;

// ---

using logger = basic_logger<char>;
using logger_no_color = basic_logger<char, basic_logger_traits<char, console::no_color>>;

using wlogger = basic_logger<wchar_t>;
using wlogger_no_color = basic_logger<wchar_t, basic_logger_traits<wchar_t, console::no_color>>;

} // saya

#endif
