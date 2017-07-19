#ifndef SAYA_HTML_HTML_STRING_HPP_
#define SAYA_HTML_HTML_STRING_HPP_

#include "saya/unsafe_string.hpp"
#include "saya/html/escape.hpp"

namespace saya {

template<class CharT>
class basic_html_string
    : public saya::basic_unsafe_string<
          basic_html_string<CharT>,
          CharT
      >
{
public:
    typedef saya::basic_unsafe_string<
        basic_html_string<CharT>,
        CharT
    > base_type;
    using typename base_type::internal_type;

private:
    static internal_type normalize(internal_type const& src)
    {
        return saya::html::unescape(src);
    }
};
typedef basic_html_string<char> html_string;
typedef basic_html_string<wchar_t> html_wstring;


} // saya

#endif
