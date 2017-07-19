#ifndef SAYA_LOGGER_STREAM_HPP_
#define SAYA_LOGGER_STREAM_HPP_

#include "saya/logger_fwd.hpp"
#include "saya/logger/level.hpp"
#include "saya/logger/level_traits.hpp"
#include "saya/syncstream.hpp"

#include "boost/format.hpp"

#include <ostream>

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

    template<class LevelTraits>
    static inline typename Logger::string_type
    format_prompt(Logger* logger, typename Logger::string_type const& frag) /*noexcept*/
    {
        using traits_type = typename Logger::traits_type;
        using color_type = typename traits_type::color_type;
        using format_type = boost::basic_format<typename Logger::char_type>;

        std::unique_lock<typename Logger::mutex_type> lock(logger->mtx_);
        auto const& prompt = logger->prompt_;

        if (prompt.empty()) {
            lock.unlock();

            if (logger->color_) {
                return boost::str(format_type(traits_type::noprompt_format())
                    % LevelTraits::template color<color_type>()
                    % LevelTraits::label()
                    % color_type::RESET()
                    % frag
                );
            } else {
                return boost::str(format_type(traits_type::noprompt_no_color_format())
                    % LevelTraits::label()
                    % frag
                );
            }

        } else {
            if (logger->color_) {
                return boost::str(format_type(traits_type::prompt_format())
                    % prompt
                    % LevelTraits::template color<color_type>()
                    % LevelTraits::label()
                    % color_type::RESET()
                    % frag
                );
            } else {
                return boost::str(format_type(traits_type::prompt_no_color_format())
                    % prompt
                    % LevelTraits::label()
                    % frag
                );
            }
        }
    }
}; // logger_access
} // detail


template<class Logger, class Level>
class basic_logger_stream
    : public std::basic_ostream<typename Logger::char_type>
    , detail::logger_access<Logger>
{
public:
    using logger_type = Logger;
    using char_type = typename Logger::char_type;
    using access_type = detail::logger_access<logger_type>;
    using ostream_type = std::basic_ostream<char_type>;
    using syncbuf_type = basic_syncbuf<char_type>;

    /*explicit*/ basic_logger_stream(logger_type* logger)
        : ostream_type()
        , buf_(access_type::select_stream(logger, Level{}))
        , logger_(logger)
    {
        this->init(&buf_);
    }

    ~basic_logger_stream()
    {
        buf_.base()->str(access_type::template format_prompt<
            basic_logger_level_traits<char_type, Level>
        >(logger_, buf_.base()->str()));
    }

    basic_logger_stream(basic_logger_stream const&) = delete;
    basic_logger_stream(basic_logger_stream&&) = default;

private:
    syncbuf_type buf_;
    logger_type* logger_;
};

} // saya

#endif
