#ifndef SAYA_IKA_AST_ID_HPP
#define SAYA_IKA_AST_ID_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/debug.hpp"
#include "saya/ika/detail/ika_id.hpp"

namespace saya { namespace ika { namespace ast {

namespace factory_tag {

struct AST {};

} // factory_tag

using NSID = ika::detail::define_ika_id<factory_tag::AST, Namespace>;
using VarID = ika::detail::define_ika_id<factory_tag::AST, Var>;
using FuncID = ika::detail::define_ika_id<factory_tag::AST, Func>;
using MacroID = ika::detail::define_ika_id<factory_tag::AST, Macro>;

using GroupID = ika::detail::define_ika_id<factory_tag::AST, Group>;
using EndpointID = ika::detail::define_ika_id<factory_tag::AST, Endpoint>;


inline std::ostream&
operator<<(std::ostream& os, NSID const& v)
{
    return os << debug::id_arg(*v.get());
}
inline std::ostream&
operator<<(std::ostream& os, VarID const& v)
{
    return os << debug::id_arg(*v.get());
}
inline std::ostream&
operator<<(std::ostream& os, FuncID const& v)
{
    return os << debug::id_arg(*v.get());
}
inline std::ostream&
operator<<(std::ostream& os, MacroID const& v)
{
    return os << debug::id_arg(*v.get());
}

inline std::ostream&
operator<<(std::ostream& os, GroupID const& v)
{
    return os << debug::id_arg(*v.get());
}
inline std::ostream&
operator<<(std::ostream& os, EndpointID const& v)
{
    return os << debug::id_arg(*v.get());
}

}}} // saya

#endif
