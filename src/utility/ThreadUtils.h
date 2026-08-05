/** The @ref util::thread::ThreadUtils class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Aug 21, 2014 3:20:37 PM
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

#ifndef THREADUTILS_H_
#define THREADUTILS_H_

/* Includes */

// local
#include "Lockable.h"
#include "RecursiveLockable.h"
#include "SynchronousQueue.h"
#include "StreamUtils.h"

namespace util {
namespace thread {

/* Class Definitions */

/** Class providing useful functions for manipulating threads and processes.
 *
 */
class ThreadUtils {
public:
    ThreadUtils() {
        /* Do nothing. */
    }

    virtual ~ThreadUtils() {
        /* Do nothing. */
    }

    /** Invoke an executable on the local filesystem, pipe the output of the command back to this
     *  application .
     *
     *  @param[in]      cmd         A pointer to the C-string containing the local system command to
     *                              execute.
     *
     *  @throw          log::MessageException if the pointer to the command string is null, the
     *                  command string is zero length, or an error occurs while executing the external
     *                  command or piping its output back to this application.
     *
     *  @return         The output of the executed command.
     */
    static std::string executeAndPipe(const char* cmd) {
        using bis::stream_buffer;
        using bis::file_descriptor_source;
        using std::string;
        using namespace std;
        ostringstream os;

        FILE* cmdPipe = NULL;
        string line;

        try {
            // Check command.
            if(cmd == NULL)
                EXC_THROW("Pointer to command string is NULL!");
            if(strlen(cmd) < 1)
                EXC_THROW("Command string is zero length!");

            // Fork, execute the command, and pipe the output of the command back to the application.
            dbg1() << "Executing external system command: " << cmd << "...";
            if(!(cmdPipe = popen(cmd, "r")))
                EXC_THROW("Failed to pipe stream from process: The call to popen() failed!");

            // Buffer the piped output of the command into an istream.
            stream_buffer<file_descriptor_source> cmdStreamBuf(
                    file_descriptor_source(fileno(cmdPipe), bis::close_handle));
            istream cmdStream(&cmdStreamBuf);

            // Capture the output of the external command.
            while(getline(cmdStream, line))
                os << line << endl;

        }CATCH_BLOCK("Failed to invoke process: '" + std::string(cmd) + "'!")

        // Return the output of the external command.
        return os.str();
    }

    /** Invoke an executable on the local filesystem, pipe the output of the command back to this
     *  application .
     *
     *  @param[in]      cmd         A reference to the string containing the local system command to
     *                              execute.
     *
     *  @throw          log::MessageException if the pointer to the command string is null, the
     *                  command string is zero length, or an error occurs while executing the external
     *                  command or piping its output back to this application.
     *
     *  @return         The output of the executed command.
     */
    static std::string executeAndPipe(const std::string& cmd) {
        return executeAndPipe(cmd.c_str());
    }
};

} /* namespace thread */
} /* namespace util */
#endif /* THREADUTILS_H_ */
