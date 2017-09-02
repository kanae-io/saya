#ifndef SAYA_LOGGER_STREAM_HPP_
#define SAYA_LOGGER_STREAM_HPP_

#include "saya/logger_fwd.hpp"
#include "saya/logger/level.hpp"
#include "saya/logger/level_traits.hpp"
#include "saya/syncstream.hpp"

#include <boost/format.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string/join.hpp>

#include <iomanip>
#include <ostream>
#include <sstream>
#include <string>


namespace saya {
namespace detail {

template<class Logger>
class logger_access
{
protected:
    static inline auto& out(Logger* logger) noexcept { return logger->env_.out; }

    static inline auto& err(Logger* logger) noexcept { return logger->env_.err; }

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

    template<class Level, class Notes>
    static inline typename Logger::string_type
    format_prompt(Logger* logger, typename Logger::string_type frag, Notes notes) /*noexcept*/
    {
        using traits_type = typename Logger::traits_type;
        using color_type = typename traits_type::color_type;
        using format_type = boost::basic_format<typename Logger::char_type>;
        using level_traits_type = typename traits_type::template level_traits_type<Level>;

        if (frag.empty()) return {};

        std::unique_lock<typename Logger::mutex_type> lock(logger->prompt_mtx_);
        auto const prompt = logger->prompt_;
        auto const need_color = logger->env_.need_color;
        lock.unlock();

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
        typename Logger::string_type notes_buf;
        if (!notes.empty()) {
            using note_traits = basic_logger_level_traits<typename Logger::char_type, logger_level::NOTE>;

            if (need_color) {
                notes_buf = boost::algorithm::join(notes | boost::adaptors::transformed([&prompt] (auto const& note) {
                    return boost::str(format_type(traits_type::note_format())
                        % boost::io::group(std::setw(traits_type::note_indent_w(prompt)), ' ')
                        % note_traits::template color<color_type>()
                        % note_traits::label()
                        % (typename Logger::string_type(color_type::RESET()) + color_type::fg::LIGHTGRAY())
                        % boost::algorithm::trim_copy(note.str())
                    );
                }), "\n");

            } else {
                notes_buf = boost::algorithm::join(notes | boost::adaptors::transformed([&prompt] (auto const& note) {
                    return boost::str(format_type(traits_type::note_no_color_format())
                        % boost::io::group(std::setw(traits_type::max_prompt_len(prompt) + 2), note_traits::label())
                        % boost::algorithm::trim_copy(note.str())
                    );
                }), "\n");
            }
            notes.clear();
        }

        // INFO, WARN, ERROR
        if (prompt.empty()) {
            if (need_color) {
                typename Logger::string_type const PADDING(traits_type::max_label_width() - level_traits_type::label_len(), ' ');

                return boost::str(format_type(traits_type::noprompt_format())
                    % level_traits_type::template color<color_type>()
                    % level_traits_type::label()
                    % (color_type::RESET() + PADDING)
                    % frag
                    % notes_buf
                );

            } else {
                return boost::str(format_type(traits_type::template noprompt_no_color_format<Level>())
                    % level_traits_type::label()
                    % frag
                    % notes_buf
                );
            }

        } else {
            typename Logger::string_type const PADDING(traits_type::max_label_width() - level_traits_type::label_len(), ' ');

            if (need_color) {
                return boost::str(format_type(traits_type::prompt_format())
                    % (logger->prompt_color_ ? logger->prompt_color_ : "")
                    % prompt
                    % color_type::RESET()
                    % level_traits_type::template color<color_type>()
                    % level_traits_type::label()
                    % (color_type::RESET() + PADDING)
                    % frag
                    % notes_buf
                );
            } else {
                return boost::str(format_type(traits_type::template prompt_no_color_format<Level>())
                    % prompt
                    % level_traits_type::label()
                    % (PADDING + frag)
                    % notes_buf
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
    using buf_type = basic_syncbuf<char_type>;

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

    template<class Notes>
    void finalize(Notes& notes)
    {
        buf_.base()->str(access_type::template format_prompt<Level>(
            logger_, buf_.base()->str(), std::move(notes)
        ));
    }

    buf_type buf_;
    logger_type* logger_;
};

} // detail

template<class Logger, class Level>
class basic_logger_stream final
    : public detail::basic_logger_stream_impl<Logger, Level>
{
    using base_type = detail::basic_logger_stream_impl<Logger, Level>;

public:
    using base_type::base_type;
    basic_logger_stream(basic_logger_stream&&) = default;

    virtual ~basic_logger_stream()
    {
        auto& note_mtx = base_type::access_type::note_mtx(this->logger_);
        typename Logger::lock_type lock(note_mtx);

        this->finalize(base_type::access_type::notes(this->logger_));
        this->buf_.emit();
    }
};

template<class Logger>
class basic_logger_stream<Logger, logger_level::NOTE>
    : public std::basic_ostringstream<typename Logger::char_type>
{
    using base_type = std::basic_ostringstream<typename Logger::char_type>;

public:
    using base_type::base_type;
};

} // saya

#endif
