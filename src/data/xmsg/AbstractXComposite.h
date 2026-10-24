/** The @ref util::xmsg::AbstractXComposite class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 22, 2014 12:39:08 PM
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
#ifndef AbstractXComposite_H_
#define AbstractXComposite_H_

/* Includes */

// local
#include "XMessageIndexable.h"

/* Definitions */

namespace util {
namespace xmsg {

/** An abstract class representing XFloat dive data that consists of multiple X-Messages.
 *
 */
template<typename T> class AbstractXComposite {

    /* Type Definitions: */

    /** Type definition for the element aggregated by this object.
     *
     */
    typedef T Element_t;

    typedef vector<Element_t> ElementVec_t;

    typedef typename ElementVec_t::iterator ElementIter_t;

    typedef typename ElementVec_t::const_iterator ElementConstIter_t;

  protected:

  private:

    /** The vector of individual X-Messages elements that compose this composite object. */
    ElementVec_t elements_;

  protected:

    /** Append one element to the vector of individual X-Messages elements that compose this
     *  composite object.
     *
     *  @param[in]      element         The element to append.
     */
    void AppendElement(const Element_t& element) {

      elements_.push_back(element);

    }

    /** Append a vector of elements to the vector of individual X-Messages elements that compose
     *  this composite object.
     *
     *  @param[in]      elements        The vector of elements to append.
     */
    void AppendAllElements(const vector<Element_t>& elements) {

      elements_.insert(elements_.end(), elements.begin(), elements.end());

    }



  public:

    /** The default constructor.
     *
     */
    AbstractXComposite() :
            elements_() {

      /* Do nothing. */

    }

    /** The destructor.
     *
     */
    virtual ~AbstractXComposite() {

      /* Do nothing. */

    }

    /** Assign an element to this composite object.
     *
     *  The default implementation appends the element; derived classes may override it to insert
     *  elements in order or reject duplicates.
     *
     *  @param[in]  element     The element to assign.
     */
    virtual void AssignElement(const Element_t& element) {

      // Default behavior: append element.
      AppendElement(element);

    }

    /** Append a vector of elements to the vector of individual X-Messages elements that compose
     *  this composite object.
     *
     *  @param[in]      elements        The vector of elements to append.
     */
    template<int I> void AppendAllElements(const static_vector<Element_t, I>& elements) {
      elements_.insert(elements_.end(), elements.begin(), elements.end());
    }

    /** Get the vector of individual X-Messages elements that compose this composite object.
     *
     *  @return         The vector of individual X-Messages elements that compose this composite
     *                  object.
     */
    const vector<Element_t>& elements() const {

      return elements_;

    }

    virtual bool IsEmpty() const {

      return num_elements() == 0;

    }

    /** Get the number of individual elements that compose this composite object.
     *
     *  @return         The number of individual elements that compose this composite object.
     */
    virtual size_t num_elements() const {

      return elements_.size();

    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* AbstractXComposite_H_ */
