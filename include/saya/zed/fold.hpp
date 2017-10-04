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


// ---------------------------------------

template<bool... Bs>
struct bool_seq {};

template<class T>
struct to_bool_seq;

template<bool... Bs>
struct to_bool_seq<
    bool_seq<Bs...>
>
{
    using type = bool_seq<Bs...>;
};

template<bool... Bs>
struct to_bool_seq<
    std::integer_sequence<bool, Bs...>
>
{
    using type = bool_seq<Bs...>;
};

template<
    template<bool...>
    class bool_template,
    class BoolSeq
>
struct bool_dispatch;

template<
    template<bool...>
    class bool_template,
    bool... Bs
>
struct bool_dispatch<
    bool_template,
    bool_seq<Bs...>
>
{
    using type = bool_template<Bs...>;
};

template<
    template<bool...>
    class bool_template,
    bool... Bs
>
struct bool_dispatch<
    bool_template,
    std::integer_sequence<bool, Bs...>
>
{
    using type = bool_template<Bs...>;
};

template<
    template<bool...>
    class bool_template,
    class BoolSeq
>
using bool_dispatch_t = typename bool_dispatch<bool_template, BoolSeq>::type;


namespace detail {

template<bool... Conds>
struct fold_logical_or_impl : std::false_type {};

template<bool Cond1>
struct fold_logical_or_impl<Cond1> : std::integral_constant<bool, Cond1> {};

template<bool Cond1, bool Cond2, bool... Rest>
struct fold_logical_or_impl<Cond1, Cond2, Rest...>
{
    using type = std::conditional_t<
        Cond1 || Cond2,
        std::true_type, // short circuit
        typename fold_logical_or_impl<
            /* Cond1 || Cond2, */ Rest...
        >::type
    >;
};

} // detail

template<bool Cond1, bool Cond2, bool... Rest>
struct fold_logical_or
{
    using type = typename detail::fold_logical_or_impl<Cond1, Cond2, Rest...>::type;
};

template<bool Cond1, bool Cond2, bool... Rest>
using fold_logical_or_t = typename fold_logical_or<Cond1, Cond2, Rest...>::type;

template<bool Cond1, bool Cond2, bool... Rest>
constexpr bool fold_logical_or_v = fold_logical_or_t<Cond1, Cond2, Rest...>::value;


namespace detail {

template<bool... Conds>
struct fold_logical_and_impl : std::true_type {};

template<bool Cond1>
struct fold_logical_and_impl<Cond1>
{
    using type = std::integral_constant<bool, Cond1>;
};

template<bool Cond1, bool Cond2, bool... Rest>
struct fold_logical_and_impl<Cond1, Cond2, Rest...>
{
    using type = std::conditional_t<
        Cond1 && Cond2,
        typename fold_logical_and_impl<
            /* Cond1 || Cond2, */ Rest...
        >::type,
        std::false_type // short circuit
    >;
};

} // detail


template<bool Cond1, bool Cond2, bool... Rest>
struct fold_logical_and
{
    using type = typename detail::fold_logical_and_impl<Cond1, Cond2, Rest...>::type;
};

template<bool Cond1, bool Cond2, bool... Rest>
using fold_logical_and_t = typename fold_logical_and<Cond1, Cond2, Rest...>::type;

template<bool Cond1, bool Cond2, bool... Rest>
constexpr bool fold_logical_and_v = fold_logical_and_t<Cond1, Cond2, Rest...>::value;

}} // saya

#endif
