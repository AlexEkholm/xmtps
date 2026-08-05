/** The @ref util::log::MessageException class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:18:48 PM
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
#ifndef MESSAGEEXCEPTION_H_
#define MESSAGEEXCEPTION_H_

/* Includes */

// local
#include "StringUtils.h"

// boost
#include <boost/exception/all.hpp>

#if defined(BUILD_NUMBER) && defined(__cplusplus) && __cplusplus >= 201103L
/** The "never throws" function attribute.
 *
 */
#   define ATTR_NO_EXCEPTIONS      noexcept
#else
/** The "never throws" function attribute.
 *
 */
#   define ATTR_NO_EXCEPTIONS      throw()
#endif //defined(BUILD_NUMBER) && defined(__cplusplus) && __cplusplus >= 201103L

/* Definitions */

namespace util {
namespace log {

using std::string;
using std::ostringstream;

/* Class Definition */

/** Exception class with associated message.
 *
 */
class MessageException:
        public std::exception,
        public boost::exception {

private:

    /* Data Members */

    /** The exception message.
     *
     */
    string message_;

public:

    /* Member Functions */

    /** Default Constructor.
     *
     */
    MessageException() {
        /* Do nothing */
    }

    /** Parameterized constructor.
     *
     * @param[in]       message         Exception message string.
     */
    MessageException(const string& message) :
                    message_(message) {
        /* Do nothing. */
    }

    /** Parameterized constructor.
     *
     * @param[in]       message         Exception message string.
     */
    MessageException(const ostringstream& message) :
                    message_(message.str()) {
        /* Do nothing. */
    }

    /** Destructor.
     *
     */
    virtual ~MessageException() ATTR_NO_EXCEPTIONS {
        /* Do nothing. */
    }

    /** Return C string representing message exception.
     *
     *  @return         C string representing message exception.
     */
    virtual const char* what() const ATTR_NO_EXCEPTIONS {
        return message_.c_str();
    }
};

} /* namespace mail */
} /* namespace utility */
#endif /* MESSAGEEXCEPTION_H_ */
