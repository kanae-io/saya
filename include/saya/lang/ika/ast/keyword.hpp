#ifndef SAYA_LANG_IKA_AST_KEYWORD_HPP_
#define SAYA_LANG_IKA_AST_KEYWORD_HPP_

#include "saya/lang/ika/ast/ast_entity.hpp"

namespace saya { namespace lang { namespace ika { namespace ast {

class Keyword : public ASTEntity
{
public:
    static Keyword const
        LAYOUT, COLOR,
        JS, CSS,
        INPUT, TEXT, NUMBER, BUTTON,
        TAB, TABLE
    ;
};

}}}} // saya

#endif
