#ifndef SAYA_SYNCSTREAM_HPP_
#define SAYA_SYNCSTREAM_HPP_

#include "saya/syncstream_fwd.hpp"
#include "saya/stream_lock.hpp"

#include <boost/assert.hpp>

#include <ostream>
#include <sstream>
#include <stdexcept>


namespace saya {

template<class CharT, class Traits, class Allocator>
class basic_syncbuf : public std::basic_stringbuf<CharT, Traits, Allocator>
{
public:
    using streambuf_type = std::basic_streambuf<CharT, Traits>;
    using stringbuf_type = std::basic_stringbuf<CharT, Traits, Allocator>;
    using string_type = std::basic_string<CharT>;
    using int_type = typename stringbuf_type::int_type;
    using off_type = typename stringbuf_type::off_type;
    using pos_type = typename stringbuf_type::pos_type;

    basic_syncbuf() = default;

    explicit basic_syncbuf(streambuf_type* out)
        : stringbuf_type()
        , sbp_(out)
        , mptr_(stream_lock::get_mutex(sbp_))
    {}

    basic_syncbuf(basic_syncbuf&& rhs) noexcept
        : stringbuf_type(std::move(rhs))
        , sbp_(std::move(rhs.sbp_))
        , mptr_(std::move(rhs.mptr_))
        , need_flush_(std::move(rhs.need_flush_))
    {}

    basic_syncbuf(basic_syncbuf const&) = delete;

    basic_syncbuf& operator=(basic_syncbuf&& rhs) noexcept
    {
        if (this != std::addressof(rhs)) {
            try {
                this->emit();
            } catch (...) {
                // ...
            }
            try {
                if (mptr_) stream_lock::release_mutex(mptr_, sbp_);
            } catch (...) {
                // ...
            }

            stringbuf_type::operator=(std::move(rhs));
            sbp_ = std::exchange(rhs.sbp_, nullptr);
            mptr_ = std::exchange(rhs.mptr_, nullptr);
            need_flush_ = std::exchange(rhs.need_flush_, false);
        }
        return *this;
    }

    virtual ~basic_syncbuf()
    {
        this->emit();
    }

    void emit() try
    {
        if (!sbp_) return;

        auto const len = this->showmanyc();
        stream_lock::lock_type lock(*mptr_);

        if (len > 0) {
            sbp_->sputn(this->pbase(), len);
        }
        if (need_flush_) {
            sbp_->pubsync();
        }

        stream_lock::release_mutex(mptr_, sbp_);
        sbp_ = nullptr;

    } catch (...) {
        try {
            if (mptr_) stream_lock::release_mutex(mptr_, sbp_);
        } catch (...) {
            // ...
        }
        sbp_ = nullptr;
        throw;
    }

    void swap(basic_syncbuf& rhs) noexcept
    {
        using std::swap;

        if (this != std::addressof(rhs)) {
            stringbuf_type::swap(rhs);
            swap(sbp_, rhs.sbp_);
            swap(mptr_, rhs.mptr_);
            swap(need_flush_, rhs.need_flush_);
        }
    }

    stringbuf_type* base() noexcept { return this; }
    stringbuf_type const* base() const noexcept { return this; }

protected:
    virtual int sync() override
    {
        need_flush_ = true;
        return 0;
    }

    // virtual bool force_flush() const { return false; }

private:
    streambuf_type* sbp_{nullptr};
    stream_lock::mutex_ptr_type mptr_;
    bool need_flush_{false};
};

} // saya


namespace saya {

template<
    class CharT, class Traits, class Allocator,
    class Syncbuf
>
class basic_osyncstream
    : public std::basic_ostream<CharT, Traits>
{
public:
    using char_type = CharT;
    using traits_type = Traits;
    using allocator_type = Allocator;
    using syncbuf_type = Syncbuf;
    using ostream_type = std::basic_ostream<CharT, Traits>;
    using streambuf_type = std::basic_streambuf<CharT, Traits>;

    basic_osyncstream(ostream_type& os)
        : ostream_type()
        , buf_(os.rdbuf())
    {
        ostream_type::init(&buf_);
    }

    explicit basic_osyncstream(streambuf_type* out = nullptr)
        : ostream_type()
        , buf_(out)
    {
        ostream_type::init(&buf_);
    }

    basic_osyncstream(basic_osyncstream&& rhs) noexcept
        : ostream_type(std::move(rhs))
        , buf_(std::move(rhs.buf_))
    {
        ostream_type::init(&buf_);
    }

    virtual ~basic_osyncstream() = default;

    basic_osyncstream& operator=(basic_osyncstream&& rhs) noexcept
    {
        if (this != std::addressof(rhs)) {
            ostream_type::operator=(std::move(rhs));
            buf_ = std::exchange(rhs.buf_, nullptr);
        }

        return *this;
    }

    void swap(basic_osyncstream& rhs) noexcept
    {
        using std::swap;

        if (this != std::addressof(rhs)) {
            ostream_type::swap(rhs);
            swap(buf_, rhs.buf_);
        }
    }

    void emit() try
    {
        typename ostream_type::sentry const s{*this};
        if (!s) {
            this->setstate(std::ios::badbit);
        } else {
            buf_.emit();
        }

    } catch (...) {
        this->setstate(std::ios::badbit);
        throw;
    }

private:
    syncbuf_type buf_;
};

} // saya

#endif
