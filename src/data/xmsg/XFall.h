/** The @ref util::xmsg::XFall class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 11:18:07 AM
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

#ifndef XFALL_H_
#define XFALL_H_

/* Includes */

#include "AbstractXFallRise.h"

namespace util {
  namespace xmsg {

    /** Class representing a fall time-series data X-Message.
     *
     */
    class XFall:
        public AbstractXFallRise {

      public:

        /* Member Functions */

        /** Parameterized constructor.
         *
         *  @param[in]  dive    The dive or cycle number of this message.
         *  @param[in]  whoi    The unique WHOI identifier.
         *  @param[in]  data    The binary fall rate X-message data to decode.
         *
         *  @exception  log::MessageException if an decode error occurs.
         */
        XFall(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                AbstractXFallRise(dive, whoi, data) {
          DecodeXMsgData(data);     // Decode message data.
        }

        /** Destructor.
         *
         */
        ~XFall() {
          /* Do nothing. */
        }

        /** Decode the fall data X-Message from the binary message data.
         *
         *  @param[in]  data    The binary message data to decode.
         *
         *  @exception  log::MessageException if the message is invalid or an error occurs during
         *              decoding.
         */
        void DecodeXMsgData(const vector<uint8_t>& data) {
          try {
            if((data.data()[0] & 0xF0) != (XMSG_TYPE_ID_FALL0 & 0xF0)) {
              EXC_THROW("Invalid X-Message unique identifier!");
            }
            UnpackData(data);       // Unpack the message data.
          }CATCH_BLOCK("Failed to decode fall time-series X-Message!")
        }

        /** Get the listing entry string for this fall data X-Message
         *
         *  @param[in]  pg      The pressure sensor sensitivity scale factor, in LSB/dBar.
         *  @param[in]  po      The pressure sensor offset, in dBar.
         *  @param[in]  sg      The salinity sensor sensitivity scale factor, in LSB/PSU.
         *  @param[in]  so      The salinity sensor offset, in PSU.
         *  @param[in]  tg      The temperature sensor sensitivity scale factor, in LSB/degC.
         *  @param[in]  to      The temperature sensor offset, in degC.
         *  @param[in]  pc      The pressure correction, in dBar, to be subtracted computed
         *                      pressure value.
         *
         *  @exception  log::MessageException if an error occurs while generating the listing string.
         *
         *  @return     The listing entry string for this fall data X-Message
         */
        const string GenerateListingString(
            uint16_t pg,
            uint16_t po,
            uint16_t sg,
            uint16_t so,
            uint16_t tg,
            uint16_t to,
            float pc) const {
          ostringstream os;
          size_t sam = 0;

          try {
            {
              const format fmt("F   %5d %4d %3d %4d %20s");
              os << format(fmt) % id_instrument() % id_dive() % (int) xmsg_index() % num_samples()
                  % GenerateIso8601String(start_time());
            }

            {
              const format fmt("\nf   %5d %4d %3d %4d ");
              for_each(
                  sensor_data().begin(),
                  sensor_data().end(),
                  [&](const XSensorTripletCodeCounts& sensor_data) {

                    os << format(fmt) % id_instrument() % id_dive() % (int) xmsg_index() % sam++
                    << sensor_data.GenerateListingString(pg, po, sg, so, tg, to, pc);

                  });
            }
          }CATCH_BLOCK("Failed to generate listing string for fall time-series data object!")

          return os.str();
        }
    };

  } /* namespace xmsg */
} /* namespace util */
#endif /* XFALL_H_ */
