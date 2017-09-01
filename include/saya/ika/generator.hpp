#ifndef SAYA_IKA_GENERATOR_HPP
#define SAYA_IKA_GENERATOR_HPP

#include "saya/logger/logger_env.hpp"

#include <string>
#include <memory>


namespace saya { namespace ika {

class generator
{
public:
    using buf_type = std::string;

    explicit generator(saya::logger_env l_env);
    ~generator() = default;

    generator(generator const&) = delete;
    generator(generator&&) noexcept = delete;

    generator& operator=(generator const&) = delete;
    generator& operator=(generator&&) noexcept = delete;

    std::unique_ptr<buf_type> generate_html() const;

private:
    saya::logger_env const l_env_;
};

}} // saya


#endif
