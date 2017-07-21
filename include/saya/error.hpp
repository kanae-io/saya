#ifndef SAYA_ERROR_HPP
#define SAYA_ERROR_HPP

#include <stdexcept>

namespace saya {

struct io_error : std::runtime_error
{
    using std::runtime_error::runtime_error;
};

} // saya

#endif
