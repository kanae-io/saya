#ifndef SAYA_IKA_COMPILER_BRIDGE_HPP
#define SAYA_IKA_COMPILER_BRIDGE_HPP

#include "saya/bridge.hpp"

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>

#include <functional>

namespace saya { namespace ika { namespace compiler_assets {

class bridge final : public saya::bridge<bridge>
{
public:
    #undef SAYA_BRIDGE_OPTION_GROUP
    #define SAYA_BRIDGE_OPTION_GROUP (compiler, "Compiler options")
    SAYA_BRIDGE_OPTION_EXISTENCE(fsyntax_only, "fsyntax-only", "Perform syntax check only")
    #undef SAYA_BRIDGE_OPTION_GROUP

    #undef SAYA_BRIDGE_OPTION_GROUP
    #define SAYA_BRIDGE_OPTION_GROUP (optimizer, "Optimizer options")
    SAYA_BRIDGE_OPTION_DEFAULTED(unsigned, O, 2u, "optimization-level,O", "Optimization levels\n  2: [default] full compile time evaluation")
    #undef SAYA_BRIDGE_OPTION_GROUP

    #undef SAYA_BRIDGE_OPTION_GROUP
    #define SAYA_BRIDGE_OPTION_GROUP (misc, "Miscellaneous options")
    SAYA_BRIDGE_OPTION_EXISTENCE(help, "help", "Print this help and exit")
    SAYA_BRIDGE_OPTION_EXISTENCE(no_color, "no-color", "Disable ANSI escape codes")
    #undef SAYA_BRIDGE_OPTION_GROUP

    #undef SAYA_BRIDGE_OPTION_GROUP
    #define SAYA_BRIDGE_OPTION_GROUP (positional, "Positional arguments")
    SAYA_BRIDGE_OPTION_POSITIONAL(
        std::vector<boost::filesystem::path>,
        input_files, "input-files",
        "Input files"
    )

protected:
    virtual void validate(base_type::vm_type const& vm) override
    {
        if (vm.count("help")) return;

        using eval_func = std::function<bool (boost::program_options::variable_value const&)>;
        auto const only_if = [&vm] (char const* option, eval_func f) {
            if (vm.count(option)) {
                if (!f(vm.at(option))) {
                    throw boost::program_options::validation_error(boost::program_options::validation_error::invalid_option_value);
                }
            }
        };

        only_if("optimization-level", [this] (auto const&) {
            if (O_ != 2) return false;
            return true;
        });
    }
};

}}} // saya

#endif
