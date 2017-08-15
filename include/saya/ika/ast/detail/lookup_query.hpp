#ifndef SAYA_IKA_AST_DETAIL_LOOKUP_QUERY_HPP
#define SAYA_IKA_AST_DETAIL_LOOKUP_QUERY_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/id.hpp"

#include <boost/functional/hash.hpp>

#include <algorithm>
#include <deque>


namespace saya { namespace ika { namespace ast { namespace detail {

template<class T>
struct LookupQuery;

template<>
struct LookupQuery<Namespace>
{
    std::deque<NSID> qualifier;
    NSID target;

    std::size_t ctx_hash{static_cast<std::size_t>(-1)};

    explicit LookupQuery(std::deque<NSID> const& qualifier, NSID const& target)
        : qualifier(qualifier)
        , target(target)
    {}

    explicit LookupQuery(NSID const& target)
        : target(target)
    {}
};

template<>
struct LookupQuery<Var>
{
    std::deque<NSID> qualifier;
    VarID target;

    std::size_t ctx_hash{static_cast<std::size_t>(-1)};

    explicit LookupQuery(std::deque<NSID> const& qualifier, VarID const& target)
        : qualifier(qualifier)
        , target(target)
    {}

    explicit LookupQuery(VarID const& target)
        : target(target)
    {}
};

template<>
struct LookupQuery<Func>
{
    std::deque<NSID> qualifier;
    FuncID target;

    std::size_t ctx_hash{static_cast<std::size_t>(-1)};

    explicit LookupQuery(std::deque<NSID> const& qualifier, FuncID const& target)
        : qualifier(qualifier)
        , target(target)
    {}

    explicit LookupQuery(FuncID const& target)
        : target(target)
    {}
};

template<>
struct LookupQuery<Macro>
{
    std::deque<NSID> qualifier;
    MacroID target;

    std::size_t ctx_hash{static_cast<std::size_t>(-1)};

    explicit LookupQuery(std::deque<NSID> const& qualifier, MacroID const& target)
        : qualifier(qualifier)
        , target(target)
    {}

    explicit LookupQuery(MacroID const& target)
        : target(target)
    {}
};

template<>
struct LookupQuery<Group>
{
    std::deque<GroupID> qualifier;
    GroupID target;

    std::size_t ctx_hash{static_cast<std::size_t>(-1)};

    explicit LookupQuery(std::deque<GroupID> const& qualifier, GroupID const& target)
        : qualifier(qualifier)
        , target(target)
    {}

    explicit LookupQuery(GroupID const& target)
        : target(target)
    {}
};

template<>
struct LookupQuery<Endpoint>
{
    std::deque<GroupID> qualifier;
    EndpointID target;

    std::size_t ctx_hash{static_cast<std::size_t>(-1)};

    explicit LookupQuery(std::deque<GroupID> const& qualifier, EndpointID const& target)
        : qualifier(qualifier)
        , target(target)
    {}

    explicit LookupQuery(EndpointID const& target)
        : target(target)
    {}
};

template<class T>
inline bool operator==(LookupQuery<T> const& lhs, LookupQuery<T> const& rhs)
{
    return
        lhs.target == rhs.target &&
        std::equal(lhs.qualifier.begin(), lhs.qualifier.end(), rhs.qualifier.begin(), rhs.qualifier.end())
    ;
}

template<class T>
inline std::size_t hash_value(LookupQuery<T> const& query)
{
    return query.ctx_hash;
}

}}}} // saya

#endif
