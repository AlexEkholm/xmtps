/** The @ref util::position::PositionUtils class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:20:02 PM
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

#ifndef POSITIONUTILITIES_H_
#define POSITIONUTILITIES_H_

/* Includes */

// local
#include "DateUtils.h"
#include "MathUtils.h"
#include "StringUtils.h"

/* Definitions */

/** Default/fill value for invalid position latitude. */
#define UTIL_POS_INVALID_LAT        99.99999

/** Default/fill value for invalid position longitude. */
#define UTIL_POS_INVALID_LON        999.99999

/** Default/fill value for invalid position standard error. */
#define UTIL_POS_INVALID_STDERR     9999.99

/** Default/fill value for invalid position variance. */
#define UTIL_POS_INVALID_VAR        99.99

namespace util {
namespace position {

using boost::format;
using std::ostringstream;
using std::string;

/* Class Definition */

/** Class providing library of useful functions for manipulating and processing geographic
 *  positions.
 *
 */
class PositionUtils: public date::DateUtils, public math::MathUtils {

private:

    /* Data Members */

    /** The equatorial radius of the earth, in kilometers. */
    static const double EARTH_RADIUS_KM_EQUATORIAL;

    /** The polar radius of the earth, in kilometers. */
    static const double EARTH_RADIUS_KM_POLAR;

public:

    /* Member Functions */

    /** Generate an IS06709 string representation of the position given by (@p lat, @p lon).
     *
     *  @param[in]      lat         The latitude of the position, in degrees North.
     *  @param[in]      lon         The longitude of the position, in degrees East.
     *
     *  @return         An IS06709 string representation of the position given by (@p lat, @p lon).
     */
    template<typename T> static std::string GenerateIso6709String(T lat, T lon) {
        ostringstream os;
        static const format fmt("%+.5f%+.5f");
        os << format(fmt) % lat % lon;
        return os.str();
    }

    /** Get the approximate surface altitude or height of earth for a given latitude, in kilometers.
     *
     *  @warning        This method provides approximate surface height estimates only! This
     *                  approximation is calculated from a reference ellipsoid -- a
     *                  mathematically-defined surface that approximates the geoid, the truer
     *                  figure of the Earth.
     *
     *  @param[in]      lat         The geographic latitude of interest, in decimal degrees.
     *
     *  @return         The approximate surface altitude or height of earth for the given latitude,
     *                  in kilometers.
     */
    template<typename T> static double CalculateEllipsoidHeight(T lat) {
        if(lat > 90) lat = 90;
        if(lat < -90) lat = -90;
        return cos(degToRad(lat)) * (EARTH_RADIUS_KM_EQUATORIAL - EARTH_RADIUS_KM_POLAR)
                + EARTH_RADIUS_KM_POLAR;
    }

    /** Get the geographical distance between two position, in kilometers, from the positions'
     *  locations, in decimal degrees.
     *
     *  @warning        This method provides approximate geographic estimates only! This
     *                  approximation is calculated from a reference ellipsoid -- a
     *                  mathematically-defined surface that approximates the geoid, the truer
     *                  figure of the Earth.
     *
     *  @note           This method uses the ‘haversine’ formula to calculate the great-circle
     *                  distance between two points – that is, the shortest distance over the
     *                  earth’s surface – giving an ‘as-the-crow-flies’ distance between the points
     *                  (ignoring any hills they fly over, of course!).
     *
     *  @param[in]      lat1        The latitude of the geographic final position, in decimal
     *                              degrees North.
     *  @param[in]      lon1        The longitude of the geographic final position, in decimal
     *                              degrees East.
     *  @param[in]      lat0        The latitude of the geographic initial position, in decimal
     *                              degrees North.
     *  @param[in]      lon0        The longitude of the geographic initial position, in decimal
     *                              degrees East.
     *
     *  @return         The geographical distance between the two given positions, in kilometers.
     */
    template<typename T> static double CalculateGeographicDistance(T lat1, T lon1, T lat0, T lon0) {
        using namespace std;

        // Convert to radians.
        double phi0 = degToRad(lat0);
        double phi1 = degToRad(lat1);

        // Calculate angular changes.
        double delta_phi = degToRad(lat1 - lat0);
        double delta_lambda = degToRad(lon1 - lon0);

        // Calculate distance.
        double a = sin(delta_phi / 2) * sin(delta_phi / 2)
                + cos(phi0) * cos(phi1) * sin(delta_lambda / 2) * sin(delta_lambda / 2);
        double c = 2 * atan2(sqrt(a), sqrt(1 - a));
        double d = CalculateEllipsoidHeight((lat1 + lat0) / 2) * c;

        return d;
    }

    /** Check position validity.
     *
     * @param[in]       lat         Latitude of position.
     * @param[in]       lon         Longitude of position.
     *
     * @return          TRUE if (lat, lon) represents a valid geographical position, or FALSE
     *                  otherwise.
     */
    template<typename T> static bool IsPositionValid(T lat, T lon) {
        return lat >= -90 && lat <= 90 && lon >= -180 && lon <= 180;
    }
};


} /* namespace position */
} /* namespace util */
#endif /* POSITIONUTILITIES_H_ */
