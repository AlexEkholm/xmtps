/** The @ref util::xmsg::XEngAscent class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 2:02:59 PM
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


#ifndef XENGASCENT_H_
#define XENGASCENT_H_

/* Includes */

// local
#include "XAlamoEngineering.h"

namespace util {
namespace xmsg {

/* Class Definitions */

/** Class representing an ascend state engineering X-Message.
 *
 *  @note           This X-Message is only used by ALAMO-type instruments.
 *  @note           This message type was added in X-Message version 2.1.
 */
class XEngAscent:
        public XAlamoEngineering {

private:

    /* Data Members */

    /** The measured internal pres case vacuum, in 0.01 inHg, at the start of the
     *  ascent/profile phase of the dive cycle.
     *
     *  @note This field was added in version 0x11.
     *
     */
    uint16_t case_vac_at_start_cinhg_;

    /** The measured internal pres case vacuum, in 0.01 inHg, at the end of the
     *  ascent/profile phase of the dive cycle.
     *
     *  @note This field was added in version 0x11.
     *
     */
    uint16_t case_vac_at_end_cinhg_;

    uint16_t fall_start_pres_counts_;

    ptime fall_start_time_;

    /** The measured system current, in mA.
     *
     *  @note This field is only used for message version 0x10. It was removed in version 0x11.
     */
    uint16_t system_current_ma_;

    /** The cumulative pump run time during the ascent phase of the dive cycle, in seconds.
     *
     *  @note This field is only used for message version 0x10. It was removed in version 0x11.
     */
    uint16_t pump_run_duration_;

public:

    /* Member Functions */

    /** Parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XEngAscent(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XAlamoEngineering(dive, whoi, data) {
        DecodeXMsgData(data);    // Decode message.
    }

    /** Destructor.
     *
     */
    ~XEngAscent() {
        /* Do nothing. */
    }

    /** Decode the ascent state engineering X-Message from the binary data.
     *
     *  @param[in]      data            The binary message to decode.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {
        try {
            // Check message.
            const uint8_t* d = CheckXMsgData(data, 0xE9, 13);

            // Decode message.
            set_engineering_version(d[3]);
            switch(engineering_version()) {

            case 0x10: // Engineering (Ascent) version 0x10.

                if(data.size() != 13) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 13 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                set_start_time(s2000(d, 4));
                set_system_current_ma(u16(d, 8));
                set_pump_run_duration(u16(d, 10));

                break;

            case 0x11: // Engineering (Ascent) version 0x10.

                if(data.size() != 13) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 13 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                set_start_time(s2000(d, 4));
                set_case_vac_at_start_cinhg(u16(d, 8));
                set_case_vac_at_end_cinhg(u16(d, 10));

                break;

            case 0x12: // Engineering (Ascent) version 0x12.

                if(data.size() != 19) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 19 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                set_start_time(s2000(d, 4));
                set_case_vac_at_start_cinhg(u16(d, 8));
                set_case_vac_at_end_cinhg(u16(d, 10));
                set_fall_start_time(s2000(d, 12));
                set_fall_start_pres_counts(u16(d, 16));

                break;

            default: { // Unsupported Engineering (Ascent) version.

                ostringstream os;
                os << "Unsupported ALAMO Engineering (Ascent) message version: 0x"
                   << hex
                   << (int) engineering_version()
                   << "!";
                EXC_THROW(os.str());
            }

                break;
            }
        }CATCH_BLOCK("Failed to ALAMO Engineering (Ascent) X-Message!")
    }

    /** Get the listing entry string for this ascent engineering data X-Message
     *
     *  @param[in]  pg      The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po      The pressure sensor offset, in dBar.
     *  @param[in]  sg      The salinity sensor sensitivity scale factor, in LSB/PSU.
     *  @param[in]  so      The salinity sensor offset, in PSU.
     *  @param[in]  tg      The temperature sensor sensitivity scale factor, in LSB/degC.
     *  @param[in]  to      The temperature sensor offset, in degC.
     *
     *  @return         The listing entry string for this ascent engineering data X-Message
     */
    const string GenerateListingString(
        uint16_t pg,
        uint16_t po,
        uint16_t sg,
        uint16_t so,
        uint16_t tg,
        uint16_t to) const {

      (void) sg;
      (void) so;
      (void) tg;
      (void) to;

      ostringstream os;

      switch(engineering_version()) {

        case 0x10: { // Engineering (Ascent) version 0x10.

          // Minor index 0.
          static const format fmt1("#   %5s %4s %4s %6s %20s %9s %9s");
          static const format fmt2("#   %5s %4s %4s %6s %20s %9s %9s");
          static const format fmt3("E09 %5d %4d %4d   0x%-02X %20s %9.3f %9d");
          os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-ascent" % "sysI"
                 % "pumpT"
             << endl;
          os << format(fmt2) % "" % "" % "" % "" % "" % "(A)" % "(sec)" << endl;
          os << format(fmt3) % id_instrument() % id_dive() % 0 % (int) engineering_version()
              % GenerateIso8601String(start_time()) % system_current_a() % pump_run_duration();
        }
          break;

        case 0x11: { // Engineering (Ascent) version 0x11.

          // Minor index 0.
          static const format fmt1("#   %5s %4s %4s %6s %20s %9s %9s");
          static const format fmt2("#   %5s %4s %4s %6s %20s %9s %9s");
          static const format fmt3("E09 %5d %4d %4d   0x%-02X %20s %9.2f %9.2f");
          os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-ascent" % "vac0"
                 % "vac1"
             << endl;
          os << format(fmt2) % "" % "" % "" % "" % "" % "(inHg)" % "(inHg)" << endl;
          os << format(fmt3) % id_instrument() % id_dive() % 0 % (int) engineering_version()
              % GenerateIso8601String(start_time()) % case_vac_at_start_inhg()
              % case_vac_at_end_inhg();
        }
          break;

        case 0x12: { // TODO Engineering (Ascent) version 0x12.

          { // Minor index 0.
            static const format fmt1("#   %5s %4s %4s %6s %20s %9s %9s");
            static const format fmt2("#   %5s %4s %4s %6s %20s %9s %9s");
            static const format fmt3("E09 %5d %4d %4d   0x%-02X %20s %9.2f %9.2f");
            os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-ascent" % "vac0"
                   % "vac1"
               << endl;
            os << format(fmt2) % "" % "" % "" % "" % "" % "(inHg)" % "(inHg)" << endl;
            os << format(fmt3) % id_instrument() % id_dive() % 0 % (int) engineering_version()
                % GenerateIso8601String(start_time()) % case_vac_at_start_inhg()
                % case_vac_at_end_inhg() << endl;
          }
          { // Minor index 1.
            static const format fmt1("#   %5s %4s %4s %20s %9s");
            static const format fmt2("#   %5s %4s %4s %20s %9.2f");
            static const format fmt3("E09 %5d %4d %4d %20s %9s");
            os << format(fmt1) % "SN" % "dive" % "midx" % "start-of-fall" % "fallP" << endl;
            os << format(fmt2) % "" % "" % "" % "" % "(dBar)" << endl;
            os << format(fmt3) % id_instrument() % id_dive() % 1
                % GenerateIso8601String(fall_start_time()) % fall_start_pres_dbar(pg, po);
          }

        }
          break;

        default: { // Unsupported Engineering (Ascent) version.
          ostringstream os;
          os << "Unsupported ALAMO Engineering (Ascent) message version: 0x"
             << hex
             << (int) engineering_version()
             << "!";
          EXC_THROW(os.str());
        }
          break;
      }

      return os.str();
    }

public:

    /* Member Functions - Accessor Methods: */

    uint16_t case_vac_at_end_cinhg() const {
        return case_vac_at_end_cinhg_;
    }

    uint16_t case_vac_at_start_cinhg() const {
        return case_vac_at_start_cinhg_;
    }

    template<typename T = float> T case_vac_at_start_inhg() const {
        return (T) case_vac_at_start_cinhg() / 100.;
    }

    template<typename T = float> T case_vac_at_end_inhg() const {
        return (T) case_vac_at_end_cinhg() / 100.;
    }

    uint16_t fall_start_pres_counts() const {
      return fall_start_pres_counts_;
    }

    template<typename T = float> T fall_start_pres_dbar(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) fall_start_pres_counts() / (T) gain - (T) offs : (T) XMSG_SENS_BAD_PRES;

        if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
            return XMSG_SENS_BAD_PRES;

        return val;
    }

    const ptime& fall_start_time() const {
      return fall_start_time_;
    }

    uint16_t pump_run_duration() const {
        return pump_run_duration_;
    }

    uint16_t system_current_ma() const {
        return system_current_ma_;
    }

    /** Get the system current, in amps.
     *
     *  @return         The system current, in amps.
     */
    template<typename T = float> T system_current_a() const {
        return (T) system_current_ma_ / 1000.;
    }

protected:

    /* Member Functions - Mutator Methods: */

    void set_case_vac_at_end_cinhg(uint16_t case_vac_at_end_cinhg) {
        case_vac_at_end_cinhg_ = case_vac_at_end_cinhg;
    }

    void set_case_vac_at_start_cinhg(uint16_t case_vac_at_start_cinhg) {
        case_vac_at_start_cinhg_ = case_vac_at_start_cinhg;
    }

    void set_fall_start_pres_counts(uint16_t fall_start_pres) {
      fall_start_pres_counts_ = fall_start_pres;
    }

    void set_fall_start_time(const ptime& fall_start_time) {
      fall_start_time_ = fall_start_time;
    }

    void set_pump_run_duration(uint16_t pump_run_duration) {
        pump_run_duration_ = pump_run_duration;
    }

    void set_system_current_ma(uint16_t system_current_ma) {
        system_current_ma_ = system_current_ma;
    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XENGASCENT_H_ */
