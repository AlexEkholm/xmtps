/** The @ref util::xmsg::XMissionCmdEcho class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 2:13:38 PM
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

#ifndef XMISSIONCMDECHO_H_
#define XMISSIONCMDECHO_H_

/* Includes */

#include "XMessage.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Class representing a ground station command echo-back X-Message.
 *
 */
class XMissionCmdEcho:
        public XMessage {

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
    XMissionCmdEcho(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XMessage(dive, whoi, data) {
        DecodeXMsgData(data); // Decode message.
    }

    /** The destructor. */
    ~XMissionCmdEcho() {
        /* Do nothing. */
    }

    /** Decode the ground station command echo-back X-Message from the binary message data.
     *
     *  @param[in]      data        The binary message data to decode.
     *
     *  @throw          util::log::MessageException if the message is invalid or an error occurs during
     *                  decoding.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {

            // Check message.
            /* const uint8_t* d = */CheckXMsgData(data, XMSG_TYPE_ID_MISS_CMD_ECHO, -1);

            /** @todo   Implement the functionality to decode the ground station command echo-back
             *          X-Message from the binary message data.
             *
             */

        }CATCH_BLOCK("Failed to decode ground station command echo-back message!")
    }

    /** Generate a listing string representation of the ground station command echo-back message.
     *
     *  @return     A listing string representation of the ground station command echo-back message.
     */
    const string GenerateListingString() const {
        ostringstream os;

        try {

            /** @todo   Implement the functionality for generating a listing string for the ground
             *          station upload command echo-back message.
             *
             */

        }CATCH_BLOCK("Failed to generate listing string ground station command echo-back message!")

        return os.str();
    }
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XMISSIONCMDECHO_H_ */
