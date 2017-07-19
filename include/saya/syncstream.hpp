#ifndef SAYA_SYNCSTREAM_HPP_
#define SAYA_SYNCSTREAM_HPP_

#include "saya/syncstream_fwd.hpp"
#include "saya/stream_lock.hpp"

#include "boost/assert.hpp"

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
        , mptr_(stream_lock::get_lock(sbp_))
    {}

    explicit basic_syncbuf(basic_syncbuf&& rhs) noexcept
        : stringbuf_type(std::move(*static_cast<stringbuf_type*>(std::addressof(rhs))))
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
                stream_lock::release_lock(mptr_, sbp_);
            } catch (...) {
                // ...
            }

            stringbuf_type::operator=(std::move(rhs));
            sbp_ = nullptr;
            mptr_.reset();
            need_flush_ = false;

            using std::swap;
            swap(sbp_, rhs.sbp_);
            swap(mptr_, rhs.mptr_);
        }
        return *this;
    }

    virtual ~basic_syncbuf() noexcept
    {
        try {
            this->emit();
        } catch (...) {
            // ...
        }

        try {
            stream_lock::release_lock(mptr_, sbp_);
        } catch (...) {
            // ...
        }
    }

    bool emit()
    {
        bool res = true;
        bool const precond = sbp_ && mptr_;
        auto const len = this->showmanyc();

        if (precond && (len > 0 || need_flush_)) {
            stream_lock::lock_type lock(*mptr_);

            if (len > 0 && len != sbp_->sputn(this->pbase(), len)) {
                res = false;
            }

            if (need_flush_) {
                if (sbp_->pubsync() == -1) {
                    res = false;
                }
            }
        }

        need_flush_ = false;
        this->str({});
        return res;
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
        if (this->force_flush()) {
            this->emit();
        } else {
            need_flush_ = true;
        }
        return 0;
    }

    virtual bool force_flush() const { return false; }

private:
    streambuf_type* sbp_{nullptr};
    stream_lock::mutex_ptr_type mptr_;
    bool need_flush_{false};
};

} // saya

template<class CharT, class Traits, class Allocator>
inline void swap(saya::basic_syncbuf<CharT, Traits, Allocator>& lhs, saya::basic_syncbuf<CharT, Traits, Allocator>& rhs)
{
    lhs.swap(rhs);
}


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

    explicit basic_osyncstream(ostream_type& os)
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

    explicit basic_osyncstream(basic_osyncstream&& rhs) noexcept
        : ostream_type(std::move(*static_cast<ostream_type*>(std::addressof(rhs))))
        , buf_(std::move(rhs.buf_))
    {
        ostream_type::init(&buf_);
    }

    virtual ~basic_osyncstream() noexcept {}

    basic_osyncstream& operator=(basic_osyncstream&& rhs) noexcept
    {
        if (this != std::addressof(rhs)) {
            ostream_type::operator=(std::move(*static_cast<ostream_type*>(std::addressof(rhs))));
            buf_ = std::move(rhs.buf_);
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

    void emit()
    {
        typename ostream_type::sentry const s{*this};
        if (!s || !buf_.emit()) {
            this->setstate(std::ios::badbit);
        }
    }

private:
    syncbuf_type buf_;
};

} // saya

template<class CharT, class Traits, class Allocator>
inline void swap(saya::basic_osyncstream<CharT, Traits, Allocator>& lhs, saya::basic_osyncstream<CharT, Traits, Allocator>& rhs)
{
    lhs.swap(rhs);
}

#endif
