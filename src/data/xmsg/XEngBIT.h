/*
 * XEngBIT.h
 *
 *  Created on: Apr 20, 2023
 *      Author: aekholm
 */

#ifndef DATA_XMSG_XENGBIT_H_
#define DATA_XMSG_XENGBIT_H_

/* Includes */

// local
#include "XAlamoBit.h"
#include "XAlamoBit2.h"

namespace util {
namespace xmsg {

class XEngBIT:
    public XMessage {

private:

    /* Data Members */

    /** A shared pointer to the ALALMO Built-in-Test (BIT) message.
     *
     *  @note           This member is only used for ALAMO-type instruments prior to firmware
     *                  version 10.x.
     */
    shared_ptr<XAlamoBit> alamo_bit_msg_;

    /** A shared pointer to the ALALMO Built-in-Test (BIT) message.
     *
     *  @note           This member is only used for ALAMO-type instruments starting with firmware
     *                  version 10.x.
     */
    shared_ptr<XAlamoBit2> alamo_bit2_msg_;

protected:

    /* Member Functions */

    /** Set the ALALMO Built-in-Test (BIT) v1 message
     *
     *  @param[in]  alamo_bit_msg   A shared pointer to the ALALMO Built-in-Test (BIT) message to
     *                              set.
     */
    void set_alamo_bit_msg(const shared_ptr<XAlamoBit>& alamo_bit_msg) {
        alamo_bit_msg_ = alamo_bit_msg;
    }

    /** Set the ALAMO Built-in-Test (BIT) v2 message.
     *
     *  @param[in]  alamo_bit2_msg  A shared pointer to the ALAMO Built-in-Test (BIT) v2 message to
     *                              set.
     */
    void set_alamo_bit2_msg(const shared_ptr<XAlamoBit2>& alamo_bit2_msg) {
        alamo_bit2_msg_ = alamo_bit2_msg;
    }

public:

    /* Member Functions */

    /** Parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XEngBIT(int16_t dive, uint16_t whoi, const vector<uint8_t> data) :
            XMessage(dive, whoi, data) {
      DecodeXMsgData(data); // Decode message.
    }

    /** Decode the BIT engineering X-Message from the binary data.
     *
     *  @param[in]      data            The binary message to decode.
     *
     *  @throw          util::log::MessageException if an error occurs while decoding.
     */
    void DecodeXMsgData(const vector<uint8_t> &data) {

      try {

        CheckXMsgData(data, -1, 13);
        if(data.at(3) == 6 || data.at(3) == 7 || data.at(3) == 8) {
          alamo_bit_msg_ = std::make_shared<XAlamoBit>(data);
        } else if(data.at(3) == 9 || data.at(3) == 10 || data.at(3) == 11) {
          alamo_bit2_msg_ = std::make_shared<XAlamoBit2>(data);
        } else {
          ostringstream os;
          os << "Unsupported BIT message format: " << (int)data.at(3) << "!";
          EXC_THROW(os.str());
        }

      } CATCH_BLOCK("Failed to decode BIT engineering X-Message!")
    }

    /** Return a listing entry string representation of the BIT engineering X-Message.
     *
     *  @param[in]  pg          The pressure sensor sensitivity (gain) scale factor, in LSB/dBar.
     *  @param[in]  po          The pressure sensor offset, in dBar.
     *  @param[in]  sg          The salinity sensor sensitivity  (gain) scale factor, in LSB/PSU.
     *  @param[in]  so          The salinity sensor offset, in PSU.
     *  @param[in]  tg          The temperature sensor sensitivity  (gain) scale factor, in LSB/degC.
     *  @param[in]  to          The temperature sensor offset, in degC.
     *
     *  @throw      util::log::MessageException if an error occurs while generating the listing
     *              entry string representation.
     *
     *  @return     A listing entry string representation of the BIT engineering X-Message.
     *
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

        if(alamo_bit_msg_) {

          os << alamo_bit_msg_->GenerateListingString(
              id_instrument(),
              id_dive(),
              pg,
              po,
              sg,
              so,
              tg,
              to);

        } else if(alamo_bit2_msg_) {

          os << alamo_bit2_msg_->GenerateListingString(
              id_instrument(),
              id_dive());

        }

      }CATCH_BLOCK("Failed to generate listing entry string representation of BIT message!")

      return os.str();
    }



};

} /* namespace xmsg */
} /* namespace util */

#endif /* DATA_XMSG_XENGBIT_H_ */
