#ifndef SAYA_KEI_LANGUAGE_HPP_
#define SAYA_KEI_LANGUAGE_HPP_

#include "saya/kei_fwd.hpp"

namespace saya { namespace kei {

enum class LanguageID : unsigned {};

namespace language {

struct Text { enum : LanguageID { ID = 0 }; };
struct Ika { enum : LanguageID { ID = 10 }; };

} // language

}} // saya

#endif
