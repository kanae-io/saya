#ifndef SAYA_IKA_AST_ATTRIBUTE_HPP
#define SAYA_IKA_AST_ATTRIBUTE_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

#include <vector>

namespace saya { namespace ika { namespace ast {

struct Attribute : ASTEntity
{
    using attr_type = boost::variant<
        kw::Inherit const*,
        kw::Eq const*,
        Expr
    >;
    std::vector<attr_type> attrs;
};

inline std::ostream& operator<<(std::ostream& os, Attribute const& v)
{
    BOOST_ASSERT(!v.attrs.empty());

    return debug::with(
        os,
        "Attr",
        debug::kv("attrs", v.attrs)
    );
}

inline std::ostream& operator<<(std::ostream& os, boost::optional<Attribute> const& v)
{
    return debug::proxy(
        os,
        "Attr",
        v
    );
}

}}} // saya

#endif
