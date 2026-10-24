/** The @ref util::xmsg::XValveCounts class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Mar 22, 2016 9:20:55 AM
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

#ifndef XVALVECOUNTS_H_
#define XVALVECOUNTS_H_

/* Includes: */

#include "XPresCodeCounts.h"

/* Definitions: */

namespace util {
namespace xmsg {

/* Types: */

/** The valve counts class.
 *
 *  An instance of this class represents a single valve operation decoded from a valve data
 *  message (see @ref XValve): the pressure, and the case vacuum before and after the valve
 *  operation.
 */
class XValveCounts {

private:

    /* Friends Classes: */

    friend class XDiveData;

private:

    /* Private Data Members: */

    /** The measured vacuum after opening the valve, in 0.01inHg.
     *
     */
    uint16_t case_vac_after_valve_cinhg_;

    /** The measured vacuum before opening the valve, in 0.01inHg.
     *
     */
    uint16_t case_vac_before_valve_cinhg_;

    /** The phase code of this valve data measurement.
     *
     */
    XPhaseCode_t phase_code_;

    /** The measured pressure, before opening the valve, in raw sensor counts.
     *
     */
    int32_t pressure_counts_;

    /** The duration that the valve was open, in seconds.
     *
     */
    int16_t valve_duration_sec_;

protected:

    /* Protected Member Functions: */

    /** Get the measured pressure value, in dBar.
     *
     *  @param[in]  pg      The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po      The pressure sensor offset, in dBar.
     *  @param[in]  pc      The pressure correction, in dBar, to be subtracted computed pressure
     *                      value.
     *
     *  @return     The measured pressure value, in dBar.
     */
    template<typename T = double> T PressureValue(uint16_t pg, uint16_t po, float pc) const {

        T val = pg ? (T) pressure_counts_ / (T) pg - (T) po - pc: XMSG_SENS_BAD_PRES;

        return (val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES) ? XMSG_SENS_BAD_PRES : val;
    }

public:

    /* Public Member Functions: */

    /** The parameterized constructor.
     *
     *  @param[in]  code    The phase code.
     *  @param[in]  count   The measured pressure, before opening the valve, in raw sensor counts.
     *  @param[in]  sec     The duration that the valve was open, in seconds.
     *  @param[in]  vac0    The measured vacuum after opening the valve, in 0.01inHg.
     *  @param[in]  vac1    The measured vacuum before opening the valve, in 0.01inHg.
     */
    XValveCounts(XPhaseCode_t code, int32_t count, int16_t sec, uint16_t vac0, uint16_t vac1)
    ATTR_NO_EXCEPTIONS :
                    case_vac_after_valve_cinhg_(vac1),
                    case_vac_before_valve_cinhg_(vac0),
                    phase_code_(code),
                    pressure_counts_(count),
                    valve_duration_sec_(sec) {

        /* Do nothing. */

    }

    /** The destructor.
     *
     */
    ~XValveCounts() {

        /* Do nothing. */

    }

    uint16_t case_vac_after_valve_cinhg() const {
        return case_vac_after_valve_cinhg_;
    }

    /** Get the measured vacuum after pumping, in inHg.
     *
     *  @return     The measured vacuum after pumping, in inHg.
     */
    template<typename T = float> T case_vac_after_pumping_inhg() const {
        return (T) case_vac_after_valve_cinhg() / (T) 100.;
    }

    uint16_t case_vac_before_valve_cinhg() const {
        return case_vac_before_valve_cinhg_;
    }

    /** Get the measured vacuum before pumping, in inHg.
     *
     *  @return     The measured vacuum before pumping, in inHg.
     */
    template<typename T = float> T case_vac_before_pumping_inhg() const {
        return (T) case_vac_before_valve_cinhg() / (T) 100.;
    }

    const string GenerateListingString(uint16_t pg, uint16_t po, float pres_correction) const {

        ostringstream os;

        try {

            static const format fmt("%10.2f "
                    "%10d "
                    "%10.2f %10.2f");
            os << format(fmt) % PressureValue(pg, po, pres_correction) //
                    % valve_duration_sec() //
                    % case_vac_before_pumping_inhg() % case_vac_after_pumping_inhg();

        }CATCH_BLOCK("Failed to generate listing string for valve counts object!")

        return os.str();
    }

    XPhaseCode_t phase_code() const {
        return phase_code_;
    }

    int32_t pressure_counts() const {
        return pressure_counts_;
    }

    int16_t valve_duration_sec() const {
        return valve_duration_sec_;
    }

protected:

    /* Protected Member Functions: */

    void set_case_vac_after_valve_cinhg(uint16_t case_vac_after_valve_cinhg) {
        case_vac_after_valve_cinhg_ = case_vac_after_valve_cinhg;
    }

    void set_case_vac_before_valve_cinhg(uint16_t case_vac_before_valve_cinhg) {
        case_vac_before_valve_cinhg_ = case_vac_before_valve_cinhg;
    }

    void set_phase_code(XPhaseCode_t phase_code) {
        phase_code_ = phase_code;
    }

    void set_pressure_counts(int32_t pressure_counts) {
        pressure_counts_ = pressure_counts;
    }

    void set_valve_duration_sec(int16_t valve_duration_sec) {
        valve_duration_sec_ = valve_duration_sec;
    }

};

} /* namespace xmsg */
} /* namespace util */

#endif /* XVALVECOUNTS_H_ */
