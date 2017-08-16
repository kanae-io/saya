#ifndef SAYA_IKA_AST_DECLARATION_HPP
#define SAYA_IKA_AST_DECLARATION_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"
#include "saya/ika/ast/group.hpp"

#include <boost/optional.hpp>


namespace saya { namespace ika { namespace ast {

struct Declaration : ASTEntity
{
    using groupable_type = boost::variant<Group*, Endpoint*>;

    groupable_type groupable;
    boost::optional<Attribute*> attr;

    bool is_inline{true}; // IMPORTANT: this must be true by default
};

inline std::ostream& operator<<(std::ostream& os, Declaration const& v)
{
    return debug::with(
        os,
        "Decl",
        debug::kv("groupable", v.groupable),
        debug::kv("attr", v.attr),
        debug::cond(v.is_inline, [] (auto& os) -> decltype(auto) { return os << "inline"; }, [] (auto& os) -> decltype(auto) { return os; })
    );
}

}}} // saya

#endif
