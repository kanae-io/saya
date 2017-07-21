#ifndef SAYA_STRING_SET_HPP_
#define SAYA_STRING_SET_HPP_

#include "saya/regex.hpp"
#include "saya/algorithm/string/set_string.hpp"

#include <boost/range/algorithm/sort.hpp>
#include <boost/range/algorithm/unique.hpp>

#include <string>

namespace saya {

template<class CharT>
class basic_string_set
{
public:
    typedef basic_string_set<CharT> self_type;
    typedef CharT char_type;
    typedef std::basic_string<char_type> string_type;
    typedef regex_traits<char_type> regex_traits_type;

    explicit basic_string_set(string_type const& str)
        : set_str_(str)
    {
        boost::unique(boost::sort(this->set_str_));
    }

    // string
    string_type const& internal_string() const { return set_str_; }
    string_type to_string() const { return set_str_; }

    // regex
    string_type to_regex() const { return regex_traits_type::brace(set_str_); } // TODO: escape regex
    string_type to_regex_not() const { return regex_traits_type::brace_not(set_str_); }

    self_type& operator+=(self_type const& rhs)
    {
        this->set_str_ += rhs.set_str_;
        unique_set(this->set_str_);
        return *this;
    }
    self_type& operator+=(string_type const& rhs)
    {
        this->set_str_ += rhs;
        unique_set(this->set_str_);
        return *this;
    }

    friend self_type operator==(self_type const& lhs, self_type const& rhs)
    {
        return lhs.set_str_ == rhs.set_str_;
    }
    friend self_type operator!=(self_type const& lhs, self_type const& rhs)
    {
        return !(lhs == rhs);
    }

    friend self_type operator+(self_type const& lhs, string_type const& rhs)
    {
        return self_type(
            unique_set_copy(lhs.set_str_ + rhs)
        );
    }
    friend self_type operator+(string_type const& lhs, self_type const& rhs)
    {
        return rhs + lhs;
    }

    friend self_type operator+(self_type const& lhs, self_type const& rhs)
    {
        return self_type(
            unique_set_copy(lhs.set_str_ + rhs.set_str_)
        );
    }

private:
    string_type set_str_;
};
typedef basic_string_set<char> string_set;
typedef basic_string_set<wchar_t> wstring_set;

} // saya
#endif
