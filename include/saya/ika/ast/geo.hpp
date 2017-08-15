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

inline std::ostream& operator<<(std::ostream& os, Geo::Dir d)
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

inline std::ostream& operator<<(std::ostream& os, Geo::Justify j)
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

inline std::ostream& operator<<(std::ostream& os, Geo const& v)
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

inline std::ostream& operator<<(std::ostream& os, Geo const* v)
{
    return debug::proxy(
        os,
        "Geo",
        v
    );
}

}}} // saya

#endif
