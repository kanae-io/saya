#ifndef SAYA_IKA_AST_KEYWORD_HPP
#define SAYA_IKA_AST_KEYWORD_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

namespace saya { namespace ika { namespace ast { namespace kw {

struct Inherit : ASTEntity {};
struct Eq : ASTEntity {};

} // kw

}}} // saya

#endif
