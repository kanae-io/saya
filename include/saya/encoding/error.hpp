#ifndef SAYA_ENCODING_ERROR_HPP_
#define SAYA_ENCODING_ERROR_HPP_

#include "boost/format.hpp"

#include <unicode/utypes.h> // u_errorName()

#include <stdexcept>

#if !defined(SAYA_DONT_COMPLAIN_ABOUT_ICU)
# if !defined(UNISTR_FROM_STRING_EXPLICIT)
  static_assert(false, "You must define UNISTR_FROM_STRING_EXPLICIT to \"explicit\"");
# endif
#endif

namespace saya {

struct unicode_error : std::runtime_error
{
    unicode_error(int const errno_)
        : std::runtime_error(boost::str(boost::format("Unicode error (ICU errno: %d)") % errno_))
    {}
};

} // saya

#endif
