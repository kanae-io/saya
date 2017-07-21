#ifndef SAYA_IKA_AST_CALLABLE_HPP
#define SAYA_IKA_AST_CALLABLE_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"
#include "saya/ika/ast/geo.hpp"
#include "saya/ika/ast/block.hpp"

#include <boost/assert.hpp>

#include <vector>

namespace saya { namespace ika { namespace ast {

struct Func : ASTEntity
{
    FuncID id;
    Block const* definition{nullptr};

    explicit Func(FuncID const& id)
        : id(id)
    {}

    void operator[](Block const* b)
    {
        BOOST_ASSERT(b);
        definition = b;
    }
};

struct Macro : ASTEntity
{
    MacroID id;
    Geo const* definition{nullptr};

    explicit Macro(MacroID const& id)
        : id(id)
    {}

    void operator[](Geo const* g)
    {
        BOOST_ASSERT(g);
        definition = g;
    }
};

struct CallParam : ASTEntity
{
    std::vector<Expr> expr_list;
};


inline std::ostream& operator<<(std::ostream& os, Func const& v)
{
    return debug::with(
        os,
        "Func",
        debug::kv("id", debug::id_arg(*v.id.get())),
        debug::kv("def", v.definition)
    );
}

inline std::ostream& operator<<(std::ostream& os, Func const* v)
{
    return debug::proxy(
        os,
        "Func",
        v
    );
}

inline std::ostream& operator<<(std::ostream& os, Macro const& v)
{
    return debug::with(
        os,
        "Macro",
        debug::kv("id", debug::id_arg(*v.id.get())),
        debug::kv("def", v.definition)
    );
}

inline std::ostream& operator<<(std::ostream& os, Macro const* v)
{
    return debug::proxy(
        os,
        "Macro",
        v
    );
}

inline std::ostream& operator<<(std::ostream& os, CallParam const& v)
{
    return debug::with(
        os,
        "Param",
        debug::kv("argn", v.expr_list.size())
    );
}

}}} // saya

#endif
