#ifndef SAYA_FILESYSTEM_HPP_
#define SAYA_FILESYSTEM_HPP_

#include "boost/filesystem/path.hpp"

#include <fstream>
#include <stdexcept>

namespace saya {

using path_t = boost::filesystem::path;

template<class CharT>
inline std::basic_string<CharT>
read(path_t const& path)
{
    std::basic_string<char> buf;
    std::basic_ifstream<char> ifs(path.string());

    if (!ifs) {
        throw std::runtime_error("Failed to open file " + path.string());
    }

    ifs.seekg(0, std::ios::end);
    auto const len = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    buf.resize(len);
    ifs.read(&buf[0], len);
    return buf;
}

} // saya

#endif
