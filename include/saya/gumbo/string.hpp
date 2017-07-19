#ifndef SAYA_GUMBO_STRING_HPP_
#define SAYA_GUMBO_STRING_HPP_

#include "saya/encoding/ustring.hpp"
#include "saya/gumbo/common.hpp"

namespace saya {

template<>
inline u8string_ make_u8string(GumboStringPiece const& gstr)
{
    if (!gstr.data || gstr.length == 0) return {};
    return u8string_{gstr.data, gstr.length};
}

template<>
inline ustring make_ustring(GumboStringPiece const& gstr)
{
    if (!gstr.data || gstr.length == 0) return {};
    return ustring{make_u8string(gstr)};
}

} // saya


#endif
