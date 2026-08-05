/** The @ref util::stream::VectorBuf template class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 14, 2014 9:11:06 AM
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
#ifndef VECTORSTREAMBUF_H_
#define VECTORSTREAMBUF_H_

/* Includes */

// local
#include "DataBuf.h"

namespace util {
namespace stream {

/* Template Definitions*/

/** Template class for wrapping a vector<T> as a std::basic_streambuf<T>.
 *
 */
template<typename CharT, typename TraitsT = std::char_traits<CharT> >
class VectorBuf: public DataBuf<CharT, TraitsT> {
public:
    /** Parameterized constructor.
     *
     *  @param[in]      vec         Vector to wrap.
     */
    VectorBuf(const std::vector<CharT> &vec) :
            DataBuf<CharT>(const_cast<CharT*>(vec.data()), vec.size()) {
        /* Do nothing. */
    }
};

/** Template class for wrapping a vector<T> as a std::basic_streambuf<char>.
 *
 */
template<typename CharT>
class VectorBuf_c: public DataBuf<char> {
public:
    /** Parameterized constructor.
     *
     *  @param[in]      vec         Vector to wrap.
     */
    VectorBuf_c(const std::vector<CharT> &vec) :
            DataBuf<char>((char*)vec.data(), vec.size()) {
        /* Do nothing. */
    }
};

} /* namespace stream */
} /* namespace util */
#endif /* VECTORSTREAMBUF_H_ */
