#ifndef SAYA_IKA_AST_GEO_HPP
#define SAYA_IKA_AST_GEO_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"
#include "saya/ika/ast/attribute.hpp"

#include <boost/optional.hpp>

namespace saya { namespace ika { namespace ast {

struct Geo : ASTEntity
{
    enum struct Dir : unsigned
    {
        HOR = 0,
        HOR_REV,
        VER,
        VER_REV,
    };

    enum struct Justify : unsigned
    {
        EXPAND = 0,
        START,
        END,
        CENTER,
    };

    Dir dir;
    Justify justify;
    boost::optional<Attribute*> attr;
    boost::optional<Block*> block;

    Geo() = default;
    explicit Geo(Dir const& dir, Justify const& j, boost::optional<Attribute*> const& attr, boost::optional<Block*> const& block)
        : dir(dir), justify(j), attr(attr), block(block)
    {}
};

}}} // saya

#endif
