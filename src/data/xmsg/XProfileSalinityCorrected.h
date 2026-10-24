/** The @ref util::xmsg::XProfileSalinityCorrected class header file.
 *
 */

#ifndef XPROFILESALINITYCORRECTED_H_
#define XPROFILESALINITYCORRECTED_H_

/* Includes */

#include "XProfileSalinity.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Dynamically corrected profile salinity (X-message 0xDC).
 *
 *  Floats with dynamic CTD correction enabled (mission parameter PPE = 2) send a second,
 *  corrected salinity channel alongside the binned profile salinity messages (0x30-0x3F). The
 *  format was inferred from production packets: a 0xDC message is a curvature-packed salinity
 *  message with four extra bytes after the size field.
 *
 *  | Byte | Content                                                        |
 *  |------|----------------------------------------------------------------|
 *  | 0    | ID 0xDC                                                        |
 *  | 1-2  | Packing format (high nibble) and message size (low 12 bits)    |
 *  | 3    | 0x00                                                           |
 *  | 4    | Message (part) index                                           |
 *  | 5    | 0x01 (channel; the only value seen)                            |
 *  | 6    | 0x00                                                           |
 *  | 7-   | Bytes 3 onward of the equivalent salinity message              |
 *
 *  The values use the salinity gain and offset, and the parts split the profile like (but not
 *  necessarily at the same points as) the salinity messages.
 */
class XProfileSalinityCorrected: public XProfileSalinity {

  public:

    /** Decode a corrected salinity message.
     *
     *  @param[in]  dive    The dive number.
     *  @param[in]  whoi    The WHOI instrument serial number.
     *  @param[in]  data    The 0xDC message.
     *
     *  @throw      util::log::MessageException if the message is malformed or uses an
     *              unsupported channel.
     */
    XProfileSalinityCorrected(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
            XProfileSalinity(dive, whoi, ToSalinityMessage(data)) {
      /* Do nothing. */
    }

    /** Convert a 0xDC message to the equivalent profile salinity (0x30-0x3F) message.
     *
     *  @param[in]  data    The 0xDC message.
     *
     *  @throw      util::log::MessageException if the message is malformed or uses an
     *              unsupported channel.
     *
     *  @return     The salinity message: ID 0x30 + part index, the size reduced by four, and the
     *              prefix bytes 3-6 removed.
     */
    static vector<uint8_t> ToSalinityMessage(const vector<uint8_t>& data) {

      vector<uint8_t> out;

      try {

        if(data.size() < 8)
          EXC_THROW("Corrected salinity message too short!");

        if(data[0] != XMSG_TYPE_ID_SALN_CORR)
          EXC_THROW("Invalid X-Message unique identifier!");

        const unsigned int size = 0x0FFF & u16(data, 1);
        if(size < 8 || size > data.size())
          EXC_THROW("Invalid corrected salinity message size: " + lexical_cast<string>(size) + "!");

        if(data[5] != 0x01)
          EXC_THROW("Unsupported corrected salinity channel: "
                    + lexical_cast<string>((int) data[5]) + "!");

        if(data[4] > 0x0F)
          EXC_THROW("Corrected salinity message index out of range: "
                    + lexical_cast<string>((int) data[4]) + "!");

        if(data[3] || data[6])
          lwrn() << "Unexpected corrected salinity prefix bytes: " << (int) data[3] << ", "
                 << (int) data[6];

        const unsigned int out_size = size - 4;

        out.reserve(out_size);
        out.push_back((uint8_t) (XMSG_TYPE_ID_SALN0 | data[4]));
        out.push_back((uint8_t) ((data[1] & 0xF0) | ((out_size >> 8) & 0x0F)));
        out.push_back((uint8_t) (out_size & 0xFF));
        out.insert(out.end(), data.begin() + 7, data.begin() + size);

      }CATCH_BLOCK("Failed to convert corrected salinity message!")

      return out;
    }
};

} /* namespace xmsg */
} /* namespace util */

#endif /* XPROFILESALINITYCORRECTED_H_ */
