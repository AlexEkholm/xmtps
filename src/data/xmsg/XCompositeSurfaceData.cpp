/** The @ref util::xmsg::XCompositeSurfaceData class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jun 22, 2016 9:15:25 AM
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

/* Includes: */

// header
#include "XCompositeSurfaceData.h"

namespace util {
namespace xmsg {

/* Static Data Members: */

template<> const int32_t AbstractXTimeSeriesDataComposite<XSurfaceData>::DERIVED_FIELDS_STATIC_GAIN_EXP = 10;

} /* namespace xmsg */
} /* namespace util */
