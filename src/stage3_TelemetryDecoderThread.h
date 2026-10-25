/** The @ref argo::iridium::xfloat::XFloatTelemetryDecoderService class header file.
 *
 *  This is the header file for the @ref argo::iridium::xfloat::XFloatTelemetryDecoderService class.
 *  This class defines the operation of the telemetry decoder thread(s), which are the third stage
 *  in the processing pipeline. The telemetry decoder thread(s) consume mail messages that were
 *  processed by the previous stage of the processing pipeline, the mail processing thread(s),
 *  @ref argo::iridium::xfloat::XFloatMailProcessingService. The processed mail messages are
 *  assigned to their corresponding dive data object. Then, the Iridium Short Burst Data (SBD)
 *  messages, that are supplied as attachments to the mail messages are decoded. The decoded data
 *  is contained in an instance of a class that inherits from @ref util::xmsg::XMessage. The
 *  resultant object is also assigned to the dive data object.
 *
 *  This file contains the class definition and the majority of the functionality of the class.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 16, 2014 2:23:02 PM
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

#ifndef XFLOATTELEMETRYDECODERSERVICE_H_
#define XFLOATTELEMETRYDECODERSERVICE_H_

/* Includes: */

// local
//#include "stage4a_NCFileWriterThread.h"
#include "stage4b_PostProcessingThread.h"

/* Definitions: */

/** Set to true to print additional debug output for telemetry decoding. */
#define DBG_X_DECODE 0

namespace argo {
namespace iridium {
namespace xfloat {

/* Class Definition: */

/** The telemetry decoder thread class.
 *
 *  This class defines the operation of the telemetry decoder thread(s), which are the third stage
 *  in the processing pipeline. The telemetry decoder thread(s) consumes processed mail messages
 *  that are produced by  the previous stage of the processing pipeline, the mail processing
 *  thread, @ref XFloatMailProcessingService.
 *
 *  For each message received from the mail processing thread, the corresponding instrument object,
 *  @ref XFloat, is retrieved. Likewise, the dive data object, @ref XFloatDiveData, corresponding
 *  to the dive or cycle number of the mail message is retrieved from the instrument object. The
 *  mail message is assigned to the dive data object. Finally, the SBD message data attached to
 *  the mail message is decoded into one of the @ref util::xmsg::XMessage which is determined by
 *  the X-Message type identifier contained in the binary message data. The resulting @ref
 *  util::xmsg::XMessage object is assigned to the @ref XFloatDiveData.
 *
 *  Zero or more instance of this thread may be instantiated after application initialization,
 *  depending on the configuration options. Each thread enters a primary loop in which messages
 *  are fetched from a common queue and processed. If the incoming queue is empty, then the thread
 *  will sleep for intervals of @ref XFLT_THREAD_SLEEP_MS milliseconds.
 *
 *  The primary loop will  continue until the daemon thread signals termination. When termination
 *  is signaled, the remaining messages in the incoming queue will be processed, then all instances
 *  of this thread will die. This ensures that processing pipeline will be thoroughly flushed
 *  before application termination, and that no messages will be partially or incompletely
 *  processed.
 */
class XFloatTelemetryDecoderService:
        public util::mysql::MysqlUtils {

    /* Friends Functions: */

    /** Friend function for populating some of the static members of this class from the
     *  configuration file options.
     */
    friend options_description
    util::config::LocalConfiguration::GenerateConfigurationFileOptionsDescription();

private:

    /* Static Data Members: */

    /** The telemetry decoder thread group.
     *
     *  This object represents the group of telemetry decoder threads that are instantiated after
     *  initialization. Each new thread that is instantiated is added to this group. Upon
     *  termination, the thread group is joined.
     */
    static thread_group thread_group_;

    /** The telemetry decoder thread objects.
     *
     *  Each instance of the telemetry decoder threads is created by invoking the functor of an
     *  instance of this runnable class. Each instance of this object that is created during
     *  instantiation of a new thread is stored in this vector.
     */
    static vector<shared_ptr<XFloatTelemetryDecoderService> > thread_objects_;

    /** The telemetry decode queue.
     *
     *  This synchronous FIFO queue is used to pass messages produced by the previous stage of the
     *  processing pipeline, the mail processing thread(s), @ref XFloatMailRetrievalService, to the
     *  telemetry decoder thread(s). All the operations performed by on the queue are atomic.
     *  Depending on the number of telemetry decoder threads, this queue may function as a
     *  one-to-one queue  or one-to-many queue.
     */
    static util::thread::SynchronousQueue<shared_ptr<XFloatMailMessage> > msg_decoding_queue_;

    /** The number of telemetry decoder threads.
     *
     *  The value of this field determines the number of telemetry decoder threads that are
     *  instantiated after initialization. Its value is set as a result of parsing the
     *  configuration file and/or command-line arguments during initialization. If the value is
     *  unspecified, then it defaults to a value of @ref CFG_DEFAULT_NDECO.
     */
    static unsigned int num_threads_;

    /** The termination signaled flag.
     *
     *  This flag is used  to signal termination of the telemetry decoder thread(s). During normal
     *  operation, the value of this field will be FALSE. Once termination is signaled, it will
     *  become TRUE causing all of the instances of the telemetry decoder thread to process the
     *  remaining messages in the incoming queue, exiting their primary loop, and then die.
     */
    static std::atomic<bool> termination_signaled_;

private:

    /* Private Member Functions: */

    /** Pop a mail message out of the telemetry decoder queue.
     *
     *  This function is used by the telemetry decoder thread(s) to receive mail messages produced
     *  by the previous stage in the processing pipeline, the mail processing thread, @ref
     *  XFloatMailProcessingService. The queue is synchronous, so the underlying pop function is
     *  atomic.
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
    static bool MsgProcessingQueuePop(shared_ptr<XFloatMailMessage>& msg,
    bool block = false) {
        return msg_decoding_queue_.pop(msg, block);
    }

    /** Set the termination signaled flag.
     *
     *  This function is called by @ref TerminateWorkerThreads() in order to cause all currently
     *  running instances of the telemetry decoder thread to exit their primary loop and die.
     *
     *  @param[in]      termination_signaled        The termination signaled value to set.
     */
    static void set_termination_signaled(bool termination_signaled = true) {
        termination_signaled_ = termination_signaled;
    }

public:

    /* Public Member Functions: */

    /** The constructor.
     *
     */
    XFloatTelemetryDecoderService() {
        /* Do nothing. */
    }

    /** The destructor.
     *
     */
    ~XFloatTelemetryDecoderService() {
        /* Do nothing. */
    }

    /** Process a dive data object
     *
     *  @param[in]  dive_data   The dive data object to process.
     *
     *  This function is invoked each time a new message for this dive is decoded. The function
     *  first checks if the dive data object is complete and proceeds accordingly.
     *
     *  If the dive data object is complete, then PHY and listing files are written, the object is
     *  saved to the database, the KML-document-is-updated flag is set to false, the dive data
     *  object is pushed into the NetCDF file writing queue, and the parent instrument object is
     *  pushed into the float processing queue.
     *
     *  If the dive data object is incomplete, then the listing file is written, the dive data
     *  object is saved to the database, and the parent instrument is pushed into the float
     *  processing queue.
     */
    void ProcessDiveData(shared_ptr<XFloatDiveData>& dive_data) const {

        namespace attr = blg::attributes;

        bool dive_complete = false;

        try {

            if(!dive_data)
                EXC_THROW("Pointer to dive data object is NULL!");

            // Set scoped log attributes.
            BOOST_LOG_SCOPED_THREAD_ATTR("WhoiID", attr::constant<int>(dive_data->id_instrument()));
            BOOST_LOG_SCOPED_THREAD_ATTR("DacID", attr::constant<int>(dive_data->id_dac()));
            BOOST_LOG_SCOPED_THREAD_ATTR("WmoID", attr::constant<int>(dive_data->id_wmo()));
            BOOST_LOG_SCOPED_THREAD_ATTR("DiveID", attr::constant<int>(dive_data->id_dive()));

            ldbg() << "Processing dive data object...";

            try {

                ldbg() << "Checking dive data completeness...";
                dive_complete = dive_data->CheckCompleteness();

            }CATCH_BLOCK("Failed to check dive data completeness!")

            // Check if the dive is complete.
            if(dive_complete) {

                lout() << "Dive complete.";

                // Get the parent instrument object.
                shared_ptr<XFloat> parent_instrument(dive_data->parent_instrument());

                // Write file and save.
                dive_data->WritePhyFile();
                dive_data->WriteListingFile();
                dive_data->SaveObjectToDatabase();

#               if defined(XMTPS_NC_FILE_WRITER_USES_QUEUE) && XMTPS_NC_FILE_WRITER_USES_QUEUE

                // Push the dive data object into the NC file writer queue.
                XFloatNCFileWriterThread::NcFileProcessingQueuePush(dive_data);

#               else

                // Update the NetCDF file status flag.
                dive_data->set_netcdf_file_updated(false);

#               endif // defined(XMTPS_NC_FILE_WRITER_USES_QUEUE) && XMTPS_NC_FILE_WRITER_USES_QUEUE


#               if defined(XMTPS_POST_PROC_USES_QUEUE) && XMTPS_POST_PROC_USES_QUEUE

                // Update KML status flag.
                parent_instrument->set_kml_document_updated(false);

                // Push the instrument object into the post-processing queue.
                XFloatPostProcessingService::FloatProcessingQueuePush(parent_instrument);

#               else

                // Update status flags.
                parent_instrument->set_kml_document_updated(false);
                parent_instrument->set_kml_file_updated(false);
                parent_instrument->set_status_file_updated(false);

#               endif // defined(XMTPS_POST_PROC_USES_QUEUE) && XMTPS_POST_PROC_USES_QUEUE

                return;

            } else {

                // Update the listing file and save after each new message.
                dive_data->WriteListingFile();
                dive_data->SaveObjectToDatabase();

#               if defined(XMTPS_POST_PROC_USES_QUEUE) && XMTPS_POST_PROC_USES_QUEUE

                // Push the instrument object into the post-processing queue.
                XFloatPostProcessingService::FloatProcessingQueuePush(
                        dive_data->parent_instrument());

#               else

                // Update status flags.
                dive_data->parent_instrument()->set_status_file_updated(false);

#               endif // defined(XMTPS_POST_PROC_USES_QUEUE) && XMTPS_POST_PROC_USES_QUEUE

            }

        }CATCH_BLOCK("Failed to process dive data object!")
    }

    /** Decode the telemetry data from a processed mail message.
     *
     *  This function is used to decode the binary Iridium Short Burst Data (SBD) message that is
     *  supplied as an attachment to the email message. Mail messages that are received from the
     *  previous stage in the processing pipeline, the mail processing thread(s), @ref
     *  XFloatMailProcessingService, are decoded using this function.
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
     *  These attributes are used by the global logger object for filtering, etc. The values of the
     *  attributes are assumed to be known since, if any were unknown, then the mail processing
     *  threads would have queried them from the SQL database.
     *
     *  Next, the float object, @c xfloat, that corresponds to the instrument that transmitted
     *  this message is retrieved. If the float object does not already exist, it will be created.
     *  Also, the metadata object for this instrument is retrieved, which contains information
     *  about the start-of-mission and deployment date/times. The time of flight of the message
     *  is examined to ensure that the message is valid. Messages that were transmitted after the
     *  start-of-mission or deployment date/times are discarded.
     *
     *  Then, the dive data object, @c xdive, which contains all the data received for a single
     *  instrument and single dive/cycle is retrieved from the float object. If the dive data
     *  object for this dive/cycle does not already exist, it will be created. The mail message,
     *  @c msg, is then assigned to the dive data object.
     *
     *  The SBD message is passed to the @ref ProcessDiveData() function to be decoded.
     *
     *  @param[in]       msg             A shared reference to the mail message to decode.
     */
    void DecodeMessage(shared_ptr<XFloatMailMessage> msg) const {

        namespace attr = blg::attributes;

        shared_ptr<XFloat> xfloat;
        shared_ptr<XFloatDiveData> xdive;
        shared_ptr<vector<uint8_t> > xmsg_id_types;

        int msg_id_dive = XFLT_DIVE_UNKNOWN;

        try {

            /* Decode the mail message: */

            // Check the message pointer.
            if(!msg)
                EXC_THROW("Null pointer to mail message object!");

            // Get the corresponding instrument and dive data objects
            xdive = (xfloat = XFloat::GetFloat(msg->id_instrument()))->GetDiveData(msg_id_dive = msg->id_dive());

            // Decode the mail message data.
            if(xdive->DecodeMailMessageData(msg))

                // Ignore invalid messages.
                return;

        }CATCH_BLOCK("Failed to decode telemetry data from mail message object!")

        try {

            /* Do message-type specific pre-processing: */

            if((xmsg_id_types = msg->xmsg_id_types()))

                // Iterate over the message type identifiers.
                for(auto it = xmsg_id_types->begin(); it < xmsg_id_types->end(); ++it)

                    switch(*it) {

                    case x::XMSG_TYPE_ID_ARGO_DATA:

                        /* Do type-specific pre-processing for Argo Data messages: */

                        if(!xdive->is_surface_pres_valid())

                            // Update the derived surface data fields.
                            xdive->UpdateDerivedSurfaceDataFields();

                        // Compute the pressure offset.
                        xdive->ComputePressureOffsetFromSurfaceTimeSeries(3);

                        break;

                    case x::XMSG_TYPE_ID_MISS_CONF0:
                    case x::XMSG_TYPE_ID_MISS_CONF1:

                        /* The mission configuration gives the sensor type, which decides whether
                         * the offset is computed; it may arrive after the surface data. */

                        // Compute the pressure offset.
                        xdive->ComputePressureOffsetFromSurfaceTimeSeries(3);

                        break;

                    case x::XMSG_TYPE_ID_GPS_ENDOFDIVE:

                        /* Do type-specific pre-processing for GPS (end-of-dive) messages: */

                        if(!xdive->is_surface_pres_valid())

                            // Update the derived surface data fields.
                            xdive->UpdateDerivedSurfaceDataFields();

                        // Compute the pressure offset.
                        xdive->ComputePressureOffsetFromSurfaceTimeSeries(3);

                        break;

                    case x::XMSG_TYPE_ID_PRES_SURF0:
                    case x::XMSG_TYPE_ID_PRES_SURF1:
                    case x::XMSG_TYPE_ID_PRES_SURF2:
                    case x::XMSG_TYPE_ID_PRES_SURF3:
                    case x::XMSG_TYPE_ID_PRES_SURF4:
                    case x::XMSG_TYPE_ID_PRES_SURF5:
                    case x::XMSG_TYPE_ID_PRES_SURF6:
                    case x::XMSG_TYPE_ID_PRES_SURF7:
                    case x::XMSG_TYPE_ID_PRES_SURF8:
                    case x::XMSG_TYPE_ID_PRES_SURF9:
                    case x::XMSG_TYPE_ID_PRES_SURFA:
                    case x::XMSG_TYPE_ID_PRES_SURFB:
                    case x::XMSG_TYPE_ID_PRES_SURFC:
                    case x::XMSG_TYPE_ID_PRES_SURFD:
                    case x::XMSG_TYPE_ID_PRES_SURFE:
                    case x::XMSG_TYPE_ID_PRES_SURFF:

                        /* Do type-specific pre-processing for surface data messages: */

                        // Update the derived surface data fields.
                        xdive->UpdateDerivedSurfaceDataFields();

                        // Compute the pressure offset.
                        xdive->ComputePressureOffsetFromSurfaceTimeSeries(3);

                        break;

                    default:

                        /* Do nothing. */

                        break;

                    } // switch(*it)

        }CATCH_BLOCK("Failed to do message-type-specific pre-processing!")

        try {

            /* Process the dive data: */

            ProcessDiveData(xdive);

        } CATCH_BLOCK("Failed to process dive data!")

        try {

            /* Do message-type-specific post-processing: */

            if(xmsg_id_types)

                // Iterate over the message type identifiers.
                for(auto it = xmsg_id_types->begin(); it < xmsg_id_types->end(); ++it)

                    switch(*it) {

                    case x::XMSG_TYPE_ID_GPS_STARTOFDIVE:

                        /* Do type-specific post-processing for GPS (start-of-dive) messages: */

                        if(msg_id_dive < 0)

                            break;

                        try {

                            shared_ptr<XFloatDiveData> previous_dive_data;

#                           if defined(BUILD_DEBUG) && BUILD_DEBUG
                            {
                                // Set the scoped thread log attributes.
                                BOOST_LOG_SCOPED_THREAD_ATTR(
                                        "WhoiID",
                                        attr::constant<int>(msg->id_instrument()));
                                BOOST_LOG_SCOPED_THREAD_ATTR(
                                        "DacID",
                                        attr::constant<int>(msg->id_dac()));
                                BOOST_LOG_SCOPED_THREAD_ATTR(
                                        "WmoID",
                                        attr::constant<int>(msg->id_wmo()));
                                BOOST_LOG_SCOPED_THREAD_ATTR(
                                        "DiveID",
                                        attr::constant<int>(msg_id_dive));
                                ldbg() << "Decoded start-of-dive GPS message. "
                                          << "Checking completeness of previous dive...";
                            }
#                           endif // defined(BUILD_DEBUG) && BUILD_DEBUG

                            // Reprocess the previous dive if it is complete.
                            if((previous_dive_data = (xfloat->GetDiveData(msg_id_dive - 1, true)))
                                    && !previous_dive_data->is_complete())

                                // Reprocess incomplete dive data.
                                ProcessDiveData(previous_dive_data);

                        }CATCH_BLOCK("Failed to do message-type-specific post-processing for "
                                "start-of-dive GPS message!")

                        break;

                    default:

                        /* Do nothing. */

                        break;

                    } // switch(*it)

        }CATCH_BLOCK("Failed to do message-type-specific post-processing!")

    }

    /** Functor operator overload function: the main entry point for the telemetry decoder
     *  thread(s).
     *
     *  This function defines the behavior of the telemetry decoder thread(s), which are
     *  responsible for decoding the binary data from the processed mail messages that are
     *  produced by the mail processing thread, @ref XFloatMailProcessingService. After
     *  initialization, zero or more instances of this thread are instantiated. The static
     *  configuration options for this thread are set as a result of parsing the command-line
     *  arguments and/or configuration file.
     *
     *  First, the scope thread attribute "ThreadName" is set to "sbd-decode". This attribute is
     *  used by the global logger object for filtering, etc. Then the thread enters its primary
     *  loop, in which it pops mail messages from the incoming queue one at a time and decodes
     *  the messages before passing them to the next stage in the processing pipeline, the
     *  post-processing analysis thread(s), @ref XFloatPostProcessingService. If the incoming
     *  message queue is empty, then the connection o the SQL database is closed, and the thread
     *  sleeps for @ref XFLT_THREAD_SLEEP_MS milliseconds.
     *
     *  Once termination is signaled by the daemon process i.e. @ref termination_signaled_ is
     *  TRUE, the thread will exit its primary loop and die.
     *
     *  @exception      None. Never throws.
     */
    void operator ()() ATTR_NO_EXCEPTIONS {

        namespace attr = blg::attributes;

        shared_ptr<XFloatMailMessage> msg;

        BOOST_LOG_SCOPED_THREAD_ATTR("ThreadName", attr::constant<string>("sbd-decode"));
        ldbg() << "Thread instantiated.";

        // Enter the primary thread loop.
        do { // } while(!termination_signaled_);

            try {

                // Pop message from queue. Do not block.
                if(MsgProcessingQueuePop(msg, false)) {

                    /*  Iterate over and decode all of the messages in the queue before checking
                     *  if terminations is signaled. If any of the messages in the queue are not
                     *  processed before termination, then there is a (slight) chance that they
                     *  will not be processed when the decoder is restarted!
                     *
                     */

                    do { // } while(MsgProcessingQueuePop(msg, false));

                        // Decode the mail message.
                        DecodeMessage(msg);

                        // Reset the message pointer.
                        msg.reset();

                    } while(MsgProcessingQueuePop(msg, false));

                } else {

                    // Close SQL connection if open, for now, and sleep.
                    CloseSqlConnection();
                    sleep(bpt::millisec(XFLT_THREAD_SLEEP_MS));
                }

            }CATCH_NOTHROW("Exception caught in telemetry decoder thread!")

        } while(!termination_signaled_);

        // Terminate.
        ldbg() << "Termination signal received! Shutting down...";

        // Drain messages queued while this thread was sleeping, so none are lost on shutdown.
        while(MsgProcessingQueuePop(msg, false)) {

            try {

                DecodeMessage(msg);

            }CATCH_NOTHROW("Exception caught while draining telemetry decoder queue!")

            msg.reset();
        }

        CloseSqlConnection();

        return;
    }

    /** Push a mail message into the telemetry decoder queue.
     *
     *  This function is used by the previous stage in the processing pipeline, the mail processing
     *  thread, @ref XFloatMailProcessingService, to pass a mail message to the telemetry decoder
     *  thread(s). The mail message processing queue is synchronous, so the underlying push
     *  function is atomic.
     *
     *  @param[in]      msg             The mail message to push into the  queue.
     *  @param[in]      block           If TRUE, this function will block until the queue
     *                                  is not full. If FALSE, this function will return FALSE
     *                                  if the queue is full (and the element will not be
     *                                  pushed into the queue).
     *
     *  @return         TRUE if the element was successfully pushed into the queue, or FALSE
     *                  otherwise.
     */
    static bool MsgDecodingQueuePush(const shared_ptr<XFloatMailMessage>& msg,
    bool block = true) {
        return msg_decoding_queue_.push(msg, block);
    }

    /** Get the number of telemetry decoder threads.
     *
     *  @return         The number of telemetry decoder threads.
     */
    static unsigned int num_threads() {
        return num_threads_;
    }

    /** Instantiate the telemetry decoder worker thread(s).
     *
     *  This function is used by the daemon thread to instantiate zero or more telemetry decoder
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
    static void StartWorkerThreads() ATTR_NO_EXCEPTIONS {

        try {

            ldbg() << "Starting telemetry decoder thread(s)...";

            for(unsigned int i = 0; i < num_threads_; i++) {

                shared_ptr<XFloatTelemetryDecoderService> thread(
                        make_shared<XFloatTelemetryDecoderService>());
                thread_objects_.push_back(thread);
                thread_group_.create_thread(*thread);
            }

        }CATCH_BLOCK("Failed to start telemetry decoder worker thread(s)!")
    }

    /** Terminate the telemetry decoder worker thread(s).
     *
     *  This function is used by the daemon thread to kill the telemetry decoder worker thread(s)
     *  upon application termination. The function signals termination, and then attempts to join
     *  the thread group.
     *
     *  @warning            If any of telemetry decoder threads fails to terminate properly, then
     *                      the @c join_all() function could block indefinitely. Precautions have
     *                      been take to ensure that this does not happen, but it may still be
     *                      possible.
     *
     *  @exception          None. Never throws.
     */
    static void TerminateWorkerThreads() ATTR_NO_EXCEPTIONS {

        try {

            ldbg() << "Terminating telemetry decoder thread(s)...";

            set_termination_signaled();

            if(num_threads_)
                thread_group_.join_all();

        }CATCH_BLOCK("Failed to terminate telemetry decoder worker thread(s)!")

    }

};

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */
#endif /* XFLOATTELEMETRYDECODERSERVICE_H_ */
