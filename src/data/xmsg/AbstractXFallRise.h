/** The @ref util::xmsg::AbstractXFallRise class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 2, 2014 10:38:13 AM
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

#ifndef ABSTRACTXFALLRISE_H_
#define ABSTRACTXFALLRISE_H_

/* Includes */

// local
#include "XMessageIndexable.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Type Definitions: */

/** Packing format type enumeration for rise and fall data X-Messages.
 *
 */
typedef enum XFallRiseDataPackFormat {

    /** The legacy 4-byte packing format.
     *
     *  @note       This format is only used by instrument previous to X-Message format version
     *              2.0.
     *
     */
    XRISEFALL_FMT_LEGACY = 0,

    /** The standard 5-byte packing format.
     *
     *  @note       This packing format was add in X-Message version 2.0.
     */
    XRISEFALL_FMT_STNDRD = 1,

    /** The standard 9-byte packing format with temperature and salinity.
     *
     *  @note       This packing format was add in X-Message version 2.2.
     */
    XRISEFALL_FMT_STNDRD_WITH_TEMP_AND_SALN = 2,

    /** Unknown, unset, or unspecified packing format.
     *
     */
    XRISEFALL_FMT_UNKNOWN = 0xFF

} XFallRiseDataPackFormat_t;

/* Class Definition: */

/** An abstract class representing a fall, rise, or surface pressure time-series data X-message.
 *
 */
class AbstractXFallRise:
        public util::date::DateUtils,
        public XMessageIndexable {

private:

    /** The packing format type identifier.
     *
     */
    XFallRiseDataPackFormat_t data_pack_format_;

    /** The start time reference.
     *
     */
    ptime start_time_;

    /** The raw counts of measured fall, rise, or surface time-series data.
     *
     */
    vector<XSensorTripletCodeCounts> sensor_data_;

protected:

    /** Set the enumerated data packing format value for this message.
     *
     *  @param[in]  data_pack_format    The enumerated data packing format value for this message
     *                                  to set.
     */
    void set_data_pack_format(XFallRiseDataPackFormat_t data_pack_format) {

        data_pack_format_ = data_pack_format;

    }

    /** Set the start time of this message.
     *
     *  @param[in]  start_time          The start time to set.
     */
    void set_start_time(const ptime& start_time) {

        start_time_ = start_time;

    }

    /** Unpack the fall/rise data from the binary message.
     *
     *  @param[in]      data            The binary message to unpack.
     *
     *  @throw          util::log::MessageException if
     *                      - the message is of insufficient length,
     *                      - an invalid packing format is encountered, or
     *                      - the message terminator is in an invalid location.
     *
     *  @return         The raw counts of measured fall or rise time-series data.
     */
    vector<XSensorTripletCodeCounts> UnpackData(const vector<uint8_t>& data) {

        try {

            const uint8_t* d = CheckXMsgData(data, -1, 7, false);

            // Clear previous sensor data, if any.
            sensor_data_.clear();

            {   // Get the message packing format.
                int fmt;
                switch((fmt = (d[1] & 0xF0) >> 4)) {
                case XRISEFALL_FMT_LEGACY:
                case XRISEFALL_FMT_STNDRD:
                case XRISEFALL_FMT_STNDRD_WITH_TEMP_AND_SALN:

                    set_data_pack_format(static_cast<XFallRiseDataPackFormat_t>(fmt));
                    break;

                default:

                    EXC_THROW(
                            "Unsupported rise/fall/surface data packing format: " + lexical_cast<string>(fmt) +"!");
                    break;
                }
            }

            // Parse the message header.
            set_start_time(s2000(d, 3));                        // Get start time.
            set_xmsg_index(0x0F & d[0]);                        // Get the message index.
            set_xmsg_size_bytes(((d[1] & 0x0F) << 8) | d[2]);   // Get the message size.

            // Check message size.
            if(xmsg_size_bytes() > data.size()) {

                EXC_THROW(
                        "Invalid XMessage size: message specifies at least " + lexical_cast<string>(xmsg_size_bytes()) + " bytes, but only " + lexical_cast<string>(xmsg_size_bytes()) +" bytes were received.");

            } else if(data.size() > xmsg_size_bytes()) {

                lwrn() << "Extra bytes in XMessage: message specifies "
                          << xmsg_size_bytes()
                          << "bytes, but "
                          << data.size()
                          << " bytes were received.";

            }

            // Check for terminator.
            if(d[xmsg_size_bytes() - 1] != 0x3B) {
                ostringstream os;
                os << "Invalid XMessage: expected terminator ';' (0x3B) at byte "
                   << (xmsg_size_bytes() - 1)
                   << ", but received 0x"
                   << hex
                   << d[xmsg_size_bytes() - 1];
                BOOST_THROW_EXCEPTION(log::MessageException(os));
            }

            {   // Unpack the sensor data.
                uint16_t sec;
                int32_t pres_counts, temp_counts = XMSG_SENS_BAD, saln_counts = XMSG_SENS_BAD;
                XPhaseCode_t phase_code = XPHASE_CODE_UNKNOWN;

                switch(data_pack_format()) {
                case XRISEFALL_FMT_LEGACY:

                    for(vector<uint8_t>::const_iterator it = data.begin() + 7; it < data.end() - 4;
                            ) {
                        sec = *it++ << 8; sec |= *it++;
                        pres_counts = *it++ << 8; pres_counts |= *it++;
                        sensor_data_.push_back(
                                XSensorTripletCodeCounts(
                                        sec,
                                        phase_code,
                                        pres_counts,
                                        temp_counts,
                                        saln_counts));
                    }
                    break;

                case XRISEFALL_FMT_STNDRD:

                    for(vector<uint8_t>::const_iterator it = data.begin() + 7; it < data.end() - 5;
                            ) {
                        sec = *it++ << 8; sec |= *it++;
                        phase_code = (XPhaseCode_t) (*it >> 4);
                        pres_counts = (*it++ & 0x0F) << 16; pres_counts |= *it++ << 8; pres_counts |= *it++;
                        sensor_data_.push_back(
                                XSensorTripletCodeCounts(
                                        sec,
                                        phase_code,
                                        pres_counts,
                                        temp_counts,
                                        saln_counts));
                    }
                    break;

                case XRISEFALL_FMT_STNDRD_WITH_TEMP_AND_SALN:

                    for(vector<uint8_t>::const_iterator it = data.begin() + 7; it < data.end() - 9;
                            ) {
                        sec = *it++ << 8; sec |= *it++;
                        phase_code = (XPhaseCode_t) (*it >> 4);
                        pres_counts = (*it++ & 0x0F) << 16; pres_counts |= *it++ << 8; pres_counts |= *it++;
                        temp_counts = *it++ << 8; temp_counts |= *it++;
                        saln_counts = *it++ << 8; saln_counts |= *it++;
                        sensor_data_.push_back(
                                XSensorTripletCodeCounts(
                                        sec,
                                        phase_code,
                                        pres_counts,
                                        temp_counts,
                                        saln_counts));
                    }
                    break;

                case XRISEFALL_FMT_UNKNOWN:

                    EXC_THROW("Unknown rise/fall/surface data packing format!");
                    break;
                }

            }

        }CATCH_BLOCK("Failed to unpack X-Message sensor data.")

        return sensor_data_;

    }

public:

    /** The parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        Binary X-message data to decode.
     *
     *  @exception  None. Never throws.
     */
    AbstractXFallRise(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) ATTR_NO_EXCEPTIONS :
                    XMessageIndexable(dive, whoi, data),
                    data_pack_format_(XRISEFALL_FMT_UNKNOWN),
                    start_time_(not_a_date_time) {

        /* Do nothing. */

    }

    /** The destructor.
     *
     */
    virtual ~AbstractXFallRise() {

        /* Do nothing. */

    }

    /** Get the enumerated data packing format type.
     *
     *  @return         The enumerated data packing format type.
     */
    XFallRiseDataPackFormat_t data_pack_format() const {

        return data_pack_format_;

    }

    /** Decode the X message.
     *
     *  @param[in]      data        The encoded X message to decode.
     *
     *  @throw          util::log::MessageException if
     *                      - an invalid X-Message type identifier is encountered.
     *                      - the message is of insufficient length,
     *                      - an invalid packing format is encountered, or
     *                      - the message terminator is in an invalid location.
     */
    virtual void DecodeXMsgData(const vector<uint8_t>& data) = 0;

    /** Get the listing entry comment string.
     *
     *  @return         The listing entry comment string.
     */
    static const string GenerateListingComment() {

        ostringstream os;

        try {

//            const format fmt("#   %5s %4s %3s %4s %20s %9s %7s %7s %9s   %s\n"
//                    "#   %5s %4s %3s %4s %21s%10s%8s %8s%9s   %s");
//            os << format(fmt)
//            % "SN" % "dive" % "idx" % "nSam" % "time" % "pres" % "temp" % "saln" % "phaseCode" % "phaseCodeName"
//            % "" % "" % "" % "" % "(start-time/+sec)" % "(dBar)" % "(degC)" % "(PSU)"  % "" % "";

          os << "#      SN dive idx nSam                 time      pres    temp    saln phaseCode   phaseCodeName"
             << endl
             << "#                                  (UTC/+sec)    (dBar)  (degC)  (PSU)";

        }CATCH_BLOCK("Failed to generate fall/rise data listing comment!")

        return os.str();
    }

    /** Get the number of samples in this message.
     *
     * @return          The number of samples in this message.
     */
    size_t num_samples() const {

        return sensor_data_.size();

    }

    /** Get the vector of sensor data triplets.
     *
     *  @return     The vector of sensor data triplets.
     */
    const vector<XSensorTripletCodeCounts>& sensor_data() const {

        return sensor_data_;

    }

    /** Get the start time of this message.
     *
     *  @return         The start time of this message.
     */
    const ptime& start_time() const {

        return start_time_;

    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* ABSTRACTXFALLRISE_H_ */
