#ifndef SAYA_ZED_DESTRUCTOR_HANDLER_HPP
#define SAYA_ZED_DESTRUCTOR_HANDLER_HPP

#include <iosfwd>
#include <deque>
#include <functional>

#include <boost/assert.hpp>

namespace saya { namespace zed {

using destructor_func_type = std::function<void ()>;


class destructor_handler;

inline void destructor_register(destructor_handler& dh, destructor_func_type f);
inline void destructor_call(destructor_handler& dh);
inline std::ostream& operator<<(std::ostream& os, destructor_handler const& dh);


class destructor_handler final
{
public:
    friend inline void destructor_register(destructor_handler& dh, destructor_func_type f)
    {
        // FRONT is important...
        dh.funcs_.push_front(std::move(f));
    }

    friend inline void destructor_call(destructor_handler& dh)
    {
        #if !defined(NDEBUG)
        if (dh.called_) {
            BOOST_ASSERT(!"destructor_handler invoked more than once");
        }
        dh.called_ = true;
        #endif

        for (auto const& f : dh.funcs_) {
            f();
        }
    }

    friend inline std::ostream&
    operator<<(std::ostream& os, destructor_handler const& dh)
    {
        return os << "(destructor handler: " << dh.funcs_.size() << " registered)";
    }

    destructor_handler() = default;
    ~destructor_handler() = default;

    destructor_handler(destructor_handler const&) = delete;
    destructor_handler(destructor_handler&&) = delete;

    destructor_handler& operator=(destructor_handler const&) = delete;
    destructor_handler& operator=(destructor_handler&&) = delete;

private:
    std::deque<destructor_func_type> funcs_;

    #if !defined(NDEBUG)
    bool called_{false};
    #endif
};

}} // saya

#endif
