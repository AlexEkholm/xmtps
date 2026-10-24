/** The @ref util::xmsg::XEngDrift class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 2:00:39 PM
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

#ifndef XENGDRIFT_H_
#define XENGDRIFT_H_

/* Includes */

#include "XAlamoEngineering.h"

namespace util {
namespace xmsg {

/** Class representing an drift/park state engineering X-Message.
 *
 *  @note           This X-Message is only used by ALAMO-type instruments.
 *  @note           This message type was introduced in the X-Message format specification
 *                  version 2.1.
 */
class XEngDrift:
        public XAlamoEngineering {

private:

    /* Data Members */

    /** The average pressure during the drift/park phase of the dive cycle, in raw pressure counts.
     */
    uint16_t average_drift_pressure_counts_;

    uint16_t average_drift_salinity_counts_;

    /** The average temperature during the drift/park phase of the dive cycle, in raw temperature
     *  counts.
     */
    uint16_t average_drift_temperature_counts_;

    /** The measured internal pressure case vacuum, in 0.01 inHg, at the start of the
     *  drift/park phase of the dive cycle.
     *
     *  @note This field was added in version 0x11.
     *
     */
    uint16_t case_vac_at_start_cinhg_;

    /** The measured internal pressure case vacuum, in 0.01 inHg, at the end of the
     *  drift/park phase of the dive cycle.
     *
     *  @note This field was added in version 0x11.
     *
     */
    uint16_t case_vac_at_end_cinhg_;

    uint8_t num_seek_attempts_;

    int16_t first_pump_duration_adjustment_s_;

    /** The measured pressure at the start of the fall-to-profile phase of the dive cycle, in raw
     *  pressure counts.
     */
    uint16_t start_of_fall_to_profile_pressure_counts_;

    /** The start time of the fall-to-profile phase of the dive cycle. */
    ptime start_of_fall_to_profile_pressure_time_;

protected:

    /* Protected Member Functions */

    void set_average_drift_pressure_counts(uint16_t average_drift_pressure_counts) {
        average_drift_pressure_counts_ = average_drift_pressure_counts;
    }

    void set_average_drift_salinity_counts(uint16_t average_drift_salinity_counts) {
        average_drift_salinity_counts_ = average_drift_salinity_counts;
    }

    void set_average_drift_temperature_counts(uint16_t average_drift_temperature_counts) {
        average_drift_temperature_counts_ = average_drift_temperature_counts;
    }

    void set_case_vac_at_end_cinhg(uint16_t case_vac_at_end_cinhg) {
        case_vac_at_end_cinhg_ = case_vac_at_end_cinhg;
    }

    void set_case_vac_at_start_cinhg(uint16_t case_vac_at_start_cinhg) {
        case_vac_at_start_cinhg_ = case_vac_at_start_cinhg;
    }

    void set_first_pump_duration_adjustment_s(int16_t first_pump_duration_adjustment_s) {
        first_pump_duration_adjustment_s_ = first_pump_duration_adjustment_s;
    }

    void set_num_seek_attempts(uint8_t num_seek_attempts) {
        num_seek_attempts_ = num_seek_attempts;
    }

    void set_start_of_fall_to_profile_pressure_counts(
            uint16_t start_of_fall_to_profile_pressure_counts) {
        start_of_fall_to_profile_pressure_counts_ = start_of_fall_to_profile_pressure_counts;
    }

    void set_start_of_fall_to_profile_pressure_time(
            const ptime& start_of_fall_to_profile_pressure_time) {
        start_of_fall_to_profile_pressure_time_ = start_of_fall_to_profile_pressure_time;
    }

public:

    /* Public Member Functions */

    /** Parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XEngDrift(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XAlamoEngineering(dive, whoi, data),
                    average_drift_pressure_counts_(XMSG_BAD_U16),
                    average_drift_salinity_counts_(XMSG_BAD_U16),
                    average_drift_temperature_counts_(XMSG_BAD_U16),
                    case_vac_at_start_cinhg_(XMSG_BAD_U16),
                    case_vac_at_end_cinhg_(XMSG_BAD_U16),
                    num_seek_attempts_(XMSG_BAD_U08),
                    first_pump_duration_adjustment_s_(XMSG_BAD_I16),
                    start_of_fall_to_profile_pressure_counts_(XMSG_BAD_U16),
                    start_of_fall_to_profile_pressure_time_(not_a_date_time) {
        DecodeXMsgData(data); // Decode message.
    }

    /** Destructor.
     *
     */
    ~XEngDrift() {
        /* Do nothing. */
    }

    /** Decode the drift/park state engineering X-Message from the binary data.
     *
     *  @param[in]      data            The binary message to decode.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {
        try {
            // Check message.
            const uint8_t* d = CheckXMsgData(data, 0xE8, 19);

            set_engineering_version(d[3]);
            switch(engineering_version()) {

            case 0x10: { // Engineering (Drift) message version 0x10.

              static const unsigned int EXPECTED_SIZE = 19;

              if(data.size() != EXPECTED_SIZE) {
                ostringstream os;
                os << "Invalid X-Message: Expected"
                   << EXPECTED_SIZE
                   << "bytes, but received "
                   << data.size()
                   << " bytes!";
                EXC_THROW(os.str());
              }
              set_start_time(s2000(d, 4));
              set_average_drift_pressure_counts(u16(d, 8));
              set_average_drift_temperature_counts(u16(d, 10));
              set_start_of_fall_to_profile_pressure_time(s2000(d, 12));
              set_start_of_fall_to_profile_pressure_counts(u16(d, 16));

            }

            break;

            case 0x11: { // Engineering (Drift) message version 0x11.

              static const unsigned int EXPECTED_SIZE = 28;

              if(data.size() != EXPECTED_SIZE) {
                ostringstream os;
                os << "Invalid X-Message: Expected"
                   << EXPECTED_SIZE
                   << "bytes, but received "
                   << data.size()
                   << " bytes!";
                EXC_THROW(os.str());
              }

              set_start_time(s2000(d, 4));
              set_case_vac_at_start_cinhg(u16(d, 8));
              set_case_vac_at_end_cinhg(u16(d, 10));
              set_average_drift_pressure_counts(u16(d, 12));
              set_average_drift_temperature_counts(u16(d, 14));
              set_average_drift_salinity_counts(u16(d, 16));
              set_num_seek_attempts(u08(d, 18));
              set_first_pump_duration_adjustment_s(u16(d, 19));
              set_start_of_fall_to_profile_pressure_time(s2000(d, 21));
              set_start_of_fall_to_profile_pressure_counts(u16(d, 25));
            }

            break;

            case 0x12: { // Engineering (Drift) message version 0x12.

              static const unsigned int EXPECTED_SIZE = 22;

              if(data.size() != EXPECTED_SIZE) {
                  ostringstream os;
                  os << "Invalid X-Message: Expected"
                     << EXPECTED_SIZE
                     << "bytes, but received "
                     << data.size()
                     << " bytes!";
                  EXC_THROW(os.str());
              }

              set_start_time(s2000(d, 4));
              set_case_vac_at_start_cinhg(u16(d, 8));
              set_case_vac_at_end_cinhg(u16(d, 10));
              set_average_drift_pressure_counts(u16(d, 12));
              set_average_drift_temperature_counts(u16(d, 14));
              set_average_drift_salinity_counts(u16(d, 16));
              set_num_seek_attempts(u08(d, 18));
              set_first_pump_duration_adjustment_s(u16(d, 19));

            }

              break;

            default: { // Unsupported Engineering (Drift) message version.

                ostringstream os;
                os << "Unsupported ALAMO Engineering (Drift) message version: 0x"
                   << hex
                   << (int) engineering_version()
                   << "!";
                EXC_THROW(os.str());
            }

                break;
            }
            // Decode the message.
            set_engineering_version(d[3]);
            if(engineering_version() == 0x10) {

            }

        }
        CATCH_BLOCK("Failed to decode ALAMO Engineering (Drift) message!")
    }

    /** Get the listing entry string for this drift/park engineering data X-Message.
     *
     *  @param[in]      pg          The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]      po          The pressure sensor offset, in dBar.
     *  @param[in]      sg          The salinity sensor sensitivity scale factor, in LSB/PSU.
     *  @param[in]      so          The salinity sensor offset, in PSU.
     *  @param[in]      tg          The temperature sensor sensitivity scale factor, in LSB/degC.
     *  @param[in]      to          The temperatyre sensor offset, in degC.
     *
     *  @return         The listing entry string for this drift/park engineering data X-Message.
     */
    const string GenerateListingString(
            uint16_t pg,
            uint16_t po,
            uint16_t sg,
            uint16_t so,
            uint16_t tg,
            uint16_t to) const {
        ostringstream os;

        try {

            switch(engineering_version()) {

            case 0x10: { // Engineering (Drift) version 0x10.

                // Minor Index 0.
                static const format fmt1("#   %5s %4s %4s %6s %20s %7s %7s %20s %7s");
                static const format fmt2("#   %5s %4s %4s %6s %20s %7s %8s %20s %7s");
                static const format fmt3("E08 %05d %4d %4d   0x%-02X %20s %7s %7.3f %20s %7s");
                os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-drift" % "meanP"
                           % "meanT" % "start-of-fall" % "fallP"
                   << endl;
                os << format(fmt2) % "" % "" % "" % "" % "" % "(dBar)" % "(degC)" % "" % "(dBar)"
                   << endl;
                os << format(fmt3) % id_instrument() % id_dive() % 0 % (int) engineering_version()
                        % GenerateIso8601String(start_time()) % XMsgFormatPressure(average_drift_pressure_dbar(pg, po))
                        % average_drift_temperature_c(tg, to)
                        % GenerateIso8601String(start_of_fall_to_profile_pressure_time_)
                        % XMsgFormatPressure(start_of_fall_to_profile_pressure_dbar(pg, po));

            }
                break;

            case 0x11: { // Engineering (Drift) version 0x11.

                // Minor index 0.
                static const format fmt1("#   %5s %4s %4s %6s %20s %9s %9s %15s %15s");
                static const format fmt2("#   %5s %4s %4s %6s %20s %9s %9s %15s %15s");
                static const format fmt3("E08 %05d %4d %4d   0x%-02X %20s %9.2f %9.2f %15d %+15d");
                os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-drift" % "vac0"
                           % "vac1" % "seeks" % "seekPumpAdjT"
                   << endl;
                os << format(fmt2) % "" % "" % "" % "" % "" % "(inHg)" % "(inHg)" % "" % "(+sec)"
                   << endl;
                os << format(fmt3) % id_instrument() % id_dive() % 0 % (int) engineering_version()
                           % GenerateIso8601String(start_time()) % case_vac_at_start_inhg()
                           % case_vac_at_end_inhg() % (int) num_seek_attempts()
                           % first_pump_duration_adjustment_s()
                   << endl;

                os << "# --- Drift Sensor Averages ------------------------------------------------"
                        "------------------------"
                   << endl;

                // Minor Index 1.
                static const format fmt4("#   %5s %4s %4s    %12s %12s %12s %12s %12s %12s");
                static const format fmt5("#   %5s %4s %4s    %12s %13s %12s %12s %12s %12s");
                static const format fmt6("E08 %05d %4d %4d    %12s %12d %12.3f %12d %12.3f %12d");
                os << format(fmt4) % "SN" % "dive" % "midx" % "pres" % "pres" % "temp" % "temp"
                           % "saln" % "saln"
                   << endl;
                os << format(fmt5) % "" % "" % "" % "(dBar)" % "(counts)" % "(degC)" % "(counts)"
                           % "(PSU)" % "(counts)"
                   << endl;
                os << format(fmt6) % id_instrument() % id_dive() % 1
                        % XMsgFormatPressure(average_drift_pressure_dbar(pg, po)) % average_drift_pressure_counts()
                        % average_drift_temperature_c(tg, to) % average_drift_temperature_counts()
                        % average_drift_salinity_psu(sg, so) % average_drift_salinity_counts();
            }
                break;

            case 0x12: { // Engineering (Drift) version 0x12.

              // Minor index 0.
              static const format fmt1("#   %5s %4s %4s %6s %20s %9s %9s %15s %15s");
              static const format fmt2("#   %5s %4s %4s %6s %20s %9s %9s %15s %15s");
              static const format fmt3("E08 %05d %4d %4d   0x%-02X %20s %9.2f %9.2f %15d %+15d");
              os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-drift" % "vac0"
                         % "vac1" % "seeks" % "seekPumpAdjT"
                 << endl;
              os << format(fmt2) % "" % "" % "" % "" % "" % "(inHg)" % "(inHg)" % "" % "(+sec)"
                 << endl;
              os << format(fmt3) % id_instrument() % id_dive() % 0 % (int) engineering_version()
                         % GenerateIso8601String(start_time()) % case_vac_at_start_inhg()
                         % case_vac_at_end_inhg() % (int) num_seek_attempts()
                         % first_pump_duration_adjustment_s()
                 << endl;

              os << "# --- Drift Sensor Averages ------------------------------------------------"
                      "------------------------"
                 << endl;

              // Minor Index 1.
              static const format fmt4("#   %5s %4s %4s    %12s %12s %12s %12s %12s %12s");
              static const format fmt5("#   %5s %4s %4s    %12s %13s %12s %12s %12s %12s");
              static const format fmt6("E08 %05d %4d %4d    %12s %12d %12.3f %12d %12.3f %12d");
              os << format(fmt4) % "SN" % "dive" % "midx" % "pres" % "pres" % "temp" % "temp"
                         % "saln" % "saln"
                 << endl;
              os << format(fmt5) % "" % "" % "" % "(dBar)" % "(counts)" % "(degC)" % "(counts)"
                         % "(PSU)" % "(counts)"
                 << endl;
              os << format(fmt6) % id_instrument() % id_dive() % 1
                      % XMsgFormatPressure(average_drift_pressure_dbar(pg, po)) % average_drift_pressure_counts()
                      % average_drift_temperature_c(tg, to) % average_drift_temperature_counts()
                      % average_drift_salinity_psu(sg, so) % average_drift_salinity_counts();
            }
              break;


            default: { // Unsupported Engineering (Drift) version.
                ostringstream os;
                os << "Unsupported ALAMO Engineering (Drift) message version: 0x"
                   << hex
                   << (int) engineering_version()
                   << "!";
                EXC_THROW(os.str());
            }
                break;
            }

        }CATCH_BLOCK("Failed to generate listing string for ALAMO Engineering (Drift) message!")

        return os.str();
    }

    uint16_t average_drift_pressure_counts() const {
        return average_drift_pressure_counts_;
    }

    uint16_t average_drift_salinity_counts() const {
        return average_drift_salinity_counts_;
    }

    uint16_t average_drift_temperature_counts() const {
        return average_drift_temperature_counts_;
    }

    uint16_t case_vac_at_end_cinhg() const {
        return case_vac_at_end_cinhg_;
    }

    template<typename T = float> T case_vac_at_end_inhg() const {
        return (T) case_vac_at_end_cinhg() / 100.;
    }

    template<typename T = float> T case_vac_at_start_inhg() const {
        return (T) case_vac_at_start_cinhg() / 100.;
    }

    uint16_t case_vac_at_start_cinhg() const {
        return case_vac_at_start_cinhg_;
    }

    int16_t first_pump_duration_adjustment_s() const {
        return first_pump_duration_adjustment_s_;
    }

    uint8_t num_seek_attempts() const {
        return num_seek_attempts_;
    }

    uint16_t start_of_fall_to_profile_pressure_counts() const {
        return start_of_fall_to_profile_pressure_counts_;
    }

    const ptime& start_of_fall_to_profile_pressure_time() const {
        return start_of_fall_to_profile_pressure_time_;
    }

    template<typename T = double> T start_of_fall_to_profile_pressure_dbar(
            int16_t gain,
            int16_t offs) const {
        T val;

        val = gain ? (T) start_of_fall_to_profile_pressure_counts() / (T) gain - (T) offs :
        XMSG_SENS_BAD_PRES;

        if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
            return XMSG_SENS_BAD_PRES;

        return val;
    }

    template<typename T = double> T average_drift_pressure_dbar(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) average_drift_pressure_counts() / (T) gain - (T) offs : XMSG_SENS_BAD_PRES;

        if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
            return XMSG_SENS_BAD_PRES;

        return val;
    }

    template<typename T = double> T average_drift_temperature_c(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) average_drift_temperature_counts() / (T) gain - (T) offs :
        XMSG_SENS_BAD_TEMP;

        if(val < XMSG_SENS_MIN_TEMP || val > XMSG_SENS_MAX_TEMP)
            return XMSG_SENS_BAD_TEMP;

        return val;
    }

    template<typename T = double> T average_drift_salinity_psu(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) average_drift_salinity_counts() / (T) gain - (T) offs : XMSG_SENS_BAD_SALN;

        if(val < XMSG_SENS_MIN_SALN || val > XMSG_SENS_MAX_SALN)
            return XMSG_SENS_BAD_SALN;

        return val;
    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XENGDRIFT_H_ */
