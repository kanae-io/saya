#ifndef SAYA_IKA_FWD_HPP
#define SAYA_IKA_FWD_HPP

namespace saya { namespace ika {

struct ika_error;
struct world_error;

struct world_access;
class world;

struct parser_traits;
class parser;

struct compile_error;
struct compiler_traits;

struct compiler_options;

template<class Grammar>
class compiler;

struct linter_traits;
class linter;

}} // saya

#endif
