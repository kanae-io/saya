#ifndef SAYA_REGEX_HPP_
#define SAYA_REGEX_HPP_

#include "saya/string_config.hpp"

#include <boost/regex.hpp>
#include <string>

namespace saya {

template<class CharT>
struct regex_traits;

#define SAYA_DEF(CHAR_TYPE, BINDER) \
    template<> \
    struct regex_traits<CHAR_TYPE> \
    { \
        typedef CHAR_TYPE char_type; \
        typedef char_type const* blob_type; \
        typedef std::basic_string<char_type> string_type; \
        typedef boost::basic_regex<char_type, boost::regex_traits<char_type>> regex_type; \
    \
        static blob_type nullstr() { return BINDER(""); } \
    \
        static blob_type begin() { return BINDER("^"); } \
        static blob_type end() { return BINDER("$"); } \
    \
        static blob_type kleene() { return BINDER("*"); } \
        static string_type kleene(string_type const& s) { return s + kleene(); } \
    \
        static blob_type plus() { return BINDER("+"); } \
        static string_type plus(string_type const& s) { return s + plus(); } \
    \
        static blob_type brace_begin() { return BINDER("["); } \
        static blob_type brace_not_begin() { return BINDER("[^"); } \
        static blob_type brace_end() { return BINDER("]"); } \
    \
        static string_type line(string_type const& s) { return begin() + s + end(); } \
        static string_type brace(string_type const& s) { return brace_begin() + s + brace_end(); } \
        static string_type brace_not(string_type const& s) { return brace_not_begin() + s + brace_end(); } \
    };

SAYA_STRING_CONFIG_DEFINE(SAYA_DEF)
#undef SAYA_DEF

} // saya
#endif
