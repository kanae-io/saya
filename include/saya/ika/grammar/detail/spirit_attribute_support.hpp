#ifndef SAYA_IKA_GRAMMAR_DETAIL_SPIRIT_ATTRIBUTE_SUPPORT_HPP
#define SAYA_IKA_GRAMMAR_DETAIL_SPIRIT_ATTRIBUTE_SUPPORT_HPP

#include "saya/ika/ast/id.hpp"
#include "saya/ika/ast/lit.hpp"

#include <boost/spirit/include/support_attributes.hpp>
#include <boost/assert.hpp>

#include <memory>
#include <type_traits>


template<class Out, class T>
struct boost::spirit::traits::print_attribute_debug<Out, std::unique_ptr<T>>
{
    static void call(Out& out, std::unique_ptr<T> const& val)
    {
        if (val) {
            boost::spirit::traits::print_attribute(out, *val);
        } else {
            out << "(invalid_unique_ptr)";
        }
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::lit::String*>
{
    static void call(Out& out, saya::ika::ast::lit::String const* val)
    {
        BOOST_ASSERT(val);
        boost::spirit::traits::print_attribute(out, *val);
    }
};

#if 0

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Var*>
{
    static void call(Out& out, saya::ika::ast::Var const* val)
    {
        if (val) {
            out << *val;
        } else {
            out << "(invalid_var_ptr)";
        }
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Func*>
{
    static void call(Out& out, saya::ika::ast::Func const* val)
    {
        if (val) {
            out << *val;
        } else {
            out << "(invalid_func_ptr)";
        }
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Macro*>
{
    static void call(Out& out, saya::ika::ast::Macro const* val)
    {
        if (val) {
            boost::spirit::traits::print_attribute(out, *val);
        } else {
            out << "(invalid_macro_ptr)";
        }
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Group*>
{
    static void call(Out& out, saya::ika::ast::Group const* val)
    {
        if (val) {
            out << *val;
        } else {
            out << "(invalid_group_ptr)";
        }
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Endpoint*>
{
    static void call(Out& out, saya::ika::ast::Endpoint const* val)
    {
        if (val) {
            out << *val;
        } else {
            out << "(invalid_endpoint_ptr)";
        }
    }
};

#endif // 0

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Block>
{
    static void call(Out& out, saya::ika::ast::Block const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::CallParam>
{
    static void call(Out& out, saya::ika::ast::CallParam const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::NSID>
{
    static void call(Out& out, saya::ika::ast::NSID const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::VarID>
{
    static void call(Out& out, saya::ika::ast::VarID const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::FuncID>
{
    static void call(Out& out, saya::ika::ast::FuncID const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::MacroID>
{
    static void call(Out& out, saya::ika::ast::MacroID const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::GroupID>
{
    static void call(Out& out, saya::ika::ast::GroupID const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::EndpointID>
{
    static void call(Out& out, saya::ika::ast::EndpointID const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::UOp<saya::ika::ast::ops::FuncCall>>
{
    static void call(Out& out, saya::ika::ast::UOp<saya::ika::ast::ops::FuncCall> const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::UOp<saya::ika::ast::ops::MacroCall>>
{
    static void call(Out& out, saya::ika::ast::UOp<saya::ika::ast::ops::MacroCall> const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::UOp<saya::ika::ast::ops::Not>>
{
    static void call(Out& out, saya::ika::ast::UOp<saya::ika::ast::ops::Not> const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::UOp<saya::ika::ast::ops::AddFamily>>
{
    static void call(Out& out, saya::ika::ast::UOp<saya::ika::ast::ops::AddFamily> const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::BOp<saya::ika::ast::ops::Pow>>
{
    static void call(Out& out, saya::ika::ast::BOp<saya::ika::ast::ops::Pow> const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::BOp<saya::ika::ast::ops::MulFamily>>
{
    static void call(Out& out, saya::ika::ast::BOp<saya::ika::ast::ops::MulFamily> const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::BOp<saya::ika::ast::ops::AddFamily>>
{
    static void call(Out& out, saya::ika::ast::BOp<saya::ika::ast::ops::AddFamily> const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::BOp<saya::ika::ast::ops::Assign>>
{
    static void call(Out& out, saya::ika::ast::BOp<saya::ika::ast::ops::Assign> const& val)
    {
        out << val;
    }
};

template<class Out, class T>
struct boost::spirit::traits::print_attribute_debug<Out, T, typename std::enable_if_t<std::is_base_of<saya::ika::ast::LiteralEntity, T>::value>>
{
    static void call(Out& out, T const& val)
    {
        out << val;
    }
};

template<class Out, class T>
struct boost::spirit::traits::print_attribute_debug<Out, T*, typename std::enable_if_t<std::is_base_of<saya::ika::ast::LiteralEntity, T>::value>>
{
    static void call(Out& out, T const* val)
    {
        BOOST_ASSERT(val);
        out << *val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Stmt>
{
    static void call(Out& out, saya::ika::ast::Stmt const& val)
    {
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Declaration>
{
    static void call(Out& out, saya::ika::ast::Declaration const& val)
    {
        out << val;
    }
};

#endif
