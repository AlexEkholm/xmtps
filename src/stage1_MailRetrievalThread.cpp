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

namespace argo {
namespace iridium {
namespace xfloat {

/* Static Variables: */

bool XFloatMailRetrievalService::is_reprocess_active_;

bool XFloatMailRetrievalService::is_reprocess_all_;

int XFloatMailRetrievalService::mail_fetch_mode_;

path XFloatMailRetrievalService::mail_file_input_directory_;

path XFloatMailRetrievalService::mail_file_processed_directory_;

string XFloatMailRetrievalService::mail_server_host_;

string XFloatMailRetrievalService::mail_server_inbox_folder_path_;

string XFloatMailRetrievalService::mail_server_password_;

string XFloatMailRetrievalService::mail_server_protocol_;

string XFloatMailRetrievalService::mail_server_user_;

size_t XFloatMailRetrievalService::max_num_msgs_to_fetch_;

unsigned int XFloatMailRetrievalService::message_start_index_(1);

bool XFloatMailRetrievalService::move_mail_messages_;

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


              // Remove mail files
              remove(mail_file_path);

              // Push message to the processing queue.
              XFloatMailProcessingService::MsgProcessingQueuePush(xmsg, true);
              is_processed = true;
              ++num_processed;

              // TODO Move raw files to "processed" folder ??


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
    using vmime::net::folderAttributes;
    using vmime::net::message;
    using vmime::net::messageSet;

    /* This set of attributes determines which parts of the messages are fetched from the mail
     * server.
     */
    static const fetchAttributes fattr(
            fetchAttributes::ENVELOPE | fetchAttributes::FULL_HEADER
                    | fetchAttributes::CONTENT_INFO | fetchAttributes::STRUCTURE
                    | fetchAttributes::UID);

    static vmime::shared_ptr<folder> folder_dest;
    static string folder_dest_name;
    static vmime::shared_ptr<folder> folder_inbox;
    static vmime::shared_ptr<message> msg;
    static size_t msg_count;
    static vector<vmime::shared_ptr<message>> msgs;
    static shared_ptr<XFloatMailMessage> xmsg;

    msgs.reserve(max_num_msgs_to_fetch_+1);

    // Initialize variables.
    folder_dest_name.clear();
    msg_count = 0;
    msgs.resize(0);
    folder_dest.reset();
    folder_inbox.reset();
    msg.reset();


    /* Message-destination-folder to UID-vector map.
     *
     * This map is used to keep track of the destination folder(s) on the mailserver to which
     * the messages will be move after having been fetched.
     */
    map<string, vector<message::uid> > folder2uid_map;

    bool is_moved;
    size_t num_not_moved = 0;

    try {

        ldbg() << "Checking server for mail messages...";

        // Open the inbox folder.
        if(!(folder_inbox = OpenMailInboxFolder()))
            EXC_THROW("Failed to open mail inbox folder!");

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
        for(auto it = msgs.begin(); it != msgs.end(); ++it) {

            is_moved = false;

            try {

                // Push message to the processing queue.
                XFloatMailProcessingService::MsgProcessingQueuePush(
                        xmsg = std::make_shared<XFloatMailMessage>(msg = *it),
                        true);

                if(move_mail_messages_) {

                    // Determine the appropriate destination folder and add UID to vector.
                    folder2uid_map[util::date::DateUtils::GenerateIso8601String(
                            xmsg->msg_header_datetime(),
                            false).substr(0, 6)].push_back(msg->getUID());
                    is_moved = true;
                }

            }CATCH_NOTHROW("Failed to process fetched message!")

            /* Skip a failed message rather than aborting the batch, which would cause the whole
             * batch to be re-fetched and re-queued forever. */
            if(!is_moved)
                ++num_not_moved;
        }

        // Reset pointers.
        msg.reset();
        xmsg.reset();

        if(!move_mail_messages_) {

            // Increment the messages start index.
            message_start_index_ = std::min(
                    message_start_index_ + max_num_msgs_to_fetch_,
                    msg_count + 1);

            // Reset pointer.
            folder_inbox.reset();

            // Return number of messages fetched.
            return msgs.size();

        }

        // Move message(s) to the appropriate folder(s).
        for(auto it = folder2uid_map.begin(); it != folder2uid_map.end(); ++it)

            try {

                // Get the destination folder.
                if(!(folder_dest = folder_inbox->getFolder(
                        folder::path::component(folder_dest_name = it->first))))

                    EXC_THROW("Failed to get destination folder: " + folder_dest_name);

                // Check if the destination folder exists.
                if(!folder_dest->exists())

                    try {

                        ldbg() << "Creating folder: " << folder_dest_name;

                        // Create the folder.
                        folder_dest->create(folderAttributes());

                    }CATCH_BLOCK("Failed to create mail folder: " + folder_dest_name)


                try {

                    // Copy the message(s) to the destination folder.
                    ldbg() << "Copying "
                              << it->second.size()
                              << " message(s) to folder: "
                              << folder_dest_name;
                    folder_inbox->copyMessages(
                            folder_dest->getFullPath(),
                            messageSet::byUID(it->second));

                }CATCH_BLOCK("Failed to copy message(s) to folder: " + folder_dest_name)

                try {

                    // Delete the message(s) from the source folder.
                    ldbg() << "Deleting " << it->second.size() << " message(s) from inbox";
                    folder_inbox->deleteMessages(messageSet::byUID(it->second));
                    folder_inbox->expunge();

                }CATCH_BLOCK("Failed to delete message(s) from the source folder!")

                // Reset pointer.
                folder_dest.reset();

            }CATCH_BLOCK("Failed to move mail message(s)!")

        /* Messages that could not be moved remain at the start of the fetched range after the
         * expunge, so step past them to avoid re-fetching them on every iteration. */
        message_start_index_ += num_not_moved;

        // Reset pointer.
        folder_inbox.reset();

        // Return number of messages fetched.
        return msgs.size();

    }CATCH_BLOCK("Failed to fetch mail messages from server!")

    return 0;

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

    ostringstream oss;

    oss << mail_server_protocol_
        << "://"
        << mail_server_user_
        << ":"
        << mail_server_password_
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
        vector<path> files(ListFiles(mail_file_input_directory_, max_num_msgs_to_fetch_));

        // Copy paths to files matching the regex.
        copy_if(files.begin(), files.end(), back_inserter(paths), [](const path & p) {
            return regex_match(p.string(), regex(".*/.*\\.(raw|txt)", regex::extended));
        });

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
 *  used by the global logger object for filtering, etc. Next, the minimum UID of the mail
 *  messages to be fetched from the server is determined. If @ref is_reprocess_all_ is TRUE,
 *  the messages will be fetched starting with UID = 0. Otherwise, if @ref is_reprocess_all_
 *  is TRUE, then the maximum mail message UID is queried from the SQL database and incremented
 *  by one to determine the minimum UID of the messages to fetch.
 *
 *  If the user has specified one or more unique instrument identifiers of mail message to
 *  reprocess, then those message will first fetched from the SQL database and passed on to the
 *  mail processing thread. Then, the thread will sleep for a short duration before proceeding
 *  to allow time for the daemon thread to signal termination if the application is configured
 *  to terminate after reprocessing.
 *
 *  Then the thread enters its primary loop. @ref FetchMailMessages() is called to fetch the
 *  next batch of mail messages from either (1) the SQL database or (2) the mail server if
 *  the messages do not exist in the database. The fetched messages are queued to be processed
 *  by the next thread in the pipeline, @ref XFloatMailProcessingService. If no new messages
 *  are fetched, then connections to the mail server and SQL database are closed and the
 *  thread sleeps for @ref sleep_seconds_if_no_new_msgs_ seconds.
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

    // Check if any data needs to be reprocessed.
    if(is_reprocess_all_) {

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


          // Get list of floats that have reported in the last 180 days.
          ldbg() << "Selecting floats reporting in the last 180 days...";
          xmsgs = XFloatMailMessage::SelectObjectsFromDatabase(
              "msg_is_valid = 1 AND iridium_position_datetime >= (CURDATE() - INTERVAL 90 day)",
              "id_instrument",
              "id_instrument");
          ldbg() << "Found " << xmsgs.size() << " floats reporting in the last 90 days.";


          // Add float identifiers to list of floats to reprocess.
          for(auto it = xmsgs.begin(); it < xmsgs.end(); ++it)
            reprocess_by_inst_ids_.insert(reprocess_by_inst_ids_.end(), (*it)->id_instrument());

        }

    }

    if(reprocess_by_inst_ids_.size() > 0) {

        // Reprocess messages for selected instruments.
        for(auto it = reprocess_by_inst_ids_.begin(); it != reprocess_by_inst_ids_.end(); ++it)

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

                // Push messages to processing queue.
                XFloatMailProcessingService::MsgProcessingQueuePushAll(xmsgs.begin(), xmsgs.end());

                // Reset instrument pointer.
                xfloat.reset();

            }CATCH_NOTHROW("Failed to reprocess messages for instrument " << *it << "!")

        /*  Sleep for a short duration to allow time for the daemon thread to signal
         *  termination if the application is not configured to run as a daemon/service.
         */
        sleep(millisec(30000));
    }

    // Enter the primary thread loop.
    switch (mail_fetch_mode_) {

      case MAIL_FETCH_SERVER:

        // Fetch messages from mail server until termination.
        while(!termination_signaled_)
          try {

              // Fetch new messages.
              num_messages_fetched = 0;
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

              // Fetch new messages.
              num_messages_fetched = 0;
              try {

                 num_messages_fetched = FetchMailMessagesFromFileSystem();

              }CATCH_NOTHROW("Failed to fetch messages from mail server!")

              // Check if any messages were fetched.
              if(num_messages_fetched)

                  // Fetch more messages.
                  continue;

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
