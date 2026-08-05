/** The @ref util::xmsg::XEngBITFail class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 2:02:19 PM
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


#ifndef XENGBITFAIL_H_
#define XENGBITFAIL_H_

/* Includes */


// local
#include "XEngBIT.h"


/* Definitions */

namespace util {
namespace xmsg {

/* Class Definition */

/** Class representing an engineering startup message for BIT failed.
 *
 */
class XEngBITFail:
        public XEngBIT {

          public:

    /* Member Functions */

    /** Parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XEngBITFail(int16_t dive, uint16_t whoi, const vector<uint8_t> data) :
            XEngBIT(dive, whoi, data) {
      /* Do nothing. */
    }

    /** The destructor.
     *
     */
    ~XEngBITFail() { /* Do nothing. */ }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XENGBITFAIL_H_ */
