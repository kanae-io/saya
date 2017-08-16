#include "saya/ika/compiler/source_data.hpp"

#include <memory>

namespace saya { namespace ika {

struct source_bundle
{
    source_id id;
    std::unique_ptr<source_data> data;
};

source_bundle
make_source(boost::filesystem::path const& p);

source_bundle
make_source(std::string const& buf);

}} // saya
