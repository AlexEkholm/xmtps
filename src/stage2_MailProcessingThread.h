/** The @ref argo::iridium::xfloat::XFloatMailProcessingService class header file.
 *
 *  This is the header file for the @ref argo::iridium::xfloat::XFloatMailProcessingService class.
 *  This class defines the operation of the mail processing thread(s), which are the second stage
 *  in the processing pipeline. The mail processing thread(s) consume messages that were fetched
 *  from the mail server in the previous stage of the processing pipeline, the mail retrieval
 *  thread, @ref argo::iridium::xfloat::XFloatMailRetrievalService. The messages are validated,
 *  saved to the SQL database, queued for consumption in the next stage of the processing pipeline,
 *  the telemetry decoder thread, @ref argo::iridium::xfloat::XFloatTelemetryDecoderService, and
 *  their file representations are written to the local file system.
 *
 *  This file contains the class definition and the majority of the functionality of the class.
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

#ifndef XFLOATMAILPROCESSINGSERVICE_H_
#define XFLOATMAILPROCESSINGSERVICE_H_

/* Includes: */

// local
#include "stage3_TelemetryDecoderThread.h"

namespace argo {
namespace iridium {
namespace xfloat {

/* Class Definition: */

/** The mail processing thread class.
 *
 *  This class defines the operation of the mail processing thread(s), which are the second stage
 *  in the processing pipeline. The mail processing thread(s) consumes messages that were fetched
 *  from the mail server in the previous stage of the processing pipeline, @ref
 *  XFloatMailRetrievalService. The messages are validated, saved to the SQL database, queued for
 *  consumption in the next stage of the processing pipeline, the telemetry decoder thread(s),
 *  @ref XFloatTelemetryDecoderService, and finally their file representations are written to the
 *  local file system.
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
class XFloatMailProcessingService:
    public util::mysql::LocalMysqlConfiguration {

    /* Friends Functions: */

    /** Friend function for populating some of the static members of this class from the
     *  configuration file options.
     */
    friend po::options_description util::config::LocalConfiguration::GenerateConfigurationFileOptionsDescription();

  private:

    /* Static Data Members: */

    /** The incoming mail message processing queue.
     *
     *  This synchronous FIFO queue is used to pass messages produced by the previous stage of the
     *  processing pipeline, the mail retrieval thread, @ref XFloatMailRetrievalService, to the
     *  mail processing thread(s). All the operations performed by on the queue are atomic.
     *  Depending on the number of mail processing threads, this queue may function as a
     *  one-to-one queue  or one-to-many queue.
     */
    static util::thread::SynchronousQueue<shared_ptr<XFloatMailMessage> > msg_processing_queue_;

    /** The number of mail processing threads.
     *
     *  The value of this field determines the number of mail processing threads that are
     *  instantiated after initialization. Its value is set as a result of parsing the
     *  configuration file and/or command-line arguments during initialization. If the value is
     *  unspecified, then it defaults to a value of @ref CFG_DEFAULT_NPROC.
     */
    static unsigned int num_threads_;

    /** The termination signaled flag.
     *
     *  This flag is used  to signal termination of the mail processing thread(s). During normal
     *  operation, the value of this field will be FALSE. Once termination is signaled, it will
     *  become TRUE causing all of the instances of the mail processing thread to process the
     *  remaining messages in the incoming queue, exiting their primary loop, and then die.
     */
    static std::atomic<bool> termination_signaled_;

    /** The mail processing thread group.
     *
     *  This object represents the group of mail processing threads that are instantiated after
     *  initialization. Each new thread that is instantiated is added to this group. Upon
     *  termination, the thread group is joined.
     */
    static thread_group thread_group_;

    /** The mail processing thread object(s).
     *
     *  Each instance of the mail processing thread is created by invoking the functor of an
     *  instance of this runnable class. Each instance of this object that is created during
     *  instantiation of a new thread is stored in this vector.
     */
    static vector<shared_ptr<XFloatMailProcessingService> > thread_objects_;

  private:

    /* Private Static Member Functions: */

    static bool MsgProcessingQueuePop(shared_ptr<XFloatMailMessage>& msg, bool block = false);

    static void set_termination_signaled(bool termination_signaled = true);

  public:

    /* Public Static Member Functions: */

    static bool MsgProcessingQueuePush(const shared_ptr<XFloatMailMessage>& msg, bool block = true);

    static void MsgProcessingQueuePushAll(
        vector<shared_ptr<XFloatMailMessage>>::const_iterator first,
        vector<shared_ptr<XFloatMailMessage>>::const_iterator last);

    static unsigned int num_threads();

    static void StartWorkerThreads() ATTR_NO_EXCEPTIONS;

    static void TerminateWorkerThreads() ATTR_NO_EXCEPTIONS;

  private:

    /* Private Member Functions: */

    void ProcessMailMsg(const shared_ptr<XFloatMailMessage>& msg) const;

  public:

    /* Public Member Functions: */

    XFloatMailProcessingService();

    ~XFloatMailProcessingService();

    void operator()() ATTR_NO_EXCEPTIONS;
};

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */

#endif /* XFLOATMAILPROCESSINGSERVICE_H_ */
