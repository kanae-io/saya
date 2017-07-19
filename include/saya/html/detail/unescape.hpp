#ifndef SAYA_HTML_DETAIL_UNESCAPE_HPP_
#define SAYA_HTML_DETAIL_UNESCAPE_HPP_

#include "saya/html/detail/unescape_traits.hpp"
#include "saya/html/detail/escape_common.hpp"
#include "saya/html/detail/named_character_references_table.hpp"

#include "boost/algorithm/string/case_conv.hpp"
#include "boost/algorithm/string/replace.hpp"

#include "boost/lexical_cast.hpp"


namespace saya { namespace html { namespace detail {

template<class CharT>
inline std::basic_string<CharT> unescape_impl(std::basic_string<CharT> src, EscapeFlags flags)
{
    if (src.empty()) return {};
    typedef CharT char_type;
    typedef std::basic_string<char_type> string_type;
    typedef typename string_type::iterator iterator_type;
    typedef detail::named_character_references_traits<char_type> named_character_references_traits_type;
    typedef detail::unescape_traits<char_type> unescape_traits_type;

    boost::match_results<iterator_type> what;
    boost::match_flag_type rgx_flags = boost::match_default;
    iterator_type start = src.begin(), end = src.end();

    // this function must not fail.
    auto const perform_replace = [&](string_type const& replace_to) -> void {
        std::size_t const last_pos_i = std::distance(src.begin(), what[0].first);

//        std::cout << "replacing " << what.str(0) << " to " << replace_to << std::endl;

        src.replace(what[0].first, what[0].second, replace_to);
        start = src.begin() + last_pos_i + replace_to.size();
        end = src.end();
    };

    auto const process = [&]() -> bool {
        string_type const found_str(what.str(0));

        string_type found_code;
        bool is_numeric = false;
        bool is_hexadecimal = false;

        BOOST_ASSERT(what.size() == 5);

        // determine whether it's named, decimal or hexadecimal
        if (what.str(1).empty() && what.str(3).empty()) {
            // named
            is_numeric = false;

        } else {
            is_numeric = true;

            if (!what.str(1).empty()) {
                // hexadecimal
                BOOST_ASSERT(!what.str(2).empty());

                is_hexadecimal = true;
                found_code = what.str(2);

            } else {
                // decimal
                BOOST_ASSERT(!what.str(3).empty());
                BOOST_ASSERT(!what.str(4).empty());

                is_hexadecimal = false;
                found_code = what.str(4);
            }
        }

        try {
            // check for each types
            auto const& table = named_character_references_traits_type::table();

            if (is_numeric) {
                if ((flags & UNESCAPE_DECIMAL) && is_hexadecimal) {
                    UChar32 const cp = std::stoull(unescape_traits_type::hexadecimal_prefix() + found_code, nullptr, 16);
                    perform_replace(saya::to<string_type>(saya::ustring(cp)));
                    return true;

                } else if ((flags & UNESCAPE_DECIMAL) && !is_hexadecimal) {
                    UChar32 const cp = boost::lexical_cast<UChar32>(found_code);
                    perform_replace(saya::to<string_type>(saya::ustring(cp)));
                    return true;
                }
            } else {
                if ((flags & UNESCAPE_NAMED) && table.count(found_str)) {
                    perform_replace(table.at(found_str));
                    return true;
                }
            }

        } catch (std::out_of_range const&) {
            return false;

        } catch (std::invalid_argument const&) {
            return false;

        } catch (boost::bad_lexical_cast const&) {
            return false;
        }
        return false;
    };

    while (boost::regex_search(start, end, what, detail::unescape_traits<char_type>::character_reference_rgx(), rgx_flags)) {
        if (!process()) {
            // skip and continue
            start = what[0].second;
        }
        rgx_flags |= boost::match_prev_avail;
    }

    return src;
}

}}} // saya

#endif
