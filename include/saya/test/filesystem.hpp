#ifndef SAYA_TEST_FILESYSTEM_HPP_
#define SAYA_TEST_FILESYSTEM_HPP_
#if defined(SAYA_TESTING)

#include <boost/filesystem/path.hpp>
#include <ostream>

namespace boost { namespace filesystem {

inline void PrintTo(::boost::filesystem::path const& p, ::std::ostream* os)
{
    *os << p.string();
}

}}

#endif
#endif
