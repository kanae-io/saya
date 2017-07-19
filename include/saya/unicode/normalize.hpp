#ifndef SAYA_UNICODE_NORMALIZE_HPP_
#define SAYA_UNICODE_NORMALIZE_HPP_

#include "saya/encoding/ustring.hpp"

#include <unicode/unorm2.h>
#include <unicode/normalizer2.h>

namespace saya { namespace unicode {

struct nfc {};
struct nfkc {};
struct nfd {};
struct nfkd {};

template<class NormalizeType>
struct normalize_traits;

template<>
struct normalize_traits<unicode::nfc>
{
    static char const* nf() { return "nfc"; }
    static UNormalization2Mode mode() { return UNORM2_COMPOSE; }
};
template<>
struct normalize_traits<unicode::nfkc>
{
    static char const* nf() { return "nfkc"; }
    static UNormalization2Mode mode() { return UNORM2_COMPOSE; }
};
template<>
struct normalize_traits<unicode::nfd>
{
    static char const* nf() { return "nfc"; }
    static UNormalization2Mode mode() { return UNORM2_DECOMPOSE; }
};
template<>
struct normalize_traits<unicode::nfkd>
{
    static char const* nf() { return "nfkc"; }
    static UNormalization2Mode mode() { return UNORM2_DECOMPOSE; }
};

template<class Normalization>
inline saya::ustring normalize(saya::ustring const& src)
{
    typedef normalize_traits<Normalization> traits_type;

    UErrorCode err = U_ZERO_ERROR;
    auto const* norm = icu::Normalizer2::getInstance(nullptr, traits_type::nf(), traits_type::mode(), err);
    saya::check_icu_failure(err);

    auto const normalized(norm->normalize(src.istr(), err));
    saya::check_icu_failure(err);

    return saya::ustring(normalized);
}

}} // saya

#endif
