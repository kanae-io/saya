#include "saya/ika/evaluator.hpp"
#include "saya/ika/ast/detail/all.hpp"

#include "saya/console/color.hpp"
#include "saya/type_traits.hpp"

#include <boost/variant/variant.hpp>
#include <boost/variant/static_visitor.hpp>

#include <boost/assert.hpp>

#include <boost/preprocessor/stringize.hpp>
#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#include <iostream>
#include <deque>


namespace {

static inline void
eval_assert(bool cond, char const* expectation)
{
    if (!cond) {
        throw saya::ika::eval_error(expectation);
    }
}

} // anon


namespace saya { namespace ika {
namespace detail { namespace visitor {

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

struct context
{
    explicit context(ast::Root& root, saya::logger* l)
        : root(root)
        , l(l)
    {
        BOOST_ASSERT(l);
        ns_stack_.push_front(root.global_ns());
    }

    void push(ast::Namespace const* v) { BOOST_ASSERT(v); ns_stack_.push_front(v); }
    void pop_ns() { ns_stack_.pop_front(); }
    bool in_global_ns() const noexcept { return ns_stack_.size() == 1; }

    context(context const&) = delete;
    context(context&&) = default;

    context& operator=(context const&) = delete;
    context& operator=(context&&) = default;

    ast::Root& root;
    saya::logger* const l;

private:
    std::deque<ast::Namespace const*> ns_stack_;
};

struct v_ast : boost::static_visitor<void>
{
    explicit v_ast(context& ctx) noexcept
        : ctx_(ctx)
    {}

    void operator()(ast::Root& e) const;
    void operator()(ast::Stmt& e) const;

    void operator()(ast::Expr& e) const;
    void operator()(ast::SideEffectExpr& e) const;
    void operator()(ast::PrimaryExpr& e) const;

    void operator()(ast::Declaration& e) const;

    void operator()(ast::GroupDefinition& e) const;
    void operator()(ast::FuncDefinition& e) const;
    void operator()(ast::MacroDefinition& e) const;

    void operator()(ast::DefaultSpecifier& e) const;

    void operator()(ast::kw::Inherit const*&) const;
    void operator()(ast::kw::Eq const*&) const;

    void operator()(ast::UOp<ast::ops::FuncCall>& e) const;
    void operator()(ast::UOp<ast::ops::MacroCall>& e) const;
    void operator()(ast::UOp<ast::ops::AddFamily>& e) const;

    void operator()(ast::BOp<ast::ops::Pow>& e) const;
    void operator()(ast::BOp<ast::ops::MulFamily>& e) const;
    void operator()(ast::BOp<ast::ops::AddFamily>& e) const;

    void operator()(ast::BOp<ast::ops::Assign>& e) const;

    void operator()(ast::Geo*& ep) const;

    void operator()(ast::Namespace*&) const;
    void operator()(ast::Var*&) const;


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
    context& ctx_;
};


namespace colored {

struct yellow
{
    char const* msg;
};
inline std::ostream& operator<<(std::ostream& os, yellow const& v)
{
    return os << console::color::BOLD() << console::color::fg::YELLOW() << v.msg << console::color::RESET();
}

struct gray
{
    char const* msg;
};
inline std::ostream& operator<<(std::ostream& os, gray const& v)
{
    return os << console::color::BOLD() << console::color::fg::DARKGRAY() << v.msg << console::color::RESET();
}

struct action
{
    char const* msg;
};
inline std::ostream& operator<<(std::ostream& os, action const& v)
{
    return os << console::color::BOLD() << console::color::fg::MAGENTA() << v.msg << console::color::RESET();
}

} // colored

// ------------------------------------------------
#define SAYA_IKA_V_IMPL(impl_ast_elem_, impl_stmt_) \
    void v_ast::operator()(ast::impl_ast_elem_& e) const \
    { \
        { this->ctx_.l->info() << ::saya::console::color::BOLD() << ::saya::console::color::fg::BLUE() << "found" << ::saya::console::color::RESET() << " " << BOOST_PP_STRINGIZE(impl_ast_elem_) << std::endl; } \
        { impl_stmt_ } \
    }

#define SAYA_IKA_V_IMPL_PTR(impl_ast_elem_, impl_stmt_) \
    void v_ast::operator()(ast::impl_ast_elem_*& ep) const \
    { \
        { this->ctx_.l->info() << ::saya::console::color::BOLD() << ::saya::console::color::fg::BLUE() << "found" << ::saya::console::color::RESET() << " " << BOOST_PP_STRINGIZE(impl_ast_elem_) << std::endl; } \
        BOOST_ASSERT(ep); \
        auto& e = *ep; \
        { impl_stmt_ } \
    }

// ------------------------------------------------

SAYA_IKA_V_IMPL(Root, {
    for (auto& stmt : e.stmt_list) {
        auto w = wrap(stmt);
        boost::apply_visitor(v_ast{ctx_}, w);
    }
})

// ------------------------------------------------

SAYA_IKA_V_IMPL(Stmt, {
    boost::apply_visitor(v_ast{ctx_}, e.st);
})

SAYA_IKA_V_IMPL(Expr, {
    boost::apply_visitor(v_ast{ctx_}, e.expr);
})

SAYA_IKA_V_IMPL(SideEffectExpr, {
    boost::apply_visitor(v_ast{ctx_}, e.expr);
})

SAYA_IKA_V_IMPL(PrimaryExpr, {
    boost::apply_visitor(v_ast{ctx_}, e.expr);
})

// ------------------------------------------------

template<class T>
struct v_groupable : boost::static_visitor<void>
{
    explicit v_groupable(T* attr)
        : attr(attr)
    {
        BOOST_ASSERT(attr);
    }

    void operator()(ast::Group* v) const
    {
        BOOST_ASSERT(v);
        (*v)[attr];
    }

    void operator()(ast::Endpoint* v) const
    {
        BOOST_ASSERT(v);
        (*v)[attr];
    }

private:
    T* attr;
};

template<>
struct v_groupable<std::nullptr_t> : boost::static_visitor<void>
{
    void operator()(ast::Group* v) const
    {
        BOOST_ASSERT(v);
        eval_assert(!v->definition, "redefinition of already defined definition");
        v->definition = nullptr;
    }

    void operator()(ast::Endpoint*) const { /* no definition for Endpoint */ }
};


SAYA_IKA_V_IMPL(Declaration, {
    if (!e.is_inline) {
        ctx_.l->info() << colored::gray{"skipped (not inlined)"} << std::endl;
        return;
    }

    ctx_.l->info() << colored::yellow{"inline declaration"} << std::endl;

    {
        ctx_.l->info() << colored::action{"assigning empty definition..."} << std::endl;
        auto const visitor = v_groupable<std::nullptr_t>{};
        boost::apply_visitor(visitor, e.groupable);
    }

    if (!e.attr && !e.additional_class) {
        ctx_.l->info() << colored::gray{"skipped (no viable definition for inline declaration)"} << std::endl;
        return;
    }

    if (e.attr) {
        ctx_.l->info() << colored::action{"assigning attribute..."} << std::endl;

        auto const visitor = v_groupable<ast::Attribute>{*e.attr};
        boost::apply_visitor(visitor, e.groupable);
    }

    if (e.additional_class) {
        ctx_.l->info() << colored::action{"assigning additional classes..."} << std::endl;

        auto const visitor = v_groupable<ast::AdditionalClass>{*e.additional_class};
        boost::apply_visitor(visitor, e.groupable);
    }
})

SAYA_IKA_V_IMPL(GroupDefinition, {
})

SAYA_IKA_V_IMPL(FuncDefinition, {
})

SAYA_IKA_V_IMPL(MacroDefinition, {
})

SAYA_IKA_V_IMPL(DefaultSpecifier, {
})

SAYA_IKA_V_IMPL_PTR(kw::Inherit const, {
})

SAYA_IKA_V_IMPL_PTR(kw::Eq const, {
})

// ------------------------------------------------

SAYA_IKA_V_IMPL(UOp<ast::ops::FuncCall>, {
})

SAYA_IKA_V_IMPL(UOp<ast::ops::MacroCall>, {
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
    if (ctx_.in_global_ns()) {
        ctx_.l->info() << colored::yellow{"global layout definition"} << std::endl;
        ctx_.root.layout = ep;
    }
})

// ------------------------------------------------

SAYA_IKA_V_IMPL_PTR(Namespace, {
    ctx_.push(ep);
    for (auto& stmt : e.stmt_list) {
        auto w = wrap(stmt);
        boost::apply_visitor(v_ast{ctx_}, w);
    }
    ctx_.pop_ns();
})

SAYA_IKA_V_IMPL_PTR(Var, {
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
    detail::visitor::context ctx{root, l_};
    auto w = detail::visitor::wrap(root);
    boost::apply_visitor(detail::visitor::v_ast{ctx}, w);

    eval_assert(root.layout, "Root must have at least one layout definition (i.e. Geo)");
}

}} // saya
