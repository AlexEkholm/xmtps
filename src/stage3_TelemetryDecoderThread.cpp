/** The @ref argo::iridium::xfloat::XFloatTelemetryDecoderService class source file.
 *
 *  This is the source file for the @ref argo::iridium::xfloat::XFloatTelemetryDecoderService class.
 *  This class defines the operation of the telemetry decoder thread(s), which are the third stage
 *  in the processing pipeline. The telemetry decoder thread(s) consume mail messages that were
 *  processed by the previous stage of the processing pipeline, the mail processing thread(s),
 *  @ref argo::iridium::xfloat::XFloatMailProcessingService. The processed mail messages are
 *  assigned to their corresponding dive data object. Then, the Iridium Short Burst Data (SBD)
 *  messages, that are supplied as attachments to the mail messages are decoded. The decoded data
 *  is contained in an instance of a class that inherits from @ref util::xmsg::XMessage. The
 *  resultant object is also assigned to the dive data object.
 *
 *  This file contains the static data members of the @ref
 *  argo::iridium::xfloat::XFloatTelemetryDecoderService class.
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

/* Includes: */

// header
#include "stage3_TelemetryDecoderThread.h"

namespace argo {
namespace iridium {
namespace xfloat {

/* Static Data Members:  */

thread_group XFloatTelemetryDecoderService::thread_group_;

vector<shared_ptr<XFloatTelemetryDecoderService> > XFloatTelemetryDecoderService::thread_objects_;

util::thread::SynchronousQueue<shared_ptr<XFloatMailMessage> > XFloatTelemetryDecoderService::msg_decoding_queue_(
XFLT_QUEUE_SIZE_TELEMETRY);

unsigned int XFloatTelemetryDecoderService::num_threads_;

std::atomic<bool> XFloatTelemetryDecoderService::termination_signaled_(false);

/* Function Definitions: */



} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */
