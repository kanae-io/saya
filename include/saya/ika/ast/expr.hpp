#ifndef SAYA_AST_EXPR_HPP
#define SAYA_AST_EXPR_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>

namespace saya { namespace ika { namespace ast {

struct UEntity : ASTEntity
{
    using entity_type = boost::variant<
        boost::recursive_wrapper<UOp<ops::FuncCall>>,
        boost::recursive_wrapper<UOp<ops::Subscript>>,
        boost::recursive_wrapper<UOp<ops::AddFamily>>,
        boost::recursive_wrapper<UOp<ops::Not>>,

        boost::recursive_wrapper<PrimaryExpr>
    >;

    entity_type entity;
};

struct PrimaryExpr : ASTEntity
{
    using expr_type = boost::variant<
        kw::Inherit const*,

        Var*,

        #include "saya/ika/vm/internal_def.hpp"

        #define SAYA_DEF_I(vtmapid, lit_name, need_pointer) \
            lit::lit_name SAYA_IKA_VM_LIT_NEED_POINTER_FOR(need_pointer),

        #define SAYA_DEF(r, data, elem) \
            SAYA_DEF_I elem

        BOOST_PP_SEQ_FOR_EACH(SAYA_DEF, _, SAYA_IKA_VM_LIT_TYPEMAP)
        #undef SAYA_DEF_I
        #undef SAYA_DEF

        #include "saya/ika/vm/internal_undef.hpp"

        boost::recursive_wrapper<Expr>
    >;

    expr_type expr;
};

struct Expr : ASTEntity
{
    using expr_type = boost::variant<
        boost::recursive_wrapper<PrimaryExpr>,

        boost::recursive_wrapper<UOp<ops::FuncCall>>,
        boost::recursive_wrapper<UOp<ops::AddFamily>>,

        boost::recursive_wrapper<BOp<ops::Pow>>,
        boost::recursive_wrapper<BOp<ops::MulFamily>>,
        boost::recursive_wrapper<BOp<ops::AddFamily>>,

        boost::recursive_wrapper<BOp<ops::Assign>>
    >;

    expr_type expr;
};

inline std::ostream& operator<<(std::ostream& os, Expr const&)
{
    return debug::fixed_omitted(os, "Expr");
}

struct SideEffectExpr : ASTEntity
{
    using expr_type = boost::variant<
        boost::recursive_wrapper<UOp<ops::MacroCall>>,
        boost::recursive_wrapper<BOp<ops::Assign>>
    >;

    expr_type expr;
};

inline std::ostream& operator<<(std::ostream& os, SideEffectExpr const&)
{
    return debug::fixed_omitted(os, "Expr");
}

}}} // saya

#endif
