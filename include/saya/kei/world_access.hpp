#ifndef SAYA_KEI_WORLD_ACCESS_HPP_
#define SAYA_KEI_WORLD_ACCESS_HPP_

#include "saya/kei_fwd.hpp"
#include "saya/kei/world.hpp"

namespace saya { namespace kei {

struct world_access
{
    template<class Lang>
    inline static void load(Lang, World* world)
    {
        return world->load(Lang{});
    }

    template<class Lang>
    inline static void unload(Lang, World* world)
    {
        return world->unload(Lang{});
    }

private:
    world_access() = delete;
};

}} // saya

#endif
