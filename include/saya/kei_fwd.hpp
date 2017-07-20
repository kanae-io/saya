#ifndef SAYA_KEI_FWD_HPP_
#define SAYA_KEI_FWD_HPP_

namespace saya { namespace kei {

enum class LanguageID : unsigned;

namespace language {

struct Text;
struct Ika;

} // language

namespace ast {

struct ast_error;
struct entity_error;
struct entity_param_error;
struct literal_error;

enum class EntityID : unsigned;
enum class EntitySubID : unsigned;

enum class EntityParamID : unsigned;

class EntityParam;
class Entity;

class Literal;

} // ast

struct world_access;
class World;

template<class CharT, class SourceLang, class TargetLang>
struct compiler_traits;

template<
    class CharT,
    class SourceLang,
    class TargetLang,
    class Traits = compiler_traits<CharT, SourceLang, TargetLang>
>
class compiler;

template<class CharT, class Lang>
struct linter_traits;

template<
    class CharT,
    class Lang,
    class Traits = linter_traits<CharT, Lang>
>
class linter;

}} // saya

#endif
