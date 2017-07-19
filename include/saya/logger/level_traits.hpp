#ifndef SAYA_LOGGER_LEVEL_TRAITS_HPP_
#define SAYA_LOGGER_LEVEL_TRAITS_HPP_

#include "saya/logger_fwd.hpp"
#include "saya/logger/level.hpp"

#include "saya/string_config.hpp"

namespace saya {

#define SAYA_DEF(CHAR_TYPE, BINDER) \
    template<> \
    struct basic_logger_level_traits<CHAR_TYPE, logger_level::INFO> \
    { \
        static constexpr std::size_t label_len = 4; \
        static CHAR_TYPE const* label() { return BINDER("INFO"); } \
        \
        template<class Color> \
        static auto color() { return Color::fg::LIGHTGRAY(); } \
    }; \
    template<> \
    struct basic_logger_level_traits<CHAR_TYPE, logger_level::WARN> \
    { \
        static constexpr std::size_t label_len = 4; \
        static CHAR_TYPE const* label() { return BINDER("WARN"); } \
        \
        template<class Color> \
        static auto color() { return std::basic_string<CHAR_TYPE>(Color::bg::YELLOW()) + Color::fg::BLACK(); } \
    }; \
    template<> \
    struct basic_logger_level_traits<CHAR_TYPE, logger_level::ERROR> \
    { \
        static constexpr std::size_t label_len = 5; \
        static CHAR_TYPE const* label() { return BINDER("ERROR"); } \
        \
        template<class Color> \
        static auto color() { return std::basic_string<CHAR_TYPE>(Color::bg::RED()) + Color::fg::WHITE(); } \
    };

SAYA_STRING_CONFIG_DEFINE(SAYA_DEF)
#undef SAYA_DEF

} // saya

#endif
