#ifndef SAYA_LANG_IKA_AST_TRAITS_HPP_
#define SAYA_LANG_IKA_AST_TRAITS_HPP_

#include "saya/lang/ika/ast_fwd.hpp"
#include "saya/string_config.hpp"

namespace saya { namespace lang { namespace ika { namespace ast {

#define SAYA_DEF(CHAR_TYPE, BINDER) \
    template<> \
    struct basic_ika_traits<CHAR_TYPE> \
    { \
    };

SAYA_STRING_CONFIG_DEFINE(SAYA_DEF)
#undef SAYA_DEF

}}}} // saya

#endif
