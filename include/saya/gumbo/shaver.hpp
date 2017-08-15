#ifndef SAYA_GUMBO_SHAVER_HPP_
#define SAYA_GUMBO_SHAVER_HPP_

#include "saya/gumbo/common.hpp"
#include "saya/gumbo/make_range.hpp"

#include "saya/string.hpp"
#include "saya/string_config.hpp"
#include "saya/algorithm.hpp"
#include "saya/encoding/format.hpp"
#include "saya/encoding/ustring.hpp"

#include <Poco/Exception.h> // SyntaxException

#include <boost/format.hpp>
#include <boost/optional.hpp>

#include <boost/range/algorithm/find_if.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/range/adaptor/map.hpp>

#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/case_conv.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/cxx11/all_of.hpp>

#include <boost/cast.hpp>

#include <functional>
#include <vector>
#include <unordered_map>
#include <string>
#include <iosfwd>

namespace saya { namespace gumbo {

typedef std::unordered_map<std::string, SanitizeType> sanitize_type_map_type;

inline static sanitize_type_map_type const&
sanitize_type_map()
{
    static sanitize_type_map_type const instance{
       {"none", SANITIZE_NONE},
       {"encoding", SANITIZE_ENCODING},
       {"syntactic", SANITIZE_SYNTACTIC},
       {"verbatim", SANITIZE_VERBATIM},
       {"api", SANITIZE_API},
    };
    return instance;
}

inline boost::optional<SanitizeType> to_type(std::string const& type_str)
{
    if (sanitize_type_map().count(type_str)) {
        return sanitize_type_map().at(type_str);
    } else {
        return boost::none;
    }
}

inline boost::optional<std::string> to_string(SanitizeType sanitize_type)
{
    auto const it = boost::find_if(sanitize_type_map(), [sanitize_type](typename sanitize_type_map_type::value_type const& kv) {
        return kv.second == sanitize_type;
    });

    if (it != sanitize_type_map().end()) {
        return it->first;
    } else {
        return boost::none;
    }
}


inline static
GumboVector const& get_children(GumboNode* node)
{
    return node->v.element.children;
}

struct invalid_html_error : std::exception {};

struct blade_base
{
    blade_base(SanitizeType sanitize_type) : sanitize_type_(sanitize_type) {}

    virtual ~blade_base() {}
    virtual void shaver_set(void* shaver) = 0;
    virtual bool is_permitted_node(GumboNode* node) const = 0;
    virtual bool is_permitted_attribute(GumboNode* node, GumboAttribute* attr) const = 0;
    virtual void process(GumboNode const& root) = 0;

    SanitizeType sanitize_type() const { return sanitize_type_; }

protected:
    SanitizeType sanitize_type_;
};


template<class CharT>
struct shaver_traits;

#undef SAYA_DEF
#define SAYA_DEF(CHAR_TYPE, BINDER) \
    template<> \
    struct shaver_traits<CHAR_TYPE> \
    { \
        typedef CHAR_TYPE char_type; \
        typedef char_type const* blob_type; \
        typedef boost::basic_regex<char_type> regex_type; \
    \
        SAYA_STRING_CONFIG_FUNCTION(regex_type, attr_value_rgx, BINDER("^(['\"])(.*?)\\1?$")) \
        SAYA_STRING_CONFIG_FUNCTION(blob_type, attr_value_to, BINDER("\\2")) \
    };

SAYA_STRING_CONFIG_DEFINE(SAYA_DEF)
#undef SAYA_DEF


template<class CharT>
class shaver
{
public:
    typedef CharT char_type;
    typedef shaver<CharT> self_type;
    typedef std::basic_string<char_type> string_type;
    typedef saya::string_traits<char_type> traits_type;
    typedef shaver_traits<char_type> shaver_traits_type;

    static string_type normalize_attr_name(u8string_ const& name_u8)
    {
        return saya::to<string_type>(
            boost::algorithm::to_lower_copy(saya::to<string_type>(name_u8))
        );
    }

    string_type stringize_attr(GumboAttribute* attr)
    {
        return boost::str(saya::forward_format<char_type>(
            "%s=%s"
        )
            % normalize_attr_name(make_u8string(attr->original_name))
            % saya::to<string_type>(make_u8string(attr->original_value))
        );
    }

    string_type stringize_attrs(GumboNode* node)
    {
        using boost::adaptors::transformed;

        return boost::join(
            saya::gumbo::make_range<GumboAttribute>(node)
                | transformed([this, &node](GumboAttribute* attr) -> string_type {
                    if (blade_->is_permitted_attribute(node, attr)) {
                        return stringize_attr(attr);
                    } else {
                        return {};
                    }
                })
            ,
            traits_type::space()
        );
    }

    string_type stringize_children(GumboNode* node)
    {
        string_type children_join_str;
        if (blade_->sanitize_type() == SANITIZE_VERBATIM) {
            // add spaces between nodes, if shaver mode is VERBATIM
            children_join_str = traits_type::space();
        } else {
        }

        using boost::adaptors::transformed;
        return boost::join(
            saya::gumbo::make_range<GumboNode>(node)
                | transformed(std::bind(&self_type::stringize, this, std::placeholders::_1))
            ,
            children_join_str
        );
    }

    string_type stringize(GumboNode* node)
    {
        if (!blade_->is_permitted_node(node)) return {};

        if (node->type == GUMBO_NODE_ELEMENT) {
            // stringize children
            auto const children_str(stringize_children(node));

            // swap text for elements w/ alternate contents
            if (blade_->sanitize_type() == SANITIZE_VERBATIM) {
                if (node->v.element.tag == GUMBO_TAG_IMG) {
                    if (auto const* attr = gumbo_get_attribute(&node->v.element.attributes, "alt")) {
                        return remove_attr_quotes(attr->original_value);

                    } else {
                        return {};
                    }

                } else if (node->v.element.tag == GUMBO_TAG_A) {
                    if (children_str.empty()) {
                        // We found some nested elements inside the <a> tag,
                        // but the stringized content was empty.
                        // In this case we use the title attribute of the <a> tag.

                        if (auto const* attr = gumbo_get_attribute(&node->v.element.attributes, "title")) {
                            return remove_attr_quotes(attr->original_value);

                        } else {
                            return {};
                        }

                    } else {
                        if (auto const* attr = gumbo_get_attribute(&node->v.element.attributes, "href")) {

                            // skip if the content was exactly the same as the href attribute
                            if (boost::trim_copy(children_str) == remove_attr_quotes(attr->original_value)) {
                                return {};
                            } else {
                                return children_str;
                            }

                        } else {
                            // no href attribute was present.
                            return children_str;
                        }
                    }

                } else {
                    // For ordinary tags, we only need the node content for VERBATIM sanitizer
                    return children_str;
                }
            }

            // stringize attrs
            auto attrs_str = stringize_attrs(node);
            if (!attrs_str.empty()) {
                attrs_str = traits_type::space() + attrs_str;
            }

            // recover the "real" tag
            string_type tagname_begin, tagname_end;

            if (node->v.element.tag != GUMBO_TAG_UNKNOWN) {
                tagname_begin = saya::to<string_type>(gumbo_normalized_tagname(node->v.element.tag));

                if (node->v.element.original_end_tag.length == 0) {
                    // auto-generated self-closing end tag
                } else {
                    tagname_end = tagname_begin;
                }
            } else {
                // unknown tag, such as namespaced original tags
                auto original_tag_copy = node->v.element.original_tag;
                gumbo_tag_from_original_text(&original_tag_copy);

                tagname_begin = saya::to<string_type>(make_u8string(
                    original_tag_copy
                ));
                tagname_end = tagname_begin; // we assume the end tag is never being auto-closed, since the begin tag is unknown to the parser.
            }

            // We seem to have 2 cases:
            // (1) The original tag was not closed by having a corrupted syntax
            // (2) The original tag was not closed since it's a self-closing tag

            // expect (2), but reject (1)
            if (tagname_end.empty() && node->v.element.children.length == 0) {
                if (blade_->sanitize_type() == SANITIZE_VERBATIM) return {};

                return boost::str(
                    saya::forward_format<char_type>(
                        "<%s%s>"
                    )
                    % tagname_begin
                    % attrs_str
                );
            } else {
                if (tagname_end.empty()) tagname_end = tagname_begin;

                return boost::str(
                    saya::forward_format<char_type>(
                        "<%s%s>%s</%s>"
                    )
                    % tagname_begin
                    % attrs_str
                    % children_str
                    % tagname_end
                );

//                % boost::join(
//                    get_children_range(node) | boost::adaptors::transformed(std::bind(&self_type::stringize, this, std::placeholders::_1)),
//                    string_type{}
//                )
            }

            BOOST_ASSERT(false);
            return {};

        } else if (node->type == GUMBO_NODE_TEXT) {
            return saya::to<string_type>(make_u8string(node->v.text.original_text));

        } else if(node->type == GUMBO_NODE_WHITESPACE) {
            if (blade_->sanitize_type() == SANITIZE_SYNTACTIC) {
                return {};
            } else if (blade_->sanitize_type() == SANITIZE_VERBATIM) {
                return traits_type::space();
            } else {
                BOOST_ASSERT(!"not implemented");
            }

        } else if(saya::any_of_equal({GUMBO_NODE_CDATA, GUMBO_NODE_COMMENT}, node->type)) {
            // do nothing
            return {};

        } else {
            std::cout << boost::format("warn: stringize for node type %d is not implemented") % node->type;
        }

        BOOST_ASSERT(false);
        return {};
    }

private:
    static string_type remove_attr_quotes(string_type const& str)
    {
        if (str.empty()) {
            return {};
        }
        return boost::regex_replace(str, shaver_traits_type::attr_value_rgx(), shaver_traits_type::attr_value_to());
    }
    static string_type remove_attr_quotes(GumboStringPiece const& str)
    {
        return remove_attr_quotes(saya::to<string_type>(make_u8string(str)));
    }

public:
    static std::string format_public_identifier(std::string const& identifier)
    {
        if (identifier.empty()) return {};
        return boost::str(
            boost::format("\"%s\"") % identifier
        );
    }

    static std::string format_system_identifier(std::string const& identifier)
    {
        if (identifier.empty()) return {};
        return boost::str(
            boost::format(" \"%s\"") % identifier
        );
    }

    string_type stringize_doctype() const
    {
        BOOST_ASSERT(doc_);

        if (blade_->sanitize_type() == SANITIZE_VERBATIM) return {};
        if (!doc_->has_doctype) return {};

        std::string const public_identifier(format_public_identifier(doc_->public_identifier));
        std::string const system_identifier(format_system_identifier(doc_->system_identifier));


        if (public_identifier.empty() && system_identifier.empty()) {
            return saya::to<string_type>(boost::str(
                boost::format(
                    "<!DOCTYPE %s>\n"
                )
                % doc_->name
            ));

        } else if (public_identifier.empty()) {
            return saya::to<string_type>(boost::str(
                boost::format(
                    "<!DOCTYPE %s SYSTEM%s>\n"
                )
                % doc_->name
                % system_identifier
            ));

        } else {
            return saya::to<string_type>(boost::str(
                boost::format(
                    "<!DOCTYPE %s PUBLIC %s%s>\n"
                )
                % doc_->name
                % public_identifier
                % system_identifier
            ));
        }
    }


    void run(GumboOutput const* output)
    {
        BOOST_ASSERT(output->document->type == GUMBO_NODE_DOCUMENT);

        run_document(output->document->v.document);
        run_root(*output->root);
    }

    void blade(std::unique_ptr<blade_base> blade)
    {
        blade->shaver_set(static_cast<void*>(this));
        blade_ = std::move(blade);
    }

    template<class BladeType>
    BladeType const* blade() const { return blade_.get(); }

    template<class BladeType>
    BladeType* blade() { return boost::polymorphic_downcast<BladeType*>(blade_.get()); }

private:
    void run_document(GumboDocument const& doc)
    {
        doc_ = &doc;
    }

    void run_root(GumboNode const& root) {
        if (!blade_) throw std::logic_error("blade has not been set");
        blade_->process(root);
    }

    GumboDocument const* doc_{nullptr};
    std::unique_ptr<blade_base> blade_;
};


}} // saya

#endif
