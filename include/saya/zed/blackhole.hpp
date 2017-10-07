#ifndef SAYA_ZED_BLACKHOLE_HPP
#define SAYA_ZED_BLACKHOLE_HPP

#include <boost/config.hpp>


namespace saya { namespace zed {

//
// usage:
//
// blackhole{(f(args), nullptr)...};
//

#ifndef SAYA_BLACKHOLE_NO_UNUSED_ATTR
#define SAYA_BLACKHOLE_MAYBE_UNUSED [[maybe_unused]]
#else
#define SAYA_BLACKHOLE_MAYBE_UNUSED BOOST_ATTRIBUTE_UNUSED
#endif

struct SAYA_BLACKHOLE_MAYBE_UNUSED
blackhole;

struct blackhole
{
    explicit blackhole(std::nullptr_t&&...) noexcept {}
};


// inline void blackhole(std::initializer_list<int>) {}


// below is a reserved implementation for future C++ standards (> 2a)
// evaluation order is not guaranteed in C++17!

#if 0

//
// usage:
//
// template<class... Args>
// std::nullptr_t f(Args&&... args) { /* ... */ }
//
// template<class... Args>
// void g(Args&&... args) {
//     blackhole(f(std::forward<Args>(args))...);
// }
//

template<class... T>
inline std::nullptr_t blackhole(T&&...) { return nullptr; }

#endif

}} // saya

#endif
