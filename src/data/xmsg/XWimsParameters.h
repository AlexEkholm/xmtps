/** The @ref util::xmsg::XWimsParameters class header file.
 *
 *  @file
 *
 *  @version    1.0
 *
 *  @date       Oct 29, 2026
 *
 *  @author     Mr. Alexander K. Ekholm <aekholm@whoi.edu>,
 *              Research Engineer,
 *              Department of Physical Oceanography,
 *              Woods Hole Oceanographic Institution
 *
 *  @copyright  2026 Woods Hole Oceanographic Institution.
 *              Released under the GNU General Public License.
 *
 *  This is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 *  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details. You should have received a copy of the GNU General
 *  Public License along with this software. If not, please visit http://www.gnu.org/licenses/.
 */

#ifndef XWIMSPARAMETERS_H_
#define XWIMSPARAMETERS_H_

/* Includes */

#include "XMessage.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Class representing a WIMS configuration parameter X-Message (0xC5).
 *
 *  There is no format document; the layout is inferred from production messages (spec C.7). A
 *  float sends one at its diagnostic dive (0) and after a "WCG -1" (WIMS Command Get, all) upload.
 *  The usual ID and 12-bit size, a version at byte 3 (1: 42 parameters; 2: 43, adding the
 *  accelerometer gain), a part index at byte 4, then one big-endian 16-bit value per WIMS
 *  configuration parameter in the float's parameter index order (the index of the WCG and WCS
 *  commands), and the ';' terminator.
 *
 *  The parameter order is not the WIMS user manual's. Parameter names are those of the WIMS
 *  firmware; see GetParam() for which are confirmed from production data and which are inferred.
 */
class XWimsParameters:
        public XMessage {

public:

    /** A WIMS configuration parameter at a given index. */
    struct Param {
        const char* name;   //!< The WIMS parameter name; "?" when it is inferred, "" when unknown.
        bool is_signed;     //!< Whether the value is signed (int16) rather than unsigned (uint16).
    };

    /** The number of parameters GetParam() names. */
    static const size_t kNumParams = 43;

    /** Get a parameter by index. Confirmed: the calibration offsets (values match the IMU), the
     *  IMU accelerometer and gyroscope ranges and sample rate, the segment length, sample rate
     *  divider and gain (all three agree with the WVS line of the float's wave spectra), and the
     *  values set by WCS uploads. Names ending in "?" are inferred from their values (defaults in
     *  the WIMS user manual); empty names are unknown.
     *
     *  @param[in]  i       The parameter index, less than kNumParams.
     *
     *  @return     The parameter at index i.
     */
    static const Param& GetParam(size_t i) {
      static const Param params[kNumParams] = {
        { "brd.leds?", false },     //  0
        { "", false },              //  1
        { "cal.acofx", true },      //  2
        { "cal.acofy", true },      //  3
        { "cal.acofz", true },      //  4
        { "cal.acscx", true },      //  5
        { "cal.acscy", true },      //  6
        { "cal.acscz", true },      //  7
        { "cal.gcofx", true },      //  8
        { "cal.gcofy", true },      //  9
        { "cal.gcofz", true },      // 10
        { "cal.gcscx", true },      // 11
        { "cal.gcscy", true },      // 12
        { "cal.gcscz", true },      // 13
        { "cal.mcofx", true },      // 14
        { "cal.mcofy", true },      // 15
        { "cal.mcofz", true },      // 16
        { "cal.mcscx?", true },     // 17
        { "cal.mcscy?", true },     // 18
        { "cal.mcscz?", true },     // 19
        { "", false },              // 20
        { "imu.dlpf?", false },     // 21
        { "imu.fsra", false },      // 22
        { "imu.fsrg", false },      // 23
        { "imu.srate", false },     // 24
        { "", false },              // 25
        { "", false },              // 26
        { "", false },              // 27
        { "wav.afilt?", false },    // 28
        { "wav.mxsam?", false },    // 29
        { "wav.mxseg?", false },    // 30
        { "", false },              // 31
        { "wav.nfwax?", false },    // 32
        { "wav.obeta?", false },    // 33
        { "wav.ostsc?", false },    // 34
        { "wav.omode?", false },    // 35
        { "wav.ozeta?", false },    // 36
        { "", false },              // 37
        { "wav.smode?", false },    // 38
        { "wav.srdiv", false },     // 39
        { "wav.slen", false },      // 40
        { "wav.twalp?", false },    // 41
        { "wav.gaina", false },     // 42
      };
      return params[i];
    }

    /* Member Functions */

    /** The parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XWimsParameters(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
            XMessage(dive, whoi, data) {
        DecodeXMsgData(data); // Decode message.
    }

    /** The destructor. */
    ~XWimsParameters() {
        /* Do nothing. */
    }

    /** Decode the WIMS configuration parameter X-Message from the binary message data.
     *
     *  @param[in]      data        The binary message data to decode.
     *
     *  @throw          util::log::MessageException if the message is invalid or an error occurs
     *                  during decoding.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {

            // Check message: ID, size and terminator; the header (ID, size, version, part) and
            // terminator need 6 bytes.
            const uint8_t* d = CheckXMsgData(data, XMSG_TYPE_ID_WIMS_PARAMS, 6);
            const size_t len = 0x0FFF & u16(d, 1);

            if(len < 6)
                EXC_THROW("WIMS parameter message too short: size " + lexical_cast<string>(len)
                          + ", but at least 6 bytes are required!");
            if((len - 6) % 2)
                EXC_THROW("WIMS parameter message has an odd number of value bytes: size "
                          + lexical_cast<string>(len) + "!");

            version_ = d[3];
            part_ = d[4];
            values_.clear();
            for(size_t i = 5; i + 2 <= len - 1; i += 2)
                values_.push_back(u16(d, i));

            if((version_ == 1 && values_.size() != 42) || (version_ == 2 && values_.size() != 43))
                lwrn() << format("WIMS parameter message version %d has %d values; %d expected.")
                        % (int) version_ % values_.size() % (version_ == 1 ? 42 : 43);

        }CATCH_BLOCK("Failed to decode WIMS parameter message!")
    }

    /** Generate a listing string representation of the parameters: one WIP line per parameter.
     *
     *  @param[in]  midx    The index of this message within the dive.
     *
     *  @return     A listing string representation of the parameters.
     */
    const string GenerateListingString(int midx) const {
        ostringstream os;

        try {

            for(size_t i = 0; i < values_.size(); ++i) {
                const char* name = i < kNumParams && GetParam(i).name[0] ? GetParam(i).name : "-";
                const int value = (i < kNumParams && GetParam(i).is_signed) ? (int) (int16_t) values_[i]
                                                                             : (int) values_[i];
                os << format("WIP %5d %4d %4d %4d %4d %4d  %-11s %6d  0x%04X") % id_instrument()
                        % id_dive() % midx % (int) version_ % (int) part_ % i % name % value
                        % values_[i] << endl;
            }

        }CATCH_BLOCK("Failed to generate listing string for WIMS parameter message!")

        return os.str();
    }

    /** Get the message version (byte 3). */
    uint8_t version() const {
        return version_;
    }

    /** Get the part index (byte 4). */
    uint8_t part() const {
        return part_;
    }

    /** Get the raw parameter values, by parameter index. */
    const vector<uint16_t>& values() const {
        return values_;
    }

private:

    /** The message version (byte 3). */
    uint8_t version_ = 0;

    /** The part index (byte 4). */
    uint8_t part_ = 0;

    /** The raw parameter values, by parameter index. */
    vector<uint16_t> values_;
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XWIMSPARAMETERS_H_ */
