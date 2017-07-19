#ifndef SAYA_STREAM_OBJECT_HPP_
#define SAYA_STREAM_OBJECT_HPP_

#include <iostream>

namespace saya {

template<class CharT>
struct stream_object;

template<>
struct stream_object<char>
{
    typedef std::basic_ostream<char> ostream_type;
    constexpr inline static ostream_type& cout() noexcept { return ::std::cout; }
    constexpr inline static ostream_type& cerr() noexcept { return ::std::cerr; }
    constexpr inline static ostream_type& clog() noexcept { return ::std::clog; }
};
template<>
struct stream_object<wchar_t>
{
    typedef std::basic_ostream<wchar_t> ostream_type;
    constexpr inline static ostream_type& cout() noexcept { return ::std::wcout; }
    constexpr inline static ostream_type& cerr() noexcept { return ::std::wcerr; }
    constexpr inline static ostream_type& clog() noexcept { return ::std::wclog; }
};

} // saya

#endif
