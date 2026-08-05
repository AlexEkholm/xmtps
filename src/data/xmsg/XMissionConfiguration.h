/** The @ref util::xmsg::XMissionConfiguration class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 11:19:37 AM
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

#ifndef XMISSIONCONFIGURATION_H_
#define XMISSIONCONFIGURATION_H_

/* Includes */

// local
#include "XAlamoMissionConfiguration.h"

/* Definitions */

namespace util {
namespace xmsg {

/** Class representing a mission configuration X-Message.
 *
 *  @note           This object supports both ALAMO and Solo-II mission configurations.
 */
class XMissionConfiguration:
        public XMessageIndexable {

private:

    /* Data Members */

    /** A shared pointer to the ALAMO mission configuration object.
     *
     *  @note       This pointer is only used for ALAMO-type mission configuration messages. If
     *              this is not an ALAMO-type message, then this pointer will be null.
     */
    shared_ptr<XAlamoMissionConfiguration> alamo_mission_configuration_;

public:

    /* Member Functions */

    /** The parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary mission configuration X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XMissionConfiguration(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XMessageIndexable(dive, whoi, data) {
        DecodeXMsgData(data);    // Decode message.
    }

    /** The destructor.
     *
     */
    ~XMissionConfiguration() {
        /* Do nothing. */
    }

    /** Decode the mission configuration message from the binary data.
     *
     *  @param[in]      data            The binary message to decode.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {

            const uint8_t* d = CheckXMsgData(data, (int) -1, (int) 5);

            // Check ID.
            if((d[0] & 0xF0) != 0xD0) {

                using namespace std;
                ostringstream os;
                os << "Invalid X-Message identifier: Expected 0xD[0:7], but received "
                   << hex
                   << setw(2)
                   << setfill('0')
                   << (int) d[0]
                   << "!";
                BOOST_THROW_EXCEPTION(log::MessageException(os));

            }

            // Get message index.
            set_xmsg_index(d[0] & 0x0F);

            // Check encoding type.
            if(d[3] == 0 && xmsg_index() == 0) {

                // This is binary encoded (ALAMO) message.
                alamo_mission_configuration_ = std::make_shared<XAlamoMissionConfiguration>(data);

            } else {

                // This is an ASCII encoded (Solo-II) message.

            }

        }CATCH_BLOCK("Failed to decode mission configuration X message!")
    }

    /** Get the listing entry string for this mission configuration X-Message.
     *
     *  @return         The listing entry string for this mission configuration X-Message.
     */
    const string GenerateListingString() const {
        ostringstream os;

        try {

            if(alamo_mission_configuration_)
                os << alamo_mission_configuration_->GenerateListingString(
                        id_instrument(),
                        id_dive(),
                        xmsg_index());

        }CATCH_BLOCK("Failed to generate listing string for mission configuration object!")

        return os.str();
    }

};

} /* namespace xmsg */
} /* namespace util */

#endif /* XMISSIONCONFIGURATION_H_ */
