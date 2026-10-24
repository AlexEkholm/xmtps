/** The @ref util::xmsg::XDriftTime class header file.
 *
 */

#ifndef XDRIFTTIME_H_
#define XDRIFTTIME_H_

/* Includes */

#include "AbstractXSensorData.h"

/* Definitions */

namespace util {
namespace xmsg {

/* Class Definitions */

/** Drift sample times (X-messages 0x78-0x7F).
 *
 *  The format was inferred from production packets. It is the same curvature-packed layout as
 *  the drift pressure, temperature and salinity messages (0x98, 0xA8, 0xB8), with one value per
 *  drift sample: the sample time in seconds since the start of the drift state (the Argo data
 *  message's drift state start time). Index 8 includes the first value.
 */
class XDriftTime: public AbstractXSensorData {

  public:

    /* Member Functions */

    /** Decode a drift sample time message.
     *
     *  @param[in]  dive    The dive number.
     *  @param[in]  whoi    The WHOI instrument serial number.
     *  @param[in]  data    The message.
     */
    XDriftTime(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
            AbstractXSensorData(dive, whoi, data) {
        DecodeXMsgData(data);   // Decode message.
    }

    /** The destructor.
     *
     */
    ~XDriftTime() {
        /* Do nothing. */
    }

    /** Decode the message.
     *
     *  @param[in]  data    The message.
     *
     *  @throw      util::log::MessageException if the message is invalid.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {
        try {
            const uint8_t* d = CheckXMsgData(data, (int) -1, (int) 5);

            if((d[0] & 0xF8) != XMSG_TYPE_ID_TIME_DRIFT8)
                EXC_THROW("Invalid X-Message unique identifier!");

            UnpackSensorData(data);

        }CATCH_BLOCK("Failed to decode drift time X-Message!")
    }
};

} /* namespace xmsg */
} /* namespace util */

#endif /* XDRIFTTIME_H_ */
