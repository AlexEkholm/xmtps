/** The thread definitions header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Dec 28, 2016 9:33:48 AM
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

#ifndef THREADDEFINITIONS_H_
#define THREADDEFINITIONS_H_

/* Includes: */

// local
#include "data/XFloat.h"

/* Definitions: */

#if !defined(XMTPS_DEBUG_FETCH_MAIL)

    /** The debug-fetch-mail flag.
     *
     *  If TRUE, then additional debug messages will be printed during the mail fetch phase of
     *  thread operation.
     */
#   define XMTPS_DEBUG_FETCH_MAIL                   (false)

#endif // !defined(XMTPS_DEBUG_FETCH_MAIL)

#if !defined(XMTPS_DEBUG_NC_FILE_WRITER_THREAD)

#   define XMTPS_DEBUG_NC_FILE_WRITER_THREAD        (false)

#endif // !defined(XMTPS_DEBUG_NC_FILE_WRITER_THREAD)

#if !defined(XMTPS_DEBUG_POST_PROC_THREAD)

#   define XMTPS_DEBUG_POST_PROC_THREAD             (true)

#endif // !defined(XMTPS_DEBUG_NC_FILE_WRITER_THREAD)

#if !defined(XMTPS_NC_FILE_WRITER_USES_QUEUE)

#   define XMTPS_NC_FILE_WRITER_USES_QUEUE          (false)

#endif // !defined(XMTPS_NC_FILE_WRITER_USES_QUEUE)

#if !defined(XMTPS_POST_PROC_USES_QUEUE)

#   define XMTPS_POST_PROC_USES_QUEUE               (false)

#endif // !defined(XMTPS_POST_PROC_USES_QUEUE)

#if !defined(XMTPS_THREAD_JOIN_SECONDS) \
    || (defined(XMTPS_THREAD_JOIN_SECONDS) && XMTPS_THREAD_JOIN_SECONDS <= 0)

    /** The maximum duration to allow for threads to join upon termination, in seconds.
     *
     */
#   define XMTPS_THREAD_JOIN_SECONDS                120

#endif // !defined(XMTPS_THREAD_JOIN_SECONDS) ...

#endif /* THREADDEFINITIONS_H_ */
