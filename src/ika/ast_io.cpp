#include "saya/ika/ast/all.hpp"
#include "saya/ika/ast/debug.hpp"

#include <boost/range/adaptor/transformed.hpp>
#include <boost/variant.hpp>
#include <boost/format.hpp>
#include <boost/assert.hpp>

#include <iostream>
#include <vector>


namespace saya { namespace ika { namespace ast { namespace io {

std::ostream& operator<<(std::ostream& os, AdditionalClass const& v)
{
    if (v.classes.empty()) {
        return debug::fixed_empty(os, "AdditionalClass");
    }

    return debug::with(
        os,
        "AdditionalClass",
        debug::kv("classes", v.classes | boost::adaptors::transformed([] (auto const& c) { return debug::id_arg("." + c); }))
    );
}

std::ostream& operator<<(std::ostream& os, std::vector<Attribute::attr_type> const&)
{
    return debug::fixed_omitted(os, "Attribute::attrs");
}

std::ostream& operator<<(std::ostream& os, Attribute const& v)
{
    BOOST_ASSERT(!v.attrs.empty());

    return debug::with(
        os,
        "Attr",
        debug::kv("attrs", v.attrs)
    );
}

std::ostream& operator<<(std::ostream& os, boost::optional<Attribute> const& v)
{
    return debug::proxy(
        os,
        "Attr",
        v
    );
}

std::ostream& operator<<(std::ostream& os, BlockParam const& v)
{
    BOOST_ASSERT(!v.args.empty());

    return debug::with(
        os,
        "Param",
        debug::kv("args", v.args)
    );
}

std::ostream& operator<<(std::ostream& os, boost::optional<BlockParam> const& v)
{
    return debug::proxy(
        os,
        "Param",
        v
    );
}

std::ostream& operator<<(std::ostream& os, Block const& v)
{
    return debug::with(
        os,
        "Block",
        debug::kv("param", v.param),
        debug::kv("sloc", v.stmt_list.size())
    );
}

std::ostream& operator<<(std::ostream& os, boost::optional<Block*> const& v)
{
    return debug::proxy(
        os,
        "Block",
        v
    );
}

std::ostream& operator<<(std::ostream& os, boost::optional<Block> const& v)
{
    return debug::proxy(
        os,
        "Block",
        v
    );
}

std::ostream& operator<<(std::ostream& os, Block const* v)
{
    return debug::proxy(
        os,
        "Block",
        v
    );
}

std::ostream& operator<<(std::ostream& os, Func const& v)
{
    return debug::with(
        os,
        "Func",
        debug::kv("id", v.id),
        debug::kv("def", v.definition)
    );
}

std::ostream& operator<<(std::ostream& os, Func const* v)
{
    return debug::proxy(
        os,
        "Func",
        v
    );
}

std::ostream& operator<<(std::ostream& os, Macro const& v)
{
    return debug::with(
        os,
        "Macro",
        debug::kv("id", v.id),
        debug::kv("def", v.definition)
    );
}

std::ostream& operator<<(std::ostream& os, Macro const* v)
{
    return debug::proxy(
        os,
        "Macro",
        v
    );
}

std::ostream& operator<<(std::ostream& os, CallParam const& v)
{
    return debug::with(
        os,
        "Param",
        debug::kv("argn", v.expr_list.size())
    );
}

std::ostream& operator<<(std::ostream& os, Declaration const& v)
{
    return debug::with(
        os,
        "Decl",
        debug::kv("groupable", v.groupable),
        debug::kv("attr", v.attr),
        debug::cond(v.is_inline, [] (auto& os) -> decltype(auto) { return os << "inline"; }, [] (auto& os) -> decltype(auto) { return os; })
    );
}


std::ostream& operator<<(std::ostream& os, FuncDefinition const& v)
{
    BOOST_ASSERT(v.func);

    return debug::with(
        os,
        "FuncDef",
        debug::kv("id", v.func->id)
    );
}

std::ostream& operator<<(std::ostream& os, MacroDefinition const& v)
{
    BOOST_ASSERT(v.macro);

    return debug::with(
        os,
        "MacroDef",
        debug::kv("id", v.macro->id)
    );
}

std::ostream& operator<<(std::ostream& os, GroupChildSpecifier const& v)
{
    BOOST_ASSERT(v.child);

    return debug::with(
        os,
        "ChildSpecifier",
        debug::kv("child", *v.child),
        debug::kv("attr", v.attr)
    );
}

std::ostream& operator<<(std::ostream& os, GroupDefinition const& v)
{
    BOOST_ASSERT(v.group);

    return debug::with(
        os,
        "GroupDef",
        debug::kv("id", v.group->id),
        debug::kv("c", v.child_specifier)
    );
}

std::ostream& operator<<(std::ostream& os, PrimaryExpr const&)
{
    return debug::fixed_omitted(os, "PrimaryExpr");
}

std::ostream& operator<<(std::ostream& os, Expr const&)
{
    return debug::fixed_omitted(os, "Expr");
}

std::ostream& operator<<(std::ostream& os, SideEffectExpr const&)
{
    return debug::fixed_omitted(os, "SideEffectExpr");
}

std::ostream& operator<<(std::ostream& os, Geo::Dir d)
{
    switch (d) {
    case Geo::Dir::HOR:
        return os << "HOR";

    case Geo::Dir::HOR_REV:
        return os << "HOR_REV";

    case Geo::Dir::VER:
        return os << "VER";

    case Geo::Dir::VER_REV:
        return os << "VER_REV";

    default:
        throw std::logic_error("not implemented");
    }
}

std::ostream& operator<<(std::ostream& os, Geo::Justify j)
{
    switch (j) {
    case Geo::Justify::EXPAND:
        return os << "EXPAND";

    case Geo::Justify::START:
        return os << "START";

    case Geo::Justify::END:
        return os << "END";

    case Geo::Justify::CENTER:
        return os << "CENTER";

    default:
        throw std::logic_error("not implemented");
    }
}

std::ostream& operator<<(std::ostream& os, Geo const& v)
{
    return debug::with(
        os,
        "Geo",
        debug::kv("dir", debug::literal(v.dir)),
        debug::kv("justify", debug::literal(v.justify)),
        debug::kv("attr", v.attr),
        debug::kv("block", v.block)
    );
}

std::ostream& operator<<(std::ostream& os, Geo const* v)
{
    return debug::proxy(
        os,
        "Geo",
        v
    );
}


std::ostream& operator<<(std::ostream& os, Group const& v)
{
    return debug::with(
        os,
        "Group",
        debug::kv("id", v.id),
        debug::kv("additional_class", v.additional_class),
        debug::kv("attr", v.attr),
        debug::cond(
            "definition",
            static_cast<bool>(v.definition),
            [] (auto& os) -> decltype(auto) {
                return os << "yes";
            },
            [] (auto& os) -> decltype(auto) {
                    return os << saya::console::color::BOLD() << saya::console::color::fg::RED() << "NONE(delayed/inlined?)" << saya::console::color::RESET();
            }
        )
    );
}

std::ostream& operator<<(std::ostream& os, Group const* v)
{
    return debug::proxy(
        os,
        "Group",
        v
    );
}

std::ostream& operator<<(std::ostream& os, Endpoint const& v)
{
    return debug::with(
        os,
        "Endpoint",
        debug::kv("id", v.id),
        debug::kv("additional_class", v.additional_class),
        debug::kv("attr", v.attr)
    );
}

std::ostream& operator<<(std::ostream& os, Endpoint const* v)
{
    return debug::proxy(
        os,
        "Endpoint",
        v
    );
}

std::ostream&
operator<<(std::ostream& os, NSID const& v)
{
    return os << debug::id_arg(*v.get());
}
std::ostream&
operator<<(std::ostream& os, VarID const& v)
{
    return os << debug::id_arg("%" + *v.get());
}
std::ostream&
operator<<(std::ostream& os, FuncID const& v)
{
    return os << debug::id_arg("@" + *v.get());
}
std::ostream&
operator<<(std::ostream& os, MacroID const& v)
{
    return os << debug::id_arg("&" + *v.get());
}

std::ostream&
operator<<(std::ostream& os, GroupID const& v)
{
    return os << debug::id_arg(/*"." +*/ *v.get());
}
std::ostream&
operator<<(std::ostream& os, EndpointID const& v)
{
    return os << debug::id_arg(/*"#" +*/ *v.get());
}


std::ostream& operator<<(std::ostream& os, kw::Inherit const*)
{
    return debug::name(os, "Inherit");
}

std::ostream& operator<<(std::ostream& os, kw::Eq const*)
{
    return debug::name(os, "Eq");
}


std::ostream& operator<<(std::ostream& os, lit::Map::value_type const& kv)
{
    return os << debug::literal(kv.first) << ": " << kv.second;
}

std::ostream& operator<<(std::ostream& os, lit::Map::internal_type const& v)
{
    return debug::with(
        os,
        "Map(internal)",
        debug::kv("_", v)
    );
}

std::ostream& operator<<(std::ostream& os, lit::Map const& v)
{
    return debug::with(
        os,
        "Map",
        debug::kv("map", v.v)
    );
}

std::ostream& operator<<(std::ostream& os, Namespace const& v)
{
    return debug::with(
        os,
        "Namespace",
        debug::kv("id", v.id),
        debug::kv("stmt_list", v.stmt_list)
    );
}

std::ostream& operator<<(std::ostream& os, UOp<ops::FuncCall> const&)
{
    return debug::fixed_id(os, "UOp", "FuncCall");
}
std::ostream& operator<<(std::ostream& os, UOp<ops::MacroCall> const&)
{
    return debug::fixed_id(os, "UOp", "MacroCall");
}
std::ostream& operator<<(std::ostream& os, UOp<ops::Not> const&)
{
    return debug::fixed_id(os, "UOp", "Not");
}
std::ostream& operator<<(std::ostream& os, UOp<ops::AddFamily> const& v)
{
    if (v.term0.first) {
        return debug::fixed_id(os, "UOp", "Add");
    } else {
        return debug::fixed_id(os, "UOp", "Sub");
    }
}

std::ostream& operator<<(std::ostream& os, UOp<ops::Subscript> const&)
{
    return debug::fixed_id(os, "UOp", "Subscript");
}
std::ostream& operator<<(std::ostream& os, BOp<ops::Pow> const&)
{
    return debug::fixed_id(os, "BOp", "Pow");
}
std::ostream& operator<<(std::ostream& os, BOp<ops::MulFamily> const& v)
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
std::ostream& operator<<(std::ostream& os, BOp<ops::AddFamily> const& v)
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
std::ostream& operator<<(std::ostream& os, BOp<ops::Assign> const&)
{
    return debug::fixed_id(os, "BOp", "Assign");
}


std::ostream& operator<<(std::ostream& os, Root const& v)
{
    return debug::with(
        os,
        "Root",
        debug::kv("stmt_list", v.stmt_list)
    );
}


std::ostream& operator<<(std::ostream& os, Stmt const& v)
{
    return debug::with(
        os,
        "Stmt",
        debug::kv("st", v.st)
    );
}


std::ostream& operator<<(std::ostream& os, DefaultSpecifier const&)
{
    return debug::fixed_omitted(
        os,
        "DefaultSpecifier"
    );
}
std::ostream& operator<<(std::ostream& os, Argument const& v)
{
    BOOST_ASSERT(v.var);

    return debug::with(
        os,
        "Arg",
        debug::kv("id", v.var->id),
        debug::cond(static_cast<bool>(v.maybe_default), [] (auto& os) -> decltype(auto) { return os << "defaulted"; }, [] (auto& os) -> decltype(auto) { return os; })
    );
}


std::ostream& operator<<(std::ostream& os, Var const& v)
{
    return debug::with(
        os,
        "Var",
        debug::kv("id", v.id)
    );
}

std::ostream& operator<<(std::ostream& os, Var const* v)
{
    return debug::proxy(
        os,
        "Var",
        v
    );
}


#include "saya/ika/vm/internal_def.hpp"

#define SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_String(lit_name) \
    return debug::with( \
        os, \
        BOOST_PP_STRINGIZE(lit_name), \
        debug::literal(v.v) \
    );

#define SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_Symbol(lit_name) \
    return debug::with( \
        os, \
        BOOST_PP_STRINGIZE(lit_name), \
        debug::literal(":" + v.v) \
    );

#define SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_Int64(lit_name) SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_String(lit_name)
#define SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_UInt64(lit_name) SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_String(lit_name)
#define SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_Real(lit_name) SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_String(lit_name)

#define SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_Color(lit_name) \
    return debug::with( \
        os, \
        BOOST_PP_STRINGIZE(lit_name), \
        debug::literal(boost::str(boost::format("#%08X") % v.v)) \
    );

#define SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_Px(lit_name) \
    return debug::with( \
        os, \
        BOOST_PP_STRINGIZE(lit_name), \
        debug::literal(boost::str(boost::format("%upx") % v.v)) \
    );

#define SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_Pct(lit_name) \
    return debug::with( \
        os, \
        BOOST_PP_STRINGIZE(lit_name), \
        debug::literal(boost::str(boost::format("%.2f%%") % v.v)) \
    );

#define SAYA_IKA_DEFINE_LIT_OSTREAM_FOR(lit_name) \
    std::ostream& operator<<(std::ostream& os, lit::lit_name const& v) \
    { \
        SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_ ## lit_name (lit_name) \
    }

#define SAYA_DEF_I(lit_name) \
    SAYA_IKA_DEFINE_LIT_OSTREAM_FOR(lit_name)

#define SAYA_DEF(z, i, d) \
    SAYA_DEF_I(BOOST_PP_SEQ_ELEM(i, d))

BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(SAYA_IKA_VM_LIT_TYPEMAP_NAMEONLY), SAYA_DEF, SAYA_IKA_VM_LIT_TYPEMAP_NAMEONLY)


#undef SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_String
#undef SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_Symbol
#undef SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_Int64
#undef SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_UInt64
#undef SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_Real
#undef SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_Color
#undef SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_Px
#undef SAYA_IKA_DEFINE_LIT_OSTREAM_FOR_Pct
#undef SAYA_IKA_DEFINE_LIT_OSTREAM_FOR
#undef SAYA_DEF_I
#undef SAYA_DEF

#include "saya/ika/vm/internal_undef.hpp"

}}}} // saya

