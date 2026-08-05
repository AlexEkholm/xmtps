/** The @ref util::stream::StreamUtils class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 5:34:26 PM
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

#ifndef STREAMUTILITIES_H_
#define STREAMUTILITIES_H_

/* Includes */

// boost
#include <boost/interprocess/streams/bufferstream.hpp>
#include <boost/interprocess/streams/vectorstream.hpp>

// local
#include "VectorBuf.h"

namespace util {
namespace stream {

/** Class providing useful functions for manipulating I/O streams.
 *
 */
class StreamUtils {
public :

    /** Get the integer value of a hexadecimal character.
     *
     * @param[in]       c       The hexadecimal character.
     *
     * @return          The integer value of a hexadecimal character, or 0 if c is not a
     *                  hexadecimal character.
     */
    static uint8_t htoi(char c) {
        if(c >= '0' && c <= '9') return c - (uint8_t)'0';
        if(c >= 'a' && c <= 'f') return c - (uint8_t)'a' + 10;
        if(c >= 'A' && c <= 'F') return c - (uint8_t)'A' + 10;
        return 0;
    }

};

} /* namespace stream */
} /* namespace util */

#endif /* STREAMUTILITIES_H_ */
