#ifndef SAYA_IKA_AST_ADDITIONAL_CLASS_HPP
#define SAYA_IKA_AST_ADDITIONAL_CLASS_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

#include <boost/range/adaptor/transformed.hpp>

#include <vector>


namespace saya { namespace ika { namespace ast {

struct AdditionalClass
{
    std::vector<std::string> classes;

    AdditionalClass() = default;
    explicit AdditionalClass(std::vector<std::string> const& classes)
        : classes(classes)
    {}
};

inline std::ostream& operator<<(std::ostream& os, AdditionalClass const& v)
{
    if (v.classes.empty()) {
        return debug::fixed_empty(os, "AdditionalClass");
    }

    return debug::with(
        os,
        "AdditionalClass",
        debug::kv("classes", v.classes | boost::adaptors::transformed([] (auto const& c) { return debug::id_arg("." + c); }))
    );
}

}}} // saya

#endif
