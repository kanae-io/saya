#ifndef SAYA_ENCODING_DETAIL_UNICODE_CAST_HPP_
#define SAYA_ENCODING_DETAIL_UNICODE_CAST_HPP_

#include "saya/encoding/detail/string_cast.hpp"
#include "saya/encoding/ustring.hpp"

namespace saya { namespace detail {

inline u8string_ to_u8string(std::string const& str)
{
    if (str.empty()) return {};
    return ustring(str).to_u8();
}
inline u8string_ to_u8string(std::wstring const& wstr)
{
    if (wstr.empty()) return {};
    return ustring(wstr).to_u8();
}
inline std::string to_string(u8string_ const& u8str)
{
    if (u8str.empty()) return {};
    return ustring(u8str).to_s();
}
inline std::wstring to_wstring(u8string_ const& u8str)
{
    if (u8str.empty()) return {};
    return ustring(u8str).to_ws();
}

template<>
struct string_cast<std::string, ::saya::u8string_>
{
    static ::saya::u8string_ cast(std::string const& str) { return to_u8string(str); }
};
template<>
struct string_cast< ::saya::u8string_, std::string>
{
    static std::string cast(::saya::u8string_ const& u8str) { return to_string(u8str); }
};
template<>
struct string_cast<std::wstring, ::saya::u8string_>
{
    static ::saya::u8string_ cast(std::wstring const& wstr) { return to_u8string(wstr); }
};
template<>
struct string_cast< ::saya::u8string_, std::wstring>
{
    static std::wstring cast(::saya::u8string_ const& u8str) { return to_wstring(u8str); }
};

template<>
struct string_cast<std::string, saya::ustring>
{
    static saya::ustring cast(std::string const& str) { return saya::ustring(str); }
};
template<>
struct string_cast<std::wstring, saya::ustring>
{
    static saya::ustring cast(std::wstring const& str) { return saya::ustring(str); }
};

template<>
struct string_cast< ::saya::ustring, std::string>
{
    static std::string cast(::saya::ustring const& ustr) { return ustr.to_s(); }
};
template<>
struct string_cast< ::saya::ustring, std::wstring>
{
    static std::wstring cast(::saya::ustring const& ustr) { return ustr.to_ws(); }
};
template<>
struct string_cast< ::saya::ustring, saya::u8string_>
{
    static saya::u8string_ cast(::saya::ustring const& ustr) { return ustr.to_u8(); }
};

}} // saya

#endif

