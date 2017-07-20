#ifndef SAYA_KEI_AST_ENTITY_HPP_
#define SAYA_KEI_AST_ENTITY_HPP_

#include "saya/kei_fwd.hpp"
#include "saya/kei/ast/error.hpp"
#include "saya/kei/ast/param.hpp"

#include <string>
#include <unordered_map>
#include <memory>


namespace saya { namespace kei { namespace ast {

enum class EntityID : unsigned {};
enum class EntitySubID : unsigned {};

struct entity_error : ast_error
{
    entity_error(EntityID eid, EntitySubID esid, std::string const& reason)
        : ast_error(reason), eid(eid), esid(esid)
    {}

    EntityID eid;
    EntitySubID esid;
};

class Entity
{
public:
    using param_ptr_type = std::unique_ptr<EntityParam>;

    virtual ~Entity() /* noexcept */ = default;
    EntityID eid() const noexcept { return eid_; }
    EntitySubID esid() const noexcept { return esid_; }

    inline virtual void assign(param_ptr_type pp) = 0;

protected:
    explicit Entity(EntityID eid, EntitySubID esid) noexcept
        : eid_(eid)
        , esid_(esid)
    {}

private:
    Entity(Entity&&) = delete;
    Entity(Entity const&) = delete;

    Entity& operator=(Entity&&) = delete;
    Entity& operator=(Entity const&) = delete;

    EntityID const eid_;
    EntitySubID const esid_;

    std::unordered_map<EntityParamID, param_ptr_type>
    params_;
};

inline void Entity::assign(Entity::param_ptr_type pp)
{
    this->params_[pp->pid()] = std::move(pp);
}

}}} // saya

#endif
