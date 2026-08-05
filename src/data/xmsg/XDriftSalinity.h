/** The @ref util::xmsg::XDriftSalinity class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 1:55:25 PM
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

#ifndef XDRIFTSALINITY_H_
#define XDRIFTSALINITY_H_

/* Includes */

#include "AbstractXSensorData.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Type Definitions */

/* Class Definitions */

/** Class representing a drift salinity time-series X-Message.
 *
 */
class XDriftSalinity: public AbstractXSensorData {
public:

    /* Member Functions */

    /** The parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XDriftSalinity(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    AbstractXSensorData(dive, whoi, data) {
        DecodeXMsgData(data); // Decode message.
    }

    /** Destructor.
     *
     */
    ~XDriftSalinity() {
        /* Do nothing. */
    }

    /** Decode the drift salinity X-Message from binary data.
     *
     *  @param[in]      data            The binary data to decode.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {
        try {
            const uint8_t* d = CheckXMsgData(data, (int) -1, (int) 5);
            if((d[0] & 0xF0) != (XMSG_TYPE_ID_SALN_DRIFT8 & 0xF0))
                EXC_THROW("Invalid X-Message unique identifier!");
            UnpackSensorData(data);
        }CATCH_BLOCK("Failed to decode drift salinity X-Message!")
    }


};

} /* namespace xmsg */
} /* namespace util */
#endif /* XDRIFTSALINITY_H_ */
