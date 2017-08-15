#ifndef SAYA_ENCODING_USTRING_HPP_
#define SAYA_ENCODING_USTRING_HPP_

#include "saya/encoding/error.hpp"
#include "saya/unicode/string.hpp"

#include <boost/iterator/iterator_facade.hpp>
#include <boost/scope_exit.hpp>

#include <unicode/unistr.h> // UnicodeString
#include <unicode/ustring.h> // u_strFromWCS, u_strToWCS
#include <unicode/uchriter.h>

#include <unicode/ucsdet.h>
#include <unicode/ucnv.h>

namespace saya {
namespace detail {

inline void check_icu_failure(UErrorCode const e)
{
    if (U_FAILURE(e)) throw unicode_error(u_errorName(e));
}

class string_base
{
public:
    typedef char* iterator;
    typedef char const* const_iterator;

    string_base() = default;

    // note that this buffer is NOT null-terminated.
    string_base(char const* data, std::size_t size)
        : buf_(data, data + size)
    {
//        buf_.insert(buf_.begin(), data, data + size);
    }

    iterator begin() { return buf_.data(); }
    iterator end() { return buf_.data() + buf_.size(); }
    const_iterator begin() const { return buf_.data(); }
    const_iterator end() const { return buf_.data() + buf_.size(); }

    char const* data() const { return buf_.data(); }
    char* data() { return buf_.data(); }

    std::size_t size() const { return buf_.size(); }

    bool empty() const { return buf_.empty(); }

    void append(char const* data, std::size_t size) { buf_.insert(buf_.end(), data, data + size); }

private:
    std::vector<char> buf_;
};

} // detail

struct u8string_ : detail::string_base
{
    using detail::string_base::string_base;
};

// ===============================================================

// fwd
class ustring;
struct ustring_iterator_base {};

// Note:
// Quote from http://www.boost.org/doc/libs/1_55_0/libs/iterator/doc/iterator_facade.html:
//
// > The C++ standard requires an iterator's value_type not be const-qualified,
// > so iterator_facade strips the const from its Value parameter in order to
// > produce the iterator's value_type. Making the Value argument const provides
// > a useful hint to iterator_facade that the iterator is a constant iterator,
// > and the default Reference argument will be correct for all lvalue iterators.

// We don't support UChar&. Only UChar const& is supported due to ICU's
// dumb implementation.

template<class Iter>
class ustring_iterator
    : public boost::iterator_facade<
        ustring_iterator<Iter>,
        UChar const, // <----------------------------- here
        boost::random_access_traversal_tag
    >, ustring_iterator_base
{
    typedef ustring_iterator<Iter> this_type;
    typedef boost::random_access_traversal_tag iterator_category;

public:
    ustring_iterator() = default;
    explicit ustring_iterator(Iter const& base)
        : base_(base)
    {}

private:
    friend class boost::iterator_core_access;

    UChar const& dereference() const { return *base_; }
    bool equal(this_type const& rhs) const { return base_ == rhs.base_; }
    void increment() { ++base_; }
    void decrement() { --base_; }
    void advance(typename Iter::difference_type n) { std::advance(base_, n); }
    typename Iter::difference_type distance_to(this_type const& rhs) const
    {
        return std::distance(base_, rhs.base_);
    }

    Iter base_;
};

class ustring
{
public:
    typedef icu::UnicodeString internal_type;
    typedef ustring_iterator<ustring> iterator;
    typedef ustring_iterator<ustring const> const_iterator;
    typedef std::ptrdiff_t difference_type;

    ustring() = default;

    explicit ustring(u8string_ const& u8str)
        : istr_(icu::UnicodeString::fromUTF8(
            icu::StringPiece{u8str.data(), ::int32_t(u8str.size())}
        ))
    {}

    explicit ustring(char const* c_str)
        : istr_(c_str)
    {}

    explicit ustring(std::string const& str)
        : istr_(str.c_str(), str.size())
    {}

     explicit ustring(std::wstring const& wstr)
        : istr_(wstr.size() + 1, {0}, {0}) // allocate only, w/o init
    {
        UErrorCode err = U_ZERO_ERROR;
        ::int32_t destlength = 0;
        u_strFromWCS(istr_.getBuffer(wstr.size() + 1), wstr.size() + 1, &destlength, wstr.c_str(), wstr.size(), &err);

        if (U_FAILURE(err)) {
            throw encoding_error();
        }

        // set the new length
        istr_.releaseBuffer(destlength);

        // Ensure our buffer is null terminated.
        // We don't need to enable this unless the conversion routine above is broken.
//        (void)istr_.getTerminatedBuffer();


//        std::cout << "wstr len: " << wstr.size() << ", istr_ len: " << istr_.length() << std::endl;
    }

    explicit ustring(UChar32 cp)
        : istr_(cp)
    {}

    explicit ustring(icu::UnicodeString const& istr)
        : istr_(istr)
    {}

    explicit ustring(icu::UnicodeString&& istr)
        : istr_(std::move(istr))
    {}

    template<
        class InputIterator,
        typename std::enable_if<!std::is_base_of<ustring_iterator_base, InputIterator>::value>::type = 0
    >
    explicit ustring(InputIterator first, InputIterator last)
        : istr_(static_cast<UChar const*>(first), std::distance(first, last))
    {}

    template<
        class InputIterator,
        typename std::enable_if<std::is_base_of<ustring_iterator_base, InputIterator>::value>::type = 0
    >
    explicit ustring(InputIterator first, InputIterator last)
        : istr_(first.p->istr().getBuffer() + first.index, last.index - first.index)
    {}


    ustring& operator=(icu::UnicodeString const& istr)
    {
        istr_ = istr;
        return *this;
    }
    ustring& operator=(icu::UnicodeString&& istr)
    {
        istr_ = std::move(istr);
        return *this;
    }

    static ustring detect_and_convert(char const* data, std::size_t size)
    {
        // detect character encoding
        UErrorCode err = U_ZERO_ERROR;
        UCharsetDetector* csd = ucsdet_open(&err);
        check_icu_failure(err);

        BOOST_SCOPE_EXIT_ALL(&csd) {
            ucsdet_close(csd);
        };

        ucsdet_setText(csd, data, size, &err);
        check_icu_failure(err);

        UCharsetMatch const* ucsm = ucsdet_detect(csd, &err);
        check_icu_failure(err);

        char const* encoding_name = ucsdet_getName(ucsm, &err);
        check_icu_failure(err);

        auto* ucnv = ucnv_open(encoding_name, &err);
        check_icu_failure(err);

        return ustring(data, size, ucnv);
    }

    typedef std::unique_ptr<UConverter, decltype(&ucnv_close)> self_closing_ucnv_type;

    explicit ustring(char const* data, std::size_t size, UConverter* ucnv)
        : ustring(data, size, {ucnv, ucnv_close}, U_ZERO_ERROR)
    {}

private:
    explicit ustring(char const* data, std::size_t size, self_closing_ucnv_type ucnv, UErrorCode err)
        : istr_(data, size, ucnv.get(), err)
    {
        check_icu_failure(err);
    }


public:
//    iterator begin() { return iterator(this, 0); }
//    iterator end() { return iterator(this, istr_.length()); }
//    const_iterator cbegin() const { return begin(); }
//    const_iterator cend() const { return end(); }

    UChar at(int32_t offset) const { return istr_.charAt(offset); }
    UChar32 at32(int32_t offset) const { return istr_.char32At(offset); }

    template<class InputIterator>
    void insert(iterator at, InputIterator first, InputIterator last)
    {
        istr_.insert(int32_t(at), static_cast<UChar const*>(first), int32_t(std::distance(first, last)));
    }

    std::size_t grapheme_length() const { return unicode::grapheme_length(istr_); }
    bool grapheme_length_lt(std::size_t len) const { return unicode::grapheme_length_lt(istr_, len); }

    bool empty() const { return istr_.isEmpty(); }
    std::size_t size() const { return istr_.length(); }

    u8string_ to_u8() const
    {
        if (istr_.length() == 0) return {};
        u8string_ res;
        istr_.toUTF8String(res);
//        std::cout << "istr_ len: " << istr_.length() << ", u8str len: " << res.size() << std::endl;
        return res;
    }

    std::string to_s() const
    {
        if (istr_.length() == 0) return {};
        std::size_t const len = istr_.extract(0, istr_.length(), nullptr, nullptr);

        std::string str(len, char(0));
        istr_.extract(0, istr_.length(), &str[0], nullptr);
//        std::cout << "istr_ len: " << istr_.length() << ", str len: " << str.size() << std::endl;
        return str;
    }

    std::wstring to_ws() const
    {
        if (istr_.length() == 0) return {};
        std::wstring res(std::size_t(istr_.length()), {0}); // allocate only
        UErrorCode err = U_ZERO_ERROR;

        u_strToWCS(
            &res[0],
            istr_.length(),
            nullptr /* no need to know the dest length */,
            istr_.getBuffer(),
            istr_.length(),
            &err
        );

        if (U_FAILURE(err)) {
            throw encoding_error();
        }

//        std::cout << "istr_ len: " << istr_.length() << ", wstr len: " << res.size() << std::endl;
        return res;
    }

    icu::UnicodeString const& istr() const { return istr_; }
    icu::UnicodeString& istr() { return istr_; }

private:
    icu::UnicodeString istr_;
};

//template<>
//inline void ustring::insert(ustring::iterator at, ustring::const_iterator first, ustring::const_iterator last)
//{
//    istr_.insert(int32_t(at), first, std::distance(first, last));
//}


// some additional helpers.

// Library user can specialize these functions for their original string types.
// Same as the standard libraries, only complete specialization is allowed.

template<class From>
inline u8string_ make_u8string(From const& from)
{
    return u8string_{from};
}

template<class From>
inline ustring make_ustring(From const& from)
{
    return ustring{from};
}

} // saya

#endif
