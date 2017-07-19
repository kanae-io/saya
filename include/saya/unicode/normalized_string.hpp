#include "saya/unsafe_string.hpp"
#include "saya/encoding/ustring.hpp"
#include "saya/unicode/normalize.hpp"

namespace saya {

template<class CharT, class Normalization = unicode::nfc>
class basic_normalized_string
    : public saya::basic_unsafe_string<
          basic_normalized_string<CharT, Normalization>,
          CharT,
          saya::ustring
      >
{
public:
    typedef std::basic_string<CharT> string_type;

    typedef saya::basic_unsafe_string<
        basic_normalized_string<CharT, Normalization>,
        CharT,
        saya::ustring
    > base_type;
    using typename base_type::internal_type;

private:
    static internal_type normalize(internal_type const& src)
    {
        return saya::unicode::normalize<Normalization>(src);
    }
};

namespace detail {
typedef unicode::nfc unicode_default_normalization_format_type;
} // detail

template<class Normalization = detail::unicode_default_normalization_format_type>
using normalized_string = basic_normalized_string<char, Normalization>;

template<class Normalization = detail::unicode_default_normalization_format_type>
using normalized_wstring = basic_normalized_string<wchar_t, Normalization>;

} // saya
