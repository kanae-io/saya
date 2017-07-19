#ifndef SAYA_SYNCSTREAM_FWD_HPP_
#define SAYA_SYNCSTREAM_FWD_HPP_

#include <iosfwd>

namespace saya {

template<class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>>
class basic_syncbuf;

using syncbuf = basic_syncbuf<char>;
using wsyncbuf = basic_syncbuf<wchar_t>;

template<
    class CharT, class Traits = std::char_traits<CharT>, class Allocator = std::allocator<CharT>,
    class Syncbuf = basic_syncbuf<CharT, Traits, Allocator>
>
class basic_osyncstream;

using osyncstream = basic_osyncstream<char>;
using wosyncstream = basic_osyncstream<wchar_t>;

} // saya

#endif
