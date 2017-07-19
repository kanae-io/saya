#ifndef SAYA_GUMBO_BLADE_HPP_
#define SAYA_GUMBO_BLADE_HPP_

#include "saya/string_config.hpp"
#include "saya/algorithm/any_of.hpp"
#include "saya/encoding/unicode.hpp"
#include "saya/unicode/normalize.hpp"
#include "saya/html/escape.hpp"
#include "saya/gumbo/shaver.hpp"

#include "boost/range/algorithm/sort.hpp"
#include "boost/range/algorithm/unique.hpp"
#include "boost/range/algorithm_ext/erase.hpp"
#include "boost/algorithm/string/split.hpp"
#include "boost/algorithm/string/case_conv.hpp"
#include "boost/regex.hpp"
#include "boost/regex/icu.hpp"

namespace saya { namespace gumbo {

namespace detail {

template<class CharT>
struct sanitizer_blade_traits;

#define DEF(CHAR_TYPE, BINDER) \
    template<> \
    struct sanitizer_blade_traits<CHAR_TYPE> \
    { \
        typedef CHAR_TYPE char_type; \
        typedef char_type const* c_str_type; \
        typedef std::basic_string<char_type> string_type; \
        typedef boost::basic_regex<char_type> regex_type; \
    \
        static regex_type spaces_rgx() { return regex_type{BINDER("\\s+")}; } \
        static saya::ustring single_space() { return saya::to<saya::ustring>(std::string("\u0020")); } \
    };
SAYA_STRING_CONFIG_DEFINE(DEF)
#undef DEF

} // detail

template<class CharT>
class sanitizer_blade : public blade_base
{
public:
    typedef detail::sanitizer_blade_traits<CharT> traits_type;
    typedef typename traits_type::char_type char_type;
    typedef saya::ustring string_type;
    typedef blade_base base_type;
    typedef shaver<char_type> shaver_type;

    using base_type::blade_base;

    virtual void shaver_set(void* shaver) override { shaver_ = static_cast<shaver_type*>(shaver); }

    virtual bool is_permitted_node(GumboNode const& node) const override
    {
        // remove comments whenever possible
        if (node.type == GUMBO_NODE_COMMENT) return false;

        // in syntactic mode...
        if (sanitize_type_ == SANITIZE_SYNTACTIC) {
            // we blacklist some elements which are for visual purpose only,
            // which are, in other words, having no verbatim meaning.
            if (
                node.type == GUMBO_NODE_ELEMENT &&
                saya::any_of_equal(
                    {
                        GUMBO_TAG_SCRIPT, GUMBO_TAG_STYLE
                    },
                    node.v.element.tag
                )
            ) {
                return false;

            } else {
                return true;
            }
        }

        // currently, we should have only 1 type matching this context
        BOOST_ASSERT(sanitize_type_ == SANITIZE_VERBATIM);


        if (node.type == GUMBO_NODE_ELEMENT) {
            if (node.v.element.tag == GUMBO_TAG_UNKNOWN) return false;

            // only <title> is allowed for <head> element
            if (node.parent && node.parent->type == GUMBO_NODE_ELEMENT && node.parent->v.element.tag == GUMBO_TAG_HEAD) {
                return node.v.element.tag == GUMBO_TAG_TITLE;
            }

            // blacklist
            if (is_ignored_ARIA(node)) return false;

            return !saya::any_of_equal(
                {
                    GUMBO_TAG_NAV, // nav.
                    GUMBO_TAG_SUP, // annotations in Wikipedia is in this tag
                    GUMBO_TAG_META,
                    GUMBO_TAG_STYLE,
                    GUMBO_TAG_SCRIPT,
                    GUMBO_TAG_TEMPLATE,
                    GUMBO_TAG_HR,
                    GUMBO_TAG_FIGURE,
                    GUMBO_TAG_FIGCAPTION,
                    GUMBO_TAG_BR,
                    GUMBO_TAG_WBR,
                    GUMBO_TAG_IFRAME,
                    GUMBO_TAG_EMBED,
                    GUMBO_TAG_OBJECT,
                    GUMBO_TAG_PARAM,
                    GUMBO_TAG_VIDEO,
                    GUMBO_TAG_AUDIO,
                    GUMBO_TAG_SOURCE,
                    GUMBO_TAG_TRACK,
                    GUMBO_TAG_CANVAS,
                    GUMBO_TAG_MAP,
                    GUMBO_TAG_AREA,
                    GUMBO_TAG_MATH,
                    GUMBO_TAG_MI,
                    GUMBO_TAG_MO,
                    GUMBO_TAG_MN,
                    GUMBO_TAG_MS,
                    GUMBO_TAG_MTEXT,
                    GUMBO_TAG_MGLYPH,
                    GUMBO_TAG_MALIGNMARK,
                    GUMBO_TAG_ANNOTATION_XML,
                    GUMBO_TAG_SVG,
                    GUMBO_TAG_FOREIGNOBJECT,
                    GUMBO_TAG_APPLET,
                    GUMBO_TAG_BGSOUND,
                    GUMBO_TAG_FRAME,
                    GUMBO_TAG_FRAMESET,
                    GUMBO_TAG_NOFRAMES,
                },
                node.v.element.tag
            );
        }

        return true;
    }
    virtual bool is_permitted_attribute(GumboNode const& /*node*/, GumboAttribute const& /*attr*/) const override
    {
        return true;
    }

    virtual void process(GumboNode const& root) override
    {
        std::basic_string<char_type> str(shaver_->stringize(root));

        if (sanitize_type_ == SANITIZE_VERBATIM) {
            str = saya::html::unescape(str);
        } else {
            str = shaver_->stringize_doctype() + str;
        }

        // ===========================================================
        // unicode start
        str_ = saya::to<string_type>(str);

        // space trimmer.
        // see http://www.fileformat.info/info/unicode/category/index.htm
        if (sanitize_type_ == SANITIZE_SYNTACTIC) {
            // NFC is fine for normal text,
            // but we need to normalize some characters by ourself
            str_ = saya::unicode::normalize<saya::unicode::nfc>(str_);
            str_.istr() = boost::u32regex_replace(
                str_.istr(),
                boost::make_u32regex("(?:[[:Z*:]]|[[:C*:]])+"),
                {"\u0020"}
            );

        } else if (sanitize_type_ == SANITIZE_VERBATIM) {
            // NFKC for verbatim text.
            str_ = saya::unicode::normalize<saya::unicode::nfkc>(str_);
            str_.istr() = boost::u32regex_replace(
                str_.istr(),
                boost::make_u32regex("(?:\u0020|[[:C*:]])+"),
                {"\u0020"}
            );

            if (is_verbatim_ikaros_) {
                str_ = token_based_normalization(str_);
            }

        } else {
            BOOST_ASSERT(false);
        }

        // finally trim.
        str_.istr().trim();
    }

    bool is_verbatim_ikaros() const { return is_verbatim_ikaros_; }
    void is_verbatim_ikaros(bool flag) { is_verbatim_ikaros_ = flag; }

    // ours
    string_type const& str() const { return str_; }

private:
    static string_type token_based_normalization(string_type src)
    {
        // remove all symbols and punctuation.
        src.istr() = boost::u32regex_replace(
            src.istr(),
            boost::make_u32regex("(?:[[:M*:]]|[[:P*:]]|[[:S*:]])+"),
            {"\u0020"}
        );

        // now we perform a further normalization: token based normalization.
        std::vector<string_type> res;
        boost::algorithm::split(res, src, boost::is_any_of(traits_type::single_space()));

        // reject some data
        boost::remove_erase_if(res, [](string_type const& token) -> bool {
            bool remove = false;
            // always remove empty token
            if (token.empty()) remove = true;

            // removing ascii character with length of 1
            // ... is a bad idea.
//            if (token.grapheme_length_lt(2)) {
//                auto const ascii_set(icu::UnicodeSet(icu::UnicodeString("[::]")));
//                auto const ch = token.istr().char32At(0);
//
//                if (ch == ...) {
//
//                }
//            }

            if (remove) {
//                std::cout << "removing token " << token.to_s() << std::endl;
                return true;
            }
            return false;
        });

        return boost::join(res, saya::to<string_type>("\u0020"));
    }

    // see: http://www.w3.org/TR/wai-aria/
    static bool is_ignored_ARIA(GumboNode const& node)
    {
        BOOST_ASSERT(node.type == GUMBO_NODE_ELEMENT);
        if (auto const* const role = gumbo_get_attribute(&node.v.element.attributes, "role")) {

            // see: http://www.w3.org/TR/wai-aria/roles#landmark_roles
            if (saya::any_of_equal({"navigation", "search"}, boost::to_lower_copy(std::string(role->value)))) {
                return true;
            }
        }
        return false;
    }

    shaver_type* shaver_{nullptr};
    bool is_verbatim_ikaros_{false};
    string_type str_;
};

}} // saya

#endif
