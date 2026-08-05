/** The @ref util::math::MathUtils class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 18, 2014 1:24:27 PM
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

#ifndef MATHUTILS_H_
#define MATHUTILS_H_

/* Includes */

// local
#include "SystemUtils.h"

/* Definitions */

namespace util {
namespace math {

/* Class Definitions */

/** Class containing commonly-used mathematical functions.
 *
 */
class MathUtils {
public:

    MathUtils() {
      /* Do nothing. */
    }

    virtual ~MathUtils() {
      /* Do nothing. */
    }

    /** Convert angular units from from degrees to decimal radians.
     *
     *  @param[in]      deg         Angle in integer or decimal degrees to convert to radians.
     *
     *  @return         Angle in decimal radians.
     */
    template <typename T> static double degToRad(T deg) {
        return (double)deg*M_PI/180.;
    }

    static inline float bitcastUint32ToFloat(uint32_t u) {

      void* x = &u;

      return *((float *) x);

    }


};

} /* namespace math */
} /* namespace util */
#endif /* MATHUTILS_H_ */
