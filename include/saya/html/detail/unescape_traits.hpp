#ifndef SAYA_HTML_DETAIL_UNESCAPE_TRAITS_HPP_
#define SAYA_HTML_DETAIL_UNESCAPE_TRAITS_HPP_

#include "saya/string_config.hpp"
#include <boost/regex.hpp>

namespace saya { namespace html { namespace detail {

template<class CharT>
struct unescape_traits;

#define DEF(CHAR_TYPE, BINDER) \
    template<> \
    struct unescape_traits<CHAR_TYPE> \
    { \
        typedef CHAR_TYPE char_type; \
        typedef std::basic_string<char_type> string_type; \
        typedef boost::basic_regex<char_type> regex_type; \
    \
        static string_type hexadecimal_prefix() \
        { \
            return BINDER("0x"); \
        } \
        static regex_type const& character_reference_rgx() \
        { \
            static regex_type const rgx{BINDER("&(?:(#[xX])([[:xdigit:]]+);|(#)([0-9]+);|[a-zA-Z]+;?)")}; \
            return rgx; \
        } \
    };

SAYA_STRING_CONFIG_DEFINE(DEF)
#undef DEF

}}} // saya

#endif
