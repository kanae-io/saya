#ifndef SAYA_PROGRAM_OPTIONS_FILESYSTEM_PATH_HPP_
#define SAYA_PROGRAM_OPTIONS_FILESYSTEM_PATH_HPP_

#include <boost/any.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/program_options/value_semantic.hpp>

#include <vector>

namespace saya { namespace program_options {

inline void validate(boost::any& v, std::vector<std::string> const& values, boost::filesystem::path*, int)
{
    std::string const& str = boost::program_options::validators::get_single_string(values);
    v = boost::filesystem::path(str);
}

inline void validate(boost::any& v, std::vector<std::string> const& values, std::vector<boost::filesystem::path>*, int)
{
    using namespace boost::program_options;
    typedef std::vector<boost::filesystem::path> t_type;

    if (v.empty()) {
        v = t_type();
    }

    t_type* p = boost::any_cast<t_type>(&v);
    BOOST_ASSERT(p);

    for (std::string const& v : values) {
        p->push_back(boost::filesystem::path(v));
    }
}

} } // saya

#endif
