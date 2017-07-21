#ifndef SAYA_IKA_AST_FUSION_ADAPT_HPP
#define SAYA_IKA_AST_FUSION_ADAPT_HPP

#include "saya/ika/ast/detail/all.hpp"

#include <boost/fusion/include/adapt_struct.hpp>

#include <boost/preprocessor/expand.hpp>
#include <boost/preprocessor/repeat.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/repetition/for.hpp>

// ---------------------------------------------

#include "saya/ika/vm/internal_def.hpp"

#define SAYA_DEF_I(vtmapid, lit_name, unused0) \
    BOOST_FUSION_ADAPT_STRUCT( \
        saya::ika::ast::lit::lit_name, \
        (saya::ika::vm::TypeID, type_id) \
        (SAYA_IKA_VM_INTERNAL_TYPE_FOR(SAYA_IKA_VM_LIT_VTYPEMAP_FOR(vtmapid)), v) \
    )

#define SAYA_DEF(z, i, d) \
    SAYA_DEF_I BOOST_PP_SEQ_ELEM(i, d)

BOOST_PP_EXPAND(BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(SAYA_IKA_VM_LIT_TYPEMAP), SAYA_DEF, SAYA_IKA_VM_LIT_TYPEMAP))

#undef SAYA_DEF_I
#undef SAYA_DEF
#include "saya/ika/vm/internal_undef.hpp"

// ---------------------------------------------

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Namespace,
    (saya::ika::ast::NSID, id)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Var,
    (saya::ika::ast::VarID, id)
    (saya::ika::vm::Value, vv)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Func,
    (saya::ika::ast::FuncID, id)
    (saya::ika::ast::Block const*, definition)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Macro,
    (saya::ika::ast::MacroID, id)
    (saya::ika::ast::Geo const*, definition)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::BlockParam,
    (std::vector<saya::ika::ast::Argument>, args)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Block,
    (boost::optional<saya::ika::ast::BlockParam>, param)
    (std::vector<saya::ika::ast::Stmt>, stmt_list)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::CallParam,
    (std::vector<saya::ika::ast::Expr>, expr_list)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::UEntity,
    (typename saya::ika::ast::UEntity::entity_type, entity)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Expr,
    (typename saya::ika::ast::Expr::expr_type, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::SideEffectExpr,
    (typename saya::ika::ast::SideEffectExpr::expr_type, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::PrimaryExpr,
    (typename saya::ika::ast::PrimaryExpr::expr_type, expr)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Stmt,
    (saya::ika::ast::Stmt::st_content_type, st)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Declaration,
    (typename saya::ika::ast::Declaration::groupable_type, groupable)
    (std::vector<std::string>, additional_class)
    (boost::optional<saya::ika::ast::Attribute>, attr)
    (bool, is_inline_definition)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::FuncDefinition,
    (saya::ika::ast::Func*, func)
    (saya::ika::ast::Block*, block)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::MacroDefinition,
    (saya::ika::ast::Macro*, macro)
    (saya::ika::ast::Geo*, geo)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::GroupChildSpecifier,
    (saya::ika::ast::Group*, child_group)
    (std::vector<std::string>, additional_class)
    (boost::optional<saya::ika::ast::Attribute>, attr)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::GroupDefinition,
    (saya::ika::ast::Group*, group)
    (boost::optional<saya::ika::ast::GroupChildSpecifier>, child_specifier)
    (saya::ika::ast::Geo*, geo)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Attribute,
    (std::vector<saya::ika::ast::Attribute::attr_type>, attrs)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Geo,
    (saya::ika::ast::Geo::Dir, dir)
    (saya::ika::ast::Geo::Justify, justify)
    (boost::optional<saya::ika::ast::Attribute>, attr)
    (boost::optional<saya::ika::ast::Block*>, block)
)

// ---------------------------------------------------

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::DefaultSpecifier,
    (saya::ika::ast::Geo const*, geo)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Argument,
    (saya::ika::ast::Var*, var)
    (boost::optional<saya::ika::ast::Expr>, maybe_default)
)

// ---------------------------------------------------

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::UOp<saya::ika::ast::ops::FuncCall>,
    (saya::ika::ast::Func const*, func)
    (saya::ika::ast::CallParam, param)
    (boost::optional<saya::ika::ast::Block const*>, block)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::UOp<saya::ika::ast::ops::MacroCall>,
    (saya::ika::ast::Macro const*, macro)
    (std::vector<std::string>, additional_class)
    (boost::optional<saya::ika::ast::CallParam>, param)
    (boost::optional<saya::ika::ast::Geo const*>, geo)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::UOp<saya::ika::ast::ops::Not>,
    (saya::ika::ast::PrimaryExpr, term0)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::UOp<saya::ika::ast::ops::AddFamily>,
    (auto, term0)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::BOp<saya::ika::ast::ops::Pow>,
    (saya::ika::ast::UEntity, term0)
    (std::vector<saya::ika::ast::UEntity>, termN)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::BOp<saya::ika::ast::ops::MulFamily>,
    (saya::ika::ast::UEntity, term0)
    (auto, termN)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::BOp<saya::ika::ast::ops::AddFamily>,
    (saya::ika::ast::UEntity, term0)
    (auto, termN)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::BOp<saya::ika::ast::ops::Assign>,
    (typename saya::ika::ast::BOp<saya::ika::ast::ops::Assign>::lhs_type, term0)
    (typename saya::ika::ast::BOp<saya::ika::ast::ops::Assign>::rhs_type, termN)
)

// ---------------------------------------------

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Group,
    (saya::ika::ast::GroupID, id)
    (boost::optional<saya::ika::ast::Attribute>, attr)
)

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Endpoint,
    (saya::ika::ast::EndpointID, id)
    (boost::optional<saya::ika::ast::Attribute>, attr)
)

// ---------------------------------------------

BOOST_FUSION_ADAPT_STRUCT(
    saya::ika::ast::Root,
    (std::vector<saya::ika::ast::Stmt>, stmt_list)
)

#endif
