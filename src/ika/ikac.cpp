#include "saya/ika/compiler/bridge.hpp"
#include "saya/ika/compiler.hpp"
#include "saya/ika/evaluator.hpp"

#include "saya/ika/ast/all.hpp"

#include "saya/logger.hpp"
#include "saya/filesystem.hpp"

#include "saya/stream_lock.hpp"

#include <boost/format.hpp>
#include <boost/core/ignore_unused.hpp>

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>


namespace {

static constexpr char const* LOGO =
    "\xE2\x96\x91\xE2\x96\x80\xE2\x96\x88\xE2\x96\x80\xE2\x96\x91\xE2\x96\x88\xE2\x96\x91\xE2\x96\x88\xE2\x96\x91\xE2\x96\x88\xE2\x96\x80\xE2\x96\x88"
    "\xE2\x96\x91\xE2\x96\x91\xE2\x96\x91\xE2\x96\x91\xE2\x96\x84\xE2\x96\x80\xE2\x96\x88\xE2\x96\x80\xE2\x96\x84\xE2\x96\x91\xE2\x96\x91\n"
    "\xE2\x96\x91\xE2\x96\x91\xE2\x96\x88\xE2\x96\x91\xE2\x96\x91\xE2\x96\x88\xE2\x96\x80\xE2\x96\x84\xE2\x96\x91\xE2\x96\x88\xE2\x96\x80\xE2\x96\x88"
    "\xE2\x96\x91\xE2\x96\x91\xE2\x96\x91\xE2\x96\x91\xE2\x96\x88\xE2\x96\x88\xE2\x96\x88\xE2\x96\x88\xE2\x96\x88\xE2\x96\x91\xE2\x96\x91\n"
    "\xE2\x96\x91\xE2\x96\x80\xE2\x96\x80\xE2\x96\x80\xE2\x96\x91\xE2\x96\x80\xE2\x96\x91\xE2\x96\x80\xE2\x96\x91\xE2\x96\x80\xE2\x96\x91\xE2\x96\x80"
    "\xE2\x96\x91\xE2\x96\x91\xE2\x96\x91\xE2\x96\x84\xE2\x96\x80\xE2\x96\x84\xE2\x96\x80\xE2\x96\x84\xE2\x96\x80\xE2\x96\x84\xE2\x96\x91\n"
;

} // anon

struct need_help_error : std::exception {};

class App
{
public:
    App() = default;
    ~App() = default;
    App(App const&) = delete;
    App& operator=(App const&) = delete;

    int main(int const argc, char const* const argv[]) try
    {
        saya::ika::compiler_assets::bridge brg;

        try {
            brg.init(argc, argv);

            // check first
            if (brg.no_color()) {
                l_.color(false);
            }

            if (brg.help()) throw need_help_error{};

            if (brg.input_files().empty()) {
                l_.error() << "no input files" << std::endl;
                throw need_help_error{};
            }

        } catch (need_help_error const&) {
            std::cout
                << boost::format("\n%s\nikac [options] [input-files]\n%s")
                    % LOGO % brg.option_group_visible()
                << std::endl
            ;
            return EXIT_FAILURE;

        } catch (std::exception const& e) {
            l_.error() << e.what() << std::endl;
            return EXIT_FAILURE;
        }

        // compile ----------------------------------------------

        saya::ika::compiler
        compiler{l_.env(), saya::ika::compiler_options{
            brg.ikastd(),
        }};

        saya::ika::evaluator
        evaluator{
            l_.env()
        };

        // for each input files
        for (auto const& path : brg.input_files()) try {
            auto const bundle = saya::ika::make_source(path);
            compiler.compile(bundle);

            // try {
            //     evaluator.eval(*root);
            // } catch (saya::ika::eval_error const& e) {
            //     l_.error() << e.what() << std::endl;
            //     return EXIT_FAILURE;
            // }

        } catch (saya::io_error const& e) {
            l_.error() << e.what() << std::endl;
            return EXIT_FAILURE;

        } catch (saya::ika::compile_error const& e) {
            l_.error() << e.what() << std::endl;
            return EXIT_FAILURE;
        }

        return EXIT_SUCCESS;

    } catch (std::exception const& e) {
        l_.note() << e.what() << std::endl;
        l_.error() << "[BUG] internal compiler error" << std::endl;
        return EXIT_FAILURE;

    } catch (...) {
        l_.note() << "No information is available, and there is nothing you can do." << std::endl;
        l_.error() << "[BUG] internal compiler error" << std::endl;
        return EXIT_FAILURE;
    }

private:
    saya::logger l_{"ikac"};
};

int main(int argc, char* argv[])
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);

    saya::stream_lock::Init();

    {
        saya::ika::vm::TypeID::init_type init_;
        boost::ignore_unused(init_);
    }
    {
        saya::ika::ast::NSID::init_type init_;
        boost::ignore_unused(init_);
    }

    App app;
    return app.main(argc, argv);
}
