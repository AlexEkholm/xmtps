/** The @ref util::xmsg::XCompositeHiResProfile class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 31, 2014 2:25:18 PM
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
#ifndef XCOMPOSITEHIRESPROFILE_H_
#define XCOMPOSITEHIRESPROFILE_H_

/* Includes */

// local
#include "AbstractXSensorDataComposite.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Class representing the aggregate raw/high-resolution/unbinned profile data for a single dive,
 *  composed from one or more raw profile data messages.
 *
 */
class XCompositeHiResProfile:
    public AbstractXSensorDataComposite<XHiResPressure, XHiResSalinity, XHiResTemperature,
    XMSG_MAX_PROF_HIRES> {

  public:

    /* Member Functions */

    /** The default constructor.
     *
     */
    XCompositeHiResProfile() :
            AbstractXSensorDataComposite<XHiResPressure, XHiResSalinity, XHiResTemperature,
            XMSG_MAX_PROF_HIRES>() {
      /* Do nothing. */

    }

    /** The destructor.
     *
     */
    ~XCompositeHiResProfile() {
      /* Do nothing. */
    }

    /** Generate a listing string representation of the this composite sensor data object.
     *
     *  @param[in]  id_inst The unique instrument identifier for this composite sensor data
     *                          object.
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

        // Print the header line.
        static const format fmt1("#   %5s %4s %11s");
        static const format fmt2("PRU %5d %4d %11d");
        os << format(fmt1) % "SN" % "dive" % "numSamples" << endl;
        os << format(fmt2) % id_inst % id_dive % max_num_samples();

        // Check if the data is complete.
        if(!IsDataComplete()) {
          os << endl << "#   (Incomplete Unbinned Profile Data)";
          return os.str();
        }

        {
          static const format fmt1("#   %5s %4s %11s %11s %11s %11s");
          static const format fmt2("#    %5s %4s %11s %11s %12s %11s");
          static const format fmt3("pru %5d %4d %11d %11s %11.3f %11.3f");

          // Iterate over the sensor data samples.
          for(size_t i = 0; i < max_num_samples(); i++) {
            if(i % 50 == 0) {
//              os << endl
//                 << "#---------------------------------------------------------------------------------------------------";
//              os << endl << format(fmt1) % "SN" % "dive" % "sampleIdx" % "pres" % "temp" % "saln";
//              os << endl << format(fmt2) % "" % "" % "" % "(dBar)" % "(degC)" % "(PSU)";
              os << endl
                 << "#---------------------------------------------------------------------------------------------------"
                 << endl
//                 << format(fmt1) % "SN" % "dive" % "sampleIdx" % "pres" % "temp" % "saln"
                 << "#      SN dive   sampleIdx        pres        temp        saln"
                 << endl
//                 << format(fmt2) % "" % "" % "" % "(dBar)" % "(degC)" % "(PSU)";
                 << "#                                (dBar)      (degC)      (PSU)";
            }

            os << endl
               << format(fmt3) % id_inst % id_dive % i % XMsgFormatPressure(SamplePresAsFloat(i, pg, po, pc))
                   % SampleTempAsFloat(i, tg, to) % SampleSalnAsFloat(i, sg, so);
          }
        }

      }CATCH_BLOCK("Failed to generate listing string for composite drift data!")

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
     *  @return     A PHY entry string representation of this composite raw profile data object.
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

          // Iterate over the sensor data samples.
          for(size_t i = 0; i < max_num_samples(); i++)
            os << endl
               << format(fmt) % SamplePresAsFloat(i, pg, po, pc) % SampleTempAsFloat(i, tg, to)
                   % SampleSalnAsFloat(i, sg, so);

        } else {

          static const format fmt("%8.2f %7.3f");

          // Iterate over the sensor data samples.
          for(size_t i = 0; i < max_num_samples(); i++)
            os << endl
               << format(fmt) % SamplePresAsFloat(i, pg, po, pc) % SampleTempAsFloat(i, tg, to);
        }

      }CATCH_BLOCK("Failed to generate PHY string for composite raw profile data!")

      return os.str();
    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XCOMPOSITEHIRESPROFILE_H_ */
