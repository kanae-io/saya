#ifndef SAYA_IKA_EVALUATOR_HPP
#define SAYA_IKA_EVALUATOR_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/error.hpp"

#include "saya/logger/logger_env.hpp"

#include <stdexcept>


namespace saya { namespace ika {

class evaluator
{
public:
    explicit evaluator(saya::logger_env l_env)
        : l_env_(std::move(l_env))
    {}
    ~evaluator() = default;

    evaluator(evaluator const&) = delete;
    evaluator(evaluator&&) noexcept = delete;

    evaluator& operator=(evaluator const&) = delete;
    evaluator& operator=(evaluator&&) noexcept = delete;

    void eval(ast::Root& root);

private:
    saya::logger_env const l_env_;
};

}} // saya

#endif
