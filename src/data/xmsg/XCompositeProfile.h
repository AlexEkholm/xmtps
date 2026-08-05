/** The @ref util::xmsg::XCompositeProfile class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 17, 2014 3:07:43 PM
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

#ifndef XCOMPOSITEPROFILE_H_
#define XCOMPOSITEPROFILE_H_

/* Includes */

// local
#include "AbstractXSensorDataComposite.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Class representing the aggregate bin-averaged profile data for a single dive, composed from one
 *  or more bin-averaged profile data messages.
 */
class XCompositeProfile:
    public AbstractXSensorDataComposite<XProfilePressure, XProfileSalinity, XProfileTemperature,
    XMSG_MAX_PROF> {

  public:

    /* Public Member Functions: */

    /** The default constructor.
     *
     */
    XCompositeProfile() :
            AbstractXSensorDataComposite<XProfilePressure, XProfileSalinity, XProfileTemperature,
            XMSG_MAX_PROF>() {
      /* Do nothing. */
    }

    /** The destructor.
     *
     */
    ~XCompositeProfile() {
      /* Do nothing. */
    }

    /** Generate a listing string representation of the this composite sensor data object.
     *
     *  @param[in]  id_inst The unique instrument identifier for this composite sensor data object.
     *  @param[in]  id_dive The dive or cycle number for this composite data object.
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
     *  @exception  util::log::MessageException if @p x if an error occurs while generating a
     *              listing string representation of this composite sensor data object.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     A listing string representation of this composite sensor data object.
     */
    const string GenerateListingString(
        int id_inst,
        int id_dive,
        uint16_t pg,
        uint16_t po,
        uint16_t sg,
        uint16_t so,
        uint16_t tg,
        uint16_t to,
        float pc) const {
      ostringstream os;

      try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        { // Print the header line.
//          static const format fmt1("#   %5s %4s %11s");
          static const format fmt2("PRO %5d %4d %11d");
          os << "#      SN dive  numSamples"
//             << format(fmt1) % "SN" % "dive" % "numSamples"
             << endl
             << format(fmt2) % id_inst % id_dive % max_num_samples();
        }

        // Check if the data is complete.
        if(!IsDataComplete()) {
          os << endl << "#   (Incomplete Binned Profile Data)";
          return os.str();
        }

        if(HasAuxTempData()) {
          static const format fmt1("#   %5s %4s %11s %11s %11s %11s %11s");
          static const format fmt2("#    %5s %4s %11s %11s %12s %12s %11s");
          static const format fmt3("pro %5d %4d %11d %11.3f %11.3f %11.3f %11.3f");

          // Iterate over the sensor data samples.
          for(size_t i = 0; i < max_num_samples(); i++) {
            if(i % 50 == 0) {
              os << endl
                 << "#---------------------------------------------------------------------------------------------------"
                 << endl
                 << "#      SN dive   sampleIdx        pres       temp1       temp2        saln"
                 << endl
                 << "#                               (dBar)      (degC)      (degC)       (PSU)"
                 ;
            }

              os << endl
                 << format(fmt3) % id_inst % id_dive % i % SamplePresAsFloat(i, pg, po, pc)
                     % SampleTempAsFloat(i, tg, to) % SampleAuxTempAsFloat(i, tg, to)
                     % SampleSalnAsFloat(i, sg, so);
          }

        } else {

          static const format fmt3("pro %5d %4d %11d %11.3f %11.3f %11.3f");

          // Iterate over the sensor data samples.
          for(size_t i = 0; i < max_num_samples(); i++) {
            if(i % 50 == 0) {
              os << endl
                 << "#---------------------------------------------------------------------------------------------------"
                 << endl
                 << "#      SN dive   sampleIdx        pres        temp        saln"
                 << endl
                 << "#                               (dBar)      (degC)       (PSU)";
            }

              os << endl
                 << format(fmt3) % id_inst % id_dive % i % SamplePresAsFloat(i, pg, po, pc)
                     % SampleTempAsFloat(i, tg, to) % SampleSalnAsFloat(i, sg, so);
          }
        }

      }CATCH_BLOCK("Failed to generate listing string for composite profile data (binned)!")

      return os.str();
    }

    /** Generate a PHY entry string from this composite bin-averaged profile data object.
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
     *  @throw      log::MessageException if an error occurs while generating the PHY string.
     *
     *  @return     A PHY entry string representation of this composite bin-averaged profile data
     *              object.
     */
    const string GeneratePhyString(
        uint16_t pg,
        uint16_t po,
        uint16_t sg,
        uint16_t so,
        uint16_t tg,
        uint16_t to,
        float pc) const {
      ostringstream os;

      try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        if(num_saln_samples()) {

          static const format fmt("%8.2f %7.3f %7.3f");


          // Ensure correct ordering
          if(num_saln_samples() > 1 && SamplePresAsFloat(0, pg, po, pc) > SamplePresAsFloat(1, pg, po, pc)) {

            // Iterate over the sensor data samples.
            for(size_t i = 0; i < max_num_samples(); i++)
              if(SamplePresAsFloat(i, pg, po, 0) > 1)
                os << endl << format(fmt) % SamplePresAsFloat(i, pg, po, pc) % SampleTempAsFloat(i, tg, to) % SampleSalnAsFloat(i, sg, so);
          } else {

            size_t mx = max_num_samples() - 1;

            // Iterate over the sensor data samples.
            for(size_t i = 0; i < max_num_samples(); i++)
              if(SamplePresAsFloat(i, pg, po, 0) > 1)
                os << endl << format(fmt) % SamplePresAsFloat(mx - i, pg, po, pc) % SampleTempAsFloat(mx - i, tg, to) % SampleSalnAsFloat(mx - i, sg, so);
          }

        } else {

          static const format fmt("%8.2f %7.3f");

          // Iterate over the sensor data samples.
          for(size_t i = 0; i < max_num_samples(); i++)
            os << endl  << format(fmt) % SamplePresAsFloat(i, pg, po, pc) % SampleTempAsFloat(i, tg, to);
        }

      }CATCH_BLOCK("Failed to generate PHY string for composite binned profile data!")

      return os.str();
    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XCOMPOSITEPROFILE_H_ */
