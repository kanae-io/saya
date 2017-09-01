#include "saya/ika/compiler/source.hpp"

#include "saya/filesystem.hpp"

#include <boost/format.hpp>

#include <iostream>
#include <string>
#include <memory>
#include <stdexcept>


namespace {
static char const* const INDENT = "  ";
}

namespace saya { namespace ika {
namespace detail {
void source_id_access::mark_as_std(source_id& id)
{
    id.is_std(true);
}
} // detail

std::ostream& operator<<(std::ostream& os, source_id const& sid)
{
    using K = SourceKind;
    auto const* const is_std_ = sid.is_std() ? "[std]" : "";

    switch (sid.kind) {
    case K::MEMORY:
        return os << "MEMORY:" << is_std_ << sid.id;

    case K::FILE:
        return os << "FILE:" << is_std_ << sid.id;

    default:
        throw std::logic_error("unhandled source_id");
    }
}

std::ostream& operator<<(std::ostream& os, source_data const& data)
{
    return os <<
        "source {\n" <<
        INDENT << "location: " << data.location << ",\n" <<
        INDENT << "size: " << data.size() << "\n" <<
        "}\n"
    ;
}

source_id source_id::init(SourceKind k, std::string const& loc, std::string const& buf)
{
    using K = SourceKind;

    source_id id;
    id.kind = k;

    switch (k) {
    case K::MEMORY:
        id.id = boost::str(boost::format("%p") % buf.data());
        break;

    case K::FILE:
        id.id = loc;
        break;

    default:
        throw std::logic_error("unhandled SourceKind in make_source_impl");
    }

    return id;
}

inline source_bundle
make_source_impl(SourceKind k, std::string loc, std::string buf)
{
    auto const id = source_id::init(k, loc, buf);
    auto data = std::make_unique<source_data>();
    data->location = std::move(loc);
    data->buf = std::move(buf);
    return {id, std::move(data)};
}

source_bundle
make_source(boost::filesystem::path const& p)
{
    return make_source_impl(SourceKind::FILE, p.generic_string(), saya::read<char>(p));
}

source_bundle
make_source(std::string buf)
{
    return make_source_impl(SourceKind::MEMORY, "(in-memory)", std::move(buf));
}

}} // saya
