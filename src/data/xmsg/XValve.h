/** The @ref util::xmsg::XValve class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Mar 22, 2016 9:35:12 AM
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

#ifndef XVALVE_H_
#define XVALVE_H_

/* Includes: */

#include "AbstractXComposite.h"
#include "XMessageIndexable.h"

/* Definitions: */

namespace util {
namespace xmsg {

/* Types: */

/** The valve data message class.
 *
 *  An instance of this class represents a decoded valve data X-message, composed of the valve
 *  operation samples (see @ref XValveCounts) reported for a dive.
 */
class XValve:
        public AbstractXComposite<XValveCounts>,
        public XMessageIndexable {

public:

    /* Public Member Functions: */

    /** Parameterized constructor.
     *
     *  @param[in]  dive    The dive or cycle number of this message.
     *  @param[in]  whoi    The unique WHOI instrument identifier.
     *  @param[in]  data    The binary X-message data to decode.
     *
     *  @throw      util::log::MessageException if the message cannot be decoded.
     */
    XValve(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XMessageIndexable(dive, whoi, data) {

        // Decode the message data.
        DecodeXMsgData(data);


    }

    ~XValve() {

        /* Do nothing. */

    }

    /** Decode a valve data X-message (type 0xEB).
     *
     *  The low nibble of byte 3 is the message index. From byte 4, each valve operation is a
     *  7-byte sample: a phase code (high nibble of the first byte), a 20-bit pressure count (low
     *  nibble plus the next two bytes), the valve duration in seconds (2 bytes) and the case
     *  vacuum after the operation (2 bytes). The case vacuum before the operation is not reported
     *  and is set to @c XMSG_BAD_U16. Each sample is stored as an @ref XValveCounts element.
     *
     *  @param[in]  data    The binary X-message data to decode.
     *
     *  @throw      util::log::MessageException if the message is shorter than its size field or
     *              the terminator is missing.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {

            const uint8_t* d = CheckXMsgData(data, 0xEB, 4, false);

            //  TODO Get the message packing format.

            // Check message size.
            if(xmsg_size_bytes() > data.size()) {
                ostringstream os;
                os << "Invalid XMessage size: message header specifies "
                   << xmsg_size_bytes()
                   << " bytes, but only "
                   << data.size()
                   << " bytes were received.";
                BOOST_THROW_EXCEPTION(log::MessageException(os));
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

            // Get the message index.
            set_xmsg_index(d[3] & 0x0F);

            { // Iterate over the sensor data.

                int32_t presCounts;
                int16_t valveSecs;
                uint16_t vac0 = XMSG_BAD_U16;
                uint16_t vac1;
                XPhaseCode_t phaseCode;
                for(vector<uint8_t>::const_iterator it = data.begin() + 4; it < data.end() - 7;) {
                    phaseCode = (XPhaseCode_t) (*it >> 4);
                    presCounts = (*it++ & 0x0F) << 16; presCounts |= *it++ << 8; presCounts |= *it++;
                    valveSecs = *it++ << 8; valveSecs |= *it++;
                    vac1 = *it++ << 8; vac1 |= *it++;
                    AppendElement(XValveCounts(phaseCode, presCounts, valveSecs, vac0, vac1));
                }
            }
        }CATCH_BLOCK("Failed to decode pump data X-Message!")
    }

    /** The the listing file comment string.
     *
     *  @return         The listing file comment string.
     */
    static const string GenerateListingComment() {
        ostringstream os;

        try {
            const format fmt("#   %5s %4s %3s "
                    "%4s "
                    "%10s "
                    "%10s "
                    "%10s %10s\n"
                    "#   %5s %4s %3s "
                    "%4s  "
                    "%10s "
                    "%10s "
                    "%10s %10s");
            os << format(fmt) % "SN" % "dive" % "idx" //
                    % "nSam" //
                    % "pres" //
                    % "valveTime" //
                    % "vac0" % "vac1" //
                    % "" % "" % "" //
                    % "" //
                    % "(dBar)" //
                    % "(sec)" //
                    % "(inHg)" % "(inHg)";
        }CATCH_BLOCK("Failed to generate listing comment for pump data object!")

        return os.str();
    }

    const string GenerateListingString(uint16_t pGain, uint16_t pOff, float pres_correction) const {
        ostringstream os;
        size_t sam = 0;

        try {
            {
                const format fmt("VLV %5d %4d %3d %4d");
                os << format(fmt) % id_instrument() % id_dive() % (int) xmsg_index()
                        % num_elements();
            }
            {
                const format fmt("\nvlv %5d %4d %3d %4d ");
                vector<XValveCounts>::const_iterator it;
                for(it = elements().begin(); it < elements().end(); it++)
                    os << format(fmt) % id_instrument() % id_dive() % (int) xmsg_index() % sam++
                       << it->GenerateListingString(pGain, pOff, pres_correction);
            }
        }CATCH_BLOCK("Failed to generate listing string from pump data object!")

        return os.str();
    }

};

/* Functions: */

} /* namespace xmsg */
} /* namespace util */

#endif /* XVALVE_H_ */
