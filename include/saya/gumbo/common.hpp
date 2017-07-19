#ifndef SAYA_GUMBO_COMMON_HPP_
#define SAYA_GUMBO_COMMON_HPP_

#include <gumbo.h>
#include <functional>

namespace saya { namespace gumbo {

enum SanitizeType
{
    SANITIZE_NONE,
    SANITIZE_ENCODING,
    SANITIZE_SYNTACTIC,
    SANITIZE_VERBATIM,
    SANITIZE_API,
};

}} // saya

namespace std {

template<>
struct hash<saya::gumbo::SanitizeType>
{
    std::size_t operator()(saya::gumbo::SanitizeType v) const
    {
        return static_cast<int>(v);
    }
};

} // std


#endif
