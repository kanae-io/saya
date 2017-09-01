#include "saya/ika/generator.hpp"
#include "saya/ika/grammar/to_html.hpp"

#include "saya/logger.hpp"

#include <boost/spirit/include/karma_char_class.hpp>
#include <boost/spirit/include/karma_generate.hpp>


namespace saya { namespace ika {

namespace {

static grammar::to_html const gr_html_;

} // anon


generator::generator(saya::logger_env l_env)
    : l_env_(std::move(l_env))
{}

std::unique_ptr<generator::buf_type>
generator::generate_html() const
{
    saya::logger l{l_env_, "ikac::gen"};
    return {};
}


}} // saya
