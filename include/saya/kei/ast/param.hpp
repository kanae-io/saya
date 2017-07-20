#ifndef SAYA_KEI_AST_PARAM_HPP_
#define SAYA_KEI_AST_PARAM_HPP_

#include "saya/kei_fwd.hpp"


namespace saya { namespace kei { namespace ast {

class EntityParam
{
public:
    enum class ID : unsigned {};

    virtual ~EntityParam() noexcept = default;
    ID pid() const noexcept { return pid_; }

protected:
    explicit EntityParam(ID pid) noexcept
        : pid_(pid)
    {}

private:
    ID const pid_;
};

}}} // saya

#endif
