/** The XFloat Definitions header file.
 *
 *  This file contains definitions that are used by the @ref argo::iridium::xfloat::XFloat class
 *  and the classes that are associated with it.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jun 9, 2014 4:12:18 PM
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

#ifndef XFLOATDEFINITIONS_H_
#define XFLOATDEFINITIONS_H_

/* Includes: */

#include "Iridium.h"                              // local
#include "IridiumPosition.h"
#include "xmsg/XAcousticPosition.h"
#include "xmsg/XArgo.h"
#include "xmsg/XCompositeAuxTemp.h"
#include "xmsg/XCompositeDrift.h"
#include "xmsg/XCompositeFall.h"
#include "xmsg/XCompositeHiResProfile.h"
#include "xmsg/XCompositeIridiumPosition.h"
#include "xmsg/XCompositeProfile.h"
#include "xmsg/XCompositePump.h"
#include "xmsg/XCompositeRise.h"
#include "xmsg/XCompositeSurfaceData.h"
#include "xmsg/XCompositeValve.h"
#include "xmsg/XEngAbort.h"
#include "xmsg/XEngAscent.h"
#include "xmsg/XEngBITFail.h"
#include "xmsg/XEngBITPass.h"
#include "xmsg/XEngDescent.h"
#include "xmsg/XEngDescent.h"
#include "xmsg/XEngDiagnostic.h"
#include "xmsg/XEngDiagnostic.h"
#include "xmsg/XEngDrift.h"
#include "xmsg/XEngIce.h"
#include "xmsg/XEngStandard.h"
#include "xmsg/XEngSurface.h"
#include "xmsg/XGPS.h"
#include "xmsg/XMfaMissionCfg.h"
#include "xmsg/XMissionCmdEcho.h"
#include "xmsg/XWave.h"
//#include "../utility/NetcdfUtils.h"
#include <vmime/vmime.hpp>                        // vmime
#include <vmime/platforms/posix/posixHandler.hpp>

/* Definitions: */

/** The default/bad/invalid dive value. */
#define XFLT_DIVE_UNKNOWN                       XMSG_DIVE_UNKNOWN

/** Default/fill value for invalid Iridium message gain value. */
#define XFLT_GAIN_INVALID                       XMSG_GAIN_INVALID

/** The default/bad/invalid unique identifier value. */
#define XFLT_ID_UNKNOWN                         XMSG_ID_UNKNOWN

/** Default/fill value for invalid Iridium message offset value. */
#define XFLT_OFFS_INVALID                       XMSG_OFFS_INVALID

/** Default/fill value for invalid Iridium message offset value. */
#define XFLT_PID_INVALID                        (-1)

/** The default/fill value for SBD data size, in bytes.
 *
 */
#define XFLT_SBD_SIZE_BYTES_UNKNOWN             (-1)

/** The default/fill value for the Iridium session status code.
 *
 */
#define XFLT_SBD_SESSION_STATUS_CODE_UNKNOWN    (-1)

/** Default/fill value for invalid position latitude. */
#define XFLT_POS_INVALID_LAT                    IRI_POS_INVALID_LAT

/** Default/fill value for invalid position longitude. */
#define XFLT_POS_INVALID_LON                    IRI_POS_INVALID_LAT

/** Default mail message processing queue capacity. */
#define XFLT_QUEUE_SIZE_MAIL_PROCESSING         (1024*8)

/** Default telemetry decode queue capacity. */
#define XFLT_QUEUE_SIZE_TELEMETRY               (1024*8)

#define XFLT_QUEUE_SIZE_PPROCESSING             (1024*64)

#define XFLT_QUEUE_SIZE_NC_FILE_WRITER          (1024*64)

/** XFloat service threads' idle sleep time, in milliseconds. */
#define XFLT_THREAD_SLEEP_MS                    (100)

/** Interval at which a mail message is retried while the SQL server is unavailable, in
 *  milliseconds. */
#define XFLT_SQL_RETRY_MS                       (10000)

/** The interval at which the daemon tread will flush the global logger, in seconds.
 *
 */
#define XFLT_DAEMON_FLUSH_LOG_SEC               (30)

#define XFLT_DAEMON_THREAD_SPINUP_DELAY_SEC     (10)

/*  Namespace Definitions: */
namespace argo {
namespace iridium {
namespace xfloat {

using boost::container::flat_map;
using boost::container::static_vector;
using boost::program_options::options_description;
using boost::this_thread::sleep;
using boost::thread_group;
using boost::thread;
using boost::cmatch;
using boost::smatch;
using boost::regex;
using boost::regex_match;
using boost::regex_search;

//using netCDF::NcAtt;
//using netCDF::NcDim;
//using netCDF::NcFile;
//using netCDF::NcGroup;
//using netCDF::NcGroupAtt;
//using netCDF::NcType;
//using netCDF::NcVar;

using std::dec;
using std::get;
using std::hex;
using std::istreambuf_iterator;
using std::make_pair;
using std::map;
using std::pair;
using std::setfill;
using std::setw;
using std::tuple;
using std::uppercase;

using vmime::net::fetchAttributes;

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */

#endif /* XFLOATDEFINITIONS_H_ */
