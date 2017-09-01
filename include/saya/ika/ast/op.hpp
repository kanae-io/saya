#ifndef SAYA_IKA_AST_OP_HPP
#define SAYA_IKA_AST_OP_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

#include <vector>


namespace saya { namespace ika { namespace ast {

template<>
struct UOp<ops::FuncCall> : ASTEntity
{
    Func* func{nullptr};
    CallParam param;
    boost::optional<Block*> block;
};

template<>
struct UOp<ops::MacroCall> : ASTEntity
{
    Macro* macro{nullptr};
    boost::optional<AdditionalClass*> additional_class;
    boost::optional<CallParam> param;
    boost::optional<Geo*> geo;
};

template<>
struct UOp<ops::Not> : ASTEntity
{
    PrimaryExpr term0;
};

template<>
struct UOp<ops::AddFamily> : ASTEntity
{
    std::pair<bool, PrimaryExpr> term0;
};
template<>
struct UOp<ops::Subscript> : ASTEntity
{
    Var* var{nullptr};
    lit::Symbol* key{nullptr};
};

// ---------------------------------------------

template<>
struct BOp<ops::Pow> : ASTEntity
{
    UEntity term0;
    std::vector<UEntity> termN;
};

template<>
struct BOp<ops::MulFamily> : ASTEntity
{
    UEntity term0;
    std::vector<
    std::pair<bool, UEntity>
    > termN;
};

template<>
struct BOp<ops::AddFamily> : ASTEntity
{
    UEntity term0;
    std::vector<
    std::pair<bool, UEntity>
    > termN;
};

template<>
struct BOp<ops::Assign> : ASTEntity
{
    using lhs_type = boost::variant<Var*, UOp<ops::Subscript>>;
    using rhs_type = UEntity;

    lhs_type term0;
    rhs_type termN;
};

}}} // saya

#endif
