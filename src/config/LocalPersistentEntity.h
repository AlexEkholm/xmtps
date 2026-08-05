/** The @ref util::mysql::LocalPersistentEntity class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 14, 2014 1:14:55 PM
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

#ifndef LOCALPERSISTENTENTITY_H_
#define LOCALPERSISTENTENTITY_H_

/* Includes */

// local
#include "../utility/SqlTableDescription.h"

namespace util {
namespace mysql {

/* Class Definition */

/** An abstract class representing a persistent database entity in the database defined by the
 *  @ref config::LocalConfiguration.
 *
 */
class LocalPersistentEntity: public LocalMysqlConfiguration{

public:

    /* Public Member Functions: */

    /** Default constructor.
     *
     */
    LocalPersistentEntity() {
        /* Do nothing. */
    }

    /** The destructor.
     *
     */
    virtual ~LocalPersistentEntity() {
        /* Do nothing. */
    }

    /** Persist this database entity.
     *
     */
    virtual void SaveObjectToDatabase() const = 0;

};

} /* namespace mysql */
} /* namespace util */
#endif /* LOCALPERSISTENTENTITY_H_ */
