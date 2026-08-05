/** The @ref util::xmsg::XPumpCounts class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 9, 2014 3:00:00 PM
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

#ifndef XPUMPCOUNTS_H_
#define XPUMPCOUNTS_H_

/* Includes */

#include "XPresCodeCounts.h"

namespace util {
namespace xmsg {

/** Class representing a single pump data measurement for a pump data time-series X-Message.
 *
 */
class XPumpCounts {

private:

    /* Data Members */

    /** The phase code of this pump data measurement.
     *
     */
    XPhaseCode_t phase_code_;

    /** The raw pressure counts of the measured pressure during pumping.
     *
     */
    int32_t pressure_counts_;

    /** The duration that the pump ran, in seconds.
     *
     */
    int16_t pump_duration_sec_;

    /** The measured voltage of the pump supply during pumping, in 0.01 volts.
     *
     */
    uint16_t pump_voltage_cv_;

    /** The measured current of the pump supply during pumping, in milliamps.
     *
     */
    uint16_t pump_current_ma_;

    /** The measured vacuum before pumping, in 0.01inHg.
     *
     */
    uint16_t case_vac_before_pumping_cinhg_;

    /** The measured vacuum after pumping, in 0.01inHg.
     *
     */
    uint16_t case_vac_after_pumping_cinhg_;

protected:

    /* Member Functions */

    /** Get the measured pressure value, in dBar.
     *
     *  @param[in]  pg          The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po          The pressure sensor offset, in dBar.
     *  @param[in]  pres_corr   The pressure correction, in dBar, to be subtracted computed
     *                          pressure value.
     *
     *  @return     The measured pressure value, in dBar.
     */
    template<typename T = double> T PressureValue(
            uint16_t pg,
            uint16_t po,
            float pres_corr) const {
        T val = pg ? (T) pressure_counts_ / (T) pg - (T) po - pres_corr : XMSG_SENS_BAD_PRES;

        return (val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES) ? XMSG_SENS_BAD_PRES : val;
    }

    /** Get the measured pump current, in amperes.
     *
     *  @return     The measured pump current, in amperes.
     */
    template<typename T = double> T pump_current_a() const {
        return (T) pump_current_ma_ / 1000.;
    }

    /** Get the measured pump voltage, in volts.
     *
     *  @return         The measured pump voltage, in volts.
     */
    template<typename T = double> T pump_voltage_v() const {
        return (T) pump_voltage_cv_ / 100.;
    }

public:

    /* Member Functions */

    /** Parameterized constructor.
     *
     *  @param[in]      code        The phase code of this pump data measurement.
     *  @param[in]      count       The raw pressure counts of the measured pressure during pumping.
     *  @param[in]      sec         The duration that the pump ran, in seconds.
     *  @param[in]      v           The measured voltage of the pump supply during pumping, in 0.01
     *                              volts.
     *  @param[in]      i           The measured current of the pump supply during pumping, in
     *                              milliamps.
     *  @param[in]      vac0        The measured vacuum counts before pumping.
     *  @param[in]      vac1        The measured vacuum counts after pumping.
     */
    XPumpCounts(
            XPhaseCode_t code,
            int32_t count,
            int16_t sec,
            uint16_t v,
            uint16_t i,
            uint16_t vac0,
            uint16_t vac1) ATTR_NO_EXCEPTIONS:
    phase_code_(code),
    pressure_counts_(count),
    pump_duration_sec_(sec),
    pump_voltage_cv_(v),
    pump_current_ma_(i),
    case_vac_before_pumping_cinhg_(vac0),
    case_vac_after_pumping_cinhg_(vac1) {
        /* Do nothing. */
    }

    /** Destructor.
     *
     */
    ~XPumpCounts() {
        /* Do nothing. */
    }

    /** Generate a listing entry string representation of this pump counts object.
     *
     *  @param[in]  pg          The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po          The pressure sensor offset, in dBar.
     *  @param[in]  pres_corr   The pressure correction, in dBar, to be subtracted computed
     *                          pressure value.
     *
     *  @throw      log::MessageException if an error occurs while generating the listing string.
     *
     *  @return     A listing entry string representation of this pump counts object.
     */
    const string GenerateListingString(uint16_t pg, uint16_t po, float pres_corr) const {
        ostringstream os;

        try {
            static const format fmt("%10.2f %10d %10.2f %10.3f %10.2f %10.2f");
            os << format(fmt) % PressureValue(pg, po, pres_corr) % pump_duration_sec_ % pump_voltage_v() % pump_current_a()
            % case_vac_before_pumping_inhg() % case_vac_after_pumping_inhg();
        }CATCH_BLOCK("Failed to generate listing string for pump counts object!")

        return os.str();
    }

    /** Get the measured vacuum after pumping, in 0.01inHg.
     *
     *  @return     The measured vacuum after pumping, in 0.01inHg.
     */
    uint16_t case_vac_after_pumping_cinhg() const
    {
        return case_vac_after_pumping_cinhg_;
    }

    /** Get the measured vacuum before pumping, in 0.01inHg.
     *
     *  @return     The measured vacuum before pumping, in 0.01inHg.
     */
    uint16_t case_vac_before_pumping_cinhg() const
    {
        return case_vac_before_pumping_cinhg_;
    }

    /** Get the measured vacuum after pumping, in inHg.
     *
     *  @return     The measured vacuum after pumping, in inHg.
     */
    template<typename T = float> T case_vac_after_pumping_inhg() const {
        return (T) case_vac_after_pumping_cinhg() / 100.;
    }

    /** Get the measured vacuum before pumping, in inHg.
     *
     *  @return     The measured vacuum before pumping, in inHg.
     */
    template<typename T = float> T case_vac_before_pumping_inhg() const {
        return (T) case_vac_before_pumping_cinhg() / 100.;
    }

};

}
/* namespace xmsg */
} /* namespace util */

#endif /* XPUMPCOUNTS_H_ */
