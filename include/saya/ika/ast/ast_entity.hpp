#ifndef SAYA_IKA_AST_AST_ENTITY_HPP
#define SAYA_IKA_AST_AST_ENTITY_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/debug.hpp"

namespace saya { namespace ika { namespace ast {

struct ASTEntity
{
    ASTEntity() noexcept = default;
    ASTEntity(ASTEntity&&) noexcept = default;
    ASTEntity(ASTEntity const&) noexcept = default;
    virtual ~ASTEntity() noexcept = default;

    ASTEntity& operator=(ASTEntity&&) noexcept = default;
    ASTEntity& operator=(ASTEntity const&) noexcept = default;
};

}}} // saya

#endif
