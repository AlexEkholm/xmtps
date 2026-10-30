/** The @ref argo::iridium::xfloat::XFloatMailRetrievalService class source file.
 *
 *  This is the source file for the @ref argo::iridium::xfloat::XFloatMailRetrievalService
 *  class, which defines the operation of the mail retrieval thread. This thread is the first
 *  stage in the  processing pipeline and is is responsible for fetching email messages for the
 *  mail server and/or SQL database, and queuing them for consumption by the next stage in the
 *  processing pipeline, @ref argo::iridium::xfloat::XFloatMailProcessingService.
 *
 *  The file contains the static member variables declared in the @ref
 *  argo::iridium::xfloat::XFloatMailRetrievalService class. These include credentials and details
 *  needed for connecting to the mail server, which are set as a result of parsing the
 *  command-line and/or configuration file options during initialization.
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

/* Includes: */

// header
#include "stage1_MailRetrievalThread.h"

// vmime
#include <vmime/utility/urlUtils.hpp>

namespace argo {
namespace iridium {
namespace xfloat {

/* Static Variables: */

bool XFloatMailRetrievalService::is_reprocess_active_;

bool XFloatMailRetrievalService::is_reprocess_all_;

int XFloatMailRetrievalService::mail_fetch_mode_;

path XFloatMailRetrievalService::mail_file_input_directory_;

path XFloatMailRetrievalService::mail_file_processed_directory_;

std::atomic<bool> XFloatMailRetrievalService::input_exhausted_(false);

string XFloatMailRetrievalService::mail_server_host_;

string XFloatMailRetrievalService::mail_server_inbox_folder_path_;

string XFloatMailRetrievalService::mail_server_password_;

string XFloatMailRetrievalService::mail_server_protocol_;

string XFloatMailRetrievalService::mail_server_user_;

size_t XFloatMailRetrievalService::max_num_msgs_to_fetch_;

unsigned int XFloatMailRetrievalService::message_start_index_(1);

bool XFloatMailRetrievalService::move_mail_messages_;

unsigned int XFloatMailRetrievalService::mail_server_timeout_s_;

map<int, pair<vmime::net::message::uid, string> > XFloatMailRetrievalService::pending_moves_;

bool XFloatMailRetrievalService::sql_server_unavailable_(false);

vector<unsigned int> XFloatMailRetrievalService::reprocess_by_inst_ids_;

unsigned int XFloatMailRetrievalService::sleep_seconds_if_no_new_msgs_;

std::atomic<bool> XFloatMailRetrievalService::termination_signaled_(false);

thread XFloatMailRetrievalService::thread_;

shared_ptr<XFloatMailRetrievalService> XFloatMailRetrievalService::thread_object_;

/* Static Function Definitions: */

/** Instantiate the mail retrieval worker thread.
 *
 *  This function is invoked by the daemon thread after initialization to instantiate the mail
 *  retrieval thread. It creates a new instance of @ref XFloatMailRetrievalService, and
 *  instantiates a new thread that invokes the functor operator of the object.
 *
 *  @exception          None. Never throws.
 */
void XFloatMailRetrievalService::StartWorkerThread() ATTR_NO_EXCEPTIONS {

    try {

        ldbg() << "Starting mail retrieval thread...";
        thread_object_ = make_shared<XFloatMailRetrievalService>();
        thread_ = thread(*thread_object_);

    } CATCH_BLOCK("Failed to start mail retrieval thread!")

}

/** Terminate the mail retrieval worker thread.
 *
 *  This function is used by the daemon thread to kill the worker thread upon application
 *  termination. The function signals termination, and then attempts to join the child thread.
 *  If the child thread does not join, then the function will timeout after @ref
 *  XMTPS_THREAD_JOIN_SECONDS seconds, a warning will be issued, and the child thread will be
 *  abandoned.
 *
 *  @exception          None. Never throws.
 */
void XFloatMailRetrievalService::TerminateWorkerThread() ATTR_NO_EXCEPTIONS {

    try {

        ldbg() << "Terminating mail retrieval thread...";
        set_termination_signaled();

        if(thread_.joinable()
                && !thread_.try_join_for(boost::chrono::seconds(XMTPS_THREAD_JOIN_SECONDS))) {

            lwrn() << "Failed to join mail retrieval thread after waiting "
                      << XMTPS_THREAD_JOIN_SECONDS
                      << " seconds!";
        }

    }CATCH_BLOCK("Failed to terminate NetCDF file writer thread!")
}

/** Check the options that need the SQL database.
 *
 *  Called by the daemon thread during initialization. Without the SQL database (sql.enable = 0)
 *  there are no stored messages to reprocess and nowhere to save fetched mail before it is moved
 *  out of the inbox, so reprocessing given on the command line (-r, -R) and mail.moveMessages are
 *  rejected. Reprocessing active floats (-A, on by default) and the reprocessing implied by
 *  --phy-enable are skipped.
 *
 *  @throw          util::log::MessageException if an option needs the SQL database but it is
 *                  disabled.
 */
void XFloatMailRetrievalService::CheckSqlDependentOptions() {

    if(is_sql_enabled())
        return;

    if(is_reprocess_all_ || !reprocess_by_inst_ids_.empty())
        EXC_THROW("Reprocessing (-r, -R) needs the SQL database, but it is disabled "
                  "(sql.enable = 0)!");

    if(move_mail_messages_ && mail_fetch_mode_ == MAIL_FETCH_SERVER)
        EXC_THROW("mail.moveMessages = 1 needs the SQL database, but it is disabled "
                  "(sql.enable = 0): messages would leave the inbox without being saved!");

    is_reprocess_active_ = false;

    lout() << "SQL database disabled (sql.enable = 0): nothing is loaded from or saved to the "
              "database, and floats are built from the mail messages read in this run.";
}

/** Has a one-shot (non-daemon) run read all of its input?
 *
 *  When reading RAW mail files (mail.fetchMode = 1), this is TRUE once every file in the input
 *  directory has been queued for processing. In the other fetch modes it is always TRUE, so a
 *  one-shot run exits after the start-up delay, as before.
 *
 *  @return         TRUE if a one-shot run may exit.
 */
bool XFloatMailRetrievalService::one_shot_input_done() {
    return mail_fetch_mode_ != MAIL_FETCH_FILESYSTEM || input_exhausted_;
}

/** Set the termination signaled flag.
 *
 *  This function is called by @ref TerminateWorkerThreads() in order to cause all currently
 *  running instances of the mail retrieval thread to exit their primary loop and die.
 *
 *  @param[in]      termination_signaled        The termination signaled value to set.
 */
void XFloatMailRetrievalService::set_termination_signaled(bool termination_signaled) {
    termination_signaled_ = termination_signaled;
}

/* Function Definitions : */

/** Connect to the mail server.
 *
 *  If the thread is not connected to the mail server i.e. @ref mail_store_ is NULL, then this
 *  function creates the @ref mail_store_ object from the mail server URL string and attempts
 *  to connect to the mail server. If the thread is already connected to the mail server, then
 *  this function will have no effect.
 *
 *  @throw          log::MessageException if the function fails to connect to the mail server.
 *
 *  @return         A shared reference to @ref mail_store_.
 */
vmime::shared_ptr<vmime::net::store> XFloatMailRetrievalService::ConnectToMailServer() {

    try {

        if(!mail_store_)

            try {

                // Get the store service object.
                ldbg() << "Connecting IMAP mail server: " << mail_server_host_ << "...";
                mail_store_ = mail_session()->getStore(
                        vmime::utility::url(GenerateMailServerUrlString()));

                // Cancel operations that receive nothing from the server for too long.
                mail_store_->setTimeoutHandlerFactory(
                        vmime::make_shared<MailTimeoutHandlerFactory>());

#                   if defined(XMTPS_DEBUG_FETCH_MAIL) && XMTPS_DEBUG_FETCH_MAIL

                // Enable the mail tracer.
                mail_store_->setTracerFactory(vmime::make_shared<MailTracerFactory>());

#                   endif // defined(XMTPS_DEBUG_FETCH_MAIL) && XMTPS_DEBUG_FETCH_MAIL

            }CATCH_BLOCK("Failed to get mail store service!")

        if(!mail_store_->isConnected())

            try {

                // Connect to the store.
                ldbg() << "Connecting to mail server...";
                mail_store_->connect();
                ldbg() << "Done. Successfully connected to IMAP store.";

            }CATCH_BLOCK("Failed to connect to the mail store!")

    }CATCH_BLOCK("Failed to connect to mail server!")

    return mail_store_;
}

/** Disconnect from the mail server.
 *
 *  This function disconnects from the mail server and releases the reference to the VMime
 *  objects. If the thread is already disconnected from the mail server, then this function
 *  will have no effect. Otherwise, the @ref inbox_folder_ object is closed and released. Then,
 *  the @ref  mail_store_ object is disconnected and released. Finally, the @ref mail_session_
 *  object is released.
 *
 *  @throw          log::MessageException if an error occurs while attempting to disconnect
 *                  from the mail server.
 */
void XFloatMailRetrievalService::DisconnectFromMailServer() ATTR_NO_EXCEPTIONS {

    try {

        ldbg() << "Disconnecting from mail server....";

        if(inbox_folder_) {

            try {

                if(inbox_folder_->isOpen())

                    try {

                        // Close mail folder.
                        inbox_folder_->close(false);

                    }CATCH_BLOCK("Failed to close the inbox folder!")

            }CATCH_NOTHROW("Exception caught while closing inbox folder object!")

            inbox_folder_.reset();

        }

        if(mail_store_) {

            try {

                if(mail_store_->isConnected())

                    try {

                        // Disconnect mail store.
                        mail_store_->disconnect();

                    }CATCH_BLOCK("Failed to disconnect the mail store!")

            }CATCH_NOTHROW("Exception caught while closing mail store object!")

            mail_store_.reset();
        }

        if(mail_session_)

            // Close mail session.
            mail_session_.reset();

    }CATCH_NOTHROW("Failed to disconnect from mail server!")

}


/** Push a mail message into the mail processing queue, waiting while the queue is full.
 *
 *  The wait ends when termination is signaled, so a full queue does not hold up shutdown; the
 *  message is then not queued.
 *
 *  @param[in]      xmsg            The mail message to push.
 *
 *  @return         TRUE if the message was queued, or FALSE if termination was signaled first.
 */
bool XFloatMailRetrievalService::PushForProcessing(const shared_ptr<XFloatMailMessage>& xmsg) {

    while(!XFloatMailProcessingService::MsgProcessingQueuePush(xmsg, false)) {

        if(termination_signaled_)
            return false;

        sleep(millisec(XFLT_THREAD_SLEEP_MS));
    }

    return true;
}

/** Fetch a batch of messages from the file system.
 *
 *  @return         The number of messages fetched.
 */
size_t XFloatMailRetrievalService::FetchMailMessagesFromFileSystem() {

    __pid_t pid;

    static vector<path> paths;
    static shared_ptr<XFloatMailMessage> xmsg;

    path mail_file_path;
    path lock_file_path;
    size_t num_processed = 0;
    bool is_processed;

    // Init vars
    xmsg.reset();
    paths.reserve(max_num_msgs_to_fetch_+1);
    paths.resize(0);


    try {

      // Iterate over mail file paths.
      paths = ListRawMailFiles();
      for(vector<path>::iterator it = paths.begin(); it != paths.end(); ++it)
        try {
          mail_file_path = *it;

          ldbg() << "Processing raw mail input file: " << (*it).string() << "...";

          // Check if file exists.
          if(!exists(*it))
              EXC_THROW("File does not exist!");

          // Check for lock file.
          lock_file_path = path(mail_file_path.string() + ".lck");
          if(exists(lock_file_path)) {

            // Get PID of previous lock.
            ifstream ifs(lock_file_path);
            ifs >> pid;
            ifs.close();

            // Compose and throw exception.
            ostringstream oss;
            oss << "Lock file already exists: " << lock_file_path << endl;
            oss << "\tPID: " << pid;
            EXC_THROW(oss.str());

          } else {

            // Write PID to lock file.
            ofstream ofs(lock_file_path);
            ofs << getpid();
            ofs.flush();
            ofs.close();

          }

          is_processed = false;

          try {

              // Read raw mail file from path
              xmsg = make_shared<XFloatMailMessage>();
              ifstream ifs(mail_file_path);
              ifs >> *(xmsg);
              ifs.close();


              if(move_mail_messages_) {


              }


              // Push message to the processing queue. On termination, leave the file for the
              // next run.
              if(!PushForProcessing(xmsg)) {
                  remove(lock_file_path);
                  break;
              }

              is_processed = true;
              ++num_processed;

              // Move the file to the processed directory, or remove it if none is set.
              if(mail_file_processed_directory_.empty())
                  remove(mail_file_path);
              else
                  try {

                      MoveFileToDirectory(mail_file_path, mail_file_processed_directory_, true);
                      remove(mail_file_path);

                  } catch(...) {

                      // Keep the file, under a name that is not fetched again.
                      lwrn() << "Failed to move raw mail input file to "
                             << mail_file_processed_directory_.string() << "; renaming it: "
                             << mail_file_path.string() << ".processed";
                      rename(mail_file_path, path(mail_file_path.string() + ".processed"));
                  }


          } CATCH_NOTHROW("Failed to parse raw mail input file!")

          if(!is_processed && exists(mail_file_path))

              try {

                  // Rename the failed file so it is not fetched again.
                  lwrn() << "Renaming unprocessable raw mail input file: "
                         << mail_file_path.string() << ".err";
                  rename(mail_file_path, path(mail_file_path.string() + ".err"));

              } CATCH_NOTHROW("Failed to rename unprocessable raw mail input file!")

          // Remove lock file
          remove(lock_file_path);

          // reset pointers
          xmsg.reset();


        } CATCH_NOTHROW("Failed to process raw mail input file!")

        // reset pointers
        xmsg.reset();

        // Return only the files actually queued, so the caller sleeps when nothing succeeded.
        return num_processed;


    } CATCH_BLOCK("Failed to fetch mail messages from file system.")

    return 0;

}

/** Fetch a batch of mail messages from the mail server.
 *
 *  Connects to the mail server and fetches a batch of mail messages. Fetched messages are
 *  pushed into the mail processing queue.
 *
 *  @return         The number of messages fetched.
 */
size_t XFloatMailRetrievalService::FetchMailMessagesFromServer() {

    using vmime::net::folder;
    using vmime::net::message;
    using vmime::net::messageSet;

    /* This set of attributes determines which parts of the messages are fetched from the mail
     * server.
     */
    static const fetchAttributes fattr(
            fetchAttributes::ENVELOPE | fetchAttributes::FULL_HEADER
                    | fetchAttributes::CONTENT_INFO | fetchAttributes::STRUCTURE
                    | fetchAttributes::UID);

    static vmime::shared_ptr<folder> folder_inbox;
    static vmime::shared_ptr<message> msg;
    static size_t msg_count;
    static vector<vmime::shared_ptr<message>> msgs;
    static shared_ptr<XFloatMailMessage> xmsg;

    msgs.reserve(max_num_msgs_to_fetch_+1);

    // Initialize variables.
    msg_count = 0;
    msgs.resize(0);
    folder_inbox.reset();
    msg.reset();


    try {

        ldbg() << "Checking server for mail messages...";

        // Open the inbox folder.
        if(!(folder_inbox = OpenMailInboxFolder()))
            EXC_THROW("Failed to open mail inbox folder!");

        // Move the messages fetched earlier that have been processed out of the inbox.
        if(move_mail_messages_)
            MoveProcessedMessages(folder_inbox);

        // Check the message start index.
        if(message_start_index_ < 1)
            message_start_index_ = 1;

        // Get the message count.
        if((msg_count = folder_inbox->getStatus()->getMessageCount()) < message_start_index_) {

            // There are no new messages to fetch.
            ldbg() << "No new messages found on mailserver.";

            // Return number of messages fetched.
            return 0;
        }

        // Construct the messages set.
        messageSet msg_set_fetch(
                messageSet::byNumber(
                        message_start_index_,
                        std::min(
                                message_start_index_ + max_num_msgs_to_fetch_ - 1,
                                msg_count)));

        // Fetch the messages.
        msgs = folder_inbox->getAndFetchMessages(msg_set_fetch, fattr);

        // Check if any new messages were received.
        if(msgs.empty()) {

            // There are no new messages to fetch.
            ldbg() << "Fetched 0 existing message(s) from the mailserver...";

            // Reset pointer.
            folder_inbox.reset();

            // Return number of messages fetched.
            return 0;

        }

        // Parse messages and update minimum UID for next mail message fetch..
        lout() << "Fetched " << msgs.size() << " new message(s) from mail server...";

        // Iterate over fetched messages.
        for(auto it = msgs.begin(); it != msgs.end(); ++it)

            try {

                xmsg = std::make_shared<XFloatMailMessage>(msg = *it);

                /* Register the message for moving before it is queued: it is moved out of the
                 * inbox only once the mail processing thread has processed it (see
                 * MoveProcessedMessages()), so a message whose processing is interrupted, e.g.
                 * by an SQL server outage, stays in the inbox. */
                if(move_mail_messages_)
                    pending_moves_[xmsg->id_msg_uid()] = make_pair(
                            msg->getUID(),
                            util::date::DateUtils::GenerateIso8601String(
                                    xmsg->msg_header_datetime(),
                                    false).substr(0, 6));

                // Push message to the processing queue. On termination, the message and the
                // rest of the batch stay in the inbox for the next run.
                if(!PushForProcessing(xmsg)) {
                    pending_moves_.erase(xmsg->id_msg_uid());
                    break;
                }

            }CATCH_NOTHROW("Failed to process fetched message!")

        // Reset pointers.
        msg.reset();
        xmsg.reset();

        /* Step past the fetched messages: they stay in the inbox until they are moved, and
         * MoveProcessedMessages() lowers the index by the number of messages it removes. */
        message_start_index_ = std::min(
                message_start_index_ + max_num_msgs_to_fetch_,
                msg_count + 1);

        // Reset pointer.
        folder_inbox.reset();

        // Return number of messages fetched.
        return msgs.size();

    }CATCH_BLOCK("Failed to fetch mail messages from server!")

    return 0;

}

/** Move the fetched mail messages that have been processed out of the inbox.
 *
 *  A fetched message is moved only once the mail processing thread has processed it, i.e. saved
 *  it to the SQL database or rejected it (see XFloatMailProcessingService::TakeProcessedMsgUids()),
 *  so a message whose processing is interrupted, e.g. by an SQL server outage or a restart, stays
 *  in the inbox and is fetched again. Each message is copied to the folder named after the year
 *  and month of its header date, then deleted from the inbox, and the inbox is expunged. If a
 *  move fails, its messages stay pending and are moved after the next fetch; if it times out
 *  (see @ref MailTimeoutHandler), the exception is rethrown so the fetch reconnects.
 *
 *  The moved messages were fetched earlier, so they are all below @ref message_start_index_,
 *  which is lowered by the number of messages removed from the inbox.
 *
 *  @param[in]      folder_inbox    The open inbox folder.
 */
void XFloatMailRetrievalService::MoveProcessedMessages(
        const vmime::shared_ptr<vmime::net::folder>& folder_inbox) {

    using vmime::net::folder;
    using vmime::net::folderAttributes;
    using vmime::net::message;
    using vmime::net::messageSet;

    // Collect the processed messages by destination folder: message UID and IMAP UID.
    map<string, vector<pair<int, message::uid> > > folder2uid_map;
    for(int uid : XFloatMailProcessingService::TakeProcessedMsgUids()) {

        auto it = pending_moves_.find(uid);
        if(it == pending_moves_.end())
            continue;

        folder2uid_map[it->second.second].push_back(make_pair(uid, it->second.first));
        pending_moves_.erase(it);
    }

    // Move message(s) to the appropriate folder(s).
    for(auto it = folder2uid_map.begin(); it != folder2uid_map.end(); ++it)

        try {

          vector<message::uid> imap_uids;
          for(const auto& u : it->second)
              imap_uids.push_back(u.second);

          try {

            // Get the destination folder.
            vmime::shared_ptr<folder> folder_dest(
                    folder_inbox->getFolder(folder::path::component(it->first)));
            if(!folder_dest)
                EXC_THROW("Failed to get destination folder: " + it->first);

            // Check if the destination folder exists.
            if(!folder_dest->exists()) {

                ldbg() << "Creating folder: " << it->first;
                folder_dest->create(folderAttributes());
            }

            // Copy the message(s) to the destination folder.
            ldbg() << "Copying " << it->second.size() << " message(s) to folder: " << it->first;
            folder_inbox->copyMessages(folder_dest->getFullPath(), messageSet::byUID(imap_uids));

            // Delete the message(s) from the inbox.
            ldbg() << "Deleting " << it->second.size() << " message(s) from inbox";
            folder_inbox->deleteMessages(messageSet::byUID(imap_uids));
            folder_inbox->expunge();

            // The removed messages were below the fetch index.
            message_start_index_ = (message_start_index_ > it->second.size())
                    ? message_start_index_ - it->second.size()
                    : 1;

          } catch(...) {

            /* Keep the messages pending, so that the move is retried after the next fetch (if
             * the copy succeeded but the delete did not, they are copied again). */
            for(const auto& u : it->second)
                pending_moves_[u.first] = make_pair(u.second, it->first);
            throw;
          }

        } catch(const vmime::exceptions::operation_timed_out&) {

            // The connection is unusable: abort the fetch, which closes and reopens it.
            throw;

        }CATCH_NOTHROW("Failed to move mail message(s) to folder: " + it->first)
}

/** Check that the SQL server is available before fetching mail messages.
 *
 *  While the SQL server is unavailable, no mail messages are fetched, so they stay on the mail
 *  server (or in the input directory) instead of waiting in memory until they can be saved. The
 *  start and end of an outage are logged once.
 *
 *  @return         TRUE if the SQL server is available.
 */
bool XFloatMailRetrievalService::SqlServerAvailableForFetching() {

    bool available = isSqlServerAvailable();

    if(!available && !sql_server_unavailable_)
        lwrn() << "SQL server unavailable: not fetching mail messages until it is available.";
    else if(available && sql_server_unavailable_)
        lout() << "SQL server available again: fetching mail messages.";

    sql_server_unavailable_ = !available;

    return available;
}

/** Generate the mail server URL string.
 *
 *  The mail server URL string is generated from the static member fields that are set as a
 *  result of parsing the configuration file and/or command-line arguments during application
 *  initialization.
 *
 *  @return         The generated mail server URL string.
 */
string XFloatMailRetrievalService::GenerateMailServerUrlString() const {

    using vmime::utility::urlUtils;

    ostringstream oss;

    // Percent-encode the credentials so characters such as '@', ':' or '#' do not break parsing.
    oss << mail_server_protocol_
        << "://"
        << urlUtils::encode(mail_server_user_)
        << ":"
        << urlUtils::encode(mail_server_password_)
        << "@"
        << mail_server_host_;

    return oss.str();
}

/** List the paths raw mail files on the local filesystem.
 *
 *  @throw          util::log::MessageException if an error occurs while listing the raw mail
 *                  paths.
 *
 *  @return         A vector containing the paths to the raw mail files on the local filesystem.
 */
vector<path> XFloatMailRetrievalService::ListRawMailFiles() {

    static vector<path> files;
    static vector<path> paths;

    // Init vectors
    files.reserve(max_num_msgs_to_fetch_+1);
    files.resize(0);
    paths.reserve(max_num_msgs_to_fetch_+1);
    paths.resize(0);


    try {

        // List all files in the raw mail file input directory.
        files = ListFiles(mail_file_input_directory_);

        // Copy paths to files matching the regex.
        copy_if(files.begin(), files.end(), back_inserter(paths), [](const path & p) {
            return regex_match(p.string(), regex(".*/.*\\.(raw|txt)", regex::extended));
        });

        // Read the files in name order, so that files named in transmission order (e.g. by
        // MOMSN) are processed in the order the messages were sent, as from the mail server.
        sort(paths.begin(), paths.end());

        // Apply the fetch limit after filtering, so lock and error files do not use it up.
        if(max_num_msgs_to_fetch_ > 0 && paths.size() > max_num_msgs_to_fetch_)
            paths.resize(max_num_msgs_to_fetch_);

    }CATCH_BLOCK("Failed to get paths to raw mail input files!")

    return paths;
}

/** Get the mail session object.
 *
 *  If the mail session does not exist, a new one will be created.
 *
 *  @return         A shared reference to the VMime session object, @ref mail_session_.
 */
const vmime::shared_ptr<vmime::net::session>& XFloatMailRetrievalService::mail_session() {
    return mail_session_ ? mail_session_ : mail_session_ = vmime::net::session::create();
}

/** Open the inbox folder on the remote mail server.
 *
 *  If the inbox folder is not already open, then this function will attempt to open it. The
 *  path to the inbox folder is specified by the static member string @ref
 *  mail_server_inbox_folder_path_, which is set as a result of parsing the configuration file
 *  and/or command-line arguments during application initialization. If the string @ref
 *  mail_server_inbox_folder_path_ is empty, then the default mail folder will be requested
 *  from the mail server.
 *
 *  @exception      util::log::MessageException of an error occurs while attempting to open
 *                  the inbox folder.
 *
 *  @return         A shared reference to the VMime folder object, @ref inbox_folder_.
 */
const vmime::shared_ptr<vmime::net::folder>& XFloatMailRetrievalService::OpenMailInboxFolder() {

    if(!inbox_folder_ || !inbox_folder_->isOpen())

        try {

            if(mail_server_inbox_folder_path_.empty()) {

                // If path is not set, get default folder.
                dbg1() << "Opening default mail folder...";
                inbox_folder_ = ConnectToMailServer()->getDefaultFolder();

            } else {

                // Otherwise, get folder specified by path.
                dbg1() << "Opening mail folder: " << mail_server_inbox_folder_path_ << "...";
                inbox_folder_ = ConnectToMailServer()->getFolder(
                        vmime::net::folder::path(mail_server_inbox_folder_path_));

            }

            // Open inbox folder.
            inbox_folder_->open(vmime::net::folder::MODE_READ_WRITE, true);

        }CATCH_BLOCK("Failed to get inbox folder!")

    return inbox_folder_;
}

/** The constructor.
 *
 *  @param[in]      prop        TRUE if propagation of mail messages to the mail processing
 *                              thread is enabled, or FALSE otherwise.
 */
XFloatMailRetrievalService::XFloatMailRetrievalService(bool prop) :
                processing_thread_propagation_enabled_(prop) {
    /* Do nothing. */
}

/** The destructor.
 *
 */
XFloatMailRetrievalService::~XFloatMailRetrievalService() {
    /* Do nothing. */
}

/** Functor operator overload function: the main entry point for the mail retrieval thread.
 *
 *  This function defines the behavior of the mail retrieval thread, which is responsible for
 *  retrieving messages from the mail server. After initialization, a single instance of this
 *  thread will be instantiated. The static configuration options for this thread are set as a
 *  result of parsing the command-line arguments and/or configuration file.
 *
 *  First, the scope thread attribute "ThreadName" is set to "mail-fetch". This attribute is
 *  used by the global logger object for filtering, etc. Next, the instruments whose messages are
 *  to be reprocessed are determined: all cached instruments if @ref is_reprocess_all_ is TRUE;
 *  otherwise, those listed in @ref reprocess_by_inst_ids_ and those with PHY generation enabled,
 *  plus, if @c is_reprocess_active_ is TRUE, those that reported in the last 90 days.
 *
 *  For each instrument to reprocess, its existing dive data is deleted from the SQL database,
 *  its stored mail messages are fetched from the SQL database and passed on to the mail
 *  processing thread. Then, the thread will sleep for a short duration before proceeding to
 *  allow time for the daemon thread to signal termination if the application is configured to
 *  terminate after reprocessing.
 *
 *  Then the thread enters its primary loop, which depends on @ref mail_fetch_mode_. In
 *  @c MAIL_FETCH_SERVER mode, @ref FetchMailMessagesFromServer() fetches the next batch of
 *  messages from the mail server, by message index starting at @ref message_start_index_. In
 *  @c MAIL_FETCH_FILESYSTEM mode, @ref FetchMailMessagesFromFileSystem() reads RAW mail files
 *  from @c mail_file_input_directory_. The fetched messages are queued to be processed by the
 *  next thread in the pipeline, @ref XFloatMailProcessingService. No messages are fetched while
 *  the SQL server is unavailable (@ref SqlServerAvailableForFetching()). If no new messages are
 *  fetched, then the connections to the mail server and SQL database are closed and the thread
 *  sleeps for @ref sleep_seconds_if_no_new_msgs_ seconds.
 *
 *  The primary loop terminates once termination is signaled (i.e. @ref termination_signaled_
 *  is TRUE) and the thread will disconnect from the mail server and then die. If termination
 *  is signaled before the thread begins its first iteration, then exactly zero iteration of
 *  the loop will be executed.
 *
 *  @exception          None. Never throws.
 */
void XFloatMailRetrievalService::operator()() ATTR_NO_EXCEPTIONS {

    namespace attr = blg::attributes;

    shared_ptr<XFloat> xfloat;
    vector<shared_ptr<XFloat> > floats;
    size_t num_messages_fetched;
    ostringstream os_where;
    unsigned int sleep_sec_count;
    vector<shared_ptr<XFloatMailMessage>> xmsgs;

    // Set scoped thread log attribute.
    BOOST_LOG_SCOPED_THREAD_ATTR("ThreadName", attr::constant<string>("mail-fetch"));

    ldbg() << "Thread started.";

    // When moving messages out of the inbox, move each only once it has been processed.
    XFloatMailProcessingService::set_track_processed_msg_uids(
            move_mail_messages_ && mail_fetch_mode_ == MAIL_FETCH_SERVER);

    // Check if any data needs to be reprocessed. Without the SQL database, there is none.
    if(!is_sql_enabled()) {

        // Nothing to reprocess.

    } else if(is_reprocess_all_) {

        // Reprocess all instrument data.
        for(auto it = (floats = XFloat::GetAllCachedFloats()).begin(); it != floats.end(); ++it)
            reprocess_by_inst_ids_.push_back((*it)->id_instrument());

    } else {

        // Reprocess data for specified instruments.
        reprocess_by_inst_ids_.insert(
                reprocess_by_inst_ids_.end(),
                XFloat::instrument_ids_to_enable_phy_generation().begin(),
                XFloat::instrument_ids_to_enable_phy_generation().end());

        // TODO Reprocess active floats.
        if(is_reprocess_active_) {

          lout() << "Reprocessing active floats...";


          // Get list of floats that have reported in the last 90 days. Only the instrument
          // identifiers are selected; selecting whole messages took several minutes.
          ldbg() << "Selecting floats reporting in the last 90 days...";
          vector<unsigned int> active_ids = XFloatMailMessage::SelectInstrumentIdsFromDatabase(
              "msg_is_valid = 1 AND iridium_position_datetime >= (CURDATE() - INTERVAL 90 day)");
          lout() << "Found " << active_ids.size() << " floats reporting in the last 90 days.";

          // Add float identifiers to list of floats to reprocess.
          reprocess_by_inst_ids_.insert(
              reprocess_by_inst_ids_.end(), active_ids.begin(), active_ids.end());

        }

    }

    if(reprocess_by_inst_ids_.size() > 0) {

        // Reprocess messages for selected instruments.
        for(auto it = reprocess_by_inst_ids_.begin();
                it != reprocess_by_inst_ids_.end() && !termination_signaled_; ++it)

            try {

                static const format fmt1("Reprocessing messages for instrument #%04d...");
                lout() << format(fmt1) % *it;

                // Delete existing dive data from database.
                XFloatDiveData::DeleteFromDatabase(*it);

                // Get the instrument object.
                xfloat = XFloat::GetFloat(*it);

                // Create criteria for fetching mail messages.
                os_where.clear();
                os_where.str("");
                os_where << "id_instrument = " + lexical_cast<string>(*it);
                if(!xfloat->start_of_mission_datetime().is_not_a_date_time())

                    // Ignore messages transmitted before start-of-mission.
                    os_where << " AND iridium_position_datetime > '"
                            + util::date::DateUtils::GenerateIso8601String(
                                    xfloat->start_of_mission_datetime(),
                                    true) + "'";

                // Fetch messages from database.
                xmsgs = XFloatMailMessage::SelectObjectsFromDatabase(
                        os_where.str(),
                        "",
                        "id_sbd_momsn");
                os_where.clear();
                os_where.str("");
                static const format fmt2("Fetched %d existing message(s) for instrument #%04d "
                        "to reprocess.");
                linf() << format(fmt2) % xmsgs.size() % *it;

                // Push messages to processing queue. On termination, stop; the messages stay in
                // the database and are reprocessed when the float is next reprocessed.
                for(auto msg_it = xmsgs.begin(); msg_it != xmsgs.end(); ++msg_it)
                    if(!PushForProcessing(*msg_it))
                        break;

                // Reset instrument pointer.
                xfloat.reset();

            }CATCH_NOTHROW("Failed to reprocess messages for instrument " << *it << "!")

        /*  Sleep for a short duration to allow time for the daemon thread to signal
         *  termination if the application is not configured to run as a daemon/service.
         */
        for(unsigned int n = 30000 / XFLT_THREAD_SLEEP_MS; n-- && !termination_signaled_; )
            sleep(millisec(XFLT_THREAD_SLEEP_MS));
    }

    // Enter the primary thread loop.
    switch (mail_fetch_mode_) {

      case MAIL_FETCH_SERVER:

        // Fetch messages from mail server until termination.
        while(!termination_signaled_)
          try {

              // Fetch new messages, unless the SQL server is unavailable.
              num_messages_fetched = 0;
              if(SqlServerAvailableForFetching())
                  try {

                      num_messages_fetched = FetchMailMessagesFromServer();

                  }CATCH_NOTHROW("Failed to fetch messages from mail server!")

              // Check if any messages were fetched.
              if(num_messages_fetched)

                  // Fetch more messages.
                  continue;

              try {

                  // Close mailserver connection.
                  DisconnectFromMailServer();

              }CATCH_NOTHROW("Failed to disconnect from the mail server!")

              try {

                  // Close SQL server connection
                  CloseSqlConnection();

              }CATCH_NOTHROW("Failed to disconnect from SQL server!")

              // If no new messages are fetched, sleep.
              sleep_sec_count = sleep_seconds_if_no_new_msgs_ * (1000 / XFLT_THREAD_SLEEP_MS);
              while(sleep_sec_count-- && !termination_signaled_)
                  sleep(millisec(XFLT_THREAD_SLEEP_MS));

          }CATCH_NOTHROW("Exception caught while retrieving messages from the mail server!")

        break;

      case MAIL_FETCH_FILESYSTEM:

        // TODO Fetch messages from filesytem until termination.
        while(!termination_signaled_)
          try {

              // Fetch new messages, unless the SQL server is unavailable.
              num_messages_fetched = 0;
              if(SqlServerAvailableForFetching())
                  try {

                     num_messages_fetched = FetchMailMessagesFromFileSystem();

                  }CATCH_NOTHROW("Failed to fetch messages from mail server!")

              // Check if any messages were fetched.
              if(num_messages_fetched)

                  // Fetch more messages.
                  continue;

              // Every input file has been queued once none is left (or the directory cannot be
              // listed), so a one-shot run can exit.
              if(!input_exhausted_) {

                  bool none_left = true;
                  try {
                      none_left = ListRawMailFiles().empty();
                  } catch(...) {
                      /* Do nothing. */
                  }

                  if(none_left) {
                      lout() << "All raw mail input files read from: "
                             << mail_file_input_directory_.string();
                      input_exhausted_ = true;
                  }
              }

              try {

                  // Close SQL server connection
                  CloseSqlConnection();

              }CATCH_NOTHROW("Failed to disconnect from SQL server!")

              // If no new messages are fetched, sleep.
              sleep_sec_count = sleep_seconds_if_no_new_msgs_ * (1000 / XFLT_THREAD_SLEEP_MS);
              while(sleep_sec_count-- && !termination_signaled_)
                  sleep(millisec(XFLT_THREAD_SLEEP_MS));

          }CATCH_NOTHROW("Exception caught while retrieving messages from the mail server!")

        break;

      default:

        // Do nothing until termination.
        while(!termination_signaled_)
          try {

            // Sleep until termination.
            sleep_sec_count = sleep_seconds_if_no_new_msgs_ * (1000 / XFLT_THREAD_SLEEP_MS);
            while(sleep_sec_count-- && !termination_signaled_)
                sleep(millisec(XFLT_THREAD_SLEEP_MS));

          } CATCH_NOTHROW("Exception caught while awaiting termination !")


        break;
    }

    // Terminate.
    ldbg() << "Termination signal received! Shutting down...";

        try {

            // Close mailserver connection.
            DisconnectFromMailServer();

        }CATCH_NOTHROW("Failed to disconnect from the mail server!")

        try {

            // Close SQL server connection
            CloseSqlConnection();

        }CATCH_NOTHROW("Failed to disconnect from SQL server!")

    return;
}

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */
