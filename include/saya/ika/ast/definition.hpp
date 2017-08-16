#ifndef SAYA_IKA_AST_DEFINITION_HPP
#define SAYA_IKA_AST_DEFINITION_HPP

#include "saya/ika/ast_fwd.hpp"

#include "saya/ika/ast/ast_entity.hpp"
#include "saya/ika/ast/callable.hpp"
#include "saya/ika/ast/block.hpp"
#include "saya/ika/ast/group.hpp"
#include "saya/ika/ast/geo.hpp"

namespace saya { namespace ika { namespace ast {

struct FuncDefinition : ASTEntity
{
    Func* func{nullptr};
    Block* block{nullptr};
};

inline std::ostream& operator<<(std::ostream& os, FuncDefinition const& v)
{
    BOOST_ASSERT(v.func);

    return debug::with(
        os,
        "FuncDef",
        debug::kv("id", v.func->id)
    );
}

struct MacroDefinition : ASTEntity
{
    Macro* macro{nullptr};
    Geo* geo{nullptr};
};

inline std::ostream& operator<<(std::ostream& os, MacroDefinition const& v)
{
    BOOST_ASSERT(v.macro);

    return debug::with(
        os,
        "MacroDef",
        debug::kv("id", v.macro->id)
    );
}

struct GroupChildSpecifier : ASTEntity
{
    GroupID child_id;
    boost::optional<AdditionalClass*> additional_class;
    boost::optional<Attribute*> attr;
};

inline std::ostream& operator<<(std::ostream& os, GroupChildSpecifier const& v)
{
    return debug::with(
        os,
        "ChildSpecifier",
        debug::kv("child_id", debug::id_arg("." + *v.child_id.get())),
        debug::kv("additional_class", v.additional_class),
        debug::kv("attr", v.attr)
    );
}

struct GroupDefinition : ASTEntity
{
    Group* group{nullptr};
    boost::optional<GroupChildSpecifier> child_specifier;
    Geo* geo{nullptr};
};

inline std::ostream& operator<<(std::ostream& os, GroupDefinition const& v)
{
    BOOST_ASSERT(v.group);

    return debug::with(
        os,
        "GroupDef",
        debug::kv("id", v.group->id),
        debug::kv("c", v.child_specifier)
    );
}

}}} // saya

#endif
