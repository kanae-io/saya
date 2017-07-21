#ifndef SAYA_GUMBO_HELPERS_HPP_
#define SAYA_GUMBO_HELPERS_HPP_

#include "saya/gumbo/common.hpp"
#include "saya/gumbo/error.hpp"
#include "saya/gumbo/make_range.hpp"

#include <boost/algorithm/string/trim_all.hpp>

namespace saya { namespace gumbo {

inline GumboAttribute const* at_attr(GumboNode const* node, std::string const& name)
{
    if (node->type != GUMBO_NODE_ELEMENT) throw is_not_an_element_error(node);

    for (auto const* attrs : saya::gumbo::make_range<GumboAttribute>(node)) {
        if (attrs->name == name) return attrs;
    }

    throw not_found_error("Attribute \"" + name + "\" not found");
}

namespace detail {

inline GumboNode const* at_tag_and_attr_value_impl(GumboNode const* node, GumboTag const tag, std::string const& attr, std::string const& value)
{
    if (node->type == GUMBO_NODE_ELEMENT) {
        if (node->v.element.tag == tag) {
            for (auto const* attrs : saya::gumbo::make_range<GumboAttribute>(node)) {
                if (attrs->name == attr && boost::algorithm::trim_all_copy(std::string(attrs->value)) == value) {
                    return node;
                }
            }
        }

        for (auto const* child : saya::gumbo::make_range<GumboNode>(node)) {
            auto const* found_child = at_tag_and_attr_value_impl(child, tag, attr, value);
            if (found_child) return found_child;
        }
    }
    return nullptr;
}

} // detail

inline GumboNode const* at_tag_and_attr_value(GumboNode const* node, GumboTag const tag, std::string const& attr, std::string const& value)
{
    auto const* found = detail::at_tag_and_attr_value_impl(node, tag, attr, value);

    if (!found) {
        throw not_found_error(std::string("Tag <") + gumbo_normalized_tagname(tag) + "> with attribute " + attr + "=\"" + value + "\" not found");
    }

    return found;
}

}} // saya

#endif
