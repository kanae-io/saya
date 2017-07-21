#ifndef SAYA_IKA_VM_VALUE_HPP
#define SAYA_IKA_VM_VALUE_HPP

#include "saya/ika/vm/type.hpp"

#include "saya/zed/pct.hpp"

#include <boost/multiprecision/cpp_dec_float.hpp>

#include <boost/variant/variant.hpp>
#include <boost/variant/static_visitor.hpp>

#include <boost/preprocessor/seq/seq.hpp> // TAIL
#include <boost/preprocessor/seq/elem.hpp>
#include <boost/preprocessor/seq/enum.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <iosfwd>


namespace saya { namespace ika { namespace vm {

#include "saya/ika/vm/internal_def.hpp"

using internal_value_type = boost::variant<
    BOOST_PP_SEQ_HEAD(SAYA_IKA_VM_INTERNAL_TYPEMAP_TYPEONLY) const*,
    BOOST_PP_SEQ_ENUM(BOOST_PP_SEQ_TAIL(SAYA_IKA_VM_INTERNAL_TYPEMAP_TYPEONLY))
>;

struct Value
{
    TypeID type_id;
    internal_value_type v;

    Value() noexcept = default;
    ~Value() = default;

    #if 0
    Value()
        : type_id(/*vm::types::Undecided()*/)
        , v(/*vm::make_undecided()*/)
    {}
    #endif

    Value(Value&&) noexcept = default;
    Value(Value const&) = default;

    // default-initialzer, maybe?
    explicit Value(TypeID const& type_id)
        : type_id(type_id), v()
    {}

    template<class T>
    explicit Value(TypeID const& type_id, T const& v)
        : type_id(type_id), v(v)
    {}

    Value& operator=(Value&&) noexcept = default;
    Value& operator=(Value const&) = default;

    void swap(Value& rhs) noexcept
    {
        using std::swap;

        if (this != std::addressof(rhs)) {
            swap(type_id, rhs.type_id);
            swap(v, rhs.v);
        }
    }
};

inline bool operator==(Value const& lhs, Value const& rhs) noexcept
{
    return (lhs.type_id == rhs.type_id) && (lhs.v == rhs.v);
}

namespace detail {

struct ValuePrinter : boost::static_visitor<std::ostream&>
{
    ValuePrinter(std::ostream& os) noexcept
        : os_(os)
    {}

    // for UndecidedType
    std::ostream& operator()(
        BOOST_PP_SEQ_HEAD(SAYA_IKA_VM_INTERNAL_TYPEMAP_TYPEONLY) const*
    ) const
    {
        return os_ << make_undecided();
    }

#define SAYA_DEF(r, _, elem) \
    std::ostream& operator()(elem const& raw) const \
    { \
        return os_ << raw; \
    }

    // for everything else
    BOOST_PP_SEQ_FOR_EACH(SAYA_DEF, _, BOOST_PP_SEQ_TAIL(SAYA_IKA_VM_INTERNAL_TYPEMAP_TYPEONLY))
#undef SAYA_DEF

private:
    std::ostream& os_;
};

} // detail

inline std::ostream& operator<<(std::ostream& os, Value const& vv)
{
    return boost::apply_visitor(detail::ValuePrinter(os), vv.v);
}

#include "saya/ika/vm/internal_undef.hpp"

}}} // saya

#endif
