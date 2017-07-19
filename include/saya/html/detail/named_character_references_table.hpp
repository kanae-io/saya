#ifndef SAYA_HTML_DETAIL_NAMED_CHARACTER_REFERENCES_TABLE_HPP_
#define SAYA_HTML_DETAIL_NAMED_CHARACTER_REFERENCES_TABLE_HPP_

#include "saya/html/detail/html_traits.hpp"

namespace saya { namespace html { namespace detail {

extern html_traits<char>::named_character_references_table_type const
NAMED_CHARACTER_REFERENCES_TABLE;

extern html_traits<wchar_t>::named_character_references_table_type const
NAMED_CHARACTER_REFERENCES_WIDE_TABLE;


template<class CharT>
struct named_character_references_traits;

template<>
struct named_character_references_traits<char>
{
    static html_traits<char>::named_character_references_table_type const&
    table() { return NAMED_CHARACTER_REFERENCES_TABLE; }
};

template<>
struct named_character_references_traits<wchar_t>
{
    static html_traits<wchar_t>::named_character_references_table_type const&
    table() { return NAMED_CHARACTER_REFERENCES_WIDE_TABLE; }
};

}}} // saya

#endif
