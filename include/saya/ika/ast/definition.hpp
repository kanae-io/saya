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

struct MacroDefinition : ASTEntity
{
    Macro* macro{nullptr};
    Geo* geo{nullptr};
};

struct GroupChildSpecifier : ASTEntity
{
    Group* child{nullptr};
    boost::optional<Attribute*> attr;
};

struct GroupDefinition : ASTEntity
{
    Group* group{nullptr};
    boost::optional<GroupChildSpecifier> child_specifier;
    Geo* geo{nullptr};
};

}}} // saya

#endif
