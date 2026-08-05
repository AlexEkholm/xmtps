/** The Iridium Definitions header file.
 *
 *  This file contains definitions that are used by the @ref argo::iridium::Iridium class and the
 *  classes that inherit from it.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jun 9, 2014 2:17:29 PM
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

#ifndef IRIDIUMDEFINITIONS_H_
#define IRIDIUMDEFINITIONS_H_

/* Includes: */

// local
#include "../utility/Position.h"
#include "../utility/FileSystemUtils.h"

/* Definitions: */

/** The default/bad/invalid dive value. */
#define IRI_DIVE_UNKNOWN            -999

/** Default/fill value for invalid Iridium message gain value. */
#define IRI_GAIN_INVALID            9999

/** The default/bad/invalid unique identifier value. */
#define IRI_ID_UNKNOWN               -1

/** Default/fill value for invalid Iridium message offset value. */
#define IRI_OFFS_INVALID            9999

/** Default/fill value for invalid Iridium Circular Error Probable (CEP) radius. */
#define IRI_POS_INVALID_CEP         999999

/** Default/fill value for invalid Iridium position latitude. */
#define IRI_POS_INVALID_LAT         UTIL_POS_INVALID_LAT

/** Default/fill value for invalid Iridium position longitude. */
#define IRI_POS_INVALID_LON         UTIL_POS_INVALID_LAT

namespace argo {
namespace iridium {
using boost::algorithm::trim;
using boost::filesystem::ifstream;
using boost::filesystem::ofstream;
using boost::filesystem::path;
using boost::format;
using boost::gregorian::date;
using boost::lexical_cast;
using std::lock_guard;
using std::make_shared;
using boost::posix_time::ptime;
using boost::posix_time::hours;
using boost::posix_time::millisec;
using boost::posix_time::minutes;
using boost::posix_time::not_a_date_time;
using boost::posix_time::seconds;
using boost::posix_time::time_duration;
using std::shared_ptr;
using std::recursive_mutex;
using std::endl;
using std::for_each;
using std::istream;
using std::istringstream;
using std::max;
using std::ostream;
using std::ostringstream;
using std::smatch;
using std::string;
using std::stringstream;
using std::regex;
using std::regex_match;
using std::regex_replace;
using std::regex_search;
using std::vector;
}
}

#endif /* IRIDIUMDEFINITIONS_H_ */
