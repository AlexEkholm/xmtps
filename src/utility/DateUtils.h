/** The @ref util::date::DateUtils class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 18, 2014 1:20:01 PM
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

#ifndef DATEUTILITIES_H_
#define DATEUTILITIES_H_

/* Includes */

// local
#include "Log.h"

// boost
#include <boost/date_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/chrono.hpp>
namespace bpt = boost::posix_time;
namespace bgt = boost::gregorian;

namespace util {
namespace date {

using std::ostringstream;
using std::string;
using boost::posix_time::not_a_date_time;
using boost::posix_time::ptime;
using boost::posix_time::second_clock;
using boost::posix_time::time_duration;
using boost::posix_time::to_iso_extended_string;
using boost::posix_time::to_iso_string;

/* Class Definitions */

/** Class providing library of useful date and time functions.
 *
 */
class DateUtils {
public:

    DateUtils() {
      /* Do nothing. */
    }

    virtual ~DateUtils() {
      /* Do nothing. */
    }

    /* Member Functions */

    static double GenerateJulianDay(const ptime& dt, const ptime& ref_dt) {

        double juld = 0;
        long td_msec = 0;

        try {

            if(dt.is_not_a_date_time())
                EXC_THROW("Invalid date/time!");
            if(ref_dt.is_not_a_date_time())
                EXC_THROW("Invalid reference date/time!");

            time_duration td = dt - ref_dt;
            td_msec = td.total_milliseconds();
            juld = (double)td_msec / (double) 1000 / (double) 3600 / (double) 24;

        }CATCH_BLOCK("Failed to convert ptime to Julian day!")

        return juld;

    }

    /** Generate an ISO8601 format date/time string representation of a POSIX time object.
     *
     *  @note           All date/time objects use the UTC time-zone.
     *
     *  @param[in]      p           POSIX time from which to generate string.
     *
     *  @return         An ISO8601 format date/time string representation of the POSIX time object. If
     *                  p is equal to boost::posix_time::not_a_date_time, then the string
     *                  "0000-00-00T00:00:00Z" will be returned.
     */
    static string GenerateIso8601String(const ptime& p, bool extended = true) {
        static const string not_a_datetime_string_extended = "0000-00-00T00:00:00Z";
        static const string not_a_datetime_string = "00000000000000";

        ostringstream oss;

        if(extended) {
            if(p.is_not_a_date_time())
                oss << not_a_datetime_string_extended;
            else
                oss << to_iso_extended_string(p) << 'Z';
        } else {
            if(p.is_not_a_date_time())
                oss << not_a_datetime_string;
            else
                oss << to_iso_string(p);
        }

        return oss.str();
    }

    /** Generate an ISO8601 format date/time string representation of the current time, in UTC.
     *
     *  @note           All date/time objects use the UTC time-zone.
     *
     *  @return         An ISO8601 format date/time string representation of the current time.
     */
    static string GenerateCurrentUtcTimeIso8601String() {
        return GenerateIso8601String(GetCurrentUtcTime());
    }

    /** Generate a POSIX time object representing the current date and time (rounded to the nearest
     *  second).
     *
     *  @return         A POSIX time object representing the current date and time (rounded to the
     *                  nearest second).
     */
    static ptime GetCurrentUtcTime() {
        return second_clock::universal_time();
    }
};

} /* namespace date */
} /* namespace util */
#endif /* DATEUTILITIES_H_ */
