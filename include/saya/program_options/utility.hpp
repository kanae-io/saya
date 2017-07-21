#ifndef SAYA_PROGRAM_OPTIONS_UTILITY_HPP
#define SAYA_PROGRAM_OPTIONS_UTILITY_HPP

#include <boost/range/algorithm/sort.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>

#include <string>
#include <vector>

namespace saya { namespace program_options {

// "p,print-option" to "print-option"
inline std::string to_option_id(std::string const& option_name)
{
    std::vector<std::string> option_name_split;
    boost::algorithm::split(option_name_split, option_name, boost::algorithm::is_any_of(","));

    boost::sort(option_name_split, [](std::string const& a, std::string const& b) { return a.size() > b.size(); });
    return option_name_split.front();
}

}} // saya
#endif
