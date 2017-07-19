#ifndef SAYA_VOIDSTREAM_FWD_HPP_
#define SAYA_VOIDSTREAM_FWD_HPP_

#include <iosfwd>

namespace saya {

template<class CharT>
class basic_voidstreambuf;

using voidstreambuf = basic_voidstreambuf<char>;
using wvoidstreambuf = basic_voidstreambuf<wchar_t>;

template<class CharT>
class basic_voidstream;

using voidstream = basic_voidstream<char>;
using wvoidstream = basic_voidstream<wchar_t>;

} // saya

#endif
