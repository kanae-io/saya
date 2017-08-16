#ifndef SAYA_IKA_AST_DEBUG_HPP
#define SAYA_IKA_AST_DEBUG_HPP

#include "saya/ika/ast_fwd.hpp"
#include "saya/console.hpp"
#include "saya/blackhole.hpp"

#include <boost/optional.hpp>
#include <boost/range/has_range_iterator.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/algorithm/copy.hpp>
#include <boost/assert.hpp>

#include <experimental/iterator>
#include <functional>


namespace saya { namespace ika { namespace ast { namespace debug {

inline std::ostream& reset(std::ostream& os)
{
    return os << saya::console::color::RESET();
}

inline std::ostream& type(std::ostream& os, char const* t)
{
    return os << saya::console::color::fg::GREEN() << t << reset;
}

namespace detail {

template<class Iterator>
struct kv_printer
{
    kv_printer(Iterator& it)
        : it(it)
    {}

    template<class KV>
    auto operator()(KV const& kv_) const
    {
        it++ = kv_;
        return nullptr;
    }

private:
    Iterator& it;
};

template<class Iterator>
inline kv_printer<Iterator>
make_kv_printer(Iterator& it)
{
    return {it};
}


template<class T>
struct literal_wrapper
{
    T const& v;
};

template<class T>
std::ostream& operator<<(std::ostream& os, literal_wrapper<T> const& v)
{
    return os << saya::console::color::UNDERLINED() << saya::console::color::fg::LIGHTRED() << v.v << reset;
}

template<class T>
struct subtype_wrapper
{
    T const& v;
};

template<class T>
std::ostream& operator<<(std::ostream& os, subtype_wrapper<T> const& v)
{
    return os << saya::console::color::UNDERLINED() << saya::console::color::fg::YELLOW() << v.v << reset;
}

template<class T>
struct id_wrapper
{
    T const& v;
};

template<class T>
std::ostream& operator<<(std::ostream& os, id_wrapper<T> const& v)
{
    return os << saya::console::color::BOLD() << saya::console::color::fg::GREEN() << v.v << reset;
}

template<class T>
struct id_arg_wrapper
{
    T const& v;
};

template<class T>
std::ostream& operator<<(std::ostream& os, id_arg_wrapper<T> const& v)
{
    return os << saya::console::color::UNDERLINED() << saya::console::color::fg::GREEN() << v.v << reset;
}


template<class K, class V, class F = void, class Enabled = void>
struct term;

template<class K, class V, class F>
struct term<K, V, F>
{
    K k;
    V const& v;
};

// template<class K, class V, class F>
// struct term<K, V&&, F>
// {
//     K k;
//     V const v;
// };

template<class K, class V, class F>
struct term<K, V const*, F>
{
    K k;
    V const* v;

    explicit term(K const& k, V const* v)
        : k(k), v(v)
    {}
};

template<class K, class V>
struct term<K, boost::optional<V>>
{
    K k;
    boost::optional<V> const& v;
};

template<class T>
static constexpr bool
is_range_v = boost::has_range_const_iterator<T>::value;

template<class T>
static constexpr bool
is_range_v<T*> = false;


template<class K, class F>
struct term<K, void, F>
{
    K k;
    std::function<std::ostream&(std::ostream&)> f;
};

template<class F>
struct term<void, void, F>
{
    std::function<std::ostream&(std::ostream&)> f;
};

template<class K, class F>
inline term<
    std::decay_t<K const>,
    void,
    std::function<std::ostream&(std::ostream&)>
>
make_term(K const& k, F&& f)
{
    return {k, std::forward<F>(f)};
}

template<class F>
inline term<
    void,
    void,
    std::function<std::ostream&(std::ostream&)>
>
make_term(F&& f)
{
    return {std::forward<F>(f)};
}

template<class T>
T const& deref_once(T const& v) { return v; }

template<class T>
T const& deref_once(T* v) { return *v; }


template<class K, class V, std::enable_if_t<!is_range_v<V>, int> = 0>
inline std::ostream& operator<<(std::ostream& os, term<K, V> const& kv_)
{
    if (!kv_.k) {
        return os << saya::console::color::BOLD() << saya::console::color::fg::CYAN() << kv_.v;
    }

    return os << saya::console::color::fg::MAGENTA() << kv_.k << reset << ": " << kv_.v;
}

template<class K, class V>
inline std::ostream& operator<<(std::ostream& os, term<K, V const*> const& kv_)
{
    os << saya::console::color::fg::MAGENTA() << kv_.k << reset << ": ";

    if (!kv_.v) {
        return os << saya::console::color::BOLD() << saya::console::color::fg::RED() << "INVALID" << reset;
    } else {
        return os << *kv_.v;
    }
}

template<class K, class V>
inline std::ostream& operator<<(std::ostream& os, term<K, boost::optional<V>> const& kv_)
{
    os << saya::console::color::fg::MAGENTA() << kv_.k << reset << ": ";

    if (!kv_.v) {
        return os << saya::console::color::BOLD() << saya::console::color::fg::DARKGRAY() << "(none)" << reset;
    } else {
        return os << deref_once(*kv_.v);
    }
}

template<class K, class Range, std::enable_if_t<is_range_v<Range>, int> = 0>
inline std::ostream& operator<<(std::ostream& os, term<K, Range, void> const& t)
{
    if (t.v.empty()) return os;

    os << "[";
    auto oj = std::experimental::make_ostream_joiner(os, ", ");
    auto kvp = detail::make_kv_printer(oj);

    for (auto const& e : t.v) {
        kvp(e);
    }

    return os << "]";
}

template<class K, class F>
inline std::ostream& operator<<(std::ostream& os, term<K, void, F> const& t)
{
    os << saya::console::color::fg::MAGENTA() << t.k << reset << ": ";
    return t.f(os);
}

template<class F>
inline std::ostream& operator<<(std::ostream& os, term<void, void, F> const& t)
{
    return t.f(os);
}

} // detail


// ----------------------------------------------------

template<class T>
inline detail::literal_wrapper<std::decay_t<T const>>
literal(T const& v)
{
    return {v};
}

// ----------------------------------------------------

template<class T>
inline detail::subtype_wrapper<std::decay_t<T const>>
subtype(T const& v)
{
    return {v};
}

template<class T>
inline detail::id_wrapper<std::decay_t<T const>>
id(T const& v)
{
    return {v};
}

// ----------------------------------------------------

template<class T>
inline detail::id_arg_wrapper<std::decay_t<T const>>
id_arg(T const& v)
{
    return {v};
}

// ----------------------------------------------------

template<class K, class V>
inline detail::term<
    std::decay_t<K const>,
    V
>
kv(K const& k, V const& v)
{
    return {k, v};
}

template<class K, class V>
inline detail::term<
    std::decay_t<K const>,
    V const*
>
kv(K const& k, V const* v)
{
    return {k, v};
}

// ----------------------------------------------------


template<class K, class T>
inline std::ostream&
fixed(std::ostream& os, K const& t, T const& what_)
{
    type(os, t);
    return os << saya::console::color::fg::DARKGRAY() << "(" << what_ << ")" << reset;
}

template<class K>
inline std::ostream&
fixed_none(std::ostream& os, K const& t)
{
    return fixed(os, t, "none");
}

template<class K>
inline std::ostream&
fixed_empty(std::ostream& os, K const& t)
{
    return fixed(os, t, "empty");
}

template<class K>
inline std::ostream&
fixed_omitted(std::ostream& os, K const& t)
{
    return fixed(os, t, "omitted");
}

template<class K, class L>
inline std::ostream&
fixed_id(std::ostream& os, K const& id_, L const& stype_)
{
    return os << id<std::decay_t<K const>>(id_) << "(" << subtype<std::decay_t<L const>>(stype_) << ")";
}

template<class K>
inline std::ostream&
fixed_id(std::ostream& os, K const& id_)
{
    return os << id<std::decay_t<K const>>(id_);
}


// ----------------------------------------------------


template<class K, class T>
inline std::ostream&
expect(std::ostream& os, K const& t, T const* v)
{
    if (!v) {
        return os << t << "(" << saya::console::color::BOLD() << saya::console::color::fg::RED() << "INVALID" << reset << ")";
    } else {
        return os << *v;
    }
}

template<class K, class T>
inline std::ostream&
expect(std::ostream& os, K const& t, boost::optional<T> const& v)
{
    if (!v) {
        return os << t << "(" << saya::console::color::BOLD() << saya::console::color::fg::RED() << "INVALID" << reset << ")";
    } else {
        return os << *v;
    }
}

inline std::ostream&
flags(std::ostream& os, std::vector<char const*> flags)
{
    if (!flags.empty()) {
        boost::copy(
            flags | boost::adaptors::transformed([] (auto const* str) -> std::string { return std::string(saya::console::color::BOLD()) + saya::console::color::fg::CYAN() + str; }),
            std::experimental::make_ostream_joiner(os, std::string(saya::console::color::RESET()) + ", ")
        );
    }

    return os << reset;
}

inline std::ostream& flags(std::ostream& os, char const* flag)
{
    return flags(os, std::vector<char const*>{flag});
}


template<class K, class TrueF, class FalseF>
inline detail::term<std::decay_t<K const>, void, std::function<std::ostream&(std::ostream&)>>
cond(K const& k, bool cond_, TrueF true_, FalseF false_)
{
    if (cond_) {
        return detail::make_term(
            k,
            std::bind([] (std::ostream& os, auto f) -> decltype(auto) { os << saya::console::color::BOLD() << saya::console::color::fg::CYAN(); f(os); return os << reset; }, std::placeholders::_1, true_)
        );

    } else {
        return detail::make_term(
            k,
            std::bind([] (std::ostream& os, auto f) -> decltype(auto) { os << saya::console::color::BOLD() << saya::console::color::fg::CYAN(); f(os); return os << reset; }, std::placeholders::_1, false_)
        );
    }
}

template<class TrueF, class FalseF>
inline detail::term<void, void, std::function<std::ostream&(std::ostream&)>>
cond(bool cond_, TrueF true_, FalseF false_)
{
    if (cond_) {
        return detail::make_term(
            std::bind([] (std::ostream& os, auto f) -> decltype(auto) { os << saya::console::color::BOLD() << saya::console::color::fg::CYAN(); f(os); return os << reset; }, std::placeholders::_1, true_)
        );

    } else {
        return detail::make_term(
            std::bind([] (std::ostream& os, auto f) -> decltype(auto) { os << saya::console::color::BOLD() << saya::console::color::fg::CYAN(); f(os); return os << reset; }, std::placeholders::_1, false_)
        );
    }
}

inline std::ostream& name(std::ostream& os, char const* t)
{
    return os << saya::console::color::fg::BLUE() << t << reset;
}

template<class K, class... KV>
inline std::ostream&
with(std::ostream& os, K const& k, KV&&... kvs)
{
    os << k << "({ ";

    auto oj = std::experimental::make_ostream_joiner(os, ", ");
    auto kvp = detail::make_kv_printer(oj);

    saya::blackhole(kvp(std::forward<KV>(kvs))...);

    return os << " })";
}

template<class... KV>
inline std::ostream&
with(std::ostream& os, char const* t, KV&&... kvs)
{
    type(os, t);
    os << "({ ";

    auto oj = std::experimental::make_ostream_joiner(os, ", ");
    auto kvp = detail::make_kv_printer(oj);

    saya::blackhole(kvp(std::forward<KV>(kvs))...);

    return os << " })";
}

template<class T>
inline std::ostream&
proxy(std::ostream& os, char const* t, boost::optional<T> const& v)
{
    if (!v) {
        return fixed_none(os, t);
    }

    return os << *v;
}

template<class T>
inline std::ostream&
proxy(std::ostream& os, char const* t, T const* v)
{
    if (!v) {
        return fixed_none(os, t);
    }

    return os << *v;
}

}}}} // saya

#endif
