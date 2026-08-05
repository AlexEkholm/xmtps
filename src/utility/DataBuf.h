/** The @ref util::stream::DataBuf class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 18, 2014 12:34:52 PM
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

#ifndef DATABUF_H_
#define DATABUF_H_

/* Includes */

// local
#include "SystemUtils.h"

// boost
#include <boost/filesystem/fstream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/stream_buffer.hpp>
namespace bis = boost::iostreams;

namespace util {
namespace stream {

using std::char_traits;
using std::basic_streambuf;

/* Class Definition */

/** Template class for wrapping an arbitrary contiguous section of data as a std::basic_streambuf
 *  object.
 *
 */
template<typename CharT, typename TraitsT = char_traits<CharT> >
class DataBuf: public basic_streambuf<CharT, TraitsT> {

public:

    /* Member Functions */

    /** Parameterized constructor.
     *
     *  @param[in]      data        A pointer to the start of the memory section of interest.
     *  @param[in]      size        The size of the memory section, in bytes.
     */
    DataBuf(const CharT* data, size_t size) {
        this->setg(const_cast<CharT*>(data), const_cast<CharT*>(data),
                   const_cast<CharT*>(data + size));
    }
};

} /* namespace stream */
} /* namespace util */
#endif /* DATABUF_H_ */
