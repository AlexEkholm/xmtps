/** The @ref util::xmsg::XGPS class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jun 9, 2014 3:57:31 PM
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
#include "XGPS.h"

namespace util {
namespace xmsg {

/* Static Data Members: */

const util::mysql::SqlTableDescription<XGPS> XGPS::sql_table_description_(
        "XGPS",

        {

        sql_cu16(XGPS, "id_instrument", &XGPS::id_instrument, &XGPS::set_id_instrument, true),

        sql_ci16(XGPS, "id_dive", &XGPS::id_dive, &XGPS::set_id_dive, true),

        sql_cou8(XGPS, "id_type", &XGPS::id_type, &XGPS::set_id_type, true),

        sql_colt(XGPS, "datetime", &XGPS::position_datetime, &XGPS::set_position_datetime),

        sql_colf(
                XGPS,
                "hdop",
                &XGPS::horizontal_dilution_of_precision,
                &XGPS::set_horizontal_dilution_of_precision),

        sql_cold(XGPS, "latitude", &XGPS::position_latitude, &XGPS::set_position_latitude),

        sql_cold(XGPS, "longitude", &XGPS::position_longitude, &XGPS::set_position_longitude),

        sql_cou8(XGPS, "num_satellites", &XGPS::num_satellites, &XGPS::set_num_satellites),

        sql_cou8(XGPS, "snr_min", &XGPS::signal_to_noise_min, &XGPS::set_signal_to_noise_min),

        sql_cou8(XGPS, "snr_mean", &XGPS::signal_to_noise_mean, &XGPS::set_signal_to_noise_mean),

        sql_cou8(XGPS, "snr_max", &XGPS::signal_to_noise_max, &XGPS::set_signal_to_noise_max),

        sql_cu16(XGPS, "time_to_fix", &XGPS::time_to_acquire_fix, &XGPS::set_time_to_acquire_fix),

        sql_colb(XGPS, "valid", &XGPS::position_is_valid, &XGPS::set_position_is_valid),

        });

/* Function Definitions: */

} /* namespace xmsg */
} /* namespace util */
