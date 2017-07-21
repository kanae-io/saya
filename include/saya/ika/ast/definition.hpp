#ifndef SAYA_IKA_AST_DEFINITION_HPP
#define SAYA_IKA_AST_DEFINITION_HPP

#include "saya/ika/ast_fwd.hpp"

#include "saya/ika/ast/ast_entity.hpp"
#include "saya/ika/ast/callable.hpp"
#include "saya/ika/ast/block.hpp"
#include "saya/ika/ast/group.hpp"
#include "saya/ika/ast/geo.hpp"

#include <boost/range/adaptor/transformed.hpp>


namespace saya { namespace ika { namespace ast {

struct FuncDefinition : ASTEntity
{
    Func* func;
    Block* block;
};

inline std::ostream& operator<<(std::ostream& os, FuncDefinition const& v)
{
    BOOST_ASSERT(v.func);

    return debug::with(
        os,
        "FuncDef",
        debug::kv("id", debug::id_arg(*v.func->id.get()))
    );
}

struct MacroDefinition : ASTEntity
{
    Macro* macro;
    Geo* geo;
};

inline std::ostream& operator<<(std::ostream& os, MacroDefinition const& v)
{
    BOOST_ASSERT(v.macro);

    return debug::with(
        os,
        "MacroDef",
        debug::kv("id", debug::id_arg(*v.macro->id.get()))
    );
}

struct GroupChildSpecifier : ASTEntity
{
    Group* child_group;
    std::vector<std::string> additional_class;
    boost::optional<Attribute> attr;
};

inline std::ostream& operator<<(std::ostream& os, GroupChildSpecifier const& v)
{
    BOOST_ASSERT(v.child_group);

    return debug::with(
        os,
        "GroupChild",
        debug::kv("id", debug::id_arg("." + *v.child_group->id.get())),
        debug::kv("additional_class", v.additional_class | boost::adaptors::transformed([] (auto const& c) { return "." + c; })),
        debug::kv("attr", v.attr)
    );
}

struct GroupDefinition : ASTEntity
{
    Group* group;
    boost::optional<GroupChildSpecifier> child_specifier;
    Geo* geo;
};

inline std::ostream& operator<<(std::ostream& os, GroupDefinition const& v)
{
    BOOST_ASSERT(v.group);

    return debug::with(
        os,
        "GroupDef",
        debug::kv("id", debug::id_arg("." + *v.group->id.get())),
        debug::kv("c", v.child_specifier)
    );
}

}}} // saya

#endif
