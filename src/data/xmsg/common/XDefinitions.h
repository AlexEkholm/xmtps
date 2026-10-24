/** The XDefinitions header file.
 *
 *  This file includes common preprocessor definitions and macros that are used by the X-Message
 *  classes.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Aug 14, 2014 11:37:15 AM
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

#ifndef XINCLUDES_H_
#define XINCLUDES_H_

/* Includes: */

#include "../../IridiumDefinitions.h"               // local libs
#include "../../../config/LocalPersistentEntity.h"
#include "../../../utility/ContainerUtils.h"
#include "../../../utility/KmlUtil.h"

#include <boost/date_time/posix_time/ptime.hpp>     // boost libs
#include <boost/format/format_class.hpp>
#include <boost/format/format_fwd.hpp>
#include <boost/format/format_implementation.hpp>
#include <boost/format/free_funcs.hpp>

#include <cstdint>                                  // std libs
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <bit>

/* Definitions */

/** The default/bad/invalid/fill/unknown value for fields of type float. */
#define XMSG_BAD_FLT                            FLT_MIN

/** The default/bad/invalid/fill/unknown value for fields of type double. */
#define XMSG_BAD_DBL                            DBL_MIN

/** The default/bad/invalid/fill/unknown value for fields of type int8_t. */
#define XMSG_BAD_I08                            INT8_MIN

/** The default/bad/invalid/fill/unknown value for fields of type int16_t. */
#define XMSG_BAD_I16                            INT16_MIN

/** The default/bad/invalid/fill/unknown value for fields of type int32_t. */
#define XMSG_BAD_I32                            INT32_MIN

/** The default/bad/invalid/fill/unknown value for fields of type uint8_t. */
#define XMSG_BAD_U08                            UINT8_MAX

/** The default/bad/invalid/fill/unknown value for fields of type uint16_t. */
#define XMSG_BAD_U16                            UINT16_MAX

/** The default/bad/invalid/fill/unknown value for fields of type uint32_t. */
#define XMSG_BAD_U32                            UINT32_MAX

/** The debug-alamo-mission-configuration flag.
 *
 *  If TRUE, then additional debug messages will be reported when decoding ALAMO mission
 *  configuration messages.
 */
#define XMSG_DEBUG_ALAMO_MISS_GFG               (false)

/** The debug-bist flag.
 *
 *  If TRUE, then additional debug messages will be reported when decoding MFA mission
 *  configuration messages.
 */
#define XMSG_DEBUG_BIST                         (true)

/** The debug-MFA-mission-configuration flag.
 *
 *  If TRUE, then additional debug messages will be reported when decoding MFA mission
 *  configuration messages.
 */
#define XMSG_DEBUG_MFA_MISS_GFG                 (true)

/** The debug-aggregation-of-time-series-data flag.
 *
 *  If TRUE, then additional debug messages will be reported when decoding aggregate time-series
 *  data messages.
 */
#define XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA    (false)

/** The debug-surface-engineering flag.
 *
 */
#define XMSG_DEBUG_ENG_SURF                     (false)

/** The debug-curvature-encoded-sensor-data flag.
 *
 *  If TRUE, then additional debug messages will be reported when decoding curvature-encoded sensor
 *  data.
 */
#define XMSG_DEBUG_SENSOR_CURVATURE             (false)

/** The debug-surface-data flag.
 *
 *  If TRUE, then additional debug messages will be reported when decoding surface data.
 */
#define XMSG_DEBUG_SURF_PRES_AGGR_DATA          (false)

/** The default/bad/invalid dive value. */
#define XMSG_DIVE_UNKNOWN                       IRI_DIVE_UNKNOWN

/** Preprocessor switch enable multiple profile data message with the same unique message
 *  identifier to be included in the same dive data object.
 *
 *  @note           This switch is a temporary fix to include the full profile data in the
 *                  intermediate listing for ALAMO #9038 which erroneously returns profile data
 *                  messages with non-unique identifiers.
 *
 *  @warning        This switch should be disabled for the release build configuration.
 *
 *  If TRUE, then multiple profile data message with the same unique message identifier will be
 *  allowed for a single dive. Otherwise, messages with duplicate unique identifiers will
 *  overwrite or replace previously received messages with the same identifier.
 */
#define XMSG_ENABLE_MULTI_PROF                  (false)

/** Default/fill value for invalid Iridium message gain value. */
#define XMSG_GAIN_INVALID                       IRI_GAIN_INVALID

/** The default/bad/invalid unique identifier value. */
#define XMSG_ID_UNKNOWN                         IRI_ID_UNKNOWN

/** The number of samples to reserve upon construction of the composite sensor data objects.
 *
 */
#define XMSG_INIT_SAM_N_RESERVE                 (1024)

/** Default/fill value for invalid Iridium message offset value. */
#define XMSG_OFFS_INVALID                       IRI_OFFS_INVALID

/** The default/bad/invalid ALAMO engineering message version. */
#define XMSG_ENG_BAD_VER                        XMSG_BAD_U08

/** The minimum allowable number of bytes in an SBD message.
 *
 *  Any SBD message that has less than this number of bytes will be considered invalid and ignored.
 *
 */
#define XMSG_SBD_MIN_BYTES                      (8)

/** The default/bad/invalid sensor counts fill value. */
#define XMSG_SENS_BAD                           (-99999)

/** The default/bad/invalid pressure sensor fill value. */
#define XMSG_SENS_BAD_PRES                      (-9.999)

/** Format a pressure for listing files.
 *
 *  Pressures are listed with two decimal places. The fill value is still listed as -9.999, so it
 *  stays distinguishable from a measured value.
 *
 *  @param[in]  p       The pressure, in dBar.
 *
 *  @return     The formatted pressure (without padding; use a %Ns field for the column width).
 */
inline std::string XMsgFormatPressure(double p) {
    std::ostringstream os;
    if(p > XMSG_SENS_BAD_PRES - 0.0005 && p < XMSG_SENS_BAD_PRES + 0.0005)
        os << "-9.999";
    else
        os << std::fixed << std::setprecision(2) << p;
    return os.str();
}

/** The default/bad/invalid salinity sensor fill value. */
#define XMSG_SENS_BAD_SALN                      (-9.999)

/** The default/bad/invalid temperature sensor fill value. */
#define XMSG_SENS_BAD_TEMP                      (-9.999)

/** The minimum valid pressure sensor value. */
#define XMSG_SENS_MIN_PRES                      (-5.000)

/** The minimum valid salinity sensor value. */
#define XMSG_SENS_MIN_SALN                      (2.000)

/** The minimum valid temperature sensor value. */
#define XMSG_SENS_MIN_TEMP                      (-5.0)

/** The maximum valid pressure sensor value. */
#define XMSG_SENS_MAX_PRES                      (2610.000)

/** The maximum valid salinity sensor value. */
#define XMSG_SENS_MAX_SALN                      (41.000)

/** The maximum number of samples for a single instrument, sensor, and dive cycle.
 *
 */
#define XMSG_SENS_MAX_SAMPLES                   (1024*16)

/** The maximum number of sensor data samples in a profile data message sub-block. */
#define XMSG_SENS_MAX_SUB                       (25)

/** The maximum valid temperature sensor value. */
#define XMSG_SENS_MAX_TEMP                      (40.000)

/** The maximum number of drift data messages that may be received for a single sensor (e.g.
 *  pressure, temperature, or salinity) and for a single dive.
 */
#define XMSG_MAX_DRFT                           (8)

/** The maximum number of fall data messages that may be received for a single dive. */
#define XMSG_MAX_FALL                           (16)

/** The maximum number of email/SBD data messages that may be received for a single dive. */
#define XMSG_MAX_MAIL                           (256)

/** The maximum number of mission configuration messages that may be received for a single dive. */
#define XMSG_MAX_MISS                           (16)

/** The maximum number of mission configuration messages that may be received for a single dive. */
#define XMSG_MAX_MFA_MISS                       ( 2 )

#if defined(XMSG_ENABLE_MULTI_PROF) && XMSG_ENABLE_MULTI_PROF

/** The maximum number of GPS position messages that may be received for a single dive. */
#   define XMSG_MAX_GPS                         (1024)

/** The maximum number of binned profile messages that may be received for a single sensor (e.g.
 *  pressure, temperature, or salinity) and for a single dive.
 */
#   define XMSG_MAX_PROF                        (256)

/** The maximum number of unbinned or row profile messages that may be received for a single sensor
 *  (e.g. pressure, temperature, or salinity) and for a single dive.
 */
#   define XMSG_MAX_PROF_HIRES                  (256)

#else

/** The maximum number of GPS position messages that may be received for a single dive. */
#   define XMSG_MAX_GPS                         (1024)

/** The maximum number of binned profile messages that may be received for a single sensor (e.g.
 *  pressure, temperature, or salinity) and for a single dive.
 */
#   define XMSG_MAX_PROF                        (16)

/** The maximum number of unbinned or row profile messages that may be received for a single sensor
 *  (e.g. pressure, temperature, or salinity) and for a single dive.
 */
#   define XMSG_MAX_PROF_HIRES                  (8)

#endif // defined(XMSG_ENABLE_MULTI_PROF) && XMSG_ENABLE_MULTI_PROF

/** The maximum number of pump data messages that may be received for a single dive. */
#define XMSG_MAX_PUMP                           (16)

/** The maximum number of rise data messages that may be received for a single dive. */
#define XMSG_MAX_RISE                           (16)

/** The maximum number of surface pressure data messages that may be received for a single dive. */
#define XMSG_MAX_SURF_PRES                      (16)

/** The maximum number of valve data messages that may be received for a single dive. */
#define XMSG_MAX_VALVE                          (16)

/** The maximum number of ocean surface wave data messages that may be received for a single dive.
 *
 */
#define XMSG_MAX_WAVE                           (256)

/** The unknown X-Message version value.
 *
 */
#define XMSG_VERSION_UNKNOWN                    (255)

/* Macro Definitions: */

/** Macro for stupid PHY file format lines. */
#define XMSG_PHY(os, name, value)   os << boost::format("%-39s %s") % name % value << "\n"

/** Macro to generate a section divider for ASCII listing files. */
#define XMSG_SECTION(x)\
do {\
    static const char* _line = "#-----------------------------------------------------------------"\
        "----------------------------------\n";\
    static const char* _sec = "# " x "\n";\
    os << _line << _sec  << _line ;\
} while(0)

/** Macro to generate a section break for ASCII listing files. */
#define XMSG_SECTION_BREAK()\
do {\
    static const char* _line = "#-----------------------------------------------------------------"\
        "----------------------------------\n";\
    os << _line;\
} while(0)

/** Macro to generate a section sub-header for ASCII listing files. */
#define XMSG_HEADER2(x)\
do {\
    static const char* _line = "#================================================================="\
        "==================================\n";\
    static const char* _sec = "# " x "\n";\
    os << _line  << _sec  << _line ;\
} while(0)

/** Macro to generate a section header for ASCII listing files. */
#define XMSG_HEADER(x)\
do {\
    static const char* _line = "#===================================================================================================";\
    os << _line << std::endl << "# " << x << std::endl << _line << std::endl;\
} while(0)

/** Get unsigned one byte [n:n] value from array x. */
#define u08(x,n)    ((uint8_t)x[n])

/** Get unsigned two byte [n:n+1] value from array x. */
#define u16(x,n)    (((uint16_t)u08(x,(n)) <<  8) | (uint16_t)u08(x,(n)+1))

/** Get unsigned three byte [n:n+2] value from array x. */
#define u24(x,n)    (((uint32_t)u08(x,(n)) << 16) | (uint32_t)(u08(x,(n)+1) << 8) | (uint32_t)u08(x,(n)+2))

/** Get unsigned four byte [n:n+3] value from array x. */
#define u32(x,n)    (((uint32_t)u16(x,(n)) << 16) | (uint32_t)u16(x,(n)+2))

/** Get signed one byte [n:n] value from array x. */
#define i08(x,n)    (( int8_t)x[n])

/** Get signed two byte [n:n+2] value from array x. */
#define i16(x,n)    ((int16_t)u16(x,n))

/** Get signed three byte [n:n+2] value from array x. */
#define i24(x,n)    (u24(x,n) >= (1<<23) ? ((int32_t)u24(x,n)) - (1 << 24) : u24(x,n))

/** Get signed four byte [n:n+3] value from array x. */
#define i32(x,n)    ((int32_t)u32(x,n))

/** Get posix time from given seconds since 2000-01-01T00:00:00Z. */
#define s2000(x,n)  (u32(x,n) ? (boost::posix_time::ptime(\
                        boost::gregorian::date(2000,boost::date_time::Jan,1),\
                        boost::posix_time::seconds((long int)u32(x,n))))\
                        : (boost::posix_time::ptime(boost::posix_time::not_a_date_time)))

/** Get posix time from given seconds since 1970-01-01T00:00:00Z. */
#define s1970(x,n)  (u32(x,n) ? (boost::posix_time::ptime(\
                        boost::gregorian::date(1970,boost::date_time::Jan,1),\
                        boost::posix_time::seconds((long int)u32(x,n))))\
                        : (boost::posix_time::ptime(boost::posix_time::not_a_date_time)))

//#define f32(x,n)    (*((float *)((void *)(&x[n]))))

#define f32(x,n)    util::math::MathUtils::bitcastUint32ToFloat(u32(x,n))

namespace util {
namespace xmsg {

/* Namespace Declarations: */

using boost::algorithm::trim;
using boost::container::flat_map;
using boost::container::static_vector;
using boost::filesystem::exists;
using boost::filesystem::file_size;
using boost::filesystem::ifstream;
using boost::filesystem::is_empty;
using boost::filesystem::is_regular_file;
using boost::filesystem::ofstream;
using boost::filesystem::path;
using boost::format;
using boost::gregorian::date;
using boost::lexical_cast;
using std::lock_guard;
using std::make_shared;
using boost::posix_time::hours;
using boost::posix_time::minutes;
using boost::posix_time::not_a_date_time;
using boost::posix_time::ptime;
using boost::posix_time::seconds;
using boost::posix_time::time_duration;
using std::recursive_mutex;
using std::shared_ptr;
using std::cmatch;
using std::dec;
using std::endl;
using std::for_each;
using std::get;
using std::hex;
using std::istream;
using std::istringstream;
using std::make_tuple;
using std::max;
using std::min;
using std::noshowbase;
using std::ostream;
using std::ostringstream;
using std::setfill;
using std::setw;
using std::smatch;
using std::string;
using std::stringstream;
using std::tuple;
using std::regex;
using std::regex_match;
using std::regex_replace;
using std::regex_search;
using std::uppercase;
using std::vector;

} /* namespace util */
} /* namespace xmsg */

#endif /* XINCLUDES_H_ */
