#ifndef SAYA_ZED_TERMINAL_HPP
#define SAYA_ZED_TERMINAL_HPP

#include "saya/zed/pct.hpp"

#include <boost/multiprecision/cpp_dec_float.hpp>
#include <boost/spirit/include/qi_real.hpp> // real_policies<T>
#include <boost/spirit/include/qi_numeric.hpp>

#include <cstdint>


#if !defined(SAYA_IKA_NO_QI_MODS)
namespace boost { namespace spirit { namespace traits {

template<>
inline bool
is_equal_to_one<
    ::boost::multiprecision::cpp_dec_float_50
>
(::boost::multiprecision::cpp_dec_float_50 const& value) {
    return value == 1;
}

}}} // boost
#endif


// internal linkage...

namespace saya { namespace zed { namespace terminals {

namespace detail {

template<class T>
struct strict_real_policies : boost::spirit::qi::real_policies<T>
{
    static constexpr bool expect_dot = true;
};

} // detail


boost::spirit::qi::real_parser<
    boost::multiprecision::cpp_dec_float_50,
    detail::strict_real_policies<boost::multiprecision::cpp_dec_float_50>
> const real_;

boost::spirit::qi::uint_parser<
    std::uint64_t, 10, 1, -1
> const uint64_;

boost::spirit::qi::int_parser<
    std::int64_t, 10, 1, -1
> const int64_;

boost::spirit::qi::uint_parser<
    zed::upct, 10, 1, 3
> const pct_;


template<class T, unsigned ExactDigits>
using hex_of_t = boost::spirit::qi::uint_parser<
    T, 16, ExactDigits, ExactDigits
>;

}}} // saya

#endif
