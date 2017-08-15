#ifndef SAYA_IKA_VM_TYPE_HPP
#define SAYA_IKA_VM_TYPE_HPP

#include "saya/ika/detail/ika_id.hpp"

#include <boost/phoenix/function.hpp>

#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/seq.hpp>
#include <boost/preprocessor/seq/for_each.hpp>

#include <iostream>
#include <cstdint>
#include <string>


namespace saya { namespace ika { namespace vm {

namespace factory_tag {
struct VM {};
} // factory_tag

using TypeID = ika::detail::define_ika_id<factory_tag::VM, factory_tag::VM>;

// please use this class as a pointer to nullptr
struct UndecidedType final
{
    ~UndecidedType() = delete;
};

// okay, so here goes the helper for you
constexpr inline UndecidedType const*
make_undecided() noexcept
{
    return static_cast<UndecidedType const*>(nullptr);
}

#if 0
// ...and the lazy version for you
namespace detail {
struct make_undecided_lazy_impl
{
    using result_type = UndecidedType const*;

    inline constexpr result_type operator()() const noexcept
    {
        return make_undecided();
    }
};
} // detail

constexpr inline auto
make_undecided_lazy() noexcept
{
    return boost::phoenix::function<detail::make_undecided_lazy_impl>()();
}
#endif

inline std::ostream& operator<<(std::ostream& os, UndecidedType const*)
{
    return os << "(undecided_type)";
}


#include "saya/ika/vm/internal_def.hpp"

namespace type_tag {
#define SAYA_DEF(r, _, elem) \
    struct elem {};

BOOST_PP_SEQ_FOR_EACH(SAYA_DEF, _, SAYA_IKA_VM_INTERNAL_TYPEMAP_NAMEONLY)
#undef SAYA_DEF
} // type_tag

namespace types {

inline TypeID Undecided() noexcept { return {}; }

#define SAYA_DEF(r, _, elem) \
    inline TypeID dispatch(type_tag::elem) \
    { \
        return TypeID{BOOST_PP_STRINGIZE(elem)}; \
    } \
    inline TypeID elem() \
    { \
        return TypeID{BOOST_PP_STRINGIZE(elem)}; \
    }

BOOST_PP_SEQ_FOR_EACH(SAYA_DEF, _, BOOST_PP_SEQ_TAIL(SAYA_IKA_VM_INTERNAL_TYPEMAP_NAMEONLY))
#undef SAYA_DEF
} // types

#include "saya/ika/vm/internal_undef.hpp"

}}} // saya

#endif
