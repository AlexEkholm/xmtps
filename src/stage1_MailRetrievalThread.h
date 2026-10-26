/** The @ref argo::iridium::xfloat::XFloatMailRetrievalService class header file.
 *
 *  This is the header file for the @ref argo::iridium::xfloat::XFloatMailRetrievalService
 *  class, which defines the operation of the mail retrieval thread. This thread is the first
 *  stage in the  processing pipeline and is is responsible for fetching email messages for the
 *  mail server and/or SQL database, and queuing them for consumption by the next stage in the
 *  processing pipeline, @ref argo::iridium::xfloat::XFloatMailProcessingService.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 16, 2014 2:21:06 PM
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

#ifndef XFLOATMAILRETRIEVALSERVICE_H_
#define XFLOATMAILRETRIEVALSERVICE_H_

/* Includes: */

// std
#include <chrono>

// local
#include "stage2_MailProcessingThread.h"

namespace argo {
namespace iridium {
namespace xfloat {

/* Class Definition: */




typedef enum MailFetchMode {
  MAIL_FETCH_SERVER = 0,
  MAIL_FETCH_FILESYSTEM = 1,
} XFloatMailFetchMode_t;

/** The mail retrieval thread class.
 *
 *  This class defines the operation of the mail retrieval thread, which is responsible for
 *  fetching email messages for the mail server and/or SQL database, and queuing them for
 *  consumption by the next stage in the processing pipeline, @ref XFloatMailProcessingService.
 *  The mail retrieval thread is the first stage in the processing pipeline.
 *
 *  A single instance of this thread will be instantiated after application initialization. The
 *  function of the thread is defined by functor operator overload function. The thread enters
 *  a primary loop in which mail messages are queried from the database and mail server. If no
 *  messages are returned, the thread disconnects from the mail server and database, and sleeps for
 *  a period of time before resuming the loop. The loop continues until termination is signaled
 *  by the daemon thread, at which point the thread will disconnect from the mail server and die.
 *
 *  All the credentials for connecting to the mail server are stored as static members of this
 *  class. These values are set as a result of parsing the command-line and/or configuration
 *  options during application initialization.
 */
class XFloatMailRetrievalService:
    public util::file::FileSystemUtils,
    public util::mysql::LocalMysqlConfiguration {

    /* Friend Functions: */

    /** Friend function for populating some of the static members of this class from the
     *  configuration file options.
     */
    friend options_description util::config::LocalConfiguration::GenerateConfigurationFileOptionsDescription();

    /** Friend function for populating static members of this class from the command-line options.
     *
     */
    friend options_description util::config::LocalConfiguration::GenerateCommandLineOptionsDescription();

  private:

    /* Nested Classes: */

#   if defined(XMTPS_DEBUG_FETCH_MAIL) && XMTPS_DEBUG_FETCH_MAIL
    /** The MailTracer class.
     *
     *  This class is used to trace server-client communications between the mailserver and the
     *  application.
     *
     */
    class MailTracer:
    public vmime::net::tracer {

      private:

      /* Private Data Members: */

      const vmime::string m_proto;
      const int m_connectionId;

      public:

      /* Public Member Functions: */

      MailTracer(const vmime::string& proto, const int connectionId) :
          m_proto(proto),
          m_connectionId(connectionId) {
        /* Do nothing. */
      }

      // Called by VMime to trace what is sent on the socket
      void traceSend(const vmime::string& line) {

        ldbg() << "[tracer]"
        << "["
        << m_proto
        << ":"
        << m_connectionId
        << "] C: "
        << line
        << std::endl;

      }

      // Called by VMime to trace what is received from the socket
      void traceReceive(const vmime::string& line) {

        ldbg() << "[tracer]"
        << "["
        << m_proto
        << ":"
        << m_connectionId
        << "] S: "
        << line
        << std::endl;

      }

    };
#   endif // defined(XMTPS_DEBUG_FETCH_MAIL) && XMTPS_DEBUG_FETCH_MAIL

#   if defined(XMTPS_DEBUG_FETCH_MAIL) && XMTPS_DEBUG_FETCH_MAIL
    class MailTracerFactory:
    public vmime::net::tracerFactory {

      public:

      /* Public Member Functions: */

      MailTracerFactory() {

        /* Do nothing. */

      }

      vmime::shared_ptr<vmime::net::tracer> create(
          vmime::shared_ptr<vmime::net::service> serv,
          const int connectionId) {

        return vmime::make_shared<MailTracer>(serv->getProtocolName(), connectionId);

      }

    };
#   endif // defined(XMTPS_DEBUG_FETCH_MAIL) && XMTPS_DEBUG_FETCH_MAIL

    /** Timeout handler for the mail server connection.
     *
     *  VMime calls it while it waits for data from the mail server. An operation is cancelled,
     *  with a vmime::exceptions::operation_timed_out exception, when nothing has been received
     *  for @ref mail_server_timeout_s_ seconds, or at once when termination is signaled, so a
     *  stalled connection cannot block the thread (and the daemon's shutdown) indefinitely.
     */
    class MailTimeoutHandler:
    public vmime::net::timeoutHandler {

      public:

      /* Public Member Functions: */

      bool isTimeOut() {

        return termination_signaled_
            || std::chrono::steady_clock::now() - start_
                > std::chrono::seconds(mail_server_timeout_s_);
      }

      void resetTimeOut() {

        start_ = std::chrono::steady_clock::now();
      }

      bool handleTimeOut() {

        if(!termination_signaled_)
          lwrn() << "No response from the mail server for " << mail_server_timeout_s_
                 << " s: operation cancelled.";

        return false;   // Cancel the operation.
      }

      private:

      /* Private Data Members: */

      std::chrono::steady_clock::time_point start_ = std::chrono::steady_clock::now();

    };

    /** Factory for @ref MailTimeoutHandler objects, one per mail server connection. */
    class MailTimeoutHandlerFactory:
    public vmime::net::timeoutHandlerFactory {

      public:

      /* Public Member Functions: */

      vmime::shared_ptr<vmime::net::timeoutHandler> create() {

        return vmime::make_shared<MailTimeoutHandler>();
      }

    };

  private:

    /* Static Data Members: */

    /** The time, in seconds, after which a mail server operation that receives nothing is
     *  cancelled (see @ref MailTimeoutHandler).
     */
    static unsigned int mail_server_timeout_s_;

    static bool is_reprocess_active_;


    /** The reprocess-all-mail-messages boolean flag.
     *
     *  The value of this field is set as a result of parsing the command-line options. If
     *  unspecified in the command-line arguments, this field defaults to a the value
     *  @ref CFG_DEFAULT_REPROCESS_ALL.
     *
     *  The value of this boolean flag determines which messages are retrieved from the mail
     *  server upon startup of the application. If TRUE, then mail messages will be fetched from
     *  mail server starting with UID 0. Otherwise, if FALSE, the starting UID of mail messages
     *  to be fetched from the server will be determined by querying the maximum UID of the mail
     *  messages stored in the SQL database. Mail message will be fetched from the mail server,
     *  starting with the UID queried from the database plus one.
     */
    static bool is_reprocess_all_;

    /** The mail fetch mode flag.
     *
     *  - 0: Fetch mail from mail server.
     *  - 1: Fetch mail from file system.
     *
     */
    static int mail_fetch_mode_;

    static path mail_file_input_directory_;

    static path mail_file_processed_directory_;

    /** The mail server host qualified name or IP address string.
     *
     *  The value of this field is set as a result of parsing the command-line options. If
     *  unspecified, it defaults to "maila.whoi.edu".
     */
    static string mail_server_host_;

    /** The mail server remote incoming mail message folder path string.
     *
     *  The value of this field is set as a result of parsing the command-line options. If
     *  unspecified, it defaults to "INBOX".
     */
    static string mail_server_inbox_folder_path_;

    /** The mail server account password string.
     *
     *  The value of this field is set as a result of parsing the command-line options.
     */
    static string mail_server_password_;

    /** The mail server protocol name string.
     *
     *  The value of this field is set as a result of parsing the command-line options. If
     *  unspecified, it defaults to "imap".
     */
    static string mail_server_protocol_;

    /** The mail server account username string.
     *
     *  The value of this field is set as a result of parsing the command-line options. If
     *  unspecified, it defaults to "imap".
     */
    static string mail_server_user_;

    /** The maximum number of messages to simultaneously fetch from the mail server.
     *
     *  The value of this field is set as a result of parsing the command-line options. If
     *  unspecified, it defaults to @ref CFG_DEFAULT_MAX_FETCH.
     */
    static size_t max_num_msgs_to_fetch_;

    /** The starting index of the sequence of messages to fetch.
     *
     *  If messages are moved out of the INBOX folder after being processed, then the messages
     *  sequence start number will always be one. Otherwise, this field will contain the starting
     *  index of the first message not fetched from the mail server.
     *
     */
    static unsigned int message_start_index_;

    /** Move mail messages flag.
     *
     *  If set, then the mail messages will be moved out of the inbox folder after being
     *  processed, i.e. once the mail processing thread has saved them to the SQL database (or
     *  rejected them). See @ref MoveProcessedMessages().
     *
     */
    static bool move_mail_messages_;

    /** Fetched mail messages waiting to be moved out of the inbox once processed: message UID
     *  (as in XFloatMailMessage::id_msg_uid()) to the IMAP UID and the destination folder.
     */
    static map<int, pair<vmime::net::message::uid, string> > pending_moves_;

    /** Whether the SQL server was found unavailable at the last check before fetching. */
    static bool sql_server_unavailable_;

    /** A vector containing the unique instrument identifier of messages to reprocess from the SQL
     *  database.
     *
     *  If the user specifies one or more instruments to reprocess by specifying their unique
     *  instrument identifiers, then those identifiers will be contained in this vector. Otherwise,
     *  this vector will be empty.
     *
     *  After the thread is instantiated, it will check the size of this vector. If the vector size
     *  is non-zero, then the thread will query the SQL database for messages with instrument
     *  identifiers matching those contained in this vector. Those messages will then be
     *  passed on to the mail processing thread.
     *
     *  Regardless of the size of this vector, the mail retrieval thread will then continue on to
     *  enter its primary loop until termination is signaled.
     *
     */
    static vector<unsigned int> reprocess_by_inst_ids_;

    /** The time to sleep if no new mail messages are fetched, in seconds.
     *
     *  If no new mail messages are fetched, the mail retrieval thread will sleep for this period
     *  of time, in increments of @ref XFLT_THREAD_SLEEP_MS.
     *
     *  The value of this field is set as a result of parsing the command-line options. If
     *  unspecified, it defaults to @ref CFG_DEFAULT_MAIL_CHK_DLY.
     */
    static unsigned int sleep_seconds_if_no_new_msgs_;

    /** The termination signaled flag.
     *
     *  This flag is used  to signal termination of the mail retrieval thread. During normal
     *  operation, the value of this field will be FALSE. Once termination is signaled, it will
     *  become TRUE causing all of the single instance of the mail retrieval to to exit its
     *  primary loop and die.
     */
    static std::atomic<bool> termination_signaled_;

    /** The instance of the mail retrieval thread.
     *
     *  The thread operation is defined by the functor operator of the runnable object, @ref
     *  thread_object_, which is invoked when this thread is instantiated.
     */
    static thread thread_;

    /** The mail retrieval thread object.
     *
     *  This is a reference to a runnable object whose functor operator is invoked during
     *  instantiation of the mail retrieval thread, @ref thread_.
     */
    static shared_ptr<XFloatMailRetrievalService> thread_object_;

  private:

    /* Dynamic Data Members: */

    /** Is propagation of mail messages to the mail processing thread enabled?
     *
     *  This field determines whether fetched messages are queued for consumption by the next
     *  thread in the processing pipeline, @ref XFloatMailProcessingService. If no instances of
     *  that thread are running, then this field will be FALSE and message will be discarded
     *  rather than queued. Otherwise, this field will be TRUE and messages will be queued for
     *  consumption by @ref XFloatMailProcessingService.
     */
    bool processing_thread_propagation_enabled_;

    /** The VMime session object reference.
     *
     *  This object is used to connect to the mail server, and create the @ref mail_store_ object.
     *  It is created when the thread connects to the mail server and released when the thread
     *  disconnects from the mail server.
     */
    vmime::shared_ptr<vmime::net::session> mail_session_;

    /** The VMime mail store object reference.
     *
     *  This object encapsulates the protocols that provide access to the mail drop. It is created
     *  when the thread connects to the mail server and released when the thread disconnects from
     *  the mail server.
     */
    vmime::shared_ptr<vmime::net::store> mail_store_;

    /** The VMime inbox folder object reference.
     *
     *  This object represents the folder on the mail server that contains incoming messages.
     *  Typically, this folder is "INBOX".
     */
    vmime::shared_ptr<vmime::net::folder> inbox_folder_;

  private:

    /* Private Static Member Functions: */

    static vector<path> ListRawMailFiles();

    static bool PushForProcessing(const shared_ptr<XFloatMailMessage>& xmsg);

    static void MoveProcessedMessages(const vmime::shared_ptr<vmime::net::folder>& folder_inbox);

    static void set_termination_signaled(bool termination_signaled = true);

    static bool SqlServerAvailableForFetching();


  public:

    /* Public Static Member Functions: */

    static void StartWorkerThread() ATTR_NO_EXCEPTIONS;

    static void TerminateWorkerThread() ATTR_NO_EXCEPTIONS;

  private:

    /* Private Member Functions: */

    vmime::shared_ptr<vmime::net::store> ConnectToMailServer();

    void DisconnectFromMailServer() ATTR_NO_EXCEPTIONS;

    size_t FetchMailMessagesFromFileSystem();

    size_t FetchMailMessagesFromServer();

    string GenerateMailServerUrlString() const;

    const vmime::shared_ptr<vmime::net::session>& mail_session();

    const vmime::shared_ptr<vmime::net::folder>& OpenMailInboxFolder();

  public:

    /* Public Member Functions: */

    XFloatMailRetrievalService(bool prop = (XFloatMailProcessingService::num_threads() > 0));

    ~XFloatMailRetrievalService();

    void operator()() ATTR_NO_EXCEPTIONS;

};

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */

#endif /* XFLOATMAILRETRIEVALSERVICE_H_ */
