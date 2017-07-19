#ifndef SAYA_SYSTEM_HPP_
#define SAYA_SYSTEM_HPP_

#include "boost/filesystem/path.hpp"
#include "boost/optional.hpp"
#include <cstdlib>

namespace saya {

inline boost::optional<std::string> getenv(std::string const& name)
{
    if (auto const* const e = std::getenv(name.c_str())) {
        std::string const dir(e);

        if ((dir.front() == '"' && dir.back() == '"') || (dir.front() == '\'' && dir.back() == '\'')) {
            if (dir.size() == 2) {
                return boost::none;
            } else {
                return std::string(dir.begin() + 1, dir.end() - 1);
            }
        } else {
            return dir;
        }
    } else {
        return boost::none;
    }
}

inline boost::optional<boost::filesystem::path> getenv_path(std::string const& name)
{
    if (auto const& p = ::saya::getenv(name)) {
        return boost::filesystem::path(*p);
    } else {
        return boost::none;
    }
}

} // saya

#endif
