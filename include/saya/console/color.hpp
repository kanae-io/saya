#ifndef SAYA_CONSOLE_COLOR_HPP_
#define SAYA_CONSOLE_COLOR_HPP_

#include "saya/console_fwd.hpp"
#include "saya/string_config.hpp"

#include "boost/preprocessor/tuple.hpp"
#include "boost/preprocessor/seq.hpp"
#include "boost/preprocessor/cat.hpp"
#include "boost/preprocessor/expand.hpp"
#include "boost/preprocessor/stringize.hpp"


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

template<class Stream>
inline Stream& operator<<(Stream& os, no_color_t const&)
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

#define SAYA_COLOR_TAG_256 \
    (1)(2)(3)(4)(5)(6)(7)(8)(9)(10)(11)(12)(13)(14)(15)(16)(17)(18)(19)(20)(21)(22)(23)(24)(25)(26)(27)(28)(29)(30)(31)(32)(33)(34)(35)(36)(37)(38)(39)(40)(41)(42)(43)(44)(45)(46)(47)(48)(49)(50)(51)(52)(53)(54)(55)(56)(57)(58)(59)(60)(61)(62)(63)(64)(65)(66)(67)(68)(69)(70)(71)(72)(73)(74)(75)(76)(77)(78)(79)(80)(81)(82)(83)(84)(85)(86)(87)(88)(89)(90)(91)(92)(93)(94)(95)(96)(97)(98)(99)(100)(101)(102)(103)(104)(105)(106)(107)(108)(109)(110)(111)(112)(113)(114)(115)(116)(117)(118)(119)(120)(121)(122)(123)(124)(125)(126)(127)(128)(129)(130)(131)(132)(133)(134)(135)(136)(137)(138)(139)(140)(141)(142)(143)(144)(145)(146)(147)(148)(149)(150)(151)(152)(153)(154)(155)(156)(157)(158)(159)(160)(161)(162)(163)(164)(165)(166)(167)(168)(169)(170)(171)(172)(173)(174)(175)(176)(177)(178)(179)(180)(181)(182)(183)(184)(185)(186)(187)(188)(189)(190)(191)(192)(193)(194)(195)(196)(197)(198)(199)(200)(201)(202)(203)(204)(205)(206)(207)(208)(209)(210)(211)(212)(213)(214)(215)(216)(217)(218)(219)(220)(221)(222)(223)(224)(225)(226)(227)(228)(229)(230)(231)(232)(233)(234)(235)(236)(237)(238)(239)(240)(241)(242)(243)(244)(245)(246)(247)(248)(249)(250)(251)(252)(253)(254)(255)(256)

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
    static BOOST_PP_TUPLE_ELEM(0, cfg) const* system() { \
        return \
            BOOST_PP_TUPLE_ELEM(1, cfg)(SAYA_CODE_PREFIX) \
            BOOST_PP_TUPLE_ELEM(1, cfg)( \
                BOOST_PP_STRINGIZE(BOOST_PP_CAT(SAYA_COLOR_, system)) \
            ) \
            BOOST_PP_TUPLE_ELEM(1, cfg)(SAYA_CODE_SUFFIX) \
        ; \
    }

#define SAYA_COLOR_COLOR_DEF(r, cfg, color) \
    static BOOST_PP_TUPLE_ELEM(0, cfg) const* color() { \
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

#define SAYA_COLOR_256_DEF(r, cfg, c256) \
    static BOOST_PP_TUPLE_ELEM(0, cfg) const* BOOST_PP_CAT(code, c256)() { \
        return \
            BOOST_PP_TUPLE_ELEM(1, cfg)(SAYA_CODE_PREFIX) \
            BOOST_PP_TUPLE_ELEM(1, cfg)( \
                BOOST_PP_STRINGIZE( \
                    BOOST_PP_CAT(BOOST_PP_CAT(SAYA_COLOR_256_, BOOST_PP_TUPLE_ELEM(2, cfg)), _ID) \
                ) \
            ) \
            BOOST_PP_TUPLE_ELEM(1, cfg)(SAYA_CODE_256_SEP) \
            BOOST_PP_TUPLE_ELEM(1, cfg)(BOOST_PP_STRINGIZE(c256)) \
            BOOST_PP_TUPLE_ELEM(1, cfg)(SAYA_CODE_SUFFIX) \
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
            BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_256_DEF, (CHAR_TYPE, BINDER, fg), SAYA_COLOR_TAG_256) \
        }; \
        struct bg { \
            BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_COLOR_DEF, (CHAR_TYPE, BINDER, bg), SAYA_COLOR_TAG_COLOR) \
            BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_256_DEF, (CHAR_TYPE, BINDER, bg), SAYA_COLOR_TAG_256) \
        }; \
    };

SAYA_STRING_CONFIG_DEFINE(SAYA_DEF)
#undef SAYA_COLOR_256_DEF
#undef SAYA_COLOR_COLOR_DEF
#undef SAYA_COLOR_SYSTEM_DEF
#undef SAYA_DEF

// no color defs -----------------------------

#define SAYA_COLOR_SYSTEM_DEF(r, _, system) \
    static detail::no_color_t system() { return {}; }

#define SAYA_COLOR_COLOR_DEF(r, _, color) \
    static detail::no_color_t color() { return {}; }

#define SAYA_COLOR_256_DEF(r, _, c256) \
    static detail::no_color_t BOOST_PP_CAT(code, c256)() { return {}; }

template<>
struct basic_color<void>
{
    BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_SYSTEM_DEF, _, SAYA_COLOR_TAG_SYSTEM)

    struct fg {
        BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_COLOR_DEF, _, SAYA_COLOR_TAG_COLOR)
        BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_256_DEF, _, SAYA_COLOR_TAG_256)
    };
    struct bg {
        BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_COLOR_DEF, _, SAYA_COLOR_TAG_COLOR)
        BOOST_PP_SEQ_FOR_EACH(SAYA_COLOR_256_DEF, _, SAYA_COLOR_TAG_256)
    };
};

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

}} // saya

#endif
