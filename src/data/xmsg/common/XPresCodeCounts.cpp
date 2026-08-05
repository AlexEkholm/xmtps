/** The @ref util::xmsg::XPresCodeCounts class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 16, 2014 1:52:44 PM
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

// header
#include "XPresCodeCounts.h"

namespace util {
namespace xmsg {

/* Static Variables */

const char* XPresCodeCounts::phase_code_names_[] = { //
        "unknown",                      //0
        "start of descent",             //1
        "first descent pump",           //2
        "seek to drift pres",           //3
        "start of drift",               //4
        "seek during drift",            //5
        "start of descent to profile",  //6
        "start of ascent",              //7
        "end of ascent",                //8
        "ice turn-around",              //9
        "descending",                   //10
        "drift/park",                   //11
        "descending to profile",        //12
        "ascending",                    //13
        "surface",                      //14
        "invalid phase code"            //15
        };

} /* namespace xmsg */
} /* namespace util */
