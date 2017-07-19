#ifndef SAYATEST_BRIDGE_HPP_
#define SAYATEST_BRIDGE_HPP_

#include "saya/bridge.hpp"
#include "boost/filesystem/path.hpp"

namespace saya_test {
namespace bridge_detail {

class bridge : public saya::bridge<bridge>
{
public:
    static bridge& get() { static bridge instance; return instance; }
    void validate(vm_type const&) override {}

//#undef SAYA_BRIDGE_OPTION_GROUP
//#define SAYA_BRIDGE_OPTION_GROUP (general, "General options")

#undef SAYA_BRIDGE_OPTION_GROUP
#define SAYA_BRIDGE_OPTION_GROUP (misc, "Miscellaneous options")
    SAYA_BRIDGE_OPTION_DEFAULTED(std::string, gtest_color, "yes", "gtest_color", "Enables Google Test color (yes/no)")

#undef SAYA_BRIDGE_OPTION_GROUP
};

} // bridge_detail_

inline static bridge_detail::bridge& bridge() { return bridge_detail::bridge::get(); }

} // saya_test

#endif
