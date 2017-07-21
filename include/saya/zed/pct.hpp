#ifndef SAYA_ZED_PCT_HPP
#define SAYA_ZED_PCT_HPP

#include <iosfwd>
#include <limits>
#include <functional>
#include <stdexcept>
#include <type_traits>

#include <cstdint>
#include <cmath>

//
// zed::basic_pct<T>
//
// high-layer non-integral 'percentage' class
//   with somewhat 'strong guarantee'
//

// for any basic_pct<T>:

// (basic attributes)
// - T must be an unsigned integral type
// - basic_pct<T> is defined like an integer, but it doesn't behave like so
//
// (traits)
// - no guarantee on initialization (does not even throw)
// - never throws on construct/copy/move/assign/destruct
// - any valid value can be constexpr
// - any valid operations can be constexpr
// - any invalid operations may throw zed::illegal_operation
//
// (behaviors)
// - defaults to 100% at compile time
// - valid range is [1%, 100%] (0 is invalid)
// - valid operations are T+T T-T T*T and N*T T*N only
// - throws if T+T > 100%
// - throws if T-T < 1%
// - if T*T < 1%, result is 1%
// - N*T and T*N are possible, but strongly discouraged
//   ('basic_pct<T> is not a ratio' philosophy)
// - for N*T, N should be unsigned integer
// - N*T may overflow without caution;
//   it will behave like (N * {T}) / {100}
// - expression has no effect for N*T where T=100%

// constructors are provided for T, int and unsigned int.
// T: because it's the base type.
// int: because it's the most fundamental type in C++.
// unsigned int: because basic_pct<T> has unsigned-traits.

namespace saya { namespace zed {

struct illegal_operation : std::runtime_error
{
    illegal_operation(char const* reason)
        : std::runtime_error(reason)
    {}
};

template<class T, class Enabled = void>
struct has_pct_traits : std::false_type {};

template<class T>
struct has_pct_traits<
    T,
    typename std::enable_if<
        std::is_nothrow_default_constructible<T>::value &&
        std::is_nothrow_move_constructible<T>::value &&
        std::is_nothrow_copy_constructible<T>::value &&
        std::is_nothrow_copy_assignable<T>::value &&
        std::is_nothrow_move_assignable<T>::value
    >::type
> : std::true_type {};


template<class T>
class basic_pct
{
public:
    using base_type = T;
    static_assert(
        std::is_integral<T>::value &&
        std::is_unsigned<T>::value,
        "T must be unsigned integral"
    );
    static_assert(has_pct_traits<T>::value, "T must satisfy has_pct_traits");

    constexpr basic_pct() noexcept = default;
    constexpr basic_pct(basic_pct const&) noexcept = default;
    constexpr basic_pct(basic_pct&&) noexcept = default;

    constexpr basic_pct& operator=(basic_pct const&) noexcept = default;
    constexpr basic_pct& operator=(basic_pct&&) noexcept = default;

    ~basic_pct() noexcept = default;

    constexpr explicit basic_pct(int i) noexcept
        : v_(i)
    {}

    constexpr explicit basic_pct(unsigned i) noexcept
        : v_(i)
    {}

    constexpr explicit basic_pct(T i) noexcept
        : v_(i)
    {}

    void swap(basic_pct& rhs) noexcept
    {
        using std::swap;

        if (this != &rhs) {
            swap(v_, rhs.v_);
        }
    }

    friend constexpr inline basic_pct
    operator+(basic_pct const& lhs, basic_pct const& rhs)
    {
        // 100 + 100 < 255
        if (lhs.v_ + rhs.v_ <= 100) {
            return basic_pct{lhs.v_ + rhs.v_};
        } else {
            throw illegal_operation("percentage exceeds 100%");
        }
    }

    friend constexpr inline basic_pct
    operator-(basic_pct const& lhs, basic_pct const& rhs)
    {
        if (lhs.v_ > rhs.v_) {
            return basic_pct{lhs.v_ - rhs.v_};
        } else {
            throw illegal_operation("percentage falls 1%");
        }
    }

    friend constexpr inline basic_pct
    operator*(basic_pct const& lhs, basic_pct const& rhs)
    {
        return basic_pct{std::max(1, lhs.v_ * rhs.v_ / 100)};
    }

    friend constexpr inline bool
    operator==(basic_pct const& lhs, basic_pct const& rhs) noexcept
    {
        return lhs.v_ == rhs.v_;
    }

    explicit operator bool() const noexcept { return true; }
    explicit operator T() const noexcept { return v_; }

private:
    T v_{100};
};

template<class T>
std::ostream& operator<<(std::ostream& os, basic_pct<T> const& rhs)
{
    return os << static_cast<T>(rhs) << '%';
}

using upct = basic_pct<std::uint8_t>;

}} // saya


namespace std {

template<>
class numeric_limits<::saya::zed::upct>
{
public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;

    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr float_denorm_style has_denorm = denorm_absent;
    static constexpr bool has_denorm_loss = false;

    static constexpr float_round_style round_style = round_toward_zero;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_bounded = true;
    static constexpr bool is_modulo = true;

    static constexpr int digits = 8; // for base-radix bits
    static constexpr int digits10 = 2;
    static constexpr int max_digits10 = 3;

    static constexpr int radix = 2;

    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;

    static constexpr bool traps = true;
    static constexpr bool tinyness_before = false;

    static constexpr ::saya::zed::upct min() noexcept { return ::saya::zed::upct{1u}; }
    static constexpr ::saya::zed::upct lowest() noexcept { return ::saya::zed::upct{1u}; }
    static constexpr ::saya::zed::upct max() noexcept { return ::saya::zed::upct{100u}; }

    static constexpr ::saya::zed::upct epsilon() noexcept { return ::saya::zed::upct{0u}; }
    static constexpr ::saya::zed::upct round_error() noexcept { return ::saya::zed::upct{}; }
    static constexpr ::saya::zed::upct infinity() noexcept { return ::saya::zed::upct{}; }

    static constexpr ::saya::zed::upct quiet_NaN() noexcept { return ::saya::zed::upct{}; }
    static constexpr ::saya::zed::upct signaling_NaN() noexcept { return ::saya::zed::upct{}; }
    static constexpr ::saya::zed::upct denorm_min() noexcept { return ::saya::zed::upct{}; }
};

template<>
class hash<::saya::zed::upct>
{
public:
    size_t operator()(::saya::zed::upct const& v) const
    {
        return hash<typename ::saya::zed::upct::base_type>()(
            static_cast<typename ::saya::zed::upct::base_type>(
                v
            )
        );
    }
};

} // std

namespace saya { namespace zed {

inline std::size_t hash_value(upct const& pct)
{
    return std::hash<upct>()(pct);
}

}} // saya

static_assert(saya::zed::has_pct_traits<saya::zed::upct>::value, "upct must satisfy has_pct_traits");

#endif
