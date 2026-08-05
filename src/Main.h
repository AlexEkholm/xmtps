/** The @ref main() function header file.
 *
 *  This file contains includes and function prototypes that are needed for the main() source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:42:32 PM
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

// local
#include "stage1_MailRetrievalThread.h"
#include "utility/SignalHandler.h"

namespace cfg = util::config;

/* Function Prototypes: */

static const util::config::LocalConfiguration& Initialize(int argc, char** argv) ATTR_NO_EXCEPTIONS;

static void StartWorkerThreads() ATTR_NO_EXCEPTIONS;

static void TerminateWorkerThreads() ATTR_NO_EXCEPTIONS;

