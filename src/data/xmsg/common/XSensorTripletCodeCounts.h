/** The @ref util::xmsg::XSensorTripletCodeCounts class header file.
 *
 *  This is the header file for the sensor triplet and phase code class, @ref
 *  util::xmsg::XSensorTripletCodeCounts. This class represents a pressure, temperature, and
 *  salinity measurement triplet and a phase code. The phase code is used to indicate the context
 *  of the measurement (see @ref util::xmsg::XPhaseCode_t). This class is used to store a single
 *  data point for the rise, fall, or surface time-series data.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Mar 18, 2015 11:16:33 AM
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

#ifndef XSENSORTRIPLETCOUNTS_H_
#define XSENSORTRIPLETCOUNTS_H_

/* Includes: */

// local
#include "XPresCodeCounts.h"

namespace util {
namespace xmsg {

/* Class Definition: */

/** The sensor triplet and phase code class.
 *
 *  This class represents a pressure, temperature, and salinity measurement triplet and a phase
 *  code. The phase code is used to indicate the context of the measurement (see @ref
 *  XPhaseCode_t). This class is used to store a single data point for the rise, fall, or surface
 *  time-series data.
 *
 */
class XSensorTripletCodeCounts:
        public XPresCodeCounts {

private:

    /** The salinity measurement, in raw sensor counts.
     *
     */
    int32_t salinity_counts_;

    /** The temperature measurement, in raw sensor counts.
     *
     */
    int32_t temperature_counts_;

public:

    /** The constructor.
     *
     *  @param[in]  sec         The elapsed time since the reference time, in seconds.
     *  @param[in]  code        The phase code of this pressure measurement.
     *  @param[in]  pres_counts The pressure measurement, in raw sensor counts.
     *  @param[in]  temp_counts The temperature measurement, in raw sensor counts.
     *  @param[in]  saln_counts The salinity measurement, in raw sensor counts.
     */
    XSensorTripletCodeCounts(
            uint16_t sec,
            XPhaseCode_t code,
            int32_t pres_counts,
            int32_t temp_counts = XMSG_SENS_BAD,
            int32_t saln_counts = XMSG_SENS_BAD) :
                    XPresCodeCounts(sec, code, pres_counts),
                    salinity_counts_(saln_counts),
                    temperature_counts_(temp_counts) {
        /* Do nothing. */
    }



    /** The destructor.
     *
     */
    ~XSensorTripletCodeCounts() {
        /* Do nothing. */
    }

    /** Generate a listing entry string from this sensor triplet object.
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
     *  @return     A listing entry string representation of this sensor triplet object.
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

        try {
            const format fmt("%+20d %9s %7.3f %7.3f %9d # %s");
            os
                    << format(fmt) % (int) elapsed_seconds_since_reference_time()
                            % XMsgFormatPressure(PressureValue(pg, po, pc)) % TemperatureValue(tg, to)
                            % SalinityValue(sg, so) % (int) phase_code()
                            % phase_code_name();
        }CATCH_BLOCK("Failed to generate listing string for sensor triplet object!")

        return os.str();
    }

    int32_t salinity_counts() const {
        return salinity_counts_;
    }

    int32_t temperature_counts() const {
        return temperature_counts_;
    }

    /** Get the measured salinity value, in PSU.
     *
     *  @param[in]  sg          The salinity sensor sensitivity scale factor, in LSB/PSU.
     *  @param[in]  so          The salinity sensor offset, in PSU.
     *
     *  @return     The measured salinity value, in PSU.
     */
    template<typename T = double> T SalinityValue(uint16_t sg, uint16_t so) const {
        T val = sg ? (T) salinity_counts_ / (T) sg - (T) so : XMSG_SENS_BAD_SALN;
        return (val < XMSG_SENS_MIN_SALN || val > XMSG_SENS_MAX_SALN) ? XMSG_SENS_BAD_SALN : val;
    }

    /** Get the measured temperature value, in degC.
     *
     *  @param[in]  tg          The temperature sensor sensitivity scale factor, in LSB/degC.
     *  @param[in]  to          The temperature sensor offset, in degC.
     *
     *  @return     The measured temperature value, in degC.
     */
    template<typename T = double> T TemperatureValue(uint16_t tg, uint16_t to) const {
        T val = tg ? (T) temperature_counts_ / (T) tg - (T) to : XMSG_SENS_BAD_TEMP;
        return (val < XMSG_SENS_MIN_TEMP || val > XMSG_SENS_MAX_TEMP) ? XMSG_SENS_BAD_TEMP : val;
    }
};

} /* namespace xmsg */
} /* namespace util */

#endif /* XSENSORTRIPLETCOUNTS_H_ */
