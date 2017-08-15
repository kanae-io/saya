#include "saya/ika/evaluator.hpp"
#include "saya/ika/ast/detail/all.hpp"

#include "saya/type_traits.hpp"

#include <boost/variant/variant.hpp>
#include <boost/variant/static_visitor.hpp>

#include <boost/assert.hpp>

#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>


namespace saya { namespace ika {
namespace detail { namespace visitor {

static inline void
eval_assert(bool cond, char const* expectation)
{
    if (!cond) {
        throw eval_error(expectation);
    }
}

template<class T, class Enabled = void>
struct variant_wrapper;

template<class T>
struct variant_wrapper<T, std::enable_if_t<!saya::is_variant_v<T>>>
{
    variant_wrapper(T& non_variant)
        : v(non_variant)
    {}

    template<class Visitor>
    void apply_visitor(Visitor const& visitor)
    {
        return visitor(v);
    }

    T& v;
};

template<class T>
struct variant_wrapper<T, std::enable_if_t<saya::is_variant_v<T>>>
{
    variant_wrapper(T& variant_)
        : v(variant_)
    {}

    template<class Visitor>
    void apply_visitor(Visitor const& visitor)
    {
        return v.apply_visitor(visitor);
    }

    T& v;
};

template<class T>
variant_wrapper<T>
wrap(T& v) { return {v}; }


struct v_ast : boost::static_visitor<void>
{
    explicit v_ast(saya::logger* l) noexcept
        : l_(l)
    {}

    void operator()(ast::Root& e) const;
    void operator()(ast::Stmt& e) const;

    void operator()(ast::Expr& e) const;
    void operator()(ast::PrimaryExpr& e) const;

    void operator()(ast::Declaration& e) const;

    void operator()(ast::GroupDefinition& e) const;
    void operator()(ast::FuncDefinition& e) const;
    void operator()(ast::MacroDefinition& e) const;

    void operator()(ast::DefaultSpecifier& e) const;

    void operator()(ast::kw::Inherit const*&) const {}
    void operator()(ast::kw::Eq const*&) const {}

    void operator()(ast::UOp<ast::ops::FuncCall>& e) const;
    void operator()(ast::UOp<ast::ops::AddFamily>& e) const;

    void operator()(ast::BOp<ast::ops::Pow>& e) const;
    void operator()(ast::BOp<ast::ops::MulFamily>& e) const;
    void operator()(ast::BOp<ast::ops::AddFamily>& e) const;

    void operator()(ast::BOp<ast::ops::Assign>& e) const;

    void operator()(ast::Geo*& ep) const;

    void operator()(ast::Var*&) const {}


    #include "saya/ika/vm/internal_def.hpp"

    #define SAYA_DEF_I(vtmapid, lit_name, need_pointer) \
        void operator()(ast::lit::lit_name SAYA_IKA_VM_LIT_NEED_POINTER_FOR(need_pointer)& e) const;

    #define SAYA_DEF(r, data, elem) \
        SAYA_DEF_I elem

    BOOST_PP_SEQ_FOR_EACH(SAYA_DEF, _, SAYA_IKA_VM_LIT_TYPEMAP)
    #undef SAYA_DEF_I
    #undef SAYA_DEF

    #include "saya/ika/vm/internal_undef.hpp"

private:
    saya::logger* const l_;
};

// ------------------------------------------------
#define SAYA_IKA_V_IMPL(impl_ast_elem_, impl_stmt_) \
    void v_ast::operator()(ast::impl_ast_elem_& e) const \
    { \
        { this->l_->info() << "found '" << BOOST_PP_STRINGIZE(impl_ast_elem_) << "'" << std::endl; } \
        { impl_stmt_ } \
    }

#define SAYA_IKA_V_IMPL_PTR(impl_ast_elem_, impl_stmt_) \
    void v_ast::operator()(ast::impl_ast_elem_*& ep) const \
    { \
        { this->l_->info() << "found '" << BOOST_PP_STRINGIZE(impl_ast_elem_) << "'" << std::endl; } \
        BOOST_ASSERT(ep); \
        auto& e = *ep; \
        { impl_stmt_ } \
    }

// ------------------------------------------------

SAYA_IKA_V_IMPL(Root, {
    for (auto& stmt : e.stmt_list) {
        auto w = wrap(stmt);
        boost::apply_visitor(v_ast{l_}, w);
    }
})

// ------------------------------------------------

SAYA_IKA_V_IMPL(Stmt, {
    boost::apply_visitor(v_ast{l_}, e.st);
})

SAYA_IKA_V_IMPL(Expr, {
    boost::apply_visitor(v_ast{l_}, e.expr);
})

SAYA_IKA_V_IMPL(PrimaryExpr, {
})

// ------------------------------------------------

SAYA_IKA_V_IMPL(Declaration, {
})

SAYA_IKA_V_IMPL(GroupDefinition, {
})

SAYA_IKA_V_IMPL(FuncDefinition, {
})

SAYA_IKA_V_IMPL(MacroDefinition, {
})

SAYA_IKA_V_IMPL(DefaultSpecifier, {
})

// ------------------------------------------------

SAYA_IKA_V_IMPL(UOp<ast::ops::FuncCall>, {
})

SAYA_IKA_V_IMPL(UOp<ast::ops::AddFamily>, {
})

SAYA_IKA_V_IMPL(BOp<ast::ops::Pow>, {
})

SAYA_IKA_V_IMPL(BOp<ast::ops::MulFamily>, {
})

SAYA_IKA_V_IMPL(BOp<ast::ops::AddFamily>, {
})

SAYA_IKA_V_IMPL(BOp<ast::ops::Assign>, {
})

// ------------------------------------------------

SAYA_IKA_V_IMPL_PTR(Geo, {
})

// ------------------------------------------------

SAYA_IKA_V_IMPL_PTR(lit::String, {

})

SAYA_IKA_V_IMPL(lit::Int64, {

})

SAYA_IKA_V_IMPL(lit::UInt64, {

})

SAYA_IKA_V_IMPL(lit::Real, {

})

SAYA_IKA_V_IMPL(lit::Color, {

})

SAYA_IKA_V_IMPL(lit::Px, {
    eval_assert(e.v <= 9999, "Px must be less than 9999px");
})

SAYA_IKA_V_IMPL(lit::Pct, {

})

// ------------------------------------------------------

#undef SAYA_IKA_V_IMPL
#undef SAYA_IKA_V_IMPL_PTR

}} // detail

// ------------------------------------------------------
// ------------------------------------------------------

void evaluator::eval(ast::Root& root)
{
    auto w = detail::visitor::wrap(root);
    boost::apply_visitor(detail::visitor::v_ast{l_}, w);
}

}} // saya
