#ifndef SAYA_LANG_IKA_AST_FWD_HPP_
#define SAYA_LANG_IKA_AST_FWD_HPP_

#include <string>

namespace saya { namespace lang { namespace ika { namespace ast {

template<class CharT>
struct basic_ika_traits;

template<class CharT, class Traits = basic_ika_traits<CharT>>
class basic_ika;

}}}} // saya

#endif
