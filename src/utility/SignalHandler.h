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

// std
#include <csignal>

// local
#include "Log.h"

/* Definitions */

namespace util {
namespace sig {

/* Class Definitions */

/** Class providing function for handling POSIX style system signals.
 *
 *  This class provides functions for graceful shutdown, triggered by catchable system signals.
 *
 *  The signal handler, @c CatchSignal(), only records the signal: a signal interrupts whichever
 *  thread happens to be running, and if that thread holds a lock (the logger's, the memory
 *  allocator's, ...) any logging, locking or memory allocation in the handler deadlocks. The daemon
 *  thread checks @ref caught_signal() in its main loop and calls @ref ExitHandler() from there.
 */
class SignalHandler {

    /* Static Data Members */

    /** The last signal caught, or 0 if none. Set only by @c CatchSignal(). */
    static volatile std::sig_atomic_t caught_signal_;

    /** The signal handler: record the signal for the daemon thread.
     *
     *  @param[in]  signo   The signal number.
     */
    static void CatchSignal(int signo) {
        caught_signal_ = signo;
    }

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
            if(signal(SIGABRT, SignalHandler::CatchSignal) == SIG_ERR)
                EXC_THROW("Failed to register signal handler for SIGABRT!");

            ldbg() << "Registering SIGINT handler...";
            if(signal(SIGINT, SignalHandler::CatchSignal) == SIG_ERR)
                EXC_THROW("Failed to register signal handler for SIGINT!");

            ldbg() << "Registering SIGQUIT handler...";
            if(signal(SIGQUIT, SignalHandler::CatchSignal) == SIG_ERR)
                EXC_THROW("Failed to register signal handler for SIGQUIT!");

            ldbg() << "Registering SIGTERM handler...";
            if(signal(SIGTERM, SignalHandler::CatchSignal) == SIG_ERR)
                EXC_THROW("Failed to register signal handler for SIGTERM!");

            ldbg() << "Registering SIGTSTP handler...";
            if(signal(SIGTSTP, SignalHandler::CatchSignal) == SIG_ERR)
                EXC_THROW("Failed to register signal handler for SIGTSTP!");

            linf() << "Done. Successfully registered system signal handlers.";
        }CATCH_BLOCK("Failed to register signal handlers!")
    }

    /** Get the last signal caught.
     *
     *  @return     The signal number, or 0 if no signal has been caught.
     */
    static int caught_signal() {
        return caught_signal_;
    }

    static void ExitHandler(int signo);
};

} /* namespace sig */
} /* namespace util */
#endif /* SIGNALHANDLER_H_ */
