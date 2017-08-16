#ifndef SAYA_LOGGER_STREAM_HPP_
#define SAYA_LOGGER_STREAM_HPP_

#include "saya/logger_fwd.hpp"
#include "saya/logger/level.hpp"
#include "saya/logger/level_traits.hpp"
#include "saya/syncstream.hpp"

#include <boost/format.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <ostream>
#include <string>


namespace saya {
namespace detail {

template<class Logger>
class logger_access
{
protected:
    static inline auto& out(Logger* logger) noexcept { return logger->out_; }

    static inline auto& err(Logger* logger) noexcept { return logger->err_; }

    static inline auto& select_stream(Logger* logger, logger_level::INFO) noexcept
    {
        return out(logger);
    }
    static inline auto& select_stream(Logger* logger, logger_level::WARN) noexcept
    {
        return err(logger);
    }
    static inline auto& select_stream(Logger* logger, logger_level::ERROR) noexcept
    {
        return err(logger);
    }
    static inline auto& select_stream(Logger* logger, logger_level::NOTE) noexcept
    {
        return err(logger);
    }

    static inline typename Logger::mutex_type& note_mtx(Logger* logger) noexcept
    {
        return logger->note_mtx_;
    }

    static inline typename Logger::notes_type& notes(Logger* logger) noexcept
    {
        return logger->notes_;
    }

    template<class Level>
    static inline typename Logger::string_type
    format_prompt(Logger* logger, typename Logger::string_type frag) /*noexcept*/
    {
        using traits_type = typename Logger::traits_type;
        using color_type = typename traits_type::color_type;
        using format_type = boost::basic_format<typename Logger::char_type>;
        using level_traits_type = typename traits_type::template level_traits_type<Level>;

        std::unique_lock<typename Logger::mutex_type> lock(logger->prompt_mtx_);
        auto const& prompt = logger->prompt_;

        if (frag.empty()) return {};

        auto const eol = frag.find('\n');

        if (eol != Logger::string_type::npos) {
            if (eol != frag.size() - 1) {
                // multi-line mode...
                frag = "\n" + frag;
            }
        }

        boost::algorithm::trim_right(frag);
        frag += '\n';

        // 'note: '
        if (level_traits_type::need_indent()) {
            typename Logger::string_type const INDENT(traits_type::indent_magic(prompt), ' ');
            typename Logger::string_type const PADDING(traits_type::max_label_width() + 2 - level_traits_type::label_len(), ' ');

            if (logger->color_.load(std::memory_order_acquire)) {
                return boost::str(format_type(traits_type::note_format())
                    % INDENT
                    % level_traits_type::template color<color_type>()
                    % level_traits_type::label()
                    % (typename Logger::string_type(color_type::RESET()) + color_type::fg::LIGHTGRAY())
                    % frag
                );

            } else {
                return boost::str(format_type(traits_type::note_no_color_format())
                    % (INDENT + PADDING)
                    % level_traits_type::label()
                    % frag
                );
            }
        }

        // INFO, WARN, ERROR
        if (prompt.empty()) {
            lock.unlock();

            if (logger->color_.load(std::memory_order_acquire)) {
                typename Logger::string_type const PADDING(traits_type::max_label_width() - level_traits_type::label_len(), ' ');

                return boost::str(format_type(traits_type::noprompt_format())
                    % level_traits_type::template color<color_type>()
                    % level_traits_type::label()
                    % (color_type::RESET() + PADDING)
                    % frag
                );

            } else {
                return boost::str(format_type(traits_type::template noprompt_no_color_format<Level>())
                    % level_traits_type::label()
                    % frag
                );
            }

        } else {
            typename Logger::string_type const PADDING(traits_type::max_label_width() - level_traits_type::label_len(), ' ');

            if (logger->color_.load(std::memory_order_acquire)) {
                return boost::str(format_type(traits_type::prompt_format())
                    % (logger->prompt_color_ ? logger->prompt_color_ : "")
                    % prompt
                    % color_type::RESET()
                    % level_traits_type::template color<color_type>()
                    % level_traits_type::label()
                    % (color_type::RESET() + PADDING)
                    % frag
                );
            } else {
                return boost::str(format_type(traits_type::template prompt_no_color_format<Level>())
                    % prompt
                    % level_traits_type::label()
                    % (PADDING + frag)
                );
            }
        }
    }
}; // logger_access

template<class Logger, class Level>
class basic_logger_stream_impl
    : public std::basic_ostream<typename Logger::char_type>
    , protected detail::logger_access<Logger>
{
public:
    using logger_type = Logger;
    using char_type = typename Logger::char_type;
    using ostream_type = std::basic_ostream<char_type>;
    using syncbuf_type = basic_syncbuf<char_type>;

    /*explicit*/ basic_logger_stream_impl(logger_type* logger)
        : ostream_type()
        , buf_(access_type::select_stream(logger, Level{}))
        , logger_(logger)
    {
        this->init(&buf_);
    }

    virtual ~basic_logger_stream_impl() = default;

    basic_logger_stream_impl(basic_logger_stream_impl const&) = delete;
    basic_logger_stream_impl(basic_logger_stream_impl&&) = default;

protected:
    using access_type = detail::logger_access<logger_type>;

    void default_destruct()
    {
        buf_.base()->str(access_type::template format_prompt<Level>(
            logger_, buf_.base()->str()
        ));
    }

    syncbuf_type buf_;
    logger_type* logger_;
};
} // detail

template<class Logger, class Level>
class basic_logger_stream final : public detail::basic_logger_stream_impl<Logger, Level>
{
    using base_type = detail::basic_logger_stream_impl<Logger, Level>;

public:
    using base_type::base_type;
    basic_logger_stream(basic_logger_stream&&) = default;

    virtual ~basic_logger_stream()
    {
        // print self
        this->default_destruct();
        this->buf_.emit();

        // print 'NOTE'

        auto& notes = base_type::access_type::notes(this->logger_);
        while (!notes.empty()) {
            notes.pop_front();
        }
    }
};

template<class Logger>
class basic_logger_stream<Logger, logger_level::NOTE> : public detail::basic_logger_stream_impl<Logger, logger_level::NOTE>
{
    using base_type = detail::basic_logger_stream_impl<Logger, logger_level::NOTE>;

public:
    using base_type::base_type;

    basic_logger_stream(basic_logger_stream&&) = default;
    virtual ~basic_logger_stream() { this->default_destruct(); }
};

} // saya

#endif
