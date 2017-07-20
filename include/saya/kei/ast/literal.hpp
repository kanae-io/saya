#ifndef SAYA_KEI_AST_LITERAL_HPP_
#define SAYA_KEI_AST_LITERAL_HPP_

#include "saya/kei_fwd.hpp"
#include "saya/kei/ast/error.hpp"
#include "saya/kei/ast/entity.hpp"

namespace saya { namespace kei { namespace ast {

struct literal_error : ast_error
{

};

class Literal : public Entity
{
public:
    enum class ID : unsigned {};
    virtual ~Literal() = default;

protected:

private:
};

}}} // saya

#endif
