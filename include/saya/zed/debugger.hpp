#ifndef SAYA_ZED_DEBUGGER_HPP
#define SAYA_ZED_DEBUGGER_HPP

#include <boost/format.hpp>

#include <boost/phoenix/bind/bind_function_object.hpp>
#include <boost/spirit/include/qi_nonterminal.hpp>

#include <memory>
#include <string>
#include <iosfwd>
#include <stdexcept>
#include <type_traits>


namespace saya { namespace zed {

struct parse_error : std::invalid_argument
{
    using std::invalid_argument::invalid_argument;
};

namespace detail {

struct error_handler_impl
{
    template<class, class, class, class>
    struct result { using type = void; };

    template<class B, class E, class W, class I>
    void operator()(
        B const _begin, E const _end,
        W const _where, I const& _info
    ) const {
        static std::string const indent("  ");

        auto line_begin_it = _begin;
        for (auto it = _where; it != _begin; --it) {
            if (it != _begin && *(it - 1) == '\n') {
                line_begin_it = it;
                break;
            }
        }

        auto line_end_it = _end;
        for (auto it = _where; it != _end; ++it) {
            if ((*it) == '\n') {
                line_end_it = it;
                break;
            }
        }

        std::size_t marker_spacer_count = 0;
        for (auto it = line_begin_it; it != _where; ++it) {
            // marker_spacer_count += std::max((*it), 0);
            ++marker_spacer_count;
        }

        std::string marker(marker_spacer_count, ' ');
        marker += "^";

        throw parse_error(boost::str(
            boost::format("Error! Expecting %s here:\n%s%s\n%s%s")
                % _info
                % indent
                % std::string(line_begin_it, line_end_it) // iterators to error-pos, end
                % indent
                % marker
        ));
    }
};

struct debugger
{
    static inline constexpr unsigned INDENT_BASE() noexcept { return 2u; }
    static inline constexpr unsigned PRINT_SOME() noexcept { return 20u; }
    static std::ostream& out() noexcept { return std::cerr; }

    unsigned& get_indent() const
    {
        static unsigned indent = 0u;
        return indent;
    }

    void print_indent(unsigned n) const
    {
        for (unsigned i = 0u, max = n * INDENT_BASE(); i != max; ++i) {
            out() << ' ';
        }
    }

    template<class Iterator>
    void print_some(
        char const* tag, unsigned /*indent*/,
        Iterator first, Iterator const& last
    ) const {
        print_indent(get_indent());
        out() << '<' << tag << '>';

        for (unsigned i = 0; first != last && i != PRINT_SOME() && *first; ++i, ++first) {
            boost::spirit::traits::print_token(out(), *first);
        }
        out() << "</" << tag << '>' << std::endl;
    }

    template<class Iterator, class Context, class State>
    void operator()(
        Iterator const& first,
        Iterator const& last,
        Context const& context,
        State state,
        std::string const& rule_name
    ) const {
        switch (state) {
        case boost::spirit::qi::pre_parse:
            print_indent(get_indent()++);
            out()
                << '<' << rule_name << '>'
                << std::endl
            ;
            print_some("try", get_indent(), first, last);
            break;

        case boost::spirit::qi::successful_parse:
            print_some("success", get_indent(), first, last);
            print_indent(get_indent());
            out() << "<attributes>";

            boost::spirit::traits::print_attribute(
                out(),
                context.attributes
            );
            out() << "</attributes>";

            if (!boost::fusion::empty(context.locals)) {
                out() <<
                    "<locals>" <<
                    context.locals <<
                    "</locals>"
                ;
            }
            out() << '\n';

            print_indent(--get_indent());
            out() <<
                "</" <<
                rule_name <<
                '>' <<
                std::endl
            ;
            break;

        case boost::spirit::qi::failed_parse:
            print_indent(get_indent());
            out() << "<fail/>" << '\n';

            print_indent(--get_indent());
            out() <<
                "</" <<
                rule_name <<
                '>' <<
                std::endl
            ;
            break;
        }
    }
};

} // detail

template<class B, class E, class W, class I>
static inline constexpr auto
lazy_error_handler(B const& b, E const& e, W const& w, I const& i)
{
    return boost::phoenix::function<detail::error_handler_impl>()(b, e, w, i);
}

template<class Rule>
inline void debug(Rule& r)
{
    boost::spirit::qi::debug(r, detail::debugger());
}

}} // saya

#endif
