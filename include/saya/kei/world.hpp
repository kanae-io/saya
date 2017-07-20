#ifndef SAYA_KEI_WORLD_HPP_
#define SAYA_KEI_WORLD_HPP_

#include "saya/kei_fwd.hpp"
#include "saya/kei/language.hpp"

#include <unordered_map>
#include <memory>

namespace saya { namespace kei {

class LanguageContext
{
public:

};

class World
{
public:
    friend class world_access;

private:
    void load(language::Text);
    void unload(language::Text);

    void load(language::Ika);
    void unload(language::Ika);

    std::unordered_map<LanguageID, std::unique_ptr<LanguageContext>>
    ctx_;
};

template<LanguageID ID>
std::string demangle(World* world, ast::EntityID eid, ast::EntitySubID esid);

template<LanguageID ID>
std::string demangle(World* world, ast::EntityParamID epid);

}} // saya

#endif
