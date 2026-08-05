/** The @ref util::config::LocalConfiguration class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:32:16 PM
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

#include "LocalConfiguration.h"

namespace util {
namespace config {

/* Static Variables */

LocalConfiguration LocalConfiguration::configuration_;

path LocalConfiguration::configuration_file_path_(CFG_DEFAULT_PATH);

bool LocalConfiguration::is_daemon_(CFG_DEFAULT_ISDAEMON);

path LocalConfiguration::pid_file_path_(CFG_DEFAULT_PATH_PID);

const char* LocalConfiguration::SPLASH = //
//"====================================================================================================\n"//
//"\n"//
//"   ██╗  ██╗███╗   ███╗████████╗██████╗ ███████╗     X-Message Mail-Telemetry Processing Service\n"//
//"   ╚██╗██╔╝████╗ ████║╚══██╔══╝██╔══██╗██╔════╝     v" PROG_VERS " (built " BUILD_DATETIME ")\n"//
//"    ╚███╔╝ ██╔████╔██║   ██║   ██████╔╝███████╗     Alexander K. Ekholm, Engineer I, Float Group\n"//
//"    ██╔██╗ ██║╚██╔╝██║   ██║   ██╔═══╝ ╚════██║     Physical Oceanography\n"//
//"   ██╔╝ ██╗██║ ╚═╝ ██║   ██║   ██║     ███████║     Woods Hole Oceanographic Institution\n"//
//"   ╚═╝  ╚═╝╚═╝     ╚═╝   ╚═╝   ╚═╝     ╚══════╝     Woods Hole, MA, USA\n"//
//"\n"//
//"====================================================================================================\n"//
//"";
"====================================================================================================\n"//
"\n"//
"  X-Message Mail-Telemetry Processing Service\n"//
"  v" PROG_VERS " (built " BUILD_DATETIME ")\n"//
"  Alexander K. Ekholm, Research Engineer, Float Group\n"//
"  Physical Oceanography\n"//
"  Woods Hole Oceanographic Institution\n"//
"  Woods Hole, MA, USA\n"//
"\n"//
"====================================================================================================\n"//
"";

const char* LocalConfiguration::SPLASH_LIS = //
"#===================================================================================================\n"
"#        _       ____  ______  ____   ________            __     ______\n"
"#       | |     / / / / / __ \\/  _/  / ____/ /___  ____ _/ /_   / ____/________  __  ______\n"
"#       | | /| / / /_/ / / / // /   / /_  / / __ \\/ __ `/ __/  / / __/ ___/ __ \\/ / / / __ \\\n"
"#       | |/ |/ / __  / /_/ // /   / __/ / / /_/ / /_/ / /_   / /_/ / /  / /_/ / /_/ / /_/ /\n"
"#       |__/|__/_/ /_/\\____/___/  /_/   /_/\\____/\\__,_/\\__/   \\____/_/   \\____/\\__,_/ .___/\n"
"#                                                                                  /_/\n"
"#===================================================================================================\n"
"#\n"
"# This data was produced and distributed as part of projects that were planned and executed by or\n"
"# in collaboration with the WHOI Float Group.\n"
"#\n"
"# This file was created by:\n"
"#\n"
"#   The X-Message Telemetry Processing Service (XMTPS)\n"
"#   version " PROG_VERS " (built " BUILD_DATETIME ")\n"
"#\n"
"# XMTPS was created and is maintained by:\n"
"#\n"
"#   Alexander K. Ekholm <aekholm@whoi.edu>\n"
"#   Research Engineer, Float Group\n"
"#   Department of Physical Oceanography\n"
"#   Woods Hole Oceanographic Institution\n"
"#   Woods Hole, MA, USA\n"
"#\n";//

/* Function Definitions */




} /* namespace config */
} /* namespace util */


