#ifndef SAYA_IKA_AST_BLOCK_HPP
#define SAYA_IKA_AST_BLOCK_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

#include "saya/ika/ast/syntax.hpp"

#include <vector>

namespace saya { namespace ika { namespace ast {

struct BlockParam : ASTEntity
{
    std::vector<Argument> args;
};

struct Block : ASTEntity
{
    boost::optional<BlockParam> param;
    std::vector<Stmt> stmt_list;

    Block() = default;
    explicit Block(boost::optional<BlockParam> const& param, std::vector<Stmt> const& stmt_list)
        : param(param), stmt_list(stmt_list)
    {}
};

}}} // saya

#endif
