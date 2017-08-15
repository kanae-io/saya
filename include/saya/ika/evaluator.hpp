#ifndef SAYA_IKA_EVALUATOR_HPP
#define SAYA_IKA_EVALUATOR_HPP

#include "saya/ika/ast_fwd.hpp"

#include "saya/logger.hpp"

#include <stdexcept>


namespace saya { namespace ika {

struct eval_error : std::invalid_argument
{
    using std::invalid_argument::invalid_argument;
};

class evaluator
{
public:
    explicit evaluator(saya::logger* l)
        : l_(l)
    {}
    ~evaluator() = default;

    evaluator(evaluator const&) = delete;
    evaluator(evaluator&&) noexcept = default;

    evaluator& operator=(evaluator const&) = delete;
    evaluator& operator=(evaluator&&) noexcept = default;

    void eval(ast::Root& root);

private:
    saya::logger* const l_;
};

}} // saya

#endif
