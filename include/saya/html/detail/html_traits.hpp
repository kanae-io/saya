#ifndef SAYA_HTML_DETAIL_HTML_TRAITS_HPP_
#define SAYA_HTML_DETAIL_HTML_TRAITS_HPP_

#include <string>
#include <unordered_map>

namespace saya { namespace html { namespace detail {

template<class CharT>
struct html_traits;

template<>
struct html_traits<char>
{
    typedef char char_type;
    typedef std::basic_string<char_type> string_type;
    typedef std::unordered_map<string_type, string_type> named_character_references_table_type;
};

template<>
struct html_traits<wchar_t>
{
    typedef wchar_t char_type;
    typedef std::basic_string<char_type> string_type;
    typedef std::unordered_map<string_type, string_type> named_character_references_table_type;
};

}}} // saya

#endif
