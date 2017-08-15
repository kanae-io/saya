#ifndef SAYA_STRING_HPP_
#define SAYA_STRING_HPP_

#include "saya/string_config.hpp"
#include "saya/string_set.hpp"
#include "saya/regex.hpp"
#include "saya/algorithm/string.hpp"

#include <boost/regex.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/unique.hpp>
#include <boost/algorithm/string.hpp>

#include <string>

namespace saya {

template<class CharT>
struct string_traits;

#define SAYA_DEF(CHAR_TYPE, BINDER) \
    template<> \
    struct string_traits<CHAR_TYPE> \
    { \
        typedef CHAR_TYPE char_type; \
        typedef std::basic_string<char_type> string_type; \
        typedef basic_string_set<char_type> string_set_type; \
    \
        static string_set_type alpha_small_set() { return string_set_type{BINDER("abcdefghijklmnopqrstuvwxyz")}; } \
        static string_set_type alpha_capital_set() { return string_set_type{BINDER("ABCDEFGHIJKLMNOPQRSTUVWXYZ")}; } \
        static string_set_type alpha_set() { return alpha_small_set() + alpha_capital_set(); } \
    \
        static string_set_type zenkaku_alpha_small_set() { return string_set_type{BINDER("ａｂｃｄｅｆｇｈｉｊｋｌｍｎｏｐｑｒｓｔｕｖｗｘｙｚ")}; } \
        static string_set_type zenkaku_alpha_capital_set() { return string_set_type{BINDER("ＡＢＣＤＥＦＧＨＩＪＫＬＭＮＯＰＱＲＳＴＵＶＷＸＹＺ")}; } \
        static string_set_type zenkaku_alpha_set() { return zenkaku_alpha_small_set() + zenkaku_alpha_capital_set(); } \
    \
        static string_set_type number_set() { return string_set_type{BINDER("0123456789")}; } \
        static string_set_type zenkaku_number_set() { return string_set_type{BINDER("０１２３４５６７８９")}; } \
    \
        static string_set_type alnum_set() { return alpha_set() + number_set(); } \
        static string_set_type zenkaku_alnum_set() { return zenkaku_alpha_set() + zenkaku_number_set(); } \
    \
        static string_type bar() { return BINDER("ー"); /* not a set! */ } \
        static string_set_type tilde_set() { return string_set_type{BINDER("～〜")}; } \
        static string_set_type tilde_and_bar_set() { return tilde_set() + bar(); } \
    \
        static string_set_type hiragana_set()    { return string_set_type{BINDER("ぁあぃいぅうぇえぉおかがきぎくぐけげこごさざしじすずせぜそぞただちぢっつづてでとどなにぬねのはばぱひびぴふぶぷへべぺほぼぽまみむめもゃやゅゆょよらりるれろゎわゐゑをん")}; } \
        static string_set_type hiragana_tr_set() { return hiragana_set(); /* for now */ } \
    \
        static string_set_type katakana_set()    { return string_set_type{BINDER("ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲンヰヱヴヵヶヷヸヹヺ")}; } \
        static string_set_type katakana_tr_set() { return string_set_type{BINDER("ァアィイゥウェエォオカガキギクグケゲコゴサザシジスズセゼソゾタダチヂッツヅテデトドナニヌネノハバパヒビピフブプヘベペホボポマミムメモャヤュユョヨラリルレロヮワヰヱヲン")}; } \
    \
        static string_set_type katakana_hk_set()      { return string_set_type{BINDER("ァアィイゥウェエォオカキクケコサシスセソタチッツテトナニヌネノハヒフヘホマミムメモャヤュユョヨラリルレロヮワヰヱヲン")}; } \
        static string_set_type hankaku_katakana_set() { return string_set_type{BINDER("ｧｱｨｲｩｳｪｴｫｵｶｷｸｹｺｻｼｽｾｿﾀﾁｯﾂﾃﾄﾅﾆﾇﾈﾉﾊﾋﾌﾍﾎﾏﾐﾑﾒﾓｬﾔｭﾕｮﾖﾗﾘﾙﾚﾛﾜﾜｲｴｦﾝ")}; } \
    \
        static string_set_type hiragana_katakana_set() { return hiragana_set() + katakana_set(); } \
    \
        static string_set_type only_hiragana_set() { return hiragana_set() + tilde_and_bar_set();  } \
        static string_set_type only_katakana_set() { return katakana_set() + tilde_and_bar_set();  } \
        static string_set_type only_hiragana_katakana_set() { return hiragana_katakana_set() + tilde_and_bar_set(); } \
    \
        static string_type space() { return BINDER("\u0020"); } \
        static string_set_type space_set() \
        { \
            /* http://d.hatena.ne.jp/s_hiiragi/20110111/1294755929 */ \
            return string_set_type{\
                /* Cf */ \
                BINDER("\u200B") \
                BINDER("\u200C") \
                BINDER("\u200D") \
                BINDER("\uFEFF") \
            \
                /* Zl */ \
                BINDER("\u2028") \
            \
                /* Zp */ \
                BINDER("\u2029") \
            \
                /* Cc */ \
                BINDER("\u0009") \
                BINDER("\u000A") \
                BINDER("\u000B") \
                BINDER("\u000C") \
                BINDER("\u000D") \
                BINDER("\u0085") \
            \
                /* Zs */ \
                BINDER("\u0020") \
                BINDER("\u00A0") \
                BINDER("\u1680") \
                BINDER("\u180E") \
                BINDER("\u2000") \
                BINDER("\u2001") \
                BINDER("\u2002") \
                BINDER("\u2003") \
                BINDER("\u2004") \
                BINDER("\u2005") \
                BINDER("\u2006") \
                BINDER("\u2007") \
                BINDER("\u2008") \
                BINDER("\u2009") \
                BINDER("\u200A") \
                BINDER("\u202F") \
                BINDER("\u205F") \
                BINDER("\u3000") \
            }; \
        } \
    };
SAYA_STRING_CONFIG_DEFINE(SAYA_DEF)
#undef SAYA_DEF

} // saya

#endif
