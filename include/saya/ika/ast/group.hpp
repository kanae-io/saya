#ifndef SAYA_IKA_AST_GROUP_HPP
#define SAYA_IKA_AST_GROUP_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

#include <boost/variant/variant.hpp>
#include <boost/optional.hpp>

#include <iosfwd>


namespace saya { namespace ika { namespace ast {

struct Group : ASTEntity
{
    GroupID id;
    boost::optional<Attribute> attr;
    Geo const* definition{nullptr};

    Group() = default;
    explicit Group(GroupID const& id)
        : id(id)
    {}
    explicit Group(GroupID const& id, boost::optional<Attribute> const& attr)
        : id(id), attr(attr)
    {}

    void operator[](Geo const* g)
    {
        BOOST_ASSERT(g);
        BOOST_ASSERT(!definition);

        definition = g;
    }
};

struct Endpoint : ASTEntity
{
    EndpointID id;
    boost::optional<Attribute> attr;

    Endpoint() = default;
    explicit Endpoint(EndpointID const& id)
        : id(id)
    {}
    explicit Endpoint(EndpointID const& id, boost::optional<Attribute> const& attr)
        : id(id), attr(attr)
    {}
};

inline std::ostream& operator<<(std::ostream& os, Group const& v)
{
    return debug::with(
        os,
        "Group",
        debug::kv("id", debug::id_arg(*v.id.get())),
        debug::kv("attr", v.attr),
        debug::cond(
            "definition",
            v.definition,
            [] (auto& os) -> decltype(auto) {
                return os << "yes";
            },
            [] (auto& os) -> decltype(auto) {
                 return os << "none";
            }
        )
    );
}

inline std::ostream& operator<<(std::ostream& os, Group const* v)
{
    return debug::proxy(
        os,
        "Group",
        v
    );
}

inline std::ostream& operator<<(std::ostream& os, Endpoint const& v)
{
    return debug::with(
        os,
        "Endpoint",
        debug::kv("id", debug::id_arg(*v.id.get())),
        debug::kv("attr", v.attr)
    );
}

inline std::ostream& operator<<(std::ostream& os, Endpoint const* v)
{
    return debug::proxy(
        os,
        "Endpoint",
        v
    );
}

}}} // saya

#endif
