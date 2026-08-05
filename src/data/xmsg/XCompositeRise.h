/** The @ref util::xmsg::XCompositeRise class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 24, 2014 2:16:15 PM
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

#ifndef XCOMPOSITERISE_H_
#define XCOMPOSITERISE_H_

/* Includes */

// local
#include "AbstractXComposite.h"
#include "XRise.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Type Definitions */

/* Class Definitions */

/** Class representing the aggregate pump data for a single dive, composed from one or more pump
 *  data messages.
 *
 */
class XCompositeRise : public AbstractXComposite<shared_ptr<XRise> > {


public:

    /* Member Functions */

    /** The default constructor.
     *
     */
    XCompositeRise() {
        /* Do nothing. */
    }

    /** The destructor.
     *
     */
    ~XCompositeRise() {
        /* Do nothing. */
    }

    /** Generate a listing entry string from this composite rise time-series data object.
     *
     *  @param[in]  pg      The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po      The pressure sensor offset, in dBar.
     *  @param[in]  sg      The salinity sensor sensitivity scale factor, in LSB/PSU.
     *  @param[in]  so      The salinity sensor offset, in PSU.
     *  @param[in]  tg      The temperature sensor sensitivity scale factor, in LSB/degC.
     *  @param[in]  to      The temperature sensor offset, in degC.
     *  @param[in]  pc      The pressure correction, in dBar, to be subtracted computed pressure
     *                      value.
     *
     *  @throw      log::MessageException if an error occurs while generating the listing string.
     *
     *  @return     A listing entry string representation of this composite rise time-series data
     *              object.
     */
    const string GenerateListingString(
            uint16_t pg,
            uint16_t po,
            uint16_t sg,
            uint16_t so,
            uint16_t tg,
            uint16_t to,
            float pc) const  {
        ostringstream os;

        try {

            os << AbstractXFallRise::GenerateListingComment();
            for(auto it = elements().begin(); it < elements().end(); it++)
                if(*it) {
                    if(it > elements().begin()) {
                        os << endl;
                        XMSG_SECTION_BREAK();
                        os << XRise::GenerateListingComment();
                    }
                    os << endl << (*it)->GenerateListingString(pg, po, sg, so, tg, to, pc);
                }

        }CATCH_BLOCK("Failed to generate listing string for composite fall data object!")

        return os.str();
    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XCOMPOSITERISE_H_ */
