#ifndef SAYA_STREAM_LOCK_HPP_
#define SAYA_STREAM_LOCK_HPP_

#include <boost/core/ignore_unused.hpp>
#include <boost/assert.hpp>

#include <unordered_map>
#include <mutex>
#include <memory>


namespace saya {

// NB: To use this entire feature, user must call `stream_lock::Init()`
// before using any saya functions AND only after possible sync_with_stdio()

class stream_lock
{
public:
    using mutex_type = std::mutex;
    using lock_type = std::lock_guard<mutex_type>;
    using mutex_ptr_type = std::shared_ptr<mutex_type>;
    using mutex_wptr_type = std::weak_ptr<mutex_type>;
    using sbuf_type = void;

    static void Init()
    {
        boost::ignore_unused(global_m_());
        boost::ignore_unused(mmap_());
    }

    static mutex_ptr_type get_mutex(sbuf_type* sbptr)
    {
        BOOST_ASSERT(sbptr);

        lock_type lock(global_m_());
        mutex_ptr_type res;
        mutex_wptr_type& mwp = mmap_()[sbptr];

        if (mwp.expired()) {
            mwp = res = std::make_shared<mutex_type>();
        } else {
            res = mwp.lock();
        }
        return res;
    }

    static void release_mutex(mutex_ptr_type& mp, sbuf_type* sbptr)
    {
        BOOST_ASSERT(mp);
        BOOST_ASSERT(sbptr);

        lock_type lock(global_m_());
        auto const it = mmap_().find(sbptr);
        BOOST_ASSERT(it != mmap_().end());

        // should we do this here, or leave it to the caller?
        mp.reset();

        if (it->second.expired()) {
            mmap_().erase(it);
        }
    }

private:
    using mmap_type = std::unordered_map<sbuf_type*, mutex_wptr_type>;

    static mutex_type& global_m_()
    {
        static mutex_type i;
        return i;
    }

    static mmap_type& mmap_()
    {
        static mmap_type i;
        return i;
    }
};

} // saya

#endif
