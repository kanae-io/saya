#ifndef SAYA_IKA_AST_ADDITIONAL_CLASS_HPP
#define SAYA_IKA_AST_ADDITIONAL_CLASS_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

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

}}} // saya

#endif
