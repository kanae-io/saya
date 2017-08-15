#include "saya/ika/grammar/to_html.hpp"

#include <boost/phoenix.hpp>
#include <boost/spirit/include/karma.hpp>
#include <boost/spirit/include/support.hpp>


namespace saya { namespace ika { namespace grammar {


class to_html::Impl
{
public:
    explicit Impl(to_html::root_rule_type& root_, saya::logger* const l)
    {
        namespace sp = boost::spirit;
        namespace sl = boost::spirit::labels;
        namespace kr = boost::spirit::karma;
        namespace ns = boost::spirit::karma::standard;
        // namespace zt = saya::zed::terminals;
        namespace phx = boost::phoenix;

        boost::ignore_unused(l);

    } // Impl::Impl

private:

}; // Impl


to_html::to_html(saya::logger* const l)
    : to_html::base_type(root_, "Ika-html")
    , impl_(std::make_unique<to_html::Impl>(root_, l))
{}

// pimpl
to_html::~to_html() = default;

}}} // saya
