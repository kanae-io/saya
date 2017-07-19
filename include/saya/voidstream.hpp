#ifndef SAYA_VOIDSTREAM_HPP_
#define SAYA_VOIDSTREAM_HPP_

#include "saya/voidstream_fwd.hpp"

#include <streambuf>
#include <ostream>

namespace saya {

template<class CharT>
class basic_voidstreambuf final : public std::basic_streambuf<CharT>
{
public:
    basic_voidstreambuf() = default;
    explicit basic_voidstreambuf(basic_voidstreambuf const&) = delete;
    basic_voidstreambuf& operator=(basic_voidstreambuf const&) = delete;
};

template<class CharT>
class basic_voidstream final
    : basic_voidstreambuf<CharT>
    , public std::basic_ostream<CharT>
{
public:
    basic_voidstream() = default;
    explicit basic_voidstream(basic_voidstream const&) = delete;
    basic_voidstream& operator=(basic_voidstream const&) = delete;
};

} // saya

#endif
