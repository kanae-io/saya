#ifndef SAYA_HTML_ESCAPE_HPP_
#define SAYA_HTML_ESCAPE_HPP_

#include "saya/html/detail/unescape.hpp"

namespace saya { namespace html {

template<class CharT>
inline std::basic_string<CharT>
unescape(std::basic_string<CharT> const& src, EscapeFlags flags = UNESCAPE_DEFAULT)
{
    if (src.empty()) return {};
    return detail::unescape_impl(src, flags);
}

}} // saya

#endif
