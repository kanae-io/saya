#ifndef SAYA_IKA_DETAIL_IKA_ID_HPP
#define SAYA_IKA_DETAIL_IKA_ID_HPP

#include "saya/flyweight.hpp"

#include <string>

namespace saya { namespace ika { namespace detail {

template<class FactoryTag, class SelfTag>
using define_ika_id = saya::flyweights::flyweight<
    std::string,
    FactoryTag,
    SelfTag
>;

}}} // saya

#endif
