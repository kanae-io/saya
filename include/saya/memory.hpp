#ifndef SAYA_MEMORY_HPP_
#define SAYA_MEMORY_HPP_

#include <memory>

namespace saya {

template<class T, class Deleter>
struct c_deleter
{
    constexpr c_deleter(Deleter d) : d_(d) {}

    constexpr auto operator()(T* ptr_copy) const
        -> decltype(std::declval<Deleter>()(&ptr_copy))
    {
        return d_(&ptr_copy);
    }

private:
    Deleter const d_;
};

} // saya

#endif
