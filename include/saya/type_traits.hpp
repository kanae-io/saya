#ifndef SAYA_TYPE_TRAITS_HPP_
#define SAYA_TYPE_TRAITS_HPP_

#include "boost/variant/variant_fwd.hpp"
#include <type_traits>

namespace saya {

extern void* enabler;

template<class T>
struct is_variant : std::false_type {};

template<BOOST_VARIANT_ENUM_PARAMS(class T)>
struct is_variant<boost::variant<BOOST_VARIANT_ENUM_PARAMS(T)>>
    : std::true_type {};

} // saya

#endif
