#ifndef SAYA_IKA_GRAMMAR_TO_HTML_HPP
#define SAYA_IKA_GRAMMAR_TO_HTML_HPP

#include "saya/ika/ast/detail/all.hpp"

#include "saya/logger.hpp"

#include <boost/spirit/include/karma_grammar.hpp>
#include <boost/spirit/include/karma_rule.hpp>

#include <memory>
#include <string>
#include <iterator>


namespace saya { namespace ika { namespace grammar {

class to_html
    : public boost::spirit::karma::grammar<
        std::back_insert_iterator<std::string>, ast::Root()
    >
{
public:
    using string_type = std::string;
    using iterator_type = std::back_insert_iterator<string_type>;
    using root_type = ast::Root;
    using root_rule_type = boost::spirit::karma::rule<iterator_type, root_type()>;

    explicit to_html(saya::logger* const l);

    ~to_html();

    to_html(to_html const&) = delete;
    to_html(to_html&&) = default;

    to_html& operator=(to_html const&) = delete;
    to_html& operator=(to_html&&) = default;

private:
    root_rule_type root_;

    class Impl;
    std::unique_ptr<Impl> impl_;
};

}}} // saya

#endif
