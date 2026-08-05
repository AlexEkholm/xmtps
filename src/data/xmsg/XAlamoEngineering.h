/** The @ref util::xmsg::XAlamoEngineering class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Aug 5, 2014 1:32:39 PM
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

#ifndef XALAMOENGINEERING_H_
#define XALAMOENGINEERING_H_

/* Includes */

#include "XMessage.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Abstract class representing a version controlled engineering message for ALAMO-type instruments.
 *
 *  @note           The X-Message types extended from this class are used by ALAMO-type instruments
 *                  only.
 *  @note           The X-Message types extended from this class were introduced in the X-Message
 *                  format specification version 2.1.
 */
class XAlamoEngineering:
        public XMessage,
        public util::date::DateUtils {
private:

    /* Data Members */

    /** The engineering message version identifier. */
    uint8_t engineering_version_;

    /** The engineering message state start time. */
    ptime start_time_;

protected:

    /* Member Functions */

    /** Set the engineering message version identifier.
     *
     *  @param[in]  engineering_version The engineering message version identifier.
     */
    void set_engineering_version(uint8_t engineering_version) {
        engineering_version_ = engineering_version;
    }

    /** Set the engineering message state start time
     *
     *  @param[in]  start_time          The engineering message state start time.
     */
    void set_start_time(const ptime& start_time) {
        start_time_ = start_time;
    }

public:

    /* Member Functions */

    /** Parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     */
    XAlamoEngineering(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XMessage(dive, whoi, data),
                    engineering_version_(XMSG_ENG_BAD_VER),
                    start_time_(not_a_date_time) {
        /* Do nothing. */
    }

    /** Destructor. */
    virtual ~XAlamoEngineering() {
        /* Do nothing. */
    }

    /** Get the engineering message state start time.
     *
     *  @return         The engineering message state start time.
     */
    const ptime& start_time() const {
        return start_time_;
    }

    /** Get the engineering message version identifier.
     *
     *  @return         The engineering message version identifier.
     */
    uint8_t engineering_version() const {
        return engineering_version_;
    }

    /** Get the engineering message verion identifier as a floating point number.
     *
     *  @return         The engineering message verion identifier as a floating point number.
     */
    float engineering_version_as_float() const {
        volatile float x = 0x0F & engineering_version_;
        while(x > 1)
            x /= 10;
        return x + ((0xF0 & engineering_version_) >> 4);
    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XALAMOENGINEERING_H_ */
