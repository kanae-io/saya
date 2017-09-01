#include "saya/ika/compiler.hpp"
#include "saya/ika/evaluator.hpp"
#include "saya/ika/generator.hpp"
#include "saya/ika/grammar/to_ast.hpp"

#include "saya/logger.hpp"
#include "saya/filesystem.hpp"

#include <boost/format.hpp>

#include <boost/spirit/include/qi_char_class.hpp>
#include <boost/spirit/include/qi_parse.hpp>

#include <boost/core/ignore_unused.hpp>

#include <iostream>
#include <future>
#include <thread>


namespace saya { namespace ika {

namespace {

static grammar::to_ast const gr_;

} // anon

using ast_type = ast::Root;


class compiler::Impl
{
public:
    explicit Impl(saya::logger_env const& l_env)
        : eval_(l_env)
        , gen_(l_env)
    {}


    evaluator const eval_;
    generator const gen_;

    std::unordered_map<source_id, std::unique_ptr<ast_type>>
    std_sources_, sources_;
};


compiler::compiler(saya::logger_env l_env, compiler_options opts)
    : l_env_(std::move(l_env))
    , opts_(std::move(opts))
    , impl_(std::make_unique<compiler::Impl>(l_env_))
{}

compiler::~compiler() = default;


void compiler::compile_ikastd() const
{
    lock_type lock(sources_mtx_);

    if (has_ikastd_) {
        throw std::invalid_argument("ikastd has been compiled more than once");
    }

    this->make_world();
    has_ikastd_ = true;
}

void compiler::compile(source_bundle const& bundle) const
{
    unique_lock_type lock(sources_mtx_);
    if (!has_ikastd_) {
        throw std::logic_error("please run compiler::compile_ikastd()");
    }

    if (impl_->sources_.count(bundle.id)) {
        throw std::invalid_argument(boost::str(boost::format("specified source already exists [%s]")
            % bundle.id
        ));
    }

    lock.unlock();
    this->compile_impl(bundle, "ikac");
}

void compiler::compile_impl(source_bundle const& bundle, char const* prompt) const
{
    #define SAYA_COMPILER_SOURCE_MESSAGE(mes) \
        boost::str(boost::format("%s [%s]") % (mes) % bundle.id)

    #define SAYA_COMPILE_ERROR(mes) \
        throw compile_error(SAYA_COMPILER_SOURCE_MESSAGE(mes))

    if (!bundle.data || bundle.data->buf.empty()) {
        throw std::invalid_argument(SAYA_COMPILER_SOURCE_MESSAGE("input buffer should not be empty"));
    }

    auto const is_std = bundle.id.is_std();

    std::unique_ptr<ast_type> root;
    auto first = std::cbegin(bundle.data->buf);
    auto const last = std::cend(bundle.data->buf);

    auto const success = boost::spirit::qi::phrase_parse(
        first, last,
        gr_,
        boost::spirit::qi::standard::blank,
        root
    );

    if (!success) {
        SAYA_COMPILE_ERROR("parse failed");
    }

    if (!(first == last)) {
        SAYA_COMPILE_ERROR("unprocessed characters remain");
    }

    // ---------------------------------------------

    saya::logger l{l_env_, prompt};
    boost::ignore_unused(l);

    #if defined(NDEBUG)
    if (!is_std) {
    #endif
        l.note() << "source: [" << bundle.id << "]" << std::endl;
        root->debug(l.note());
        l.info() << "parse success" << std::endl;
    #if defined(NDEBUG)
    }
    #endif

    #undef SAYA_COMPILER_SOURCE_MESSAGE
    #undef SAYA_COMPILE_ERROR

    impl_->sources_[bundle.id] = std::move(root);
}

void compiler::make_world() const
{
    if (!boost::filesystem::is_directory(opts_.ikastd)) {
        throw std::invalid_argument("given path \"" + opts_.ikastd.generic_string() + "\" is not a valid ikastd directory");
    }

    static char const* const PROMPT = "ikac::std";
    saya::logger l{l_env_, PROMPT};

    l.info() << "reading data..." << std::endl;

    for (auto it = boost::filesystem::recursive_directory_iterator(opts_.ikastd), end = boost::filesystem::recursive_directory_iterator(); it != end; ++it) {
        // skip dot-files
        if (it->path().extension() != ".ika") {
            continue;
        }

        auto const stat = boost::filesystem::status(*it);

        switch (stat.type()) {
        case boost::filesystem::regular_file:
            l.info() << "found [" << it->path().filename().generic_string() << "]" << std::endl;
            break;

        default:
            continue;
        }

        l.info() << "compiling..." << std::endl;

        auto bundle = make_source(it->path());
        source_id_access::mark_as_std(bundle.id);
        this->compile_impl(bundle, PROMPT);
    }

    if (impl_->std_sources_.empty()) {
        throw compile_error("no valid ikastd source found!");
    }

    l.info() << "compiled." << std::endl;
}

}} // saya
