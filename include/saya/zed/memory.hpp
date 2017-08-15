#ifndef SAYA_ZED_MEMORY_HPP
#define SAYA_ZED_MEMORY_HPP

#include <boost/phoenix/function.hpp>
#include <memory>

namespace saya { namespace zed {
namespace detail {
template<class T>
struct make_shared_impl
{
    using result_type = std::shared_ptr<T>;

    template<class... Args>
    inline result_type operator()(Args&&... args) const
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
};

template<class T>
struct make_unique_impl
{
    using result_type = std::unique_ptr<T>;

    template<class... Args>
    inline result_type operator()(Args&&... args) const
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
};
} // detail

template<class T, class... Args>
inline auto make_shared(Args&&... args)
    -> decltype(boost::phoenix::function<detail::make_shared_impl<T>>()(std::forward<Args>(args)...))
{
    return boost::phoenix::function<detail::make_shared_impl<T>>()(std::forward<Args>(args)...);
}

template<class T, class... Args>
inline auto make_unique(Args&&... args)
    -> decltype(boost::phoenix::function<detail::make_unique_impl<T>>()(std::forward<Args>(args)...))
{
    return boost::phoenix::function<detail::make_unique_impl<T>>()(std::forward<Args>(args)...);
}

}} // saya

#endif
