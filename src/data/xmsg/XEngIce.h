/** The @ref util::xmsg::XEngIce class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Mar 22, 2016 4:39:49 PM
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

#ifndef XENGICE_H_
#define XENGICE_H_

/* Includes: */

#include "XAlamoEngineering.h"

/* Definitions: */

namespace util {
namespace xmsg {

/* Types: */

/** The ALAMO engineering (ice) message class.
 *
 *  An instance of this class represents a decoded ALAMO engineering (ice detection) X-message,
 *  which reports the ice detection status, surfacing attempts, threshold test values and any
 *  error codes recorded by the instrument.
 */
class XEngIce:
        public XAlamoEngineering {


    /** The engineering (ice) error code class.
     *
     *  An instance of this object represents a single error code reported in the ALAMO engineering
     *  (ice) data message. Each error consists of a timestamp and a 16-bit error code.
     */
    class XEngIceErrorCode:
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
        XEngIceErrorCode(ptime date_time, uint32_t error_code) :
                        date_time_(date_time),
                        error_code_(error_code) {
            /* Do nothing. */
        }

        /** The destructor.
         *
         */
        ~XEngIceErrorCode() {
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

    uint16_t case_vac_at_end_cinhg_;

    uint16_t case_vac_at_start_cinhg_;

    uint8_t status_operation_mode_;

    uint8_t status_urgency_;

    uint8_t status_ice_detection_;

    uint8_t num_surface_attempts_;

    uint16_t num_stored_sbd_msgs_;

    uint32_t time_since_last_transmission_secs_;

    uint16_t pres_min_counts_;

    uint16_t temp_threshold_test_;

    uint16_t saln_surface_test_;

    uint8_t num_errors_;

    /** The error code values for this dive, if any.
     *
     *
     */
    vector<std::shared_ptr<XEngIceErrorCode> > error_codes_;

public:

    /* Public Member Functions: */

    /** The parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XEngIce(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XAlamoEngineering(dive, whoi, data),
                    error_codes_(),
                    num_errors_(0) {

        // Decode message.
        DecodeXMsgData(data);

    }

    /** The destructor.
     *
     */
    ~XEngIce() {

        /* Do nothing. */

    }

    /** Decode an engineering (ice) X-message.
     *
     *  Byte 3 holds the message version. Versions 0x10 and 0x11 share the fields in bytes 4-19:
     *  operation mode, ice detection status, urgency, number of surfacing attempts, number of
     *  stored SBD messages, time since the last transmission, minimum pressure, temperature
     *  threshold test and surface salinity test. Version 0x11 adds error codes; its layout is
     *  described in the 0x11 case below.
     *
     *  @param[in]  data    The binary X-message data to decode.
     *
     *  @throw      util::log::MessageException if the message type, size or terminator is invalid,
     *              the version is unsupported, or (version 0x11) the error count does not match the
     *              number of error code entries.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {

            // Check message.
            const uint8_t* d = CheckXMsgData(data, XMSG_TYPE_ID_ENG_ICE, 21);

            // Decode message.
            set_engineering_version(d[3]);
            switch(engineering_version()) {

            case 0x10: // Engineering (Ice) message version 0x10.

                set_start_time(not_a_date_time);
                set_status_operation_mode(u08(d, 4));
                set_status_ice_detection(u08(d, 5));
                set_status_urgency(u08(d, 6));
                set_num_surface_attempts(u08(d, 7));
                set_num_stored_sbd_msgs(u16(d, 8));
                set_time_since_last_transmission_secs(u32(d, 10));
                set_pres_min_counts(u16(d, 14));
                set_temp_threshold_test(u16(d, 16));
                set_saln_surface_test(u16(d, 18));

                break;

            case 0x11: // TODO Engineering (Ice) message version 0x11.

                /* Message layout (from transmitted messages): bytes 4-19 as in version 0x10,
                 * bytes 20-21 an undocumented u16 field (observed values 0-30, apparently a
                 * counter; not decoded),
                 * byte 22 the number of error codes, then one 6-byte entry per error code from
                 * byte 23 (s2000 timestamp, u16 code), then the ';' terminator. A message with no
                 * error codes is 24 bytes. */

                // Check message size.
                if(data.size() < 24) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected at least 24 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                // Parse message.
                set_start_time(not_a_date_time);
                set_status_operation_mode(u08(d, 4));
                set_status_ice_detection(u08(d, 5));
                set_status_urgency(u08(d, 6));
                set_num_surface_attempts(u08(d, 7));
                set_num_stored_sbd_msgs(u16(d, 8));
                set_time_since_last_transmission_secs(u32(d, 10));
                set_pres_min_counts(u16(d, 14));
                set_temp_threshold_test(u16(d, 16));
                set_saln_surface_test(u16(d, 18));
                set_num_errors(u08(d, 22));

                // Parse the error codes (each entry must end before the terminator).
                for(size_t i = 0; 6 * i + 29 < xmsg_size_bytes(); i++)
                    error_codes_.push_back(
                            std::make_shared<XEngIceErrorCode>(
                                    s2000(d, (6 * i) + 23),
                                    u16(d, (6 * i) + 27)));

                // Check the number of error codes.
                if(num_errors() != error_codes().size()) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected "
                       << (int) num_errors()
                       << " error codes, but received "
                       << error_codes().size()
                       << " error codes!";
#                   if defined(XMSG_DEBUG_ENG_SURF) && XMSG_DEBUG_ENG_SURF
                    {
                        lwrn() << os.str();
                    }
#                   else
                    {
                        EXC_THROW(os.str());
                    }
#                   endif
                }



                break;

            default: { // Unsupported Engineering (Ice) message version.

                ostringstream os;
                os << "Unsupported ALAMO Engineering (Ice) message version: 0x"
                   << hex
                   << (int) engineering_version()
                   << "!";
                EXC_THROW(os.str());
            }

            }

        }CATCH_BLOCK("Failed to decode ALAMO Engineering (Ice) message!")

    }

    const string GenerateListingString(
            uint16_t pg,
            uint16_t po,
            uint16_t sg,
            uint16_t so,
            uint16_t tg,
            uint16_t to) const {
        ostringstream os;

        (void)tg;           // Unused parameters.
        (void)to;

        try {

            switch(engineering_version()) {

            case 0x10: { // Engineering (Ice) version 0x10.

                // Minor Index 0.
                static const format fmt1("#   %5s %4s %4s "
                        "%6s %8s");
                static const format fmt2("#   %5s %4s %4s "
                        "%6s %8s");
                static const format fmt3("E0C %05d %4d %4d   "
                        "0x%-02X %8d");
                os << format(fmt1) % "SN" % "dive" % "midx" //
                           % "engVer" % "mode"  //
                   << endl;
                os << format(fmt2) % "" % "" % "" //
                           % "" % "" //
                   << endl;
                os << format(fmt3) % id_instrument() % id_dive() % 0 // midx
                           % (int) engineering_version() % (int) status_operation_mode()
                   << endl;

                // Minor Index 1.
                os << "# --- Status --------------------------------------------------------------"
                   "-------------------------"
                   << endl;
                static const format fmt4("#   %5s %4s %4s "
                        "%15s %15s %15s %15s %15s");
                static const format fmt5("#   %5s %4s %4s "
                        "%15s %15s %15s %15s %15s");
                static const format fmt6("E0C %05d %4d %4d "
                        "%15d %15d %15d %15d %15d");
                os << format(fmt4) % "SN" % "dive" % "midx" //
                           % "ice-status" % "urgency" % "surfAttempts" % "numStoredSBD" % "lastXmit"
                   << endl;
                os << format(fmt5) % "" % "" % ""  //
                           % "" % "" % "" % "" % "(sec)"  //
                   << endl;
                os << format(fmt6) % id_instrument() % id_dive()
                           % 1 // midx
                           % (int) status_ice_detection() % (int) status_urgency()
                           % (int) num_surface_attempts() % (int) num_stored_sbd_msgs()
                           % time_since_last_transmission_secs() //

                   << endl;

                // Minor Index 2.
                static const format fmt7("#   %5s %4s %4s "
                        "%15s %15s %15s");
                static const format fmt8("#   %5s %4s %4s "
                        "%15s %16s %15s");
                static const format fmt9("E0C %05d %4d %4d "
                        "%15.3f %15.3d %15.3d");
                os << format(fmt7) % "SN" % "dive" % "midx" //
                           % "minPres" % "testTemp" % "testSalinity"
                   << endl;
                os << format(fmt8) % "" % "" % ""  //
                           % "(dBar)" % "(degC)" % "(PSU)"  //
                   << endl;
                os << format(fmt9) % id_instrument() % id_dive()
                        % 2 // midx
                        % GetDecodedPresMin(pg, po) % temp_threshold_test()
                        % GetDecodedSalnSurfaceTest(sg, so);
            }

                break;

            case 0x11: { // TODO Engineering (Ice) message version 0x11.

              // Minor Index 0.
              static const format fmt1("#   %5s %4s %4s "
                      "%6s %8s");
              static const format fmt2("#   %5s %4s %4s "
                      "%6s %8s");
              static const format fmt3("E0C %05d %4d %4d   "
                      "0x%-02X %8d");
              os << format(fmt1) % "SN" % "dive" % "midx" //
                         % "engVer" % "mode"  //
                 << endl;
              os << format(fmt2) % "" % "" % "" //
                         % "" % "" //
                 << endl;
              os << format(fmt3) % id_instrument() % id_dive() % 0 // midx
                         % (int) engineering_version() % (int) status_operation_mode()
                 << endl;

              // Minor Index 1.
              os << "# --- Status --------------------------------------------------------------"
                 "-------------------------"
                 << endl;
              static const format fmt4("#   %5s %4s %4s "
                      "%15s %15s %15s %15s %15s");
              static const format fmt5("#   %5s %4s %4s "
                      "%15s %15s %15s %15s %15s");
              static const format fmt6("E0C %05d %4d %4d "
                      "%15d %15d %15d %15d %15d");
              os << format(fmt4) % "SN" % "dive" % "midx" //
                         % "ice-status" % "urgency" % "surfAttempts" % "numStoredSBD" % "lastXmit"
                 << endl;
              os << format(fmt5) % "" % "" % ""  //
                         % "" % "" % "" % "" % "(sec)"  //
                 << endl;
              os << format(fmt6) % id_instrument() % id_dive()
                         % 1 // midx
                         % (int) status_ice_detection() % (int) status_urgency()
                         % (int) num_surface_attempts() % (int) num_stored_sbd_msgs()
                         % time_since_last_transmission_secs() //

                 << endl;

              // Minor Index 2.
              static const format fmt7("#   %5s %4s %4s "
                      "%15s %15s %15s");
              static const format fmt8("#   %5s %4s %4s "
                      "%15s %16s %15s");
              static const format fmt9("E0C %05d %4d %4d "
                      "%15.3f %15.3d %15.3d");
              os << format(fmt7) % "SN" % "dive" % "midx" //
                         % "minPres" % "testTemp" % "testSalinity"
                 << endl;
              os << format(fmt8) % "" % "" % ""  //
                         % "(dBar)" % "(degC)" % "(PSU)"  //
                 << endl;
              os << format(fmt9) % id_instrument() % id_dive()
                      % 2 // midx
                      % GetDecodedPresMin(pg, po) % temp_threshold_test()
                      % GetDecodedSalnSurfaceTest(sg, so) << endl;

              // Minor Index 3.
              os << "# --- Errors --------------------------------------------------------------"
                 "-------------------------"
                 << endl;
              static const format fmt13("#   %5s %4s %4s %15s");
              static const format fmt14("#   %5s %4s %4s %15s");
              static const format fmt15("E0C %05d %4d %4d %15d");
              os << format(fmt13) % "SN" % "dive" % "midx" % "nErr" << endl;
              os << format(fmt14) % "" % "" % "" % "" << endl;
              os << format(fmt15) % id_instrument() % id_dive() % 3// midx
                      % (int) num_errors();

              // Error codes.
              if(!error_codes().size())
                  break;
              static const format fmt16("#   %5s %4s %4s %20s %15s");
              static const format fmt_err("e0c %05d %4d %4d %20s      0x%08X");

              int midx;

              if(num_errors()) {
                  os << endl << format(fmt16) % "SN" % "dive" % "idx" % "date-time" % "err-code";
                  midx = 0;
                  for(vector<shared_ptr<XEngIceErrorCode>>::const_iterator it = error_codes_.begin();
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
        }CATCH_BLOCK("Failed to generate listing string for ALAMO Engineering (Ice) message!")

        return os.str();
    }

    template<typename T = float> T GetDecodedCaseVacAtEnd() const {
        return (T) case_vac_at_end_cinhg() / (T) 100.;
    }

    template<typename T = float> T GetDecodedCaseVacAtStart() const {
        return (T) case_vac_at_start_cinhg() / (T) 100.;
    }

    template<typename T = double> T GetDecodedPresMin(int16_t gain, int16_t offs) const {
        T val;

        val = (T) pres_min_counts() / (T) gain - (T) offs;

        if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
            return XMSG_SENS_BAD_PRES;

        return val;
    }

    template<typename T = double> T GetDecodedTempThresholdTest(int16_t gain, int16_t offs) const {
        T val;

        val = (T) temp_threshold_test() / (T) gain - (T) offs;

        if(val < XMSG_SENS_MIN_TEMP || val > XMSG_SENS_MIN_TEMP)
            return XMSG_SENS_BAD_PRES;

        return val;
    }

    template<typename T = double> T GetDecodedSalnSurfaceTest(int16_t gain, int16_t offs) const {
        T val;

        val = (T) saln_surface_test() / (T) gain - (T) offs;

        if(val < XMSG_SENS_MIN_SALN || val > XMSG_SENS_MIN_SALN)
            return XMSG_SENS_MIN_SALN;

        return val;
    }

public:

    /* Public Accessor Methods: */

    uint16_t case_vac_at_end_cinhg() const {
        return case_vac_at_end_cinhg_;
    }

    uint16_t case_vac_at_start_cinhg() const {
        return case_vac_at_start_cinhg_;
    }

    uint16_t num_stored_sbd_msgs() const {
        return num_stored_sbd_msgs_;
    }

    uint8_t num_surface_attempts() const {
        return num_surface_attempts_;
    }

    uint16_t pres_min_counts() const {
        return pres_min_counts_;
    }

    uint16_t saln_surface_test() const {
        return saln_surface_test_;
    }

    uint8_t status_ice_detection() const {
        return status_ice_detection_;
    }

    uint8_t status_operation_mode() const {
        return status_operation_mode_;
    }

    uint8_t status_urgency() const {
        return status_urgency_;
    }

    uint16_t temp_threshold_test() const {
        return temp_threshold_test_;
    }

    uint32_t time_since_last_transmission_secs() const {
        return time_since_last_transmission_secs_;
    }


    const vector<shared_ptr<XEngIceErrorCode> >& error_codes() const {
      return error_codes_;
    }

    uint8_t num_errors() const {
      return num_errors_;
    }


protected:

    /* Protected Mutator Methods: */

    void set_case_vac_at_end_cinhg(uint16_t case_vac_at_end_cinhg) {
        case_vac_at_end_cinhg_ = case_vac_at_end_cinhg;
    }

    void set_case_vac_at_start_cinhg(uint16_t case_vac_at_start_cinhg) {
        case_vac_at_start_cinhg_ = case_vac_at_start_cinhg;
    }

    void set_num_stored_sbd_msgs(uint16_t num_stored_sbd_msgs) {
        num_stored_sbd_msgs_ = num_stored_sbd_msgs;
    }

    void set_num_surface_attempts(uint8_t num_surface_attempts) {
        num_surface_attempts_ = num_surface_attempts;
    }

    void set_pres_min_counts(uint16_t pres_min_counts) {
        pres_min_counts_ = pres_min_counts;
    }

    void set_saln_surface_test(uint16_t saln_surface_test) {
        saln_surface_test_ = saln_surface_test;
    }

    void set_status_ice_detection(uint8_t status_ice_detection) {
        status_ice_detection_ = status_ice_detection;
    }

    void set_status_operation_mode(uint8_t status_operation_mode) {
        status_operation_mode_ = status_operation_mode;
    }

    void set_status_urgency(uint8_t status_urgency) {
        status_urgency_ = status_urgency;
    }

    void set_temp_threshold_test(uint16_t temp_threshold_test) {
        temp_threshold_test_ = temp_threshold_test;
    }

    void set_time_since_last_transmission_secs(uint32_t time_since_last_transmission_secs) {
        time_since_last_transmission_secs_ = time_since_last_transmission_secs;
    }


    void set_num_errors(uint8_t num_errors) {
      num_errors_ = num_errors;
    }

};

/* Functions: */

} /* namespace xmsg */
} /* namespace util */

#endif /* XENGICE_H_ */
