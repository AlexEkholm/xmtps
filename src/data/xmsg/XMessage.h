/** The @ref util::xmsg::XMessage class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jun 9, 2014 2:43:58 PM
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
#ifndef XMSG_H_
#define XMSG_H_

/* Includes */

// local
#include "common/XCommon.h"

/* Definitions */

namespace util {
  namespace xmsg {

    /* Type Definitions */

    /** Enumeration of the unique X message identifiers.
     *
     */
    typedef enum XMsgTypeId {
      XMSG_TYPE_ID_GPS_DIAGNOSTIC = 0x00,    //!< GPS0
      XMSG_TYPE_ID_GPS_STARTOFDIVE = 0x01,    //!< GPS1
      XMSG_TYPE_ID_GPS_ENDOFDIVE = 0x02,    //!< GPS2
      XMSG_TYPE_ID_GPS_ABORT = 0x03,    //!< GPS3
      XMSG_TYPE_ID_GPS_BITEST = 0x05,    //!< GPS5
//     = 0x06,
//     = 0x07,
//     = 0x08,
//     = 0x09,
//     = 0x0A,
//     = 0x0B,
//     = 0x0C,
//     = 0x0D,
//     = 0x0E,
//     = 0x0F,
      XMSG_TYPE_ID_PRES0 = 0x10,    //!< PRES0
      XMSG_TYPE_ID_PRES1 = 0x11,    //!< PRES1
      XMSG_TYPE_ID_PRES2 = 0x12,    //!< PRES2
      XMSG_TYPE_ID_PRES3 = 0x13,    //!< PRES3
      XMSG_TYPE_ID_PRES4 = 0x14,    //!< PRES4
      XMSG_TYPE_ID_PRES5 = 0x15,    //!< PRES5
      XMSG_TYPE_ID_PRES6 = 0x16,    //!< PRES6
      XMSG_TYPE_ID_PRES7 = 0x17,    //!< PRES7
      XMSG_TYPE_ID_PRES8 = 0x18,    //!< PRES8
      XMSG_TYPE_ID_PRES9 = 0x19,    //!< PRES9
      XMSG_TYPE_ID_PRESA = 0x1A,    //!< PRESA
      XMSG_TYPE_ID_PRESB = 0x1B,    //!< PRESB
      XMSG_TYPE_ID_PRESC = 0x1C,    //!< PRESC
      XMSG_TYPE_ID_PRESD = 0x1D,    //!< PRESD
      XMSG_TYPE_ID_PRESE = 0x1E,    //!< PRESE
      XMSG_TYPE_ID_PRESF = 0x1F,    //!< PRESF
      XMSG_TYPE_ID_TEMP0 = 0x20,    //!< TEMP0
      XMSG_TYPE_ID_TEMP1 = 0x21,    //!< TEMP1
      XMSG_TYPE_ID_TEMP2 = 0x22,    //!< TEMP2
      XMSG_TYPE_ID_TEMP3 = 0x23,    //!< TEMP3
      XMSG_TYPE_ID_TEMP4 = 0x24,    //!< TEMP4
      XMSG_TYPE_ID_TEMP5 = 0x25,    //!< TEMP5
      XMSG_TYPE_ID_TEMP6 = 0x26,    //!< TEMP6
      XMSG_TYPE_ID_TEMP7 = 0x27,    //!< TEMP7
      XMSG_TYPE_ID_TEMP8 = 0x28,    //!< TEMP8
      XMSG_TYPE_ID_TEMP9 = 0x29,    //!< TEMP9
      XMSG_TYPE_ID_TEMPA = 0x2A,    //!< TEMPA
      XMSG_TYPE_ID_TEMPB = 0x2B,    //!< TEMPB
      XMSG_TYPE_ID_TEMPC = 0x2C,    //!< TEMPC
      XMSG_TYPE_ID_TEMPD = 0x2D,    //!< TEMPD
      XMSG_TYPE_ID_TEMPE = 0x2E,    //!< TEMPE
      XMSG_TYPE_ID_TEMPF = 0x2F,    //!< TEMPF
      XMSG_TYPE_ID_SALN0 = 0x30,    //!< SALN0
      XMSG_TYPE_ID_SALN1 = 0x31,    //!< SALN1
      XMSG_TYPE_ID_SALN2 = 0x32,    //!< SALN2
      XMSG_TYPE_ID_SALN3 = 0x33,    //!< SALN3
      XMSG_TYPE_ID_SALN4 = 0x34,    //!< SALN4
      XMSG_TYPE_ID_SALN5 = 0x35,    //!< SALN5
      XMSG_TYPE_ID_SALN6 = 0x36,    //!< SALN6
      XMSG_TYPE_ID_SALN7 = 0x37,    //!< SALN7
      XMSG_TYPE_ID_SALN8 = 0x38,    //!< SALN8
      XMSG_TYPE_ID_SALN9 = 0x39,    //!< SALN9
      XMSG_TYPE_ID_SALNA = 0x3A,    //!< SALNA
      XMSG_TYPE_ID_SALNB = 0x3B,    //!< SALNB
      XMSG_TYPE_ID_SALNC = 0x3C,    //!< SALNC
      XMSG_TYPE_ID_SALND = 0x3D,    //!< SALND
      XMSG_TYPE_ID_SALNE = 0x3E,    //!< SALNE
      XMSG_TYPE_ID_SALNF = 0x3F,    //!< SALNF
      XMSG_TYPE_ID_FALL0 = 0x40,    //!< FALL0
      XMSG_TYPE_ID_FALL1 = 0x41,    //!< FALL1
      XMSG_TYPE_ID_FALL2 = 0x42,    //!< FALL2
      XMSG_TYPE_ID_FALL3 = 0x43,    //!< FALL3
      XMSG_TYPE_ID_FALL4 = 0x44,    //!< FALL4
      XMSG_TYPE_ID_FALL5 = 0x45,    //!< FALL5
      XMSG_TYPE_ID_FALL6 = 0x46,    //!< FALL6
      XMSG_TYPE_ID_FALL7 = 0x47,    //!< FALL7
      XMSG_TYPE_ID_FALL8 = 0x48,    //!< FALL8
      XMSG_TYPE_ID_FALL9 = 0x49,    //!< FALL9
      XMSG_TYPE_ID_FALLA = 0x4A,    //!< FALLA
      XMSG_TYPE_ID_FALLB = 0x4B,    //!< FALLB
      XMSG_TYPE_ID_FALLC = 0x4C,    //!< FALLC
      XMSG_TYPE_ID_FALLD = 0x4D,    //!< FALLD
      XMSG_TYPE_ID_FALLE = 0x4E,    //!< FALLE
      XMSG_TYPE_ID_FALLF = 0x4F,    //!< FALLF
      XMSG_TYPE_ID_RISE0 = 0x50,    //!< XMSG_TYPE_ID_RISE0
      XMSG_TYPE_ID_RISE1 = 0x51,    //!< XMSG_TYPE_ID_RISE1
      XMSG_TYPE_ID_RISE2 = 0x52,    //!< XMSG_TYPE_ID_RISE2
      XMSG_TYPE_ID_RISE3 = 0x53,    //!< XMSG_TYPE_ID_RISE3
      XMSG_TYPE_ID_RISE4 = 0x54,    //!< XMSG_TYPE_ID_RISE4
      XMSG_TYPE_ID_RISE5 = 0x55,    //!< XMSG_TYPE_ID_RISE5
      XMSG_TYPE_ID_RISE6 = 0x56,    //!< XMSG_TYPE_ID_RISE6
      XMSG_TYPE_ID_RISE7 = 0x57,    //!< XMSG_TYPE_ID_RISE7
      XMSG_TYPE_ID_RISE8 = 0x58,    //!< XMSG_TYPE_ID_RISE8
      XMSG_TYPE_ID_RISE9 = 0x59,    //!< XMSG_TYPE_ID_RISE9
      XMSG_TYPE_ID_RISEA = 0x5A,    //!< XMSG_TYPE_ID_RISEA
      XMSG_TYPE_ID_RISEB = 0x5B,    //!< XMSG_TYPE_ID_RISEB
      XMSG_TYPE_ID_RISEC = 0x5C,    //!< XMSG_TYPE_ID_RISEC
      XMSG_TYPE_ID_RISED = 0x5D,    //!< XMSG_TYPE_ID_RISED
      XMSG_TYPE_ID_RISEE = 0x5E,    //!< XMSG_TYPE_ID_RISEE
      XMSG_TYPE_ID_RISEF = 0x5F,    //!< XMSG_TYPE_ID_RISEF
      XMSG_TYPE_ID_PUMP0 = 0x60,    //!< XMSG_TYPE_ID_PUMP0
      XMSG_TYPE_ID_PUMP1 = 0x61,    //!< XMSG_TYPE_ID_PUMP1
      XMSG_TYPE_ID_PUMP2 = 0x62,    //!< XMSG_TYPE_ID_PUMP2
      XMSG_TYPE_ID_PUMP3 = 0x63,    //!< XMSG_TYPE_ID_PUMP3
      XMSG_TYPE_ID_PUMP4 = 0x64,    //!< XMSG_TYPE_ID_PUMP4
      XMSG_TYPE_ID_PUMP5 = 0x65,    //!< XMSG_TYPE_ID_PUMP5
      XMSG_TYPE_ID_PUMP6 = 0x66,    //!< XMSG_TYPE_ID_PUMP6
      XMSG_TYPE_ID_PUMP7 = 0x67,    //!< XMSG_TYPE_ID_PUMP7
      XMSG_TYPE_ID_PUMP8 = 0x68,    //!< XMSG_TYPE_ID_PUMP8
      XMSG_TYPE_ID_PUMP9 = 0x69,    //!< XMSG_TYPE_ID_PUMP9
      XMSG_TYPE_ID_PUMPA = 0x6A,    //!< XMSG_TYPE_ID_PUMPA
      XMSG_TYPE_ID_PUMPB = 0x6B,    //!< XMSG_TYPE_ID_PUMPB
      XMSG_TYPE_ID_PUMPC = 0x6C,    //!< XMSG_TYPE_ID_PUMPC
      XMSG_TYPE_ID_PUMPD = 0x6D,    //!< XMSG_TYPE_ID_PUMPD
      XMSG_TYPE_ID_PUMPE = 0x6E,    //!< XMSG_TYPE_ID_PUMPE
      XMSG_TYPE_ID_PUMPF = 0x6F,    //!< XMSG_TYPE_ID_PUMPF
      XMSG_TYPE_ID_PRES_SURF0 = 0x80,    //!< XMSG_TYPE_ID_PRES_SURF0
      XMSG_TYPE_ID_PRES_SURF1 = 0x81,    //!< XMSG_TYPE_ID_PRES_SURF1
      XMSG_TYPE_ID_PRES_SURF2 = 0x82,    //!< XMSG_TYPE_ID_PRES_SURF2
      XMSG_TYPE_ID_PRES_SURF3 = 0x83,    //!< XMSG_TYPE_ID_PRES_SURF3
      XMSG_TYPE_ID_PRES_SURF4 = 0x84,    //!< XMSG_TYPE_ID_PRES_SURF4
      XMSG_TYPE_ID_PRES_SURF5 = 0x85,    //!< XMSG_TYPE_ID_PRES_SURF5
      XMSG_TYPE_ID_PRES_SURF6 = 0x86,    //!< XMSG_TYPE_ID_PRES_SURF6
      XMSG_TYPE_ID_PRES_SURF7 = 0x87,    //!< XMSG_TYPE_ID_PRES_SURF7
      XMSG_TYPE_ID_PRES_SURF8 = 0x88,    //!< XMSG_TYPE_ID_PRES_SURF8
      XMSG_TYPE_ID_PRES_SURF9 = 0x89,    //!< XMSG_TYPE_ID_PRES_SURF9
      XMSG_TYPE_ID_PRES_SURFA = 0x8A,    //!< XMSG_TYPE_ID_PRES_SURFA
      XMSG_TYPE_ID_PRES_SURFB = 0x8B,    //!< XMSG_TYPE_ID_PRES_SURFB
      XMSG_TYPE_ID_PRES_SURFC = 0x8C,    //!< XMSG_TYPE_ID_PRES_SURFC
      XMSG_TYPE_ID_PRES_SURFD = 0x8D,    //!< XMSG_TYPE_ID_PRES_SURFD
      XMSG_TYPE_ID_PRES_SURFE = 0x8E,    //!< XMSG_TYPE_ID_PRES_SURFE
      XMSG_TYPE_ID_PRES_SURFF = 0x8F,    //!< XMSG_TYPE_ID_PRES_SURFF
      XMSG_TYPE_ID_PRES_HI_RES0 = 0x90,    //!< XMSG_TYPE_ID_PRES_HI_RES0
      XMSG_TYPE_ID_PRES_HI_RES1 = 0x91,    //!< XMSG_TYPE_ID_PRES_HI_RES1
      XMSG_TYPE_ID_PRES_HI_RES2 = 0x92,    //!< XMSG_TYPE_ID_PRES_HI_RES2
      XMSG_TYPE_ID_PRES_HI_RES3 = 0x93,    //!< XMSG_TYPE_ID_PRES_HI_RES3
      XMSG_TYPE_ID_PRES_HI_RES4 = 0x94,    //!< XMSG_TYPE_ID_PRES_HI_RES4
      XMSG_TYPE_ID_PRES_HI_RES5 = 0x95,    //!< XMSG_TYPE_ID_PRES_HI_RES5
      XMSG_TYPE_ID_PRES_HI_RES6 = 0x96,    //!< XMSG_TYPE_ID_PRES_HI_RES6
      XMSG_TYPE_ID_PRES_HI_RES7 = 0x97,    //!< XMSG_TYPE_ID_PRES_HI_RES7
      XMSG_TYPE_ID_PRES_DRIFT8 = 0x98,    //!< XMSG_TYPE_ID_PRES_DRIFT8
      XMSG_TYPE_ID_PRES_DRIFT9 = 0x99,    //!< XMSG_TYPE_ID_PRES_DRIFT9
      XMSG_TYPE_ID_PRES_DRIFTA = 0x9A,    //!< XMSG_TYPE_ID_PRES_DRIFTA
      XMSG_TYPE_ID_PRES_DRIFTB = 0x9B,    //!< XMSG_TYPE_ID_PRES_DRIFTB
      XMSG_TYPE_ID_PRES_DRIFTC = 0x9C,    //!< XMSG_TYPE_ID_PRES_DRIFTC
      XMSG_TYPE_ID_PRES_DRIFTD = 0x9D,    //!< XMSG_TYPE_ID_PRES_DRIFTD
      XMSG_TYPE_ID_PRES_DRIFTE = 0x9E,    //!< XMSG_TYPE_ID_PRES_DRIFTE
      XMSG_TYPE_ID_PRES_DRIFTF = 0x9F,    //!< XMSG_TYPE_ID_PRES_DRIFTF
      XMSG_TYPE_ID_TEMP_HI_RES0 = 0xA0,    //!< XMSG_TYPE_ID_TEMP_HI_RES0
      XMSG_TYPE_ID_TEMP_HI_RES1 = 0xA1,    //!< XMSG_TYPE_ID_TEMP_HI_RES1
      XMSG_TYPE_ID_TEMP_HI_RES2 = 0xA2,    //!< XMSG_TYPE_ID_TEMP_HI_RES2
      XMSG_TYPE_ID_TEMP_HI_RES3 = 0xA3,    //!< XMSG_TYPE_ID_TEMP_HI_RES3
      XMSG_TYPE_ID_TEMP_HI_RES4 = 0xA4,    //!< XMSG_TYPE_ID_TEMP_HI_RES4
      XMSG_TYPE_ID_TEMP_HI_RES5 = 0xA5,    //!< XMSG_TYPE_ID_TEMP_HI_RES5
      XMSG_TYPE_ID_TEMP_HI_RES6 = 0xA6,    //!< XMSG_TYPE_ID_TEMP_HI_RES6
      XMSG_TYPE_ID_TEMP_HI_RES7 = 0xA7,    //!< XMSG_TYPE_ID_TEMP_HI_RES7
      XMSG_TYPE_ID_TEMP_DRIFT8 = 0xA8,    //!< XMSG_TYPE_ID_TEMP_DRIFT8
      XMSG_TYPE_ID_TEMP_DRIFT9 = 0xA9,    //!< XMSG_TYPE_ID_TEMP_DRIFT9
      XMSG_TYPE_ID_TEMP_DRIFTA = 0xAA,    //!< XMSG_TYPE_ID_TEMP_DRIFTA
      XMSG_TYPE_ID_TEMP_DRIFTB = 0xAB,    //!< XMSG_TYPE_ID_TEMP_DRIFTB
      XMSG_TYPE_ID_TEMP_DRIFTC = 0xAC,    //!< XMSG_TYPE_ID_TEMP_DRIFTC
      XMSG_TYPE_ID_TEMP_DRIFTD = 0xAD,    //!< XMSG_TYPE_ID_TEMP_DRIFTD
      XMSG_TYPE_ID_TEMP_DRIFTE = 0xAE,    //!< XMSG_TYPE_ID_TEMP_DRIFTE
      XMSG_TYPE_ID_TEMP_DRIFTF = 0xAF,    //!< XMSG_TYPE_ID_TEMP_DRIFTF
      XMSG_TYPE_ID_SALN_HI_RES0 = 0xB0,    //!< XMSG_TYPE_ID_SALN_HI_RES0
      XMSG_TYPE_ID_SALN_HI_RES1 = 0xB1,    //!< XMSG_TYPE_ID_SALN_HI_RES1
      XMSG_TYPE_ID_SALN_HI_RES2 = 0xB2,    //!< XMSG_TYPE_ID_SALN_HI_RES2
      XMSG_TYPE_ID_SALN_HI_RES3 = 0xB3,    //!< XMSG_TYPE_ID_SALN_HI_RES3
      XMSG_TYPE_ID_SALN_HI_RES4 = 0xB4,    //!< XMSG_TYPE_ID_SALN_HI_RES4
      XMSG_TYPE_ID_SALN_HI_RES5 = 0xB5,    //!< XMSG_TYPE_ID_SALN_HI_RES5
      XMSG_TYPE_ID_SALN_HI_RES6 = 0xB6,    //!< XMSG_TYPE_ID_SALN_HI_RES6
      XMSG_TYPE_ID_SALN_HI_RES7 = 0xB7,    //!< XMSG_TYPE_ID_SALN_HI_RES7
      XMSG_TYPE_ID_SALN_DRIFT8 = 0xB8,    //!< XMSG_TYPE_ID_SALN_DRIFT8
      XMSG_TYPE_ID_SALN_DRIFT9 = 0xB9,    //!< XMSG_TYPE_ID_SALN_DRIFT9
      XMSG_TYPE_ID_SALN_DRIFTA = 0xBA,    //!< XMSG_TYPE_ID_SALN_DRIFTA
      XMSG_TYPE_ID_SALN_DRIFTB = 0xBB,    //!< XMSG_TYPE_ID_SALN_DRIFTB
      XMSG_TYPE_ID_SALN_DRIFTC = 0xBC,    //!< XMSG_TYPE_ID_SALN_DRIFTC
      XMSG_TYPE_ID_SALN_DRIFTD = 0xBD,    //!< XMSG_TYPE_ID_SALN_DRIFTD
      XMSG_TYPE_ID_SALN_DRIFTE = 0xBE,    //!< XMSG_TYPE_ID_SALN_DRIFTE
      XMSG_TYPE_ID_SALN_DRIFTF = 0xBF,    //!< XMSG_TYPE_ID_SALN_DRIFTF
//       = 0xC0,    /
      XMSG_TYPE_ID_ALAMO_WAVE_C1 = 0xC1,    //!< ALAMO_WAVE
      XMSG_TYPE_ID_AUX_SENSOR_DATA = 0xC2,    //!< ALAMO_WAVE
      // = 0xC2
      XMSG_TYPE_ID_ALAMO_WAVE_C3 = 0xC3,    //!< ALAMO_WAVE
      // = 0xC4
      // = 0xC5
      // = 0xC6
      // = 0xC7
      // = 0xC8
      // = 0xC9
      XMSG_TYPE_ID_MIZ_ACOUSTIC = 0xCA, //!< MIZ_ACOUSTIC
      // = 0xCB
      // = 0xCC
      // = 0xCD
      // = 0xCE
      // = 0xCF
      XMSG_TYPE_ID_MISS_CONF0 = 0xD0,    //!< MISS_CONF0
      XMSG_TYPE_ID_MISS_CONF1 = 0xD1,    //!< MISS_CONF1
      XMSG_TYPE_ID_MISS_CONF2 = 0xD2,    //!< MISS_CONF2
      XMSG_TYPE_ID_MISS_CONF3 = 0xD3,    //!< MISS_CONF3
      XMSG_TYPE_ID_MISS_CONF4 = 0xD4,    //!< MISS_CONF4
      XMSG_TYPE_ID_MISS_CONF5 = 0xD5,    //!< MISS_CONF5
      XMSG_TYPE_ID_MISS_CONF6 = 0xD6,    //!< MISS_CONF6
      XMSG_TYPE_ID_MISS_CONF7 = 0xD7,    //!< MISS_CONF7
      // = 0xD8
      // = 0xD9
      // = 0xDA
      // = 0xDB
      // = 0xDC
      // = 0xDD
      // = 0xDE
      // = 0xDF
      XMSG_TYPE_ID_MISS_CMD_ECHO = 0xDE,    //!< MISS_CMD_ECHO
      // = 0xDF
      XMSG_TYPE_ID_ENG_DIAGNOSTIC = 0xE0,    //!< ENG_DIAGNOSTIC
//    ENG_ = 0xE1,
      XMSG_TYPE_ID_ENG_DIVE = 0xE2,      //!< ENG_DIVE
      XMSG_TYPE_ID_ENG_ABORT = 0xE3,      //!< ENG_MISS_START
//    ENG = 0xE4,
      XMSG_TYPE_ID_ENG_BIT_PASS = 0xE5,    //!< ENG_BIT_PASS
      XMSG_TYPE_ID_ENG_BIT_FAIL = 0xE6,    //!< ENG_BIT_FAIL
      XMSG_TYPE_ID_ENG_DESCENT = 0xE7,
      XMSG_TYPE_ID_ENG_PARK = 0xE8,
      XMSG_TYPE_ID_ENG_ASCENT = 0xE9,
      XMSG_TYPE_ID_ENG_SURFACE = 0xEA,
      XMSG_TYPE_ID_ENG_VALVE = 0xEB,
      XMSG_TYPE_ID_ENG_ICE = 0xEC,
//    ENG = 0xED,
//    ENG = 0xEE,
//    ENG = 0xEF,
      XMSG_TYPE_ID_ARGO_DATA = 0xF0,     //!< ARGO_DATA
      XMSG_TYPE_ID_TEST = 0xF1,          //!< TEST
      XMSG_TYPE_ID_UNKNOWN = 0xFF    //!< UNKNOWN
    } XMsgTypeId_t;

    /* Class Definition */

    /** Class representing a message following the X format specification.
     *
     */
    class XMessage {

      private:

        /* Private Data Members: */

        /** The dive or cycle number of this message. */
        int16_t id_dive_;

        /** The unique WHOI identifier of this instrument. */
        uint16_t id_instrument_;

        /** The X-message type identifier. */
        XMsgTypeId_t id_xmsg_type_;

        /** The size of the X message, in bytes. */
        uint16_t xmsg_size_bytes_;

      protected:

        /* Protected Member Functions: */

        /** The default constructor.
         *
         *  @param[in]  dive            The dive number of this X-Message object.
         *  @param[in]  whoi            The unique instrument identifier.
         *  @param[in]  type            The X-Message type identifier.
         *
         *  @exception  None. Never throws.
         */
        XMessage(
            int16_t dive = XMSG_DIVE_UNKNOWN,
            uint16_t whoi = XMSG_ID_UNKNOWN,
            XMsgTypeId_t type = XMSG_TYPE_ID_UNKNOWN) ATTR_NO_EXCEPTIONS :
                id_dive_(dive),
                id_instrument_(whoi),
                id_xmsg_type_(type),
                xmsg_size_bytes_(0) {
          /* Do nothing. */
        }

        /** Set the dive or cycle number of this message.
         *
         *  @param[in]  id_dive         The dive or cycle number of this message to set.
         */
        void set_id_dive(int16_t id_dive) {
          id_dive_ = id_dive;
        }

        /** Set the unique instrument identifier.
         *
         *  @param[in]  id_instrument   The unique instrument identifier to set.
         */
        void set_id_instrument(uint16_t id_instrument) {
          id_instrument_ = id_instrument;
        }

        /** Set the X-Message type identifier.
         *
         *  @param[in]  id_xmsg_type    The X-Message type identifier to set.
         */
        void set_id_xmsg_type(XMsgTypeId_t id_xmsg_type) {
          id_xmsg_type_ = id_xmsg_type;
        }

        /** Set the size of the X-Message, in bytes.
         *
         *  @param[in]  xmsg_size_bytes The size of the X-Message to set, in bytes.
         */
        void set_xmsg_size_bytes(uint16_t xmsg_size_bytes) {
          xmsg_size_bytes_ = xmsg_size_bytes;
        }

      public:

        /* Public Member Fucntions: */

        /** The Parameterized constructor.
         *
         *  @param[in]  dive        The dive or cycle number of this message.
         *  @param[in]  whoi        The unique WHOI identifier.
         *  @param[in]  data        The binary X-message data to decode.
         *
         *  @exception  None. Never throws.
         */
        XMessage(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) ATTR_NO_EXCEPTIONS :
                id_dive_(dive),
                id_instrument_(whoi),
                id_xmsg_type_((data.empty() ? XMSG_TYPE_ID_UNKNOWN : (XMsgTypeId_t) data.at(0))),
                xmsg_size_bytes_(data.size()) {
          /* Do nothing. */
        }

        /** The destructor.
         *
         */
        virtual ~XMessage() {
          /* Do nothing. */
        }

        /** Check the X-message for the correct ID, minimum size, and message terminator.
         *
         *  @param[in]      data        The X-message binary data to check.
         *  @param[in]      id          The expected ID of the X-message. If this field is less that 0,
         *                              then the ID check will be skipped.
         *  @param[in]      minBytes    The minimum number of bytes expected in a valid message.
         *  @param[in]      parseSize   If TRUE, then the message size field will be parsed from the
         *                              X-Message. Otherwise, the size field will not be parsed.
         *
         *  @exception      util::log::MessageException if
         *                      - the message does not contain the expected ID,
         *                      - the message does not contain the minimum number of expected bytes,
         *                      - the message does not contain a terminator (';') in the correct location,
         *                          or,
         *                      - any other error occurs.
         *
         *  @return         A pointer to the underlying data buffer of data.
         */
        const uint8_t* CheckXMsgData(const vector<uint8_t>& data, int id, int minBytes,
        bool parseSize = true) {
          try {
            // Check minimum message size.
            if(data.empty()) {
              ostringstream msg;
              msg << "Invalid XMessage: Message is empty! Expected at least "
                  << minBytes
                  << " bytes, but received 0 bytes!";
              BOOST_THROW_EXCEPTION(log::MessageException(msg));
            }

            // Check minimum message size.
            if((int) data.size() < minBytes) {
              ostringstream msg;
              msg << "Invalid XMessage: expected at least "
                  << minBytes
                  << " bytes, but only received "
                  << data.size()
                  << " bytes!";
              BOOST_THROW_EXCEPTION(log::MessageException(msg));
            }

            // Get byte array.
            const uint8_t* d = data.data();

            // Check X ID.
            if(id >= 0 && d[0] != id) {
              ostringstream msg;
              msg << "Invalid XMessage ID: expected 0x"
                  << setfill('0')
                  << setw(2)
                  << hex
                  << id
                  << ", received 0x"
                  << hex
                  << (int) d[0]
                  << "!";
              EXC_THROW(msg.str());
            }
            set_id_xmsg_type((XMsgTypeId_t) d[0]);

            if(parseSize) {
              // Check message size.
              set_xmsg_size_bytes(0x0FFF & u16(d, 1));
              if(xmsg_size_bytes() > data.size()) {
                ostringstream msg;
                msg << "Invalid XMessage size: message specifies at least "
                    << xmsg_size_bytes()
                    << " bytes, but only "
                    << data.size()
                    << " bytes were received.";
                EXC_THROW(msg.str());
              } else if(data.size() > xmsg_size_bytes()) {
                lwrn() << "Extra bytes in XMessage: message specifies "
                       << xmsg_size_bytes()
                       << "bytes, but "
                       << data.size()
                       << " bytes were received.";
              }

              // Check for terminator.
              if(d[xmsg_size_bytes() - 1] != 0x3B) {
                ostringstream msg;
                msg << "Invalid XMessage: expected terminator ';' (0x3B) at byte "
                    << (xmsg_size_bytes() - 1)
                    << ", but received 0x"
                    << hex
                    << d[xmsg_size_bytes() - 1];
                EXC_THROW(msg.str());
              }
            }

            return d;

          }CATCH_BLOCK("X-message failed check!")

          return NULL;
        }

        /** Decode the X-Message data.
         *
         *  @param[in]      data        The encoded X message to decode.
         */
        virtual void DecodeXMsgData(const vector<uint8_t>& data) = 0;

        /** Get the dive or cycle number of this X-Message.
         *
         *  @return         The dive or cycle number of this X-Message.
         */
        int16_t id_dive() const {
          return id_dive_;
        }

        /** Get the unique instrument identifier of this X-Message.
         *
         *  @return         The unique instrument identifier of this X-Message.
         */
        uint16_t id_instrument() const {
          return id_instrument_;
        }

        /** Get the enumerated X-Message type identifier.
         *
         *  @return         The enumerated X-Message type identifier.
         */
        XMsgTypeId_t id_xmsg_type() const {
          return id_xmsg_type_;
        }

        /** Get the size of the X-Message, in bytes.
         *
         *  @return         The size of the X-Message, in bytes.
         */
        uint16_t xmsg_size_bytes() const {
          return xmsg_size_bytes_;
        }

    };

  } /* namespace xmsg */
} /* namespace util */

#endif /* XMSG_H_ */
