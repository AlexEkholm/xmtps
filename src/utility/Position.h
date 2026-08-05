/** The @ref util::position::Position class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:19:36 PM
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

#ifndef POSITIONS_H_
#define POSITIONS_H_

/* Includes */

// local
#include "PositionUtils.h"

/* Definitions */

namespace util {
namespace position {

using std::string;
using boost::posix_time::ptime;
using boost::posix_time::hours;
using boost::posix_time::minutes;
using boost::posix_time::seconds;
using boost::gregorian::date;
using boost::gregorian::days;

/** Abstract class representing a position, date, and time.
 *
 */
class Position:
        protected PositionUtils {

private:

    /* Private Data Members: */

    /** The date and time of the position.
     *
     */
    ptime position_datetime_;

    /** The latitude of the position, in decimal degrees.
     *
     */
    double position_latitude_;

    /** The longitude of the position, in decimal degrees.
     *
     */
    double position_longitude_;

    /** The valid position indicator.
     *
     */
    bool position_is_valid_;

protected:

    /* Protected Member Functions: */

    /** Set the date/time of the position.
     *
     *  @param[in]  datetime    The date/time of the position to set.
     */
    void set_position_datetime(const ptime& datetime) {
        position_datetime_ = datetime;
    }

    /** Set the date and time of this position from the given GPS week, day, and time.
     *
     *  @param[in]  gpsWeek     The GPS week, in weeks since midnight of January 6, 1980.
     *  @param[in]  gpsDay      The GPS day of week (0 - 6).
     *  @param[in]  gpsHour     The UTC hour of day (0 - 23).
     *  @param[in]  gpsMinute   The UTC minute of hour (0 - 59)..
     *  @param[in]  gpsSecond   The UTC second of minute (0 - 59).
     */
    void set_gps_position_datetime(
            unsigned int gpsWeek,
            unsigned int gpsDay,
            unsigned int gpsHour = 0,
            unsigned int gpsMinute = 0,
            unsigned int gpsSecond = 0) {
        ptime time(
                date(1980, boost::date_time::Jan, 6),
                hours(gpsHour) + minutes(gpsMinute) + seconds(gpsSecond));
        set_position_datetime(time + days((gpsWeek * 7) + gpsDay));
    }

    /** Set the position is valid flag.
     *
     *  @param[in]  is_valid    TRUE if the position is valid, or FALSE otherwise.
     */
    void set_position_is_valid(bool is_valid = true) {
        position_is_valid_ = is_valid;
    }

    /** Set the latitude of the position, in decimal degrees.
     *
     *  @param[in]  lat         The latitude of the position, in decimal degrees.
     */
    void set_position_latitude(double lat) {
        set_position_is_valid(IsPositionValid(position_latitude_ = lat, position_longitude_));
    }

    /** Set the longitude of the position, in decimal degrees.
     *
     *  @param[in]  lon         The longitude of the position, in decimal degrees.
     */
    void set_position_longitude(double lon) {
        position_longitude_ = lon;
        set_position_is_valid(IsPositionValid(position_latitude_, position_longitude_ = lon));
    }

public:

    /* Public Member Functions: */

    /** The default constructor.
     *
     *  @exception  None. Never throws.
     */
    Position() ATTR_NO_EXCEPTIONS :
                    position_datetime_(bpt::not_a_date_time),
                    position_latitude_(UTIL_POS_INVALID_LAT),
                    position_longitude_(UTIL_POS_INVALID_LON),
                    position_is_valid_(false) {
        /* Do nothing. */
    }

    /** The destructor. */
    virtual ~Position() {
        /* Do nothing. */
    }

    /** Get the date/time of the position.
     *
     *  @return         The date/time of the position.
     */
    ptime position_datetime() const {
        return position_datetime_;
    }

    /** Get a formatted string representing the position date/time composed using the ISO
     *  8601 standard.
     *
     *  @return     The formatted string representation of the position date/time.
     */
    string position_datetime_as_iso8601_string() const {
        return GenerateIso8601String(position_datetime_);
    }

    /** Is the position valid?
     *
     *  @return     TRUE if the position is valid, or false otherwise.
     */
    bool position_is_valid() const {
        return position_is_valid_;
    }

    /** Get the latitude of the position, in decimal degrees.
     *
     *  @return     The latitude of the position, in decimal degrees.
     */
    double position_latitude() const {
        return position_latitude_;
    }

    /** Get the longitude of the position, in decimal degrees.
     *
     *  @return     The longitude of the position, in decimal degrees.
     */
    double position_longitude() const {
        return position_longitude_;
    }

    /** Get a formatted string representing the position composed using the ISO 6709
     *  standard.
     *
     *  @return     The formatted string representation of the position.
     */
    string postition_as_iso6709_string() const {
        return GenerateIso6709String(position_latitude_, position_longitude_);
    }

};

} /* namespace position */
} /* namespace util */

#endif /* POSITIONS_H_ */
