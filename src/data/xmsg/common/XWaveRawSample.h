/** The @ref util::xmsg::XWaveRawSample class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 27, 2014 4:15:28 PM
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

#ifndef XWAVERAWSAMPLE_H_
#define XWAVERAWSAMPLE_H_

/* Includes */

// local
#include "XDefinitions.h"



namespace util {
namespace xmsg {

/** Class representing a single raw motion measurement in the surface time series wave data
 *  X-message.
 *
 *  @note           This class is used for raw data mode only.
 *
 */
class XWaveRawSample {

    /* Friends */

    friend class XWave;

private:

    /** Measured acceleration in the starboard (x'), bow (y'), and upward (z') directions. */
    int16_t accel[3];

    /** Measured magnetism in the starboard (x'), bow (y'), and upward (z') directions.  */
    int16_t magnt[3];

    /** Measured rotational velocity in the gamma (starboard-up), beta (bow-up), and alpha
     *  (increasing azimuth) directions.
     */
    int16_t omega[3];

public:

    /** Default constructor.
     *
     */
    XWaveRawSample() ATTR_NO_EXCEPTIONS {
        /* Do nothing. */
    }

    /** Destructor.
     *
     */
    ~XWaveRawSample() {
        /* Do nothing. */
    }

    /** Generate a listing entry string representation of this raw motion measurement.
     *
     *  @param[in]      assf            The accelerometer sensitivity scale factor, in LSB/g.
     *  @param[in]      gssf            The gyroscope sensitivity scale factor, in LSB/(deg/s).
     *  @param[in]      mssf            The magnetometer sensitivity scale factor, in LSB/uT.
     *
     *  @return         A listing entry string representation of this raw motion measurement.
     */
    const string GenerateListingString(double assf, double gssf, double mssf) const {
        ostringstream os;

        try {
            static const format fmt("%12.6f %12.6f %12.6f %9.1f %9.1f %9.1f %12.6f %12.6f %12.6f");
            os << format(fmt) % (accel[0] / assf) % (accel[1] / assf) % (accel[2] / assf)
                    % (magnt[0] / mssf) % (magnt[1] / mssf) % (magnt[2] / mssf) % (omega[0] / gssf)
                    % (omega[1] / gssf) % (omega[2] / gssf);
        }CATCH_BLOCK("Failed to generate listing string from XWaveRawSample object!")

        return os.str();
    }
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XWAVERAWSAMPLE_H_ */
