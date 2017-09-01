#ifndef SAYA_IKA_AST_FWD_HPP
#define SAYA_IKA_AST_FWD_HPP

#include <boost/optional.hpp>
#include <boost/variant/variant.hpp>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <iosfwd>
#include <memory>
#include <vector>


namespace saya { namespace ika { namespace ast {

struct ast_error;
struct scope_error;

namespace factory_tag {

struct AST_NSOnly;
struct AST;

} // factory_tag


struct ASTEntity;
struct LiteralEntity;

class Root;

namespace ops {
struct FuncCall {};
struct MacroCall {};

struct Not {};

struct Pow {};
struct MulFamily {};
struct AddFamily {};

struct Assign {};

struct Subscript {};

} // ops

template<class Op>
struct UOp;

template<class Op>
struct BOp;

// --------------------------------------------------

namespace lit {

#include "saya/ika/vm/internal_def.hpp"

#define SAYA_DEF_I(unused0, lit_name, unused1) \
    struct lit_name;

#define SAYA_DEF(r, data, elem) \
    SAYA_DEF_I elem

BOOST_PP_SEQ_FOR_EACH(SAYA_DEF, _, SAYA_IKA_VM_LIT_TYPEMAP)
#undef SAYA_DEF_I
#undef SAYA_DEF

#include "saya/ika/vm/internal_undef.hpp"

struct Map;

} // lit

// --------------------------------------------------

struct AdditionalClass;

struct Var;

struct Func;
struct Macro;

struct Block;
struct BlockParam;
struct CallParam;

struct Namespace;

// --------------------------------------------------

struct DefaultSpecifier;
struct Argument;

// --------------------------------------------------

struct UEntity;

struct PrimaryExpr;
struct Expr;
struct SideEffectExpr;

struct EmptyExpr;

struct Stmt;

struct Declaration;

struct FuncDefinition;
struct MacroDefinition;
struct GroupDefinition;

struct GroupChildSpecifier;

struct Attribute;

// --------------------------------------------------

namespace kw {

struct Inherit;
struct Eq;

} // kw

using Placeholdable = boost::variant<kw::Inherit const*, kw::Eq const*>;

// --------------------------------------------------

struct Geo;

// --------------------------------------------------

struct Group;
struct Endpoint;

// --------------------------------------------------

}}} // saya

#endif
