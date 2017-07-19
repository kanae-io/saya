#ifndef SAYA_UNICODE_STRING_HPP_
#define SAYA_UNICODE_STRING_HPP_

#include "saya/encoding/error.hpp"
#include <unicode/unistr.h>
#include <unicode/brkiter.h>

#include <functional>

namespace saya { namespace unicode {

inline icu::BreakIterator* break_iterator_instance(icu::UnicodeString const& istr)
{
    // We don't use the system's default locale.
    //
    // From http://userguide.icu-project.org/boundaryanalysis:
    //
    // > For ICU 2.6, Break Iterators for the Thai locale will make use of
    // > a Thai dictionary for finding word and line boundaries; all other
    // > locales will use the default boundary rules.
    //
    // This is just insane.

    UErrorCode err = U_ZERO_ERROR;
    auto* const it = icu::BreakIterator::createCharacterInstance(icu::Locale::getUS(), err);
    detail::check_icu_failure(err);
    it->setText(istr);
    return it;
}

// LESS THAN
inline bool grapheme_length_lt(icu::UnicodeString const& istr, std::size_t len)
{
    if (len < 1) {
        throw std::range_error("grapheme_length_lt: len must be >= 1");
    }

    auto* const it = break_iterator_instance(istr);
    std::size_t index = 0;
    while (it->next() != icu::BreakIterator::DONE) {
        if (!(index < len)) {
            return false;
        }
    }
    return true;
}

template<class F>
void each_grapheme(icu::UnicodeString const& istr, F const& f)
{
    if (istr.isEmpty()) return;

    // loop.
    auto* const it = break_iterator_instance(istr);
    while (it->next() != BreakIterator::DONE && f(it)) {}
}

inline std::size_t grapheme_length(icu::UnicodeString const& istr)
{
    std::size_t len = 0;
    each_grapheme(istr, [&] (icu::BreakIterator*) { ++len; return true; });
    return len;
}

}} // saya

#endif
