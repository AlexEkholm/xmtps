/** The @ref util::config::LocalConfiguration class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:32:16 PM
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

#ifndef LOCALCONFIGURATION_H_
#define LOCALCONFIGURATION_H_

/* Includes */

// local
#include "../utility/FileSystemUtils.h"

// boost
#include <boost/program_options.hpp>
namespace po = boost::program_options;

// system libraries
#include <cerrno>
#include <cstring>
#include <fcntl.h>

/* Definitions */

#if !defined(BUILD_NUMBER)

#error "BUILD_NUMBER is not defined!"

#endif // !defined(BUILD_NUMBER)

#if !defined(BUILD_DATETIME)

#error "BUILD_DATETIME is not defined!"

#endif // !defined(BUILD_DATETIME)

/** Program name. */
#define PROG_NAME                       "xmtps"

/** Supported X-Message version. */
#define PROG_XVERS                      2.2

/** Program major version. */
#define PROG_MAJOR_VERS                 0

/** Program version. */
#define PROG_MINOR_VERS                 6

#if defined(BUILD_DEBUG) && BUILD_DEBUG

/** Program version. */
#   define PROG_VERS                   PP_STR(PROG_MAJOR_VERS) "." PP_STR(PROG_MINOR_VERS) "." \
                                        PP_STR(BUILD_NUMBER) "a"

#else

/** Program version. */
#	define PROG_VERS                   PP_STR(PROG_MAJOR_VERS) "." PP_STR(PROG_MINOR_VERS) "." \
                                        PP_STR(BUILD_NUMBER) "b"

#endif // defined(BUILD_DEBUG) && BUILD_DEBUG

/** Brief program description. */
#define PROG_DESC                       "X-Message Mail-Telemetry Processing Service"

/** Program identifier string. */
#define PROG_ID                         "=== " PROG_NAME " v" PROG_VERS " - " PROG_DESC " ==="

/** The default value for daemon/service mode operation, if unspecified in the configuration
 *  settings.
 */
#define CFG_DEFAULT_ISDAEMON            (false)

/** The default configuration file path to use, if not specified in the command line arguments. */
#define CFG_DEFAULT_PATH                "../cfg/options.cfg"

/** The default path to the PID file. */
#define CFG_DEFAULT_PATH_PID            "/var/lock/" PROG_NAME "/lock.pid"

/** The default configuration file path to use, if not specified in the command line arguments. */
#define CFG_DEFAULT_MAIL_CHK_DLY        (60)

/** The default number of mail messages to fetch concurrently, if unspecified in the configuration
 *  settings.
 */
#define CFG_DEFAULT_MAX_FETCH           (127)

/** The placeholder value for an unspecified starting mail message UID */
#define CFG_DEFAULT_MIN_UID             (0)

/** The default number of mail message processing threads to instantiate, if unspecified in the
 *  configuration settings.
 */
#define CFG_DEFAULT_NPROC               (1)

/** The default number of telemetry decoder threads to instantiate, if unspecified in the
 *  configuration settings.
 */
#define CFG_DEFAULT_NDECO               (1)

/** The default value for the reprocess-all boolean switch, if unspecified in the configuration
 *  settings.
 */
#define CFG_DEFAULT_REPROCESS_ALL       (false)

#define CFG_DEFAULT_REPROCESS_ACTIVE    (true)

/** The configuration option line length for printing to standard output. */
#define CFG_FMT_LINE_LEN                (100)

/** The configuration option description minimum line length. */
#define CFG_FMT_DESC_LEN                (85)

namespace util {
namespace config {

using boost::filesystem::exists;
using boost::filesystem::ifstream;
using boost::filesystem::ofstream;
using boost::filesystem::path;
using boost::filesystem::remove;
using boost::program_options::bool_switch;
using boost::program_options::notify;
using boost::program_options::options_description;
using boost::program_options::store;
using boost::program_options::variables_map;
using boost::program_options::value;
using std::cout;
using std::endl;
using std::ostringstream;
using std::string;
using std::vector;

/* Class Definitions */

/** Class representing all program run-time configuration options.
 *
 *  This class defines all run-time configuration options, descriptions, and default values. The
 *  boost::program_options library is used parse run-time configuration options from a
 *  configuration file and/or Command-Line Interface (CLI) arguments.
 *
 */
class LocalConfiguration:
        public file::FileSystemUtils {

private:

    /* Data Members */

    /** Statically-allocated global run-time configuration options object.
     *
     *  All program run-time configuration settings are stored here.
     *
     */
    static LocalConfiguration configuration_;

    /** Configuration file path string. */
    static path configuration_file_path_;

    /** Indicates whether the program is to run as a daemon.
     *
     *
     */
    static bool is_daemon_;

    /** The path to the PID file on the local filesystem.
     *
     *  This PID file is used to ensure that only a single instance of the program is running
     *  at any time. The file contains a single ASCII integer corresponding to the PID of the
     *  running instance. It is created during program initialization, and is removed upon
     *  termination. The file is checked periodically during execution to ensure that it still
     *  exists and that the PID is correct.
     */
    static path pid_file_path_;

    /** The splash string.
     *
     *  This c-string is printed to standard output upon program startup,
     *
     */
    static const char* SPLASH;

    /** The listing splash string.
     *
     *  The file header for created ASCII files such as RAW mail messages, and intermediate listing
     *  files.
     */
    static const char* SPLASH_LIS;

private:

    /* Member Functions */

    /** Parse the run-time configuration options from the command-line arguments and/or the
     *  configuration file.
     *
     *  Populates the configuration options object.
     *
     *  @param[in]      argc        The number of command-line arguments.
     *  @param[in]      argv        The command-line argument list.
     *
     *  @throw          log::MessageException if an error occurs while parsing the command-line
     *                  arguments or the configuration file.
     */
    void ParseProgramOptions(int argc, char** argv) ATTR_NO_EXCEPTIONS {
        using util::log::Log;

        try {
            cout << "Loading configuration..." << endl;

            // Generate available options.
            options_description cliOptDesc = GenerateCommandLineOptionsDescription();
            options_description cfgOptDesc = GenerateConfigurationFileOptionsDescription();
            options_description allOptDesc("Available Options");
            allOptDesc.add(cliOptDesc);
            allOptDesc.add(cfgOptDesc);

            // Parse command-line options.
            variables_map vm;
            store(parse_command_line(argc, argv, allOptDesc), vm);
            notify(vm);

            /* Process configuration options. */

            // Help text.
            if(vm.count("help")) {
                cout << allOptDesc << endl;
                exit(EXIT_SUCCESS);
            }

            // Version string.
            if(vm.count("version")) {
                exit(EXIT_SUCCESS);
            }

            // Set logging level.
            if(vm.count("debug"))
                Log::set_log_reporting_level(util::log::LOG_DBG3);
            else if(vm.count("verbose"))
                Log::set_log_reporting_level(util::log::LOG_INF);
            else if(vm.count("quiet"))
                Log::set_log_reporting_level(util::log::LOG_WRN);
            else
                Log::set_log_reporting_level(DEFAULT_LOG_LEVEL);

            // Open configuration file.
            if(!exists(configuration_file_path_))
                EXC_THROW("Configuration file not found!");
            ifstream ifs_cfg(configuration_file_path_);
            if(!ifs_cfg)
                EXC_THROW(
                        "Failed to read configuration file: " + configuration_file_path_.string());

            // Parse configuration file options.
            store(parse_config_file(ifs_cfg, cfgOptDesc, false), vm);
            notify(vm);
            ifs_cfg.close();

            cout << "Done." << endl;
        }CATCH_BLOCK("Failed to configure program options!")
    }

public:

    /* Member Functions */

    /** Configure the application run-time settings and acquire a run-time lock.
     *
     *  Parse the configuration options, then acquire a run-time lock. Verify the run-time lock before
     *  returning.
     *
     *  @param[in]      argc        The number of command-line arguments.
     *  @param[in]      argv        The command-line argument list.
     *
     *  @throw          log::MessageException if an error occurs while parsing the run-time
     *                  configuration options or acquiring the run-time lock.
     *
     *  @return         A const reference to the configuration_ object.
     */
    static const LocalConfiguration& InitializeConfiguration(int argc, char** argv) ATTR_NO_EXCEPTIONS {
        using util::log::Log;

        try {
            // First, print the program version string splash text.
            cout << endl << LocalConfiguration::SPLASH << endl;

            // Configure run-time options.
            configuration_.ParseProgramOptions(argc, argv);

            // Acquire run-time lock.
            configuration_.AcquireRuntimeLock();

        }CATCH_BLOCK("Failed to configure run-time options!")

        return configuration_;
    }


public:

    /* Member Functions */

    /** The default constructor.
     *
     */
    LocalConfiguration() {
        /* Do nothing. */
    }

    /** The destructor.
     *
     */
    ~LocalConfiguration() {
        /* Do nothing. */
    }

    /** Acquire a run-time lock.
     *
     *  A PID file is created upon successful acquisition of a run-time lock. This PID file is used
     *  to ensure that only a single instance of the program is running at any time. The file contains
     *  a single ASCII integer corresponding to the PID of the running instance. It is created during
     *  program initialization, and is removed upon termination. The file is checked periodically
     *  during execution to ensure that it still exists and that the PID is correct.
     *
     *  @throw          util::log::MessageException if acquisition of the run-time lock fails e.g.
     *                  another instance of the program has already acquired a run-time lock.
     *
     *  @note           To release the run-time lock, call the releaseRuntimeLock() function.
     *
     *  @return         The PID of the process acquiring the run-time lock.
     */
    static __pid_t AcquireRuntimeLock() ATTR_NO_EXCEPTIONS {
        __pid_t pid = 0;

        try {
            cout << "Acquiring run-time lock...";

            // Create lock file directory, if necessary.
            GetDirectoryPathFromString(pid_file_path_.parent_path().string(), true);

            /* Create the lock file exclusively, so two instances starting at the same time cannot
             * both acquire the lock. */
            int fd = ::open(pid_file_path_.c_str(), O_WRONLY | O_CREAT | O_EXCL, 0644);
            if(fd < 0) {

                if(errno != EEXIST)
                    EXC_THROW("Failed to create PID file: " + pid_file_path_.string() + ": "
                              + strerror(errno));

                // Get PID of previous lock.
                ifstream ifs(pid_file_path_);
                ifs >> pid;
                ifs.close();

                // Compose and throw exception.
                ostringstream oss;
                oss << "PID file already exists: " << pid_file_path_ << endl;
                oss << "\tPID: " << pid;
                EXC_THROW(oss.str());
            }

            // Write PID to lock file.
            ostringstream oss_pid;
            oss_pid << (pid = getpid());
            const string pid_str = oss_pid.str();
            ssize_t num_written = ::write(fd, pid_str.data(), pid_str.size());
            ::close(fd);
            if(num_written != (ssize_t) pid_str.size()) {
                ::unlink(pid_file_path_.c_str());
                EXC_THROW("Failed to write PID file: " + pid_file_path_.string() + "!");
            }

            // Check the lock.
            CheckRuntimeLock();

            cout << " Done. Run-time lock successfully acquired. PID is " << pid << "." << endl;

        }CATCH_BLOCK("FATAL: Failed to acquire run-time lock!");
        return pid;
    }

    /** Check the run-time lock.
     *
     *  This method checks the run-time lock file to ensure that this instance of the program retains
     *  the run-time lock acquired using the acquireRuntimeLock() function. This means that the lock
     *  file exists, is accessible, and contains the PID of this instance of the program.
     *
     *
     *  @throw          log::MessageException if the run-time lock check fails e.g. the run-time lock
     *                  file does not exist, is not accessible, or contains the incorrect PID.
     */
    static void CheckRuntimeLock() {

        __pid_t pid = 0;
        try {
            // Check PID file.
            if(exists(pid_file_path_)) {
                // Check if lock file PID matches current PID.
                ifstream ifs(pid_file_path_);
                ifs >> pid;
                if(pid != getpid()) {
                    // Compose and throw exception.
                    ostringstream oss;
                    oss << "PID mismatch (expected " << getpid() << ", got " << pid << ")!";
                    EXC_THROW(oss.str());
                }
            } else {
                // Compose and throw exception.
                EXC_THROW("PID file does not exist: " + pid_file_path_.string() + "!");
            }

        }CATCH_BLOCK("Run-time lock check failed!");
    }

    static options_description GenerateConfigurationFileOptionsDescription() ATTR_NO_EXCEPTIONS;

    static options_description GenerateCommandLineOptionsDescription() ATTR_NO_EXCEPTIONS;

    /** Release the run-time lock.
     *
     *  This method releases the run-time lock acquired by the AcquireRuntimeLock() function. It
     *  deletes the lock file from the local filesystem
     *
     */
    static void ReleaseRuntimeLock() {
        try {
            lout() << "Releasing run-time lock...";

            // Delete run-time lock file, but only if it belongs to this process.
            if(exists(pid_file_path_)) {
                __pid_t pid = 0;
                ifstream ifs(pid_file_path_);
                ifs >> pid;
                ifs.close();
                if(pid != getpid()) {
                    lwrn() << "Not removing PID file owned by another process (PID " << pid
                           << "): " << pid_file_path_;
                    return;
                }

                ldbg() << "Removing PID file: " << pid_file_path_ << " ...";
                remove(pid_file_path_);
                if(exists(pid_file_path_))
                    EXC_THROW("Failed to remove PID file: " + pid_file_path_.string() + "!");
            }

            linf() << "Done. Run-time lock successfully removed.";
        }CATCH_BLOCK("Failed to release run-time lock!")
    }

    /** Get the local configuration run-time settings object.
     *
     *  @return         The local configuration run-time settings object.
     */
    static const LocalConfiguration& configuration() {
        return configuration_;
    }

    /** Is the application configured to run as a daemon process?
     *
     *  @return         TRUE if the application is configured to run as a daemon process, or FALSE
     *                  otherwise.
     */
    static bool is_daemon() {
        return is_daemon_;
    }

    /** Get the splash listing text string.
     *
     *  @return         The splash listing text string.
     */
    static const char* splash_lis() {
        return SPLASH_LIS;
    }

};

} /* namespace config */
} /* namespace util */

#endif /* LOCALCONFIGURATION_H_ */
