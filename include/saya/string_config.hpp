#ifndef SAYA_STRING_CONFIG_HPP_
#define SAYA_STRING_CONFIG_HPP_

#include <boost/preprocessor/expand.hpp>
#include <boost/preprocessor/cat.hpp>

#include <string>

#define SAYA_STRING_CONFIG_CHAR_BINDER(str) BOOST_PP_EXPAND(str)
#define SAYA_STRING_CONFIG_WCHAR_BINDER(str) BOOST_PP_CAT(L, str)

#define SAYA_STRING_CONFIG_FUNCTION(type, name, value) \
    static type const& name() { static type instance{value}; return instance; }

#define SAYA_STRING_CONFIG_DEFINE(DEF) \
    DEF(char, SAYA_STRING_CONFIG_CHAR_BINDER) \
    DEF(wchar_t, SAYA_STRING_CONFIG_WCHAR_BINDER)

#define SAYA_STRING_CONFIG_DEFINE_WITH_DEF(DEF, DEF_DEF) \
    DEF(char, SAYA_STRING_CONFIG_CHAR_BINDER, DEF_DEF) \
    DEF(wchar_t, SAYA_STRING_CONFIG_WCHAR_BINDER, DEF_DEF)


#endif
