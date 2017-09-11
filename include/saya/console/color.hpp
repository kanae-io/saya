#ifndef SAYA_CONSOLE_COLOR_HPP_
#define SAYA_CONSOLE_COLOR_HPP_

#include "saya/console_fwd.hpp"
#include "saya/string_config.hpp"

#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/expand.hpp>
#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/arithmetic/add.hpp>

#include <iostream>
#include <string>


namespace saya { namespace console { namespace detail {

struct no_color_t
{
    inline explicit operator std::string() { return {}; }
    inline explicit operator std::wstring() { return {}; }
};

inline std::string operator+(no_color_t const&, char const* c) { return {c}; }
inline std::string operator+(char const* c, no_color_t const&) { return {c}; }
inline std::wstring operator+(no_color_t const&, wchar_t const* c) { return {c}; }
inline std::wstring operator+(wchar_t const* c, no_color_t const&) { return {c}; }

inline std::ostream& operator<<(std::ostream& os, no_color_t const&)
{
    return os;
}

}}} // saya

namespace saya { namespace console {

#define SAYA_CODE_PREFIX "\x1B["
#define SAYA_CODE_SUFFIX "m"
#define SAYA_CODE_256_SEP ";5;"

// nasty generation
// fg = 0, bg = 10
// reset = +20
// basic = +30, high = +90
#define SAYA_COLOR_256_fg_ID           38
#define SAYA_COLOR_256_bg_ID           48

#define SAYA_COLOR_TAG_SYSTEM \
    (RESET) \
    (BOLD)(DIM)(UNDERLINED)(BLINK)(INVERSE)(HIDDEN) \
    (RESET_BOLD)(RESET_DIM)(RESET_UNDERLINED)(RESET_BLINK)(RESET_INVERSE)(RESET_HIDDEN)

#define SAYA_COLOR_TAG_COLOR \
    (DEFAULT)(BLACK)(RED)(GREEN)(YELLOW)(BLUE)(MAGENTA)(CYAN)(LIGHTGRAY)(DARKGRAY)(LIGHTRED)(LIGHTGREEN)(LIGHTYELLOW)(LIGHTBLUE)(LIGHTMAGENTA)(LIGHTCYAN)(WHITE)

#define SAYA_COLOR_RESET               0

#define SAYA_COLOR_BOLD                1
#define SAYA_COLOR_DIM                 2
#define SAYA_COLOR_UNDERLINED          4
#define SAYA_COLOR_BLINK               5
#define SAYA_COLOR_INVERSE             7
#define SAYA_COLOR_HIDDEN              8

#define SAYA_COLOR_RESET_BOLD          21
#define SAYA_COLOR_RESET_DIM           22
#define SAYA_COLOR_RESET_UNDERLINED    24
#define SAYA_COLOR_RESET_BLINK         25
#define SAYA_COLOR_RESET_INVERSE       27
#define SAYA_COLOR_RESET_HIDDEN        28

#define SAYA_COLOR_fg_DEFAULT          39
#define SAYA_COLOR_fg_BLACK            30
#define SAYA_COLOR_fg_RED              31
#define SAYA_COLOR_fg_GREEN            32
#define SAYA_COLOR_fg_YELLOW           33
#define SAYA_COLOR_fg_BLUE             34
#define SAYA_COLOR_fg_MAGENTA          35
#define SAYA_COLOR_fg_CYAN             36
#define SAYA_COLOR_fg_LIGHTGRAY        37

#define SAYA_COLOR_bg_DEFAULT          49
#define SAYA_COLOR_bg_BLACK            40
#define SAYA_COLOR_bg_RED              41
#define SAYA_COLOR_bg_GREEN            42
#define SAYA_COLOR_bg_YELLOW           43
#define SAYA_COLOR_bg_BLUE             44
#define SAYA_COLOR_bg_MAGENTA          45
#define SAYA_COLOR_bg_CYAN             46
#define SAYA_COLOR_bg_LIGHTGRAY        47

#define SAYA_COLOR_fg_DARKGRAY         90
#define SAYA_COLOR_fg_LIGHTRED         91
#define SAYA_COLOR_fg_LIGHTGREEN       92
#define SAYA_COLOR_fg_LIGHTYELLOW      93
#define SAYA_COLOR_fg_LIGHTBLUE        94
#define SAYA_COLOR_fg_LIGHTMAGENTA     95
#define SAYA_COLOR_fg_LIGHTCYAN        96
#define SAYA_COLOR_fg_WHITE            97

#define SAYA_COLOR_bg_DARKGRAY         100
#define SAYA_COLOR_bg_LIGHTRED         101
#define SAYA_COLOR_bg_LIGHTGREEN       102
#define SAYA_COLOR_bg_LIGHTYELLOW      103
#define SAYA_COLOR_bg_LIGHTBLUE        104
#define SAYA_COLOR_bg_LIGHTMAGENTA     105
#define SAYA_COLOR_bg_LIGHTCYAN        106
#define SAYA_COLOR_bg_WHITE            107


#define SAYA_COLOR_SYSTEM_DEF(r, cfg, system) \
    static constexpr BOOST_PP_TUPLE_ELEM(0, cfg) const* system() noexcept { \
        return \
            BOOST_PP_TUPLE_ELEM(1, cfg)(SAYA_CODE_PREFIX) \
            BOOST_PP_TUPLE_ELEM(1, cfg)( \
                BOOST_PP_STRINGIZE(BOOST_PP_CAT(SAYA_COLOR_, system)) \
            ) \
            BOOST_PP_TUPLE_ELEM(1, cfg)(SAYA_CODE_SUFFIX) \
        ; \
    }

#define SAYA_COLOR_COLOR_DEF(r, cfg, color) \
    static constexpr BOOST_PP_TUPLE_ELEM(0, cfg) const* color() noexcept { \
        return \
            BOOST_PP_TUPLE_ELEM(1, cfg)(SAYA_CODE_PREFIX) \
            BOOST_PP_TUPLE_ELEM(1, cfg)( \
                BOOST_PP_STRINGIZE( \
                    BOOST_PP_CAT(BOOST_PP_CAT(BOOST_PP_CAT(SAYA_COLOR_, BOOST_PP_TUPLE_ELEM(2, cfg)), _), color) \
                ) \
            ) \
            BOOST_PP_TUPLE_ELEM(1, cfg)(SAYA_CODE_SUFFIX) \
        ; \
    }

#define SAYA_DEF_I_FOR(cfg, i) \
    BOOST_PP_ADD(BOOST_PP_TUPLE_ELEM(0, cfg), i)

#define SAYA_COLOR_256_DEF(z, i, cfg) \
    static constexpr BOOST_PP_TUPLE_ELEM(1, cfg) const* BOOST_PP_CAT(code, SAYA_DEF_I_FOR(cfg, i))() noexcept { \
        return \
            BOOST_PP_TUPLE_ELEM(2, cfg)(SAYA_CODE_PREFIX) \
            BOOST_PP_TUPLE_ELEM(2, cfg)( \
                BOOST_PP_STRINGIZE( \
                    BOOST_PP_CAT(BOOST_PP_CAT(SAYA_COLOR_256_, BOOST_PP_TUPLE_ELEM(3, cfg)), _ID) \
                ) \
            ) \
            BOOST_PP_TUPLE_ELEM(2, cfg)(SAYA_CODE_256_SEP) \
            BOOST_PP_TUPLE_ELEM(2, cfg)(BOOST_PP_STRINGIZE(SAYA_DEF_I_FOR(cfg, i))) \
            BOOST_PP_TUPLE_ELEM(2, cfg)(SAYA_CODE_SUFFIX) \
        ; \
    }

#define SAYA_DEF(CHAR_TYPE, BINDER) \
    template<> \
    struct basic_color<CHAR_TYPE> \
    { \
        BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_SYSTEM_DEF, (CHAR_TYPE, BINDER), SAYA_COLOR_TAG_SYSTEM) \
        \
        struct fg { \
            BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_COLOR_DEF, (CHAR_TYPE, BINDER, fg), SAYA_COLOR_TAG_COLOR) \
            BOOST_PP_REPEAT(127, SAYA_COLOR_256_DEF, (0, CHAR_TYPE, BINDER, fg)) \
        }; \
        struct bg { \
            BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_COLOR_DEF, (CHAR_TYPE, BINDER, bg), SAYA_COLOR_TAG_COLOR) \
            BOOST_PP_REPEAT(127, SAYA_COLOR_256_DEF, (0, CHAR_TYPE, BINDER, bg)) \
        }; \
    };

SAYA_STRING_CONFIG_DEFINE(SAYA_DEF)
#undef SAYA_COLOR_256_DEF
#undef SAYA_COLOR_COLOR_DEF
#undef SAYA_COLOR_SYSTEM_DEF
#undef SAYA_DEF

// no color defs -----------------------------

#define SAYA_COLOR_SYSTEM_DEF(r, _, system) \
    static constexpr detail::no_color_t system() noexcept { return {}; }

#define SAYA_COLOR_COLOR_DEF(r, _, color) \
    static constexpr detail::no_color_t color() noexcept { return {}; }

#define SAYA_COLOR_256_DEF(z, i, cfg) \
    static constexpr detail::no_color_t BOOST_PP_CAT(code, SAYA_DEF_I_FOR(cfg, i))() noexcept { return {}; }

template<>
struct basic_color<void>
{
    BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_SYSTEM_DEF, _, SAYA_COLOR_TAG_SYSTEM)

    struct fg {
        BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_COLOR_DEF, _, SAYA_COLOR_TAG_COLOR)
        BOOST_PP_REPEAT(127, SAYA_COLOR_256_DEF, (0))
        BOOST_PP_REPEAT(127, SAYA_COLOR_256_DEF, (127))
    };
    struct bg {
        BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_COLOR_DEF, _, SAYA_COLOR_TAG_COLOR)
        BOOST_PP_REPEAT(127, SAYA_COLOR_256_DEF, (0))
        BOOST_PP_REPEAT(127, SAYA_COLOR_256_DEF, (127))
    };
};

inline static void list_colors()
{
    static auto const expand5 = [] (unsigned c) {
        static std::string const SPACER{" "};

        if (c < 10) {
            return SPACER + "  " + std::to_string(c) + SPACER;
        } else if (c < 100) {
            return SPACER + " " + std::to_string(c) + SPACER;
        } else {
            return SPACER + std::to_string(c) + SPACER;
        }
    };

    #define SAYA_DEF_DUMMY \
        basic_color<char>::fg::YELLOW() << \
        basic_color<char>::bg::BLACK() << \
        basic_color<char>::BOLD() << \
        basic_color<char>::UNDERLINED() << \
        "(dirty dummy string)" << basic_color<char>::RESET() << " " << \
        basic_color<char>::fg::YELLOW() << \
        basic_color<char>::bg::BLACK() << \
        basic_color<char>::BOLD() << \
        basic_color<char>::UNDERLINED()

    std::cout << "\n[system] saya::console::NAME(); -----------------------\n";
    #undef SAYA_DEF
    #define SAYA_DEF(r, _, system) \
        std::cout << SAYA_DEF_DUMMY << basic_color<char>::system() << BOOST_PP_STRINGIZE(system) << basic_color<char>::RESET() << "\n";
    BOOST_PP_SEQ_FOR_EACH(SAYA_DEF, _, SAYA_COLOR_TAG_SYSTEM)

    #undef SAYA_DEF_DUMMY

    // -------------------------------------------

    std::cout << "\n[fg] saya::console::fg::NAME(); ------------------------\n";
    #undef SAYA_DEF
    #define SAYA_DEF(r, _, c) \
        std::cout << basic_color<char>::fg::c() << BOOST_PP_STRINGIZE(c) << basic_color<char>::RESET() << "\n";
    BOOST_PP_SEQ_FOR_EACH(SAYA_DEF, _, SAYA_COLOR_TAG_COLOR)

    std::cout << "\n[bg] saya::console::bg::NAME(); ------------------------\n";
    #undef SAYA_DEF
    #define SAYA_DEF(r, _, c) \
        std::cout << basic_color<char>::bg::c() << BOOST_PP_STRINGIZE(c) << basic_color<char>::RESET() << "\n";
    BOOST_PP_SEQ_FOR_EACH(SAYA_DEF, _, SAYA_COLOR_TAG_COLOR)

    std::cout << "\n\n[fg - 256] saya::console::fg::codeN(); ------------------------\n";
    #undef SAYA_DEF
    #define SAYA_DEF(z, i, _) \
        std::cout << basic_color<char>::fg::BOOST_PP_CAT(code, i)() << expand5(i) << basic_color<char>::RESET() << " "; \
        if (i % 8 == 0) std::cout << "\n";
    BOOST_PP_REPEAT(127, SAYA_DEF, _)

    std::cout << "\n\n[bg - 256] saya::console::bg::codeN(); ------------------------\n";
    #undef SAYA_DEF
    #define SAYA_DEF(z, i, _) \
        std::cout << basic_color<char>::bg::BOOST_PP_CAT(code, i)() << expand5(i) << basic_color<char>::RESET() << " "; \
        if (i % 8 == 0) std::cout << "\n";
    BOOST_PP_REPEAT(127, SAYA_DEF, _)

    std::cout << std::endl;
}

#undef SAYA_COLOR_256_DEF
#undef SAYA_COLOR_COLOR_DEF
#undef SAYA_COLOR_SYSTEM_DEF

#undef SAYA_COLOR_TAG_256
#undef SAYA_COLOR_TAG_COLOR
#undef SAYA_COLOR_TAG_SYSTEM

#undef SAYA_CODE_PREFIX
#undef SAYA_CODE_SUFFIX
#undef SAYA_CODE_256_SEP
#undef SAYA_COLOR_256_fg_ID
#undef SAYA_COLOR_256_bg_ID

#undef SAYA_COLOR_RESET

#undef SAYA_COLOR_BOLD
#undef SAYA_COLOR_DIM
#undef SAYA_COLOR_UNDERLINED
#undef SAYA_COLOR_BLINK
#undef SAYA_COLOR_INVERSE
#undef SAYA_COLOR_HIDDEN

#undef SAYA_COLOR_RESET_BOLD
#undef SAYA_COLOR_RESET_DIM
#undef SAYA_COLOR_RESET_UNDERLINED
#undef SAYA_COLOR_RESET_BLINK
#undef SAYA_COLOR_RESET_INVERSE
#undef SAYA_COLOR_RESET_HIDDEN

#undef SAYA_COLOR_fg_DEFAULT
#undef SAYA_COLOR_fg_BLACK
#undef SAYA_COLOR_fg_RED
#undef SAYA_COLOR_fg_GREEN
#undef SAYA_COLOR_fg_YELLOW
#undef SAYA_COLOR_fg_BLUE
#undef SAYA_COLOR_fg_MAGENTA
#undef SAYA_COLOR_fg_CYAN
#undef SAYA_COLOR_fg_LIGHTGRAY

#undef SAYA_COLOR_bg_DEFAULT
#undef SAYA_COLOR_bg_BLACK
#undef SAYA_COLOR_bg_RED
#undef SAYA_COLOR_bg_GREEN
#undef SAYA_COLOR_bg_YELLOW
#undef SAYA_COLOR_bg_BLUE
#undef SAYA_COLOR_bg_MAGENTA
#undef SAYA_COLOR_bg_CYAN
#undef SAYA_COLOR_bg_LIGHTGRAY

#undef SAYA_COLOR_fg_DARKGRAY
#undef SAYA_COLOR_fg_LIGHTRED
#undef SAYA_COLOR_fg_LIGHTGREEN
#undef SAYA_COLOR_fg_LIGHTYELLOW
#undef SAYA_COLOR_fg_LIGHTBLUE
#undef SAYA_COLOR_fg_LIGHTMAGENTA
#undef SAYA_COLOR_fg_LIGHTCYAN
#undef SAYA_COLOR_fg_WHITE

#undef SAYA_COLOR_bg_DARKGRAY
#undef SAYA_COLOR_bg_LIGHTRED
#undef SAYA_COLOR_bg_LIGHTGREEN
#undef SAYA_COLOR_bg_LIGHTYELLOW
#undef SAYA_COLOR_bg_LIGHTBLUE
#undef SAYA_COLOR_bg_LIGHTMAGENTA
#undef SAYA_COLOR_bg_LIGHTCYAN
#undef SAYA_COLOR_bg_WHITE

#undef SAYA_DEF_I_FOR
#undef SAYA_DEF

}} // saya

#endif
