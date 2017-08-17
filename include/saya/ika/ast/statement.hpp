#ifndef SAYA_IKA_AST_STATEMENT_HPP
#define SAYA_IKA_AST_STATEMENT_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

namespace saya { namespace ika { namespace ast {

struct Stmt : ASTEntity
{
    using st_content_type = boost::variant<
        boost::recursive_wrapper<Namespace*>,

        boost::recursive_wrapper<Declaration>,

        boost::recursive_wrapper<Geo*>,

        boost::recursive_wrapper<DefaultSpecifier>,

        boost::recursive_wrapper<GroupDefinition>,
        boost::recursive_wrapper<FuncDefinition>,
        boost::recursive_wrapper<MacroDefinition>,

        SideEffectExpr,
        Expr
    >;

    st_content_type st;
};

}}} // saya

#endif
