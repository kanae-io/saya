#ifndef SAYA_IKA_AST_GROUP_HPP
#define SAYA_IKA_AST_GROUP_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

#include <boost/variant/variant.hpp>
#include <boost/optional.hpp>

#include <iosfwd>
#include <deque>

namespace saya { namespace ika { namespace ast {

struct Group : ASTEntity
{
    static constexpr char const* GLOBAL_ID() noexcept { return "[global]"; }
    static constexpr char const* SEP() noexcept { return ">"; }

    std::deque<GroupID> qualifier;
    GroupID id;
    boost::optional<AdditionalClass*> additional_class;
    boost::optional<Attribute*> attr;
    boost::optional<Geo*> definition;

    Group() = default;

    explicit Group(GroupID const& id)
        : id(id)
    {}
    explicit Group(std::deque<GroupID> const& qualifier, GroupID const& id, boost::optional<AdditionalClass*> const& additional_class)
        : qualifier(qualifier)
        , id(id)
        , additional_class(additional_class)
    {}

    void operator[](AdditionalClass* v)
    {
        BOOST_ASSERT(v);
        BOOST_ASSERT(!additional_class);

        additional_class = v;
    }

    void operator[](Attribute* v)
    {
        BOOST_ASSERT(v);
        BOOST_ASSERT(!attr);

        attr = v;
    }

    void operator[](Geo* v)
    {
        BOOST_ASSERT(v);
        BOOST_ASSERT(!definition);

        definition = v;
    }
};

struct Endpoint : ASTEntity
{
    EndpointID id;
    boost::optional<AdditionalClass*> additional_class;
    boost::optional<Attribute*> attr;

    Endpoint() = default;
    explicit Endpoint(EndpointID const& id, boost::optional<AdditionalClass*> const& additional_class)
        : id(id)
        , additional_class(additional_class)
    {}
    explicit Endpoint(EndpointID const& id, boost::optional<AdditionalClass*> const& additional_class, boost::optional<Attribute*> const& attr)
        : id(id), additional_class(additional_class), attr(attr)
    {}

    void operator[](AdditionalClass* v)
    {
        BOOST_ASSERT(v);
        BOOST_ASSERT(!additional_class);

        additional_class = v;
    }

    void operator[](Attribute* v)
    {
        BOOST_ASSERT(v);
        BOOST_ASSERT(!attr);

        attr = v;
    }
};

}}} // saya

#endif
