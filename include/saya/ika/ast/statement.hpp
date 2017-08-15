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
        boost::recursive_wrapper<Declaration>,

        boost::recursive_wrapper<Geo*>,

        boost::recursive_wrapper<DefaultSpecifier>,

        boost::recursive_wrapper<GroupDefinition>,
        boost::recursive_wrapper<FuncDefinition>,
        boost::recursive_wrapper<MacroDefinition>,

        Expr
    >;

    st_content_type st;
};

inline std::ostream& operator<<(std::ostream& os, Stmt const& v)
{
    return debug::with(
        os,
        "Stmt",
        debug::kv("st", v.st)
    );
}

}}} // saya

#endif
