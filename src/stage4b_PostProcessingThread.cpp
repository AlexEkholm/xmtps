/** The @ref argo::iridium::xfloat::XFloatPostProcessingService class source file.
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


/* Includes: */

// header
#include "stage4b_PostProcessingThread.h"

namespace argo {
namespace iridium {
namespace xfloat {

/* Static Data Members: */

#if defined(XMTPS_NC_FILE_WRITER_USES_QUEUE) && XMTPS_NC_FILE_WRITER_USES_QUEUE

util::thread::SynchronousQueue<shared_ptr<XFloat> > XFloatPostProcessingService::float_processing_queue_(
        XFLT_QUEUE_SIZE_PPROCESSING);

#endif // defined(XMTPS_NC_FILE_WRITER_USES_QUEUE) && XMTPS_NC_FILE_WRITER_USES_QUEUE

unsigned int XFloatPostProcessingService::idle_sleep_seconds_(60);

thread XFloatPostProcessingService::thread_;

shared_ptr<XFloatPostProcessingService>  XFloatPostProcessingService::thread_object_;

std::atomic<bool> XFloatPostProcessingService::termination_signaled_(false);

bool XFloatPostProcessingService::final_pass_on_termination_(false);

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */
