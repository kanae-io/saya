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

    Attribute() = default;
    explicit Attribute(std::vector<attr_type> const& attrs)
        : attrs(attrs)
    {}
};

}}} // saya

#endif
