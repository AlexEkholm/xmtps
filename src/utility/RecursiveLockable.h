/** The @ref util::thread::RecursiveLockable class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 18, 2014 12:28:02 PM
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

#ifndef RECURSIVELOCKABLE_H_
#define RECURSIVELOCKABLE_H_

/* Includes */

// local
#include "Log.h"

// boost
#include <boost/thread/locks.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/thread/recursive_mutex.hpp>

namespace util {
namespace thread {

using std::recursive_mutex;

/* Class Definitions */

/** A recursively lockable class.
 *
 *  @note       This class uses a recursive mutex which means that it may be locked multiple times
 *              by the same thread.
 */
class RecursiveLockable {

protected:

    /* Data members */

    /** Recursive mutual exclusion lock for synchronizing operations. */
    mutable recursive_mutex mutex_;

public:

    /* Member Functions */

    /** Default constructor.
     *
     */
    RecursiveLockable() {
        /* Do nothing. */
    }

    /** Destructor.
     *
     */
    virtual ~RecursiveLockable() {
        /* Do nothing. */
    }

    /** Lock the object.
     *
     *  @param[in]      block       If TRUE, then the calling thread will block until the lock
     *                              becomes available.
     *
     *  @throw          util::log::MessageException if an error occurs.
     *
     *  @return         TRUE if the lock was successfully obtained, or FALSE if block is FALSE and
     *                  another thread owns the lock on the mutex.
     */
    bool LockObject(bool block) {
        try {
            if(block) mutex_.lock();
            else return mutex_.try_lock();
            return true;
        }CATCH_BLOCK("Failed to lock recursive lockable object!")
        return false;
    }

    /** Unlock the object.
     *
     *  @throw          util::log::MessageException if an error occurs.
     */
    void UnlockObject() {
        try {
            mutex_.unlock();
        }CATCH_BLOCK("Failed to unlock recursive lockable object!")
    }
};

} /* namespace thread */
} /* namespace util */
#endif /* RECURSIVELOCKABLE_H_ */
