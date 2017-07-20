#ifndef SAYA_LANG_IKA_QI_HPP_
#define SAYA_LANG_IKA_QI_HPP_

#include "saya/lang/ika/ast.hpp"

// TODO FIXME INITIAL DEV ONLY
#include "boost/spirit/include/qi.hpp"

namespace saya { namespace lang { namespace ika {

template<class CharT, class Iterator>
class grammar_qi : boost::spirit::qi::grammar<Iterator, basic_ika<CharT>(), typename grammar_traits<CharT>::space_type>
{
public:
private:
};

}}} // saya

#endif
