#ifndef SAYA_IKA_COMPILER_HPP
#define SAYA_IKA_COMPILER_HPP

#include "saya/ika/compiler/source.hpp"

#include "saya/ika/ast_fwd.hpp"

#include "saya/logger/logger_env.hpp"

#include <boost/filesystem/path.hpp>

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <mutex>
#include <stdexcept>


namespace saya { namespace ika {

struct compile_error : std::invalid_argument
{
    compile_error(std::string const& reason)
        : std::invalid_argument(reason)
    {}
};

struct compiler_options
{
    boost::filesystem::path ikastd;
};

class compiler : detail::source_id_access
{
public:
    using ast_type = ast::Root;
    using mutex_type = std::mutex;
    using lock_type = std::lock_guard<mutex_type>;
    using unique_lock_type = std::unique_lock<mutex_type>;

    explicit compiler(saya::logger_env l_env, compiler_options opts);
    compiler(compiler const&) = delete;
    compiler(compiler&&) = delete;

    ~compiler() = default;

    compiler& operator=(compiler const&) = delete;
    compiler& operator=(compiler&&) = delete;

    // -----------------------------------------

    void compile_ikastd() const;
    void compile(source_bundle const& bundle) const;

private:
    void compile_impl(source_bundle const& bundle) const;
    void make_world() const;

    saya::logger_env const l_env_;
    compiler_options const opts_;

    mutable bool has_ikastd_{false};

    mutable std::mutex sources_mtx_;

    mutable std::unordered_set<source_id>
    wip_sources_;

    mutable std::unordered_map<source_id, std::unique_ptr<ast_type>>
    std_sources_, sources_;
};

}} // saya

#endif
