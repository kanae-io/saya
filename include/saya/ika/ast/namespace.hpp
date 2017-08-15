#ifndef SAYA_IKA_AST_NAMESPACE_HPP
#define SAYA_IKA_AST_NAMESPACE_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

namespace saya { namespace ika { namespace ast {

struct Namespace : ASTEntity
{
    static constexpr char const* SEP() noexcept { return "::"; }
    static constexpr char const* GLOBAL_ID() noexcept { return "[global]"; }

    NSID id;

    Namespace() = default;
    explicit Namespace(NSID const& id)
        : id(id)
    {}
};

inline std::ostream& operator<<(std::ostream& os, Namespace const& v)
{
    return debug::with(
        os,
        "Namespace",
        debug::kv("id", debug::id_arg(*v.id.get()))
    );
}

}}} // saya

#endif
