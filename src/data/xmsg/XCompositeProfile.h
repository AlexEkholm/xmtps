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
     *  @return     A listing string representation of this composite sensor data object. When the
     *              float sends a complete dynamically corrected salinity channel (0xDC), each line
     *              also has the corrected salinity (salnCorr).
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

        // Add the dynamically corrected salinity column when the float sends a complete channel.
        const bool corr = IsCorrectedSalnDataComplete();
        static const format fmt_corr(" %11.3f");

        if(HasCorrectedSalnData() && !corr)
          os << endl << "#   (Incomplete Corrected Salinity Data)";

        if(HasAuxTempData()) {
          static const format fmt1("#   %5s %4s %11s %11s %11s %11s %11s");
          static const format fmt2("#    %5s %4s %11s %11s %12s %12s %11s");
          static const format fmt3("pro %5d %4d %11d %11s %11.3f %11.3f %11.3f");

          // Iterate over the sensor data samples.
          for(size_t i = 0; i < max_num_samples(); i++) {
            if(i % 50 == 0) {
              os << endl
                 << "#---------------------------------------------------------------------------------------------------"
                 << endl
                 << "#      SN dive   sampleIdx        pres       temp1       temp2        saln"
                 << (corr ? "    salnCorr" : "")
                 << endl
                 << "#                               (dBar)      (degC)      (degC)       (PSU)"
                 << (corr ? "       (PSU)" : "")
                 ;
            }

              os << endl
                 << format(fmt3) % id_inst % id_dive % i % XMsgFormatPressure(SamplePresAsFloat(i, pg, po, pc))
                     % SampleTempAsFloat(i, tg, to) % SampleAuxTempAsFloat(i, tg, to)
                     % SampleSalnAsFloat(i, sg, so);
              if(corr)
                os << format(fmt_corr) % SampleCorrectedSalnAsFloat(i, sg, so);
          }

        } else {

          static const format fmt3("pro %5d %4d %11d %11s %11.3f %11.3f");

          // Iterate over the sensor data samples.
          for(size_t i = 0; i < max_num_samples(); i++) {
            if(i % 50 == 0) {
              os << endl
                 << "#---------------------------------------------------------------------------------------------------"
                 << endl
                 << "#      SN dive   sampleIdx        pres        temp        saln"
                 << (corr ? "    salnCorr" : "")
                 << endl
                 << "#                               (dBar)      (degC)       (PSU)"
                 << (corr ? "       (PSU)" : "");
            }

              os << endl
                 << format(fmt3) % id_inst % id_dive % i % XMsgFormatPressure(SamplePresAsFloat(i, pg, po, pc))
                     % SampleTempAsFloat(i, tg, to) % SampleSalnAsFloat(i, sg, so);
              if(corr)
                os << format(fmt_corr) % SampleCorrectedSalnAsFloat(i, sg, so);
          }
        }

      }CATCH_BLOCK("Failed to generate listing string for composite profile data (binned)!")

      return os.str();
    }

    /** Generate a description of the columns in the binned profile listing (PRD/prd lines).
     *
     *  Only the columns present for this dive are described. Each prd line gives the column name
     *  and its units (- if none); the description follows the comment character.
     *
     *  @param[in]  id_inst The instrument serial number.
     *  @param[in]  id_dive The dive number.
     *
     *  @throw      log::MessageException if an error occurs while generating the string.
     *
     *  @return     The column descriptions.
     */
    const string GenerateColumnDescriptionString(int id_inst, int id_dive) const {
      ostringstream os;

      try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        // Column name, units, description.
        vector<vector<string>> cols;
        cols.push_back({"sampleIdx", "-", "Bin index, in the order the float reports the bins."});
        cols.push_back({"pres", "dBar",
                        "Bin pressure, less the surface pressure offset (see Pressure Correction)."});

        if(HasAuxTempData()) {
          cols.push_back({"temp1", "degC", "CTD temperature."});
          cols.push_back({"temp2", "degC", "Auxiliary temperature channel."});
        } else {
          cols.push_back({"temp", "degC", "CTD temperature."});
        }

        cols.push_back({"saln", "PSU", "Salinity reported by the CTD."});

        if(IsCorrectedSalnDataComplete())
          cols.push_back({"salnCorr", "PSU",
                          "Dynamically corrected salinity, computed on the float (mission parameter PPE)."});

        static const format fmt1("PRD %5d %4d %11d");
        static const format fmt2("prd %5d %4d %11s %8s  # %s");

        os << "#      SN dive  numColumns"
           << endl
           << format(fmt1) % id_inst % id_dive % cols.size()
           << endl
           << "#---------------------------------------------------------------------------------------------------"
           << endl
           << "#      SN dive      column    units  description";

        // Wrap the descriptions so listing lines stay under 100 characters. The prd fields take
        // 39 characters; continuation lines are comments aligned with the description.
        static const size_t DESC_INDENT = 39;
        static const size_t DESC_WIDTH = 99 - DESC_INDENT;

        for(const auto& c : cols) {

          vector<string> rows;
          istringstream words(c[2]);
          string word, row;
          while(words >> word) {
            if(!row.empty() && row.size() + 1 + word.size() > DESC_WIDTH) {
              rows.push_back(row);
              row.clear();
            }
            row += (row.empty() ? "" : " ") + word;
          }
          rows.push_back(row);

          os << endl << format(fmt2) % id_inst % id_dive % c[0] % c[1] % rows.at(0);
          for(size_t k = 1; k < rows.size(); k++)
            os << endl << "#" << string(DESC_INDENT - 1, ' ') << rows.at(k);
        }

        os << endl << "#   Values outside the valid range are listed as -9.999.";

      }CATCH_BLOCK("Failed to generate profile column descriptions!")

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
