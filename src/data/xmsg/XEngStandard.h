/** The @ref util::xmsg::XEngStandard class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 1:59:47 PM
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

#ifndef XENGSTANDARD_H_
#define XENGSTANDARD_H_

/* Includes */

#include "XMessage.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Type Definitions */

/* Class Definitions */

/** Class representing an engineering message for a standard dive cycle.
 *
 */
class XEngStandard: public XMessage {

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
    XEngStandard(uint16_t dive, uint16_t whoi, const std::vector<uint8_t>& data) :
            XMessage(dive, whoi, data) {
        DecodeXMsgData(data); // Decode message.
    }

    /** Destructor.
     *
     */
    ~XEngStandard() {
        /* Do nothing. */
    }

    /** Decode the standard dive engineering X-Message from the binary data.
     *
     *  @param[in]      data            The binary message to decode.
     */
    void DecodeXMsgData(const std::vector<uint8_t>& data) {

        try {

            // Check message.
            /* const uint8_t* d = */ CheckXMsgData(data, XMSG_TYPE_ID_ENG_DIVE, -1);

            /** @todo   Implement the functionality for decoding the Solo-II standard dive/cycle
             *          engineering data message.
             */

        }CATCH_BLOCK("Failed to decode standard dive engineering X-Message!")
    }

    /** Generate a listing string representation of the standard dive engineering data message.
     *
     *  @return     A listing string representation of the standard dive engineering data message.
     */
    const string GenerateListingString() const {
        ostringstream os;

        try {

            /** @todo   Implement the functionality for generating a listing string for the Solo-II
             *          standard dive engineering data message.
             *
             */

        } CATCH_BLOCK("Failed to !")

        return os.str();
    }
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XENGSTANDARD_H_ */
