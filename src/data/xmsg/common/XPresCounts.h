/** The @ref util::xmsg::XPresCounts class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 8, 2014 10:42:06 AM
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

#ifndef XPRESCOUNTS_H_
#define XPRESCOUNTS_H_

/* Includes: */

// local
#include "XDefinitions.h"

namespace util {
namespace xmsg {

/** Class representing a raw pressure measurement for a rise or fall data X-Message.
 *
 */
class XPresCounts {

    /* Friends Classes: */

    friend class XDiveData;

private:

    /* Data Members: */

    /** The elapsed time since the reference time, in seconds.
     *
     */
    uint16_t elapsed_seconds_since_reference_time_;

    /** The pressure measurement, in raw sensor counts.
     *
     */
    int32_t pressure_counts_;

public:

    /* Member Functions: */

    /** The parameterized constructor.
     *
     *  @param[in]      sec         The elapsed time since the reference time, in seconds.
     *  @param[in]      counts      The pressure measurement, in raw counts.
     */
    XPresCounts(uint16_t sec, int32_t counts) :
                    elapsed_seconds_since_reference_time_(sec),
                    pressure_counts_(counts) {
        /* Do nothing. */
    }

    /** The destructor
     *
     */
    ~XPresCounts() {
        /* Do nothing. */
    }

    /** Get the elapsed time since the reference time, in seconds.
     *
     *  @return     The elapsed time since the reference time, in seconds.
     */
    uint16_t elapsed_seconds_since_reference_time() const {
        return elapsed_seconds_since_reference_time_;
    }

    /** Generate a listing entry string from this pressure measurement object.
     *
     *  @param[in]  pg      The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po      The pressure sensor offset, in dBar.
     *  @param[in]  pc      The pressure correction, in dBar, to be subtracted computed pressure
     *                      value.
     *
     *  @throw      log::MessageException if an error occurs while generating the listing string.
     *
     *  @return     A listing entry string representation of this pressure measurement object.
     */
    const string GenerateListingString(uint16_t pg, uint16_t po, float pc) const {
        ostringstream os;
        try {
            const format fmt("%+20d %8.3f");
            os << format(fmt) % (int) elapsed_seconds_since_reference_time_
                    % PressureValue(pg, po, pc);
        }CATCH_BLOCK("Failed to generate listing string for pressure-counts object!")
        return os.str();
    }

    int32_t pressure_counts() const {
        return pressure_counts_;
    }

    /** Get the measured pressure value, in dBar.
     *
     *  @param[in]  pg          The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po          The pressure sensor offset, in dBar.
     *  @param[in]  pres_corr   The pressure correction, in dBar, to be subtracted computed
     *                          pressure value.
     *
     *  @return     The measured pressure value, in dBar.
     */
    template<typename T = float> T PressureValue(uint16_t pg, uint16_t po, float pres_corr) const {
        T val = pg ? (T) pressure_counts_ / (T) pg - (T) po - pres_corr: (T) XMSG_SENS_BAD_PRES;

        return (val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES) ? (T) XMSG_SENS_BAD_PRES : val;
    }


};

} /* namespace xmsg */
} /* namespace util */
#endif /* XPRESCOUNTS_H_ */
