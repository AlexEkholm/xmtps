/** The @ref util::xmsg::XAlamoBit class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Aug 14, 2014 11:24:25 AM
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

#ifndef XALAMOBIT_H_
#define XALAMOBIT_H_

/* Includes */

#include "XMessage.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Class representing the BIT test of an ALAMO float.
 *
 */
class XAlamoBit {

private:

    /* Data Members */

    /** The Built-in-Test (BIT) version identifier.
     *
     */
    uint8_t bit_version_;

    /** The software version identifier.
     *
     */
    uint32_t float_software_version_;

    /** The Built-in-Test (BIT) status code.
     *
     */
    uint32_t bit_status_code_;

    uint16_t bit_duration_;

    uint16_t bit_pressure_;

    uint16_t bit_temperature_;

    uint16_t bit_salinity_;

    bool bit_passed_;

    uint32_t ctd_status_;

    uint16_t vac_at_start_of_bit_;

    uint16_t vac_at_end_of_bit_;

    uint16_t battery_voltage_at_start_of_bit_;

    uint16_t battery_voltage_at_end_of_bit_;

    uint16_t air_valve_open_position_;

    uint16_t air_valve_open_cal_position_;

    uint16_t air_valve_closed_position_;

    uint16_t air_valve_closed_cal_position_;

    uint16_t air_valve_middle_position_;

    uint16_t air_valve_middle_cal_position_;

    uint16_t air_pump_test_duration_;

    uint8_t air_pump_cycle_count_;

    uint16_t air_pressure_max_;

    uint16_t air_pump_max_load_current_;

    uint16_t air_pump_max_load_current_12v_;

    uint16_t air_pump_max_load_battery_voltage_;

    uint16_t oil_valve_open_position_;

    uint16_t oil_valve_open_cal_position_;

    uint16_t oil_valve_closed_position_;

    uint16_t oil_valve_closed_cal_position_;

    uint16_t oil_pump_test_duration_;

    uint16_t oil_pump_max_load_current_;

    uint16_t oil_pump_max_load_current_12v_;

    uint16_t oil_pump_max_load_battery_voltage_;

    uint16_t pres_counts_after_offset_reset_;

    uint16_t pres_counts_before_offset_reset_;

    /** The measured pressure offset, in 1/1000 dBar.
     *
     */
    uint16_t pres_offset_;

protected:

    /* Member Functions */

    /** Set the Built-in-Test (BIT) status code.
     *
     *  @param[in]  bit_status_code     The Built-in-Test (BIT) status code to set.
     */
    void set_bit_status_code(uint32_t bit_status_code) {
        bit_status_code_ = bit_status_code;
    }

    /** Set the Built-in-Test (BIT) version.
     *
     *  @param[in]  bit_version         The Built-in-Test (BIT) version to set.
     */
    void set_bit_version(uint8_t bit_version) {
        bit_version_ = bit_version;
    }

    /** Set the instrument software version.
     *
     *  @param[in]  float_software_version  The instrument software version to set.
     */
    void set_float_software_version(uint32_t float_software_version) {
        float_software_version_ = float_software_version;
    }

    void set_air_pressure_max(uint16_t air_pressure_max) {
        air_pressure_max_ = air_pressure_max;
    }

    void set_air_pump_cycle_count(uint8_t air_pump_cycle_count) {
        air_pump_cycle_count_ = air_pump_cycle_count;
    }

    void set_air_pump_max_load_battery_voltage(uint16_t air_pump_max_load_battery_voltage) {
        air_pump_max_load_battery_voltage_ = air_pump_max_load_battery_voltage;
    }

    void set_air_pump_max_load_current(uint16_t air_pump_max_load_current) {
        air_pump_max_load_current_ = air_pump_max_load_current;
    }

    void set_air_pump_max_load_current_12v(uint16_t air_pump_max_load_current_12v) {
        air_pump_max_load_current_12v_ = air_pump_max_load_current_12v;
    }

    void set_air_pump_test_duration(uint16_t air_pump_test_duration) {
        air_pump_test_duration_ = air_pump_test_duration;
    }

    void set_air_valve_closed_cal_position(uint16_t air_valve_closed_cal_position) {
        air_valve_closed_cal_position_ = air_valve_closed_cal_position;
    }

    void set_air_valve_closed_position(uint16_t air_valve_closed_position) {
        air_valve_closed_position_ = air_valve_closed_position;
    }

    void set_air_valve_middle_cal_position(uint16_t air_valve_middle_cal_position) {
        air_valve_middle_cal_position_ = air_valve_middle_cal_position;
    }

    void set_air_valve_middle_position(uint16_t air_valve_middle_position) {
        air_valve_middle_position_ = air_valve_middle_position;
    }

    void set_air_valve_open_cal_position(uint16_t air_valve_open_cal_position) {
        air_valve_open_cal_position_ = air_valve_open_cal_position;
    }

    void set_air_valve_open_position(uint16_t air_valve_open_position) {
        air_valve_open_position_ = air_valve_open_position;
    }

    void set_battery_voltage_at_end_of_bit(uint16_t battery_voltage_at_end_of_bit) {
        battery_voltage_at_end_of_bit_ = battery_voltage_at_end_of_bit;
    }

    void set_battery_voltage_at_start_of_bit(uint16_t battery_voltage_at_start_of_bit) {
        battery_voltage_at_start_of_bit_ = battery_voltage_at_start_of_bit;
    }

    void set_bit_duration(uint16_t bit_duration) {
        bit_duration_ = bit_duration;
    }

    void set_bit_passed(bool bit_passed) {
        bit_passed_ = bit_passed;
    }

    void set_bit_pressure(uint16_t bit_pressure) {
        bit_pressure_ = bit_pressure;
    }

    void set_bit_salinity(uint16_t bit_salinity) {
        bit_salinity_ = bit_salinity;
    }

    void set_bit_temperature(uint16_t bit_temperature) {
        bit_temperature_ = bit_temperature;
    }

    void set_ctd_status(uint32_t ctd_status) {
        ctd_status_ = ctd_status;
    }

    void set_oil_pump_max_load_current(uint16_t oil_pump_max_load_current) {
        oil_pump_max_load_current_ = oil_pump_max_load_current;
    }

    void set_oil_pump_max_load_current_12v(uint16_t oil_pump_max_load_current_12v) {
        oil_pump_max_load_current_12v_ = oil_pump_max_load_current_12v;
    }

    void set_oil_pump_max_load_battery_voltage(uint16_t oil_pump_max_load_battery_voltage) {
        oil_pump_max_load_battery_voltage_ = oil_pump_max_load_battery_voltage;
    }

    void set_oil_pump_test_duration(uint16_t oil_pump_test_duration) {
        oil_pump_test_duration_ = oil_pump_test_duration;
    }

    void set_oil_valve_closed_cal_position(uint16_t oil_valve_closed_cal_position) {
        oil_valve_closed_cal_position_ = oil_valve_closed_cal_position;
    }

    void set_oil_valve_closed_position(uint16_t oil_valve_closed_position) {
        oil_valve_closed_position_ = oil_valve_closed_position;
    }

    void set_oil_valve_open_cal_position(uint16_t oil_valve_open_cal_position) {
        oil_valve_open_cal_position_ = oil_valve_open_cal_position;
    }

    void set_oil_valve_open_position(uint16_t oil_valve_open_position) {
        oil_valve_open_position_ = oil_valve_open_position;
    }

    void set_vac_at_end_of_bit(uint16_t vac_at_end_of_bit) {
        vac_at_end_of_bit_ = vac_at_end_of_bit;
    }

    void set_vac_at_start_of_bit(uint16_t vac_at_start_of_bit) {
        vac_at_start_of_bit_ = vac_at_start_of_bit;
    }

    void set_pres_counts_after_offset_reset(uint16_t pres_counts_after_offset_reset) {
        pres_counts_after_offset_reset_ = pres_counts_after_offset_reset;
    }

    void set_pres_counts_before_offset_reset(uint16_t pres_counts_before_offset_reset) {
        pres_counts_before_offset_reset_ = pres_counts_before_offset_reset;
    }

    void set_pres_offset(uint16_t pres_offset) {
        pres_offset_ = pres_offset;
    }

public:

    /* Member Functions */

    /** The parameterized constructor.
     *
     *  @param[in]      data        The binary data to decode.
     *
     *  @throw          log::MessageException if an error occurs while decoding.
     */
    XAlamoBit(const vector<uint8_t>& data) :
                    bit_version_(0xFF),
                    float_software_version_(0xFFFFFF),
                    bit_status_code_(0xFFFFFF),
                    bit_duration_(0xFFFF),
                    bit_pressure_(0xFFFF),
                    bit_temperature_(0xFFFF),
                    bit_salinity_(0xFFFF),
                    ctd_status_(0xFFFFFFFF),
                    vac_at_start_of_bit_(0xFFFF),
                    vac_at_end_of_bit_(0xFFFF),
                    battery_voltage_at_start_of_bit_(0xFFFF),
                    battery_voltage_at_end_of_bit_(0xFFFF),
                    air_valve_open_position_(0xFFFF),
                    air_valve_open_cal_position_(0xFFFF),
                    air_valve_closed_position_(0xFFFF),
                    air_valve_closed_cal_position_(0xFFFF),
                    air_valve_middle_position_(0xFFFF),
                    air_valve_middle_cal_position_(0xFFFF),
                    air_pump_test_duration_(0xFFFF),
                    air_pump_cycle_count_(0),
                    air_pressure_max_(0xFFFF),
                    air_pump_max_load_current_(0xFFFF),
                    air_pump_max_load_current_12v_(0xFFFF),
                    air_pump_max_load_battery_voltage_(0xFFFF),
                    oil_valve_open_position_(0xFFFF),
                    oil_valve_open_cal_position_(0xFFFF),
                    oil_valve_closed_position_(0xFFFF),
                    oil_valve_closed_cal_position_(0xFFFF),
                    oil_pump_test_duration_(0xFFFF),
                    oil_pump_max_load_current_(0xFFFF),
                    oil_pump_max_load_current_12v_(0xFFFF),
                    oil_pump_max_load_battery_voltage_(0xFFFF) {

        // Decode the message.
        DecodeXMsgData(data);

    }

    /** The destructor. */
    ~XAlamoBit() {
        /* Do nothing. */
    }

    /** Decode the ALAMO BIT test engineering message from the binary message data.
     *
     *  @param[in]      data        The binary data to decode.
     *
     *  @throw          log::MessageException if an error occurs while decoding.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {
        try {
            const uint8_t* d = data.data();

            // Check message size.
            if(data.size() < 13) {
                ostringstream os;
                os << "Invalid X-Message: Expected at least 13 bytes, but only received"
                   << data.size()
                   << " bytes!";
                EXC_THROW(os.str());
            }

            // Check if BIT passed or failed.
            if(d[0] == 0xE5)
                set_bit_passed(true);
            else if(d[0] == 0xE6)
                set_bit_passed(false);
            else {
                ostringstream os;
                os << "Invalid X-Message: Expected X-Message format identifiers 0xE5 or 0xE6, but received 0x"
                   << std::hex
                   << d[0]
                   << "!";
                EXC_THROW(os.str());
            }

            // Get BIT version.
            set_bit_version(u08(d, 3));

            switch(bit_version_) {

            case 6: // ALAMO BIT version 6.

                // Check the message size.
                if(data.size() != 13) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected at 13 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                set_float_software_version(u32(d, 4));
                set_bit_status_code(u32(d, 8));

                break;

            case 7: // ALAMO BIT version 7.

                // Check the message size.
                if(data.size() != 72) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 72 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                set_float_software_version(u32(d, 4));
                set_bit_status_code(u32(d, 8));
                set_bit_duration(u16(d, 12));
                set_bit_pressure(u16(d, 14));
                set_bit_temperature(u16(d, 16));
                set_bit_salinity(u16(d, 18));
                set_ctd_status(u32(d, 20));
                set_vac_at_start_of_bit(u16(d, 24));
                set_vac_at_end_of_bit(u16(d, 26));
                set_battery_voltage_at_start_of_bit(u16(d, 28));
                set_battery_voltage_at_end_of_bit(u16(d, 30));
                set_air_valve_open_position(u16(d, 32));
                set_air_valve_open_cal_position(u16(d, 34));
                set_air_valve_closed_position(u16(d, 36));
                set_air_valve_closed_cal_position(u16(d, 38));
                set_air_valve_middle_position(u16(d, 40));
                set_air_valve_middle_cal_position(u16(d, 42));
                set_air_pump_test_duration(u16(d, 44));
                set_air_pump_cycle_count(u08(d, 46));
                set_air_pressure_max(u16(d, 47));
                set_air_pump_max_load_current(u16(d, 49));
                set_air_pump_max_load_current_12v(u16(d, 51));
                set_air_pump_max_load_battery_voltage(u16(d, 53));
                set_oil_valve_open_position(u16(d, 55));
                set_oil_valve_open_cal_position(u16(d, 57));
                set_oil_valve_closed_position(u16(d, 59));
                set_oil_valve_closed_cal_position(u16(d, 61));
                set_oil_pump_test_duration(u16(d, 63));
                set_oil_pump_max_load_current(u16(d, 65));
                set_oil_pump_max_load_current_12v(u16(d, 67));
                set_oil_pump_max_load_battery_voltage(u16(d, 69));

                break;

            case 8: // ALAMO BIT version 8.

              // Check the message size.
              if(data.size() != 76) {
                  ostringstream os;
                  os << "Invalid X-Message: Expected 76 bytes, but received "
                     << data.size()
                     << " bytes!";
                  EXC_THROW(os.str());
              }

              set_float_software_version(u32(d, 4));
              set_bit_status_code(u32(d, 8));
              set_bit_duration(u16(d, 12));
              set_pres_counts_before_offset_reset(u16(d, 14));
              set_pres_offset(u16(d, 16));
              set_pres_counts_after_offset_reset(u16(d, 18));
              set_bit_temperature(u16(d, 20));
              set_bit_salinity(u16(d, 22));
              set_ctd_status(u32(d, 24));
              set_vac_at_start_of_bit(u16(d, 28));
              set_vac_at_end_of_bit(u16(d, 30));
              set_battery_voltage_at_start_of_bit(u16(d, 32));
              set_battery_voltage_at_end_of_bit(u16(d, 34));
              set_air_valve_open_position(u16(d, 36));
              set_air_valve_open_cal_position(u16(d, 38));
              set_air_valve_closed_position(u16(d, 40));
              set_air_valve_closed_cal_position(u16(d, 42));
              set_air_valve_middle_position(u16(d, 44));
              set_air_valve_middle_cal_position(u16(d, 46));
              set_air_pump_test_duration(u16(d, 48));
              set_air_pump_cycle_count(u08(d, 50));
              set_air_pressure_max(u16(d, 51));
              set_air_pump_max_load_current(u16(d, 53));
              set_air_pump_max_load_current_12v(u16(d, 55));
              set_air_pump_max_load_battery_voltage(u16(d, 57));
              set_oil_valve_open_position(u16(d, 59));
              set_oil_valve_open_cal_position(u16(d, 61));
              set_oil_valve_closed_position(u16(d, 63));
              set_oil_valve_closed_cal_position(u16(d, 65));
              set_oil_pump_test_duration(u16(d, 67));
              set_oil_pump_max_load_current(u16(d, 69));
              set_oil_pump_max_load_current_12v(u16(d, 71));
              set_oil_pump_max_load_battery_voltage(u16(d, 73));

              break;

            default: { // Unsupported ALAMO BIT version.

                  ostringstream os;
                  os << "Unsupported ALAMO BIT version: " << (int) bit_version_ << "!";
                  EXC_THROW(os.str());
              }

              break;
            }
        }CATCH_BLOCK("Failed to decode ALAMO Engineering (BIT) data message!")
    }

    /** Generate a listing entry string representation of the the ALAMO BIT test engineering
     *  message.
     *
     *  @param[in]      whoi        The instrument serial number.
     *  @param[in]      dive        The dive number of this ALAMO BIT test engineering message.
     *  @param[in]      pg          The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]      po          The pressure sensor offset, in dBar.
     *  @param[in]      sg          The salinity sensor sensitivity scale factor, in LSB/PSU.
     *  @param[in]      so          The salinity sensor offset, in PSU.
     *  @param[in]      tg          The temperature sensor sensitivity scale factor, in LSB/degC.
     *  @param[in]      to          The temperatUre sensor offset, in degC.
     *
     *  @return         A listing entry string representation of the the ALAMO BIT test engineering
     *                  message.
     */
    const string GenerateListingString(
            int whoi,
            int dive,
            uint16_t pg,
            uint16_t po,
            uint16_t sg,
            uint16_t so,
            uint16_t tg,
            uint16_t to) const {
        ostringstream os;

        static const format fmt1("#   %5s %4s %4s "
                "%15s "
                "%15s "
                "%15s");
        static const format fmt2("E0%c %5d %4d %4d "
                "%15d "
                "     0x%08X "
                "     0x%08X");

        switch(bit_version_) {

        case 6: { // ALAMO BIT version 6.

            // Minor index 0.
            os << format(fmt1) % "SN" % "dive" % "midx" % "BITVer" % "SWVer" % "BITResults" << endl;
            os << format(fmt2) % (is_bit_passed() ? '5' : '6') % whoi % dive % 0
                    % (int) bit_version() % float_software_version() % bit_status_code();

        }

            break;

        case 7: { // ALAMO BIT version 7.

            // Minor index 0.
            os << format(fmt1) % "SN" % "dive" % "midx" % "BITVer" % "SWVer" % "BITResults" << endl;
            os << format(fmt2) % (is_bit_passed() ? '5' : '6') % whoi % dive % 0
                       % (int) bit_version() % float_software_version() % bit_status_code()
               << endl;

            // Minor index 1.
            static const format fmt3("#   %5s %4s %4s %7s   %8s %8s   %7s %8s   %7s %8s");
            static const format fmt4("#   %5s %4s %4s %7s   %8s %9s   %7s %8s   %7s %8s");
            static const format fmt5("E0%c %5d %4d %4d %7d   %8.3f %8d   %7.3f %8d   %7.3f %8d");
            os << "# --------------------------------------------------------------------------------------------------"
               << endl;
            os << format(fmt3) % "SN" % "dive" % "midx" % "BITTime" % "pres" % "pres" % "temp"
                       % "temp" % "saln" % "saln"
               << endl;
            os << format(fmt4) % "" % "" % "" % "(sec)" % "(dBar)" % "(counts)" % "(degC)"
                       % "(counts)" % "(PSU)" % "(counts)"
               << endl;
            os << format(fmt5) % (is_bit_passed() ? '5' : '6') % whoi % dive % 1 % bit_duration()
                       % GetDecodedBitPressure(pg, po) % bit_pressure()
                       % GetDecodedBitTemperature(tg, to) % bit_temperature()
                       % GetDecodedBitSalinity(sg, so) % bit_salinity()
               << endl;

            // Minor index 2.
            static const format fmt6("#   %5s %4s %4s %10s %8s %8s %8s %8s");
            static const format fmt7("#   %5s %4s %4s %10s %8s %8s %8s %8s");
            static const format fmt8("E0%c %5d %4d %4d 0x%08X %8.2f %8.2f %8.2f %8.2f");
            os << "# --------------------------------------------------------------------------------------------------"
               << endl;
            os << format(fmt6) % "SN" % "dive" % "midx" % "CTDStat" % "VacStart" % "VacEnd"
                       % "BatStart" % "BatEnd"
               << endl;
            os << format(fmt7) % "" % "" % "" % "" % "(inHg)" % "(inHg)" % "(V)" % "(V)" << endl;
            os << format(fmt8) % (is_bit_passed() ? '5' : '6') % whoi % dive
                       % 2 //
                       % ctd_status() % GetDecodedVacAtStartOfBit() % GetDecodedVacAtEndOfBit()
                       % GetDecodedBatteryVoltageAtStartOfBit()
                       % GetDecodedBatteryVoltageAtEndOfBit()
               << endl;

            // Minor index 3.
            static const format fmt9("#   %5s %4s %4s %13s %13s %11s %11s %14s %13s");
            static const format fmt10("#   %5s %4s %4s %13s %13s %11s %11s %14s %13s");
            static const format fmt11("E0%c %5d %4d %4d %13.3f %13.3f %11.3f %11.3f %14.3f %13.3f");
            os << "# --- Air Valve (AV) Tests -------------------------------------------------------------------------"
               << endl;
            os << format(fmt9) % "SN" % "dive" % "midx" % "AVOpenPos" % "AVOpenPosCal" % "AVMidPos"
                       % "AVMidPosCal" % "AVClosePos" % "AVClosePosCal"
               << endl;
            os << format(fmt10) % "" % "" % "" % "(V)" % "(V)" % "(V)" % "(V)" % "(V)" % "(V)"
               << endl;
            os << format(fmt11) % (is_bit_passed() ? '5' : '6') % whoi % dive % 3
                       % ((float) air_valve_open_position_ / 1000)
                       % ((float) air_valve_open_cal_position_ / 1000)
                       % ((float) air_valve_middle_position_ / 1000)
                       % ((float) air_valve_middle_cal_position_ / 1000)
                       % ((float) air_valve_closed_position_ / 1000)
                       % ((float) air_valve_closed_cal_position_ / 1000)
               << endl;

            // Minor index 4.
            static const format fmt12("#   %5s %4s %4s %13s %13s %11s %11s %14s %13s");
            static const format fmt13("#   %5s %4s %4s %13s %13s %11s %11s %14s %13s");
            static const format fmt14("E0%c %5d %4d %4d %13d %13d %11.3f %11.3f %14.3f %13.3f");
            os << "# --- Air Pump (AP) Tests --------------------------------------------------------------------------"
               << endl;
            os << format(fmt12) % "SN" % "dive" % "midx" % "APTestTime" % "APCycleCount"
                       % "APMxAPres" % "APMxLoadCur" % "APMxLoadCur12V" % "APMxLoadBatV"
               << endl;
            os << format(fmt13) % "" % "" % "" % "(sec)" % "" % "(PSI)" % "(A)" % "(A)" % "(V)"
               << endl;
            os << format(fmt14) % (is_bit_passed() ? '5' : '6') % whoi % dive % 4
                       % air_pump_test_duration() % (int) air_pump_cycle_count()
                       % ((float) air_pressure_max() / 1000)
                       % ((float) air_pump_max_load_current() / 1000)
                       % ((float) air_pump_max_load_current_12v() / 1000)
                       % ((float) air_pump_max_load_battery_voltage() / 1000)
               << endl;

            // Minor index 5.
            static const format fmt15("#   %5s %4s %4s %13s %13s %14s %14s");
            static const format fmt16("#   %5s %4s %4s %13s %13s %14s %14s");
            static const format fmt17("E0%c %5d %4d %4d %13.3f %13.3f %14.3f %14.3f");
            os << "# --- Oil Valve (OV) Tests -------------------------------------------------------------------------"
               << endl;
            os << format(fmt15) % "SN" % "dive" % "midx" % "OVOpenPos" % "OVOpenPosCal"
                       % "OVClosePos" % "OVClosePosCal"
               << endl;
            os << format(fmt16) % "" % "" % "" % "(V)" % "(V)" % "(V)" % "(V)" << endl;
            os << format(fmt17) % (is_bit_passed() ? '5' : '6') % whoi % dive % 5
                       % ((float) oil_valve_open_position() / 1000)
                       % ((float) oil_valve_open_cal_position() / 1000)
                       % ((float) oil_valve_closed_position() / 1000)
                       % ((float) oil_valve_closed_cal_position() / 1000)
               << endl;

            // Minor index 6.
            static const format fmt18("#   %5s %4s %4s %13s %13s %14s %14s");
            static const format fmt19("#   %5s %4s %4s %13s %13s %14s %14s");
            static const format fmt20("E0%c %5d %4d %4d %13d %13.3f %14.3f %14.3f");
            os << "# --- Oil Pump (OP) Tests --------------------------------------------------------------------------"
               << endl;
            os << format(fmt18) % "SN" % "dive" % "midx" % "OPTestTime" % "OPMxLoadCur"
                       % "OPMxLoadCur12V" % "OPMxLoadBatV"
               << endl;
            os << format(fmt19) % "" % "" % "" % "(sec)" % "(A)" % "(A)" % "(V)" << endl;
            os << format(fmt20) % (is_bit_passed() ? '5' : '6') % whoi % dive % 6
                    % oil_pump_test_duration() % ((float) oil_pump_max_load_current() / 1000)
                    % ((float) oil_pump_max_load_current_12v() / 1000)
                    % ((float) oil_pump_max_load_battery_voltage() / 1000);

        }

            break;

        case 8: { // ALAMO BIT version 8.

            // Minor index 0.
            os << format(fmt1) % "SN" % "dive" % "midx" % "BITVer" % "SWVer" % "BITResults" << endl;
            os << format(fmt2) % (is_bit_passed() ? '5' : '6') % whoi % dive % 0
                       % (int) bit_version() % float_software_version() % bit_status_code()
               << endl;

            // Minor index 1.
            static const format fmt21("#   %5s %4s %4s " //
                    "%15s"
                    "%15s "
                    );
            static const format fmt22("#   %5s %4s %4s "
                    "%15s"
                    "%15s "
                    );
            static const format fmt23("E0%c %5d %4d %4d "
                    "%15d"
                    "     0x%08X "
                    );
            os << "# --------------------------------------------------------------------------------------------------"
               << endl;
            os << format(fmt21) % "SN" % "dive" % "midx" //
                    % "BITTime" //
                    % "CTDStat" //
               << endl;
            os << format(fmt22) % "" % "" % "" //
                    % "(sec)" //
                    % "" //
               << endl;
            os << format(fmt23) % (is_bit_passed() ? '5' : '6') % whoi % dive
                       % 1 // midx
                       % bit_duration() //
                       % ctd_status() //
               << endl;

            // Minor index 2.
            static const format fmt24("#   %5s %4s %4s " //
                    "%15s %15s "
                    "%15s %15s "
                    "%15s"
                    );
            static const format fmt25("#   %5s %4s %4s "
                    "%15s %15s "
                    "%15s %15s "
                    "%15s"
                    );
            static const format fmt26("E0%c %5d %4d %4d "
                    "%15.3f %15d "
                    "%15.3f %15d "
                    "%15.3d"
                    );
            os << "# --------------------------------------------------------------------------------------------------"
               << endl;
            os << format(fmt24) % "SN" % "dive" % "midx" //
                    % "presPostReset" % "presPostReset" //
                    % "presPreReset" % "presPreReset" //
                    % "presOffset" //
               << endl;
            os << format(fmt25) % "" % "" % "" //
                    % "(dBar)" % "(counts)" //
                    % "(dBar)" % "(counts)" //
                    % "(dBar)"
               << endl;
            os << format(fmt26) % (is_bit_passed() ? '5' : '6') % whoi % dive
                       % 2 // midx
                       % GetDecodedBitPressureAfterOffset(pg, po) % pres_counts_after_offset_reset() //
                       % GetDecodedBitPressureBeforeOffset(pg, po) % pres_counts_before_offset_reset() //
                       % GetDecodedBitPressureOffset() //
               << endl;

            // Minor index 3.
            static const format fmt3("#   %5s %4s %4s " //
                    "%15s %15s "
                    "%15s %15s "
                    );
            static const format fmt4("#   %5s %4s %4s "
                    "%16s %15s "
                    "%15s %15s "
                    );
            static const format fmt5("E0%c %5d %4d %4d "
                    "%15.3f %15d "
                    "%15.3f %15d "
                    );
            os << "# --------------------------------------------------------------------------------------------------"
               << endl;
            os << format(fmt3) % "SN" % "dive" % "midx" //
                    % "temp"  % "temp"  //
                    % "saln" % "saln" //
               << endl;
            os << format(fmt4) % "" % "" % ""  //
                    % "(degC)" % "(counts)"  //
                    % "(PSU)" % "(counts)" //
               << endl;
            os << format(fmt5) % (is_bit_passed() ? '5' : '6') % whoi % dive
                       % 3 // midx
                       % GetDecodedBitTemperature(tg, to) % bit_temperature() //
                       % GetDecodedBitSalinity(sg, so) % bit_salinity() //
               << endl;

            // Minor index 4.
            static const format fmt6("#   %5s %4s %4s "
                    "%15s %15s "
                    "%15s %15s");
            static const format fmt7("#   %5s %4s %4s "
                    "%15s %15s "
                    "%15s %15s");
            static const format fmt8("E0%c %5d %4d %4d "
                    "%15.2f %15.2f "
                    "%15.2f %15.2f");
            os << "# --------------------------------------------------------------------------------------------------"
               << endl;
            os << format(fmt6) % "SN" % "dive" % "midx" //
                    % "VacStart" % "VacEnd" //
                       % "BatStart" % "BatEnd" //
               << endl;
            os << format(fmt7) % "" % "" % ""
                    % "(inHg)" % "(inHg)" //pres
                    % "(V)" % "(V)"  //
                    << endl;
            os << format(fmt8) % (is_bit_passed() ? '5' : '6') % whoi % dive
                       % 4 // midx
                       % GetDecodedVacAtStartOfBit() % GetDecodedVacAtEndOfBit() //
                       % GetDecodedBatteryVoltageAtStartOfBit()
                       % GetDecodedBatteryVoltageAtEndOfBit() //
               << endl;

            // Minor index 5.
            static const format fmt9("#   %5s %4s %4s %13s %13s %11s %11s %14s %13s");
            static const format fmt10("#   %5s %4s %4s %13s %13s %11s %11s %14s %13s");
            static const format fmt11("E0%c %5d %4d %4d %13.3f %13.3f %11.3f %11.3f %14.3f %13.3f");
            os << "# --- Air Valve (AV) Tests -------------------------------------------------------------------------"
               << endl;
            os << format(fmt9) % "SN" % "dive" % "midx" % "AVOpenPos" % "AVOpenPosCal" % "AVMidPos"
                       % "AVMidPosCal" % "AVClosePos" % "AVClosePosCal"
               << endl;
            os << format(fmt10) % "" % "" % "" % "(V)" % "(V)" % "(V)" % "(V)" % "(V)" % "(V)"
               << endl;
            os << format(fmt11) % (is_bit_passed() ? '5' : '6') % whoi % dive
                       % 5 // midx
                       % ((float) air_valve_open_position_ / 1000)
                       % ((float) air_valve_open_cal_position_ / 1000)
                       % ((float) air_valve_middle_position_ / 1000)
                       % ((float) air_valve_middle_cal_position_ / 1000)
                       % ((float) air_valve_closed_position_ / 1000)
                       % ((float) air_valve_closed_cal_position_ / 1000)
               << endl;

            // Minor index 6.
            static const format fmt12("#   %5s %4s %4s %13s %13s %11s %11s %14s %13s");
            static const format fmt13("#   %5s %4s %4s %13s %13s %11s %11s %14s %13s");
            static const format fmt14("E0%c %5d %4d %4d %13d %13d %11.3f %11.3f %14.3f %13.3f");
            os << "# --- Air Pump (AP) Tests --------------------------------------------------------------------------"
               << endl;
            os << format(fmt12) % "SN" % "dive" % "midx" % "APTestTime" % "APCycleCount"
                       % "APMxAPres" % "APMxLoadCur" % "APMxLoadCur12V" % "APMxLoadBatV"
               << endl;
            os << format(fmt13) % "" % "" % "" % "(sec)" % "" % "(PSI)" % "(A)" % "(A)" % "(V)"
               << endl;
            os << format(fmt14) % (is_bit_passed() ? '5' : '6') % whoi % dive
                       % 6 // midx
                       % air_pump_test_duration() % (int) air_pump_cycle_count()
                       % ((float) air_pressure_max() / 1000)
                       % ((float) air_pump_max_load_current() / 1000)
                       % ((float) air_pump_max_load_current_12v() / 1000)
                       % ((float) air_pump_max_load_battery_voltage() / 1000)
               << endl;

            // Minor index 7.
            static const format fmt15("#   %5s %4s %4s %15s %15s %15s %15s");
            static const format fmt16("#   %5s %4s %4s %15s %15s %15s %15s");
            static const format fmt17("E0%c %5d %4d %4d %15.3f %15.3f %15.3f %15.3f");
            os << "# --- Oil Valve (OV) Tests -------------------------------------------------------------------------"
               << endl;
            os << format(fmt15) % "SN" % "dive" % "midx" % "OVOpenPos" % "OVOpenPosCal"
                       % "OVClosePos" % "OVClosePosCal"
               << endl;
            os << format(fmt16) % "" % "" % "" % "(V)" % "(V)" % "(V)" % "(V)" << endl;
            os << format(fmt17) % (is_bit_passed() ? '5' : '6') % whoi % dive
                       % 7 // midx
                       % ((float) oil_valve_open_position() / 1000)
                       % ((float) oil_valve_open_cal_position() / 1000)
                       % ((float) oil_valve_closed_position() / 1000)
                       % ((float) oil_valve_closed_cal_position() / 1000)
               << endl;

            // Minor index 8.
            static const format fmt18("#   %5s %4s %4s %15s %15s %15s %15s");
            static const format fmt19("#   %5s %4s %4s %15s %15s %15s %15s");
            static const format fmt20("E0%c %5d %4d %4d %15d %15.3f %15.3f %15.3f");
            os << "# --- Oil Pump (OP) Tests --------------------------------------------------------------------------"
               << endl;
            os << format(fmt18) % "SN" % "dive" % "midx" % "OPTestTime" % "OPMxLoadCur"
                       % "OPMxLoadCur12V" % "OPMxLoadBatV"
               << endl;
            os << format(fmt19) % "" % "" % "" % "(sec)" % "(A)" % "(A)" % "(V)" << endl;
            os << format(fmt20) % (is_bit_passed() ? '5' : '6') % whoi % dive
                    % 8 // midx
                    % oil_pump_test_duration() % ((float) oil_pump_max_load_current() / 1000)
                    % ((float) oil_pump_max_load_current_12v() / 1000)
                    % ((float) oil_pump_max_load_battery_voltage() / 1000);

        }

            break;

        default: { // Unsupported Engineering (BIT) version.
            ostringstream os;
            os << "Unsupported ALAMO Engineering (BIT) message version: 0x"
               << hex
               << (int) bit_version()
               << "!";
            EXC_THROW(os.str());
        }
            break;
        }

        return os.str();
    }

    /** Get the Built-in-Test (BIT) status code.
     *
     *  @return         The Built-in-Test (BIT) status code.
     */
    uint32_t bit_status_code() const {
        return bit_status_code_;
    }

    /** Get the Built-in-Test (BIT) version.
     *
     *  @return         The Built-in-Test (BIT) version.
     */
    uint8_t bit_version() const {
        return bit_version_;
    }

    /** Get the instrument software version.
     *
     *  @return         The instrument software version.
     */
    uint32_t float_software_version() const {
        return float_software_version_;
    }

    uint16_t air_pressure_max() const {
        return air_pressure_max_;
    }

    uint8_t air_pump_cycle_count() const {
        return air_pump_cycle_count_;
    }

    uint16_t air_pump_max_load_battery_voltage() const {
        return air_pump_max_load_battery_voltage_;
    }

    uint16_t air_pump_max_load_current() const {
        return air_pump_max_load_current_;
    }

    uint16_t air_pump_max_load_current_12v() const {
        return air_pump_max_load_current_12v_;
    }

    uint16_t air_pump_test_duration() const {
        return air_pump_test_duration_;
    }

    uint16_t air_valve_closed_cal_position() const {
        return air_valve_closed_cal_position_;
    }

    uint16_t air_valve_closed_position() const {
        return air_valve_closed_position_;
    }

    uint16_t air_valve_middle_cal_position() const {
        return air_valve_middle_cal_position_;
    }

    uint16_t air_valve_middle_position() const {
        return air_valve_middle_position_;
    }

    uint16_t air_valve_open_cal_position() const {
        return air_valve_open_cal_position_;
    }

    uint16_t air_valve_open_position() const {
        return air_valve_open_position_;
    }

    uint16_t battery_voltage_at_end_of_bit() const {
        return battery_voltage_at_end_of_bit_;
    }

    uint16_t battery_voltage_at_start_of_bit() const {
        return battery_voltage_at_start_of_bit_;
    }

    uint16_t bit_duration() const {
        return bit_duration_;
    }

    uint16_t bit_pressure() const {
        return bit_pressure_;
    }

    uint16_t bit_salinity() const {
        return bit_salinity_;
    }

    uint16_t bit_temperature() const {
        return bit_temperature_;
    }

    uint32_t ctd_status() const {
        return ctd_status_;
    }

    template<typename T = double> T GetDecodedBitPressure(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) bit_pressure_ / (T) gain - (T) offs : XMSG_SENS_BAD_PRES;

        if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
            return XMSG_SENS_BAD_PRES;

        return val;
    }

    template<typename T = double> T GetDecodedBitPressureAfterOffset(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) pres_counts_after_offset_reset() / (T) gain - (T) offs : XMSG_SENS_BAD_PRES;

        if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
            return XMSG_SENS_BAD_PRES;

        return val;
    }

    template<typename T = double> T GetDecodedBitPressureBeforeOffset(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) pres_counts_before_offset_reset() / (T) gain - (T) offs : XMSG_SENS_BAD_PRES;

        if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
            return XMSG_SENS_BAD_PRES;

        return val;
    }

    template<typename T = double> T GetDecodedBitPressureOffset() const {
        T val;

        val =  (T) pres_offset() / (T) 1000;

        if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
            return XMSG_SENS_BAD_PRES;

        return val;
    }

    template<typename T = double> T GetDecodedBitSalinity(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) bit_salinity_ / (T) gain - (T) offs : XMSG_SENS_BAD_SALN;

        if(val < XMSG_SENS_MIN_SALN || val > XMSG_SENS_MAX_SALN)
            return XMSG_SENS_BAD_SALN;

        return val;
    }

    template<typename T = double> T GetDecodedBitTemperature(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) bit_temperature_ / (T) gain - (T) offs : XMSG_SENS_BAD_TEMP;

        if(val < XMSG_SENS_MIN_TEMP || val > XMSG_SENS_MAX_TEMP)
            return XMSG_SENS_BAD_TEMP;

        return val;
    }

    template<typename T = double> T GetDecodedVacAtEndOfBit() const {
        return (T) vac_at_end_of_bit() / (T) 100;
    }
    template<typename T = double> T GetDecodedVacAtStartOfBit() const {
        return (T) vac_at_start_of_bit() / (T) 100;
    }

    template<typename T = double> T GetDecodedBatteryVoltageAtEndOfBit() const {
        return (T) battery_voltage_at_end_of_bit() / (T) 100;
    }

    template<typename T = double> T GetDecodedBatteryVoltageAtStartOfBit() const {
        return (T) battery_voltage_at_start_of_bit() / (T) 100;
    }

    bool is_bit_passed() const {
        return bit_passed_;
    }

    uint16_t oil_pump_max_load_current() const {
        return oil_pump_max_load_current_;
    }

    uint16_t oil_pump_max_load_current_12v() const {
        return oil_pump_max_load_current_12v_;
    }

    uint16_t oil_pump_max_load_battery_voltage() const {
        return oil_pump_max_load_battery_voltage_;
    }

    uint16_t oil_pump_test_duration() const {
        return oil_pump_test_duration_;
    }

    uint16_t oil_valve_closed_cal_position() const {
        return oil_valve_closed_cal_position_;
    }

    uint16_t oil_valve_closed_position() const {
        return oil_valve_closed_position_;
    }

    uint16_t oil_valve_open_cal_position() const {
        return oil_valve_open_cal_position_;
    }

    uint16_t oil_valve_open_position() const {
        return oil_valve_open_position_;
    }

    uint16_t vac_at_end_of_bit() const {
        return vac_at_end_of_bit_;
    }

    uint16_t vac_at_start_of_bit() const {
        return vac_at_start_of_bit_;
    }

    uint16_t pres_counts_after_offset_reset() const {
        return pres_counts_after_offset_reset_;
    }

    uint16_t pres_counts_before_offset_reset() const {
        return pres_counts_before_offset_reset_;
    }

    uint16_t pres_offset() const {
        return pres_offset_;
    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XALAMOBIT_H_ */
