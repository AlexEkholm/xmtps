/** The The @ref util::sig::SignalHandler class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 18, 2014 12:49:07 PM
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

#ifndef SIGNALHANDLER_H_
#define SIGNALHANDLER_H_

/* Includes */

#include "Log.h"

/* Definitions */

namespace util {
namespace sig {

/* Class Definitions */

/** Class providing function for handling POSIX style system signals.
 *
 *  This class provides functions for graceful shutdown, triggered by catchable system signals.
 *
 */
class SignalHandler {

public:

    /* Member Functions */

    /** Register signal handlers functions for UNIX-style system signals.
     *
     *  @exception      None. Never throws.
     *
     */
    static void RegisterSignalHandlers() ATTR_NO_EXCEPTIONS {
        try {
            /* Exit Signal Handlers */
            lout() << "Registering system signal handlers...";

            ldbg() << "Registering SIGABRT handler...";
            if(signal(SIGABRT, SignalHandler::ExitHandler) == SIG_ERR)
                EXC_THROW("Failed to register signal handler for SIGABRT!");

            ldbg() << "Registering SIGINT handler...";
            if(signal(SIGINT, SignalHandler::ExitHandler) == SIG_ERR)
                EXC_THROW("Failed to register signal handler for SIGINT!");

            ldbg() << "Registering SIGQUIT handler...";
            if(signal(SIGQUIT, SignalHandler::ExitHandler) == SIG_ERR)
                EXC_THROW("Failed to register signal handler for SIGQUIT!");

            ldbg() << "Registering SIGTERM handler...";
            if(signal(SIGTERM, SignalHandler::ExitHandler) == SIG_ERR)
                EXC_THROW("Failed to register signal handler for SIGTERM!");

            ldbg() << "Registering SIGTSTP handler...";
            if(signal(SIGTSTP, SignalHandler::ExitHandler) == SIG_ERR)
                EXC_THROW("Failed to register signal handler for SIGTSTP!");

            linf() << "Done. Successfully registered system signal handlers.";
        }CATCH_BLOCK("Failed to register signal handlers!")
    }

    static void ExitHandler(int signo);
};

} /* namespace sig */
} /* namespace util */
#endif /* SIGNALHANDLER_H_ */
