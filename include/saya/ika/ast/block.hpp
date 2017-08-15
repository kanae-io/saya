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

inline std::ostream& operator<<(std::ostream& os, BlockParam const& v)
{
    BOOST_ASSERT(!v.args.empty());

    return debug::with(
        os,
        "Param",
        debug::kv("args", v.args)
    );
}

inline std::ostream& operator<<(std::ostream& os, boost::optional<BlockParam> const& v)
{
    return debug::proxy(
        os,
        "Param",
        v
    );
}

struct Block : ASTEntity
{
    boost::optional<BlockParam> param;
    std::vector<Stmt> stmt_list;

    Block() = default;
    explicit Block(boost::optional<BlockParam> const& param, std::vector<Stmt> const& stmt_list)
        : param(param), stmt_list(stmt_list)
    {}
};

inline std::ostream& operator<<(std::ostream& os, Block const& v)
{
    return debug::with(
        os,
        "Block",
        debug::kv("param", v.param),
        debug::kv("sloc", v.stmt_list.size())
    );
}

inline std::ostream& operator<<(std::ostream& os, boost::optional<Block*> const& v)
{
    return debug::proxy(
        os,
        "Block",
        v
    );
}

inline std::ostream& operator<<(std::ostream& os, boost::optional<Block> const& v)
{
    return debug::proxy(
        os,
        "Block",
        v
    );
}

inline std::ostream& operator<<(std::ostream& os, Block const* v)
{
    return debug::proxy(
        os,
        "Block",
        v
    );
}

}}} // saya

#endif
