#ifndef SAYA_PROGRAM_OPTIONS_COMMA_SEPARATED_STRING_HPP_
#define SAYA_PROGRAM_OPTIONS_COMMA_SEPARATED_STRING_HPP_

#include "boost/any.hpp"
#include "boost/tokenizer.hpp"
#include "boost/token_functions.hpp"
#include "boost/program_options/value_semantic.hpp"

#include <algorithm>
#include <vector>
#include <string>

namespace saya { namespace program_options {

// strong typedef...
class comma_separated_string : protected std::vector<std::string>
{
public:
    using std::vector<std::string>::iterator;
    using std::vector<std::string>::const_iterator;
    using std::vector<std::string>::value_type;
    using std::vector<std::string>::get_allocator;
    using std::vector<std::string>::max_size;
    using std::vector<std::string>::size;
    using std::vector<std::string>::empty;
    using std::vector<std::string>::capacity;
    using std::vector<std::string>::assign;
    using std::vector<std::string>::begin;
    using std::vector<std::string>::end;
    using std::vector<std::string>::rbegin;
    using std::vector<std::string>::rend;
    using std::vector<std::string>::operator[];
    using std::vector<std::string>::at;
    using std::vector<std::string>::resize;
    using std::vector<std::string>::front;
    using std::vector<std::string>::back;
    using std::vector<std::string>::push_back;
    using std::vector<std::string>::pop_back;
    using std::vector<std::string>::insert;
    using std::vector<std::string>::erase;
    using std::vector<std::string>::clear;
    using std::vector<std::string>::swap;
};

inline void validate(boost::any& v, std::vector<std::string> const& values, comma_separated_string*, int)
{
    using namespace boost::program_options;

    if (v.empty()) {
        v = boost::any(comma_separated_string());
    }

    comma_separated_string* p = boost::any_cast<comma_separated_string>(&v);
    BOOST_ASSERT(p);

    boost::char_separator<char> sep(",");
    for (std::string const& v : values) {
        if (v.find(",")) {
            // tokenize values and push them back onto p->values
            boost::tokenizer<boost::char_separator<char> > tok(v, sep);
            std::copy(tok.begin(), tok.end(), std::back_inserter(*p));

        } else {
            p->push_back(v);
        }
    }
}

} } // saya
#endif

