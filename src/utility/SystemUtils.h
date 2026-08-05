/** The SystemUtils class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Aug 21, 2014 10:41:32 AM
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

#ifndef SYSTEMUTILITIES_H_
#define SYSTEMUTILITIES_H_

/* Includes */

// STL C library
#include <cassert>
#include <cfloat>
#include <cmath>
#include <cinttypes>
#include <csignal>
#include <cstdio>
#include <cstdlib>
//#include <cstring>

// STL container library
#include <map>
#include <vector>

// STL I/O library
#include <iomanip>
#include <sstream>
#include <streambuf>

// STL other libraries
#include <atomic>
#include <algorithm>
#include <regex>
#include <string>

// system libraries
#include <unistd.h>

/* Definitions */

/** The deprecated function attribute macro.
 *
 */
#define ATTR_DEPRECATED __attribute__ ((__deprecated__))

#endif /* SYSTEMUTILITIES_H_ */
