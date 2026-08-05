/** The @ref argo::iridium::IridiumPosition class header file.
 *
 *  This is the header file for the @ref argo::iridium::IridiumPosition class. This class
 *  represents a position that was acquired through the Iridium system. It represents an
 *  estimate of the position of the instrument and is provided in the body of an email message
 *  received from the Iridium system along the an Iridium Short Burst Data (SBD) message
 *  attachment.
 *
 *  This file contains the class definition and the complete functionality of the class.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:00:20 PM
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

#ifndef IRIDIUMPOSITION_H_
#define IRIDIUMPOSITION_H_

/* Includes: */

// local
#include "IridiumDefinitions.h"

namespace argo {
namespace iridium {

using boost::posix_time::ptime;

/* Class Definition: */

/** The Iridium position class.
 *
 *  This class represents a position that was acquired through the Iridium system. It represents
 *  an estimate of the position of the instrument and is provided in the body of an email message
 *  received from the Iridium system along the an Iridium Short Burst Data (SBD) message
 *  attachment.
 *
 */
class IridiumPosition: protected util::position::Position {

private:

    /* Dynamic Data Members: */

    /** Iridium position Circular Error Probable (CEP) radius, in kilometers.
     *
     *  This field defines the estimate of an error of the Iridium position. It is provided by
     *  the Iridium system, along with the Iridium position, in the body of the email containing
     *  the Iridium SBD message.
     *
     *  Since multiple Iridium messages are transmitted for each dive cycle, a better estimate of
     *  the instrument position is provided by a composite position, constructed from two or more
     *  individual positions. To compute this composite position, each position is weighted
     *  proportionally to the inverse of the CEP radius.
     */
    int iridium_position_cep_radius_;

protected:

    /* Protected Member Functions: */

    /** Set the Iridium position Circular Error Probable (CEP) radius, in kilometers.
     *
     *  @param[in]  iridium_position_cep_radius The Iridium position Circular Error Probable (CEP)
     *                                          radius to set, in kilometers.
     */
    void set_iridium_position_cep_radius(int iridium_position_cep_radius) {
        iridium_position_cep_radius_ = iridium_position_cep_radius;
    }

    /** Set the Iridium position date/time.
     *
     *  @param[in]  iridium_position_datetime   The Iridium position date/time to set.
     */
    void set_iridium_position_datetime(const ptime& iridium_position_datetime) {
        set_position_datetime(iridium_position_datetime);
    }

    /** Set the Iridium position is valid flag.
     *
     *  @param[in]  iridium_position_is_valid   The Iridium position is valid flag to set.
     */
    void set_iridium_position_is_valid(bool iridium_position_is_valid) {
        set_position_is_valid(iridium_position_is_valid);
    }

    /** Set the Iridium position latitude, in decimal degrees.
     *
     *  @param[in]  iridium_position_latitude   The Iridium position latitude to set, in decimal
     *                                          degrees.
     */
    void set_iridium_position_latitude(double iridium_position_latitude) {
        set_position_latitude(iridium_position_latitude);
    }

    /** Set the Iridium position longitude, in decimal degrees.
     *
     *  @param[in]  iridium_position_longitude  The Iridium position longitude to set, in decimal
     *                                          degrees.
     */
    void set_iridium_position_longitude(double iridium_position_longitude) {
        set_position_longitude(iridium_position_longitude);
    }

public:

    /* Public Member Functions: */

    /** The default constructor.
     *
     */
    IridiumPosition() :
            Position(), iridium_position_cep_radius_(IRI_POS_INVALID_CEP) {
        /* Do nothing. */
    }

    /** The destructor.
     *
     */
    ~IridiumPosition() {
        /* Do nothing. */
    }

    /** Get the Iridium position Circular Error Probable (CEP) radius, in kilometers
     *
     *  @return     The Iridium position Circular Error Probable (CEP) radius, in kilometers
     */
    int iridium_position_cep_radius() const {
        return iridium_position_cep_radius_;
    }

    /** Get a formatted string representing the Iridium position composed using the ISO 6709
     *  standard.
     *
     *  @return     The formatted string representation of the Iridium position.
     */
    string iridium_position_as_iso6709_string() const {
        return postition_as_iso6709_string();
    }

    /** Get a formatted string representing the Iridium position date/time composed using the ISO
     *  8601 standard.
     *
     *  @return     The formatted string representation of the Iridium position date/time.
     */
    string iridium_position_datetime_as_iso8601_string() const {
        return position_datetime_as_iso8601_string();
    }

    /** Get the Iridium position date/time.
     *
     *  @return     The Iridium position date/time.
     */
    ptime iridium_position_datetime() const {
        return position_datetime();
    }

    /** Get the Iridium position is valid flag.
     *
     *  @return     TRUE if the Iridium position is valid, or FALSE otherwise.
     */
    bool iridium_position_is_valid() const {
        return position_is_valid();
    }

    /** Get the Iridium position latitude.
     *
     *  @return     The Iridium position latitude.
     */
    double iridium_position_latitude() const {
        return position_latitude();
    }

    /** Get the Iridium position longitude.
     *
     *  @return     The Iridium position longitude.
     */
    double iridium_position_longitude() const {
        return position_longitude();
    }
};

} /* namespace iridium */
} /* namespace argo */

#endif /* IRIDIUMPOSITION_H_ */
