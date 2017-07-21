#ifndef SAYA_GUMBO_MAKE_RANGE_HPP_
#define SAYA_GUMBO_MAKE_RANGE_HPP_

#include "saya/gumbo/common.hpp"

#include <boost/range/iterator_range.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include <functional>

namespace saya { namespace gumbo {

namespace detail {

//template<class ReturnType>
//struct gumbo_range_cast
//{
//    typedef ReturnType& result_type;
//
//    inline result_type operator()(typename saya::respective_const_adder<ReturnType, void*>::type element) const
//    {
//        return *static_cast<ReturnType*>(element);
//    }
//};

template<class ReturnType>
struct gumbo_range_cast
{
    typedef ReturnType* result_type;

    inline result_type operator()(typename saya::respective_const_adder<ReturnType, void*>::type element) const
    {
        return static_cast<ReturnType*>(element);
    }
};


template<
    class ReturnType,
    class GumboVectorType
>
inline auto make_range_vector(GumboVectorType& v)
    -> decltype(boost::make_iterator_range(v.data, v.data + v.length) | boost::adaptors::transformed(gumbo_range_cast<ReturnType>()))
{
    static_assert(
        std::is_same<
            typename std::remove_const<GumboVectorType>::type,
            ::GumboVector
        >::value,
        "you should pass GumboVector to make_range_vector"
    );

    return boost::make_iterator_range(v.data, v.data + v.length) | boost::adaptors::transformed(gumbo_range_cast<ReturnType>());
}

template<class ReturnType, class GumboNodeType>
struct make_range_impl
{
    static_assert(
        std::is_same<
            typename std::remove_const<GumboNodeType>::type*,
            ::GumboNode*
        >::value,
        "you should pass a pointer to GumboNode to make_range"
    );

    static inline auto impl(GumboNodeType* node)
        -> decltype(make_range_vector<ReturnType>(node->v.element.children))
    {
        return make_range_vector<ReturnType>(node->v.element.children);
    }
};

template<class GumboNodeType>
struct make_range_impl< ::GumboAttribute, GumboNodeType>
{
    static_assert(
        std::is_same<
            typename std::remove_const<GumboNodeType>::type*,
            ::GumboNode*
        >::value,
        "you should pass a pointer to GumboNode to make_range"
    );

    static inline auto impl(GumboNodeType* node)
        -> decltype(make_range_vector< ::GumboAttribute>(node->v.element.attributes))
    {
        return make_range_vector< ::GumboAttribute>(node->v.element.attributes);
    }
};

} // detail


template<class ReturnType, class GumboNodeType>
inline auto make_range(GumboNodeType* node)
    -> decltype(detail::make_range_impl<ReturnType, GumboNodeType>::impl(node))
{
    return detail::make_range_impl<ReturnType, GumboNodeType>::impl(node);
}

}} // saya

#endif
