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
        static constexpr std::size_t label_len() noexcept { return 4; } \
        static CHAR_TYPE const* label() noexcept { return BINDER("INFO"); } \
        static constexpr bool need_paren() noexcept { return true; } \
        \
        template<class Color> \
        static auto color() { return std::basic_string<CHAR_TYPE>(Color::bg::DARKGRAY()) + Color::fg::WHITE(); } \
    }; \
    template<> \
    struct basic_logger_level_traits<CHAR_TYPE, logger_level::WARN> \
    { \
        static constexpr std::size_t label_len() noexcept { return 4; } \
        static constexpr CHAR_TYPE const* label() noexcept { return BINDER("WARN"); } \
        static constexpr bool need_paren() noexcept { return true; } \
        \
        template<class Color> \
        static auto color() { return std::basic_string<CHAR_TYPE>(Color::bg::YELLOW()) + Color::fg::BLACK(); } \
    }; \
    template<> \
    struct basic_logger_level_traits<CHAR_TYPE, logger_level::ERROR> \
    { \
        static constexpr std::size_t label_len() noexcept { return 5; } \
        static constexpr CHAR_TYPE const* label() noexcept { return BINDER("ERROR"); } \
        static constexpr bool need_paren() noexcept { return true; } \
        \
        template<class Color> \
        static auto color() { return std::basic_string<CHAR_TYPE>(Color::bg::RED()) + Color::fg::WHITE(); } \
    }; \
    template<> \
    struct basic_logger_level_traits<CHAR_TYPE, logger_level::NOTE> \
    { \
        static constexpr std::size_t label_len() noexcept { return 5; } \
        static constexpr CHAR_TYPE const* label() noexcept { return BINDER("note:"); } \
        static constexpr bool need_paren() noexcept { return false; } \
        \
        template<class Color> \
        static auto color() { return std::basic_string<CHAR_TYPE>(Color::BOLD()) + Color::fg::DARKGRAY(); } \
    };

SAYA_STRING_CONFIG_DEFINE(SAYA_DEF)
#undef SAYA_DEF

} // saya

#endif
