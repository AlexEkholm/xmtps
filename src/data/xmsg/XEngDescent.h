/** The @ref util::xmsg::XEngDescent class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 2:00:18 PM
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

#ifndef XENGDESCENT_H_
#define XENGDESCENT_H_

/* Includes */

#include "XAlamoEngineering.h"

namespace util {
namespace xmsg {

/* Class Definition */

/** Class representing an descend state engineering X-Message.
 *
 *  @note           This X-Message is only used by ALAMO-type instruments.
 *  @note           This message type was added in X-Message version 2.1.
 */
class XEngDescent:
        public XAlamoEngineering {

private:

    /* Data Members */

    /** The air valve middle position at surface, in .001 V.
     *
     *  @note This field was added in version 0x11.
     */
    uint16_t air_valve_middle_position_at_surface_mv_;

    /** The measured internal pressure case vacuum, in 0.01 inHg, at the start of the
     *  descent-to-drift/park-pressure phase of the dive cycle.
     *
     *  @note This field was added in version 0x11.
     *
     */
    uint16_t case_vac_at_start_cinhg_;

    /** The measured internal pressure case vacuum, in 0.01 inHg, at the end of the
     *  descent-to-drift/park-pressure phase of the dive cycle.
     *
     *  @note This field was added in version 0x11.
     *
     */
    uint16_t case_vac_at_end_cinhg_;

    /** The measured internal pressure case vacuum, in 0.01 inHg.
     *
     *  @note This field is only used for message version 0x10. It was removed in version 0x11.
     *
     */
    uint16_t case_vac_cinhg_;

    /** The the first-pump run time adjustment, in seconds.
     *
     */
    int16_t first_pump_duration_adjustment_s_;

    /** The number of seek attempts.
     *
     */
    uint8_t num_seek_attempts_;

    /** The oil valve open position at surface, in .001 V.
     *
     *  @note This field was added in version 0x11.
     */
    uint16_t oil_valve_open_position_at_surface_mv_;

    /** The oil valve closed position during fall pump, in .001 V.
     *
     *  @todo Clarify the description of the "oil valve position during fall pump" field.
     *
     *  @note This field was added in version 0x11.
     *
     *
     */
    uint16_t oil_valve_closed_position_during_fall_pump_mv_;

    /** The measured pump current, in milliamps.
     *
     *  @note This field is only used for message version 0x10. It was removed in version 0x11.
     *
     */
    uint16_t pump_current_ma_;

    /** The cumulative pump run time for the descent state of the dive cycle, in seconds.
     *
     *  @note This field is only used for message version 0x10. It was removed in version 0x11.
     *
     */
    uint16_t pump_duration_sec_;

    /** The measured system current, in milliamps.
     *
     *  @note This field is only used for message version 0x10. It was removed in version 0x11.
     */
    uint16_t system_current_ma_;

    /** The measured system current, in millivolts.
     *
     *  @note This field is only used for message version 0x10. It was removed in version 0x11.
     */
    uint16_t system_voltage_mv_;

protected:

    /* Member Functions */

    void set_air_valve_middle_position_at_surface_mv(
            uint16_t air_valve_middle_position_at_surface_mv) {
        air_valve_middle_position_at_surface_mv_ = air_valve_middle_position_at_surface_mv;
    }

    void set_case_vac_at_end_cinhg(uint16_t case_vac_at_end_cinhg) {
        case_vac_at_end_cinhg_ = case_vac_at_end_cinhg;
    }

    void set_case_vac_at_start_cinhg(uint16_t case_vac_at_start_cinhg) {
        case_vac_at_start_cinhg_ = case_vac_at_start_cinhg;
    }

    void set_case_vac_cinhg(uint16_t case_vac_cinhg) {
        case_vac_cinhg_ = case_vac_cinhg;
    }

    void set_num_seek_attempts(uint8_t num_seek_attempts) {
        num_seek_attempts_ = num_seek_attempts;
    }

    void set_oil_valve_closed_position_during_fall_pump_mv(
            uint16_t oil_valve_closed_position_during_fall_pump_mv) {
        oil_valve_closed_position_during_fall_pump_mv_ = oil_valve_closed_position_during_fall_pump_mv;
    }

    void set_oil_valve_open_position_at_surface_mv(uint16_t oil_valve_open_position_at_surface_mv) {
        oil_valve_open_position_at_surface_mv_ = oil_valve_open_position_at_surface_mv;
    }

    void set_pump_current_ma(uint16_t pump_current_ma) {
        pump_current_ma_ = pump_current_ma;
    }

    void set_pump_duration_sec(uint16_t pump_duration_sec) {
        pump_duration_sec_ = pump_duration_sec;
    }

    void set_first_pump_duration_adjustment_s(int16_t first_pump_duration_adjustment_s) {
        first_pump_duration_adjustment_s_ = first_pump_duration_adjustment_s;
    }

    void set_system_current_ma(uint16_t system_current_ma) {
        system_current_ma_ = system_current_ma;
    }

    void set_system_voltage_mv(uint16_t system_voltage_mv) {
        system_voltage_mv_ = system_voltage_mv;
    }

public:

    /* Member Functions */

    /** The parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XEngDescent(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XAlamoEngineering(dive, whoi, data),
                    air_valve_middle_position_at_surface_mv_(XMSG_BAD_U16),
                    case_vac_at_start_cinhg_(XMSG_BAD_U16),
                    case_vac_at_end_cinhg_(XMSG_BAD_U16),
                    case_vac_cinhg_(XMSG_BAD_U16),
                    first_pump_duration_adjustment_s_(XMSG_BAD_I16),
                    num_seek_attempts_(XMSG_BAD_U08),
                    oil_valve_open_position_at_surface_mv_(XMSG_BAD_U16),
                    oil_valve_closed_position_during_fall_pump_mv_(XMSG_BAD_U16),
                    pump_current_ma_(XMSG_BAD_U16),
                    pump_duration_sec_(XMSG_BAD_U16),
                    system_current_ma_(XMSG_BAD_U16),
                    system_voltage_mv_(XMSG_BAD_U16) {
        DecodeXMsgData(data); // Decode message.
    }

    /** The destructor.
     *
     */
    ~XEngDescent() {
        /* Do nothing. */
    }

    /** Decode the descent state engineering X-Message from the binary data.
     *
     *  @param[in]      data            The binary message to decode.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {
        try {
            // Check message.
            const uint8_t* d = CheckXMsgData(data, 0xE7, 22);

            set_engineering_version(d[3]);
            switch(engineering_version()) {

            case 0x10: // Engineering (Descent) version 0x10.

                // Check the message size.
                if(data.size() != 23 && data.size() != 27) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 23 or 27 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                set_start_time(s2000(d, 4));
                set_pump_duration_sec(u16(d, 8));
                set_pump_current_ma(u16(d, 10));
                set_system_current_ma(u16(d, 12));
                if(xmsg_size_bytes() == 27) {
                    set_system_voltage_mv(u16(d, 14));
                    set_case_vac_cinhg(u16(d, 16));
                    set_first_pump_duration_adjustment_s(i16(d, 18));
                }

                break;

            case 0x11: // Engineering (Descent) version 0x10.

                // Check the message size.
                if(data.size() != 22) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 22 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                set_start_time(s2000(d, 4));
                set_case_vac_at_start_cinhg(u16(d, 8));
                set_case_vac_at_end_cinhg(u16(d, 10));
                set_oil_valve_open_position_at_surface_mv(u16(d, 12));
                set_oil_valve_closed_position_during_fall_pump_mv(u16(d, 14));
                set_air_valve_middle_position_at_surface_mv(u16(d, 16));
                set_num_seek_attempts(u08(d, 18));
                set_first_pump_duration_adjustment_s(i16(d, 19));

                break;

            default: { // Unsupported Engineering (Descent) version.

                ostringstream os;
                os << "Unsupported ALAMO Engineering (Descent) message version: 0x"
                   << hex
                   << (int) engineering_version()
                   << "!";
                EXC_THROW(os.str());
            }

                break;
            }

        }CATCH_BLOCK("Failed to decode ALAMO Engineering (Descent) X-Message!")
    }

    /** Get the listing entry string for this descent engineering data X-Message
     *
     *  @return         The listing entry string for this descent engineering data X-Message
     */
    const string GenerateListingString() const {
        ostringstream os;

        try {

            switch(engineering_version()) {

            case 0x10: { // Engineering (Descent) version 0x10.

                static const format fmt1("#   %5s %4s %4s %6s %20s %5s %6s %6s %6s %6s %7s");
                static const format fmt2("#   %5s %4s %4s %6s %20s %5s %6s %6s %6s %7s%7s");
                static const format fmt3(
                        "E07 %05d %4d %4d   0x%-02X %20s %5d %6.3f %6.3f %6.3f %6.2f %+7d");
                os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-descent"
                           % "pumpT"

                           % "pumpI" % "sysI" % "sysV" % "vac" % "pumpAdj"
                   << endl;
                os << format(fmt2) % "" % "" % "" % "" % "" % "(sec)" % "(A)" % "(A)" % "(V)"
                           % "(inHg)" % "(+sec)"
                   << endl;
                os << format(fmt3) % id_instrument() % id_dive() % 0 % (int) engineering_version()
                        % GenerateIso8601String(start_time()) % pump_duration_sec()
                        % pump_current_a() % system_current_a() % system_voltage_v()
                        % case_vac_inhg() % first_pump_duration_adjustment_s();
            }
                break;

            case 0x11: { // Engineering (Descent) version 0x11.

                // Minor index 0.
                static const format fmt1("#   %5s %4s %4s %6s %20s %9s %9s %15s %15s");
                static const format fmt2("#   %5s %4s %4s %6s %20s %9s %9s %15s %15s");
                static const format fmt3("E07 %05d %4d %4d   0x%-02X %20s %9.2f %9.2f %15d %+15d");
                os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-descent" % "vac0"
                           % "vac1" % "seeks" % "seekPumpAdjT"
                   << endl;
                os << format(fmt2) % "" % "" % "" % "" % "" % "(inHg)" % "(inHg)" % "" % "(+sec)"
                   << endl;
                os << format(fmt3) % id_instrument() % id_dive() % 0 % (int) engineering_version()
                           % GenerateIso8601String(start_time()) % case_vac_at_start_inhg()
                           % case_vac_at_end_inhg() % (int) num_seek_attempts()
                           % first_pump_duration_adjustment_s()
                   << endl;

                // Minor index 1.
                static const format fmt4("#   %5s %4s %4s %24s %24s %29s");
                static const format fmt5("#   %5s %4s %4s %24s %24s %29s");
                static const format fmt6("E07 %05d %4d %4d %24.3f %24.3f %29.3f");
                os << "# --- Valve Position Measurements ------------------------------------------"
                        "------------------------"
                   << endl;
                os << format(fmt4) % "SN" % "dive" % "midx" % "OilValveOpenPosSurf"
                           % "OilValveClosePosFallPump" % "AirValveMidPosSurf"
                   << endl;
                os << format(fmt5) % "" % "" % "" % "(V)" % "(V)" % "(V)" << endl;
                os << format(fmt6) % id_instrument() % id_dive() % 1
                        % oil_valve_open_position_at_surface_v()
                        % oil_valve_closed_position_during_fall_pump_v()
                        % air_valve_middle_position_at_surface_v();
            }
                break;

            default: { // Unsupported Engineering (Descent) version.
                ostringstream os;
                os << "Unsupported ALAMO Engineering (Descent) message version: 0x"
                   << hex
                   << (int) engineering_version()
                   << "!";
                EXC_THROW(os.str());
            }
                break;
            }

        }CATCH_BLOCK("Failed to generate listing string for ALAMO Engineering (Descent) message!")

        return os.str();
    }

    uint16_t air_valve_middle_position_at_surface_mv() const {
        return air_valve_middle_position_at_surface_mv_;
    }

    uint16_t case_vac_at_end_cinhg() const {
        return case_vac_at_end_cinhg_;
    }

    uint16_t case_vac_at_start_cinhg() const {
        return case_vac_at_start_cinhg_;
    }

    uint16_t case_vac_cinhg() const {
        return case_vac_cinhg_;
    }

    uint8_t num_seek_attempts() const {
        return num_seek_attempts_;
    }

    uint16_t oil_valve_closed_position_during_fall_pump_mv() const {
        return oil_valve_closed_position_during_fall_pump_mv_;
    }

    uint16_t oil_valve_open_position_at_surface_mv() const {
        return oil_valve_open_position_at_surface_mv_;
    }

    uint16_t pump_current_ma() const {
        return pump_current_ma_;
    }

    uint16_t pump_duration_sec() const {
        return pump_duration_sec_;
    }

    int16_t first_pump_duration_adjustment_s() const {
        return first_pump_duration_adjustment_s_;
    }

    uint16_t system_current_ma() const {
        return system_current_ma_;
    }

    uint16_t system_voltage_mv() const {
        return system_voltage_mv_;
    }

    /** Get the measured pressure case vacuum, in inches of mercury.
     *
     *  @return         The measured pressure case vacuum, in inches of mercury.
     */
    template<typename T = float> T case_vac_inhg() const {
        return (T) case_vac_cinhg() / 100.;
    }

    template<typename T = float> T case_vac_at_start_inhg() const {
        return (T) case_vac_at_start_cinhg() / 100.;
    }

    template<typename T = float> T case_vac_at_end_inhg() const {
        return (T) case_vac_at_end_cinhg() / 100.;
    }

    template<typename T = float> T oil_valve_open_position_at_surface_v() const {
        return (T) oil_valve_open_position_at_surface_mv() / 1000.;
    }

    template<typename T = float> T oil_valve_closed_position_during_fall_pump_v() const {
        return (T) oil_valve_closed_position_during_fall_pump_mv() / 1000.;
    }

    template<typename T = float> T air_valve_middle_position_at_surface_v() const {
        return (T) air_valve_middle_position_at_surface_mv() / 1000.;
    }

    /** Get the measured pump current, in amperes.
     *
     *  @return         The measured pump current, in amperes.
     */
    template<typename T = float> T pump_current_a() const {
        return (T) pump_current_ma_ / 1000.;
    }

    /** Get the measured system current, in amperes.
     *
     *  @return         The measured system current, in amperes.
     */
    template<typename T = float> T system_current_a() const {
        return (T) system_current_ma_ / 1000.;
    }

    /** Get the measured system voltage, in volts.
     *
     *  @return         The measured system voltage, in volts.
     */
    template<typename T = float> T system_voltage_v() const {
        return (T) system_voltage_mv_ / 1000.;
    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XENGDESCENT_H_ */
