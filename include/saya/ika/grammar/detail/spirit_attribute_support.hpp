#ifndef SAYA_IKA_GRAMMAR_DETAIL_SPIRIT_ATTRIBUTE_SUPPORT_HPP
#define SAYA_IKA_GRAMMAR_DETAIL_SPIRIT_ATTRIBUTE_SUPPORT_HPP

#include "saya/ika/ast/io_fwd.hpp"

#include <boost/spirit/include/support_attributes.hpp>
#include <boost/assert.hpp>

#include <memory>
#include <type_traits>


template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Namespace*>
{
    static void call(Out& out, saya::ika::ast::Namespace const* val)
    {
        using ::saya::ika::ast::io::operator<<;
        if (val) {
            out << *val;
        } else {
            out << "(invalid_namespace_ptr)";
        }
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Var*>
{
    static void call(Out& out, saya::ika::ast::Var const* val)
    {
        using ::saya::ika::ast::io::operator<<;
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
        using ::saya::ika::ast::io::operator<<;
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
        using ::saya::ika::ast::io::operator<<;
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
        using ::saya::ika::ast::io::operator<<;
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
        using ::saya::ika::ast::io::operator<<;
        if (val) {
            out << *val;
        } else {
            out << "(invalid_endpoint_ptr)";
        }
    }
};


template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Block>
{
    static void call(Out& out, saya::ika::ast::Block const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::CallParam>
{
    static void call(Out& out, saya::ika::ast::CallParam const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::NSID>
{
    static void call(Out& out, saya::ika::ast::NSID const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::VarID>
{
    static void call(Out& out, saya::ika::ast::VarID const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::FuncID>
{
    static void call(Out& out, saya::ika::ast::FuncID const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::MacroID>
{
    static void call(Out& out, saya::ika::ast::MacroID const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::GroupID>
{
    static void call(Out& out, saya::ika::ast::GroupID const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::EndpointID>
{
    static void call(Out& out, saya::ika::ast::EndpointID const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::UOp<saya::ika::ast::ops::FuncCall>>
{
    static void call(Out& out, saya::ika::ast::UOp<saya::ika::ast::ops::FuncCall> const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::UOp<saya::ika::ast::ops::MacroCall>>
{
    static void call(Out& out, saya::ika::ast::UOp<saya::ika::ast::ops::MacroCall> const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::UOp<saya::ika::ast::ops::Not>>
{
    static void call(Out& out, saya::ika::ast::UOp<saya::ika::ast::ops::Not> const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::UOp<saya::ika::ast::ops::AddFamily>>
{
    static void call(Out& out, saya::ika::ast::UOp<saya::ika::ast::ops::AddFamily> const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::BOp<saya::ika::ast::ops::Pow>>
{
    static void call(Out& out, saya::ika::ast::BOp<saya::ika::ast::ops::Pow> const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::BOp<saya::ika::ast::ops::MulFamily>>
{
    static void call(Out& out, saya::ika::ast::BOp<saya::ika::ast::ops::MulFamily> const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::BOp<saya::ika::ast::ops::AddFamily>>
{
    static void call(Out& out, saya::ika::ast::BOp<saya::ika::ast::ops::AddFamily> const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::BOp<saya::ika::ast::ops::Assign>>
{
    static void call(Out& out, saya::ika::ast::BOp<saya::ika::ast::ops::Assign> const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out, class T>
struct boost::spirit::traits::print_attribute_debug<Out, T, typename std::enable_if_t<std::is_base_of<saya::ika::ast::LiteralEntity, T>::value>>
{
    static void call(Out& out, T const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out, class T>
struct boost::spirit::traits::print_attribute_debug<Out, T*, typename std::enable_if_t<std::is_base_of<saya::ika::ast::LiteralEntity, T>::value>>
{
    static void call(Out& out, T const* val)
    {
        using ::saya::ika::ast::io::operator<<;
        BOOST_ASSERT(val);
        out << *val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Declaration>
{
    static void call(Out& out, saya::ika::ast::Declaration const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Stmt>
{
    static void call(Out& out, saya::ika::ast::Stmt const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        out << val;
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, saya::ika::ast::Root*>
{
    static void call(Out& out, saya::ika::ast::Root const* val)
    {
        using ::saya::ika::ast::io::operator<<;
        if (val) {
            out << saya::console::color::fg::BLUE() << saya::console::color::BOLD() << "Context" << saya::console::color::RESET();
        } else {
            out << "(invalid_root_ctx)";
        }
    }
};

template<class Out>
struct boost::spirit::traits::print_attribute_debug<Out, std::unique_ptr<saya::ika::ast::Root>>
{
    static void call(Out& out, std::unique_ptr<saya::ika::ast::Root> const& val)
    {
        using ::saya::ika::ast::io::operator<<;
        if (val) {
            boost::spirit::traits::print_attribute(out, *val);
        } else {
            out << "(invalid_unique_ptr)";
        }
    }
};

#endif
