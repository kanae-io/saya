#ifndef SAYA_NORMALIZER_HPP_
#define SAYA_NORMALIZER_HPP_

#include "saya/string.hpp"
#include "saya/regex.hpp"

namespace saya { namespace normalizer {

template<class CharT>
inline std::basic_string<CharT>
space(std::basic_string<CharT> const& str)
{
    typedef saya::regex_traits<CharT> regex_traits;
    typedef saya::string_traits<CharT> string_traits;

    return boost::regex_replace(
        str,
        typename regex_traits::regex_type(
            regex_traits::plus(
                string_traits::space_set().to_regex()
            )
        ),
        string_traits::space()
    );
} // space

template<class CharT>
inline std::basic_string<CharT>
bar(std::basic_string<CharT> const& str, std::basic_string<CharT> const& bar_str = string_traits<CharT>::bar())
{
    typedef regex_traits<CharT> regex_traits;
    typedef string_traits<CharT> string_traits;

    return boost::regex_replace(
        str,
        typename regex_traits::regex_type(
            string_traits::tilde_set().to_regex()
        ),
        bar_str
    );
}

template<class CharT>
inline std::basic_string<CharT>
zenkaku_katakana(std::basic_string<CharT> const& str)
{
    typedef string_traits<CharT> string_traits;
    return tr_copy<CharT>(str, string_traits::hankaku_katakana(), string_traits::zenkaku_katakana());
}

template<class CharT>
inline std::basic_string<CharT>
hiragana(std::basic_string<CharT> const& str)
{
    typedef string_traits<CharT> string_traits;
    return tr_copy<CharT>(
        zenkaku_katakana(str),
        string_traits::katakana_tr_set(), string_traits::hiragana_tr_set()
    );
}
template<class CharT>
inline std::basic_string<CharT>
katakana(std::basic_string<CharT> const& str)
{
    typedef string_traits<CharT> string_traits;
    return tr_copy<CharT>(
        zenkaku_katakana(str),
        string_traits::hiragana_tr_set(), string_traits::katakana_tr_set()
    );
}

template<class CharT>
inline std::basic_string<CharT>
number(std::basic_string<CharT> const& str)
{
    return tr_copy<CharT>(str, string_traits<CharT>::zenkaku_number_set(), string_traits<CharT>::number_set());
}

template<class CharT>
inline std::basic_string<CharT>
alpha(std::basic_string<CharT> const& str)
{
    return tr_copy<CharT>(str, string_traits<CharT>::zenkaku_alpha_set(), string_traits<CharT>::alpha_set());
}

template<class CharT>
inline std::basic_string<CharT>
alnum(std::basic_string<CharT> const& str)
{
    return tr_copy<CharT>(str, string_traits<CharT>::zenkaku_alnum_set(), string_traits<CharT>::alnum_set());
}

template<class CharT>
inline std::basic_string<CharT>
hankaku(std::basic_string<CharT> const& str)
{
    return tr_copy<CharT>(str, string_traits<CharT>::zenkaku_alnum_set(), string_traits<CharT>::alnum_set());
}

template<class CharT>
inline std::basic_string<CharT>
yomigana(std::basic_string<CharT> const& str)
{
    typedef string_traits<CharT> string_traits;

    return tr_copy<CharT>(
        katakana(str),
        string_traits::zenkaku_alnum_set(),
        string_traits::alnum_set()
    );
}



}} // saya

#endif
