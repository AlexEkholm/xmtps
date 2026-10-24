/** The @ref util::xmsg::XCompositeValve class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Mar 22, 2016 10:08:10 AM
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

#ifndef XCOMPOSITEVALVE_H_
#define XCOMPOSITEVALVE_H_

/* Includes: */

#include "XValve.h"

/* Definitions: */

namespace util {
namespace xmsg {

/* Types: */

/** The composite valve data class.
 *
 *  An instance of this class aggregates the valve data messages (see @ref XValve) received for a
 *  single dive.
 */
class XCompositeValve:
        public AbstractXComposite<shared_ptr<XValve> > {


public:

    /* Public Member Functions: */


    XCompositeValve() {

        /* Do nothing. */

    }

     ~XCompositeValve() {

         /* Do nothing. */

     }

     const string GenerateListingString(uint16_t pGain, uint16_t pOff, float pres_correction) const {

         ostringstream os;

         try {

             os << XValve::GenerateListingComment();

             vector<shared_ptr<XValve> >::const_iterator it;
             for(it = elements().begin(); it < elements().end(); it++)

                 if(*it) {

                     if(it > elements().begin()) {

                         os << endl;
                         XMSG_SECTION_BREAK();
                         os << XValve::GenerateListingComment();

                     }

                     os << endl << (*it)->GenerateListingString(pGain, pOff, pres_correction);

                 }

         }CATCH_BLOCK("Failed to generate listing string for composite valve data object!")

         return os.str();

     }

};

/* Functions: */

} /* namespace xmsg */
} /* namespace util */

#endif /* XCOMPOSITEVALVE_H_ */
