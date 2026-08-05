/** The @ref argo::iridium::Iridium class header file.
 *
 *  This is the header file for the @ref argo::iridium::Iridium class. This class represents an
 *  individual instrument and its associated unique identifiers. This class is not used explicitly.
 *  Rather, all other classes that are associated with an instrument's unique identifiers inherit
 *  from this class.
 *
 *  This file contains the class definition and the complete functionality of the class.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Apr 3, 2014 2:45:21 PM
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

#ifndef ARGOIRIDIUM_H_
#define ARGOIRIDIUM_H_

/* Includes: */

// local
#include "IridiumDefinitions.h"
#include "../utility/RecursiveLockable.h"

/* Definitions */

namespace argo {
namespace iridium {

/** The Iridium instrument class.
 *
 *  This class represents an individual instrument and its associated unique identifiers. This
 *  class is not used explicitly. Rather, all other classes that are associated with an
 *  instrument's unique identifiers inherit from this class.
 *
 *  @note   This object is lockable. It contains a recursive mutual exclusion (mutex) lock in
 *          order to synchronize access to and modification of objects of this type by various
 *          worker threads. Once the object is locked, any other thread that attempts to access
 *          or modify the object will block until the initial lock is released. Since the mutex
 *          is recursive, the object may be locked multiple times by the same thread without
 *          causing the thread to block indefinitely.
 */
class Iridium:
        public util::thread::RecursiveLockable {

private:

    /* Dynamic Data Members: */

    /** The unique Data Assembly Center (DAC) identifier. */
    int id_dac_;

    /** The unique International Mobile Equipment Identifier (IMEI). */
    long id_imei_;

    /** The unique instrument identifier or instrument serial number. */
    int id_instrument_;

    /** The unique World Meteorological Organization (WMO) identifier. */
    int id_wmo_;

protected:

    /* Protected Member Functions: */

    /** The default constructor.
     *
     */
    Iridium() :
            id_dac_(IRI_ID_UNKNOWN),
            id_imei_(IRI_ID_UNKNOWN),
            id_instrument_(IRI_ID_UNKNOWN),
            id_wmo_(IRI_ID_UNKNOWN) {
        /* Do nothing. */
    }

    /** Set the unique Data Assembly Center (DAC) identifier.
     *
     *  @param[in]  id_dac          The unique Data Assembly Center (DAC) identifier.
     */
    void set_id_dac(int id_dac) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        id_dac_ = id_dac;
    }

    /** Set the unique International Mobile Equipment Identifier (IMEI).
     *
     *  @param[in]  id_imei         The unique International Mobile Equipment Identifier (IMEI).
     */
    void set_id_imei(long id_imei) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        id_imei_ = id_imei;
    }

    /** Set the unique instrument identifier or instrument serial number.
     *
     *  @param[in] id_instrument    The unique instrument identifier or instrument serial number
     */
    void set_id_instrument(int id_instrument) {
        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        id_instrument_ = id_instrument;
    }

    /** Set the unique World Meteorological Organization (WMO) identifier.
     *
     *  @param[in] id_wmo           The unique World Meteorological Organization (WMO) identifier.
     */
    void set_id_wmo(int id_wmo) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        id_wmo_ = id_wmo;
    }

public:

    /* Public Member Functions: */

    /** The parameterized constructor.
     *
     *  @param[in]      imei        The unique International Mobile Equipment Identifier (IMEI).
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      wmo         The unique World Meteorological Organization (WMO) identifier.
     *  @param[in]      dac         The unique Data Assembly Center (DAC) identifier.
     */
    Iridium(int whoi, long imei = IRI_ID_UNKNOWN, int wmo = IRI_ID_UNKNOWN, int dac =
    IRI_ID_UNKNOWN) :
            id_dac_(dac),
            id_imei_(imei),
            id_instrument_(whoi),
            id_wmo_(wmo) {
        /* Do nothing. */
    }

    /** The destructor.
     *
     */
    virtual ~Iridium() {
        /* Do nothing. */
    }

    /** Get the unique Data Assembly Center (DAC) identifier.
     *
     *  @return     The unique Data Assembly Center (DAC) identifier.
     */
    int id_dac() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return id_dac_;
    }

    /** Get the unique International Mobile Equipment Identifier (IMEI).
     *
     *  @return     The unique International Mobile Equipment Identifier (IMEI).
     */
    long id_imei() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return id_imei_;
    }

    /** Get the unique instrument identifier or instrument serial number.
     *
     *  @return     The unique instrument identifier or instrument serial number.
     */
    int id_instrument() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return id_instrument_;
    }

    /** Get the unique World Meteorological Organization (WMO) identifier.
     *
     *  @return     The unique World Meteorological Organization (WMO) identifier.
     */
    int id_wmo() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return id_wmo_;
    }

};

} /* namespace iridium */
} /* namespace argo */

#endif /* ARGOIRIDIUM_H_ */
