/** The @ref util::xmsg::XCompositeAuxTemp class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 31, 2017 3:42:03 PM
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

#ifndef DATA_XMSG_XCOMPOSITEAUXTEMP_H_
#define DATA_XMSG_XCOMPOSITEAUXTEMP_H_

/* Includes: */

#include "AbstractXComposite.h"
#include "XProfileTemperature.h"

/* Definitions: */

namespace util {
namespace xmsg {

/* Types: */

/** The composite auxiliary temperature class.
 *
 *  An instance of this class aggregates the auxiliary-channel profile temperature messages (see
 *  @ref XProfileTemperature) received for a single dive.
 */
class XCompositeAuxTemp:
    public AbstractXComposite<shared_ptr<XProfileTemperature> > {
  public:

    XCompositeAuxTemp() {
      /* Do nothing. */
    }

    ~XCompositeAuxTemp() {
      /* Do nothing. */
    }

    const string GenerateListingString(int id_inst, int id_dive, uint16_t tGain, uint16_t tOff) {

      int idx = 0;
      ostringstream os;


      // Print the header line.
      static const format fmt1("#   %4s %4s");
      static const format fmt2("AXT %4d %4d");
      os << format(fmt1) % "SN" % "dive"  << endl;
      os << format(fmt2) % id_inst % id_dive << endl;


      static const format fmt3("#   %4s %4s %3s %11s %11s");
      static const format fmt4("#    %4s %4s %3s %11s %11s");
      static const format fmt5("axt %4d %4d %3s %11d %11.3f");

      for(auto it = elements().begin(); it < elements().end(); ++it) {

        const shared_ptr<XProfileTemperature> atemp = *it;

        os << format(fmt3) % "SN" % "dive" % "idx"
            % "temp" % "temp" << endl;
        os << format(fmt4) % "" % "" % ""
            % "(degC)" % "(counts)" << endl;


        for(unsigned int i = 0; i < atemp->num_samples(); ++i) {
          os << format(fmt5) % id_inst % id_dive % idx % atemp->Sample(i)
                  % atemp->SampleValueAsTemperature(i, tGain, tOff) << endl;
        }

        idx++;

      }

      return os.str();
    }

};

/* Functions: */

} /* namespace xmsg */
} /* namespace util */

#endif /* DATA_XMSG_XCOMPOSITEAUXTEMP_H_ */
