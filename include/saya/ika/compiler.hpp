#ifndef SAYA_IKA_COMPILER_HPP
#define SAYA_IKA_COMPILER_HPP

#include "saya/ika/error.hpp"
#include "saya/ika/compiler/source.hpp"

#include "saya/logger/logger_env.hpp"

#include <boost/filesystem/path.hpp>

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>

namespace saya { namespace ika {

struct compiler_options
{
    boost::filesystem::path ikastd;
};

class compiler : detail::source_id_access
{
public:
    using mutex_type = std::mutex;
    using lock_type = std::lock_guard<mutex_type>;
    using unique_lock_type = std::unique_lock<mutex_type>;

    explicit compiler(saya::logger_env l_env, compiler_options opts);
    compiler(compiler const&) = delete;
    compiler(compiler&&) = delete;

    ~compiler();

    compiler& operator=(compiler const&) = delete;
    compiler& operator=(compiler&&) = delete;

    // -----------------------------------------

    void compile_ikastd() const;
    void compile(source_bundle const& bundle) const;

private:
    void compile_impl(source_bundle const& bundle, char const* prompt) const;
    void make_world() const;

    saya::logger_env const l_env_;
    compiler_options const opts_;

    mutable bool has_ikastd_{false};

    mutable std::mutex sources_mtx_;

    mutable std::unordered_set<source_id>
    wip_sources_;

    class Impl;
    mutable std::unique_ptr<Impl> impl_;
};

}} // saya

#endif
