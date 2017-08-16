#ifndef SAYA_IKA_AST_META_LIT_HPP
#define SAYA_IKA_AST_META_LIT_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"
#include "saya/ika/ast/lit.hpp"
#include "saya/ika/ast/expr.hpp"

#include <unordered_map>

namespace saya { namespace ika { namespace ast { namespace lit {

#include "saya/ika/vm/internal_def.hpp"

struct Map : LiteralEntity
{
    static constexpr std::size_t MAGIC_ID() noexcept { return BOOST_PP_SEQ_SIZE(SAYA_IKA_VM_LIT_TYPEMAP) + 0; }
    static char const* LIT_NAME() noexcept { return "Map"; }
    static vm::TypeID TYPE_ID() { return vm::TypeID{"Map"}; }

    using self_type = Map;
    using key_type = Symbol;
    using internal_type = std::unordered_map<key_type, Expr>;
    using value_type = typename internal_type::value_type;
    using mapped_type = typename internal_type::mapped_type;
    internal_type v;

    Map() = default;
    Map(Map const&) = default;
    Map(Map&& rhs) = default;

    explicit Map(internal_type v)
        : LiteralEntity(TYPE_ID())
        , v(std::move(v))
    {}

    Map& operator=(Map const&) = default;
    Map& operator=(Map&&) = default;

    auto begin() const { return v.begin(); }
    auto begin() { return v.begin(); }

    auto end() const { return v.end(); }
    auto end() { return v.end(); }

    bool empty() const noexcept { return v.empty(); }
    bool has_key(key_type const& k) const noexcept { return v.count(k); }

    mapped_type& operator[](key_type const& k) { return v[k]; }
    mapped_type& at(key_type const& k) { return v.at(k); }
    mapped_type const& at(key_type const& k) const { return v.at(k); }

    void swap(Map& rhs) noexcept
    {
        using std::swap;
        if (this != std::addressof(rhs)) {
            LiteralEntity::swap(rhs);
            swap(v, rhs.v);
        }
    }
};

inline std::ostream& operator<<(std::ostream& os, Map::value_type const& kv)
{
    return os << debug::literal(kv.first) << ": " << kv.second;
}

inline std::ostream& operator<<(std::ostream& os, Map const& v)
{
    return debug::with(
        os,
        "Map",
        debug::kv("map", v)
    );
}

#include "saya/ika/vm/internal_undef.hpp"

}}}} // saya

#endif
