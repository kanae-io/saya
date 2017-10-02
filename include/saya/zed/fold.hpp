#ifndef SAYA_ZED_FOLD_HPP
#define SAYA_ZED_FOLD_HPP

#include <type_traits>
#include <utility>


namespace saya { namespace zed {

template<class T, T...>
struct fold_add;

template<class T, T V1, T V2, T... Vs>
struct fold_add<T, V1, V2, Vs...>
{
    static constexpr T value = fold_add<T, V1 + V2, Vs...>::value;
};

template<class T, T V1>
struct fold_add<T, V1>
{
    static constexpr T value = V1;
};

template<class T, T... Args>
constexpr auto fold_add_v = fold_add<T, Args...>::value;

// ----------------------------------

template<class T, T...>
struct fold_subtract;

template<class T, T V1, T V2, T... Vs>
struct fold_subtract<T, V1, V2, Vs...>
{
    static constexpr T value = fold_subtract<T, V1 - V2, Vs...>::value;
};

template<class T, T V1>
struct fold_subtract<T, V1>
{
    static constexpr T value = V1;
};

template<class T, T... Args>
constexpr auto fold_subtract_v = fold_subtract<T, Args...>::value;

}} // saya

#endif
