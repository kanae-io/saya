#ifndef SAYA_IKA_AST_SYNTAX_HPP
#define SAYA_IKA_AST_SYNTAX_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"
#include "saya/ika/ast/expr.hpp"
#include "saya/ika/ast/var.hpp"

#include <boost/optional.hpp>


namespace saya { namespace ika { namespace ast {

struct DefaultSpecifier : ASTEntity
{
    Geo* geo{nullptr};
};

inline std::ostream& operator<<(std::ostream& os, DefaultSpecifier const&)
{
    return debug::fixed_omitted(
        os,
        "DefaultSpecifier"
    );
}

struct Argument : ASTEntity
{
    Var* var{nullptr};
    boost::optional<Expr> maybe_default;
};

inline std::ostream& operator<<(std::ostream& os, Argument const& v)
{
    BOOST_ASSERT(v.var);

    return debug::with(
        os,
        "Arg",
        debug::kv("id", v.var->id),
        debug::cond(static_cast<bool>(v.maybe_default), [] (auto& os) -> decltype(auto) { return os << "defaulted"; }, [] (auto& os) -> decltype(auto) { return os; })
    );
}

}}} // saya

#endif
