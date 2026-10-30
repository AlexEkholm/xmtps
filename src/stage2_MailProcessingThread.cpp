/** The @ref argo::iridium::xfloat::XFloatMailProcessingService class source file.
 *
 *  This is the source file for the @ref argo::iridium::xfloat::XFloatMailProcessingService class.
 *  This class defines the operation of the mail processing thread(s), which are the second stage
 *  in the processing pipeline. The mail processing thread consumes messages that were fetched
 *  from the mail server in the previous stage of the processing pipeline, the mail retrieval
 *  thread, @ref argo::iridium::xfloat::XFloatMailRetrievalService. The messages are validated,
 *  saved to the SQL database, queued for consumption in the next stage of the processing pipeline,
 *  the telemetry decoder thread, @ref argo::iridium::xfloat::XFloatTelemetryDecoderService, and
 *  their file representations are written to the local file system.
 *
 *  This file contains the static data members of the @ref
 *  argo::iridium::xfloat::XFloatMailProcessingService class.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 16, 2014 2:25:44 PM
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
#include "stage2_MailProcessingThread.h"

namespace argo {
namespace iridium {
namespace xfloat {

/* Static Variables: */

thread_group XFloatMailProcessingService::thread_group_;

vector<shared_ptr<XFloatMailProcessingService> > XFloatMailProcessingService::thread_objects_;

util::thread::SynchronousQueue<shared_ptr<XFloatMailMessage> > XFloatMailProcessingService::msg_processing_queue_(XFLT_QUEUE_SIZE_MAIL_PROCESSING);

unsigned int XFloatMailProcessingService::num_threads_;

std::atomic<bool> XFloatMailProcessingService::termination_signaled_(false);

vector<int> XFloatMailProcessingService::processed_msg_uids_;

std::mutex XFloatMailProcessingService::processed_msg_uids_mutex_;

std::atomic<bool> XFloatMailProcessingService::track_processed_msg_uids_(false);

/* Function Definitions: */

/** Pop a mail message out of the mail processing queue.
 *
 *  This function is used by the mail processing thread(s) to receive mail messages produced
 *  by the previous stage in the processing pipeline, the mail retrieval thread, @ref
 *  XFloatMailRetrievalService. The mail message processing queue is synchronous, so the
 *  underlying pop function is atomic.
 *
 *  @param[out]     msg             The destination of the mail message to pop out of the
 *                                  queue.
 *  @param[in]      block           If TRUE, this function will block until the queue
 *                                  is not empty. If FALSE, this function will return FALSE
 *                                  if the queue is empty (and no element will be popped
 *                                  out of the queue).
 *
 *  @return         TRUE if the element was successfully popped out of the queue, or FALSE
 *                  otherwise.
 */
bool XFloatMailProcessingService::MsgProcessingQueuePop(shared_ptr<XFloatMailMessage>& msg,
bool block) {
  return msg_processing_queue_.pop(msg, block);
}

/** Process a mail message.
 *
 *  This function is responsible for processing and validating a mail message before queuing
 *  the message for consumption by the next thread in the processing pipeline, @ref
 *  XFloatTelemetryDecoderService.
 *
 *  First, the function checks the message reference @c msg. If it is NULL, then the function
 *  throws an exception. Otherwise, it continues on to set the global logger attributes:
 *   - DiveID,
 *   - MsgUID,
 *   - PcktID,
 *   - MomsnID,
 *   - WhoiID,
 *   - DacID, and
 *   - WmoID.
 *
 *  If the values of the WhoiID, DacID, or WmoID attributes are unknown, the thread will
 *  attempt to query these identifiers from the SQL database (without the SQL database, the WhoiID
 *  is taken from a float with the same IMEI read in this run). These attributes are used by the
 *  global logger object for filtering, etc.
 *
 *  Next, the message processing begins. If the message header is invalid, then the message is
 *  discarded. Otherwise, the raw mail file and binary SBD data file are written to the local
 *  filesystem and the message, with the paths to those files, is saved to the SQL database.
 *  Finally, if the SBD data is valid, the message is queued for consumption by the next thread
 *  in the processing pipeline, @ref XFloatTelemetryDecoderService. A message is therefore only
 *  decoded once it has been saved.
 *
 *  @param[in]      msg         A shared reference to the mail message to process.
 *
 *  @throw          util::log::MessageException if an error occurs while processing the mail
 *                  message or the shared pointer to the mail message is NULL.
 */
void XFloatMailProcessingService::ProcessMailMsg(const shared_ptr<XFloatMailMessage>& msg) const {

  namespace attr = blg::attributes;
  using util::log::LogLevel_t;

  bool push = (XFloatTelemetryDecoderService::num_threads() > 0);

  try {

    // Check the pointer.
    if(!msg)
      EXC_THROW("Pointer to mail message object is null!");

    // Set the global logger attributes.
    BOOST_LOG_SCOPED_THREAD_ATTR("MsgUID", attr::constant<int>(msg->id_msg_uid()));

    // Check the message header.
    if(!msg->msg_header_is_valid()) {

      lwrn() << "Ignoring mail message with invalid header.";
      return;

    }

    // Set the scoped log attributes.
    BOOST_LOG_SCOPED_THREAD_ATTR("DiveID", attr::constant<int>(msg->id_dive()));
    BOOST_LOG_SCOPED_THREAD_ATTR("PcktID", attr::constant<int>(msg->id_sbd_packet()));
    BOOST_LOG_SCOPED_THREAD_ATTR("MomsnID", attr::constant<int>(msg->id_sbd_momsn()));

    // Check the WHOI ID. Without the SQL database, look for a float read in this run with the
    // same IMEI.
    if(msg->id_instrument() == XFLT_ID_UNKNOWN && !is_sql_enabled()) {

      for(const shared_ptr<XFloat>& x : XFloat::GetAllCachedFloats())
        if(x && x->id_imei() == msg->id_imei()) {
          msg->set_id_instrument(x->id_instrument());
          break;
        }

    } else if(msg->id_instrument() == XFLT_ID_UNKNOWN)

      try {

        // Query the WHOI ID from the database.
        ldbg() << "The instrument serial number is unknown. "
               << "Attempting to query the ID from the database...";

        msg->set_id_instrument(
            XFloat::SelectValueFromDatabase<int>(
                "id_instrument",
                "id_imei = " + lexical_cast<string>(msg->id_imei()),
                "",
                "id_instrument DESC"));

      }CATCH_BLOCK("Failed to query the unique instrument identifier from the database!")

    // Set the scoped log attribute.
    BOOST_LOG_SCOPED_THREAD_ATTR("WhoiID", attr::constant<int>(msg->id_instrument()));

    // Check that WHOI ID is in valid range.
    if(msg->id_instrument() > 11999 || msg->id_instrument() < 1000) {
      lwrn() << "Ignoring mail message with invalid serial number.";
      return;
    }

    // Check the DAC ID.
    if(msg->id_dac() == XFLT_ID_UNKNOWN)
      msg->set_id_dac(XFloat::GetFloat(msg->id_instrument())->id_dac());

    // Check the WMO ID.
    if(msg->id_wmo() == XFLT_ID_UNKNOWN)
      msg->set_id_wmo(XFloat::GetFloat(msg->id_instrument())->id_wmo());

    // Set the scoped log attributes.
    BOOST_LOG_SCOPED_THREAD_ATTR("DacID", attr::constant<int>(msg->id_dac()));
    BOOST_LOG_SCOPED_THREAD_ATTR("WmoID", attr::constant<int>(msg->id_wmo()));

    linf() << "Processing message...";

    // Write the files.
    msg->WriteFiles();

    // Persist the mail message to the SQL database, before it is decoded, so that a message is
    // only decoded once it is saved.
    msg->SaveObjectToDatabase();

    // Check if the message attachment is valid.
    if(!msg->sbd_data_is_valid())

      lwrn() << "Ignoring mail message with invalid binary SBD attachment.";

    else if(push)

      // Submit message to telemetry decoder queue.
      XFloatTelemetryDecoderService::MsgDecodingQueuePush(msg);

  }CATCH_BLOCK("Failed to process mail message!")

  return;
}

/** Process a mail message, unless the SQL server is unavailable.
 *
 *  Calls @ref ProcessMailMsg(). If that fails and the SQL server cannot be reached, the message
 *  has not been saved and must be kept: FALSE is returned and the caller retries the message.
 *  Any other failure is passed on, as before, and the message is dropped. A message that was
 *  processed or dropped is recorded as processed (see @ref TakeProcessedMsgUids()).
 *
 *  @param[in]      msg         A shared reference to the mail message to process.
 *
 *  @throw          util::log::MessageException if processing failed while the SQL server is
 *                  available.
 *
 *  @return         TRUE if the message was processed, FALSE if the SQL server is unavailable.
 */
bool XFloatMailProcessingService::TryProcessMailMsg(const shared_ptr<XFloatMailMessage>& msg) const {

  try {

    ProcessMailMsg(msg);

  } catch(...) {

    if(!isSqlServerAvailable())
      return false;

    MarkMsgProcessed(msg);
    throw;
  }

  MarkMsgProcessed(msg);
  return true;
}

/** Process a mail message, holding it while the SQL server is unavailable.
 *
 *  A message that cannot be saved because the SQL server is unavailable is not dropped: it is
 *  retried every @ref XFLT_SQL_RETRY_MS milliseconds until it is processed or termination is
 *  signaled. The start and end of an outage are logged once.
 *
 *  @param[in]      msg         A shared reference to the mail message to process.
 *
 *  @throw          util::log::MessageException if processing failed while the SQL server is
 *                  available.
 *
 *  @return         TRUE if the message was processed, FALSE if termination was signaled while
 *                  the SQL server was still unavailable.
 */
bool XFloatMailProcessingService::ProcessMailMsgWhenSqlAvailable(
    const shared_ptr<XFloatMailMessage>& msg) const {

  bool waiting = false;

  while(!TryProcessMailMsg(msg)) {

    if(!waiting) {
      lwrn() << "SQL server unavailable: holding mail messages and retrying every "
             << XFLT_SQL_RETRY_MS / 1000 << " s...";
      waiting = true;
    }

    CloseSqlConnection();
    for(unsigned int n = XFLT_SQL_RETRY_MS / XFLT_THREAD_SLEEP_MS; n-- && !termination_signaled_; )
      sleep(millisec(XFLT_THREAD_SLEEP_MS));

    if(termination_signaled_)
      return false;
  }

  if(waiting)
    lout() << "SQL server available again: mail message processing resumed.";

  return true;
}

/** Record a mail message as processed, if processed message UIDs are being tracked.
 *
 *  @param[in]      msg         A shared reference to the processed mail message.
 */
void XFloatMailProcessingService::MarkMsgProcessed(const shared_ptr<XFloatMailMessage>& msg) {

  if(!track_processed_msg_uids_ || !msg)
    return;

  std::lock_guard<std::mutex> guard(processed_msg_uids_mutex_);
  processed_msg_uids_.push_back(msg->id_msg_uid());
}

/** Set whether the UIDs of processed mail messages are recorded.
 *
 *  The mail retrieval thread enables this when it moves mail messages out of the inbox, so that
 *  it moves a message only once it has been processed.
 *
 *  @param[in]      track       TRUE to record the UIDs of processed mail messages.
 */
void XFloatMailProcessingService::set_track_processed_msg_uids(bool track) {
  track_processed_msg_uids_ = track;
}

/** Take the UIDs of the mail messages processed since the last call.
 *
 *  @return         The UIDs of the mail messages saved to the SQL database or rejected since the
 *                  previous call. The list is cleared.
 */
vector<int> XFloatMailProcessingService::TakeProcessedMsgUids() {

  vector<int> uids;

  std::lock_guard<std::mutex> guard(processed_msg_uids_mutex_);
  uids.swap(processed_msg_uids_);

  return uids;
}

/** Set the termination signaled flag.
 *
 *  This function is called by @ref TerminateWorkerThreads() in order to cause all currently
 *  running instances of the mail processing thread to exit their primary loop and die.
 *
 *  @param[in]      termination_signaled        The termination signaled value to set.
 */
void XFloatMailProcessingService::set_termination_signaled(bool termination_signaled) {
  termination_signaled_ = termination_signaled;
}

/** The constructor.
 *
 */
XFloatMailProcessingService::XFloatMailProcessingService() {
  /* Do nothing. */
}

/** The destructor.
 *
 */
XFloatMailProcessingService::~XFloatMailProcessingService() {
  /* Do nothing. */
}

/** Functor operator overload function: the main entry point for the mail processing thread(s).
 *
 *  This function defines the behavior of the mail processing thread(s), which are responsible
 *  for processing the messages fetched from the mail server by the mail retrieval thread,
 *  @ref XFloatMailRetrievalService. After initialization, zero or more instances of this
 *  thread are instantiated. The static configuration options for this thread are set as a
 *  result of parsing the command-line arguments and/or configuration file.
 *
 *  First, the scope thread attribute "ThreadName" is set to "mail-process". This attribute is
 *  used by the global logger object for filtering, etc. Then the thread enters its primary
 *  loop, in which it pops mail messages from the incoming queue one at a time and processes
 *  the messages before passing them to the next stage in the processing pipeline, @ref
 *  XFloatTelemetryDecoderService. If the incoming message queue is empty, then the connection
 *  to the SQL database is closed, and the thread sleeps for @ref XFLT_THREAD_SLEEP_MS
 *  milliseconds.
 *
 *  Once termination is signaled by the daemon process i.e. @ref termination_signaled_ is
 *  TRUE, the thread will exit its primary loop and die.
 *
 *  @exception      None. Never throws.
 */
void XFloatMailProcessingService::operator ()() ATTR_NO_EXCEPTIONS {

  namespace attr = blg::attributes;

  BOOST_LOG_SCOPED_THREAD_ATTR("ThreadName", attr::constant<std::string>("mail-process"));
  ldbg() << "Thread instantiated.";

  shared_ptr<XFloatMailMessage> msg;
  size_t num_unsaved = 0;
  bool sql_unavailable = false;

  // Enter the primary thread loop.
  do { // } while(!termination_signaled_);

    try {

      // Drop a message whose processing failed in the previous iteration.
      msg.reset();

      // Pop message from queue. Do not block.
      if(MsgProcessingQueuePop(msg, false)) {

        do {

          // Process the message, holding it while the SQL server is unavailable. On
          // termination the held message is retried once more below.
          if(!ProcessMailMsgWhenSqlAvailable(msg))
            break;

          // Reset the message pointer.
          msg.reset();

        } while(MsgProcessingQueuePop(msg, false));

      } else {

        // Close SQL connection if open, for now, and sleep.
        CloseSqlConnection();
        sleep(millisec(XFLT_THREAD_SLEEP_MS));
      }

    }CATCH_NOTHROW("Exception caught in mail processing thread!")

  } while(!termination_signaled_);

  // Terminate.
  ldbg() << "Termination signal received! Shutting down...";

  /* Drain the held message and the messages still queued, so none are lost on shutdown. Once
   * the SQL server is found unavailable, the rest are not tried (each try may wait for a
   * connection timeout); they have not been saved. */
  do {

    if(msg) {

      if(sql_unavailable)
        ++num_unsaved;
      else
        try {

          if(!TryProcessMailMsg(msg)) {
            sql_unavailable = true;
            ++num_unsaved;
          }

        }CATCH_NOTHROW("Exception caught while draining mail processing queue!")
    }

    msg.reset();

  } while(MsgProcessingQueuePop(msg, false));

  if(num_unsaved)
    lwrn() << num_unsaved << " mail message(s) not processed because the SQL server is "
           << "unavailable. Messages still on the mail server are fetched again at the next "
           << "start.";

  CloseSqlConnection();

  return;
}

/** Push a mail message into the mail processing queue.
 *
 *  This function is used by the previous stage in the processing pipeline, the mail retrieval
 *  thread, @ref XFloatMailRetrievalService, to pass a mail message to the mail processing
 *  thread(s). The mail message processing queue is synchronous, so the underlying push
 *  function is atomic.
 *
 *  @param[in]      msg             The mail message to push into the mail processing queue.
 *  @param[in]      block           If TRUE, this function will block until the queue
 *                                  is not full. If FALSE, this function will return FALSE
 *                                  if the queue is full (and the element will not be
 *                                  pushed into the queue).
 *
 *  @return         TRUE if the element was successfully pushed into the queue, or FALSE
 *                  otherwise.
 */
bool XFloatMailProcessingService::MsgProcessingQueuePush(const shared_ptr<XFloatMailMessage>& msg,
bool block) {
  return msg_processing_queue_.push(msg, block);
}

/** Push multiple mail messages into the mail processing queue.
 *
 *  This function is used by the previous stage in the processing pipeline, the mail retrieval
 *  thread, @ref XFloatMailRetrievalService, to pass a group of mail messages to the mail
 *  processing thread. The mail message processing queue is synchronous, so the underlying
 *  push function is atomic.
 *
 *  @param[in]      first,last      Input iterators to the initial and final positions in a
 *                                  range. The range used is @p [first,last), which includes
 *                                  all the elements between first and last, including the
 *                                  element pointed by first but not the element pointed by
 *                                  last.
 */
void XFloatMailProcessingService::MsgProcessingQueuePushAll(
    vector<shared_ptr<XFloatMailMessage>>::const_iterator first,
    vector<shared_ptr<XFloatMailMessage>>::const_iterator last) {
  msg_processing_queue_.push(first, last);
}

/** Get the number of mail message processing threads.
 *
 *  @return         The number of mail message processing threads.
 */
unsigned int XFloatMailProcessingService::num_threads() {
  return num_threads_;
}

/** Instantiate the mail processing worker thread(s).
 *
 *  This function is used by the daemon thread to instantiate zero or more mail processing
 *  threads after initialization. The number of threads that are instantiated by this function
 *  is determined by the value of @ref num_threads_, which is set as a result of parsing the
 *  command-line argument and/or configuration file during initialization.
 *
 *  Each thread is instantiated by creating an instance of this class, which is pushed into
 *  the @ref thread_objects_ vector, and invoking the functor of each object. Each thread
 *  that is created is added to the @ref thread_group_ object.
 *
 *  @exception          None. Never throws.
 */
void XFloatMailProcessingService::StartWorkerThreads() ATTR_NO_EXCEPTIONS {

  ldbg() << "Starting mail processing thread(s)...";

  for(unsigned int i = 0; i < num_threads_; i++) {

    shared_ptr<XFloatMailProcessingService> thread(make_shared<XFloatMailProcessingService>());
    thread_objects_.push_back(thread);
    thread_group_.create_thread(*thread);

  }
}

/** Terminate the mail processing worker thread(s).
 *
 *  This function is used by the daemon thread to kill the mail message processing worker
 *  thread(s) upon application termination. The function signals termination, and then
 *  attempts to join the thread group.
 *
 *  @warning            If any of the mail message processing threads fails to terminate
 *                      properly, then the @c join_all() function could block indefinitely.
 *                      Precautions have been take to ensure that this does not happen, but
 *                      it may still be  possible.
 *
 *  @exception          None. Never throws.
 */
void XFloatMailProcessingService::TerminateWorkerThreads() ATTR_NO_EXCEPTIONS {
  ldbg() << "Terminating mail processing thread(s)...";
  set_termination_signaled();
  if(num_threads_)
    thread_group_.join_all();
}

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */
