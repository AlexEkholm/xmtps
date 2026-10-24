/** The @ref util::xmsg::XPresCodeCounts class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 8, 2014 10:47:35 AM
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

#ifndef XPRESCOUNTCODES_H_
#define XPRESCOUNTCODES_H_

/* Includes: */

// local
#include "XPresCounts.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Type Definitions: */

/** The X-message phase codes enumeration.
 *
 *  The phase code is used to indicate the context of a measurement i.e. what phase of the dive
 *  cycle the measurement was taken.
 *
 */
typedef enum XPhaseCode {

    /** Phase code 0: unknown mission phase.
     *
     *  This code is only used for instruments with an X-Message format previous to version 2.0.
     *
     */
    XPHASE_CODE_UNKNOWN = 0,

    /** Phase code 1: start of the descent-from-surface phase of the dive cycle.
     *
     */
    XPHASE_CODE_START_OF_SINK = 1,

    /** Phase code 2: first pump (Tlastx) at 100 dBar.
     *
     */
    XPHASE_CODE_BUOYANCY_AT_100DBAR = 2,

    /** Phase code 3: seek during the descent-to-drift/park-pressure.
     *
     */
    XPHASE_CODE_SEEK = 3,

    /** Phase code 4: the beginning of the drift phase of the dive cycle.
     *
     */
    XPHASE_CODE_BEGINNING_OF_DRIFT = 4,

    /** Phase code 5: seek to drift/park pressure during the drift phase of the dive cycle.
     *
     */
    XPHASE_CODE_SEEK_DURING_DRIFT = 5,

    /** Phase code 6: beginning of the descent from drift/park depth to profile depth.
     *
     */
    XPHASE_CODE_BEGINING_OF_FALL_TO_PROFILE = 6,

    /** Phase code 7: start of the ascent/profile/rise phase of the dive cycle.
     *
     */
    XPHASE_CODE_START_OF_RISE = 7,

    /** Phase code 8: end of the ascent/profile/rise phase of the dive cycle.
     *
     */
    XPHASE_CODE_END_OF_RISE = 8,

    /** Phase code 9: turn-around due to the ice detection algorithm.
     *
     */
    XPHASE_CODE_ICE_TURNAROUND = 9,

    /** Phase code 10: the descent-from-surface phase of the dive cycle.
     *
     */
    XPHASE_CODE_SINKING = 10,

    /** Phase code 11: the drift phase of the dive cycle.
     *
     */
    XPHASE_CODE_DRIFTING = 11,

    /** Phase code 12: the descent from drift/park depth to profile depth.
     *
     */
    XPHASE_CODE_FALLING_TO_PROFILE = 12,

    /** Phase code 13:  the ascent/profile/rise phase of the dive cycle.
     *
     */
    XPHASE_CODE_RISING = 13,

    /** Phase code 14: the surface phase of the dive cycle.
     *
     */
    XPHASE_CODE_SURFACE = 14,

    /** Phase code 15: invalid.
     *
     *  This code is not used.
     *
     */
    XPHASE_CODE_INVALID = 15
} XPhaseCode_t;

/* Class Definition: */

/** Class representing a raw pressure measurement and phase code for a rise or fall data X-Message.
 *
 */
class XPresCodeCounts:
        public XPresCounts {

private:

    /* Data Members: */

    /** The phase code names.
     *
     */
    static const char* phase_code_names_[];

    /** The phase code of this pressure measurement. */
    XPhaseCode_t phase_code_;

public:

    /* Member Functions: */

    /** The constructor.
     *
     *  @param[in]      sec         The elapsed time since the reference time, in seconds.
     *  @param[in]      code        The phase code of this pressure measurement.
     *  @param[in]      counts      The pressure measurement, in raw sensor counts.
     */
    XPresCodeCounts(uint16_t sec, XPhaseCode_t code, int32_t counts) :
                    XPresCounts(sec, counts),
                    phase_code_(code) {
        /* Do nothing. */
    }

    /** The destructor.
     *
     */
    ~XPresCodeCounts() {
        /* Do nothing. */
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
            const format fmt(" %9d # %s");
            os << XPresCounts::GenerateListingString(pg, po, pc)
               << format(fmt) % (int) phase_code_ % GetPhaseCodeName(phase_code_);
        }CATCH_BLOCK("Failed to generate listing string for pressure-code-counts object!")

        return os.str();
    }

    /** Get the phase code name C-string for a given phase code value.
     *
     *  @param[in]  code    The phase code.
     *
     *  @return     The phase code name C-string for the given phase code value.
     */
    static const char* GetPhaseCodeName(XPhaseCode_t code) {

        try {

            if(code > 15)
                EXC_THROW("Unsupported phase code: " + lexical_cast<string>((int)code) + "!");

            return phase_code_names_[code];

        }CATCH_BLOCK("Failed to get phase code name!")

        return NULL;
    }

    /** Get the phase code.
     *
     *  @return     The phase code.
     */
    XPhaseCode_t phase_code() const {
        return phase_code_;
    }

    /** Get the phase code name C-sting.
     *
     *  @return     The phase code name C-sting.
     */
    const char* phase_code_name() const {
        return GetPhaseCodeName(phase_code_);
    }
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XPRESCOUNTCODES_H_ */
