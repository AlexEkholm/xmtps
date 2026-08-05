/** The @ref util::log::Log class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:16:21 PM
 *
 *  @author     Mr. Alexander K. Ekholm <aekholm@whoi.edu>,
 *              Engineer I,
 *              Department of Physical Oceanography,
 *              Woods Hole Oceanographic Institution
 *
 *  @copyright  2014 Woods Hole Oceanographic Institution.
 *              Released under the GNU General Public License.
 *
 *  This is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 *  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details. You should have received a copy of the GNU General
 *  Public License along with this software. If not, please visit http://www.gnu.org/licenses/.
 */

#ifndef LOG_H_
#define LOG_H_

/* Includes */

// local
#include "MessageException.h"

// boost filesystem
#include <boost/filesystem.hpp>
namespace bfs = boost::filesystem;

// boost postix_time
#include <boost/date_time/posix_time/posix_time_types.hpp>

// boost log
#include <boost/log/attributes/attribute.hpp>
#include <boost/log/attributes/attribute_cast.hpp>
#include <boost/log/attributes/attribute_name.hpp>
#include <boost/log/attributes/attribute_value.hpp>
#include <boost/log/attributes/scoped_attribute.hpp>
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/value_ref.hpp>
namespace blg = boost::log;

/* Definitions */

#define LOG_INSTRUMENT_SPECIFIC ( false )

/** The default logging level to use, if none is specified in the run-time configuration. */
#define DEFAULT_LOG_LEVEL       util::log::LOG_LOG

/** The number of unique log level names.
 *
 *  @deprecated     Log names are no long used.
 */
#define N_LOG_LEVEL_NAMES       6

// boost log attribute keyword definitions
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_sevr, "Severity", blg::trivial::severity_level)
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_dac, "DacID", int)
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_dive, "DiveID", int)
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_momsn, "MomsnID", int)
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_muid, "MsgUID", int)
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_pckt, "PcktID", int)
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_xmid, "XmsgID", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_whoi, "WhoiID", int)
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_wmo, "WmoID", int)
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_tname, "ThreadName", std::string)
BOOST_LOG_ATTRIBUTE_KEYWORD(attr_tnum, "ThreadNum", int)
#pragma GCC diagnostic pop

/*  Definitions - Macros:  */


/** Returns a char* of the value of a given preprocessor definition. */
#   define X_PP_STR(x)  #x

/** Returns a char* of the name of a given preprocessor definition. */
#   define PP_STR(x)    X_PP_STR(x)

/*  Definitions - Macros - Log: */

#   define LOG_SET_SCOPED_THREAD_NAME(NAME) \
    BOOST_LOG_SCOPED_THREAD_ATTR("ThreadName", boost::log::attributes::constant<string>(NAME));

/** Returns output stream for given log level. */
#   define LOG(level)  if (level > util::log::Log::log_reporting_level()) { /* Do nothing. */ }\
                        else util::log::Log(level)

/** Returns an output stream for a fatal error message. */
#   define lfat()      LOG(util::log::LOG_FAT)

/** Returns an output stream for an error message. */
#   define lerr()      LOG(util::log::LOG_ERR)

/** Returns an output stream for a warning message. */
#   define lwrn()      LOG(util::log::LOG_WRN)

/** Returns an output stream for a log message. */
#   define lout()      LOG(util::log::LOG_LOG)

/** Returns an output stream for a log message. */
#   define llog()      LOG(util::log::LOG_LOG)

/** Returns an output stream for an info message. */
#   define linf()      LOG(util::log::LOG_INF)

/** Returns an output stream for a debug message. */
#   define ldbg()      LOG(util::log::LOG_DBG)

/** Returns an output stream for a debug trace level 1 message. */
#   define dbg1()      LOG(util::log::LOG_DBG1) << __PRETTY_FUNCTION__ << ": "

/** Returns an output stream for a debug trace level 2 message. */
#   define dbg3()      LOG(util::log::LOG_DBG3) << __PRETTY_FUNCTION__ << ": "

/** Returns an output stream for a debug trace level 3 message. */
#   define dbg2()      LOG(util::log::LOG_DBG2) << __PRETTY_FUNCTION__ << ": "

// exception handling

/** Return a string containing diagnostic information for a given exception, e. */
#   define exc_diag(e) boost::diagnostic_information(e, true)

/** Catch standard exception (no throw). */
#   define exc_catchStandard_noThrow(e, msg)\
do {\
    lerr() << msg << std::endl\
        << __FILE__ << "(" << __LINE__ << "): Throw in " << __PRETTY_FUNCTION__ << std::endl\
        << "\tCaused by: " << std::endl\
        << exc_diag(e);\
} while(0)

/** Catch standard exception (throws exception). */
#   define exc_catchStandard(e, msg)\
do {\
    BOOST_THROW_EXCEPTION(\
        util::log::MessageException(std::string(msg)+"\n\tCaused by:\n"+exc_diag(e))\
    );\
} while(0)

/** Catch arbitrary exception (no throw). */
#   define exc_catchAny_noThrow(msg)\
do {\
    lerr() << msg << std::endl\
        << " Caused by: " << __FILE__ << "(" << __LINE__ << "): "\
        << "Throw in " << __PRETTY_FUNCTION__;\
} while(0)

/** Catch arbitrary exception (throws exception). */
#   define exc_catchAny(msg)\
do {\
    exc_catchAny_noThrow(msg);\
    BOOST_THROW_EXCEPTION(util::log::MessageException(msg));\
} while(0)

/** Standardized catch block for exception handling (throws exception). */
#   define CATCH_BLOCK(msg)\
catch(std::regex_error& e) {\
    if(e.code() == std::regex_constants::error_backref) {\
        lerr() << "regex_error caught: error_backref";\
    } else if(e.code() == std::regex_constants::error_badbrace) {\
        lerr() << "regex_error caught: error_badbrace";\
    } else if(e.code() == std::regex_constants::error_badrepeat) {\
        lerr() << "regex_error caught: error_badrepeat";\
    } else if(e.code() == std::regex_constants::error_brace) {\
        lerr() << "regex_error caught: error_brace";\
    } else if(e.code() == std::regex_constants::error_brack) {\
        lerr() << "regex_error caught: error_brack";\
    } else if(e.code() == std::regex_constants::error_collate) {\
        lerr() << "regex_error caught: error_collate";\
    } else if(e.code() == std::regex_constants::error_complexity) {\
        lerr() << "regex_error caught: error_complexity";\
    } else if(e.code() == std::regex_constants::error_ctype) {\
        lerr() << "regex_error caught: error_ctype";\
    } else if(e.code() == std::regex_constants::error_escape) {\
        lerr() << "regex_error caught: error_escape";\
    } else if(e.code() == std::regex_constants::error_paren) {\
        lerr() << "regex_error caught: error_paren";\
    } else if(e.code() == std::regex_constants::error_range) {\
        lerr() << "regex_error caught: error_range";\
    } else if(e.code() == std::regex_constants::error_space) {\
        lerr() << "regex_error caught: error_space";\
    } else if(e.code() == std::regex_constants::error_stack) {\
        lerr() << "regex_error caught: error_stack";\
    }\
    exc_catchStandard(e, msg);\
} catch(std::exception& e) {\
    exc_catchStandard(e, msg);\
} catch(...) {\
    exc_catchAny(msg);\
}

/** Standardized catch block for exception handling (no throw). */
#   define CATCH_NOTHROW(msg)\
catch(std::regex_error& e) {\
    if(e.code() == std::regex_constants::error_backref) {\
        lerr() << "regex_error caught: error_backref";\
    } else if(e.code() == std::regex_constants::error_badbrace) {\
        lerr() << "regex_error caught: error_badbrace";\
    } else if(e.code() == std::regex_constants::error_badrepeat) {\
        lerr() << "regex_error caught: error_badrepeat";\
    } else if(e.code() == std::regex_constants::error_brace) {\
        lerr() << "regex_error caught: error_brace";\
    } else if(e.code() == std::regex_constants::error_brack) {\
        lerr() << "regex_error caught: error_brack";\
    } else if(e.code() == std::regex_constants::error_collate) {\
        lerr() << "regex_error caught: error_collate";\
    } else if(e.code() == std::regex_constants::error_complexity) {\
        lerr() << "regex_error caught: error_complexity";\
    } else if(e.code() == std::regex_constants::error_ctype) {\
        lerr() << "regex_error caught: error_ctype";\
    } else if(e.code() == std::regex_constants::error_escape) {\
        lerr() << "regex_error caught: error_escape";\
    } else if(e.code() == std::regex_constants::error_paren) {\
        lerr() << "regex_error caught: error_paren";\
    } else if(e.code() == std::regex_constants::error_range) {\
        lerr() << "regex_error caught: error_range";\
    } else if(e.code() == std::regex_constants::error_space) {\
        lerr() << "regex_error caught: error_space";\
    } else if(e.code() == std::regex_constants::error_stack) {\
        lerr() << "regex_error caught: error_stack";\
    }\
    exc_catchStandard_noThrow(e, msg);\
} catch(std::exception& e) {\
    exc_catchStandard_noThrow(e, msg);\
} catch(...) {\
    exc_catchAny_noThrow(msg);\
}

/** Throw an exception. */
#   define EXC_THROW(msg) BOOST_THROW_EXCEPTION(util::log::MessageException(std::string() + msg))

namespace util {
namespace config {
// forward declaration
class LocalConfiguration;
} /* namespace config */

namespace log {

namespace attr = boost::log::attributes;
namespace expr = boost::log::expressions;
namespace keys = boost::log::keywords;
using boost::algorithm::trim;
using boost::filesystem::is_directory;
using boost::filesystem::path;
using boost::format;
using boost::log::add_common_attributes;
using boost::log::core;
using boost::log::sinks::file::rotation_at_time_point;
using boost::log::sinks::file::as_file_name_composer;
using boost::log::sinks::synchronous_sink;
using boost::log::sinks::text_multifile_backend;
using std::make_shared;
using boost::posix_time::ptime;
using std::shared_ptr;
using std::cout;
using std::cerr;
using std::endl;
using std::exception;
using std::ios;
using std::setfill;
using std::setw;
using std::string;
using std::ostream;
using std::ostringstream;

/* Enumerations */

/**	Logging levels enumeration.
 *
 */
typedef enum LogLevel {
    LOG_FAT = 0,                    ///< Logging level 0: Fatal errors only.
    LOG_ERR = 1,                    ///< Logging level 1: Errors only.
    LOG_WRN = 2,                    ///< Logging level 2: Warnings, i.e. quiet.
    LOG_LOG = 3,                    ///< Logging level 3: Standard/Default output logging level.
    LOG_INF = 4,                    ///< Logging level 4: Informational messages, i.e. verbose.
    LOG_DBG = 5,                    ///< Logging level 5: Debugging messages.
    LOG_DBG1 = 6,                   ///< Logging level 6: More debugging messages.
    LOG_DBG2 = 7,                   ///< Logging level 7: Even more debugging messages.
    LOG_DBG3 = 8                    ///< Logging level 8: Most debugging messages.
} LogLevel_t;

/* Type Definitions */

/** A thread-safe severity logger object type.
 *
 *  @note           This logger type uses the severity levels defined by
 *                  blg::trivial::severity_level in the boost log library.
 *
 */
typedef blg::sources::severity_logger_mt<blg::trivial::severity_level> sev_logger_mt;

/* Class Definitions */

/** The logger class.
 *
 *  This class is a wrapper for the boost thread-safe severity logger object. It provides
 *  facilities for formatted console and file logging and supports filtering by message severity
 *  and other attributes.
 *
 */
class Log {

    /* Friends */

    friend class config::LocalConfiguration;

private:

    /* Static Data Members */

    /** The global severity logger object.
     *
     *  This object is used as a global logger for all log messages generated by the application.
     *  It includes support for log message attributes that are used for filtering and formatting
     *  of the log messages. It automatically filters the destination (log files, standard output,
     *  etc.) of log messages, and handles daily rotation of log files.
     */
    static sev_logger_mt global_logger_;

    /** The log directory path.
     *
     *  Log files generated as an output of this application will be written to this directory
     *  as well as sub-directories of this directory.
     *
     *  This field is set as a result of parsing the configuration options during initialization.
     *  During initialization, the application checks to ensure that this directory exists, and
     *  throws an error if it does not. Thus, during the subsequent duration of execution of the
     *  application, this directory is assumed to exist on the local filesystem.
     */
    static path log_directory_path_;

    /** The global reporting level.
     *
     *  This field is used to filter the log messages by severity. Only log message with a severity
     *  that is less than or equal to this value will be reported by the global logger.
     *
     *  This field is set a result of parsing the configuration options during initialization of
     *  the application. If the log level is unspecified in the configuration options, then it will
     *  default to a value of @ref DEFAULT_LOG_LEVEL.
     */
    static LogLevel_t log_reporting_level_;

private:

    /* Dynamic Data Members */

    /** The severity level of this log message. */
    LogLevel_t log_msg_severity_level_;

    /** The log message output stream. */
    ostringstream log_msg_stream_;

private:

    /** Set the reporting level of the global logger.
     *
     *  @param[in]  level   The reporting level of the global logger to set.
     */
    static void set_log_reporting_level(LogLevel_t level) {
        log_reporting_level_ = level;
    }

public:

    /* Member Functions */

    /** The default constructor.
     *
     *  @param[in]  level   The message severity level.
     */
    Log(LogLevel_t level) ATTR_NO_EXCEPTIONS :
            log_msg_severity_level_(level) {
      /* Do nothing. */
    }

    /** The destructor.
     *
     *  Flushes the global severity logger's output stream.
     */
    ~Log() {
        Flush(log_msg_severity_level_);
    }

    /** Flush the log's message buffer buffer.
     *
     *  @exception      None. Never throws.
     *
     *  @param[in]      log         The logger.
     *
     *  @return         A const reference to this log's output stream.
     */
    template<typename T = sev_logger_mt> ostream& Flush(
            T& log = global_logger_) ATTR_NO_EXCEPTIONS {
        return Flush(log_msg_severity_level_, log);
    }

    /** Flush the log's message buffer buffer.
     *
     *  @exception      None. Never throws.
     *
     *  @param[in]      lev         The severity level at which to flush the log.
     *  @param[in]      log         The logger.
     *
     *  @return         A const reference to this log's output stream.
     */
    template<typename T = sev_logger_mt> ostream& Flush(
            LogLevel_t lev,
            T& log = global_logger_) ATTR_NO_EXCEPTIONS {

        using blg::trivial::severity_level;

        try {

            string msg(log_msg_stream_.str());
            log_msg_stream_.str("");
            log_msg_stream_.clear();
            trim(msg);

            if(lev <= log_reporting_level_ && !msg.empty()) switch(lev) {
                case LOG_FAT: { // fatal

                    BOOST_LOG_SEV(log, severity_level::fatal) << msg;

                }

                break;

                case LOG_ERR: { // error

                    BOOST_LOG_SEV(log, severity_level::error) << msg;
                }

                break;

                case LOG_WRN: { // warning

                    BOOST_LOG_SEV(log, severity_level::warning) << msg;

                }

                break;

                case LOG_LOG: // info
                case LOG_INF: {

                    BOOST_LOG_SEV(log, severity_level::info) << msg;

                }

                break;

#           if defined(BUILD_DEBUG) && BUILD_DEBUG

                case LOG_DBG: { // debug

                    BOOST_LOG_SEV(log, severity_level::debug) << msg;

                }

                break;

                case LOG_DBG1: // trace
                case LOG_DBG2:
                case LOG_DBG3: {

                    BOOST_LOG_SEV(log, severity_level::trace) << msg;

                }

                break;

#           endif // defined(BUILD_DEBUG) && BUILD_DEBUG

                default:

                /* Do nothing. */

                break;

            }

        } catch(exception& e) {

            cerr << "LOG ERROR: Failed to Flush log object! Caused by:" << exc_diag(e);

        } catch(...) {

            cerr << "LOG ERROR: Failed to Flush log object! Caused unknown.";

        }

        return log_msg_stream_;
    }

    /** Flush the global logger object.
     *
     *  @exception      None. Never throws.
     *
     */
    static void FlushGlobalLogger() ATTR_NO_EXCEPTIONS {
        blg::core::get()->flush();
    }

    /** Initialize the global logger.
     *
     *  Adds common attributes to the global logger and defines the format of the log output.
     *
     *  Adds a log file at logPath/%Y-%m-%d.%3N.log that is rotated at midnight (local time) or every
     *  time each log file exceeds 10MB.
     *
     *  Also adds a console log.
     *
     *  Adds a float-specific log file.
     *
     *  @throw          MessageException if the directory specified by logPath does not exist, or an
     *                  error occurs while initializing the global logger.
     */
    static void InitializeLog() ATTR_NO_EXCEPTIONS {

        try {
            cout << "Initializing logger...";

            try { // Add logger attributes.
                add_common_attributes();
            }CATCH_BLOCK("Failed to add common attributes!")

            // Define the log format.
            auto fmt = expr::stream
            << expr::format_date_time<ptime>("TimeStamp", "%Y-%m-%dT%H:%M:%S.%f ")
            << setfill(' ') << setw(12) << attr_tname << " "
            << setfill(' ') << setw(7) << attr_sevr << " "
            << expr::if_(expr::has_attr(attr_whoi))
            [
            expr::stream  <<  "Float[" <<  setfill('0')  <<  setw(5)  <<  attr_whoi  <<  "|"
                <<  expr::if_(expr::has_attr(attr_dac) && expr::is_in_range(attr_dac, 1000, 10000))
                    [
                     expr::stream << setfill('0') << setw(5) << attr_dac
                    ]
                    .else_
                    [
                     expr::stream << "?????"
                    ]
                <<  "|"
                <<  expr::if_(expr::has_attr(attr_wmo) && expr::is_in_range(attr_wmo, 1000000, 10000000))
                    [
                     expr::stream << setfill('0') << setw(7) << attr_wmo
                    ]
                    .else_
                    [
                    expr::stream  <<  "???????"
                    ]
                <<  "] "
                <<  expr::if_(expr::has_attr(attr_dive) || expr::has_attr(attr_muid))
                    [
                    expr::stream <<  "Dive["
                        <<  expr::if_(expr::has_attr(attr_dive)  && expr::is_in_range(attr_dive, -1, 10000))
                            [
                             expr::stream  <<  setfill(' ')  <<  setw(4)  <<  attr_dive
                            ]
                            .else_
                            [
                             expr::stream << "????"
                            ]
                        <<  "] "
                        <<  expr::if_(expr::has_attr(attr_muid))
                            [
                             expr::stream << "SBD["
                                <<  expr::if_(expr::has_attr(attr_muid) && expr::is_in_range(attr_muid, 0, 1000000))
                                    [
                                     expr::stream << setfill('0') << setw(6) << attr_muid
                                    ]
                                    .else_
                                    [
                                     expr::stream << "??????"
                                    ]
                                << "|"
                                <<  expr::if_(expr::has_attr(attr_momsn) && expr::is_in_range(attr_momsn, 0, 1000000))
                                    [
                                     expr::stream << setfill('0') << setw(6) << attr_momsn
                                    ]
                                    .else_
                                    [
                                     expr::stream << "??????"
                                    ]
                                <<  "|"
                                <<  expr::if_(expr::has_attr(attr_pckt) && expr::is_in_range(attr_pckt, 0, 256))
                                    [
                                     expr::stream  <<  setfill('0') << setw(3) << attr_pckt
                                    ]
                                    .else_
                                    [
                                     expr::stream  <<  "???"
                                    ]
                                <<  "] "
                                <<  expr::if_(expr::has_attr(attr_xmid))
                                    [
                                     expr::stream  <<  "Xmsg["  <<  attr_xmid  <<  "] "
                                    ]
                                    .else_
                                    [
                                     expr::stream  <<  setfill('-')  <<  setw(11)  <<  " "
                                    ]
                            ]
                            .else_
                            [
                             expr::stream << setfill('-') << setw(23) << " " << setw(11) << " "
                            ]
                    ]
                    .else_
                    [
                     expr::stream << setfill('-') << setw(11) << " " << setw(23) << " " << setw(11) << " "
                    ]
            ]
            .else_
            [
             expr::stream << setfill('-') << setw(27) << " " << setw(11) << " " << setw(23) << " " << setw(11) << " "
            ]
            << expr::message;

            try {   // Add a global console log.
                boost::log::add_console_log(cout, keys::format = (fmt));
            }CATCH_BLOCK("Failed to add console log!")

            try {   // Add a global file log.
                // Check the log path.
                if(!is_directory(log_directory_path_))
                EXC_THROW("Invalid path to log directory:" + log_directory_path_.string());
                string log_path = log_directory_path_.string() + "/%Y-%m-%d.log";
                add_file_log(
                        keys::file_name = log_path,//
                        keys::format = (fmt),//
                        keys::open_mode = (ios::out|ios::app),//
                        //                keys::rotation_size = 10 * 1024 * 1024, //
                        keys::time_based_rotation = rotation_at_time_point(0, 0, 0));
            }CATCH_BLOCK("Failed to add file log!")

            try {   // Add a global file log.
                // Check the log path.
                if(!is_directory(log_directory_path_))
                EXC_THROW("Invalid path to log directory:" + log_directory_path_.string());
                string log_path = log_directory_path_.string() + "/%Y-%m-%d.error.log";
                add_file_log(
                        keys::file_name = log_path,//
                        keys::filter = (attr_sevr >= blg::trivial::warning),
                        keys::format = (fmt),//
                        keys::open_mode = (ios::out|ios::app),//
                        //                keys::rotation_size = 10 * 1024 * 1024, //
                        keys::time_based_rotation = rotation_at_time_point(0, 0, 0));
            }CATCH_BLOCK("Failed to add file log!")

#if defined(LOG_INSTRUMENT_SPECIFIC) && LOG_INSTRUMENT_SPECIFIC

            try {   // Add instrument specific log files.
                shared_ptr<core> core = core::get();
                shared_ptr<text_multifile_backend> backend ( make_shared<text_multifile_backend>());

                // Set file naming pattern.
                backend->set_file_name_composer(
                        as_file_name_composer(
                                expr::stream << log_directory_path_.string() << "/"
                                << attr_whoi << "/"
                                << expr::format_date_time<ptime>("TimeStamp", "%Y-%m-%d")
                                << ".log"
                        )
                );

                // Create the backend sink.
                typedef synchronous_sink<text_multifile_backend> mf_sink_t;
                shared_ptr<mf_sink_t> sink(new mf_sink_t(backend));

                // Set the log filter.
                sink->set_filter(expr::has_attr(attr_whoi) && expr::is_in_range(attr_whoi, 1000, 10000));

                // Set the log formatter.
                sink->set_formatter((fmt));

                // Add the sink.
                core->add_sink(sink);

            }CATCH_BLOCK("Failed to add instrument-specific log files!")

#endif // defined(LOG_INSTRUMENT_SPECIFIC) && LOG_INSTRUMENT_SPECIFIC

            cout << " Done." << endl;

        }CATCH_BLOCK("Failed to initialize logger!")
    }

    /** Get the global logger message severity reporting level.
     *
     *  @return         The global logger message severity reporting level.
     */
    static LogLevel_t log_reporting_level() {
        return log_reporting_level_;
    }

    /** The template input stream operator overload function.
     *
     *  @exception      None. Never throws.
     *
     *  @param[in]      x           A const reference to the object invoking the input stream
     *                              operation. This may be any object that supports the input
     *                              stream operation.
     *
     *  @return         A const reference to this log's output stream.
     */
    template<typename T> ostream& operator<<(const T& x) ATTR_NO_EXCEPTIONS {
        log_msg_stream_ << x;
        return log_msg_stream_;
    }

};

}
/* namespace log */
} /* namespace util */
#endif /* LOG_H_ */
