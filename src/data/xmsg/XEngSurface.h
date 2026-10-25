/** The @ref util::xmsg::XEngSurface class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 2:03:16 PM
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

#ifndef XENGSURFACE_H_
#define XENGSURFACE_H_

/* Includes */

#include "XAlamoEngineering.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Class representing an surface state engineering X-Message.
 *
 *  @note           This X-Message is only used by ALAMO-type instruments.
 *  @note           This message type was added in X-Message version 2.1.
 */
class XEngSurface:
        public XAlamoEngineering {

    /** The engineering surface data error code class.
     *
     *  An instance of this object represents a single error code reported in the ALAMO engineering
     *  (surface) data message. Each error consists of a timestamp and a
     *
     */
    class XEngErrorCode:
            public util::date::DateUtils {

    private:

        /** The timestamp for the error code.
         *
         */
        ptime date_time_;

        /** The error code.
         *
         */
        uint32_t error_code_;

    protected:

        /** Set the timestamp for the error code.
         *
         *  @param[in]  date_time   The timestamp for the error code.
         */
        void set_date_time(const ptime& date_time) {
            date_time_ = date_time;
        }

        /** Set the error code.
         *
         *  @param[in]  error_code  The error code.
         */
        void set_error_code(uint32_t error_code) {
            error_code_ = error_code;
        }

    public:

        /** The parameterized constructor.
         *
         *  @param[in]  date_time   The timestamp of the error code.
         *  @param[in]  error_code  The error code.
         */
        XEngErrorCode(ptime date_time, uint32_t error_code) :
                        date_time_(date_time),
                        error_code_(error_code) {
            /* Do nothing. */
        }

        /** The destructor.
         *
         */
        ~XEngErrorCode() {
            /* Do nothing. */
        }

        /** Get the timestamp for the error code.
         *
         *  @return     The timestamp for the error code.
         */
        const ptime& date_time() const {
            return date_time_;
        }

        /** Generate an ISO8601 format date/time string representation of the error date/time.
         *
         *  @note           All date/time objects use the UTC time-zone.
         *
         *  @return         An ISO8601 format date/time string representation of the error
         *                  date/time. If @ref date_time() returns
         *                  boost::posix_time::not_a_date_time, then the string
         *                  "0000-00-00T00:00:00Z" will be returned.
         */
        string DateTimeAs8601String() const {
            return GenerateIso8601String(date_time());
        }

        /** Get the error code.
         *
         *  @return     The error code.
         */
        uint32_t error_code() const {
            return error_code_;
        }




    };

private:

    /* Private Data Members: */

    /** The maximum measured air bladder pressure during the air pump, in .001 PSI.
     *
     *  @note This field was added in version 0x11.
     *
     */
    uint16_t air_bladder_max_pres_mpsi_;

    /** The measured air pump current, in mA.
     *
     *  @note This field was added in version 0x11.
     */
    uint16_t air_pump_current_ma_;

    /** The measured air pump run time, in seconds
     *
     *  @note This field was added in version 0x11.
     */
    uint16_t air_pump_runtime_s_;

    /** The air valve closed calibration position, in mV.
     *
     *  @note This field was added in version 0x11.
     */
    uint16_t air_valve_closed_cal_position_mv_;

    /** The air valve mid calibration position, in mV.
     *
     *  @note This field was added in version 0x11.
     */
    uint16_t air_valve_mid_cal_position_mv_;

    /** The air valve mid position at the end of the surface phase of the dive cycle, in mV.
     *
     *  @note This field was added in version 0x11.
     */
    uint16_t air_valve_mid_position_at_end_mv_;

    /** The air valve open calibration position, in mV.
     *
     *  @note This field was added in version 0x11.
     */
    uint16_t air_valve_open_cal_position_mv_;

    /** The air valve open position at the start of the surface phase of the dive cycle, in mV.
     *
     *  @note This field was added in version 0x11.
     */
    uint16_t air_valve_open_position_at_start_mv_;

    /** The measured system battery voltage, in mV.
     *
     *  @note This field is only used for message version 0x10. It was removed in version 0x11.
     *
     *
     */
    uint16_t battery_voltage_mv_;

    /** The measured internal pressure case vacuum, in 0.01 inHg, at the end of the
     *  surface phase of the dive cycle.
     *
     *  @note This field was added in version 0x11.
     *
     */
    uint16_t case_vac_at_end_cinhg_;

    /** The measured internal pressure case vacuum, in 0.01 inHg, at the start of the
     *  surface phase of the dive cycle.
     *
     *  @note This field was added in version 0x11.
     *
     */
    uint16_t case_vac_at_start_cinhg_;

    /** The measured internal pressure case vacuum, in 0.01 inHg.
     *
     *  @note This field is only used for message version 0x10. It was removed in version 0x11.
     *
     *
     */
    uint16_t case_vac_cingh_;

    /** The CTD status code.
     *
     *  @todo           Define status code values for the ALAMO surface engineering data message.
     */
    uint32_t ctd_status_code_;

    /** The error code values for this dive, if any.
     *
     *
     */
    vector<shared_ptr<XEngErrorCode> > error_codes_;

    /** The number of error codes reported for this dive cycle.
     *
     */
    uint8_t num_errors_;

    /** The number of packets to be transmitted for the current dive.
     *
     *
     */
    uint16_t num_sbd_packets_for_dive_;

    /** The number of attempts to transmit in the previous Iridium session.
     *
     *
     */
    uint16_t num_transmit_attempts_for_previous_dive_;

    /** The oil valve closed calibration position, in mV.
     *
     */
    uint16_t oil_valve_closed_cal_position_mv_;

    /** The oil valve closed position at the end of the surface phase of the dive cycle, in mV.
     *
     */
    uint16_t oil_valve_closed_position_at_end_mv_;

    /** The oil valve open calibration position at the end of the surface phase of the dive cycle,
     *  in mV.
     *
     */
    uint16_t oil_valve_open_cal_position_mv_;

    /** The oil valve open position during air pump, in mV.
     *
     */
    uint16_t oil_valve_open_position_during_air_assist_mv_;

    /** The measured pressure sensor offset at the surface, in 1/1000 dBar.
     *
     */
    int16_t pressure_offset_reading_;

    /** The measured system current, in mA.
     *
     *  @note This field is only used for message version 0x10. It was removed in version 0x11.
     *
     *
     */
    uint16_t system_current_ma_;

    /* Suface data sampling: */

    ptime surface_sampling_start_time_;

    ptime surface_sampling_end_time_;

    /* Private Data Members - Fields added in version 0x12: */

    int16_t gps_rtc_time_delta_;

    /* Private Data Members - Fields added in version 0x13: */

    uint16_t prof_raw_sample_first_pres_;

    uint16_t prof_raw_sample_first_temp_;

    uint16_t prof_raw_sample_first_saln_;

    uint16_t prof_raw_sample_last_pres_;

    uint16_t prof_raw_sample_last_temp_;

    uint16_t prof_raw_sample_last_saln_;

    uint16_t prof_num_bins_;

    uint16_t prof_num_bins_empty_;

    uint16_t prof_num_raw_samples_;


public:

    /* Public Member Functions */

    /** The parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wreorder"
    XEngSurface(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XAlamoEngineering(dive, whoi, data),
                    air_bladder_max_pres_mpsi_(XMSG_BAD_U16),
                    air_pump_current_ma_(XMSG_BAD_U16),
                    air_pump_runtime_s_(XMSG_BAD_U16),
                    air_valve_closed_cal_position_mv_(XMSG_BAD_U16),
                    air_valve_mid_cal_position_mv_(XMSG_BAD_U16),
                    air_valve_mid_position_at_end_mv_(XMSG_BAD_U16),
                    air_valve_open_cal_position_mv_(XMSG_BAD_U16),
                    air_valve_open_position_at_start_mv_(XMSG_BAD_U16),
                    battery_voltage_mv_(XMSG_BAD_U16),
                    case_vac_at_end_cinhg_(XMSG_BAD_U16),
                    case_vac_at_start_cinhg_(XMSG_BAD_U16),
                    case_vac_cingh_(XMSG_BAD_U16),
                    ctd_status_code_(XMSG_BAD_U32),
                    error_codes_(),
                    num_errors_(XMSG_BAD_U08),
                    num_sbd_packets_for_dive_(XMSG_BAD_U16),
                    num_transmit_attempts_for_previous_dive_(XMSG_BAD_U16),
                    oil_valve_closed_cal_position_mv_(XMSG_BAD_U16),
                    oil_valve_closed_position_at_end_mv_(XMSG_BAD_U16),
                    oil_valve_open_cal_position_mv_(XMSG_BAD_U16),
                    oil_valve_open_position_during_air_assist_mv_(XMSG_BAD_U16),
                    pressure_offset_reading_(XMSG_BAD_I16),
                    surface_sampling_end_time_(not_a_date_time),
                    surface_sampling_start_time_(not_a_date_time),
                    system_current_ma_(XMSG_BAD_U16),
                    gps_rtc_time_delta_(XMSG_BAD_I16),
                    prof_raw_sample_first_pres_(XMSG_BAD_U16),
                    prof_raw_sample_first_temp_(XMSG_BAD_U16),
                    prof_raw_sample_first_saln_(XMSG_BAD_U16),
                    prof_raw_sample_last_pres_(XMSG_BAD_U16),
                    prof_raw_sample_last_temp_(XMSG_BAD_U16),
                    prof_raw_sample_last_saln_(XMSG_BAD_U16),
                    prof_num_bins_(XMSG_BAD_U16),
                    prof_num_bins_empty_(XMSG_BAD_U16),
                    prof_num_raw_samples_(XMSG_BAD_U16) {

        // Decode message.
        DecodeXMsgData(data);
    }
#   pragma GCC diagnostic pop

    /** The destructor.
     *
     */
    ~XEngSurface() {

        /* Do nothing. */

    }

    /** Decode the surface state engineering X-Message from the binary data.
     *
     *  @param[in]      data            The binary message to decode.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {

            // Check message.
            const uint8_t* d = CheckXMsgData(data, XMSG_TYPE_ID_ENG_SURFACE, 46);

            // Decode message.
            set_engineering_version(d[3]);
            switch(engineering_version()) {

            case 0x10: { // Engineering (Surface) message version 0x10.

              static const unsigned int MIN_SIZE = 48;

              if(data.size() < MIN_SIZE) {
                  ostringstream os;
                  os << "Invalid X-Message: Expected at least " << MIN_SIZE << " bytes, but "
                     <<"received " << data.size() << " bytes!";
#                 if defined(XMSG_DEBUG_ENG_SURF) && XMSG_DEBUG_ENG_SURF
                    lwrn() << os.str();
#                 else
                    EXC_THROW(os.str());
#                 endif
              }

              set_start_time(s2000(d, 4));
              set_ctd_status_code(u32(d, 8));
              set_num_sbd_packets_for_dive(u16(d, 12));
              set_num_transmit_attempts_for_previous_dive(u16(d, 14));
              set_case_vac_cingh(u16(d, 16));
              set_battery_voltage_mv(u16(d, 18));
              set_system_current_ma(u16(d, 20));
              set_num_errors(u08(d, 46));

              // Parse the error codes.
              for(size_t i = 0; 4 * i + 51 < xmsg_size_bytes(); i++)
                  error_codes_.push_back(
                          make_shared<XEngErrorCode>(not_a_date_time, u32(d, (4 * i) + 47)));

            }
              break;

            case 0x11: { // Engineering (Surface) message version 0x11.

              static const unsigned int MIN_SIZE = 46;

              if(data.size() < MIN_SIZE) {
                  ostringstream os;
                  os << "Invalid X-Message: Expected at least " << MIN_SIZE << " bytes, but "
                     <<"received " << data.size() << " bytes!";
#                 if defined(XMSG_DEBUG_ENG_SURF) && XMSG_DEBUG_ENG_SURF
                    lwrn() << os.str();
#                 else
                    EXC_THROW(os.str());
#                 endif
              }

              set_start_time(s2000(d, 4));
              set_case_vac_at_start_cinhg(u16(d, 8));
              set_case_vac_at_end_cinhg(u16(d, 10));
              set_ctd_status_code(u32(d, 12));
              set_num_sbd_packets_for_dive(u16(d, 16));
              set_num_transmit_attempts_for_previous_dive(u16(d, 18));
              set_air_pump_current_ma(u16(d, 20));
              set_air_pump_runtime_s(u16(d, 22));
              set_air_bladder_max_pres_mpsi(u16(d, 24));
              set_air_valve_open_position_at_start_mv(u16(d, 26));
              set_air_valve_mid_position_at_end_mv(u16(d, 28));
              set_oil_valve_open_position_during_air_assist_mv(u16(d, 30));
              set_oil_valve_closed_position_at_end_mv(u16(d, 32));
              set_air_valve_open_cal_position_mv(u16(d, 34));
              set_air_valve_closed_cal_position_mv(u16(d, 36));
              set_air_valve_mid_cal_position_mv(u16(d, 38));
              set_oil_valve_open_cal_position_mv(u16(d, 40));
              set_oil_valve_closed_cal_position_mv(u16(d, 42));
              set_num_errors(u08(d, 44));

              // Parse the error codes.
              for(size_t i = 0; 6 * i + 50 < xmsg_size_bytes(); i++)
                  error_codes_.push_back(
                          make_shared<XEngErrorCode>(
                                  s2000(d, (6 * i) + 45),
                                  u16(d, (6 * i) + 49)));

            }
              break;

            case 0x12: { // Engineering (Surface) message version 0x12.

              static const unsigned int MIN_SIZE = 50;

              if(data.size() < MIN_SIZE) {
                  ostringstream os;
                  os << "Invalid X-Message: Expected at least " << MIN_SIZE << " bytes, but "
                     <<"received " << data.size() << " bytes!";
#                 if defined(XMSG_DEBUG_ENG_SURF) && XMSG_DEBUG_ENG_SURF
                    lwrn() << os.str();
#                 else
                    EXC_THROW(os.str());
#                 endif
              }

              set_start_time(s2000(d, 4));
              set_case_vac_at_start_cinhg(u16(d, 8));
              set_case_vac_at_end_cinhg(u16(d, 10));
              set_ctd_status_code(u32(d, 12 + 2));
              set_num_sbd_packets_for_dive(u16(d, 16 + 2));
              set_num_transmit_attempts_for_previous_dive(u16(d, 18 + 2));
              set_air_pump_current_ma(u16(d, 20 + 2));
              set_air_pump_runtime_s(u16(d, 22 + 2));
              set_air_bladder_max_pres_mpsi(u16(d, 24 + 2));
              set_air_valve_open_position_at_start_mv(u16(d, 26 + 2));
              set_air_valve_mid_position_at_end_mv(u16(d, 28 + 2));
              set_oil_valve_open_position_during_air_assist_mv(u16(d, 30 + 2));
              set_oil_valve_closed_position_at_end_mv(u16(d, 32 + 2));
              set_air_valve_open_cal_position_mv(u16(d, 34 + 2));
              set_air_valve_closed_cal_position_mv(u16(d, 36 + 2));
              set_air_valve_mid_cal_position_mv(u16(d, 38 + 2));
              set_oil_valve_open_cal_position_mv(u16(d, 40 + 2));
              set_oil_valve_closed_cal_position_mv(u16(d, 42 + 2));
              set_pressure_offset_reading(i16(d, 44 + 2));
              set_num_errors(u08(d, 46 + 2));

              // Parse the error codes.
              for(size_t i = 0; 6 * i + 52 + 2 < xmsg_size_bytes(); i++)
                  error_codes_.push_back(
                          make_shared<XEngErrorCode>(
                                  s2000(d, (6 * i) + 47 + 2),
                                  u16(d, (6 * i) + 51 + 2)));
            }
              break;

            case 0x13: { // Engineering (Surface) message version 0x13.

              static const unsigned int MIN_SIZE = 68;

              if(data.size() < MIN_SIZE) {
                  ostringstream os;
                  os << "Invalid X-Message: Expected at least " << MIN_SIZE << " bytes, but "
                     <<"received " << data.size() << " bytes!";
#                 if defined(XMSG_DEBUG_ENG_SURF) && XMSG_DEBUG_ENG_SURF
                    lwrn() << os.str();
#                 else
                    EXC_THROW(os.str());
#                 endif
              }

              set_start_time(                                     s2000(  d, 4));
              set_num_sbd_packets_for_dive(                       u16(    d, 8));
              set_num_transmit_attempts_for_previous_dive(        u16(    d, 10));
              set_case_vac_at_start_cinhg(                        u16(    d, 12));
              set_case_vac_at_end_cinhg(                          u16(    d, 14));
              set_gps_rtc_time_delta(                             i16(    d, 16));
              set_ctd_status_code(                                u32(    d, 18));
              set_prof_raw_sample_first_pres(                     u16(    d, 22));
              set_prof_raw_sample_first_temp(                     u16(    d, 24));
              set_prof_raw_sample_first_saln(                     u16(    d, 26));
              set_prof_raw_sample_last_pres(                      u16(    d, 28));
              set_prof_raw_sample_last_temp(                      u16(    d, 30));
              set_prof_raw_sample_last_saln(                      u16(    d, 32));
              set_prof_num_bins(                                  u16(    d, 34));
              set_prof_num_raw_samples(                           u16(    d, 36));
              set_prof_num_bins_empty(                            u16(    d, 38));
              set_air_pump_current_ma(                            u16(    d, 40));
              set_air_pump_runtime_s(                             u16(    d, 42));
              set_air_bladder_max_pres_mpsi(                      u16(    d, 44));
              set_air_valve_open_position_at_start_mv(            u16(    d, 46));
              set_air_valve_mid_position_at_end_mv(               u16(    d, 48));
              set_oil_valve_open_position_during_air_assist_mv(   u16(    d, 50));
              set_oil_valve_closed_position_at_end_mv(            u16(    d, 52));
              set_air_valve_open_cal_position_mv(                 u16(    d, 54));
              set_air_valve_closed_cal_position_mv(               u16(    d, 56));
              set_air_valve_mid_cal_position_mv(                  u16(    d, 58));
              set_oil_valve_open_cal_position_mv(                 u16(    d, 60));
              set_oil_valve_closed_cal_position_mv(               u16(    d, 62));
              set_pressure_offset_reading(                        i16(    d, 64));
              set_num_errors(                                     u08(    d, 66));

              // Parse the error codes.
              for(size_t i = 1; 6 * i + 67 < xmsg_size_bytes(); i++)
                  error_codes_.push_back(
                          make_shared<XEngErrorCode>(             s2000(  d, (6 * i) + 61),
                                                                  u16(    d, (6 * i) + 65)));

              break;
          }

            case 0x14: { // Engineering (Surface) message version 0x14.

              static const unsigned int MIN_SIZE = 76;

              if(data.size() < MIN_SIZE) {
                  ostringstream os;
                  os << "Invalid X-Message: Expected at least " << MIN_SIZE << " bytes, but "
                     <<"received " << data.size() << " bytes!";
#                 if defined(XMSG_DEBUG_ENG_SURF) && XMSG_DEBUG_ENG_SURF
                    lwrn() << os.str();
#                 else
                    EXC_THROW(os.str());
#                 endif
              }

              set_start_time(                                     s2000(  d, 4));
              set_surface_sampling_start_time(                    s2000(  d, 8));
              set_surface_sampling_end_time(                      s2000(  d, 12));
              set_num_sbd_packets_for_dive(                       u16(    d, 16));
              set_num_transmit_attempts_for_previous_dive(        u16(    d, 18));
              set_case_vac_at_start_cinhg(                        u16(    d, 20));
              set_case_vac_at_end_cinhg(                          u16(    d, 22));
              set_gps_rtc_time_delta(                             i16(    d, 24));
              set_ctd_status_code(                                u32(    d, 26));
              set_prof_raw_sample_first_pres(                     u16(    d, 30));
              set_prof_raw_sample_first_temp(                     u16(    d, 32));
              set_prof_raw_sample_first_saln(                     u16(    d, 34));
              set_prof_raw_sample_last_pres(                      u16(    d, 36));
              set_prof_raw_sample_last_temp(                      u16(    d, 38));
              set_prof_raw_sample_last_saln(                      u16(    d, 40));
              set_prof_num_bins(                                  u16(    d, 42));
              set_prof_num_raw_samples(                           u16(    d, 44));
              set_prof_num_bins_empty(                            u16(    d, 46));
              set_air_pump_current_ma(                            u16(    d, 48));
              set_air_pump_runtime_s(                             u16(    d, 50));
              set_air_bladder_max_pres_mpsi(                      u16(    d, 52));
              set_air_valve_open_position_at_start_mv(            u16(    d, 54));
              set_air_valve_mid_position_at_end_mv(               u16(    d, 56));
              set_oil_valve_open_position_during_air_assist_mv(   u16(    d, 58));
              set_oil_valve_closed_position_at_end_mv(            u16(    d, 60));
              set_air_valve_open_cal_position_mv(                 u16(    d, 62));
              set_air_valve_closed_cal_position_mv(               u16(    d, 64));
              set_air_valve_mid_cal_position_mv(                  u16(    d, 66));
              set_oil_valve_open_cal_position_mv(                 u16(    d, 68));
              set_oil_valve_closed_cal_position_mv(               u16(    d, 70));
              set_pressure_offset_reading(                        i16(    d, 72));
              set_num_errors(                                     u08(    d, 74));

              // Parse the error codes.
              for(size_t i = 0; 6 * i + 80 < xmsg_size_bytes(); i++)
                  error_codes_.push_back(
                          make_shared<XEngErrorCode>(             s2000(  d, (6 * i) + 75),
                                                                  u16(    d, (6 * i) + 79)));

              break;
            }

            default: {

              /* Handle unsupported Engineering (Surface) message version: */

              ostringstream os;
              os << "Unsupported ALAMO Engineering (Surface) message version: 0x" << hex
                 << (int) engineering_version() << "!";
              EXC_THROW(os.str());
              }

              break;
            }

            // Check the number of error codes.
            if(num_errors() != error_codes().size()) {
                ostringstream os;
                os << "Invalid X-Message: Expected " << (int) num_errors() << " error codes, "
                   << "but received " << error_codes().size() << " error codes!";
#               if defined(XMSG_DEBUG_ENG_SURF) && XMSG_DEBUG_ENG_SURF
                  lwrn() << os.str();
#               else
                  EXC_THROW(os.str());
#               endif
            }

        }CATCH_BLOCK("Failed to decode ALAMO Engineering (Surface) message!")
    }

    /** Generate the listing entry string for this surface engineering data X-Message.
     *
     *  @return         The listing entry string for this surface engineering data X-Message.
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

            case 0x10: { // Engineering (Surface) version 0x10.

                // Minor index 0.
                static const format fmt1("#   %5s %4s %4s %6s %20s %10s %5s %6s %6s %6s %5s %4s");
                static const format fmt2("#   %5s %4s %4s %6s %20s %10s %5s %6s %7s %6s %5s %4s");
                static const format fmt3(
                        "E0A %05d %4d %4d   0x%-02X %20s 0x%08X %5d %6d %6.2f %6.3f %5.3f %4d");

                os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-surface"
                           % "CTD-status" % "nSBD" % "nAtmpt" % "vac" % "sysV" % "sysI" % "nErr"
                   << endl;
                os << format(fmt2) % "" % "" % "" % "" % "" % "" % "" % "" % "(inHg)" % "(V)"
                           % "(A)" % ""
                   << endl;
                os << format(fmt3) % id_instrument() % id_dive() % 0 % (int) engineering_version()
                        % GenerateIso8601String(start_time()) % ctd_status_code()
                        % num_sbd_packets_for_dive() % num_transmit_attempts_for_previous_dive()
                        % case_vacuum_inhg() % battery_voltage_v() % system_current_a()
                        % num_errors();

                // Error codes.
                static const format fmt4("#   %5s %4s %4s %20s %15s");
                static const format fmt_err("e0A %05d %4d %4d %20s      0x%08X");
                if(num_errors()) {
                    os << endl << format(fmt4) % "SN" % "dive" % "idx" % "date-time" % "err-code";
                    int midx = 0;
                    for(vector<shared_ptr<XEngErrorCode>>::const_iterator it = error_codes_.begin();
                            it < error_codes_.end(); it++)
                        os << endl
                           << format(fmt_err) % id_instrument() % id_dive() % midx++
                                   % (*it)->DateTimeAs8601String() % (*it)->error_code();
                }
            }
                break;

            case 0x11: { // Engineering (Surface) version 0x11.

                // Minor Index 0.
                static const format fmt1("#   %5s %4s %4s %6s %20s %6s %6s %6s %12s");
                static const format fmt2("#   %5s %4s %4s %6s %20s %6s %6s %6s %12s");
                static const format fmt3("E0A %05d %4d %4d   0x%-02X %20s %6.2f %6.2f %6d %12d");
                os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-surface" % "vac0"
                           % "vac1" % "nSBD" % "nPrevAttempt"
                   << endl;
                os << format(fmt2) % "" % "" % "" % "" % "" % "(inHg)" % "(inHg)" % "" % "" << endl;
                os << format(fmt3) % id_instrument() % id_dive() % 0 % (int) engineering_version()
                           % GenerateIso8601String(start_time()) % case_vac_at_start_inhg()
                           % case_vac_at_end_inhg() % num_sbd_packets_for_dive()
                           % num_transmit_attempts_for_previous_dive()
                   << endl;

                // Minor Index 1.
                os << "# --- Air Pump & Bladder --------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt4("#   %5s %4s %4s %20s %20s %20s");
                static const format fmt5("#   %5s %4s %4s %20s %20s %20s");
                static const format fmt6("E0A %05d %4d %4d %20.3f %20d %20.3f");
                os << format(fmt4) % "SN" % "dive" % "midx" % "APCur" % "APRunT" % "AirBladMxP"
                   << endl;
                os << format(fmt5) % "" % "" % "" % "(A)" % "(sec)" % "(PSI)" << endl;
                os << format(fmt6) % id_instrument() % id_dive() % 1 % air_pump_current_a()
                           % air_pump_runtime_s() % air_bladder_max_pres_psi()
                   << endl;

                // Minor Index 2.
                os << "# --- Air Valve Position --------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt7("#   %5s %4s %4s %15s %15s %15s %15s %15s");
                static const format fmt8("#   %5s %4s %4s %15s %15s %15s %15s %15s");
                static const format fmt9("E0A %05d %4d %4d %15.3f %15.3f %15.3f %15.3f %15.3f");
                os << format(fmt7) % "SN" % "dive" % "midx" % "AVOpenStart" % "AVMidEnd"
                           % "ACVpenCal" % "AVMidCal" % "AVCloseCal"
                   << endl;
                os << format(fmt8) % "" % "" % "" % "(V)" % "(V)" % "(V)" % "(V)" % "(V)" << endl;
                os << format(fmt9) % id_instrument() % id_dive() % 2
                           % air_valve_open_position_at_start_v()
                           % air_valve_mid_position_at_end_v() % air_valve_open_cal_position_v()
                           % air_valve_mid_cal_position_v() % air_valve_closed_cal_position_v()
                   << endl;

                // Minor Index 3.
                os << "# --- Oil Valve Position --------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt10("#   %5s %4s %4s %15s %15s %15s %15s");
                static const format fmt11("#   %5s %4s %4s %15s %15s %15s %15s");
                static const format fmt12("E0A %05d %4d %4d %15.3f %15.3f %15.3f %15.3f");
                os << format(fmt10) % "SN" % "dive" % "midx" % "OVOpenAirPmp" % "OVCloseEnd"
                           % "OVOpenCal" % "OVClosCal"
                   << endl;
                os << format(fmt11) % "" % "" % "" % "(V)" % "(V)" % "(V)" % "(V)" << endl;
                os << format(fmt12) % id_instrument() % id_dive() % 3
                           % oil_valve_open_position_during_air_assist_v()
                           % oil_valve_closed_position_at_end_v() % oil_valve_open_cal_position_v()
                           % oil_valve_closed_cal_position_v()
                   << endl;

                // Minor Index 4.
                os << "# --- Errors --------------------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt13("#   %5s %4s %4s %15s");
                static const format fmt14("#   %5s %4s %4s %15s");
                static const format fmt15("e0A %05d %4d %4d %15d");
                os << format(fmt13) % "SN" % "dive" % "midx" % "nErr" << endl;
                os << format(fmt14) % "" % "" % "" % "" << endl;
                os << format(fmt15) % id_instrument() % id_dive() % 4 % (int) num_errors();

                // Error codes.
                if(!error_codes().size())
                    break;
                static const format fmt16("#   %5s %4s %4s %20s %15s");
                static const format fmt_err("e0A %05d %4d %4d %20s      0x%08X");

                if(num_errors()) {
                    os << endl << format(fmt16) % "SN" % "dive" % "idx" % "date-time" % "err-code";
                    int midx = 0;
                    for(vector<shared_ptr<XEngErrorCode>>::const_iterator it = error_codes_.begin();
                            it < error_codes_.end(); it++)
                        os << endl
                           << format(fmt_err) % id_instrument() % id_dive() % midx++
                                   % (*it)->DateTimeAs8601String() % (*it)->error_code();
                }
            }
                break;

            case 0x12: { // Engineering (Surface) version 0x12.

                // Minor Index 0.
                static const format fmt1("#   %5s %4s %4s %6s %20s %9s %9s %15s %15s");
                static const format fmt2("#   %5s %4s %4s %6s %20s %9s %9s %15s %15s");
                static const format fmt3("E0A %05d %4d %4d   0x%-02X %20s %9.2f %9.2f %15d %15d");
                os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-surface" % "vac0"
                           % "vac1" % "nSBD" % "nPrevAttempt"
                   << endl;
                os << format(fmt2) % "" % "" % "" % "" % "" % "(inHg)" % "(inHg)" % "" % "" << endl;
                os << format(fmt3) % id_instrument() % id_dive() % 0 % (int) engineering_version()
                           % GenerateIso8601String(start_time()) % case_vac_at_start_inhg()
                           % case_vac_at_end_inhg() % num_sbd_packets_for_dive()
                           % num_transmit_attempts_for_previous_dive()
                   << endl;

                // Minor Index 1.
                os << "# --- Air Pump & Bladder --------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt4("#   %5s %4s %4s %15s %15s %15s");
                static const format fmt5("#   %5s %4s %4s %15s %15s %15s");
                static const format fmt6("E0A %05d %4d %4d %15.3f %15d %15.3f");
                os << format(fmt4) % "SN" % "dive" % "midx" % "APCur" % "APRunT" % "AirBladMxP"
                   << endl;
                os << format(fmt5) % "" % "" % "" % "(A)" % "(sec)" % "(PSI)" << endl;
                os << format(fmt6) % id_instrument() % id_dive() % 1 % air_pump_current_a()
                           % air_pump_runtime_s() % air_bladder_max_pres_psi()
                   << endl;

                // Minor Index 2.
                os << "# --- Air Valve Position --------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt7("#   %5s %4s %4s %15s %15s %15s %15s %15s");
                static const format fmt8("#   %5s %4s %4s %15s %15s %15s %15s %15s");
                static const format fmt9("E0A %05d %4d %4d %15.3f %15.3f %15.3f %15.3f %15.3f");
                os << format(fmt7) % "SN" % "dive" % "midx" % "AVOpenStart" % "AVMidEnd"
                           % "ACVpenCal" % "AVMidCal" % "AVCloseCal"
                   << endl;
                os << format(fmt8) % "" % "" % "" % "(V)" % "(V)" % "(V)" % "(V)" % "(V)" << endl;
                os << format(fmt9) % id_instrument() % id_dive() % 2
                           % air_valve_open_position_at_start_v()
                           % air_valve_mid_position_at_end_v() % air_valve_open_cal_position_v()
                           % air_valve_mid_cal_position_v() % air_valve_closed_cal_position_v()
                   << endl;

                // Minor Index 3.
                os << "# --- Oil Valve Position --------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt10("#   %5s %4s %4s %15s %15s %15s %15s");
                static const format fmt11("#   %5s %4s %4s %15s %15s %15s %15s");
                static const format fmt12("E0A %05d %4d %4d %15.3f %15.3f %15.3f %15.3f");
                os << format(fmt10) % "SN" % "dive" % "midx" % "OVOpenAirPmp" % "OVCloseEnd"
                           % "OVOpenCal" % "OVClosCal"
                   << endl;
                os << format(fmt11) % "" % "" % "" % "(V)" % "(V)" % "(V)" % "(V)" << endl;
                os << format(fmt12) % id_instrument() % id_dive() % 3
                           % oil_valve_open_position_during_air_assist_v()
                           % oil_valve_closed_position_at_end_v() % oil_valve_open_cal_position_v()
                           % oil_valve_closed_cal_position_v()
                   << endl;

                // Minor Index 4.
                os << "# --- Surface Pressure ----------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt17("#   %5s %4s %4s "
                        "%15s");
                static const format fmt18("#   %5s %4s %4s "
                        "%15s");
                static const format fmt19("E0A %05d %4d %4d "
                        "%15s");
                os << format(fmt17) % "SN" % "dive" % "midx" //
                           % "presOffset" //
                   << endl;
                os << format(fmt18) % "" % "" % "" //
                           % "(dBar)"
                   << endl;
                os << format(fmt19) % id_instrument() % id_dive() % 4 // midx
                        % XMsgFormatPressure(GetDecodedBitPressureOffset()) //
                   << endl;

                // Minor Index 5.
                os << "# --- Errors --------------------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt13("#   %5s %4s %4s %15s");
                static const format fmt14("#   %5s %4s %4s %15s");
                static const format fmt15("e0A %05d %4d %4d %15d");
                os << format(fmt13) % "SN" % "dive" % "midx" % "nErr" << endl;
                os << format(fmt14) % "" % "" % "" % "" << endl;
                os << format(fmt15) % id_instrument() % id_dive() % 5 // midx
                        % (int) num_errors();

                // Error codes.
                if(!error_codes().size())
                    break;
                static const format fmt16("#   %5s %4s %4s %20s %15s");
                static const format fmt_err("e0A %05d %4d %4d %20s      0x%08X");

                if(num_errors()) {
                    os << endl << format(fmt16) % "SN" % "dive" % "idx" % "date-time" % "err-code";
                    int midx = 0;
                    for(vector<shared_ptr<XEngErrorCode>>::const_iterator it = error_codes_.begin();
                            it < error_codes_.end(); it++)
                        os << endl
                           << format(fmt_err) % id_instrument() % id_dive() % midx++
                                   % (*it)->DateTimeAs8601String() % (*it)->error_code();
                }
            }
                break;

            case 0x13: { // Engineering (Surface) version 0x13.

                unsigned int midx = 0;

                {   static const format fmt1("#   %5s %4s %4s %6s %20s %9s %9s %15s %15s");
                    static const format fmt2("#   %5s %4s %4s %6s %20s %9s %9s %15s %15s");
                    static const format fmt3(
                            "E0A %05d %4d %4d   0x%-02X %20s %9.2f %9.2f %15d %15d");
                    os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-surface"
                               % "vac0" % "vac1" % "nSBD" % "nPrevAttempt"
                       << endl;
                    os << format(fmt2) % "" % "" % "" % "" % "" % "(inHg)" % "(inHg)" % "" % ""
                       << endl;
                    os << format(fmt3) % id_instrument() % id_dive() % midx++
                               % (int) engineering_version() % GenerateIso8601String(start_time())
                               % case_vac_at_start_inhg() % case_vac_at_end_inhg()
                               % num_sbd_packets_for_dive()
                               % num_transmit_attempts_for_previous_dive()
                       << endl;
                }

                os << "# --- Air Pump & Bladder --------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt4("#   %5s %4s %4s %15s %15s %15s");
                static const format fmt5("#   %5s %4s %4s %15s %15s %15s");
                static const format fmt6("E0A %05d %4d %4d %15.3f %15d %15.3f");
                os << format(fmt4) % "SN" % "dive" % "midx" % "APCur" % "APRunT" % "AirBladMxP"
                   << endl;
                os << format(fmt5) % "" % "" % "" % "(A)" % "(sec)" % "(PSI)" << endl;
                os << format(fmt6) % id_instrument() % id_dive() % midx++ % air_pump_current_a()
                           % air_pump_runtime_s() % air_bladder_max_pres_psi()
                   << endl;

                os << "# --- Air Valve Position --------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt7("#   %5s %4s %4s %15s %15s %15s %15s %15s");
                static const format fmt8("#   %5s %4s %4s %15s %15s %15s %15s %15s");
                static const format fmt9("E0A %05d %4d %4d %15.3f %15.3f %15.3f %15.3f %15.3f");
                os << format(fmt7) % "SN" % "dive" % "midx" % "AVOpenStart" % "AVMidEnd"
                           % "ACVpenCal" % "AVMidCal" % "AVCloseCal"
                   << endl;
                os << format(fmt8) % "" % "" % "" % "(V)" % "(V)" % "(V)" % "(V)" % "(V)" << endl;
                os << format(fmt9) % id_instrument() % id_dive() % midx++
                           % air_valve_open_position_at_start_v()
                           % air_valve_mid_position_at_end_v() % air_valve_open_cal_position_v()
                           % air_valve_mid_cal_position_v() % air_valve_closed_cal_position_v()
                   << endl;

                os << "# --- Oil Valve Position --------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt10("#   %5s %4s %4s %15s %15s %15s %15s");
                static const format fmt11("#   %5s %4s %4s %15s %15s %15s %15s");
                static const format fmt12("E0A %05d %4d %4d %15.3f %15.3f %15.3f %15.3f");
                os << format(fmt10) % "SN" % "dive" % "midx" % "OVOpenAirPmp" % "OVCloseEnd"
                           % "OVOpenCal" % "OVClosCal"
                   << endl;
                os << format(fmt11) % "" % "" % "" % "(V)" % "(V)" % "(V)" % "(V)" << endl;
                os << format(fmt12) % id_instrument() % id_dive() % midx++
                           % oil_valve_open_position_during_air_assist_v()
                           % oil_valve_closed_position_at_end_v() % oil_valve_open_cal_position_v()
                           % oil_valve_closed_cal_position_v()
                   << endl;

                os << "# --- Surface Pressure ----------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt17("#   %5s %4s %4s "
                        "%15s");
                static const format fmt18("#   %5s %4s %4s "
                        "%15s");
                static const format fmt19("E0A %05d %4d %4d "
                        "%15s");
                os << format(fmt17) % "SN" % "dive" % "midx" //
                           % "presOffset" //
                   << endl;
                os << format(fmt18) % "" % "" % "" //
                           % "(dBar)"
                   << endl;
                os << format(fmt19) % id_instrument() % id_dive() % midx++ // midx
                        % XMsgFormatPressure(GetDecodedBitPressureOffset()) //
                   << endl;

                {   os << "# --- Profile - First Sample ------------------------------------------"
                        "-----------------------------"
                       << endl;
                    static const format fmt1(
                        "#   %5s %4s %4s "
                        "   %12s %12s "
                        "%12s %12s "
                        "%12s %12s");
                    static const format fmt2(
                        "#   %5s %4s %4s "
                        "   %12s %12s "
                        "%13s %12s "
                        "%12s %12s");
                    static const format fmt3(
                        "E0A %05d %4d %4d "
                        "   %12s %12d "
                        "%12.3f %12d "
                        "%12.3f %12d");
                    os << format(fmt1)
                        % "SN"          % "dive"        % "midx"
                        % "firstPres"   % "firstPres"
                        % "firstTemp"   % "firstTemp"
                        % "firstSaln"   % "firstSaln"
                        << endl;
                    os << format(fmt2)
                        % ""            % ""            % ""
                        % "(dBar)"      % "(counts)"
                        % "(degC)"        % "(counts)"
                        % "(PSU)"       % "(counts)"
                        << endl;
                    os << format(fmt3)
                        % id_instrument() % id_dive() % midx++
                        % XMsgFormatPressure(prof_raw_sample_first_pres_dbar(pg, po)) % prof_raw_sample_first_pres_counts()
                        % prof_raw_sample_first_temp_degc(tg, to) % prof_raw_sample_first_temp_counts()
                        % prof_raw_sample_first_saln_psu(sg, so)  % prof_raw_sample_first_saln_counts()
                        << endl;
                    os << "# --- Profile - Last Sample -------------------------------------------"
                        "-----------------------------"
                        << endl;
                    os << format(fmt1)
                        % "SN"          % "dive"        % "midx"
                        % "lastPres"    % "lastPres"
                        % "lastTemp"    % "lastTemp"
                        % "lastSaln"    % "lastSaln"
                        << endl;
                    os << format(fmt2)
                        % ""            % ""            % ""
                        % "(dBar)"      % "(counts)"
                        % "(degC)"        % "(counts)"
                        % "(PSU)"       % "(counts)"
                        << endl;
                    os << format(fmt3)
                        % id_instrument() % id_dive() % midx++
                        % XMsgFormatPressure(prof_raw_sample_last_pres_dbar(pg, po)) % prof_raw_sample_last_pres_counts()
                        % prof_raw_sample_last_temp_degc(tg, to) % prof_raw_sample_last_temp_counts()
                        % prof_raw_sample_last_saln_psu(sg, so)  % prof_raw_sample_last_saln_counts()
                        << endl;
                }

                {   os << "# --- Profile - Meta --------------------------------------------------"
                            "-----------------------------"
                        << endl;
                    static const format fmt1(
                        "#   %5s %4s %4s "
                        "%15s %15s %15s");
                    static const format fmt2(
                        "E0A %05d %4d %4d "
                        "%15d %15d %15d");
                    os << format(fmt1)
                         % "SN"                 % "dive"                % "midx"
                         % "numBins"            % "numEmptyBins"        % "numRawSamples"
                         << endl;
                    os << format(fmt2)
                         % id_instrument()      % id_dive()             % midx++
                         % prof_num_bins()      % prof_num_bins_empty() % prof_num_raw_samples()
                         << endl;
                }

                {   os << "# --- Real-Time Clock--------------------------------------------------"
                            "-----------------------------"
                        << endl;
                    static const format fmt1(
                        "#   %5s %4s %4s "
                        "%15s");
                    static const format fmt2(
                        "#   %5s %4s %4s "
                        "%15s");
                    static const format fmt3(
                        "E0A %05d %4d %4d "
                        "%15d");
                    os << format(fmt1)
                         % "SN"                 % "dive"                % "midx"
                         % "gpsOffset"
                         << endl;
                    os << format(fmt2)
                         % ""                   % ""                    % ""
                         % "(sec)"
                         << endl;
                    os << format(fmt3)
                         % id_instrument()      % id_dive()             % midx++
                         % gps_rtc_time_delta()
                         << endl;
                }

                // Minor Index 8.
                os << "# --- Errors --------------------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt13("#   %5s %4s %4s %15s");
                static const format fmt14("#   %5s %4s %4s %15s");
                static const format fmt15("E0A %05d %4d %4d %15d");
                os << format(fmt13) % "SN" % "dive" % "midx" % "nErr" << endl;
                os << format(fmt14) % "" % "" % "" % "" << endl;
                os << format(fmt15) % id_instrument() % id_dive() % midx++ // midx
                        % (int) num_errors();

                // Error codes.
                if(!error_codes().size())
                    break;
                static const format fmt16("#   %5s %4s %4s %20s %15s");
                static const format fmt_err("e0A %05d %4d %4d %20s      0x%08X");

                if(num_errors()) {
                    os << endl << format(fmt16) % "SN" % "dive" % "idx" % "date-time" % "err-code";
                    midx = 0;
                    for(vector<shared_ptr<XEngErrorCode>>::const_iterator it = error_codes_.begin();
                            it < error_codes_.end(); it++)
                        os << endl
                           << format(fmt_err) % id_instrument() % id_dive() % midx++
                                   % (*it)->DateTimeAs8601String() % (*it)->error_code();
                }
            }
                break;

            case 0x14: { // Engineering (Surface) version 0x14.

                unsigned int midx = 0;

                {   static const format fmt1("#   %5s %4s %4s %6s %20s %9s %9s %15s %15s");
                    static const format fmt2("#   %5s %4s %4s %6s %20s %9s %9s %15s %15s");
                    static const format fmt3(
                            "E0A %05d %4d %4d   0x%-02X %20s %9.2f %9.2f %15d %15d");
                    os << format(fmt1) % "SN" % "dive" % "midx" % "engVer" % "start-of-surface"
                               % "vac0" % "vac1" % "nSBD" % "nPrevAttempt"
                       << endl;
                    os << format(fmt2) % "" % "" % "" % "" % "" % "(inHg)" % "(inHg)" % "" % ""
                       << endl;
                    os << format(fmt3) % id_instrument() % id_dive() % midx++
                               % (int) engineering_version() % GenerateIso8601String(start_time())
                               % case_vac_at_start_inhg() % case_vac_at_end_inhg()
                               % num_sbd_packets_for_dive()
                               % num_transmit_attempts_for_previous_dive()
                       << endl;
                }

                os << "# --- Air Pump & Bladder --------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt4("#   %5s %4s %4s %15s %15s %15s");
                static const format fmt5("#   %5s %4s %4s %15s %15s %15s");
                static const format fmt6("E0A %05d %4d %4d %15.3f %15d %15.3f");
                os << format(fmt4) % "SN" % "dive" % "midx" % "APCur" % "APRunT" % "AirBladMxP"
                   << endl;
                os << format(fmt5) % "" % "" % "" % "(A)" % "(sec)" % "(PSI)" << endl;
                os << format(fmt6) % id_instrument() % id_dive() % midx++ % air_pump_current_a()
                           % air_pump_runtime_s() % air_bladder_max_pres_psi()
                   << endl;

                os << "# --- Air Valve Position --------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt7("#   %5s %4s %4s %15s %15s %15s %15s %15s");
                static const format fmt8("#   %5s %4s %4s %15s %15s %15s %15s %15s");
                static const format fmt9("E0A %05d %4d %4d %15.3f %15.3f %15.3f %15.3f %15.3f");
                os << format(fmt7) % "SN" % "dive" % "midx" % "AVOpenStart" % "AVMidEnd"
                           % "ACVpenCal" % "AVMidCal" % "AVCloseCal"
                   << endl;
                os << format(fmt8) % "" % "" % "" % "(V)" % "(V)" % "(V)" % "(V)" % "(V)" << endl;
                os << format(fmt9) % id_instrument() % id_dive() % midx++
                           % air_valve_open_position_at_start_v()
                           % air_valve_mid_position_at_end_v() % air_valve_open_cal_position_v()
                           % air_valve_mid_cal_position_v() % air_valve_closed_cal_position_v()
                   << endl;

                os << "# --- Oil Valve Position --------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt10("#   %5s %4s %4s %15s %15s %15s %15s");
                static const format fmt11("#   %5s %4s %4s %15s %15s %15s %15s");
                static const format fmt12("E0A %05d %4d %4d %15.3f %15.3f %15.3f %15.3f");
                os << format(fmt10) % "SN" % "dive" % "midx" % "OVOpenAirPmp" % "OVCloseEnd"
                           % "OVOpenCal" % "OVClosCal"
                   << endl;
                os << format(fmt11) % "" % "" % "" % "(V)" % "(V)" % "(V)" % "(V)" << endl;
                os << format(fmt12) % id_instrument() % id_dive() % midx++
                           % oil_valve_open_position_during_air_assist_v()
                           % oil_valve_closed_position_at_end_v() % oil_valve_open_cal_position_v()
                           % oil_valve_closed_cal_position_v()
                   << endl;

                os << "# --- Surface Pressure ----------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt17("#   %5s %4s %4s "
                        "%15s");
                static const format fmt18("#   %5s %4s %4s "
                        "%15s");
                static const format fmt19("E0A %05d %4d %4d "
                        "%15s");
                os << format(fmt17) % "SN" % "dive" % "midx" //
                           % "presOffset" //
                   << endl;
                os << format(fmt18) % "" % "" % "" //
                           % "(dBar)"
                   << endl;
                os << format(fmt19) % id_instrument() % id_dive() % midx++ // midx
                        % XMsgFormatPressure(GetDecodedBitPressureOffset()) //
                   << endl;

                {   os << "# --- Profile - First Sample ------------------------------------------"
                        "-----------------------------"
                       << endl;
                    static const format fmt1(
                        "#   %5s %4s %4s "
                        "   %12s %12s "
                        "%12s %12s "
                        "%12s %12s");
                    static const format fmt2(
                        "#   %5s %4s %4s "
                        "   %12s %12s "
                        "%13s %12s "
                        "%12s %12s");
                    static const format fmt3(
                        "E0A %05d %4d %4d "
                        "   %12s %12d "
                        "%12.3f %12d "
                        "%12.3f %12d");
                    os << format(fmt1)
                        % "SN"          % "dive"        % "midx"
                        % "firstPres"   % "firstPres"
                        % "firstTemp"   % "firstTemp"
                        % "firstSaln"   % "firstSaln"
                        << endl;
                    os << format(fmt2)
                        % ""            % ""            % ""
                        % "(dBar)"      % "(counts)"
                        % "(degC)"        % "(counts)"
                        % "(PSU)"       % "(counts)"
                        << endl;
                    os << format(fmt3)
                        % id_instrument() % id_dive() % midx++
                        % XMsgFormatPressure(prof_raw_sample_first_pres_dbar(pg, po)) % prof_raw_sample_first_pres_counts()
                        % prof_raw_sample_first_temp_degc(tg, to) % prof_raw_sample_first_temp_counts()
                        % prof_raw_sample_first_saln_psu(sg, so)  % prof_raw_sample_first_saln_counts()
                        << endl;
                    os << "# --- Profile - Last Sample -------------------------------------------"
                        "-----------------------------"
                        << endl;
                    os << format(fmt1)
                        % "SN"          % "dive"        % "midx"
                        % "lastPres"    % "lastPres"
                        % "lastTemp"    % "lastTemp"
                        % "lastSaln"    % "lastSaln"
                        << endl;
                    os << format(fmt2)
                        % ""            % ""            % ""
                        % "(dBar)"      % "(counts)"
                        % "(degC)"        % "(counts)"
                        % "(PSU)"       % "(counts)"
                        << endl;
                    os << format(fmt3)
                        % id_instrument() % id_dive() % midx++
                        % XMsgFormatPressure(prof_raw_sample_last_pres_dbar(pg, po)) % prof_raw_sample_last_pres_counts()
                        % prof_raw_sample_last_temp_degc(tg, to) % prof_raw_sample_last_temp_counts()
                        % prof_raw_sample_last_saln_psu(sg, so)  % prof_raw_sample_last_saln_counts()
                        << endl;
                }

                {   os << "# --- Profile - Meta --------------------------------------------------"
                            "-----------------------------"
                        << endl;
                    static const format fmt1(
                        "#   %5s %4s %4s "
                        "%15s %15s %15s");
                    static const format fmt2(
                        "E0A %05d %4d %4d "
                        "%15d %15d %15d");
                    os << format(fmt1)
                         % "SN"                 % "dive"                % "midx"
                         % "numBins"            % "numEmptyBins"        % "numRawSamples"
                         << endl;
                    os << format(fmt2)
                         % id_instrument()      % id_dive()             % midx++
                         % prof_num_bins()      % prof_num_bins_empty() % prof_num_raw_samples()
                         << endl;
                }

                {   os << "# --- Real-Time Clock--------------------------------------------------"
                            "-----------------------------"
                        << endl;
                    static const format fmt1(
                        "#   %5s %4s %4s "
                        "%15s");
                    static const format fmt2(
                        "#   %5s %4s %4s "
                        "%15s");
                    static const format fmt3(
                        "E0A %05d %4d %4d "
                        "%15d");
                    os << format(fmt1)
                         % "SN"                 % "dive"                % "midx"
                         % "gpsOffset"
                         << endl;
                    os << format(fmt2)
                         % ""                   % ""                    % ""
                         % "(sec)"
                         << endl;
                    os << format(fmt3)
                         % id_instrument()      % id_dive()             % midx++
                         % gps_rtc_time_delta()
                         << endl;
                }

                {   os << "# --- Surface Sampling ------------------------------------------------"
                            "-----------------------------"
                        << endl;
                    static const format fmt1(
                        "#   %5s %4s %4s "
                        "%20s %20s");
                    static const format fmt2(
                        "#   %5s %4s %4s "
                        "%20s %20s");
                    static const format fmt3(
                        "E0A %05d %4d %4d "
                        "%20s %20s");
                    os << format(fmt1)
                         % "SN"                 % "dive"                % "midx"
                         % "start"              % "end"
                         << endl;
                    os << format(fmt2)
                         % ""                   % ""                    % ""
                         % ""                   % ""
                         << endl;
                    os << format(fmt3)
                         % id_instrument()      % id_dive()             % midx++
                         % GenerateIso8601String(surface_sampling_start_time())
                         %  GenerateIso8601String(surface_sampling_end_time())
                         << endl;
                }

                // Minor Index 8.
                os << "# --- Errors --------------------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt13("#   %5s %4s %4s %15s");
                static const format fmt14("#   %5s %4s %4s %15s");
                static const format fmt15("E0A %05d %4d %4d %15d");
                os << format(fmt13) % "SN" % "dive" % "midx" % "nErr" << endl;
                os << format(fmt14) % "" % "" % "" % "" << endl;
                os << format(fmt15) % id_instrument() % id_dive() % midx++ // midx
                        % (int) num_errors();

                // Error codes.
                if(!error_codes().size())
                    break;
                static const format fmt16("#   %5s %4s %4s %20s %15s");
                static const format fmt_err("e0A %05d %4d %4d %20s      0x%08X");

                if(num_errors()) {
                    os << endl << format(fmt16) % "SN" % "dive" % "idx" % "date-time" % "err-code";
                    midx = 0;
                    for(vector<shared_ptr<XEngErrorCode>>::const_iterator it = error_codes_.begin();
                            it < error_codes_.end(); it++)
                        os << endl
                           << format(fmt_err) % id_instrument() % id_dive() % midx++
                                   % (*it)->DateTimeAs8601String() % (*it)->error_code();
                }
            }
                break;

            default: { // Unsupported Engineering (Surface) version.
                ostringstream os;
                os << "Unsupported ALAMO Engineering (Surface) message version: 0x"
                   << hex
                   << (int) engineering_version()
                   << "!";
                EXC_THROW(os.str());
            }
                break;
            }

        }CATCH_BLOCK("Failed to generate listing string for ALAMO Engineering (Surface) message!")

        return os.str();
    }

public:

    /* Public Member Functions - Accessor Methods: */

    uint16_t air_bladder_max_pres_mpsi() const {
        return air_bladder_max_pres_mpsi_;
    }

    template<typename T = float> T air_bladder_max_pres_psi() const {
        return (T) air_bladder_max_pres_mpsi() / 1000.;
    }

    template<typename T = float> T air_pump_current_a() const {
        return (T) air_pump_current_ma() / 1000.;
    }

    uint16_t air_pump_current_ma() const {
        return air_pump_current_ma_;
    }

    uint16_t air_pump_runtime_s() const {
        return air_pump_runtime_s_;
    }

    uint16_t air_valve_closed_cal_position_mv() const {
        return air_valve_closed_cal_position_mv_;
    }

    template<typename T = float> T air_valve_closed_cal_position_v() const {
        return (T) air_valve_closed_cal_position_mv() / 1000.;
    }

    uint16_t air_valve_mid_cal_position_mv() const {
        return air_valve_mid_cal_position_mv_;
    }

    template<typename T = float> T air_valve_mid_cal_position_v() const {
        return (T) air_valve_mid_cal_position_mv() / 1000.;
    }

    uint16_t air_valve_mid_position_at_end_mv() const {
        return air_valve_mid_position_at_end_mv_;
    }

    template<typename T = float> T air_valve_mid_position_at_end_v() const {
        return (T) air_valve_mid_position_at_end_mv() / 1000.;
    }

    uint16_t air_valve_open_cal_position_mv() const {
        return air_valve_open_cal_position_mv_;
    }

    template<typename T = float> T air_valve_open_cal_position_v() const {
        return (T) air_valve_open_cal_position_mv() / 1000.;
    }

    uint16_t air_valve_open_position_at_start_mv() const {
        return air_valve_open_position_at_start_mv_;
    }

    template<typename T = float> T air_valve_open_position_at_start_v() const {
        return (T) air_valve_open_position_at_start_mv() / 1000.;
    }

    uint16_t battery_voltage_mv() const {
        return battery_voltage_mv_;
    }

    /** Get the measured system battery voltage, in volts.
     *
     *  @return The measured system battery voltage, in volts.
     */
    template<typename T = float> T battery_voltage_v() const {
        return (T) battery_voltage_mv_ / 1000.;
    }

    uint16_t case_vac_at_end_cinhg() const {
        return case_vac_at_end_cinhg_;
    }

    template<typename T = float> T case_vac_at_end_inhg() const {
        return (T) case_vac_at_end_cinhg() / 100.;
    }

    uint16_t case_vac_at_start_cinhg() const {
        return case_vac_at_start_cinhg_;
    }

    template<typename T = float> T case_vac_at_start_inhg() const {
        return (T) case_vac_at_start_cinhg() / 100.;
    }

    uint16_t case_vac_cingh() const {
        return case_vac_cingh_;
    }

    /** Get the measured internal pressure case vacuum, in inches of Mercury.
     *
     *  @return     The measured internal pressure case vacuum, in inches of Mercury.
     */
    template<typename T = float> T case_vacuum_inhg() const {
        return (T) case_vac_cingh_ / 100.;
    }

    uint32_t ctd_status_code() const {
        return ctd_status_code_;
    }


    template<typename T = double> T GetDecodedBitPressureOffset() const {
        T val;

        val = (T) pressure_offset_reading() / (T) 1000;

        if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
            return XMSG_SENS_BAD_PRES;

        return val;
    }

    int16_t gps_rtc_time_delta() const {
        return gps_rtc_time_delta_;
    }

    uint8_t num_errors() const {
        return num_errors_;
    }

    uint16_t num_sbd_packets_for_dive() const {
        return num_sbd_packets_for_dive_;
    }

    uint16_t num_transmit_attempts_for_previous_dive() const {
        return num_transmit_attempts_for_previous_dive_;
    }

    uint16_t oil_valve_closed_cal_position_mv() const {
        return oil_valve_closed_cal_position_mv_;
    }

    template<typename T = float> T oil_valve_closed_cal_position_v() const {
        return (T) oil_valve_closed_cal_position_mv() / 1000.;
    }

    uint16_t oil_valve_closed_position_at_end_mv() const {
        return oil_valve_closed_position_at_end_mv_;
    }

    template<typename T = float> T oil_valve_closed_position_at_end_v() const {
        return (T) oil_valve_closed_position_at_end_mv() / 1000.;
    }

    uint16_t oil_valve_open_cal_position_mv() const {
        return oil_valve_open_cal_position_mv_;
    }

    template<typename T = float> T oil_valve_open_cal_position_v() const {
        return (T) oil_valve_open_cal_position_mv() / 1000.;
    }

    uint16_t oil_valve_open_position_during_air_assist_mv() const {
        return oil_valve_open_position_during_air_assist_mv_;
    }

    template<typename T = float> T oil_valve_open_position_during_air_assist_v() const {
        return (T) oil_valve_open_position_during_air_assist_mv() / 1000.;
    }

    /** Get the measured system current, in amperes.
     *
     *  @return     The measured system current, in amperes.
     */
    template<typename T = float> T system_current_a() const {
        return (T) system_current_ma_ / 1000.;
    }

    uint16_t system_current_ma() const {
        return system_current_ma_;
    }

    const vector<shared_ptr<XEngErrorCode> >& error_codes() const {
        return error_codes_;
    }

    int16_t pressure_offset_reading() const {
        return pressure_offset_reading_;
    }

    uint16_t prof_num_bins() const {
        return prof_num_bins_;
    }

    uint16_t prof_num_bins_empty() const {
        return prof_num_bins_empty_;
    }

    uint16_t prof_raw_sample_first_pres_counts() const {
        return prof_raw_sample_first_pres_;
    }

    template<typename T = float> T prof_raw_sample_first_pres_dbar(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) prof_raw_sample_first_pres_counts() / (T) gain - (T) offs : (T) XMSG_SENS_BAD_PRES;

        if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
            return XMSG_SENS_BAD_PRES;

        return val;
    }


    uint16_t prof_raw_sample_first_saln_counts() const {
        return prof_raw_sample_first_saln_;
    }

    template<typename T = float> T prof_raw_sample_first_saln_psu(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) prof_raw_sample_first_saln_counts() / (T) gain - (T) offs : (T) XMSG_SENS_BAD_SALN;

        if(val < XMSG_SENS_MIN_SALN || val > XMSG_SENS_MAX_SALN)
            return XMSG_SENS_BAD_SALN;

        return val;
    }

    uint16_t prof_raw_sample_first_temp_counts() const {
        return prof_raw_sample_first_temp_;
    }

    template<typename T = float> T prof_raw_sample_first_temp_degc(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) prof_raw_sample_first_temp_counts() / (T) gain - (T) offs : (T) XMSG_SENS_BAD_TEMP;

        if(val < XMSG_SENS_MIN_TEMP || val > XMSG_SENS_MAX_TEMP)
            return XMSG_SENS_BAD_TEMP;

        return val;
    }

    uint16_t prof_raw_sample_last_pres_counts() const {
        return prof_raw_sample_last_pres_;
    }

    template<typename T = float> T prof_raw_sample_last_pres_dbar(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) prof_raw_sample_last_pres_counts() / (T) gain - (T) offs : (T) XMSG_SENS_BAD_PRES;

        if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
            return XMSG_SENS_BAD_PRES;

        return val;
    }

    uint16_t prof_raw_sample_last_saln_counts() const {
        return prof_raw_sample_last_saln_;
    }

    template<typename T = float> T prof_raw_sample_last_saln_psu(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) prof_raw_sample_last_saln_counts() / (T) gain - (T) offs : (T) XMSG_SENS_BAD_SALN;

        if(val < XMSG_SENS_MIN_SALN || val > XMSG_SENS_MAX_SALN)
            return XMSG_SENS_BAD_SALN;

        return val;
    }


    uint16_t prof_raw_sample_last_temp_counts() const {
        return prof_raw_sample_last_temp_;
    }

    template<typename T = float> T prof_raw_sample_last_temp_degc(int16_t gain, int16_t offs) const {
        T val;

        val = gain ? (T) prof_raw_sample_last_temp_counts() / (T) gain - (T) offs : (T) XMSG_SENS_BAD_TEMP;

        if(val < XMSG_SENS_MIN_TEMP || val > XMSG_SENS_MAX_TEMP)
            return XMSG_SENS_BAD_TEMP;

        return val;
    }

    uint16_t prof_num_raw_samples() const {
        return prof_num_raw_samples_;
    }

    const ptime& surface_sampling_end_time() const {
      return surface_sampling_end_time_;
    }

    const ptime& surface_sampling_start_time() const {
      return surface_sampling_start_time_;
    }

protected:

    /* Protected Member Functions - Mutator Methods: */

    void set_air_bladder_max_pres_mpsi(uint16_t air_bladder_max_pres_mpsi) {
        air_bladder_max_pres_mpsi_ = air_bladder_max_pres_mpsi;
    }

    void set_air_pump_current_ma(uint16_t air_pump_current_ma) {
        air_pump_current_ma_ = air_pump_current_ma;
    }

    void set_air_pump_runtime_s(uint16_t air_pump_runtime_s) {
        air_pump_runtime_s_ = air_pump_runtime_s;
    }

    void set_air_valve_closed_cal_position_mv(uint16_t air_valve_closed_cal_position_mv) {
        air_valve_closed_cal_position_mv_ = air_valve_closed_cal_position_mv;
    }

    void set_air_valve_mid_cal_position_mv(uint16_t air_valve_mid_cal_position_mv) {
        air_valve_mid_cal_position_mv_ = air_valve_mid_cal_position_mv;
    }

    void set_air_valve_open_cal_position_mv(uint16_t air_valve_open_cal_position_mv) {
        air_valve_open_cal_position_mv_ = air_valve_open_cal_position_mv;
    }

    void set_air_valve_mid_position_at_end_mv(uint16_t air_valve_mid_position_at_end_mv) {
        air_valve_mid_position_at_end_mv_ = air_valve_mid_position_at_end_mv;
    }

    void set_air_valve_open_position_at_start_mv(uint16_t air_valve_open_position_at_start_mv) {
        air_valve_open_position_at_start_mv_ = air_valve_open_position_at_start_mv;
    }

    void set_battery_voltage_mv(uint16_t battery_voltage_mv) {
        battery_voltage_mv_ = battery_voltage_mv;
    }

    void set_case_vac_at_end_cinhg(uint16_t case_vac_at_end_cinhg) {
        case_vac_at_end_cinhg_ = case_vac_at_end_cinhg;
    }

    void set_case_vac_at_start_cinhg(uint16_t case_vac_at_start_cinhg) {
        case_vac_at_start_cinhg_ = case_vac_at_start_cinhg;
    }

    void set_case_vac_cingh(uint16_t case_vac_cingh) {
        case_vac_cingh_ = case_vac_cingh;
    }

    void set_ctd_status_code(uint32_t ctd_status_code) {
        ctd_status_code_ = ctd_status_code;
    }

    void set_error_codes(const vector<shared_ptr<XEngErrorCode> >& error_codes) {
        error_codes_ = error_codes;
    }

    void set_num_errors(uint8_t num_errors) {
        num_errors_ = num_errors;
    }

    void set_num_sbd_packets_for_dive(uint16_t num_sbd_packets_for_dive) {
        num_sbd_packets_for_dive_ = num_sbd_packets_for_dive;
    }

    void set_num_transmit_attempts_for_previous_dive(
            uint16_t num_transmit_attempts_for_previous_dive) {
        num_transmit_attempts_for_previous_dive_ = num_transmit_attempts_for_previous_dive;
    }

    void set_oil_valve_closed_cal_position_mv(uint16_t oil_valve_closed_cal_position_mv) {
        oil_valve_closed_cal_position_mv_ = oil_valve_closed_cal_position_mv;
    }

    void set_oil_valve_closed_position_at_end_mv(uint16_t oil_valve_closed_position_at_end_mv) {
        oil_valve_closed_position_at_end_mv_ = oil_valve_closed_position_at_end_mv;
    }

    void set_oil_valve_open_cal_position_mv(uint16_t oil_valve_open_cal_position_mv) {
        oil_valve_open_cal_position_mv_ = oil_valve_open_cal_position_mv;
    }

    void set_oil_valve_open_position_during_air_assist_mv(
            uint16_t oil_valve_open_position_during_air_assist_mv) {
        oil_valve_open_position_during_air_assist_mv_ = oil_valve_open_position_during_air_assist_mv;
    }

    void set_pressure_offset_reading(int16_t pressure_offset_reading) {
        pressure_offset_reading_ = pressure_offset_reading;
    }

    void set_system_current_ma(uint16_t system_current_ma) {
        system_current_ma_ = system_current_ma;
    }

    void set_prof_num_bins(uint16_t prof_num_bins) {
        prof_num_bins_ = prof_num_bins;
    }

    void set_prof_num_bins_empty(uint16_t prof_num_bins_empty) {
        prof_num_bins_empty_ = prof_num_bins_empty;
    }

    void set_prof_raw_sample_first_pres(uint16_t prof_raw_sample_first_pres) {
        prof_raw_sample_first_pres_ = prof_raw_sample_first_pres;
    }

    void set_prof_raw_sample_first_saln(uint16_t prof_raw_sample_first_saln) {
        prof_raw_sample_first_saln_ = prof_raw_sample_first_saln;
    }

    void set_prof_raw_sample_first_temp(uint16_t prof_raw_sample_first_temp) {
        prof_raw_sample_first_temp_ = prof_raw_sample_first_temp;
    }

    void set_prof_raw_sample_last_pres(uint16_t prof_raw_sample_last_pres) {
        prof_raw_sample_last_pres_ = prof_raw_sample_last_pres;
    }

    void set_prof_raw_sample_last_saln(uint16_t prof_raw_sample_last_saln) {
        prof_raw_sample_last_saln_ = prof_raw_sample_last_saln;
    }

    void set_prof_raw_sample_last_temp(uint16_t prof_raw_sample_last_temp) {
        prof_raw_sample_last_temp_ = prof_raw_sample_last_temp;
    }

    void set_prof_num_raw_samples(uint16_t prof_num_raw_samples) {
        prof_num_raw_samples_ = prof_num_raw_samples;
    }

    void set_gps_rtc_time_delta(int16_t gps_rtc_time_delta) {
        gps_rtc_time_delta_ = gps_rtc_time_delta;
    }

    void set_surface_sampling_end_time(const ptime& surface_sampling_end_time) {
      surface_sampling_end_time_ = surface_sampling_end_time;
    }

    void set_surface_sampling_start_time(const ptime& surface_sampling_start_time) {
      surface_sampling_start_time_ = surface_sampling_start_time;
    }
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XENGSURFACE_H_ */
