#ifndef SAYA_CONSOLE_FWD_HPP_
#define SAYA_CONSOLE_FWD_HPP_

namespace saya { namespace console {

template<class CharT>
struct basic_color;

using color = basic_color<char>;
using wcolor = basic_color<wchar_t>;
using no_color = basic_color<void>;

}} // saya

#endif
