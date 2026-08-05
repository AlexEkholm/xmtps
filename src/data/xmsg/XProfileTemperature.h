/** The @ref util::xmsg::XProfileTemperature class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 11:16:19 AM
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

#ifndef XPROFILETEMPERATURE_H_
#define XPROFILETEMPERATURE_H_

/* Includes */

#include "AbstractXSensorData.h"

/* Definitions */

namespace util {
  namespace xmsg {

    /* Class Definitions */

    /** Class representing a profile temperature X-Message.
     *
     */
    class XProfileTemperature:
        public AbstractXSensorData {


      public:

        /* Member Functions */

        /** Parameterized constructor.
         *
         *  @param[in]      dive        The dive or cycle number of this message.
         *  @param[in]      whoi        The unique WHOI identifier.
         *  @param[in]      data        The binary profile temperature X-message data to decode.
         *
         *  @throw          util::log::MessageException if an decode error occurs.
         */
        XProfileTemperature(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                AbstractXSensorData(dive, whoi, data) {
          DecodeXMsgData(data);    // Decode GPS message.
        }

        /** Destructor.
         *
         */
        ~XProfileTemperature() {
          /* Do nothing. */
        }

        /** Decode the bin-averaged profile temperature X-Message from the binary message data.
         *
         *  @param[in]      data        The binary message data to decode.
         *
         *  @throw          util::log::MessageException if the message is invalid or an error occurs during
         *                  decoding.
         */
        void DecodeXMsgData(const vector<uint8_t>& data) {
          try {
            const uint8_t* d = CheckXMsgData(data, (int) -1, (int) 5);
            if((d[0] & 0xF0) != (XMSG_TYPE_ID_TEMP0 & 0xF0))
              EXC_THROW("Invalid X-Message unique identifier!");
            UnpackSensorData(data);
          }CATCH_BLOCK("Failed to decode profile temperature X-Message!")
        }
    };

  } /* namespace xmsg */
} /* namespace util */
#endif /* XPROFILETEMPERATURE_H_ */
