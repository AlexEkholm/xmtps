/** The @ref util::xmsg::XSurfaceData class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jun 16, 2014 11:31:04 PM
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

#ifndef XSURFACEPRESSURE_H_
#define XSURFACEPRESSURE_H_

/* Includes: */

// local
#include "XMessageIndexable.h"
#include "AbstractXFallRise.h"

namespace util {
namespace xmsg {

/* Class Definition: */

/** The surface time-series data X-Message class.
 *
 *
 *
 */
class XSurfaceData:
        public AbstractXFallRise {

public:

    /* Member Functions: */

    /** Parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        Binary surface pressure X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XSurfaceData(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    AbstractXFallRise(dive, whoi, data) {

        // Decode the surface pressure message.
        DecodeXMsgData(data);

    }

    /** Destructor.
     *
     */
    ~XSurfaceData() {

        /* Do nothing. */

    }

    /** Decode the surface pressure X-message.
     *
     *  @param[in]      data            Binary surface pressure X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {

            // The the identifier.
            if((data.data()[0] & 0xF0) != (XMSG_TYPE_ID_PRES_SURF0 & 0xF0))
                EXC_THROW("Invalid message type identifier.!");

            // Unpack the message data.
            UnpackData(data);

        }CATCH_BLOCK("Failed to decode surface data message!")
    }

    /** Generate the listing entry string for this surface time-series data X-Message.
     *
     *  @param[in]  pg      The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po      The pressure sensor offset, in dBar.
     *  @param[in]  sg      The salinity sensor sensitivity scale factor, in LSB/PSU.
     *  @param[in]  so      The salinity sensor offset, in PSU.
     *  @param[in]  tg      The temperature sensor sensitivity scale factor, in LSB/degC.
     *  @param[in]  to      The temperature sensor offset, in degC.
     *  @param[in]  pc      The pressure correction, in dBar, to be subtracted computed pressure
     *                      value.
     *
     *  @throw      log::MessageException if an error occurs while generating the listing string.
     *
     *  @return     The listing entry string for this surface time-series data X-Message.
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
                const format fmt("SFC %5d %4d %3d %4d %20s");
                os << format(fmt) % id_instrument() % id_dive() % (int) xmsg_index() % num_samples()
                        % GenerateIso8601String(start_time());
            }

            {
                const format fmt("\nsfc %5d %4d %3d %4d ");
                for_each(
                        sensor_data().begin(),
                        sensor_data().end(),
                        [&](const XSensorTripletCodeCounts& sensor_data) {

                            os << format(fmt)
                                    % id_instrument() % id_dive() % (int) xmsg_index() % sam++
                                << sensor_data.GenerateListingString(pg, po, sg, so, tg, to, pc);

                        });
            }
        }CATCH_BLOCK("Failed to generate listing string for surface time-series data object!")

        return os.str();
    }
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XSURFACEPRESSURE_H_ */
