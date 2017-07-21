#ifndef SAYA_IKA_GRAMMAR_TO_AST_HPP
#define SAYA_IKA_GRAMMAR_TO_AST_HPP

// #include "saya/zed/destructor_handler.hpp"

#include "saya/ika/ast/detail/all.hpp"

#include "saya/logger.hpp"

#include <boost/spirit/include/qi_grammar.hpp>
#include <boost/spirit/include/qi_rule.hpp>
#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/support_unused.hpp>

#include <memory>
#include <string>


namespace saya { namespace ika { namespace grammar {

class to_ast
    : public boost::spirit::qi::grammar<
        std::string::const_iterator,
        std::unique_ptr<ast::Root>(),
        boost::spirit::qi::standard::blank_type
    >
{
public:
    using space_type = boost::spirit::qi::standard::space_type;
    using blank_type = boost::spirit::qi::standard::blank_type;

    using iterator_type = std::string::const_iterator;
    using root_type = std::unique_ptr<ast::Root>;
    using root_rule_type = boost::spirit::qi::rule<iterator_type, root_type(), blank_type>;

    explicit to_ast(saya::logger* const l);

    ~to_ast();

    to_ast(to_ast const&) = delete;
    to_ast(to_ast&&) = default;

    to_ast& operator=(to_ast const&) = delete;
    to_ast& operator=(to_ast&&) = default;

private:
    root_rule_type root_;

    class Impl;
    std::unique_ptr<Impl> impl_;
};

}}} // saya

#endif
