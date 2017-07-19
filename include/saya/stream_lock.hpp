#ifndef SAYA_STREAM_LOCK_HPP_
#define SAYA_STREAM_LOCK_HPP_

#include <unordered_map>
#include <mutex>
#include <memory>

namespace saya {

class stream_lock
{
public:
    using mutex_type = std::mutex;
    using lock_type = std::lock_guard<mutex_type>;
    using mutex_ptr_type = std::shared_ptr<mutex_type>;
    using mutex_wptr_type = std::weak_ptr<mutex_type>;
    using sbuf_type = void;

    static mutex_ptr_type get_lock(sbuf_type* sbptr)
    {
        if (!sbptr) return {};
        lock_type lock(m());
        mutex_ptr_type res;
        mutex_wptr_type& mwp = mtx_for_buf()[sbptr];

        if (mwp.expired()) {
            mwp = res = std::make_shared<mutex_type>();
        } else {
            res = mwp.lock();
        }
        return res;
    }

    static void release_lock(mutex_ptr_type& mp, sbuf_type* sbptr)
    {
        if (!mp && !sbptr) return;
        lock_type lock(m());
        auto const it = mtx_for_buf().find(sbptr);
        mp.reset();

        if (it != mtx_for_buf().end() && it->second.expired()) {
            mtx_for_buf().erase(it);
        }
    }

private:
    static mutex_type&
    m()
    {
        static mutex_type i;
        return i;
    }
    static std::unordered_map<sbuf_type*, mutex_wptr_type>&
    mtx_for_buf()
    {
        static std::unordered_map<sbuf_type*, mutex_wptr_type> i;
        return i;
    }
};

} // saya

#endif
