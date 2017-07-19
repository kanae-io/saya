#ifndef SAYA_LOGGER_TRAITS_HPP_
#define SAYA_LOGGER_TRAITS_HPP_

#include "saya/logger_fwd.hpp"
#include "saya/logger/level_traits.hpp"

#include "saya/string_config.hpp"
#include "saya/stream_object.hpp"

#include <string>
#include <ostream>
#include <streambuf>

namespace saya {

#define SAYA_DEF(CHAR_TYPE, BINDER) \
    template<class Color> \
    struct basic_logger_traits<CHAR_TYPE, Color> \
    { \
        using char_type = CHAR_TYPE; \
        using color_type = Color; \
        using string_type = std::basic_string<char_type>; \
        using ostream_type = std::basic_ostream<char_type>; \
        using streambuf_type = std::basic_streambuf<char_type>; \
        using stream_object_type = stream_object<char_type>; \
        \
        template<class Level> \
        using level_traits_type = basic_logger_level_traits<char_type, Level>; \
        \
        static char_type const* prompt_format() { return BINDER("[%s] %s(%s)%s %s"); } \
        static char_type const* prompt_no_color_format() { return BINDER("[%s] (%s) %s"); } \
        \
        template<class Level> \
        static std::size_t prompt_size(string_type const& prompt) { return level_traits_type<Level>::label_len + 6 + prompt.size(); } \
        \
        static char_type const* noprompt_format() { return BINDER("%s(%s)%s %s"); } \
        static char_type const* noprompt_no_color_format() { return BINDER("(%s) %s"); } \
        \
        template<class Level> \
        static std::size_t noprompt_size() { return level_traits_type<Level>::label_len + 3; } \
    };

SAYA_STRING_CONFIG_DEFINE(SAYA_DEF)
#undef SAYA_DEF

} // saya

#endif
