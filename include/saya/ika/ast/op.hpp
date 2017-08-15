#ifndef SAYA_IKA_AST_OP_HPP
#define SAYA_IKA_AST_OP_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

#include <boost/range/algorithm/copy.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <vector>


namespace saya { namespace ika { namespace ast {

template<>
struct UOp<ops::FuncCall> : ASTEntity
{
    Func const* func;
    CallParam param;
    boost::optional<Block const*> block;
};
inline std::ostream& operator<<(std::ostream& os, UOp<ops::FuncCall> const&)
{
    return debug::fixed_id(os, "UOp", "FuncCall");
}

template<>
struct UOp<ops::MacroCall> : ASTEntity
{
    Macro const* macro;
    std::vector<std::string> additional_class;
    boost::optional<CallParam> param;
    boost::optional<Geo const*> geo;
};
inline std::ostream& operator<<(std::ostream& os, UOp<ops::MacroCall> const&)
{
    return debug::fixed_id(os, "UOp", "MacroCall");
}

template<>
struct UOp<ops::Not> : ASTEntity
{
    PrimaryExpr term0;
};
inline std::ostream& operator<<(std::ostream& os, UOp<ops::Not> const&)
{
    return debug::fixed_id(os, "UOp", "Not");
}

template<>
struct UOp<ops::AddFamily> : ASTEntity
{
    std::pair<bool, PrimaryExpr> term0;
};
inline std::ostream& operator<<(std::ostream& os, UOp<ops::AddFamily> const& v)
{
    if (v.term0.first) {
        return debug::fixed_id(os, "UOp", "Add");
    } else {
        return debug::fixed_id(os, "UOp", "Sub");
    }
}

// ---------------------------------------------

template<>
struct BOp<ops::Pow> : ASTEntity
{
    UEntity term0;
    std::vector<UEntity> termN;
};
inline std::ostream& operator<<(std::ostream& os, BOp<ops::Pow> const&)
{
    return debug::fixed_id(os, "BOp", "Pow");
}

template<>
struct BOp<ops::MulFamily> : ASTEntity
{
    UEntity term0;
    std::vector<
        std::pair<bool, UEntity>
    > termN;
};
inline std::ostream& operator<<(std::ostream& os, BOp<ops::MulFamily> const& v)
{
    return debug::with(
        os,
        debug::id("BOp"),
        debug::kv(
            "ops",
            v.termN | boost::adaptors::transformed([] (auto const& kv) { return kv.first ? debug::id_arg("Mul") : debug::id_arg("Div"); })
        )
    );
}

template<>
struct BOp<ops::AddFamily> : ASTEntity
{
    UEntity term0;
    std::vector<
        std::pair<bool, UEntity>
    > termN;
};
inline std::ostream& operator<<(std::ostream& os, BOp<ops::AddFamily> const& v)
{
    return debug::with(
        os,
        debug::id("BOp"),
        debug::kv(
            "ops",
            v.termN | boost::adaptors::transformed([] (auto const& kv) { return kv.first ? debug::id_arg("Add") : debug::id_arg("Sub"); })
        )
    );
}

template<>
struct BOp<ops::Assign> : ASTEntity
{
    using lhs_type = Var*;
    using rhs_type = UEntity;

    lhs_type term0;
    rhs_type termN;
};
inline std::ostream& operator<<(std::ostream& os, BOp<ops::Assign> const&)
{
    return debug::fixed_id(os, "BOp", "Assign");
}

}}} // saya

#endif
