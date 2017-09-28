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

// ----------------------------------

template<class...>
struct seq_concat;

template<class T, T... I1, T... I2, class... Rest>
struct seq_concat<std::integer_sequence<T, I1...>, std::integer_sequence<T, I2...>, Rest...>
{
    using type = typename seq_concat<
        std::integer_sequence<T, I1..., I2...>, Rest...
    >::type;
};

template<class T, T... I1>
struct seq_concat<std::integer_sequence<T, I1...>>
{
    using type = std::integer_sequence<T, I1...>;
};

template<class... Seqs>
using seq_concat_t = typename seq_concat<Seqs...>::type;

template<class... Seqs>
inline constexpr auto make_seq_concat(Seqs...)
{
    return seq_concat_t<Seqs...>{};
}


template<int Ofs, class...>
struct seq_offset;

template<int Ofs, class T, T... Is>
struct seq_offset<Ofs, std::integer_sequence<T, Is...>>
{
    using type = std::integer_sequence<T, (Ofs + Is)...>;
};

template<int Ofs, class Seq>
using seq_offset_t = typename seq_offset<Ofs, Seq>::type;

template<int Ofs, class Seq>
constexpr inline auto
make_seq_offset(Seq)
{
    return seq_offset_t<Ofs, Seq>{};
}

}} // saya

#endif
