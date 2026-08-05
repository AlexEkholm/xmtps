/** The @ref util::xmsg::XCompositePump class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 22, 2014 12:46:38 PM
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


#ifndef XCOMPOSITEPUMP_H_
#define XCOMPOSITEPUMP_H_

/* Includes */

// local
#include "XPump.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Class representing the aggregate pump time-series data for a single dive, composed from one or
 *  more pump time-series messages.
 */
class XCompositePump: public AbstractXComposite<shared_ptr<XPump> > {


public:

    /* Public Member Functions: */

    /** The default constructor.
     *
     */
    XCompositePump() {
        /* Do nothing. */
    }

    /** The destructor.
     *
     */
    ~XCompositePump() {
        /* Do nothing. */
    }

    /** Generate a listing entry string from this composite pump data object.
     *
     *  @param[in]  pg      The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po      The pressure sensor offset, in dBar.
     *  @param[in]  pc      The pressure correction, in dBar, to be subtracted computed pressure
     *                      value.
     *
     *  @throw      log::MessageException if an error occurs while generating the listing string.
     *
     *  @return     A listing entry string representation of this composite pump data object.
     */
    const string GenerateListingString(uint16_t pg, uint16_t po, float pc) const {

        ostringstream os;

        try {

            os << XPump::GenerateListingComment();

            vector<shared_ptr<XPump> >::const_iterator it;

            for(it = elements().begin(); it < elements().end(); it++)

                if(*it) {

                    if(it > elements().begin()) {

                        os << endl;
                        XMSG_SECTION_BREAK();
                        os << XPump::GenerateListingComment();

                    }

                    os << endl << (*it)->GenerateListingString(pg, po, pc);

                }

        }CATCH_BLOCK("Failed to generate listing string for composite pump data object!")

        return os.str();
    }
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XCOMPOSITEPUMP_H_ */
