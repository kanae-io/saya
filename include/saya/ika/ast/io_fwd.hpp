#ifndef SAYA_IKA_AST_IO_FWD_HPP
#define SAYA_IKA_AST_IO_FWD_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/id.hpp"

#include "saya/type_traits.hpp"

#include <boost/variant/static_visitor.hpp>

#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/repeat.hpp>

#include <iosfwd>


namespace saya { namespace ika { namespace ast { namespace io {

std::ostream& operator<<(std::ostream& os, AdditionalClass const& v);
std::ostream& operator<<(std::ostream& os, Attribute const& v);
std::ostream& operator<<(std::ostream& os, boost::optional<Attribute> const& v);
std::ostream& operator<<(std::ostream& os, BlockParam const& v);
std::ostream& operator<<(std::ostream& os, boost::optional<BlockParam> const& v);
std::ostream& operator<<(std::ostream& os, Block const& v);
std::ostream& operator<<(std::ostream& os, boost::optional<Block*> const& v);
std::ostream& operator<<(std::ostream& os, boost::optional<Block> const& v);
std::ostream& operator<<(std::ostream& os, Block const* v);
std::ostream& operator<<(std::ostream& os, Func const& v);
std::ostream& operator<<(std::ostream& os, Func const* v);
std::ostream& operator<<(std::ostream& os, Macro const& v);
std::ostream& operator<<(std::ostream& os, Macro const* v);
std::ostream& operator<<(std::ostream& os, CallParam const& v);
std::ostream& operator<<(std::ostream& os, Declaration const& v);
std::ostream& operator<<(std::ostream& os, FuncDefinition const& v);
std::ostream& operator<<(std::ostream& os, MacroDefinition const& v);
std::ostream& operator<<(std::ostream& os, GroupChildSpecifier const& v);
std::ostream& operator<<(std::ostream& os, GroupDefinition const& v);
std::ostream& operator<<(std::ostream& os, PrimaryExpr const&);
std::ostream& operator<<(std::ostream& os, Expr const&);
std::ostream& operator<<(std::ostream& os, SideEffectExpr const&);
std::ostream& operator<<(std::ostream& os, Geo const& v);
std::ostream& operator<<(std::ostream& os, Geo const* v);
std::ostream& operator<<(std::ostream& os, Group const& v);
std::ostream& operator<<(std::ostream& os, Group const* v);
std::ostream& operator<<(std::ostream& os, Endpoint const& v);
std::ostream& operator<<(std::ostream& os, Endpoint const* v);
std::ostream& operator<<(std::ostream& os, NSID const& v);
std::ostream& operator<<(std::ostream& os, VarID const& v);
std::ostream& operator<<(std::ostream& os, FuncID const& v);
std::ostream& operator<<(std::ostream& os, MacroID const& v);
std::ostream& operator<<(std::ostream& os, GroupID const& v);
std::ostream& operator<<(std::ostream& os, EndpointID const& v);
std::ostream& operator<<(std::ostream& os, kw::Inherit const*);
std::ostream& operator<<(std::ostream& os, kw::Eq const*);
std::ostream& operator<<(std::ostream& os, lit::Map const& v);
std::ostream& operator<<(std::ostream& os, Namespace const& v);
std::ostream& operator<<(std::ostream& os, UOp<ops::FuncCall> const&);
std::ostream& operator<<(std::ostream& os, UOp<ops::MacroCall> const&);
std::ostream& operator<<(std::ostream& os, UOp<ops::Not> const&);
std::ostream& operator<<(std::ostream& os, UOp<ops::AddFamily> const& v);
std::ostream& operator<<(std::ostream& os, UOp<ops::Subscript> const&);
std::ostream& operator<<(std::ostream& os, BOp<ops::Pow> const&);
std::ostream& operator<<(std::ostream& os, BOp<ops::MulFamily> const& v);
std::ostream& operator<<(std::ostream& os, BOp<ops::AddFamily> const& v);
std::ostream& operator<<(std::ostream& os, BOp<ops::Assign> const&);
std::ostream& operator<<(std::ostream& os, Root const& v);
std::ostream& operator<<(std::ostream& os, Stmt const& v);
std::ostream& operator<<(std::ostream& os, DefaultSpecifier const&);
std::ostream& operator<<(std::ostream& os, Argument const& v);
std::ostream& operator<<(std::ostream& os, Var const& v);
std::ostream& operator<<(std::ostream& os, Var const* v);


#include "saya/ika/vm/internal_def.hpp"

#define SAYA_DEF_I(lit_name) \
    std::ostream& operator<<(std::ostream& os, lit::lit_name const& v);

#define SAYA_DEF(z, i, d) \
    SAYA_DEF_I(BOOST_PP_SEQ_ELEM(i, d))

BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(SAYA_IKA_VM_LIT_TYPEMAP_NAMEONLY), SAYA_DEF, SAYA_IKA_VM_LIT_TYPEMAP_NAMEONLY)

#undef SAYA_DEF_I
#undef SAYA_DEF

#include "saya/ika/vm/internal_undef.hpp"


struct variant_printer : boost::static_visitor<std::ostream&>
{
    explicit variant_printer(std::ostream& os)
        : os_(os)
    {}

    template<class T, std::enable_if_t<!saya::is_variant_v<T>, int> = 0>
    std::ostream& operator()(T const& v) const
    {
        using ::saya::ika::ast::io::operator<<;
        return os_ << v;
    }

    template<class T, std::enable_if_t<saya::is_variant_v<T>, int> = 0>
    std::ostream& operator()(T const& v) const
    {
        using ::saya::ika::ast::io::operator<<;
        return boost::apply_visitor(*this, v);
    }

private:
    std::ostream& os_;
};

}}}} // saya

#endif
