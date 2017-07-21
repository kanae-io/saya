#ifndef SAYA_IKA_AST_GENERATOR_HPP
#define SAYA_IKA_AST_GENERATOR_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/ika/ast/ast_entity.hpp"

#include <type_traits>

namespace saya { namespace ika { namespace ast {

template<class T>
struct generator;

namespace detail {

template<class T, class CRTP>
class generator_base
{
    static_assert(std::is_base_of<ASTEntity, T>::value, "T must derive from ASTEntity");

public:
    template<class... Args>
    generator_base(Args&&... args)
        : store_(std::forward<Args>(args)...)
    {}

    generator_base(generator_base const&) = delete;
    generator_base(generator_base&&) = default;

    generator_base& operator=(generator_base const&) = delete;
    generator_base& operator=(generator_base&&) = default;

    inline T const operator*() const { return store_; }

    template<class... T>
    inline CRTP& operator[](T&&... ts)
    {
        (static_cast<CRTP*>(this)->append(std::forward<T>(ts)), ...);
        return *static_cast<CRTP*>(this);
    }

    template<class T>
    inline CRTP& operator[](generator<T>&& g)
    {
        static_cast<CRTP*>(this)->append(std::move(g.store_));
        return *static_cast<CRTP*>(this);
    }

protected:
    T store_;
};

} // detail

template<>
struct generator<std::vector<Stmt>> : detail::generator_base<std::vector<Stmt>, generator<std::vector<Stmt>>>
{
    using detail::generator_base::generator_base;
    void append(Stmt&& v)
    {
        store_.emplace_back(std::move(v));
    }
};

template<class T, class... Args>
inline generator<T>
generate(Args&&... args)
{
    return generator<T>(std::forward<Args>(args)...);
}

}}} // saya

#endif
