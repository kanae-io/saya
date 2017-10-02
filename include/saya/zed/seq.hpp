#ifndef SAYA_ZED_SEQ_HPP
#define SAYA_ZED_SEQ_HPP

#include <boost/variant/variant_fwd.hpp>

#include <type_traits>
#include <utility>


namespace saya { namespace zed {

template<class...>
struct empty_seq {};

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
