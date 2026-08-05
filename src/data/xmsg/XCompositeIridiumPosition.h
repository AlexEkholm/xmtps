/** The @ref util::xmsg::XCompositeIridiumPosition class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 25, 2014 1:47:14 PM
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

#ifndef XCOMPOSITEIRIDIUMPOSITION_H_
#define XCOMPOSITEIRIDIUMPOSITION_H_

/* Includes */

// local
#include "AbstractXComposite.h"

/* Definitions */

/** Default/fill value for invalid position latitude. */
#define XMSG_POS_INVALID_LAT        UTIL_POS_INVALID_LAT

/** Default/fill value for invalid position longitude. */
#define XMSG_POS_INVALID_LON        UTIL_POS_INVALID_LON

/** Default/fill value for invalid position standard error. */
#define XMSG_POS_INVALID_STDERR     UTIL_POS_INVALID_STDERR

/** Default/fill value for invalid position variance. */
#define XMSG_POS_INVALID_VAR        UTIL_POS_INVALID_VAR

namespace argo {
namespace iridium {
namespace xfloat {
// forward declaration
class XFloatMailMessage;
} /* namespace argo */
} /* namespace iridium */
} /* namespace xfloat */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Class representing the aggregate Iridium position for a single dive, composed from one or more
 *  Iridium message positions.
 */
class XCompositeIridiumPosition:
        public AbstractXComposite<shared_ptr<argo::iridium::xfloat::XFloatMailMessage> >,
        public util::position::Position {
private:

    /* Data Members */

    /** The calculated standard error of this composite Iridium position, in km. */
    double position_standard_error_;

    /** The calculated variance of this composite Iridium position, in km^2. */
    double position_variance_;

    /** The Iridium session communication time, in seconds.
     *
     *  @note           This value is calculated from the difference in time between the first and
     *                  last Iridium messages that are included in this composite Iridium position.
     */
    int total_satellite_comm_time_seconds_;

protected:

    /* Member Functions */

    /** Set the calculated standard error of this composite Iridium position, in km.
     *
     *  @param[in]  position_standard_error The calculated standard error of this composite
     *                                      Iridium position, in km
     */
    void set_position_standard_error(double position_standard_error) {
        position_standard_error_ = position_standard_error;
    }

    /** Set the calculated variance of this composite Iridium position, in km^2.
     *
     *  @param[in] position_variance        The calculated variance of this composite Iridium
     *                                      position, in km^2
     */
    void set_position_variance(double position_variance) {
        position_variance_ = position_variance;
    }

    /** Set the Iridium session communication time, in seconds.
     *
     *  @param[in]  time                    The Iridium session communication time, in seconds.
     */
    void set_total_satellite_comm_time_seconds(int time) {
        total_satellite_comm_time_seconds_ = time;
    }

public:
    /* Member Functions */

    /** The default constructor.
     *
     */
    XCompositeIridiumPosition() :
                    Position(),
                    position_standard_error_(XMSG_POS_INVALID_STDERR),
                    position_variance_(XMSG_POS_INVALID_VAR),
                    total_satellite_comm_time_seconds_(0) {
        /* Do nothing. */
    }

    /** The destructor.
     *
     */
    ~XCompositeIridiumPosition() {
        /* Do nothing. */
    }

    void ComputeAverageIrdiumPosition();

    /** Generate the listing entry comment.
     *
     *  @return         The listing entry comment.
     */
    static const string GenerateListingComment() {
        ostringstream os;

        const format fmt("#   %4s %4s %20s %21s %10s %10s %10s %10s\n"
                         "#    %4s %4s %20s %21s %10s %10s %10s %10s");

        try {
//            os << format(fmt) % "SN" % "dive" % "weighted-avg-time" % "weighted-avg-pos" //
//                    % "nPos" % "variance" % "stdError" % "satTime" //
//                    % "" % "" % "" % "" //
//                    % "" % "(km^2)" % "(km)" % "(sec)";

          os << "#      SN dive    weighted-avg-time    weighted-avg-pos       nPos   variance   stdError    satTime"
             << endl
             << "#                                                                      (km^2)       (km)      (sec)";

        }CATCH_BLOCK("Failed to generate composite iridium position listing comment!")

        return os.str();
    }

    /** Generate a listing entry string from this composite Iridum position object.
     *
     *  @param[in]      whoi        The unique WHOI instrument identifier or serial number.
     *  @param[in]      dive        The dive or cycle number of this composite Iridum position object.
     *
     *  @return         A listing entry string from this composite Iridum position object.
     */
    const string GenerateListingString(int whoi, int dive) const {
        ostringstream os;

        try {
            const format fmt("IRI %5d %4d %20s %19s %10d %10.2f %10.2f %10d");
            os << GenerateListingComment() << endl
                    << format(fmt) % whoi % dive % position_datetime_as_iso8601_string()
                            % postition_as_iso6709_string() % elements().size()
                            % position_variance_ % position_standard_error_ % total_satellite_comm_time_seconds_;

        }CATCH_BLOCK("Failed to generate listing string from composite Iridium position!")

        return os.str();
    }

    /** Get the calculated variance of this composite Iridium position, in km^2.
     *
     *  @return         The calculated variance of this composite Iridium position, in km^2.
     */
    double position_standard_error() const {
        return position_standard_error_;
    }

    /** Get the calculated variance of this composite Iridium position, in km^2.
     *
     *  @return         The calculated variance of this composite Iridium position, in km^2.
     */
    double position_variance() const {
        return position_variance_;
    }

    /** Get the Iridium session communication time, in seconds.
     *
     *  @return         The Iridium session communication time, in seconds.
     */
    int total_satellite_comm_time_seconds() const {
        return total_satellite_comm_time_seconds_;
    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XCOMPOSITEIRIDIUMPOSITION_H_ */
