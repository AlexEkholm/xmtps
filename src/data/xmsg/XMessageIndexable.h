/** The @ref util::xmsg::XMessageIndexable class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Aug 2, 2014 5:31:10 PM
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

#ifndef XMESSAGEINDEXABLE_H_
#define XMESSAGEINDEXABLE_H_

/* Includes */

// local
#include "XMessage.h"

namespace util {
namespace xmsg {

/* Class Definitions */

/** Class representing an indexable X-Message. */
class XMessageIndexable:
    public util::xmsg::XMessage {

private:

  /* Data Members */

  /** The index of this X-Message. */
  uint8_t xmsg_index_;

protected:

  /* Member Functions */

  /** The default constructor.
   *
   *  @exception  None. Never throws.
   */
  XMessageIndexable() ATTR_NO_EXCEPTIONS :
          XMessage(),
          xmsg_index_(0xFF) {
    /* Do nothing. */
  }

  /** Set the index of this X-Message.
   *
   *  @param[in]  xmsg_index      The index of this X-Message.
   */
  void set_xmsg_index(uint8_t xmsg_index) {
    xmsg_index_ = xmsg_index;
  }

public:

  /* Member Functions */

  /** The parameterized constructor.
   *
   *  @param[in]  dive        The dive or cycle number of this message.
   *  @param[in]  whoi        The unique WHOI identifier.
   *  @param[in]  data        The binary X-message data to decode.
   *
   *  @exception  None. Never throws.
   */
  XMessageIndexable(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) ATTR_NO_EXCEPTIONS :
          XMessage(dive, whoi, data),
          xmsg_index_(0xFF) {
    /* Do nothing. */
  }

  /** The destructor.
   *
   */
  ~XMessageIndexable() {
    /* Do nothing. */
  }

  /** Decode the X-Message data.
   *
   *  @param[in]      data        The encoded X message to decode.
   */
  virtual void DecodeXMsgData(const vector<uint8_t>& data) = 0;

  /** Get the index of this X-Message.
   *
   *  @return         The index of this X-Message.
   */
  uint8_t xmsg_index() const {
    return xmsg_index_;
  }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XMESSAGEINDEXABLE_H_ */
