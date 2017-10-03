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


template<bool... Conds>
struct fold_logical_or /* never defined */;

template<bool Cond1>
struct fold_logical_or<Cond1>
{
    using type = std::integral_constant<bool, Cond1>;
};

template<bool Cond1, bool Cond2, bool... Rest>
struct fold_logical_or<Cond1, Cond2, Rest...>
{
    using type = std::conditional_t<
        Cond1 || Cond2,
        std::true_type, // short circuit
        typename fold_logical_or<
            /* Cond1 || Cond2, */ Rest...
        >::type
    >;
};

template<bool... Conds>
using fold_logical_or_t = typename fold_logical_or<Conds...>::type;


}} // saya

#endif
