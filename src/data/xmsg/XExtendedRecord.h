/** The @ref util::xmsg::XExtendedRecord class header file.
 *
 *  @file
 *
 *  @version    1.0
 *
 *  @date       Oct 27, 2026
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

#ifndef XEXTENDEDRECORD_H_
#define XEXTENDEDRECORD_H_

/* Includes */

#include <algorithm>
#include <cstring>

#include "XMessage.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Class representing an extended record X-Message (0xFF).
 *
 *  There is no format document; the layout is inferred from production messages (spec C.6): the
 *  usual ID and 12-bit size, a record type at byte 3 and its version at byte 4, the record from
 *  byte 5, and the ';' terminator. Known record types:
 *
 *  - 0xCA v0x20 (17 bytes): u24 event code, u32 time, f32 value [inferred: a pressure in dBar].
 *  - 0xD1 v0x10 (22 bytes): u16 counter, u32 time, u16 count, u32 time, u32 value.
 *  - 0xAE v0x10/0x11 (12/13 bytes): u16 at offset 5 (always 0), u16 float serial number, then 2
 *    (v0x10) or 3 (v0x11) configuration bytes.
 *  - 0xDB (any size; byte 4 is not a version): a CTD exchange: u16 command length at bytes 4-5,
 *    the command text (e.g. "ctdrw fetch\r|5"), then u16 reply length and the CTD's reply.
 *  - 0xFA (281 bytes): text: a processor fault dump (ARM Cortex-M fault and stacked registers);
 *    all NUL when empty.
 *
 *  Times are UNIX seconds, or seconds since the controller started when below 2000-01-01. Other
 *  record types are listed as hex.
 */
class XExtendedRecord:
        public XMessage {

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
    XExtendedRecord(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
            XMessage(dive, whoi, data) {
        DecodeXMsgData(data); // Decode message.
    }

    /** The destructor. */
    ~XExtendedRecord() {
        /* Do nothing. */
    }

    /** Decode the extended record X-Message from the binary message data.
     *
     *  @param[in]      data        The binary message data to decode.
     *
     *  @throw          util::log::MessageException if the message is invalid or an error occurs during
     *                  decoding.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {

            // Check message: ID, size, type, version and terminator.
            const uint8_t* d = CheckXMsgData(data, XMSG_TYPE_ID_UNKNOWN, 6);
            const size_t len = 0x0FFF & u16(d, 1);

            // CheckXMsgData checks the data length, but the record ends at the size field, which
            // need only be 4 or more: the header (ID, size, type, version) and terminator need 6.
            if(len < 6)
                EXC_THROW("Extended record message too short: size " + lexical_cast<string>(len)
                          + ", but at least 6 bytes are required!");

            record_type_ = d[3];
            record_version_ = d[4];
            record_.assign(d + 5, d + len - 1);

        }CATCH_BLOCK("Failed to decode extended record message!")
    }

    /** Generate a listing string representation of the extended record: one XFF line.
     *
     *  @param[in]  ridx    The index of this record within the dive.
     *
     *  @return     A listing string representation of the extended record.
     */
    const string GenerateListingString(int ridx) const {
        ostringstream os;

        try {

            os << format("XFF %5d %4d %4d 0x%02X 0x%02X  %s") % id_instrument() % id_dive() % ridx
                    % (int) record_type_ % (int) record_version_ % DescribeRecord() << endl;

        }CATCH_BLOCK("Failed to generate listing string for extended record message!")

        return os.str();
    }

    /** Get the record type (byte 3). */
    uint8_t record_type() const {
        return record_type_;
    }

    /** Get the record version (byte 4). */
    uint8_t record_version() const {
        return record_version_;
    }

    /** Get the record bytes (from byte 5, without the terminator). */
    const vector<uint8_t>& record() const {
        return record_;
    }

private:

    /** Describe the record: the decoded fields of a known record type, otherwise its hex. */
    string DescribeRecord() const {

        ostringstream os;
        const vector<uint8_t>& r = record_;

        if(record_type_ == 0xCA && record_version_ == 0x20 && r.size() == 11) {

            // u24 event code, u32 time, f32 value
            uint32_t v = (uint32_t) r[7] << 24 | (uint32_t) r[8] << 16 | (uint32_t) r[9] << 8 | r[10];
            float value;
            memcpy(&value, &v, sizeof(value));
            os << format("event 0x%06X  time %s  value %.4f")
                    % ((uint32_t) r[0] << 16 | (uint32_t) r[1] << 8 | r[2]) % FormatTime(U32(r, 3))
                    % value;

        } else if(record_type_ == 0xD1 && record_version_ == 0x10 && r.size() == 16) {

            // u16 counter, u32 time, u16 count, u32 time, u32 value
            os << format("counter %d  time %s  count %d  time %s  value %u") % U16(r, 0)
                    % FormatTime(U32(r, 2)) % U16(r, 6) % FormatTime(U32(r, 8)) % U32(r, 12);

        } else if(record_type_ == 0xAE && (record_version_ == 0x10 || record_version_ == 0x11)
                && r.size() >= 4) {

            // u16 (0), u16 serial number, configuration bytes
            os << format("serial %d  config") % U16(r, 2);
            for(size_t i = 4; i < r.size(); ++i)
                os << format(" 0x%02X") % (int) r[i];

        } else if(record_type_ == 0xDB && !r.empty()
                && (size_t) (record_version_ << 8 | r[0]) + 1 <= r.size()) {

            // CTD exchange: u16 command length (bytes 4-5), command, u16 reply length, reply
            const size_t ncmd = record_version_ << 8 | r[0];
            os << "command \"" << Text(vector<uint8_t>(r.begin() + 1, r.begin() + 1 + ncmd)) << "\"";
            const size_t i = 1 + ncmd;
            if(i + 2 <= r.size()) {
                const size_t nrep = min<size_t>(U16(r, i), r.size() - i - 2);
                os << "  reply \"" << Text(vector<uint8_t>(r.begin() + i + 2, r.begin() + i + 2 + nrep))
                   << "\"";
            }

        } else if(record_type_ == 0xFA) {

            // Text, all NUL when empty.
            if(all_of(r.begin(), r.end(), [](uint8_t c) { return c == 0; }))
                os << "(empty)";
            else
                os << "text \"" << Text(r) << "\"";

        } else {

            os << "hex";
            for(uint8_t c : r)
                os << format(" %02X") % (int) c;
        }

        return os.str();
    }

    /** Read a big-endian u16 from a record. */
    static uint16_t U16(const vector<uint8_t>& r, size_t i) {
        return (uint16_t) (r[i] << 8 | r[i + 1]);
    }

    /** Read a big-endian u32 from a record. */
    static uint32_t U32(const vector<uint8_t>& r, size_t i) {
        return (uint32_t) r[i] << 24 | (uint32_t) r[i + 1] << 16 | (uint32_t) r[i + 2] << 8 | r[i + 3];
    }

    /** Format a record time: UNIX seconds as an ISO 8601 time, or values below 2000-01-01 as
     *  seconds since the controller started ('+402s'). */
    static string FormatTime(uint32_t t) {
        if(t < 946684800)
            return (format("+%us") % t).str();
        return util::date::DateUtils::GenerateIso8601String(boost::posix_time::from_time_t(t));
    }

    /** Format record text on one line: CR and LF as \r and \n, other non-printable bytes as \xNN,
     *  trailing NULs dropped. */
    static string Text(const vector<uint8_t>& r) {
        size_t n = r.size();
        while(n && r[n - 1] == 0)
            --n;
        ostringstream os;
        for(size_t i = 0; i < n; ++i) {
            const uint8_t c = r[i];
            if(c == '\r')
                os << "\\r";
            else if(c == '\n')
                os << "\\n";
            else if(c == '"' || c == '\\')
                os << '\\' << (char) c;
            else if(c >= 0x20 && c < 0x7F)
                os << (char) c;
            else
                os << format("\\x%02X") % (int) c;
        }
        return os.str();
    }

    /** The record type (byte 3). */
    uint8_t record_type_ = 0;

    /** The record version (byte 4). */
    uint8_t record_version_ = 0;

    /** The record bytes (from byte 5, without the terminator). */
    vector<uint8_t> record_;
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XEXTENDEDRECORD_H_ */
