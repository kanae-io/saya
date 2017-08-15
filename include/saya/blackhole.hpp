#ifndef SAYA_BLACKHOLE_HPP
#define SAYA_BLACKHOLE_HPP

namespace saya {

// usage:
//
// template<class... Args>
// std::nullptr_t f(Args&&... args) { /* ... */ }
//
// template<class... Args>
// void g(Args&&... args) {
//     blackhole(f(std::forward<Args>(args))...);
// }

template<class... T>
inline void blackhole(T&&...) {}

} // saya

#endif
