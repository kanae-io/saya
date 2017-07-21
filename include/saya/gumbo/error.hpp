#ifndef SAYA_GUMBO_ERROR_HPP_
#define SAYA_GUMBO_ERROR_HPP_

#include "saya/error.hpp"
#include "saya/gumbo/common.hpp"

#include <boost/format.hpp>

namespace saya { namespace gumbo {

struct gumbo_error : saya_error
{
    using saya_error::saya_error;
};

struct not_found_error : gumbo_error
{
    using gumbo_error::gumbo_error;
};

struct is_not_an_element_error : gumbo_error
{
    is_not_an_element_error(GumboNode const* node)
        : gumbo_error(boost::str(
            boost::format(
                "Specified node is not an element. Node type: %d"
            ) % node->type
          ))
    {}
};

}} // saya

#endif
