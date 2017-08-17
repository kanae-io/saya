#ifndef SAYA_IKA_AST_LIT_HPP
#define SAYA_IKA_AST_LIT_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

#include "saya/zed/pct.hpp"

#include <boost/format.hpp>

#include <boost/multiprecision/cpp_dec_float.hpp>

#include <boost/variant/variant.hpp>
#include <boost/functional/hash.hpp>

#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/tuple/push_back.hpp>
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/size.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/repetition/for.hpp>

#include <functional>
#include <type_traits>
#include <cstdint>


namespace saya { namespace ika { namespace ast {

struct LiteralEntity : ASTEntity
{
    vm::TypeID type_id;

    LiteralEntity() noexcept = default;
    LiteralEntity(LiteralEntity const&) = default;
    LiteralEntity(LiteralEntity&&) noexcept = default;

    LiteralEntity& operator=(LiteralEntity const&) = default;
    LiteralEntity& operator=(LiteralEntity&&) noexcept = default;

    virtual ~LiteralEntity() = default;

    explicit LiteralEntity(vm::TypeID&& type_id)
        : type_id(std::move(type_id))
    {}

    // explicit LiteralEntity(vm::TypeID const& type_id)
    //     : type_id(type_id)
    // {}

    void swap(LiteralEntity& rhs) noexcept
    {
        using std::swap;
        if (this != std::addressof(rhs)) {
            swap(type_id, rhs.type_id);
        }
    }

    std::size_t hash2() const { return this->hash2_impl(); }

protected:
    virtual std::size_t hash2_impl() const = 0;
};

}}} // saya


#include "saya/ika/vm/internal_def.hpp"

#define SAYA_IKA_DEFINE_LIT_NAMESPACE_BEGIN namespace saya { namespace ika { namespace ast { namespace lit {
#define SAYA_IKA_DEFINE_LIT_NAMESPACE_END }}}}

#define SAYA_IKA_DEFINE_LIT_BEGIN(magic_i, vtypemap, lit_name) \
    SAYA_IKA_DEFINE_LIT_NAMESPACE_BEGIN \
    \
    std::size_t hash_value(lit_name const& v); \
    \
    struct lit_name : LiteralEntity \
    { \
        static constexpr std::size_t MAGIC_ID() noexcept { return magic_i; } \
        SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR(lit_name) \
        static char const* LIT_NAME() noexcept { return BOOST_PP_STRINGIZE(lit_name); } \
        static vm::TypeID TYPE_ID() { return vm::types::SAYA_IKA_VM_INTERNAL_NAME_FOR(vtypemap)(); } \
        \
        using self_type = lit_name; \
        using internal_type = SAYA_IKA_VM_INTERNAL_TYPE_FOR(vtypemap); \
        internal_type v; \
        \
        lit_name() = default; \
        lit_name(lit_name const&) = default; \
        lit_name(lit_name&& rhs) = default; \
        \
        explicit lit_name(internal_type v) \
            : LiteralEntity(TYPE_ID()) \
            , v(std::move(v)) \
        {} \
        \
        lit_name& operator=(lit_name const&) = default; \
        lit_name& operator=(lit_name&&) = default; \
        \
        void swap(lit_name& rhs) noexcept \
        { \
            using std::swap; \
            if (this != std::addressof(rhs)) { \
                LiteralEntity::swap(rhs); \
                swap(v, rhs.v); \
            } \
        } \
        \
        inline bool operator==(lit_name const& rhs) const \
        { \
            return v == rhs.v; \
        } \
        \
        protected: \
        virtual std::size_t hash2_impl() const override \
        { \
            return hash_value(*this); \
        }

#define SAYA_IKA_DEFINE_LIT_END(lit_name) \
    }; \
    \
    inline std::size_t hash_value(lit_name const& v) \
    { \
        std::size_t seed = 0; \
        boost::hash_combine(seed, lit_name::MAGIC_ID()); \
        boost::hash_combine(seed, v.v); \
        return seed; \
    } \
    \
    SAYA_IKA_DEFINE_LIT_NAMESPACE_END

// ------------------------------------------------------

#define SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_String(lit_name)

#define SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_Symbol(lit_name) \
    static constexpr char MAGIC_TOKEN() noexcept { return ':'; }

#define SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_Int64(lit_name) \
    static constexpr char MAGIC_TOKEN() noexcept { return '-'; }

#define SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_UInt64(lit_name) SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_String(lit_name)
#define SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_Real(lit_name) SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_String(lit_name)
#define SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_Color(lit_name) \
    static constexpr char MAGIC_TOKEN() noexcept { return '#'; }

#define SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_Px(lit_name) SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_String(lit_name)
#define SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_Pct(lit_name) SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_String(lit_name)

#define SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR(lit_name) SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_ ## lit_name (lit_name)

// ------------------------------------------------------

#define SAYA_IKA_DEFINE_LIT_BODY_FOR_String(lit_name)
#define SAYA_IKA_DEFINE_LIT_BODY_FOR_Symbol(lit_name)
#define SAYA_IKA_DEFINE_LIT_BODY_FOR_Int64(lit_name)
#define SAYA_IKA_DEFINE_LIT_BODY_FOR_UInt64(lit_name)
#define SAYA_IKA_DEFINE_LIT_BODY_FOR_Real(lit_name)

#define SAYA_IKA_DEFINE_LIT_BODY_FOR_Color(lit_name) \
    public: \
        using a_mp_type = boost::multiprecision::number< \
            boost::multiprecision::cpp_dec_float< \
                3, std::int32_t \
            >, \
            boost::multiprecision::et_on \
        >; \
        \
        std::uint8_t a_i() const noexcept { \
            return v & 0xFF; \
        } \
        float a_f() const noexcept { \
            return (v & 0xFF) * 0.01f; \
        } \
        a_mp_type a_mp() const noexcept { \
            return a_mp_type{v & 0xFF} / 100; \
        } \
        std::uint8_t r() const noexcept { \
            return v & 0xFF000000; \
        } \
        std::uint8_t g() const noexcept { \
            return v & 0x00FF0000; \
        } \
        std::uint8_t b() const noexcept { \
            return v & 0x0000FF00; \
        }

#define SAYA_IKA_DEFINE_LIT_BODY_FOR_Px(lit_name)
#define SAYA_IKA_DEFINE_LIT_BODY_FOR_Pct(lit_name)


#define SAYA_IKA_DEFINE_LIT_BODY_FOR(lit_name) SAYA_IKA_DEFINE_LIT_BODY_FOR_ ## lit_name (lit_name)

#define SAYA_DEF_II(vtmapid, lit_name, unused0, magic_i) \
    SAYA_IKA_DEFINE_LIT_BEGIN( \
        magic_i, \
        SAYA_IKA_VM_LIT_VTYPEMAP_FOR(vtmapid), \
        lit_name \
    ) \
    SAYA_IKA_DEFINE_LIT_BODY_FOR(lit_name) \
    SAYA_IKA_DEFINE_LIT_END(lit_name)

#define SAYA_DEF_I(tup) \
    SAYA_DEF_II tup

#define SAYA_DEF(z, i, d) \
    SAYA_DEF_I(BOOST_PP_TUPLE_PUSH_BACK(BOOST_PP_SEQ_ELEM(i, d), i))

BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(SAYA_IKA_VM_LIT_TYPEMAP), SAYA_DEF, SAYA_IKA_VM_LIT_TYPEMAP)

#undef SAYA_DEF_II
#undef SAYA_DEF_I
#undef SAYA_DEF

#define SAYA_DEF_I(lit_name) \
    namespace std { \
    template<> \
    struct hash<::saya::ika::ast::lit::lit_name> \
    { \
        size_t operator()(::saya::ika::ast::lit::lit_name const& v) const \
        { \
            return ::saya::ika::ast::lit::hash_value(v); \
        } \
    }; \
    }

#define SAYA_DEF(z, i, d) SAYA_DEF_I(BOOST_PP_SEQ_ELEM(i, d))

BOOST_PP_REPEAT(BOOST_PP_SEQ_SIZE(SAYA_IKA_VM_LIT_TYPEMAP_NAMEONLY), SAYA_DEF, SAYA_IKA_VM_LIT_TYPEMAP_NAMEONLY)
#undef SAYA_DEF_I
#undef SAYA_DEF


namespace saya { namespace ika { namespace ast { namespace lit {

using all_type = boost::variant<
    BOOST_PP_SEQ_ENUM(SAYA_IKA_VM_LIT_TYPEMAP_NAMEONLY)
>;

}}}} // saya

// -----------

#undef SAYA_IKA_DEFINE_LIT_NAMESPACE_BEGIN
#undef SAYA_IKA_DEFINE_LIT_NAMESPACE_END

#undef SAYA_IKA_DEFINE_LIT_BEGIN
#undef SAYA_IKA_DEFINE_LIT_END

#undef SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_String
#undef SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_Symbol
#undef SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_Int64
#undef SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_UInt64
#undef SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_Real
#undef SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_Color
#undef SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_Px
#undef SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR_Pct
#undef SAYA_IKA_DEFINE_LIT_MAGIC_TOKEN_FOR

#undef SAYA_IKA_DEFINE_LIT_BODY_FOR_String
#undef SAYA_IKA_DEFINE_LIT_BODY_FOR_Symbol
#undef SAYA_IKA_DEFINE_LIT_BODY_FOR_Int64
#undef SAYA_IKA_DEFINE_LIT_BODY_FOR_UInt64
#undef SAYA_IKA_DEFINE_LIT_BODY_FOR_Real
#undef SAYA_IKA_DEFINE_LIT_BODY_FOR_Color
#undef SAYA_IKA_DEFINE_LIT_BODY_FOR_Px
#undef SAYA_IKA_DEFINE_LIT_BODY_FOR_Pct
#undef SAYA_IKA_DEFINE_LIT_BODY_FOR
#undef SAYA_DEF

#include "saya/ika/vm/internal_undef.hpp"

namespace saya { namespace ika { namespace ast { namespace lit {

template<class T, class Enabled = void>
struct deep_hash;

template<class T>
struct deep_hash<T, typename std::enable_if_t<std::is_same<LiteralEntity, T>::value>>
{
    inline std::size_t operator()(T const* lvp) const
    {
        return lvp->hash2();
    }
};

}}}} // saya

#endif
