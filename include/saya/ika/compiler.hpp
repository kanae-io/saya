#ifndef SAYA_IKA_COMPILER_HPP
#define SAYA_IKA_COMPILER_HPP

#include "saya/ika_fwd.hpp"
#include "saya/logger.hpp"

#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_parse.hpp>

#include <stdexcept>


namespace saya { namespace ika {

struct compile_error : std::invalid_argument
{
    compile_error(std::string const& reason)
        : std::invalid_argument(reason)
    {}
};

struct compiler_options
{
    // TBD
};

template<class Grammar>
class compiler
{
public:
    using grammar_type = Grammar;
    using root_type = typename grammar_type::root_type;
    using buf_type = std::string;
    using iterator_type = typename buf_type::const_iterator;
    static_assert(std::is_same<typename Grammar::iterator_type, iterator_type>::value, "iterator types must match");

    explicit compiler(logger* l, compiler_options opts = {}) noexcept
        : l_(l)
        , opts_(std::move(opts))
        , gr_(l_)
    {}

    compiler(compiler const&) = delete;
    compiler(compiler&&) = delete;

    compiler& operator=(compiler const&) = delete;
    compiler& operator=(compiler&&) = delete;

    // -----------------------------------------

    void set_buf(buf_type const* buf) noexcept
    {
        buf_ = buf;
    }

    inline root_type compile() const
    {
        if (!buf_ || buf_->empty()) {
            throw std::invalid_argument("input buffer should not be empty");
        }

        typename grammar_type::root_type root;
        auto first = std::cbegin(*buf_);
        auto const last = std::cend(*buf_);

        auto const success = boost::spirit::qi::phrase_parse(
            first, last,
            gr_,
            boost::spirit::qi::standard::blank,
            root
        );

        root->debug(l_->note());

        if (!success) {
            throw compile_error("parse failed");
        }

        if (!(first == last)) {
            throw compile_error("unprocessed characters remain");
        }

        l_->info() << "parse success";
        return root;
    }

private:
    /* mutable */ logger* const l_;
    compiler_options const opts_;
    Grammar const gr_;

    std::string const* buf_{nullptr};
};

}} // saya

#endif
