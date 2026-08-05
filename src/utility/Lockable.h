/** The @ref util::thread::Lockable class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 18, 2014 12:38:49 PM
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

#ifndef LOCKABLE_H_
#define LOCKABLE_H_

/* Includes */

// local
#include "Log.h"

// boost
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/mutex.hpp>

namespace util {
namespace thread {

using boost::mutex;

/* Class Definitions */

/** A lockable class.
 *
 *  @note           The mutex used for locking the object is not recursive, which means that it may
 *                  be lock at most one time, even by the same thread.
 */
class Lockable {

protected:

    /* Data Members */

    /** Mutual exclusion lock for synchronizing operations. */
    mutable mutex mutex_;

public:

    /* Member Functions */

    /** The constructor.
     *
     */
    Lockable() {
        /* Do nothing. */
    }

    /** The destructor.
     *
     */
    virtual ~Lockable();

    /** Lock the object.
     *
     *  @param[in]      block       If TRUE, then the calling thread will block until the lock
     *                              becomes available. The default value is TRUE.
     *
     *  @throw          util::log::MessageException if an error occurs.
     *
     *  @return         TRUE if the lock was successfully obtained, or FALSE if block is FALSE and
     *                  the mutex is already locked.
     */
    bool LockObject(bool block) {
        try {
            if(block) mutex_.lock();
            else return mutex_.try_lock();
            return true;
        }CATCH_BLOCK("Failed to lock lockable object!")
        return false;
    }

    /** Unlock the object.
     *
     *  @throw          util::log::MessageException if an error occurs.
     */
    void UnlockObject() {
        try {
            mutex_.unlock();
        }CATCH_BLOCK("Failed to unlock lockable object!")
    }
};

} /* namespace thread */
} /* namespace util */
#endif /* LOCKABLE_H_ */
