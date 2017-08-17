#ifndef SAYA_IKA_ERROR_HPP
#define SAYA_IKA_ERROR_HPP

#include <stdexcept>

namespace saya { namespace ika {

struct compile_error : std::invalid_argument
{
    compile_error(std::string const& reason)
        : std::invalid_argument(reason)
    {}
};

struct eval_error : std::invalid_argument
{
    using std::invalid_argument::invalid_argument;
};

}} // saya

#endif
