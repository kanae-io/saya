#ifndef SAYA_ZED_SEQ_HPP
#define SAYA_ZED_SEQ_HPP

#include <boost/variant/variant_fwd.hpp>

#include <type_traits>
#include <tuple>
#include <utility>


namespace saya { namespace zed {

struct void_elem {};

template<class T>
struct is_void : std::conditional_t<std::is_void_v<T>, std::true_type, std::false_type> {};

template<>
struct is_void<void_elem> : std::true_type {};

template<class T>
constexpr bool is_void_v = is_void<T>::value;


template<class...>
struct empty_seq {};

template<class Seq>
struct compact;

template<class Seq>
using compact_t = typename compact<Seq>::type;



namespace detail {

template<std::size_t I, class... Args>
struct count_impl
{
    static constexpr auto value = count_impl<I + 1, Args...>::value;
};

template<std::size_t I>
struct count_impl<I>
{
    static constexpr auto value = I;
};

} // detail

template<class T>
struct count;

template<class... Args>
struct count<std::tuple<Args...>>
    : std::integral_constant<std::size_t, detail::count_impl<0, Args...>::value>
{};

template<class T>
constexpr auto count_v = count<T>::value;


template<class...>
struct is_empty : std::false_type {};

template<class... VoidArgs>
struct is_empty<empty_seq<VoidArgs...>> : std::true_type {};

template<>
struct is_empty<std::tuple<>> : std::true_type {};

template<class Arg1, class... Args>
struct is_empty<std::tuple<Arg1, Args...>> : std::integral_constant<bool, is_empty<compact_t<std::tuple<Arg1, Args...>>>::value> {};

template<>
struct is_empty<std::tuple<void>> : std::true_type {};


namespace detail {

template<class Seq, class...>
struct compact_impl;

template<class... SeqArgs>
struct compact_impl<std::tuple<SeqArgs...>>
{
    using type = std::tuple<SeqArgs...>;
};

template<class Arg1, class... SeqArgs, class... Rest>
struct compact_impl<std::tuple<SeqArgs...>, Arg1, Rest...>
{
    using type = std::conditional_t<
        is_void_v<Arg1>,
        typename compact_impl<std::tuple<SeqArgs...>, Rest...>::type,
        typename compact_impl<std::tuple<SeqArgs..., Arg1>, Rest...>::type
    >;
};

template<class Seq>
struct compact_impl_dispatch;

template<class... Args>
struct compact_impl_dispatch<
    std::tuple<Args...>
>
    : compact_impl<std::tuple<>, Args...>
{};

} // detail


template<class Seq>
struct compact : detail::compact_impl_dispatch<Seq> {};


namespace detail {

template<class Seq, class... Args>
struct reversed_impl;

template<class... SeqArgs>
struct reversed_impl<std::tuple<SeqArgs...>>
{
    using type = std::tuple<SeqArgs...>;
};

template<class Arg1, class... SeqArgs, class... Rest>
struct reversed_impl<std::tuple<SeqArgs...>, Arg1, Rest...>
{
    using type = typename reversed_impl<std::tuple<Arg1, SeqArgs...>, Rest...>::type;
};

template<class Seq>
struct reversed_impl_dispatch;

template<class... Args>
struct reversed_impl_dispatch<
    std::tuple<Args...>
>
    : reversed_impl<std::tuple<>, Args...>
{};

} // detail

template<class Seq>
struct reversed : detail::reversed_impl_dispatch<Seq> {};

template<class Seq>
using reversed_t = typename reversed<Seq>::type;


template<class...>
struct t_seq_concat;

template<class... T1, class... T2, class... Rest>
struct t_seq_concat<std::tuple<T1...>, std::tuple<T2...>, Rest...>
{
    using type = typename t_seq_concat<
        std::tuple<T1..., T2...>, Rest...
    >::type;
};

template<class... T1>
struct t_seq_concat<std::tuple<T1...>>
{
    using type = std::tuple<T1...>;
};

template<class... Seqs>
using t_seq_concat_t = typename t_seq_concat<Seqs...>::type;


template<class...>
struct t_seq_variant;

template<class... Ts>
struct t_seq_variant<std::tuple<Ts...>>
{
    using type = boost::variant<Ts...>;
};

template<class Tup>
using t_seq_variant_t = typename t_seq_variant<Tup>::type;



template<class...>
struct i_seq_concat;

template<class T, T... I1, T... I2, class... Rest>
struct i_seq_concat<std::integer_sequence<T, I1...>, std::integer_sequence<T, I2...>, Rest...>
{
    using type = typename i_seq_concat<
        std::integer_sequence<T, I1..., I2...>, Rest...
    >::type;
};

template<class T, T... I1>
struct i_seq_concat<std::integer_sequence<T, I1...>>
{
    using type = std::integer_sequence<T, I1...>;
};

template<class... Seqs>
using i_seq_concat_t = typename i_seq_concat<Seqs...>::type;

template<class... Seqs>
inline constexpr auto make_i_seq_concat(Seqs...)
{
    return i_seq_concat_t<Seqs...>{};
}


template<class T, T, class Seq>
struct i_seq_offset;

template<class T, T Ofs, T... Is>
struct i_seq_offset<T, Ofs, std::integer_sequence<T, Is...>>
{
    using type = std::integer_sequence<T, (Ofs + Is)...>;
};

template<class T, T Ofs, class Seq>
using i_seq_offset_t = typename i_seq_offset<T, Ofs, Seq>::type;

template<class T, T Ofs, class Seq>
constexpr inline auto
make_i_seq_offset(Seq)
{
    return i_seq_offset_t<T, Ofs, Seq>{};
}

}} // saya

#endif
