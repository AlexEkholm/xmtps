/** The @ref util::xmsg::XPump class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 11:18:42 AM
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

#ifndef XPUMP_H_
#define XPUMP_H_

/* Includes */

// local
#include "AbstractXComposite.h"
#include "XMessageIndexable.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Type Definitions */

/** Enumeration type definition of pump data X-Message packing formats. */
typedef enum XPumpPackingFormat {

    /** X-Message pump data packing format 0: The legacy packing format identifier. */
    XPUMP_PACK_LEGACY = 0,

    /** X-Message pump data packing format 1: The standard packing format identifier. */
    XPUMP_PACK_STANDARD = 1,

    /** X-Message pump data packing format 2: The (tentative) packing format identifier. */
    XPUMP_PACK_STANDARD2 = 2,

    /** X-Message pump data packing format 99: The unknown/unset/invalid packing format identifier. */
    XPUMP_PACK_UNKNOWN = 99
} XPumpPackingFormat_t;

/* Class Definitions */

/** Class representing a pump data X-Message.
 *
 */
class XPump:
        public AbstractXComposite<XPumpCounts>,
        public XMessageIndexable {

private:

    /* Private Data Members: */

    /** The packing format of this pump data message. */
    XPumpPackingFormat_t pump_packing_format_;

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
    XPump(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XMessageIndexable(dive, whoi, data),
                    pump_packing_format_(XPUMP_PACK_UNKNOWN) {

        // Decode the message data.
        DecodeXMsgData(data);
    }

    /** The destructor.
     *
     */
    ~XPump() {
        /* Do nothing. */
    }

    /** Decode the pump data message from the binary data to decode.
     *
     *  @param[in]      data        The binary data to decode.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {
        try {
            const uint8_t* d = CheckXMsgData(data, -1, 3, false);

            set_xmsg_index(0x0F & d[0]);       // Get the message index.

            {    // Get the message packing format.
                int f;
                if((f = (d[1] & 0xF0) >> 4) == 2) {
                    pump_packing_format_ = XPUMP_PACK_STANDARD2;
                    set_xmsg_size_bytes(((d[1] & 0x0F) << 8) | d[2]);
                } else if(f == 1) {
                    pump_packing_format_ = XPUMP_PACK_STANDARD;
                    set_xmsg_size_bytes(((d[1] & 0x0F) << 8) | d[2]);
                } else if(f == 0) {
                    pump_packing_format_ = XPUMP_PACK_LEGACY;
                    set_xmsg_size_bytes((d[1] << 8) | d[2]);
                } else {
                    ostringstream os;
                    os << "Invalid pump packing format specifier! Expected 0 (legacy) or 1 "
                       "(standard), but received "
                       << f << "!";
                    BOOST_THROW_EXCEPTION(log::MessageException(os));
                }
            }

            // Check message size.
            if(xmsg_size_bytes() > data.size()) {
                ostringstream os;
                os << "Invalid XMessage size: message specifies at least "
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

            {    // Iterate over the sensor data.
                int32_t presCounts;
                int16_t pumpSecs;
                uint16_t voltage;
                uint16_t current;
                uint16_t vac0 = XMSG_BAD_U16;
                uint16_t vac1;
                if(pump_packing_format_ == XPUMP_PACK_LEGACY)
                    for(vector<uint8_t>::const_iterator it = data.begin() + 3; it < data.end() - 10;
                            ) {
                        presCounts = ((*it++) << 8) | (*it++);
                        pumpSecs = ((*it++) << 8) | (*it++);
                        voltage = ((*it++) << 8) | (*it++);
                        current = ((*it++) << 8) | (*it++);
                        vac0 = *it++;
                        vac1 = *it++;
                        AppendElement(
                                XPumpCounts(
                                        XPHASE_CODE_UNKNOWN,
                                        presCounts,
                                        pumpSecs,
                                        voltage,
                                        current,
                                        vac0,
                                        vac1));
                    }
                else if(pump_packing_format_ == XPUMP_PACK_STANDARD)
                    for(vector<uint8_t>::const_iterator it = data.begin() + 3; it < data.end() - 11;
                            ) {
                        XPhaseCode_t phaseCode = (XPhaseCode_t) (*it >> 4);
                        presCounts = (((*it++ & 0x0F) << 16)) | ((*it++) << 8) | (*it++);
                        pumpSecs = ((*it++) << 8) | (*it++);
                        voltage = ((*it++) << 8) | (*it++);
                        current = ((*it++) << 8) | (*it++);
                        vac0 = *it++;
                        vac1 = *it++;
                        AppendElement(
                                XPumpCounts(
                                        phaseCode,
                                        presCounts,
                                        pumpSecs,
                                        voltage,
                                        current,
                                        vac0 * 10,
                                        vac1 * 10));
                    }
                else if(pump_packing_format_ == XPUMP_PACK_STANDARD2)
                    for(vector<uint8_t>::const_iterator it = data.begin() + 3; it < data.end() - 11;
                            ) {
                        XPhaseCode_t phaseCode = (XPhaseCode_t) (*it >> 4);
                        presCounts = (((*it++ & 0x0F) << 16)) | ((*it++) << 8) | (*it++);
                        pumpSecs = ((*it++) << 8) | (*it++);
                        voltage = ((*it++) << 8) | (*it++);
                        current = ((*it++) << 8) | (*it++);
                        vac1 = ((*it++) << 8) | (*it++);
                        AppendElement(
                                XPumpCounts(
                                        phaseCode,
                                        presCounts,
                                        pumpSecs,
                                        voltage,
                                        current,
                                        vac0,
                                        vac1));
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
            const format fmt("#   %5s %4s %3s %4s %10s %10s %10s %10s %10s %10s\n"
                    "#   %5s %4s %3s %4s  %10s %10s %10s %10s %10s %10s");
            os << format(fmt) % "SN" % "dive" % "idx" % "nSam" % "pres" % "pumpT" % "pumpV"
                    % "pumpI" % "vac0" % "vac1" % "" % "" % "" % "" % "(dBar)" % "(sec)" % "(V)"
                    % "(A)" % "(inHg)" % "(inHg)";
        }CATCH_BLOCK("Failed to generate listing comment for pump data object!")

        return os.str();
    }

    /** Generate the listing file entry string for this pump data message.
     *
     *  @param[in]  pg      The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po      The pressure sensor offset, in dBar.
     *  @param[in]  pc      The pressure correction, in dBar, to be subtracted computed pressure
     *                      value.
     *
     *  @throw      log::MessageException if an error occurs while generating the listing string.
     *
     *  @return     The listing file entry string for this pump data message.
     */
    const string GenerateListingString(uint16_t pg, uint16_t po, float pc) const {
        ostringstream os;
        size_t sam = 0;

        try {
            {
                const format fmt("PMP %5d %4d %3d %4d");
                os << format(fmt) % id_instrument() % id_dive() % (int) xmsg_index()
                        % num_elements();
            }
            {
                const format fmt("\npmp %5d %4d %3d %4d ");
                vector<XPumpCounts>::const_iterator it;
                for(it = elements().begin(); it < elements().end(); it++)
                    os << format(fmt) % id_instrument() % id_dive() % (int) xmsg_index() % sam++
                       << it->GenerateListingString(pg, po, pc);
            }
        }CATCH_BLOCK("Failed to generate listing string from pump data object!")

        return os.str();
    }

    /** Get the enumerated pump packing format identifier.
     *
     *  @return         The enumerated pump packing format identifier.
     */
    XPumpPackingFormat_t pump_packing_format() const {
        return pump_packing_format_;
    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XPUMP_H_ */
