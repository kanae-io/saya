#ifndef SAYA_ZED_META_HPP
#define SAYA_ZED_META_HPP

#include "saya/zed/seq.hpp"
#include "saya/zed/fold.hpp"


namespace saya { namespace zed {

struct meta_arg {};

template<class... Args>
class template_
{
public:
    static constexpr std::size_t args_count = sizeof...(Args);
    static constexpr std::size_t meta_arg_count =
        std::integral_constant<std::size_t, (0 + std::is_same_v<Args, meta_arg>)...>::value
    ;
    static constexpr bool has_meta_arg = meta_arg_count > 0;

    static_assert(has_meta_arg, "variadic meta template must have at least one meta_arg");
};

template<class MT1>
class template_<MT1>
{
public:
    static constexpr std::size_t args_count = 1;
    static constexpr std::size_t meta_arg_count = std::is_same_v<MT1, meta_arg>;
    static constexpr bool has_meta_arg = meta_arg_count > 0;

    static_assert(has_meta_arg, "unary meta template must have exact one meta_arg");
};


template<class T1, class T2>
struct eq : std::conditional_t<(T1::value == T2::value), std::true_type, std::false_type> {};

template<class T1, class T2>
struct not_eq : std::conditional_t<(T1::value != T2::value), std::true_type, std::false_type> {};


template<class T>
struct is_true : std::is_same<T, std::true_type> {};

template<class T>
struct is_false : std::is_same<T, std::false_type> {};


template<class T1, class T2>
struct less : std::conditional_t<(T1::value < T2::value), std::true_type, std::false_type> {};

template<class T1, class T2>
struct less_eq : std::conditional_t<(T1::value <= T2::value), std::true_type, std::false_type> {};

template<class T1, class T2>
struct greater : std::conditional_t<(T1::value > T2::value), std::true_type, std::false_type> {};

template<class T1, class T2>
struct greater_eq : std::conditional_t<(T1::value >= T2::value), std::true_type, std::false_type> {};


namespace detail {

namespace aux_ {

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
struct meta_apply;

template<
    template<class>
    class Template, // unary meta
    class Arg,
    class... MTs
>
struct meta_apply<
    Template,
    template_<MTs...>, // auto-assign
    Arg
>
{
    using type = Template<Arg>;
};

template<
    class MT1, class MT2,
    template<class, class>
    class Template, // binary meta
    class Arg
>
struct meta_apply<
    Template,
    template_<MT1, MT2>,
    Arg
>
{
    static_assert(std::is_same_v<MT1, meta_arg> || std::is_same_v<MT2, meta_arg>, "binary meta type: either MT1 or MT2 must be meta_arg");

    using type = std::conditional_t<
        std::is_same_v<MT1, meta_arg>,
        Template<Arg, MT2>,
        Template<MT1, Arg>
    >;
};


template<
    template<class, class>
    class Template, // binary meta
    class Arg1, class Arg2,
    class... MTs
>
struct meta_apply<
    Template,
    template_<MTs...>, // auto-assign
    Arg1, Arg2
>
{
    using type = Template<Arg1, Arg2>;
};

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
using meta_apply_t = typename meta_apply<Template, MetaTemplate, Args...>::type;

} // aux_


template<
    class Result,
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
struct meta_apply_impl;

template<
    class Result,
    template<class...>
    class Template,
    class MetaTemplate
>
struct meta_apply_impl<
    Result, Template, MetaTemplate
>
{
    using type = Result;
};

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class Arg,
    class... ResultArgs,
    class... Rest
>
struct meta_apply_impl<
    std::tuple<ResultArgs...>,
    Template,
    MetaTemplate,
    Arg, Rest...
>
{
    using type = typename meta_apply_impl<
        std::tuple<
            ResultArgs...,
            aux_::meta_apply_t<Template, MetaTemplate, Arg>
        >,
        Template,
        MetaTemplate,
        Rest...
    >::type;
};

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
struct meta_apply_dispatcher
{
    using type = typename meta_apply_impl<
        std::tuple<>, Template, MetaTemplate, Args...
    >::type;
};

} // detail


template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
using meta_apply = typename detail::meta_apply_dispatcher<Template, MetaTemplate, Args...>::type;


namespace detail {

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
struct meta_t_dispatcher
{
    using type = std::tuple<
        typename meta_apply<Template, MetaTemplate, Args...>::type...
    >;
};

} // detail

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
using meta_t = typename detail::meta_t_dispatcher<Template, MetaTemplate, Args...>::type;


namespace detail {

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
struct meta_v_dispatcher
{
    using type = bool_seq<
        meta_apply<Template, MetaTemplate, Args>::value...
    >;
};

} // detail

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
using meta_v = typename detail::meta_v_dispatcher<Template, MetaTemplate, Args...>::type;


namespace detail {

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
struct all_of_dispatcher
{
    using type = bool_dispatch_t<fold_logical_and, meta_v<Template, MetaTemplate, Args...>>;
};

} // detail

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
using all_of = typename detail::all_of_dispatcher<Template, MetaTemplate, Args...>::type;


template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
constexpr bool all_of_v = all_of<Template, MetaTemplate, Args...>::value;


namespace detail {

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
struct any_of_dispatcher
{
    using type = bool_dispatch<fold_logical_or, meta_v<Template, MetaTemplate, Args...>>;
};

} // detail

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
using any_of = typename detail::any_of_dispatcher<Template, MetaTemplate, Args...>::type;

template<
    template<class...>
    class Template,
    class MetaTemplate,
    class... Args
>
constexpr bool any_of_v = any_of<Template, MetaTemplate, Args...>::value;


namespace detail {

template<
    class Result,
    template<class, class>
    class BinaryTemplate,
    class MetaTemplate,
    class... Args
>
struct reduced_impl;

template<
    template<class, class>
    class BinaryTemplate,
    class MetaTemplate,
    class Arg1, class Arg2, class... Rest
>
struct reduced_impl<
    std::tuple<>,
    BinaryTemplate,
    MetaTemplate,
    Arg1, Arg2, Rest...
>
{
    using type = typename reduced_impl<
        std::tuple<BinaryTemplate<Arg1, Arg2>>,
        BinaryTemplate,
        MetaTemplate,
        Rest...
    >;
};

template<
    template<class, class>
    class BinaryTemplate,
    class MetaTemplate,
    class PrevA, class PrevB,
    class Last,
    class... Result
>
struct reduced_impl<
    std::tuple<
        Result...,
        BinaryTemplate<PrevA, PrevB>
    >,
    BinaryTemplate,
    MetaTemplate,
    Last
>
{
    using type = std::tuple<
        Result...,
        BinaryTemplate<PrevA, PrevB>,
        BinaryTemplate<PrevB, Last>
    >;
};

template<
    template<class, class>
    class BinaryTemplate,
    class MetaTemplate,
    class PrevA, class PrevB,
    class Current,
    class... Result,
    class... Rest
>
struct reduced_impl<
    std::tuple<
        Result...,
        BinaryTemplate<PrevA, PrevB>
    >,
    BinaryTemplate,
    MetaTemplate,
    Current,
    Rest...
>
{
    using type = typename reduced_impl<
        std::tuple<
            Result...,
            BinaryTemplate<PrevA, PrevB>,
            BinaryTemplate<PrevB, Current>
        >,
        BinaryTemplate,
        MetaTemplate,
        Rest...
    >::type;
};

} // detail

template<
    template<class, class>
    class BinaryTemplate,
    class MetaTemplate,
    class... Args
>
struct reduced
{
    using type = typename detail::reduced_impl<
        std::tuple<>,
        BinaryTemplate,
        MetaTemplate,
        Args...
    >::type;
};

template<
    template<class, class>
    class BinaryTemplate,
    class MetaTemplate,
    class... Args
>
struct reduced<
    BinaryTemplate,
    MetaTemplate,
    std::tuple<Args...>
>
{
    using type = typename detail::reduced_impl<
        std::tuple<>,
        BinaryTemplate,
        MetaTemplate,
        Args...
    >::type;
};

template<
    template<class, class>
    class BinaryTemplate,
    class MetaTemplate,
    class... Args
>
using reduced_t = typename reduced<BinaryTemplate, MetaTemplate, Args...>::type;


template<class... Args>
struct paired
{
    using type = reduced_t<std::pair, template_<>, Args...>;
};

template<class... Args>
using paired_t = typename paired<Args...>::type;


}} // saya

#endif
