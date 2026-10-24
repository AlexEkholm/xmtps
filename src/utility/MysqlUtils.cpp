/** The @ref util::mysql::MysqlUtils class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:19:03 PM
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

/* Includes */

// header
#include "MysqlUtils.h"

namespace util {
namespace mysql {

/* Static Members */

/** @var        util::mysql::MysqlUtils::sql_connection_
 *  @brief      Thread-specific MySQL connection, released by @ref MysqlUtils::CleanupSqlConnection()
 *              when the thread exits.
 */
thread_specific_ptr<Connection> MysqlUtils::sql_connection_(&CleanupSqlConnection);

/** @var        util::mysql::MysqlUtils::sql_driver_
 *  @brief      Thread-specific reference to the MySQL driver. Its cleanup function,
 *              @ref MysqlUtils::CleanupSqlDriver(), does not delete the shared driver instance.
 */
thread_specific_ptr<MySQL_Driver> MysqlUtils::sql_driver_(&CleanupSqlDriver);

} /* namespace mysql */
} /* namespace util */
