#ifndef SAYA_ZED_BLACKHOLE_HPP
#define SAYA_ZED_BLACKHOLE_HPP

namespace saya { namespace zed {

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

}} // saya

#endif
