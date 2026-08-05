/** The The @ref util::thread::SynchronousQueue class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 18, 2014 1:44:13 PM
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
#ifndef SYNCHRONOUSQUEUE_H_
#define SYNCHRONOUSQUEUE_H_

/* Includes */

// boost
#include <boost/circular_buffer.hpp>
#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>
#include <boost/thread/tss.hpp>

namespace util {
namespace thread {

/* Class Definition */

/** Synchronous queue template.
 *
 *  This template provides synchronized access to the underlying circular buffer and may be
 *  implemented for data of arbitrary type.
 *
 */
template<typename T> class SynchronousQueue {

private:

    /* Data Members */

    /** Circular buffer for synchronized data storage.
     *
     */
    boost::circular_buffer<T> buffer;

    /** Conditional variable indicating that the buffer is not full.
     *
     */
    boost::condition bufferNotFull;

    /** Conditional variable indicating that the buffer is not empty.
     *
     */
    boost::condition bufferNotEmpty;

    /** Mutual exclusion lock for synchronizing operations.
     *
     */
    mutable boost::mutex mutex;

public:

    /* Member Functions */

    /** Constructor.
     *
     *  @param[in]      n               Capacity of the underlying circular buffer to set.
     */
    SynchronousQueue(size_t n) :
                    buffer(n) {

        /* Do nothing. */

    }

    ~SynchronousQueue() {

        /* Do nothing. */

    }

    /** Push element into queue.
     *
     *  @param[in]      elem        Element to push into queue.
     *  @param[in]      block       If TRUE, this function will block until the queue is not full.
     *                              If FALSE, this function will return FALSE if the queue is full
     *                              (and the element will not be pushed into the queue).
     *
     *  @return         TRUE if the element was successfully pushed into the queue, or FALSE
     *                  otherwise.
     */
    bool push(const T& elem, bool block) {

        {
            boost::unique_lock<boost::mutex> lock(mutex);

            if(block)

                // If function is blocking, block until buffer is not full.
                while(buffer.full())

                    bufferNotFull.wait(lock);

            else if(buffer.full())

                // Otherwise, return FALSE if the buffer is full.
                return false;

            // Push data into buffer.
            buffer.push_back(elem);
        }

        // Notify waiting threads (if any), that buffer is no longer empty.
        bufferNotEmpty.notify_one();

        return true;
    }

    bool pushIfNotContained(const T& elem, bool block) ATTR_DEPRECATED {

        if(!contains(elem))
        return push(elem, block);
        else
        return false;

    }

    /** Push multiple elements into queue.
     *
     *  @param[in]      first,last  Input iterators to the initial and final positions in a range.
     *                              The range used is @p [first,last), which includes all the
     *                              elements between first and last, including the element pointed
     *                              by first but not the element pointed by last.
     */
    template<typename Iter> void push(Iter first, Iter last) {

        while(first != last) {

            {
                // Get an exclusive lock.
                boost::unique_lock<boost::mutex> lock(mutex);

                while(buffer.full())

                    bufferNotFull.wait(lock);

                // Push data into buffer.
                buffer.push_back(*first++);
            }

            // Notify waiting thread.
            bufferNotEmpty.notify_one();
        }

        // Notify waiting thread.
//        bufferNotEmpty.notify_all();

    }

    /** Is the queue empty?
     *
     *  @return         TRUE if the buffer is empty, or FALSE otherwise.
     */
    bool empty() const ATTR_DEPRECATED {
        mutex::scoped_lock lock(mutex);
        return buffer.empty();
    }

    /** Is the queue full?
     *
     *  @return         TRUE if the buffer is full, or FALSE otherwise.
     */
    bool full() const ATTR_DEPRECATED {
        mutex::scoped_lock lock(mutex);
        return buffer.full();
    }

    /** Get the number of elements currently in the queue.
     *
     *  @return         The number of elements currently in the queue.
     */
    size_t size() const ATTR_DEPRECATED {
        mutex::scoped_lock lock(mutex);
        return buffer.size();
    }

    /** Pop element out of queue.
     *
     *  @param[out]     elem        The destination of element to pop out of the queue.
     *  @param[in]      block       If TRUE, this function will block until the queue is not empty.
     *                              If FALSE, this function will return FALSE if the queue is empty
     *                              (and no element will be popped out of the queue).
     *
     *  @return         TRUE if the element was successfully popped out of the queue, or FALSE
     *                  otherwise.
     */
    bool pop(T& elem, bool block) {

        {
            boost::unique_lock<boost::mutex> lock(mutex);
            if(block)

                // If function is blocking, block until buffer is not empty.
                while(buffer.empty())

                    bufferNotEmpty.wait(lock);

            else if(buffer.empty())

                // Otherwise, return FALSE if the buffer is empty.
                return false;

            // Pop element out of queue.
            elem = buffer.front();
            buffer.pop_front();
        }

        // Notify waiting threads (if any), that buffer is no longer full.
        bufferNotFull.notify_one();

        return true;
    }

    bool contains(const T& elem) ATTR_DEPRECATED {
        mutex::scoped_lock lock(mutex);

        bool contains_elem = false;

        for_each(buffer.begin(), buffer.end(), [&](const T& obj) {
                    if(obj == elem)
                    contains_elem = true;
                });

        return contains_elem;
    }

};

} /* namespace thread */
} /* namespace util */
#endif /* SYNCHRONOUSQUEUE_H_ */
