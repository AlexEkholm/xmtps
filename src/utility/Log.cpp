/** The @ref util::log::Log class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:16:21 PM
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

/* Includes */

#include "Log.h"

namespace util {
namespace log {

/* Static Variables */

//shared_ptr<blg::sinks::synchronous_sink<blg::sinks::text_file_backend> > Log::global_file_logger_sink_;

sev_logger_mt Log::global_logger_;

path Log::log_directory_path_;

LogLevel_t Log::log_reporting_level_ = DEFAULT_LOG_LEVEL;

} /* namespace log */
} /* namespace util */
