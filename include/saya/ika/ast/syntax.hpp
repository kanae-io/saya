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
    Geo* definition{nullptr};
};

struct Argument : ASTEntity
{
    Var* var{nullptr};
    boost::optional<Expr> maybe_default;
};


}}} // saya

#endif
