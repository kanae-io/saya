#ifndef SAYA_ZED_CORE_HPP
#define SAYA_ZED_CORE_HPP

#include <boost/phoenix/core/value.hpp>

namespace saya { namespace zed {

template<class T>
inline auto lazy_nullptr()
{
    return boost::phoenix::val(static_cast<T>(nullptr));
}

}} // saya

#endif
