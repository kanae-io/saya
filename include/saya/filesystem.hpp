#ifndef SAYA_FILESYSTEM_HPP_
#define SAYA_FILESYSTEM_HPP_

#include "saya/error.hpp"
#include <boost/filesystem.hpp>

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

namespace saya {

template<class T>
inline std::vector<T>
read_bin(boost::filesystem::path const& path)
{
    std::vector<T> buf;
    std::basic_ifstream<char> ifs(path.string());

    if (!ifs) {
        throw io_error("failed to open file '" + path.string() + "'");
    }

    ifs.seekg(0, std::ios::end);
    auto const len = ifs.tellg();
    ifs.clear(); // clear EOF
    ifs.seekg(0, std::ios::beg);

    if (!ifs) {
        throw io_error("failed to seek file '" + path.string() + "'");
    }

    buf.resize(len);
    ifs.read(buf.data(), len);

    if (!ifs) {
        throw io_error("failed to read file '" + path.string() + "'");
    }

    // DONT move...
    return buf;
}

template<class CharT>
inline std::basic_string<CharT>
read(boost::filesystem::path const& path)
{
    std::basic_ifstream<CharT> ifs(path.string());

    if (!ifs) {
        throw io_error("failed to open file '" + path.string() + "'");
    }

    return std::basic_string<CharT>(
        std::istreambuf_iterator<CharT>{ifs},
        std::istreambuf_iterator<CharT>{}
    );
}

} // saya

#endif
