/** The @ref argo::iridium::xfloat::XFloatPostProcessingService class header file.
 *
 *  This class is defines the operation of the post-processing analysis thread(s), which are the
 *  fourth stage of the processing pipeline. The post-processing analysis thread(s) are
 *  responsible for generating plots, Matlab, and NetCDF files as well as performing some analysis
 *  of the decoded telemetry fields in order to derive performance metrics of the instrument.
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

#ifndef XFLOATPOSTPROCESSINGSERVICE_H_
#define XFLOATPOSTPROCESSINGSERVICE_H_

/* Includes: */

// local
#include "ThreadDefinitions.h"
#include "utility/MysqlUtils.h"

namespace argo {
namespace iridium {
namespace xfloat {

/* Class Definition: */

/** The post-processing analysis thread class.
 *
 *  This class is defines the operation of the post-processing analysis thread(s), which are the
 *  fourth stage of the processing pipeline. The post-processing analysis thread(s) are
 *  responsible for generating plots, Matlab, and NetCDF files as well as performing some analysis
 *  of the decoded telemetry fields in order to derive performance metrics of the instrument.
 */
class XFloatPostProcessingService:
        public util::kml::KmlUtil {

    /* Friends Functions: */

    /** Friend function for populating some of the static members of this class from the
     *  configuration file options.
     */
    friend options_description
    util::config::LocalConfiguration::GenerateConfigurationFileOptionsDescription();

private:

    /* Static Data Members: */

#   if defined(XMTPS_POST_PROC_USES_QUEUE) && XMTPS_POST_PROC_USES_QUEUE

    static util::thread::SynchronousQueue<shared_ptr<XFloat> > float_processing_queue_;

#   endif // defined(XMTPS_POST_PROC_USES_QUEUE) && XMTPS_POST_PROC_USES_QUEUE

    /** Duration to sleep when not performing post-processing, in seconds.
     *
     *  The thread sleeps for this duration between periodically iterating over all cached
     *  instrument data objects and performing post-processing as needed.
     */
    static unsigned int idle_sleep_seconds_;

    /** The post-processing analysis thread group.
     *
     *  This object represents the group of post-processing analysis threads that are instantiated after
     *  initialization. Each new thread that is instantiated is added to this group. Upon
     *  termination, the thread group is joined.
     */
    static thread thread_;

    /** The post-processing analysis thread objects.
     *
     *  Each instance of the post-processing analysis threads is created by invoking the functor of an
     *  instance of this runnable class. Each instance of this object that is created during
     *  instantiation of a new thread is stored in this vector.
     */
    static shared_ptr<XFloatPostProcessingService > thread_object_;

    /** The termination signaled flag.
     *
     *  This flag is used  to signal termination of the post-processing analysis thread. During
     *  normal operation, the value of this field will be FALSE. Once termination is signaled, it
     *  will become TRUE causing all of the single instance of the mail retrieval to to exit its
     *   primary loop and die.
     */
    static std::atomic<bool> termination_signaled_;

private:

    /* Private Member Functions: */

#   if defined(XMTPS_POST_PROC_USES_QUEUE) && XMTPS_POST_PROC_USES_QUEUE

    static bool FloatProcessingQueuePop(shared_ptr<XFloat>& flt,
    bool block = false) {
        return float_processing_queue_.pop(flt, block);
    }

#   endif // defined(XMTPS_POST_PROC_USES_QUEUE) && XMTPS_POST_PROC_USES_QUEUE

    static void set_termination_signaled(bool termination_signaled = true) {
        termination_signaled_ = termination_signaled;
    }

public:

    /* Public Member Functions: */

    /** The constructor.
     *
     */
    XFloatPostProcessingService() {

        /* Do nothing. */

    }

    /** The destructor.
     *
     */
    ~XFloatPostProcessingService() {
        /* Do nothing. */
    }

#   if defined(XMTPS_POST_PROC_USES_QUEUE) && XMTPS_POST_PROC_USES_QUEUE

    static bool FloatProcessingQueuePush(const shared_ptr<XFloat>& flt, bool block = true) {
        return float_processing_queue_.push(flt, block);
    }

#   endif // defined(XMTPS_POST_PROC_USES_QUEUE) && XMTPS_POST_PROC_USES_QUEUE

    /** Functor operator overload function: the main entry point for the post-processing analysis
     *  thread.
     *
     *  This function defines the behavior of the post-processing analysis thread which is
     *  responsible for generating tertiary files (e.g. Google Earth KML and status files).
     *
     *  During execution the primary loop consists of the following. First, sleep for
     *  @ref idle_sleep_seconds_. Then, iterate over all of the cached instrument objects and
     *  update files as needed. This loop continues until termination is signaled by the
     *  daemon/service thread.
     *
     *  Once termination is signaled by the daemon process i.e. @ref termination_signaled_ is
     *  TRUE, the thread will exit its primary loop and die.
     *
     *  @exception      None. Never throws.
     */
    void operator ()() ATTR_NO_EXCEPTIONS {

        namespace attr = blg::attributes;

        bool update_master;
        vector<shared_ptr<XFloat> > insts;
        shared_ptr<XFloat> inst;
        unsigned int sleep_ms;

        BOOST_LOG_SCOPED_THREAD_ATTR("ThreadName", attr::constant<string>("post-process"));
        ldbg() << "Thread instantiated.";

        // Enter the primary thread loop.
        do { // } while(!termination_signaled_);

            try {

#               if defined(XMTPS_POST_PROC_USES_QUEUE) && XMTPS_POST_PROC_USES_QUEUE

                /* Read instrument objects from queue: */

                // Pop message from queue. Do not block.
                if(FloatProcessingQueuePop(inst, false)) {

                    do {

                        // Write KML file.
                        inst->WriteKmlFile();

                        // Write the status file.
                        inst->WriteStatusFile();

                        // Reset the instrument pointer.
                        inst.reset();

                    } while(FloatProcessingQueuePop(inst, false));

                    // Update the master KML file.
                    XFloat::WriteKmlMasterFile();

                }

                // After all instruments are processed, sleep.
                sleep(millisec(XFLT_THREAD_SLEEP_MS));

#           else

                /* Iterate over instrument objects and update as needed: */

                // Sleep unless terminated.
                for(sleep_ms = 0; sleep_ms < idle_sleep_seconds_ * 1000 && !termination_signaled_;
                        sleep_ms += XFLT_THREAD_SLEEP_MS)

                    sleep(millisec(XFLT_THREAD_SLEEP_MS));

                // Iterate over instrument objects.
                update_master = false;
                insts = XFloat::GetAllCachedFloats();
                for(auto float_it = insts.begin();
                        float_it < insts.end() && !termination_signaled_; ++float_it) {

                    if(!(inst = *float_it))
                        continue;

                    // Set scoped thread log attributes.
                    BOOST_LOG_SCOPED_THREAD_ATTR("WhoiID", attr::constant<int>(inst->id_instrument()));
                    BOOST_LOG_SCOPED_THREAD_ATTR("DacID", attr::constant<int>(inst->id_dac()));
                    BOOST_LOG_SCOPED_THREAD_ATTR("WmoID", attr::constant<int>(inst->id_wmo()));

#                   if defined(XMTPS_DEBUG_POST_PROC_THREAD) \
                        && XMTPS_DEBUG_NC_FILE_WRITER_THREAD

                    ldbg() << "Performing post-processing tasks for instrument #"
                            << inst->id_instrument() << "...";

#                   endif // defined(XMTPS_DEBUG_POST_PROC_THREAD) ...

                    // Write KML file.
                    if(inst->WriteKmlFile())
                        update_master = true;

                    // Write the status file.
                    inst->WriteStatusFile();

                    inst.reset();
                }
                insts.clear();

                if(update_master) {

                    // Update the master KML file.
                    XFloat::WriteKmlMasterFile();

                } 
                

                // Close SQL connection when not in use.
                util::mysql::MysqlUtils::CloseSqlConnection();
                
#           endif // defined(XMTPS_POST_PROC_USES_QUEUE) && XMTPS_POST_PROC_USES_QUEUE

            }CATCH_NOTHROW("Exception caught in post-processing analysis thread!")

        } while(!termination_signaled_);

        // Terminate.
        ldbg() << "Termination signal received! Shutting down...";

        return;
    }

    /** Instantiate the post-processing analysis worker thread.
     *
     *  This function is invoked by the daemon thread after initialization to instantiate the
     *  post-processing analysis thread. It creates a new instance of
     *  @ref XFloatPostProcessingService, and instantiates a new thread that invokes the functor
     *  operator of the object.
     *
     *  @exception          None. Never throws.
     */
    static void StartWorkerThread() ATTR_NO_EXCEPTIONS {

        try {

            ldbg() << "Starting post-processing analysis thread...";
            thread_object_ = make_shared<XFloatPostProcessingService>();
            thread_ = thread(*thread_object_);

        }CATCH_BLOCK("Failed to start post-processing analysis worker thread!")
    }

    /** Terminate the mail post-processing analysis thread.
     *
     *  This function is used by the daemon thread to kill the worker thread upon application
     *  termination. The function signals termination, and then attempts to join the child thread.
     *  If the child thread does not join, then the function will timeout after @ref
     *  XMTPS_THREAD_JOIN_SECONDS seconds, a warning will be issued, and the child thread will be
     *  abandoned.
     *
     *  @exception          None. Never throws.
     */
    static void TerminateWorkerThread() ATTR_NO_EXCEPTIONS {

        try {

            ldbg() << "Terminating post-processing analysis thread...";
            set_termination_signaled();

            if(thread_.joinable()
                    && !thread_.try_join_for(boost::chrono::seconds(XMTPS_THREAD_JOIN_SECONDS)))

                lwrn() << "Failed to join post-processing analysis thread after waiting "
                          << XMTPS_THREAD_JOIN_SECONDS
                          << " seconds!";
            else

                ldbg() << "Thread terminated.";

        }CATCH_BLOCK("Failed to terminate post-processing analysis worker thread!")

    }
};

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */

#endif /* XFLOATPOSTPROCESSINGSERVICE_H_ */
