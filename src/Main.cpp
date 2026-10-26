/** The @ref main() function source file.
 *
 *  This file contains the main program entry point, @ref main(), which defines the functional
 *  operation of the daemon thread. This thread is responsible for for performing the application
 *  initialization, instantiating and terminating the worker threads, maintaining the run-time
 *  lock, and finally invoking the exit handler.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:42:20 PM
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

/* Includes: */

#include "Main.h"     // header

/* Function Definitions: */

/** The main program (daemon thread) entry point.
 *
 *  The operation of the daemon thread of the application is completely contained within this
 *  function. First, the program is initialized by calling @ref Initialize() which parses the
 *  configuration options, acquires the run-time lock, sets up the global logger object, registers
 *  the signal handlers, and finally loads requisite data from both files on the local filesystem
 *  and the database. Then, the runtime lock will be checked and the thread will sleep for
 *  @ref XFLT_THREAD_SLEEP_MS milliseconds. If the application is configured to run as a daemon,
 *  then the last step will be repeated indefinitely until a termination signal is received.
 *  Finally, the exit handler, @ref util::sig::SignalHandler::ExitHandler(), is invoked to
 *  terminate the worker threads, save the locally cached data to the database, release the
 *  run-time lock, and flush the global logger.
 *
 *  @note       If, at any time during execution after initialization, a catchable signal is
 *              received, the execution of the daemon thread will be interrupted and the exit
 *              handler function @ref util::sig::SignalHandler::ExitHandler() will be invoked. The
 *              exit handler terminates the worker threads, saves the locally cached data to the
 *              database, releases the run-time lock, and flushes the global logger.
 *
 *  @param[in]  argc    The number of command-line arguments.
 *  @param[in]  argv    The command-line argument list.
 *
 *  @exception  util::log::MessageException if application initialization fails.
 *
 *  @return     EXIT_SUCCESS on successful completion or non-zero value on failure.
 */
int main(int argc, char** argv) {

  using boost::posix_time::millisec;
  using boost::posix_time::seconds;
  using boost::this_thread::sleep;
  using util::config::LocalConfiguration;
  using util::log::Log;
  using util::sig::SignalHandler;

  unsigned int ms_since_log_last_flushed = 0;

  LOG_SET_SCOPED_THREAD_NAME("daemon");

  try {

    /* Application Initialization: */

    LocalConfiguration cfg(Initialize(argc, argv));   // Initialize application.
    StartWorkerThreads();                             // Start worker threads.
    for(int delay_msec = XFLT_DAEMON_THREAD_SPINUP_DELAY_SEC * 1000;
        delay_msec >= 0;
        delay_msec -= XFLT_THREAD_SLEEP_MS) {

      cfg.CheckRuntimeLock();                         // Check runtime lock.

      sleep(millisec(XFLT_THREAD_SLEEP_MS));

      if((ms_since_log_last_flushed += XFLT_THREAD_SLEEP_MS) > XFLT_DAEMON_FLUSH_LOG_SEC * 1000) {
        Log::FlushGlobalLogger();                     // Flush global logger.
        ms_since_log_last_flushed = 0;
      }

    };

    /* Main Program Loop:
     *  - Checks run-time lock.
     *  - Flushes global log periodically.
     *  - If daemon, loops until termination signal is encountered.
     */

    do {

      cfg.CheckRuntimeLock();                         // Check runtime lock.

      sleep(millisec(XFLT_THREAD_SLEEP_MS));

      if((ms_since_log_last_flushed += XFLT_THREAD_SLEEP_MS) > XFLT_DAEMON_FLUSH_LOG_SEC * 1000) {
        Log::FlushGlobalLogger();                     // Flush global logger.
        ms_since_log_last_flushed = 0;
      }

    } while(cfg.is_daemon());                         // If daemon, loop until termination.

  }CATCH_NOTHROW("Daemon thread encountered fatal exception!")

  SignalHandler::ExitHandler(SIGTERM);                // Call the exit handler.

  /* Dead Code: */

  exit(EXIT_SUCCESS);
}

/** Initialize the application.
 *
 *  All startup initialization of the application is performed by this function. First, the
 *  local configuration initialization is invoked, which parses the configuration file and/or the
 *  command-line arguments and also acquires a run-time lock. Then the global logger is
 *  initialized, setting the sources, sinks, formats, and filters of the global logger. Next,
 *  the signal handlers are registered, defining the behavior of the application if a catchable
 *  signal is encountered from the system. Finally, requisite data and metadata is loaded from
 *  files on the local file system and from the SQL database.
 *
 *  @param[in]  argc    The number of command-line arguments.
 *  @param[in]  argv    The command-line argument list.
 *
 *  @exception  None. Never throws.
 *
 *  @return     The local configuration object.
 */
const util::config::LocalConfiguration& Initialize(int argc, char** argv) ATTR_NO_EXCEPTIONS {

  using argo::iridium::xfloat::XFloat;
  using util::config::LocalConfiguration;
  using util::log::Log;
  using util::sig::SignalHandler;

  try {

    /* Initialize Local Configuration:
     *  - Parse configuration options.
     *  - Acquire run-time lock. */

    try {
      LocalConfiguration::InitializeConfiguration(argc, argv);
    }CATCH_BLOCK("Failed to configure application run-time options!")

    /* Initialize Global Logger: */

    try {
      Log::InitializeLog();
    }CATCH_BLOCK("Failed to initialize the global logger!")

    /* Register Signal Handlers: */

    try {
      SignalHandler::RegisterSignalHandlers();
    }CATCH_BLOCK("Failed to register signal handlers!")

    /* Load Cached Instrument Data: */

    try {
      XFloat::LoadLocalCacheFromDatabase();
    }CATCH_BLOCK("Failed to load cached instrument data!")

    /* Load Metadata: */

    try {
      XFloat::LoadMetadataFromFiles();
    }CATCH_BLOCK("Failed to load metadata!")

    /* Do Startup Tasks: */

    try {
      XFloat::StartupTasks();
    }CATCH_BLOCK("Failed to run XFloat startup tasks!")

  }CATCH_BLOCK("Failed to initialize the application!")

  return LocalConfiguration::configuration();
}

/** Start worker threads.
 *
 *  A call to this function will instantiate all worker threads that comprise the pipeline of the
 *  application: (1) the mail retrieval thread, (2) the mail processing thread(s), and (3) the
 *  telemetry decoder threads.
 *
 *  @exception  None. Never throws.
 */
void StartWorkerThreads() ATTR_NO_EXCEPTIONS {

  using argo::iridium::xfloat::XFloatMailRetrievalService;
  using argo::iridium::xfloat::XFloatMailProcessingService;
  using argo::iridium::xfloat::XFloatTelemetryDecoderService;
  using argo::iridium::xfloat::XFloatPostProcessingService;

  try {

    lout() << "Starting worker threads...";

    XFloatMailRetrievalService::StartWorkerThread();
    XFloatMailProcessingService::StartWorkerThreads();
    XFloatTelemetryDecoderService::StartWorkerThreads();
    XFloatPostProcessingService::StartWorkerThread();

    linf() << "Done. All worker threads successfully started.";

  } CATCH_BLOCK("Failed to start worker threads!")
}

/** Terminate the worker threads.
 *
 *  A call to this function will perform and orderly shutdown of the the worker threads. The
 *  threads will be shutdown in order of the pipeline of the application. Each thread or thread
 *  group will be allowed to complete its current task(s) before the shutdown occurs, and before
 *  shutting down the next stage of the threads in the pipeline. This way, all the data that is
 *  in the pipeline when the shutdown is invoked will be processes before the application
 *  terminates.
 *
 *  @warning    This function will block until all threads are shut down. If any of the worker
 *              threads hang, then this function may block indefinitely.
 *
 *  @exception  None. Never throws.
 */
void TerminateWorkerThreads() ATTR_NO_EXCEPTIONS {

  using argo::iridium::xfloat::XFloatMailRetrievalService;
  using argo::iridium::xfloat::XFloatMailProcessingService;
  using argo::iridium::xfloat::XFloatTelemetryDecoderService;
  using argo::iridium::xfloat::XFloatPostProcessingService;

  try {

    lout() << "Terminating worker threads...";

    XFloatMailRetrievalService::TerminateWorkerThread();
    XFloatMailProcessingService::TerminateWorkerThreads();
    XFloatTelemetryDecoderService::TerminateWorkerThreads();
    XFloatPostProcessingService::TerminateWorkerThread();

    linf() << "Done. All worker threads successfully terminated.";

  }CATCH_BLOCK("Failed to terminate worker threads!")
}

namespace util {
namespace config {

/** Get the command-line interface configuration options' description.
 *
 *  This function fully describes the configuration options that may be passed to the application
 *  via the command-line. It includes the short- and long-hand versions of the options, the value
 *  types and destinations, the default values, and a brief description of each option.
 *
 *  @exception  None. Never throws.
 *
 *  @return     Command-line interface configuration options description.
 */
options_description LocalConfiguration::GenerateCommandLineOptionsDescription() ATTR_NO_EXCEPTIONS {

  using argo::iridium::xfloat::XFloat;
  using argo::iridium::xfloat::XFloatMailRetrievalService;

  options_description desc_cli("General Options", CFG_FMT_LINE_LEN, CFG_FMT_DESC_LEN);

  try {

    desc_cli.add_options()

        ("config,c", //
            value<path>(&configuration_file_path_) //
              ->default_value(CFG_DEFAULT_PATH) //
              ->value_name("PATH"), //
            "Set the local filesystem path to the configuration file.\n")

        ("phy-enable",
            value<vector<unsigned int> >(&XFloat::instrument_ids_to_enable_phy_generation_)
              ->value_name("ID"),
            "Enable PHY file generation for specified instrument.\n")

        ("phy-disable",
            value<vector<unsigned int> >(&XFloat::instrument_ids_to_disable_phy_generation_)
              ->value_name("ID"),
            "Disable PHY file generation for specified instrument.\n")

        ("debug,d",
            "Enable debug output.\n")

        ("daemon,D",
            bool_switch(&is_daemon_)
              ->default_value(CFG_DEFAULT_ISDAEMON),
            "Run as daemon process/service. If this option is set, then the program will run "
            "indefinitely until termination is signaled by the user, or until the run-time lock "
            "is revoked. Otherwise, the program will run once and then exit.\n")

        ("help,h",
            "Print program usage message and options descriptions and then exit.\n")

        ("quiet,q",
            "Print less verbose output. Only warning and error messages will be logged.\n")

        ("reprocess-active,A",
            value<bool>(&XFloatMailRetrievalService::is_reprocess_active_) //
              ->default_value(CFG_DEFAULT_REPROCESS_ACTIVE) //
              ->value_name("0|1"),
            "Reprocess active floats at startup.\n")

        ("reprocess-all,R",
            bool_switch(&XFloatMailRetrievalService::is_reprocess_all_)
              ->default_value(
            CFG_DEFAULT_REPROCESS_ALL),
            "Reprocess all mail messages. All stored mail messages will be retrieved from the "
            "database and reprocessed.\n")

        ("reprocess,r",
            value<vector<unsigned int> >(&XFloatMailRetrievalService::reprocess_by_inst_ids_)
              ->multitoken()->value_name("UID"),
            "Reprocess all mail messages for the given unique instrument identifier. This option "
            "may be invoked multiple time reprocess the mail message for multiple instruments. If "
            "invoked, all stored mail messages matching the specified instrument identifier(s) "
            "will be retrieved from the database and reprocessed before proceeding with normal "
            "operation.\n")

        ("verbose,v",
            "Print more verbose output. Additional informational messages will be logged.\n")

        ("version,V",
            "Print the program version string and exit.\n");

  }CATCH_BLOCK("Failed to get configuration options description!")

  return desc_cli;
}

/** Get configuration file options' description.
 *
 *  This function fully describes the configuration options that may be passed to the application
 *  via the configuration file. It includes the short- and long-hand versions of the options, the
 *  value types and destinations, the default values, and a brief description of each option.
 *
 *  @exception  None. Never throws.
 *
 *  @return     Configuration file options description.
 */
options_description LocalConfiguration::GenerateConfigurationFileOptionsDescription()
    ATTR_NO_EXCEPTIONS {

  using argo::iridium::xfloat::XFloat;
  using argo::iridium::xfloat::XFloatDiveData;
  using argo::iridium::xfloat::XFloatMailFetchMode_t;
  using argo::iridium::xfloat::XFloatMailMessage;
  using argo::iridium::xfloat::XFloatMailProcessingService;
  using argo::iridium::xfloat::XFloatMailRawFileFormat_t;
  using argo::iridium::xfloat::XFloatMailRetrievalService;
  using argo::iridium::xfloat::XFloatMetadata;
  using argo::iridium::xfloat::XFloatPostProcessingService;
  using argo::iridium::xfloat::XFloatTelemetryDecoderService;
  using util::log::Log;
  using util::mysql::LocalMysqlConfiguration;

  options_description desc_cfg(
      "Configuration File Override Options (use these options to override configuration file "
      "settings)",
      CFG_FMT_LINE_LEN,
      CFG_FMT_DESC_LEN);
  options_description desc_dir(
      "Directory Configuration Options",
      CFG_FMT_LINE_LEN,
      CFG_FMT_DESC_LEN);
  options_description desc_file(
      "File Generation Configuration Options",
      CFG_FMT_LINE_LEN,
      CFG_FMT_DESC_LEN);
  options_description desc_mail(
      "Mail Server Configuration Options",
      CFG_FMT_LINE_LEN,
      CFG_FMT_DESC_LEN);
  options_description desc_sql(
      "SQL Server Configurations Options",
      CFG_FMT_LINE_LEN,
      CFG_FMT_DESC_LEN);

  try {

    /* Define run-time options: */

    desc_cfg.add_options()

        ("maxFetch,m",
            value<size_t>(&XFloatMailRetrievalService::max_num_msgs_to_fetch_) //
              ->default_value(CFG_DEFAULT_MAX_FETCH) //
              ->value_name("N"),
            "Set the maximum number of mail message to simultaneously fetch from the mail "
            "server or file system.\n")

        ("msgStartIdx,M",
            value<unsigned int>(&XFloatMailRetrievalService::message_start_index_) //
              ->default_value(1) //
              ->value_name("N"),
            "Set the starting index of the messages to fetch from the mail server. This option is "
            "only used if --mail.fetchMode=0.\n")

        ("nProcThrd,n",
            value<unsigned int>(&XFloatMailProcessingService::num_threads_) //
              ->default_value(CFG_DEFAULT_NPROC) //
              ->value_name("N"),
            "Set the number of concurrent mail message processing threads to instantiate.\n")

        ("nDecoThrd,N",
            value<unsigned int>(&XFloatTelemetryDecoderService::num_threads_) //
              ->default_value(CFG_DEFAULT_NDECO) //
              ->value_name("N"),
            "Set the number of concurrent telemetry decoder threads to instantiate.\n")

        ("pid-file,p",
            value<path>(&pid_file_path_) //
              ->default_value(CFG_DEFAULT_PATH_PID) //
              ->value_name("PATH"),
            "Set the path to the PID file on the local filesystem. This PID file is used to ensure "
            "that only a single instance of the program is running at any time. The file contains "
            "a single ASCII integer corresponding to the PID of the running instance. It is "
            "created during program initialization, and is removed upon termination. The file is "
            "checked periodically during execution to ensure that it still exists and that the PID "
            "is correct.\n");

    /* Define directory options: */

    desc_dir.add_options()

        ("dir.kml",
            value<path>(&XFloat::kml_directory_path_) //
              ->default_value("kml") //
              ->value_name("PATH"),
            "Set the path to the KML directory on the local filesystem. If --gen.kml=1, then KML "
            "files will be written to this directory.\n")

        ("dir.lis",
            value<path>(&XFloatDiveData::intermediate_listing_directory_path_) //
              ->default_value("lis") //
              ->value_name("PATH"),
            "Set the path to the intermediate listings directory on the local filesystem. "
            "Intermediate listings will be generated and written to subdirectories of this path.\n")

        ("dir.log",
            value<path>(&Log::log_directory_path_) //
              ->default_value("log") //
              ->value_name("PATH"),
            "Set the path to the log directory on the local filesystem.\n")

//        ("dir.nc",
//            value<path>(&XFloatDiveData::netcdf_directory_path_) //
//              ->value_name("PATH"),
//            "Set the path to the NetCDF file directory on the local filesystem. NetCDF files will "
//            "be generated and written to subdirectories of this path.\n")

        ("dir.meta",
            value<path>(&XFloatMetadata::metadata_directory_path_) //
              ->default_value("meta") //
              ->value_name("PATH"),
            "Set the path to the ALAMO metadata directory.\n")

        ("dir.raw",
            value<path>(&XFloatMailMessage::raw_directory_path_) //
              ->default_value("raw") //
              ->value_name("PATH"),
            "Set the path to the RAW mail message output directory.\n")

        ("dir.mail",
            value<path>(&XFloatMailRetrievalService::mail_file_input_directory_) //
              ->default_value("raw/incoming") //
              ->value_name("PATH"),
            "Set the path to the RAW mail message input directory.\n")

        ("dir.mail.processed",
            value<path>(&XFloatMailRetrievalService::mail_file_processed_directory_) //
              ->default_value("raw/processed") //
              ->value_name("PATH"),
            "Set the path to the processed RAW mail message output directory.\n")

        ("dir.sbd",
            value<path>(&XFloatMailMessage::sbd_directory_path_) //
              ->default_value("sbd") //
              ->value_name("PATH"),
            "Set the path to the SBD message directory.\n")

        ("dir.stat",
            value<path>(&XFloat::status_directory_path_) //
             ->default_value("stat") //
              ->value_name("PATH"),
            "Set the path to the status file directory.\n")

        ("dir.phy",
            value<path>(&XFloatDiveData::phy_directory_path_) //
              ->default_value("phy") //
              ->value_name("PATH"),
            "Set the ALAMO PHY listing directory path. PHY listing files will be generated and "
            "written to subdirectories of this path.\n");

    /* Define file generation options: */

    desc_file.add_options()

        ("gen.kml",
            value<bool>(&XFloat::kml_file_generation_is_enabled_) //
              ->default_value(true) //
              ->value_name("0|1"),
            "Generate ALAMO Google Earth KML files. If TRUE, Google Earth KML files will be "
            "generated and written to the KML directory on the local filesystem. Otherwise, KML "
            "files will not be generated or written to the local filesystem.\n")

        ("gen.lis",
            value<bool>(&XFloatDiveData::intermediate_listing_file_generation_is_enabled_) //
              ->default_value(true) //
              ->value_name("0|1"),
            "Generate ALAMO intermediate listing files. If TRUE, intermediate listing files will "
            "be generated and written to the intermediate listing directory on the local "
            "filesystem. Otherwise, intermediate listing files will not be generated or written to "
            "the local filesystem.\n")

//        ("gen.nc",
//            value<bool>(&XFloatDiveData::netcdf_file_generation_is_enabled_) //
//              ->default_value(true) //
//              ->value_name("0|1"),
//            "Generate ALAMO NetCDF files. If TRUE, NetCDF files will be generated and written to "
//            "the NetCDF directory on the local filesystem. Otherwise, NetCDF files will not be "
//            "generated or written to the local filesystem.\n")

        ("gen.phy",
            value<bool>(&XFloatDiveData::phy_file_generation_is_enabled_) //
              ->default_value(true) //
              ->value_name("0|1"),
            "Generate ALAMO PHY listing files. If TRUE, PHY listing files will be generated and "
            "written to the PHY listing files directory on the local filesystem. Otherwise, PHY "
            "listing files will not be generated or written to the local filesystem.\n")

        ("gen.raw",
            value<bool>(&XFloatMailMessage::raw_file_generation_is_enabled_) //
              ->default_value(true) //
              ->value_name("0|1"),
            "Generate ALAMO RAW mail message files. If TRUE, RAW mail message files will be "
            "generated  and written to the mail message directory on the local filesystem. "
            "Otherwise, RAW mail message files will not be generated or written to the local "
            "filesystem.\n")

        ("gen.raw.format",
            value<int>(&XFloatMailMessage::raw_file_format_) //
              ->default_value(XFloatMailRawFileFormat_t::XMAIL_DFLT_RAW_FILE_FORMAT) //
              ->value_name("0|1"),
            "Set the RAW file format. 0: standard, 1: extended.\n")

        ("gen.sbd",
            value<bool>(&XFloatMailMessage::sbd_file_generation_is_enabled_) //
              ->default_value(false) //
              ->value_name("0|1"),
            "Generate ALAMO binary SBD message files. If TRUE, binary SBD message files will be "
            "generated and written to the mail message directory on the local filesystem. "
            "Otherwise, binary SBD message files will not be generated or written to the local "
            "filesystem.\n")

        ("gen.stat", value<bool>(&XFloat::status_file_generation_is_enabled_) //
              ->default_value(false) //
              ->value_name("0|1"),
            "Generate ALAMO status files. If TRUE, status files will be generated and written to "
            "the mail message directory on the local filesystem. Otherwise, status files will not "
            "be generated or written to the local filesystem.\n")

        ("poc.lis",
            value<bool>(&XFloatDiveData::pressure_offset_correction_enabled_list_file_) //
              ->default_value(true) //
              ->value_name("0|1"),
            "Enable surface pressure offset correction for sensor data in generated intermediate "
            "listing files. Pressure offset corrections are only applied to floats with RBR sensor "
            "packages.\n")

//        ("poc.nc",
//            value<bool>(&XFloatDiveData::pressure_offset_correction_enabled_netcdf_file_) //
//              ->default_value(true) //
//              ->value_name("0|1"),
//            "Enable pressure offset correction for sensor data in generated netcdf files. Pressure "
//            "offset corrections are only applied to floats with RBR sensor packages.\n")

        ("poc.phy",
            value<bool>(&XFloatDiveData::pressure_offset_correction_enabled_phy_file_) //
              ->default_value(true) //
              ->value_name("0|1"),
            "Enable pressure offset correction for sensor data in generated phy files. Pressure "
            "offset corrections are only applied to floats with RBR sensor packages.\n");

    /* Mail server configuration options: */

    desc_mail.add_options()

        ("mail.host",
            value<string>(&XFloatMailRetrievalService::mail_server_host_) //
              ->default_value("maila.whoi.edu"),
            "Set the mail server host qualified name or IP address.\n")

        ("mail.fetchMode",
            value<int>(&XFloatMailRetrievalService::mail_fetch_mode_) //
              ->default_value((int)XFloatMailFetchMode_t::MAIL_FETCH_SERVER) //
              ->value_name("0|1"),
            "Set the mail fetch mode. 0: fetch mail message from server, 1: fetch mail messages "
            "from file system.\n")


        ("mail.folder",
            value<string>(&XFloatMailRetrievalService::mail_server_inbox_folder_path_) //
              ->default_value(""),
            "Set the mail server remote incoming message folder path.\n")

        ("mail.moveMessages",
            value<bool>(&XFloatMailRetrievalService::move_mail_messages_) //
              ->default_value(false),
            "Set whether the mail messages are moved from the inbox folder after being processed. "
            "If true, mail message will be moved to the the folder INBOX.<YYYYMM> where <YYYYMM> "
            "is the year and month of the message header date.\n")

        ("mail.password",
            value<string>(&XFloatMailRetrievalService::mail_server_password_),
            "Set the mail server account password.\n")

        ("mail.protocol",
            value<string>(&XFloatMailRetrievalService::mail_server_protocol_) //
              ->default_value("imap"),
            "Set the mail server protocol.\n")

        ("mail.timeout",
            value<unsigned int>(&XFloatMailRetrievalService::mail_server_timeout_s_) //
              ->default_value(120) //
              ->value_name("SEC"),
            "Set the time, in seconds, after which a mail server operation that receives no data "
            "is cancelled; the connection is then closed and reopened on the next check.\n")

        ("mail.user",
            value<string>(&XFloatMailRetrievalService::mail_server_user_) //
              ->default_value("alamo"),
            "Set the mail server account username.\n")

        ("mail.validSbdFilenameRegex",
            value<string>(&XFloatMailMessage::sbd_file_valid_name_regex_) //
              ->default_value(XMAIL_DFLT_SBD_FILE_VALID_NAME_REGEX),
            "Set the regular expression used to validate the SBD attachment filename.\n")

        ("mail.validSenderRegex",
            value<string>(&XFloatMailMessage::msg_header_valid_sender_regex_) //
              ->default_value(XMAIL_DFLT_MSG_HEADER_VALID_SNDR_REGEX),
            "Set the regular expression string to match valid mail message sender(s).\n")

        ("mail.validSubjectRegex",
            value<string>(&XFloatMailMessage::msg_header_valid_subject_regex_) //
              ->default_value(XMAIL_DFLT_MSG_HEADER_VALID_SUBJ_REGEX),
            "Set the regular expression string to match valid mail message subject(s).\n")

        ("mail.xDlyS",
            value<unsigned int>(&XFloatMailRetrievalService::sleep_seconds_if_no_new_msgs_) //
              ->default_value(CFG_DEFAULT_MAIL_CHK_DLY),
            "Set the interval between checks of the mail server for new mail messages, in "
            "seconds.\n");

    /* Define SQL server options: */

    desc_sql.add_options()

        ("sql.host",
            value<string>(&LocalMysqlConfiguration::address_) //
              ->default_value(CFG_MYSQL_DFLT_ADDRESS),
            "Set the SQL server qualified name or IP address.\n")

        ("sql.password",
            value<string>(&LocalMysqlConfiguration::password_),
            "Set the SQL server account password.\n")

        ("sql.port",
            value<unsigned int>(&LocalMysqlConfiguration::port_) //
              ->default_value(CFG_MYSQL_DFLT_PORT),
            "Set the SQL server port number.\n")

        ("sql.protocol",
            value<string>(&LocalMysqlConfiguration::protocol_) //
              ->default_value(CFG_MYSQL_DFLT_PROTOCOL),
            "Set the SQL server protocol e.g. \"tcp\".\n")

        ("sql.schema",
            value<string>(&LocalMysqlConfiguration::schema_) //
              ->default_value(CFG_MYSQL_DFLT_SCHEMA),
            "Set the SQL server schema name.\n")

        ("sql.user",
            value<string>(&LocalMysqlConfiguration::user_),
            "Set the SQL server account username.\n");

    /* Define config file options: */

    desc_cfg.add(desc_dir);
    desc_cfg.add(desc_file);
    desc_cfg.add(desc_mail);
    desc_cfg.add(desc_sql);

  }CATCH_BLOCK("Failed to get configuration file run-time options description!")

  return desc_cfg;
}

} /* namespace config */

namespace sig {

/** The exit handler.
 *
 *  This function is responsible for handling signals received from the local system. It is
 *  invoked when a signal is caught, but may also be invoked "manually" anywhere in the execution
 *  of the primary daemon thread to terminate the application, for example, at the bequest of a
 *  user.
 *
 *  This function attempts an orderly shutdown of the application. First, the worker threads are
 *  terminated. Then, locally cached data is saved to the database. Next, run-time lock is
 *  released. Finally, the globally logger is flushed and the application is terminated.
 *
 *  @param[in]  signo   The signal number.
 *
 *  @exception  None. Never throws.
 */
void SignalHandler::ExitHandler(int signo) {

  using argo::iridium::xfloat::XFloat;
  using util::config::LocalConfiguration;
  using util::log::Log;

  try {
    switch(signo) {

      case SIGABRT:

        lout() << "Caught SIGABRT! Shutting down...";
        break;

      case SIGINT:

        lout() << "Caught SIGINT! Shutting down...";
        break;

      case SIGQUIT:

        lout() << "Caught SIGQUIT! Shutting down...";
        break;

      case SIGTERM:

        lout() << "Caught SIGTERM! Shutting down...";
        break;

      case SIGTSTP:

        lout() << "Caught SIGTSTP! Shutting down...";
        break;

      default:

        lout() << "Caught unknown signal! Shutting down...";
        break;
    }

    TerminateWorkerThreads();                     // Terminate worker threads.

    try {
      XFloat::SaveLocalCacheToDatabase();         // Save cached data.
    }CATCH_NOTHROW("Failed to save local cached data to database!")

    try {
      LocalConfiguration::ReleaseRuntimeLock();   // Release run-time lock.
    }CATCH_NOTHROW("Failed to release run-time lock!")

    try {
      Log::FlushGlobalLogger();                   // Flush global logs.
    }CATCH_NOTHROW("Failed to flush the global logger!")

    exit(EXIT_SUCCESS);

  }CATCH_NOTHROW("Exception thrown in exit handler!")

  exit(EXIT_FAILURE);
}

} /* namespace sig */
} /* namespace util */
