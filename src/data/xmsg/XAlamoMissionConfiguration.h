/** The @ref util::xmsg::XAlamoMissionConfiguration class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 15, 2014 4:02:57 PM
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

#ifndef XALAMOMISSIONCONFIGURATION_H_
#define XALAMOMISSIONCONFIGURATION_H_

/* Includes */

#include "../../utility/DateUtils.h"
#include "../../utility/Log.h"
#include "common/XDefinitions.h"

namespace util {
namespace xmsg {

/** Class representing an ALAMO Mission Configuration message.
 *
 */
class XAlamoMissionConfiguration:
        public util::date::DateUtils {

private:

    /* Data Members */

    /** The software version identifier.  */
    uint32_t fw_version_;

    /** The message version identifier.
     *
     */
    uint8_t msg_version_;

    /** The sensor type identifier.
     *
     */
    uint8_t sensor_type_;

    /* Data Members - General Configuration */

    /** The mission, dive, or cycle number for this mission configuration. */
    int16_t msn_;

    /** The target descent/drift/park pressure, in dBar. */
    uint16_t dtd_;

    /** The maximum descent/fall/profile pressure, in dBar. */
    uint16_t mds_;

    /** The auto-transmit mission configuration message flag.
     *
     */
    uint8_t apt_;

    /** The bluetooth mode.
     *
     */
    uint8_t btm_;

    /** The profile data encode method.
     *
     */
    uint8_t xas_;

    /** The initial trigger depth, in dBar.
     *
     */
    uint16_t mtd_;

    /** The initial trigger timeout/interval, in milliseconds.
     *
     */
    uint16_t mtt_;

    /* Data Members - Ice Configuration */

    uint8_t icm_;

    uint16_t imm_;

    uint8_t ism_;

    uint16_t ica_[2];

    uint16_t icb_[2];

    uint16_t imt_[2];

    uint16_t ilt_[2];

    uint16_t icn_;

    uint16_t icx_;

    int16_t ict_;

    uint16_t ics_;

    uint8_t iss_[2];

    uint16_t ist_;

    uint16_t iso_;

    uint8_t isp_;

    uint8_t iue_[2];

    uint16_t icd_[2];

    uint32_t icr_[2];

    uint8_t icg_[2];

    uint8_t isa_;

    uint16_t isd_;

    uint16_t iat_;

    uint16_t isv_;

    uint16_t isw_;

    uint16_t ico_[2];

    uint32_t icf_;

    uint32_t icw_;

    /* Data Members - IMU Configuration */

    /** The motion processing unit (MPU) data acquisition operational mode.
     *
     */
    uint8_t mam_;

    /** The motion processing unit (MPU) sampling duration, in seconds.
     *
     */
    uint16_t mat_;

    uint16_t aca_;

    uint16_t acg_;

    uint16_t acr_;

    /* Data Members - BIT Configuration */

    uint16_t pnb_;

    uint16_t pnc_;

    uint16_t pxc_;

    /* Data Members - Calibration */

    uint16_t pbl_;

    uint16_t pbh_;

    uint16_t pp0_;

    uint16_t pp1_;

    uint16_t pvl_;

    uint16_t pvh_;

    uint16_t pv0_;

    uint16_t pv5_;

    uint16_t pc0_;

    uint16_t pc1_;

    /* Data Members - Profile Configuration */

    /** The CTD profiling mode.
     *
     */
    uint8_t cps_;

    /** The CTD sampling type.
     *
     */
    uint8_t cst_;

    /** The profile cutoff pressure.
     *
     *  @todo   Determine gain for profile cutoff pressure field.
     *
     *  @note   This field was added in firmware version 0x02006900.
     *
     */
    uint16_t ctf_;

    /** The pressure sensor offset reset lower pressure limit.
     *
     *  @todo   Determine gain for pressure sensor offset reset lower pressure limit field.
     *
     *  @note   This field was added in firmware version 0x02006900.
     *
     */
    uint16_t ctl_;

    /** The pressure sensor offset reset lower pressure limit.
     *
     *  @todo   Determine gain for pressure sensor offset reset lower pressure limit field.
     *
     *
     *  @note   This field was added in firmware version 0x02006900.
     *
     */
    uint16_t ctu_;

    /** The CTD continuous sampling period, in milliseconds.
     *
     */
    uint32_t csp_;

    /** The CTD sampling period pressure boundaries, in dBar.
     *
     */
    uint16_t cpb_[3];

    /** The CTD sampling period bin sizes, in 0.1 dBar
     *
     */
    uint16_t cpn_[3];

    /** The CTD sampling period interval, in milliseconds.
     *
     */
    uint16_t cpp_[3];

    /* Data Members - Descent State Configuration */

    /** The start time of the descend state of the dive cycle.
     *
     */
    ptime descend_start_time_;

    /** The descent number.
     *
     */
    uint16_t dns_;

    uint8_t dra_;

    /** The descent state start pressure, in dBar.
     *
     */
    uint16_t dsd_;

    /** The descent state start timeout, in seconds.
     *
     */
    uint16_t dst_;

    uint16_t ddf_;

    uint16_t ddc_;

    /** The descent state sampling interval, in milliseconds.
     *
     */
    uint32_t dsi_;

    /** The descent state data storage pressure interval, in dBar.
     *
     */
    uint16_t ddd_;

    /** The descent state data storage interval, in seconds.
     *
     */
    uint16_t ddt_;

    /** The descent state fall timeout, in seconds.
     *
     */
    uint16_t dft_;

    uint8_t dcm_;

    uint8_t dal_;

    uint16_t dct_;


    uint8_t dca_;

    uint16_t drc_;

    uint16_t drt_;

    uint16_t dcx_;

    uint16_t dcp_;

    uint16_t dcf_;

    uint16_t dcd_;

    uint16_t dcr_;

    uint16_t dsw_;

    uint16_t duc_;

    uint16_t dpt_;

    uint16_t dut_;

    uint16_t dpd_;

    uint16_t dvt_;

    uint16_t dvd_;

    uint16_t dvw_;


    uint16_t pfr_;

    uint16_t pfc_;

    uint8_t ftm_;

    uint8_t fta_;

    uint16_t fto_;

    uint16_t fos_;

    uint16_t ddp_;

    uint16_t dds_;

    /* Data Members - Drift/Park State Configuration */

    /** The start time of the park/drift state of the dive cycle.
     *
     */
    ptime park_start_time_;

    /** The park/drift number.
     *
     */
    uint16_t pns_;

    /** The park/drift state timeout, in seconds.
     *
     */
    uint32_t pto_;

    /** The bit-encoded park/drift state hour timeout trigger(s).
     *
     */
    uint32_t pth_;

    /** The park/drift state sampling mode.
     *
     */
    uint8_t psa_;

    /** The park/drift state sample interval/timeout, in milliseconds.
     *
     */
    uint32_t psi_;

    /** The park/drift state fall mode.
     *
     */
    uint8_t pfm_;

    uint16_t pft_;

    /** The park/drift state fall depth (i.e. profile depth), in dBar.
     *
     */
    uint16_t pfd_;

    uint16_t pfv_;

    /** The park/drift state data storage interval/timeout, in seconds.
     *
     */
    uint16_t pdt_;

    uint8_t psm_;

    uint16_t psd_;

    uint16_t pst_;

    uint16_t psv_;

    uint16_t psw_;

    /* Data Members - Ascent State Configuration */

    /** The start time of the ascent state of the dive cycle.
     *
     */
    ptime ascent_start_time_;

    /** The ascent number.
     *
     */
    uint16_t asn_;

    /** The ascent state rise rate, in centimeters per second.
     *
     */
    uint16_t arr_;

    uint16_t apm_;

    /** The ascent state pump on duration/interval/timeout, in seconds.
     *
     */
    uint16_t apn_;

    /** The ascent state pump off duration/interval/timeout, in seconds.
     *
     */
    uint16_t apf_;

    /** The ascent state sampling interval/timeout, in milliseconds.
     *
     */
    uint32_t asi_;

    /** The ascent state surface threshold pressure, in dBar.
     *
     */
    uint16_t asd_;

    /** The ascent state data storage pressure interval, in dBar.
     *
     */
    uint16_t add_;

    /** The ascent state data storage interval timeout, in seconds.
     *
     */
    uint16_t adt_;

    uint16_t arc_;

    uint16_t ahr_;

    uint16_t ahd_;

    /* Data Members - Surface State Configuration */

    /** The start time of the surface state of the dive cycle.
     *
     */
    ptime surface_start_time_;

    /** The surface number.
     *
     */
    uint16_t sfn_;

    /** The surface state sensor depth compensation offset, in dBar.
     *
     */
    int16_t sdc_;

    /** The surface state GPS timeout, in minutes.
     *
     */
    uint16_t sgt_;

    /** The surface state Iridium session timeout, in minutes.
     *
     */
    uint16_t set_;

    /** The surface state beacon mode flag.
     *
     */
    uint8_t sbm_;

    /** The surface state beacon interval timeout, in seconds.
     *
     */
    uint16_t sbt_;

    uint32_t sdt_;

    uint16_t ssa_;

    /** The surface state sampling interval timeout, in milliseconds.
     *
     */
    uint32_t ssi_;

    /** The surface state sampling storage duration/interval/timeout, in seconds.
     *
     */
    uint16_t sds_;

    uint8_t sam_;

    uint16_t sax_;

    uint16_t san_;

    uint16_t sao_;

    uint16_t sap_;

    uint16_t spt_;

    uint16_t ssp_;

private:

    /* Private Member Functions: */

    /** Default constructor.
     *
     */
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wreorder"
    XAlamoMissionConfiguration(void) :
            aca_(XMSG_BAD_U16),
            acg_(XMSG_BAD_U16),
            acr_(XMSG_BAD_U16),
            add_(XMSG_BAD_U16),
            adt_(XMSG_BAD_U16),
            ahd_(XMSG_BAD_U16),
            ahr_(XMSG_BAD_U16),
            apf_(XMSG_BAD_U16),
            apm_(XMSG_BAD_U16),
            apn_(XMSG_BAD_U16),
            apt_(XMSG_BAD_U08),
            arc_(XMSG_BAD_U16),
            arr_(XMSG_BAD_U16),
            asd_(XMSG_BAD_U16),
            asi_(XMSG_BAD_U32),
            asn_(XMSG_BAD_U16),
            btm_(XMSG_BAD_U08),
            cpb_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
            cpn_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
            cpp_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
            cps_(XMSG_BAD_U08),
            csp_(XMSG_BAD_U32),
            cst_(XMSG_BAD_U08),
            ctf_(XMSG_BAD_U16),
            ctl_(XMSG_BAD_U16),
            ctu_(XMSG_BAD_U16),
            dal_(XMSG_BAD_U08),
            dca_(XMSG_BAD_U08),
            dcd_(XMSG_BAD_U16),
            dcf_(XMSG_BAD_U16),
            dcm_(XMSG_BAD_U08),
            dcp_(XMSG_BAD_U16),
            dcr_(XMSG_BAD_U16),
            dct_(XMSG_BAD_U16),
            dcx_(XMSG_BAD_U16),
            ddc_(XMSG_BAD_U16),
            ddd_(XMSG_BAD_U16),
            ddf_(XMSG_BAD_U16),
            ddp_(XMSG_BAD_U16),
            dds_(XMSG_BAD_U16),
            ddt_(XMSG_BAD_U16),
            dft_(XMSG_BAD_U16),
            dns_(XMSG_BAD_U16),
            dpd_(XMSG_BAD_U16),
            dpt_(XMSG_BAD_U16),
            dra_(XMSG_BAD_U08),
            drc_(XMSG_BAD_U16),
            drt_(XMSG_BAD_U16),
            dsd_(XMSG_BAD_U16),
            dsi_(XMSG_BAD_U32),
            dst_(XMSG_BAD_U16),
            dsw_(XMSG_BAD_U16),
            dtd_(XMSG_BAD_U16),
            duc_(XMSG_BAD_U16),
            dut_(XMSG_BAD_U16),
            dvd_(XMSG_BAD_U16),
            dvt_(XMSG_BAD_U16),
            dvw_(XMSG_BAD_U16),
            fos_(XMSG_BAD_U16),
            fta_(XMSG_BAD_U08),
            ftm_(XMSG_BAD_U08),
            fto_(XMSG_BAD_U16),
            fw_version_(XMSG_BAD_U32),
            iat_(XMSG_BAD_U16),
            ica_{ XMSG_BAD_U16, XMSG_BAD_U16, },
            icb_{ XMSG_BAD_U16, XMSG_BAD_U16, },
            icd_{ XMSG_BAD_U16, XMSG_BAD_U16, },
            icf_(XMSG_BAD_U32),
            icg_{ XMSG_BAD_U08, XMSG_BAD_U08, },
            icm_(XMSG_BAD_U08),
            icn_(XMSG_BAD_U16),
            ico_{ XMSG_BAD_U16, XMSG_BAD_U16, },
            icr_{ XMSG_BAD_U32, XMSG_BAD_U32, },
            ics_(XMSG_BAD_U16),
            ict_(XMSG_BAD_I16),
            icw_(XMSG_BAD_U32),
            icx_(XMSG_BAD_U16),
            ilt_{ XMSG_BAD_U16, XMSG_BAD_U16, },
            imm_(XMSG_BAD_U16),
            imt_{ XMSG_BAD_U16, XMSG_BAD_U16, },
            isa_(XMSG_BAD_U08),
            isd_(XMSG_BAD_U16),
            ism_(XMSG_BAD_U08),
            iso_(XMSG_BAD_U16),
            isp_(XMSG_BAD_U08),
            iss_{ XMSG_BAD_U08, XMSG_BAD_U08, },
            ist_(XMSG_BAD_U16),
            isv_(XMSG_BAD_U16),
            isw_(XMSG_BAD_U16),
            iue_{ XMSG_BAD_U08, XMSG_BAD_U08, },
            mam_(XMSG_BAD_U08),
            mat_(XMSG_BAD_U16),
            mds_(XMSG_BAD_U16),
            msg_version_(XMSG_BAD_U08),
            msn_(XMSG_BAD_I16),
            mtd_(XMSG_BAD_U16),
            mtt_(XMSG_BAD_U16),
            pbh_(XMSG_BAD_U16),
            pbl_(XMSG_BAD_U16),
            pc0_(XMSG_BAD_U16),
            pc1_(XMSG_BAD_U16),
            pdt_(XMSG_BAD_U16),
            pfc_(XMSG_BAD_U16),
            pfd_(XMSG_BAD_U16),
            pfm_(XMSG_BAD_U08),
            pfr_(XMSG_BAD_U16),
            pft_(XMSG_BAD_U16),
            pfv_(XMSG_BAD_U16),
            pnb_(XMSG_BAD_U16),
            pnc_(XMSG_BAD_U16),
            pns_(XMSG_BAD_U16),
            pp0_(XMSG_BAD_U16),
            pp1_(XMSG_BAD_U16),
            psa_(XMSG_BAD_U08),
            psd_(XMSG_BAD_U16),
            psi_(XMSG_BAD_U32),
            psm_(XMSG_BAD_U08),
            pst_(XMSG_BAD_U16),
            psv_(XMSG_BAD_U16),
            psw_(XMSG_BAD_U16),
            pth_(XMSG_BAD_U32),
            pto_(XMSG_BAD_U32),
            pv0_(XMSG_BAD_U16),
            pv5_(XMSG_BAD_U16),
            pvh_(XMSG_BAD_U16),
            pvl_(XMSG_BAD_U16),
            pxc_(XMSG_BAD_U16),
            sam_(XMSG_BAD_U08),
            san_(XMSG_BAD_U16),
            sao_(XMSG_BAD_U16),
            sap_(XMSG_BAD_U16),
            sax_(XMSG_BAD_U16),
            sbm_(XMSG_BAD_U08),
            sbt_(XMSG_BAD_U16),
            sdc_(XMSG_BAD_I16),
            sds_(XMSG_BAD_U16),
            sdt_(XMSG_BAD_U32),
            sensor_type_(XMSG_BAD_U08),
            set_(XMSG_BAD_U16),
            sfn_(XMSG_BAD_U16),
            sgt_(XMSG_BAD_U16),
            spt_(XMSG_BAD_U16),
            ssa_(XMSG_BAD_U16),
            ssi_(XMSG_BAD_U32),
            ssp_(XMSG_BAD_U16),
            xas_(XMSG_BAD_U08) {
        /* Do nothing. */
    }
#   pragma GCC diagnostic pop

public:

    /* Public Member Functions: */

    /** Parameterized Constructor.
     *
     *  @param[in]      data        The binary data to decode.
     *
     *  @throw          log::MessageException if the message is invalid or an error occurs while
     *                  decoding.
     */
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wreorder"
    XAlamoMissionConfiguration(const vector<uint8_t>& data):
            aca_(XMSG_BAD_U16),
            acg_(XMSG_BAD_U16),
            acr_(XMSG_BAD_U16),
            add_(XMSG_BAD_U16),
            adt_(XMSG_BAD_U16),
            ahd_(XMSG_BAD_U16),
            ahr_(XMSG_BAD_U16),
            apf_(XMSG_BAD_U16),
            apm_(XMSG_BAD_U16),
            apn_(XMSG_BAD_U16),
            apt_(XMSG_BAD_U08),
            arc_(XMSG_BAD_U16),
            arr_(XMSG_BAD_U16),
            asd_(XMSG_BAD_U16),
            asi_(XMSG_BAD_U32),
            asn_(XMSG_BAD_U16),
            btm_(XMSG_BAD_U08),
            cpb_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
            cpn_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
            cpp_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
            cps_(XMSG_BAD_U08),
            csp_(XMSG_BAD_U32),
            cst_(XMSG_BAD_U08),
            ctf_(XMSG_BAD_U16),
            ctl_(XMSG_BAD_U16),
            ctu_(XMSG_BAD_U16),
            dal_(XMSG_BAD_U08),
            dca_(XMSG_BAD_U08),
            dcd_(XMSG_BAD_U16),
            dcf_(XMSG_BAD_U16),
            dcm_(XMSG_BAD_U08),
            dcp_(XMSG_BAD_U16),
            dcr_(XMSG_BAD_U16),
            dct_(XMSG_BAD_U16),
            dcx_(XMSG_BAD_U16),
            ddc_(XMSG_BAD_U16),
            ddd_(XMSG_BAD_U16),
            ddf_(XMSG_BAD_U16),
            ddp_(XMSG_BAD_U16),
            dds_(XMSG_BAD_U16),
            ddt_(XMSG_BAD_U16),
            dft_(XMSG_BAD_U16),
            dns_(XMSG_BAD_U16),
            dpd_(XMSG_BAD_U16),
            dpt_(XMSG_BAD_U16),
            dra_(XMSG_BAD_U08),
            drc_(XMSG_BAD_U16),
            drt_(XMSG_BAD_U16),
            dsd_(XMSG_BAD_U16),
            dsi_(XMSG_BAD_U32),
            dst_(XMSG_BAD_U16),
            dsw_(XMSG_BAD_U16),
            dtd_(XMSG_BAD_U16),
            duc_(XMSG_BAD_U16),
            dut_(XMSG_BAD_U16),
            dvd_(XMSG_BAD_U16),
            dvt_(XMSG_BAD_U16),
            dvw_(XMSG_BAD_U16),
            fos_(XMSG_BAD_U16),
            fta_(XMSG_BAD_U08),
            ftm_(XMSG_BAD_U08),
            fto_(XMSG_BAD_U16),
            fw_version_(XMSG_BAD_U32),
            iat_(XMSG_BAD_U16),
            ica_{ XMSG_BAD_U16, XMSG_BAD_U16, },
            icb_{ XMSG_BAD_U16, XMSG_BAD_U16, },
            icd_{ XMSG_BAD_U16, XMSG_BAD_U16, },
            icf_(XMSG_BAD_U32),
            icg_{ XMSG_BAD_U08, XMSG_BAD_U08, },
            icm_(XMSG_BAD_U08),
            icn_(XMSG_BAD_U16),
            ico_{ XMSG_BAD_U16, XMSG_BAD_U16, },
            icr_{ XMSG_BAD_U32, XMSG_BAD_U32, },
            ics_(XMSG_BAD_U16),
            ict_(XMSG_BAD_I16),
            icw_(XMSG_BAD_U32),
            icx_(XMSG_BAD_U16),
            ilt_{ XMSG_BAD_U16, XMSG_BAD_U16, },
            imm_(XMSG_BAD_U16),
            imt_{ XMSG_BAD_U16, XMSG_BAD_U16, },
            isa_(XMSG_BAD_U08),
            isd_(XMSG_BAD_U16),
            ism_(XMSG_BAD_U08),
            iso_(XMSG_BAD_U16),
            isp_(XMSG_BAD_U08),
            iss_{ XMSG_BAD_U08, XMSG_BAD_U08, },
            ist_(XMSG_BAD_U16),
            isv_(XMSG_BAD_U16),
            isw_(XMSG_BAD_U16),
            iue_{ XMSG_BAD_U08, XMSG_BAD_U08, },
            mam_(XMSG_BAD_U08),
            mat_(XMSG_BAD_U16),
            mds_(XMSG_BAD_U16),
            msg_version_(XMSG_BAD_U08),
            msn_(XMSG_BAD_I16),
            mtd_(XMSG_BAD_U16),
            mtt_(XMSG_BAD_U16),
            pbh_(XMSG_BAD_U16),
            pbl_(XMSG_BAD_U16),
            pc0_(XMSG_BAD_U16),
            pc1_(XMSG_BAD_U16),
            pdt_(XMSG_BAD_U16),
            pfc_(XMSG_BAD_U16),
            pfd_(XMSG_BAD_U16),
            pfm_(XMSG_BAD_U08),
            pfr_(XMSG_BAD_U16),
            pft_(XMSG_BAD_U16),
            pfv_(XMSG_BAD_U16),
            pnb_(XMSG_BAD_U16),
            pnc_(XMSG_BAD_U16),
            pns_(XMSG_BAD_U16),
            pp0_(XMSG_BAD_U16),
            pp1_(XMSG_BAD_U16),
            psa_(XMSG_BAD_U08),
            psd_(XMSG_BAD_U16),
            psi_(XMSG_BAD_U32),
            psm_(XMSG_BAD_U08),
            pst_(XMSG_BAD_U16),
            psv_(XMSG_BAD_U16),
            psw_(XMSG_BAD_U16),
            pth_(XMSG_BAD_U32),
            pto_(XMSG_BAD_U32),
            pv0_(XMSG_BAD_U16),
            pv5_(XMSG_BAD_U16),
            pvh_(XMSG_BAD_U16),
            pvl_(XMSG_BAD_U16),
            pxc_(XMSG_BAD_U16),
            sam_(XMSG_BAD_U08),
            san_(XMSG_BAD_U16),
            sao_(XMSG_BAD_U16),
            sap_(XMSG_BAD_U16),
            sax_(XMSG_BAD_U16),
            sbm_(XMSG_BAD_U08),
            sbt_(XMSG_BAD_U16),
            sdc_(XMSG_BAD_I16),
            sds_(XMSG_BAD_U16),
            sdt_(XMSG_BAD_U32),
            sensor_type_(XMSG_BAD_U08),
            set_(XMSG_BAD_U16),
            sfn_(XMSG_BAD_U16),
            sgt_(XMSG_BAD_U16),
            spt_(XMSG_BAD_U16),
            ssa_(XMSG_BAD_U16),
            ssi_(XMSG_BAD_U32),
            ssp_(XMSG_BAD_U16),
            xas_(XMSG_BAD_U08) {
        DecodeXMsgData(data); // Decode the message
    }
#   pragma GCC diagnostic pop

    /** Destructor.
     *
     */
    ~XAlamoMissionConfiguration() {
        /* Do nothing. */
    }

    /** Decode the ALAMO mission configuration message from the binary message data.
     *
     *  @param[in]      data        The binary data to decode.
     *
     *  @throw          log::MessageException if the message is invalid or an error occurs while
     *                  decoding.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {
            const uint8_t* d = data.data();

            set_fw_version(u32(d, 4));

            /* Mission Configuration */

            switch(fw_version()) {
            case 0x02006200: // ALAMO Software version 0x02006200.
            case 0x02006300: // ALAMO Software version 0x02006300.
            case 0x02006700: // ALAMO Software version 0x02006700.
            case 0x02006800: // ALAMO Software version 0x02006800.

            {

                if(data.size() != 323) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 323 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                set_msn(u16(d, 8));
                set_dtd(u16(d, 10));
                set_mds(u16(d, 12));
                set_apt(u8(d, 14));
                set_xas(u8(d, 15));
                set_cps(u8(d, 16));
                set_cst(u8(d, 17));
                set_csp(u32(d, 18));
                cpb_[0] = u16(d, 22);
                cpb_[1] = u16(d, 24);
                cpb_[2] = u16(d, 26);
                cpn_[0] = u16(d, 28);
                cpn_[1] = u16(d, 30);
                cpn_[2] = u16(d, 32);
                cpp_[0] = u16(d, 34);
                cpp_[1] = u16(d, 36);
                cpp_[2] = u16(d, 38);
                set_mtd(u16(d, 40));
                set_mtt(u32(d, 42));
                set_mam(u8(d, 46));
                set_mat(u16(d, 47));
                set_aca(u16(d, 49));
                set_acg(u16(d, 51));
                set_acr(u16(d, 53));
                set_pnb(u16(d, 55));
                set_pnc(u16(d, 57));
                set_pxc(u16(d, 59));
                set_pbl(u16(d, 61));
                set_pbh(u16(d, 63));
                set_pp0(u16(d, 65));
                set_pp1(u16(d, 67));
                set_pvl(u16(d, 69));
                set_pvh(u16(d, 71));
                set_pv0(u16(d, 73));
                set_pv5(u16(d, 75));
                set_pc0(u16(d, 77));
                set_pc1(u16(d, 79));
                set_descend_start_time(s2000(d, 99));
                set_dns(u16(d, 103));
                set_dra(u16(d, 105));
                set_dsd(u16(d, 107));
                set_dst(u16(d, 109));
                set_ddf(u16(d, 111));
                set_ddc(u16(d, 113));
                set_dsi(u32(d, 115));
                set_ddd(u16(d, 119));
                set_ddt(u16(d, 121));
                set_dft(u16(d, 123));
                set_dcm(u8(d, 125));
                set_dal(u8(d, 126));
                set_dct(u16(d, 127));
                set_dca(u8(d, 129));
                set_drc(u16(d, 130));
                set_drt(u16(d, 132));
                set_dcx(u16(d, 134));
                set_dcp(u16(d, 136));
                set_dcf(u16(d, 138));
                set_dcd(u16(d, 140));
                set_dcr(u16(d, 142));
                set_dsw(u16(d, 144));
                set_duc(u16(d, 146));
                set_dpt(u16(d, 148));
                set_dut(u16(d, 150));
                set_dpd(u16(d, 152));
                set_dvt(u16(d, 154));
                set_pfr(u16(d, 156));
                set_pfc(u16(d, 158));
                set_ddp(u16(d, 160));
                set_dds(u16(d, 162));
                set_park_start_time(s2000(d, 179));
                set_pns(u16(d, 183));
                set_pto(u32(d, 185));
                set_pth(u32(d, 189));
                set_psa(u8(d, 193));
                set_psi(u32(d, 194));
                set_pfm(u8(d, 198));
                set_pft(u16(d, 199));
                set_pfd(u16(d, 201));
                set_pfv(u16(d, 203));
                set_pdt(u16(d, 205));
                set_psm(u8(d, 207));
                set_psd(u16(d, 208));
                set_pst(u16(d, 210));
                set_psv(u16(d, 212));
                set_psw(u16(d, 214));
                set_ascent_start_time(s2000(d, 227));
                set_asn(u16(d, 231));
                set_arr(u16(d, 233));
                set_apm(u16(d, 235));
                set_apn(u16(d, 237));
                set_apf(u16(d, 239));
                set_asi(u32(d, 241));
                set_asd(u16(d, 245));
                set_add(u16(d, 247));
                set_adt(u16(d, 249));
                set_arc(u16(d, 251));
                set_ahr(u16(d, 253));
                set_ahd(u16(d, 255));
                set_surface_start_time(s2000(d, 271));
                set_sfn(u16(d, 275));
                set_sdc(i16(d, 277));
                set_sgt(u16(d, 279));
                set_set(u16(d, 281));
                set_sbm(u8(d, 283));
                set_sbt(u16(d, 284));
                set_sdt(u32(d, 286));
                set_ssa(u16(d, 290));
                set_ssi(u32(d, 292));
                set_sds(u16(d, 296));
                set_sam(u8(d, 298));
                set_sax(u16(d, 299));
                set_san(u16(d, 301));
                set_sao(u16(d, 303));
                set_sap(u16(d, 305));
                set_spt(u16(d, 307));

            }

                break;

            case 0x02006900: // ALAMO Software version 0x02006900.
            case 0x02036200: // ALAMO Software version 0x02036200.
            case 0x02046200: // ALAMO Software version 0x02046200.
            case 0x02056200: // ALAMO Software version 0x02056200.

            {

                if(data.size() != 322) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 323 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                /* Config. - General: */

                set_msn(u16(d, 8));
                set_dtd(u16(d, 10));
                set_mds(u16(d, 12));
                set_apt(u8(d, 14));
                set_xas(u8(d, 15));
                set_cps(u8(d, 16));
                set_cst(u8(d, 17));
                set_ctf(u16(d, 18));
                set_ctl(u16(d, 20));
                set_ctu(u16(d, 22));

                set_csp(u32(d, 18 + 6));
                cpb_[0] = u16(d, 22 + 6);
                cpb_[1] = u16(d, 24 + 6);
                cpb_[2] = u16(d, 26 + 6);
                cpn_[0] = u16(d, 28 + 6);
                cpn_[1] = u16(d, 30 + 6);
                cpn_[2] = u16(d, 32 + 6);
                cpp_[0] = u16(d, 34 + 6);
                cpp_[1] = u16(d, 36 + 6);
                cpp_[2] = u16(d, 38 + 6);
                set_mtd(u16(d, 40 + 6));
                set_mtt(u32(d, 42 + 6));
                set_mam(u8(d, 46 + 6));
                set_mat(u16(d, 47 + 6));
                set_aca(u16(d, 49 + 6));
                set_acg(u16(d, 51 + 6));
                set_acr(u16(d, 53 + 6));
                set_pnb(u16(d, 55 + 6));
                set_pnc(u16(d, 57 + 6));
                set_pxc(u16(d, 59 + 6));
                set_pbl(u16(d, 61 + 6));
                set_pbh(u16(d, 63 + 6));
                set_pp0(u16(d, 65 + 6));
                set_pp1(u16(d, 67 + 6));
                set_pvl(u16(d, 69 + 6));
                set_pvh(u16(d, 71 + 6));
                set_pv0(u16(d, 73 + 6));
                set_pv5(u16(d, 75 + 6));
                set_pc0(u16(d, 77 + 6));
                set_pc1(u16(d, 79 + 6));

                set_icm(u8(d, 87));
                set_imm(u16(d, 88));
                set_ica0(u16(d, 90));
                set_ica1(u16(d, 92));
                set_icb0(u16(d, 94));
                set_icb1(u16(d, 96));
                set_imt0(u16(d, 98));
                set_imt1(u16(d, 100));
                set_ilt0(u16(d, 102));
                set_ilt1(u16(d, 104));
                set_icn(u16(d, 106));
                set_icx(u16(d, 108));
                set_ict(i16(d, 110));
                set_ics(u16(d, 112));
                set_iss0(u8(d, 114));
                set_iss1(u8(d, 115));
                set_ist(u16(d, 116));
                set_iso(u16(d, 118));
                set_isp(u8(d, 120));
                set_iue0(u8(d, 121));
                set_iue1(u8(d, 122));
                set_icd0(u16(d, 123));
                set_icd1(u16(d, 125));
                set_icr0(u32(d, 127));
                set_icr1(u32(d, 131));
                set_icg0(u8(d, 135));
                set_icg1(u8(d, 136));
                set_ico0(u16(d, 137));
                set_ico1(u16(d, 139));
                set_icf(u8(d, 141));
                set_icw(u8(d, 145));

                set_descend_start_time(s2000(d, 9 + 50));
                set_dns(u16(d, 103 + 50));
                set_dra(u16(d, 105 + 50));
                set_dsd(u16(d, 107 + 50));
                set_dst(u16(d, 109 + 50));
                set_ddf(u16(d, 111 + 50));
                set_ddc(u16(d, 113 + 50));
                set_dsi(u32(d, 115 + 50));
                set_ddd(u16(d, 119 + 50));
                set_ddt(u16(d, 121));
                set_dft(u16(d, 123 + 50));
                set_dcm(u8(d, 125 + 50));
                set_dal(u8(d, 126 + 50));
                set_dct(u16(d, 127 + 50));
                set_dca(u8(d, 129 + 50));
                set_drc(u16(d, 130 + 50));
                set_drt(u16(d, 132 + 50));
                set_dcx(u16(d, 134 + 50));
                set_dcp(u16(d, 136 + 50));
                set_dcf(u16(d, 138 + 50));
                set_dcd(u16(d, 140 + 50));
                set_dcr(u16(d, 142 + 50));
                set_dsw(u16(d, 144 + 50));
                set_duc(u16(d, 146 + 50));
                set_dpt(u16(d, 148 + 50));
                set_dut(u16(d, 150 + 50));
                set_dpd(u16(d, 152 + 50));
                set_dvt(u16(d, 154 + 50));
                set_pfr(u16(d, 156 + 50));
                set_pfc(u16(d, 158 + 50));
                set_ddp(u16(d, 160 + 50));
                set_dds(u16(d, 162 + 50));
                set_park_start_time(s2000(d, 179 + 35));
                set_pns(u16(d, 183 + 35));
                set_pto(u32(d, 185 + 35));
                set_pth(u32(d, 189 + 35));
                set_psa(u8(d, 193 + 35));
                set_psi(u32(d, 194 + 35));
                set_pfm(u8(d, 198 + 35));
                set_pft(u16(d, 199 + 35));
                set_pfd(u16(d, 201 + 35));
                set_pfv(u16(d, 203 + 35));
                set_pdt(u16(d, 205 + 35));
                set_psm(u8(d, 207 + 35));
                set_psd(u16(d, 208 + 35));
                set_pst(u16(d, 210 + 35));
                set_psv(u16(d, 212 + 35));
                set_psw(u16(d, 214 + 35));
                set_ascent_start_time(s2000(d, 227 + 24));
                set_asn(u16(d, 231 + 24));
                set_arr(u16(d, 233 + 24));
                set_apm(u16(d, 235 + 24));
                set_apn(u16(d, 237 + 24));
                set_apf(u16(d, 239 + 24));
                set_asi(u32(d, 241 + 24));
                set_asd(u16(d, 245 + 24));
                set_add(u16(d, 247 + 24));
                set_adt(u16(d, 249 + 24));
                set_arc(u16(d, 251 + 24));
                set_ahr(u16(d, 253 + 24));
                set_ahd(u16(d, 255 + 24));
                set_surface_start_time(s2000(d, 271 + 10));
                set_sfn(u16(d, 275 + 10));
                set_sdc(i16(d, 277 + 10));
                set_sgt(u16(d, 279 + 10));
                set_set(u16(d, 281 + 10));
                set_sbm(u8(d, 283 + 10));
                set_sbt(u16(d, 284 + 10));
                set_sdt(u32(d, 286 + 10));
                set_ssa(u16(d, 290 + 10));
                set_ssi(u32(d, 292 + 10));
                set_sds(u16(d, 296 + 10));
                set_sam(u8(d, 298 + 10));
                set_sax(u16(d, 299 + 10));
                set_san(u16(d, 301 + 10));
                set_sao(u16(d, 303 + 10));
                set_sap(u16(d, 305 + 10));
                set_spt(u16(d, 307 + 10));

                /** @todo Implement ALAMO mission parameter: SSP.
                 *
                 */
//              set_ssp(u16(d, 309 + 10));
            }

                break;

            case 0x03006500: // ALAMO Software version 0x03006500.

            {

                if(data.size() != 328) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 328 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                /* Config. - General: */

                set_msn(u16(d, 8));
                set_dtd(u16(d, 10));
                set_mds(u16(d, 12));
                set_apt(u8(d, 14));
                set_xas(u8(d, 15));
                set_cps(u8(d, 16));
                set_cst(u8(d, 17));
                set_ctf(u16(d, 18));
                set_ctl(u16(d, 20));
                set_ctu(u16(d, 22));

                set_csp(u32(d, 18 + 6));
                cpb_[0] = u16(d, 22 + 6);
                cpb_[1] = u16(d, 24 + 6);
                cpb_[2] = u16(d, 26 + 6);
                cpn_[0] = u16(d, 28 + 6);
                cpn_[1] = u16(d, 30 + 6);
                cpn_[2] = u16(d, 32 + 6);
                cpp_[0] = u16(d, 34 + 6);
                cpp_[1] = u16(d, 36 + 6);
                cpp_[2] = u16(d, 38 + 6);
                set_mtd(u16(d, 40 + 6));
                set_mtt(u32(d, 42 + 6));
                set_mam(u8(d, 46 + 6));
                set_mat(u16(d, 47 + 6));
                set_aca(u16(d, 49 + 6));
                set_acg(u16(d, 51 + 6));
                set_acr(u16(d, 53 + 6));
                set_pnb(u16(d, 55 + 6));
                set_pnc(u16(d, 57 + 6));
                set_pxc(u16(d, 59 + 6));
                set_pbl(u16(d, 61 + 6));
                set_pbh(u16(d, 63 + 6));
                set_pp0(u16(d, 65 + 6));
                set_pp1(u16(d, 67 + 6));
                set_pvl(u16(d, 69 + 6));
                set_pvh(u16(d, 71 + 6));
                set_pv0(u16(d, 73 + 6));
                set_pv5(u16(d, 75 + 6));
                set_pc0(u16(d, 77 + 6));
                set_pc1(u16(d, 79 + 6));

                set_icm(u8(d, 87));
                set_imm(u16(d, 88));
                set_ica0(u16(d, 90));
                set_ica1(u16(d, 92));
                set_icb0(u16(d, 94));
                set_icb1(u16(d, 96));
                set_imt0(u16(d, 98));
                set_imt1(u16(d, 100));
                set_ilt0(u16(d, 102));
                set_ilt1(u16(d, 104));
                set_icn(u16(d, 106));
                set_icx(u16(d, 108));
                set_ict(i16(d, 110));
                set_ics(u16(d, 112));
                set_iss0(u8(d, 114));
                set_iss1(u8(d, 115));
                set_ist(u16(d, 116));
                set_iso(u16(d, 118));
                set_isp(u8(d, 120));
                set_iue0(u8(d, 121));
                set_iue1(u8(d, 122));
                set_icd0(u16(d, 123));
                set_icd1(u16(d, 125));
                set_icr0(u32(d, 127));
                set_icr1(u32(d, 131));
                set_icg0(u8(d, 135));
                set_icg1(u8(d, 136));


                set_ico0(u16(d, 137 + 9));
                set_ico1(u16(d, 139 + 9));
                set_icf(u8(d, 141 + 9));
                set_icw(u8(d, 145 + 9));

                set_descend_start_time(s2000(d, 158));
                set_dns(u16(d, 103 + 59));
                set_dra(u16(d, 105 + 59));
                set_dsd(u16(d, 107 + 59));
                set_dst(u16(d, 109 + 59));
                set_ddf(u16(d, 111 + 59));
                set_ddc(u16(d, 113 + 59));
                set_dsi(u32(d, 115 + 59));
                set_ddd(u16(d, 119 + 59));
                set_ddt(u16(d, 121));
                set_dft(u16(d, 123 + 59));
                set_dcm(u8(d, 125 + 59));
                set_dal(u8(d, 126 + 59));
                set_dct(u16(d, 127 + 59));
                set_dca(u8(d, 129 + 59));
                set_drc(u16(d, 130 + 59));
                set_drt(u16(d, 132 + 59));
                set_dcx(u16(d, 134 + 59));
                set_dcp(u16(d, 136 + 59));
                set_dcf(u16(d, 138 + 59));
                set_dcd(u16(d, 140 + 59));
                set_dcr(u16(d, 142 + 59));
                set_dsw(u16(d, 144 + 59));
                set_duc(u16(d, 146 + 59));
                set_dpt(u16(d, 148 + 59));
                set_dut(u16(d, 150 + 59));
                set_dpd(u16(d, 152 + 59));
                set_dvt(u16(d, 154 + 59));
                set_pfr(u16(d, 156 + 59));
                set_pfc(u16(d, 158 + 59));
                set_ddp(u16(d, 160 + 59));
                set_dds(u16(d, 162 + 59));
                set_park_start_time(s2000(d, 179 + 44));
                set_pns(u16(d, 183 + 44));
                set_pto(u32(d, 185 + 44));
                set_pth(u32(d, 189 + 44));
                set_psa(u8(d, 193 + 44));
                set_psi(u32(d, 194 + 44));
                set_pfm(u8(d, 198 + 44));
                set_pft(u16(d, 199 + 44));
                set_pfd(u16(d, 201 + 44));
                set_pfv(u16(d, 203 + 44));
                set_pdt(u16(d, 205 + 44));
                set_psm(u8(d, 207 + 44));
                set_psd(u16(d, 208 + 44));
                set_pst(u16(d, 210 + 44));
                set_psv(u16(d, 212 + 44));
                set_psw(u16(d, 214 + 44));
                set_ascent_start_time(s2000(d, 227 + 33));
                set_asn(u16(d, 231 + 33));
                set_arr(u16(d, 233 + 33));
                set_apm(u16(d, 235 + 33));
                set_apn(u16(d, 237 + 33));
                set_apf(u16(d, 239 + 33));
                set_asi(u32(d, 241 + 33));
                set_asd(u16(d, 245 + 33));
                set_add(u16(d, 247 + 33));
                set_adt(u16(d, 249 + 33));
                set_arc(u16(d, 251 + 33));
                set_ahr(u16(d, 253 + 33));
                set_ahd(u16(d, 255 + 33));
                set_surface_start_time(s2000(d, 271 + 19));
                set_sfn(u16(d, 275 + 19));
                set_sdc(i16(d, 277 + 19));
                set_sgt(u16(d, 279 + 19));
                set_set(u16(d, 281 + 19));
                set_sbm(u8(d, 283 + 19));
                set_sbt(u16(d, 284 + 19));
                set_sdt(u32(d, 286 + 19));
                set_ssa(u16(d, 290 + 19));
                set_ssi(u32(d, 292 + 19));
                set_sds(u16(d, 296 + 19));
                set_sam(u8(d, 298 + 19));
                set_sax(u16(d, 299 + 19));
                set_san(u16(d, 301 + 19));
                set_sao(u16(d, 303 + 19));
                set_sap(u16(d, 305 + 19));
                set_spt(XMSG_BAD_U16);

                /** @todo Implement ALAMO mission parameter: SSP.
                 *
                 */
//                set_ssp(u16(d, 309 + 10));
            }

                break;

            case 0x03016700: // ALAMO Software version 0x03016700 (Solo-II Hybrid #10083).

                if(data.size() != 328) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 328 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                /* Config. - General: */

                set_msn(u16(d, 8));
                set_dtd(u16(d, 10));
                set_mds(u16(d, 12));
                set_apt(u8(d, 14));
                set_xas(u8(d, 15));
                set_cps(u8(d, 16));
                set_cst(u8(d, 17));
                set_ctf(u16(d, 18));
                set_ctl(u16(d, 20));
                set_ctu(u16(d, 22));

                set_csp(u32(d, 18 + 6));
                cpb_[0] = u16(d, 22 + 6);
                cpb_[1] = u16(d, 24 + 6);
                cpb_[2] = u16(d, 26 + 6);
                cpn_[0] = u16(d, 28 + 6);
                cpn_[1] = u16(d, 30 + 6);
                cpn_[2] = u16(d, 32 + 6);
                cpp_[0] = u16(d, 34 + 6);
                cpp_[1] = u16(d, 36 + 6);
                cpp_[2] = u16(d, 38 + 6);
                set_mtd(u16(d, 40 + 6));
                set_mtt(u32(d, 42 + 6));
                set_mam(u8(d, 46 + 6));
                set_mat(u16(d, 47 + 6));
                set_aca(u16(d, 49 + 6));
                set_acg(u16(d, 51 + 6));
                set_acr(u16(d, 53 + 6));
                set_pnb(u16(d, 55 + 6));
                set_pnc(u16(d, 57 + 6));
                set_pxc(u16(d, 59 + 6));
                set_pbl(u16(d, 61 + 6));
                set_pbh(u16(d, 63 + 6));
                set_pp0(u16(d, 65 + 6));
                set_pp1(u16(d, 67 + 6));
                set_pvl(u16(d, 69 + 6));
                set_pvh(u16(d, 71 + 6));
                set_pv0(u16(d, 73 + 6));
                set_pv5(u16(d, 75 + 6));
                set_pc0(u16(d, 77 + 6));
                set_pc1(u16(d, 79 + 6));

                set_icm(u8(d, 87));
                set_imm(u16(d, 88));
                set_ica0(u16(d, 90));
                set_ica1(u16(d, 92));
                set_icb0(u16(d, 94));
                set_icb1(u16(d, 96));
                set_imt0(u16(d, 98));
                set_imt1(u16(d, 100));
                set_ilt0(u16(d, 102));
                set_ilt1(u16(d, 104));
                set_icn(u16(d, 106));
                set_icx(u16(d, 108));
                set_ict(i16(d, 110));
                set_ics(u16(d, 112));
                set_iss0(u8(d, 114));
                set_iss1(u8(d, 115));
                set_ist(u16(d, 116 + 1));
                set_iso(u16(d, 118 + 1));
                set_isp(u8(d, 120 + 1));
                set_iue0(u8(d, 121 + 1));
                set_iue1(u8(d, 122 + 1));
                set_icd0(u16(d, 123 + 1));
                set_icd1(u16(d, 125 + 1));
                set_icr0(u32(d, 127 + 1));
                set_icr1(u32(d, 131 + 1));

                set_icg0(u8(d, 135 + 1));
                set_icg1(u8(d, 136 + 1));
                set_ico0(u16(d, 137 + 10));
                set_ico1(u16(d, 139 + 10));
                set_icf(u8(d, 141 + 10));
                set_icw(u8(d, 145 + 10));

                set_descend_start_time(s2000(d, 159));
                set_dns(u16(d, 103 + 60));
                set_dra(u16(d, 105 + 60));
                set_dsd(u16(d, 107 + 60));
                set_dst(u16(d, 109 + 60));
                set_ddf(u16(d, 111 + 60));
                set_ddc(u16(d, 113 + 60));
                set_dsi(u32(d, 115 + 60));
                set_ddd(u16(d, 119 + 60));
                set_ddt(u16(d, 121));
                set_dft(u16(d, 123 + 60));
                set_dcm(u8(d, 125 + 60));
                set_dal(u8(d, 126 + 60));
                set_dct(u16(d, 127 + 60));
                set_dca(u8(d, 129 + 60));
                set_drc(u16(d, 130 + 60));
                set_drt(u16(d, 132 + 60));
                set_dcx(u16(d, 134 + 60));
                set_dcp(u16(d, 136 + 60));
                set_dcf(u16(d, 138 + 60));
                set_dcd(u16(d, 140 + 60));
                set_dcr(u16(d, 142 + 60));
                set_dsw(u16(d, 144 + 60));
                set_duc(u16(d, 146 + 60));
                set_dpt(u16(d, 148 + 60));
                set_dut(u16(d, 150 + 60));
                set_dpd(u16(d, 152 + 60));
                set_dvt(u16(d, 154 + 60));
                set_pfr(u16(d, 156 + 60));
                set_pfc(u16(d, 158 + 60));
                set_ddp(u16(d, 160 + 60));
                set_dds(u16(d, 162 + 60));
                set_park_start_time(s2000(d, 179 + 45));
                set_pns(u16(d, 183 + 45));
                set_pto(u32(d, 185 + 45));
                set_pth(u32(d, 189 + 45));
                set_psa(u8(d, 193 + 45));
                set_psi(u32(d, 194 + 45));
                set_pfm(u8(d, 198 + 45));
                set_pft(u16(d, 199 + 45));
                set_pfd(u16(d, 201 + 45));
                set_pfv(u16(d, 203 + 45));
                set_pdt(u16(d, 205 + 45));
                set_psm(u8(d, 207 + 45));
                set_psd(u16(d, 208 + 45));
                set_pst(u16(d, 210 + 45));
                set_psv(u16(d, 212 + 45));
                set_psw(u16(d, 214 + 45));
                set_ascent_start_time(s2000(d, 227 + 34));
                set_asn(u16(d, 231 + 34));
                set_arr(u16(d, 233 + 34));
                set_apm(u16(d, 235 + 34));
                set_apn(u16(d, 237 + 34));
                set_apf(u16(d, 239 + 34));
                set_asi(u32(d, 241 + 34));
                set_asd(u16(d, 245 + 34));
                set_add(u16(d, 247 + 34));
                set_adt(u16(d, 249 + 34));
                set_arc(u16(d, 251 + 34));
                set_ahr(u16(d, 253 + 34));
                set_ahd(u16(d, 255 + 34));
                set_surface_start_time(s2000(d, 271 + 20));
                set_sfn(u16(d, 275 + 20));
                set_sdc(i16(d, 277 + 20));
                set_sgt(u16(d, 279 + 20));
                set_set(u16(d, 281 + 20));
                set_sbm(u8(d, 283 + 20));
                set_sbt(u16(d, 284 + 20));
                set_sdt(u32(d, 286 + 20));
                set_ssa(u16(d, 290 + 20));
                set_ssi(u32(d, 292 + 20));
                set_sds(u16(d, 296 + 20));
                set_sam(u8(d, 298 + 20));
                set_sax(u16(d, 299 + 20));
                set_san(u16(d, 301 + 20));
                set_sao(u16(d, 303 + 20));
                set_sap(u16(d, 305 + 20));

                // TODO: Implement additional parameters.
                set_spt(XMSG_BAD_U16);

                break;

            case 0x03026100: // ALAMO Software version 0x03026100 (2016 NASCar floats).

                if(data.size() != 328) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 328 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                /* Config. - General: */

                set_msn(u16(d, 8));
                set_dtd(u16(d, 10));
                set_mds(u16(d, 12));
                set_apt(u8(d, 14));
                set_btm(u8(d, 15));
                set_xas(u8(d, 15 + 1));
                set_cps(u8(d, 16 + 1));
                set_cst(u8(d, 17 + 1));
                set_ctf(u16(d, 18 + 1));
                set_ctl(u16(d, 20 + 1));
                set_ctu(u16(d, 22 + 1));

                set_csp(u32(d, 18 + 6 + 1));
                cpb_[0] = u16(d, 22 + 7);
                cpb_[1] = u16(d, 24 + 7);
                cpb_[2] = u16(d, 26 + 7);
                cpn_[0] = u16(d, 28 + 7);
                cpn_[1] = u16(d, 30 + 7);
                cpn_[2] = u16(d, 32 + 7);
                cpp_[0] = u16(d, 34 + 7);
                cpp_[1] = u16(d, 36 + 7);
                cpp_[2] = u16(d, 38 + 7);
                set_mtd(u16(d, 40 + 6 + 1));
                set_mtt(u32(d, 42 + 6 + 1));
                set_mam(u8(d, 46 + 6 + 1));
                set_mat(u16(d, 47 + 6 + 1));
                set_aca(u16(d, 49 + 6 + 1));
                set_acg(u16(d, 51 + 6 + 1));
                set_acr(u16(d, 53 + 6 + 1));
                set_pnb(u16(d, 55 + 6 + 1));
                set_pnc(u16(d, 57 + 6 + 1));
                set_pxc(u16(d, 59 + 6 + 1));
                set_pbl(u16(d, 61 + 6 + 1));
                set_pbh(u16(d, 63 + 6 + 1));
                set_pp0(u16(d, 65 + 6 + 1));
                set_pp1(u16(d, 67 + 6 + 1));
                set_pvl(u16(d, 69 + 6 + 1));
                set_pvh(u16(d, 71 + 6 + 1));
                set_pv0(u16(d, 73 + 6 + 1));
                set_pv5(u16(d, 75 + 6 + 1));
                set_pc0(u16(d, 77 + 6 + 1));
                set_pc1(u16(d, 79 + 6 + 1));

                set_icm(u8(d, 87 + 1));
                set_imm(u16(d, 88 + 1));
                set_ica0(u16(d, 90 + 1));
                set_ica1(u16(d, 92 + 1));
                set_icb0(u16(d, 94 + 1));
                set_icb1(u16(d, 96 + 1));
                set_imt0(u16(d, 98 + 1));
                set_imt1(u16(d, 100 + 1));
                set_ilt0(u16(d, 102 + 1));
                set_ilt1(u16(d, 104 + 1));
                set_icn(u16(d, 106 + 1));
                set_icx(u16(d, 108 + 1));
                set_ict(i16(d, 110 + 1));
                set_ics(u16(d, 112 + 1));
                set_iss0(u8(d, 114 + 1));
                set_iss1(u8(d, 115 + 1));
                set_ist(u16(d, 116 + 1 + 1));
                set_iso(u16(d, 118 + 1 + 1));
                set_isp(u8(d, 120 + 1 + 1));
                set_iue0(u8(d, 121 + 1 + 1));
                set_iue1(u8(d, 122 + 1 + 1));
                set_icd0(u16(d, 123 + 1 + 1));
                set_icd1(u16(d, 125 + 1 + 1));
                set_icr0(u32(d, 127 + 1 + 1));
                set_icr1(u32(d, 131 + 1 + 1));

                set_icg0(u8(d, 135 + 1 + 1));
                set_icg1(u8(d, 136 + 1 + 1));
                set_ico0(u16(d, 137 + 10 + 1));
                set_ico1(u16(d, 139 + 10 + 1));
                set_icf(u8(d, 141 + 10 + 1));
                set_icw(u8(d, 145 + 10 + 1));

                set_descend_start_time(s2000(d, 160));
                set_dns(u16(d, 103 + 61));
                set_dra(u16(d, 105 + 61));
                set_dsd(u16(d, 107 + 61));
                set_dst(u16(d, 109 + 61));
                set_ddf(u16(d, 111 + 61));
                set_ddc(u16(d, 113 + 61));
                set_dsi(u32(d, 115 + 61));
                set_ddd(u16(d, 119 + 61));
                set_ddt(u16(d, 121));
                set_dft(u16(d, 123 + 61));
                set_dcm(u8(d, 125 + 61));
                set_dal(u8(d, 126 + 61));
                set_dct(u16(d, 127 + 61));
                set_dca(u8(d, 129 + 61));
                set_drc(u16(d, 130 + 61));
                set_drt(u16(d, 132 + 61));
                set_dcx(u16(d, 134 + 61));
                set_dcp(u16(d, 136 + 61));
                set_dcf(u16(d, 138 + 61));
                set_dcd(u16(d, 140 + 61));
                set_dcr(u16(d, 142 + 61));
                set_dsw(u16(d, 144 + 61));
                set_duc(u16(d, 146 + 61));
                set_dpt(u16(d, 148 + 61));
                set_dut(u16(d, 150 + 61));
                set_dpd(u16(d, 152 + 61));
                set_dvt(u16(d, 154 + 61));
                set_pfr(u16(d, 156 + 61));
                set_pfc(u16(d, 158 + 61));
                set_ddp(u16(d, 160 + 61));
                set_dds(u16(d, 162 + 61));
                set_park_start_time(s2000(d, 179 + 46));
                set_pns(u16(d, 183 + 46));
                set_pto(u32(d, 185 + 46));
                set_pth(u32(d, 189 + 46));
                set_psa(u8(d, 193 + 46));
                set_psi(u32(d, 194 + 46));
                set_pfm(u8(d, 198 + 46));
                set_pft(u16(d, 199 + 46));
                set_pfd(u16(d, 201 + 46));
                set_pfv(u16(d, 203 + 46));
                set_pdt(u16(d, 205 + 46));
                set_psm(u8(d, 207 + 46));
                set_psd(u16(d, 208 + 46));
                set_pst(u16(d, 210 + 46));
                set_psv(u16(d, 212 + 46));
                set_psw(u16(d, 214 + 46));
                set_ascent_start_time(s2000(d, 227 + 35));
                set_asn(u16(d, 231 + 35));
                set_arr(u16(d, 233 + 35));
                set_apm(u16(d, 235 + 35));
                set_apn(u16(d, 237 + 35));
                set_apf(u16(d, 239 + 35));
                set_asi(u32(d, 241 + 35));
                set_asd(u16(d, 245 + 35));
                set_add(u16(d, 247 + 35));
                set_adt(u16(d, 249 + 35));
                set_arc(u16(d, 251 + 35));
                set_ahr(u16(d, 253 + 35));
                set_ahd(u16(d, 255 + 35));
                set_surface_start_time(s2000(d, 271 + 21));
                set_sfn(u16(d, 275 + 21));
                set_sdc(i16(d, 277 + 21));
                set_sgt(u16(d, 279 + 21));
                set_set(u16(d, 281 + 21));
                set_sbm(u8(d, 283 + 21));
                set_sbt(u16(d, 284 + 21));
                set_sdt(u32(d, 286 + 21));
                set_ssa(u16(d, 290 + 21));
                set_ssi(u32(d, 292 + 21));
                set_sds(u16(d, 296 + 21));
                set_sam(u8(d, 298 + 21));
                set_sax(u16(d, 299 + 21));
                set_san(u16(d, 301 + 21));
                set_sao(u16(d, 303 + 21));
                set_sap(XMSG_BAD_U16);

                // TODO: Implement additional parameters.
                set_spt(XMSG_BAD_U16);

                break;

            case 0x04016100: // ALAMO Software version 0x04016100 (2016 NASCar floats).

                if(data.size() != 328) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 328 bytes, but received "
                       << data.size()
                       << " bytes!";
                    EXC_THROW(os.str());
                }

                /* Config. - General: */

                set_msn(u16(d, 8));
                set_dtd(u16(d, 10));
                set_mds(u16(d, 12));
                set_apt(u8(d, 14));
                set_btm(u8(d, 15));
                set_xas(u8(d, 15 + 1));
                set_cps(u8(d, 16 + 1));
                set_cst(u8(d, 17 + 1));
                set_ctf(u16(d, 18 + 1));
                set_ctl(u16(d, 20 + 1));
                set_ctu(u16(d, 22 + 1));

                set_csp(u32(d, 18 + 6 + 1));
                cpb_[0] = u16(d, 22 + 7);
                cpb_[1] = u16(d, 24 + 7);
                cpb_[2] = u16(d, 26 + 7);
                cpn_[0] = u16(d, 28 + 7);
                cpn_[1] = u16(d, 30 + 7);
                cpn_[2] = u16(d, 32 + 7);
                cpp_[0] = u16(d, 34 + 7);
                cpp_[1] = u16(d, 36 + 7);
                cpp_[2] = u16(d, 38 + 7);
                set_mtd(u16(d, 40 + 6 + 1));
                set_mtt(u32(d, 42 + 6 + 1));
                set_mam(u8(d, 46 + 6 + 1));
                set_mat(u16(d, 47 + 6 + 1));
                set_aca(u16(d, 49 + 6 + 1));
                set_acg(u16(d, 51 + 6 + 1));
                set_acr(u16(d, 53 + 6 + 1));
                set_pnb(u16(d, 55 + 6 + 1));
                set_pnc(u16(d, 57 + 6 + 1));
                set_pxc(u16(d, 59 + 6 + 1));
                set_pbl(u16(d, 61 + 6 + 1));
                set_pbh(u16(d, 63 + 6 + 1));
                set_pp0(u16(d, 65 + 6 + 1));
                set_pp1(u16(d, 67 + 6 + 1));
                set_pvl(u16(d, 69 + 6 + 1));
                set_pvh(u16(d, 71 + 6 + 1));
                set_pv0(u16(d, 73 + 6 + 1));
                set_pv5(u16(d, 75 + 6 + 1));
                set_pc0(u16(d, 77 + 6 + 1));
                set_pc1(u16(d, 79 + 6 + 1));

                set_icm(u8(d, 87 + 1));
                set_imm(u16(d, 88 + 1));
                set_ica0(u16(d, 90 + 1));
                set_ica1(u16(d, 92 + 1));
                set_icb0(u16(d, 94 + 1));
                set_icb1(u16(d, 96 + 1));
                set_imt0(u16(d, 98 + 1));
                set_imt1(u16(d, 100 + 1));
                set_ilt0(u16(d, 102 + 1));
                set_ilt1(u16(d, 104 + 1));
                set_icn(u16(d, 106 + 1));
                set_icx(u16(d, 108 + 1));
                set_ict(i16(d, 110 + 1));
                set_ics(u16(d, 112 + 1));
                set_iss0(u8(d, 114 + 1));
                set_iss1(u8(d, 115 + 1));
                set_ist(u16(d, 116 + 1 + 1));
                set_iso(u16(d, 118 + 1 + 1));
                set_isp(u8(d, 120 + 1 + 1));
                set_iue0(u8(d, 121 + 1 + 1));
                set_iue1(u8(d, 122 + 1 + 1));
                set_icd0(u16(d, 123 + 1 + 1));
                set_icd1(u16(d, 125 + 1 + 1));
                set_icr0(u32(d, 127 + 1 + 1));
                set_icr1(u32(d, 131 + 1 + 1));

                set_icg0(u8(d, 135 + 1 + 1));
                set_icg1(u8(d, 136 + 1 + 1));
                set_ico0(u16(d, 137 + 10 + 1));
                set_ico1(u16(d, 139 + 10 + 1));
                set_icf(u8(d, 141 + 10 + 1));
                set_icw(u8(d, 145 + 10 + 1));

                set_descend_start_time(s2000(d, 160));
                set_dns(u16(d, 103 + 61));
                set_dra(u16(d, 105 + 61));
                set_dsd(u16(d, 107 + 61));
                set_dst(u16(d, 109 + 61));
                set_ddf(u16(d, 111 + 61));
                set_ddc(u16(d, 113 + 61));
                set_dsi(u32(d, 115 + 61));
                set_ddd(u16(d, 119 + 61));
                set_ddt(u16(d, 121));
                set_dft(u16(d, 123 + 61));
                set_dcm(u8(d, 125 + 61));
                set_dal(u8(d, 126 + 61));
                set_dct(u16(d, 127 + 61));
                set_dca(u8(d, 129 + 61));
                set_drc(u16(d, 130 + 61));
                set_drt(u16(d, 132 + 61));
                set_dcx(u16(d, 134 + 61));
                set_dcp(u16(d, 136 + 61));
                set_dcf(u16(d, 138 + 61));
                set_dcd(u16(d, 140 + 61));
                set_dcr(u16(d, 142 + 61));
                set_dsw(u16(d, 144 + 61));
                set_duc(u16(d, 146 + 61));
                set_dpt(u16(d, 148 + 61));
                set_dut(u16(d, 150 + 61));
                set_dpd(u16(d, 152 + 61));
                set_dvt(u16(d, 154 + 61));
                set_pfr(u16(d, 156 + 61));
                set_pfc(u16(d, 158 + 61));
                set_ddp(u16(d, 160 + 61));
                set_dds(u16(d, 162 + 61));
                set_park_start_time(s2000(d, 179 + 46));
                set_pns(u16(d, 183 + 46));
                set_pto(u32(d, 185 + 46));
                set_pth(u32(d, 189 + 46));
                set_psa(u8(d, 193 + 46));
                set_psi(u32(d, 194 + 46));
                set_pfm(u8(d, 198 + 46));
                set_pft(u16(d, 199 + 46));
                set_pfd(u16(d, 201 + 46));
                set_pfv(u16(d, 203 + 46));
                set_pdt(u16(d, 205 + 46));
                set_psm(u8(d, 207 + 46));
                set_psd(u16(d, 208 + 46));
                set_pst(u16(d, 210 + 46));
                set_psv(u16(d, 212 + 46));
                set_psw(u16(d, 214 + 46));
                set_ascent_start_time(s2000(d, 227 + 35));
                set_asn(u16(d, 231 + 35));
                set_arr(u16(d, 233 + 35));
                set_apm(u16(d, 235 + 35));
                set_apn(u16(d, 237 + 35));
                set_apf(u16(d, 239 + 35));
                set_asi(u32(d, 241 + 35));
                set_asd(u16(d, 245 + 35));
                set_add(u16(d, 247 + 35));
                set_adt(u16(d, 249 + 35));
                set_arc(u16(d, 251 + 35));
                set_ahr(u16(d, 253 + 35));
                set_ahd(u16(d, 255 + 35));
                set_surface_start_time(s2000(d, 271 + 21));
                set_sfn(u16(d, 275 + 21));
                set_sdc(i16(d, 277 + 21));
                set_sgt(u16(d, 279 + 21));
                set_set(u16(d, 281 + 21));
                set_sbm(u8(d, 283 + 21));
                set_sbt(u16(d, 284 + 21));
                set_sdt(u32(d, 286 + 21));
                set_ssa(u16(d, 290 + 21));
                set_ssi(u32(d, 292 + 21));
                set_sds(u16(d, 296 + 21));
                set_sam(u8(d, 298 + 21));
                set_sax(u16(d, 299 + 21));
                set_san(u16(d, 301 + 21));
                set_sao(u16(d, 303 + 21));
                set_sap(XMSG_BAD_U16);

                // TODO: Implement additional parameters.
                set_spt(XMSG_BAD_U16);

                break;

            case 0x04026200: // PAR floats


                if(data.size() != 323) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 323 bytes, but received "
                       << data.size()
                       << " bytes!";
#                   if defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG
                    {
                        lwrn() << os.str();
                    }
#                   else
                    {
                        EXC_THROW(os.str());
                    }
#                   endif // defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG
                }

                /* Config. - General: */

                set_msg_version(u8(d, 8));
                set_sensor_type(u8(d, 9));
                set_msn(u16(d, 10));
                set_dtd(u16(d, 12));
                set_mds(u16(d, 14));
                set_apt(u8(d, 16));
                set_btm(u8(d, 17));
                set_xas(u8(d, 18));
                set_cps(u8(d, 19));
                set_cst(u8(d, 20));
                set_ctf(u16(d, 21));
                set_ctl(u16(d, 23));
                set_ctu(u16(d, 25));

                set_csp(u32(d, 27));
                cpb_[0] = u16(d, 31);
                cpb_[1] = u16(d, 33);
                cpb_[2] = u16(d, 35);
                cpn_[0] = u16(d, 37);
                cpn_[1] = u16(d, 39);
                cpn_[2] = u16(d, 41);
                cpp_[0] = u16(d, 43);
                cpp_[1] = u16(d, 45);
                cpp_[2] = u16(d, 47);
                set_mtd(u16(d, 49));
                set_mtt(u32(d, 51));
                set_mam(u8(d, 55));
                set_mat(u16(d, 56));
                set_aca(u16(d, 58));
                set_acg(u16(d, 60));
                set_acr(u16(d, 62));
                set_pnb(u16(d, 64));
                set_pnc(u16(d, 66));
                set_pxc(u16(d, 68));
                set_pbl(u16(d, 70));
                set_pbh(u16(d, 72));
                set_pp0(u16(d, 74));
                set_pp1(u16(d, 76));
                set_pvl(u16(d, 78));
                set_pvh(u16(d, 80));
                set_pv0(u16(d, 82));
                set_pv5(u16(d, 84));
                set_pc0(u16(d, 86));
                set_pc1(u16(d, 88));

                set_icm(u8(d, 90));
                set_imm(u16(d, 91));
                set_ica0(u16(d, 93));
                set_ica1(u16(d, 95));
                set_icb0(u16(d, 97));
                set_icb1(u16(d, 99));
                set_imt0(u16(d, 101));
                set_imt1(u16(d, 103));
                set_ilt0(u16(d, 105));
                set_ilt1(u16(d, 107));
                set_icn(u16(d, 109));
                set_icx(u16(d, 111));
                set_ict(i16(d, 113));
                set_ics(u16(d, 115));
                set_iss0(u8(d, 117));
                set_iss1(u8(d, 118));


                set_ist(u16(d, 119+ 1));
                set_iso(u16(d, 121+ 1));
                set_isp(u8(d, 123+ 1));
                set_iue0(u8(d, 124+ 1));
                set_iue1(u8(d, 125+ 1));
                set_icd0(u16(d, 126+ 1));
                set_icd1(u16(d, 128+ 1));
                set_icr0(u32(d, 130+ 1));
                set_icr1(u32(d, 134+ 1));
                set_icg0(u8(d, 138+ 1));
                set_icg1(u8(d, 139+ 1));
                set_isa(u8(d, 140+ 1));
                set_isd(u16(d, 141+ 1));
                set_iat(u16(d, 143+ 1));
                set_isv(u16(d, 145+ 1));
                set_isw(u16(d, 147+ 1));
                set_ico0(u16(d, 149+ 1));
                set_ico1(u16(d, 151+ 1));
                set_icf(u32(d, 153+ 1));
                set_icw(u32(d, 157+ 1));

                /* Descent state configuration: */

                set_descend_start_time(bpt::not_a_date_time);
                set_dns(u16(d, 161+ 1));
                set_dra(u16(d, 163+ 1));
                set_dsd(u16(d, 165+ 1));
                set_dst(u16(d, 167+ 1));
                set_ddf(u16(d, 169+ 1));
                set_ddc(u16(d, 171+ 1));
                set_dsi(u32(d, 173+ 1));
                set_ddd(u16(d, 177+ 1));
                set_ddt(u16(d, 179+ 1));
                set_dft(u16(d, 181+ 1));
                set_dcm(u8(d, 183+ 1));
                set_dal(u8(d, 184+ 1));
                set_dct(u16(d, 185+ 1));
                set_dca(u8(d, 187+ 1));
                set_drc(u16(d, 188+ 1));
                set_drt(u16(d, 190+ 1));
                set_dcx(u16(d, 192+ 1));
                set_dcp(u16(d, 194+ 1));
                set_dcf(u16(d, 196+ 1));
                set_dcd(u16(d, 198+ 1));
                set_dcr(u16(d, 200+ 1));
                set_dsw(u16(d, 202+ 1));
                set_duc(u16(d, 204+ 1));
                set_dpt(u16(d, 206+ 1));
                set_dut(u16(d, 208+ 1));
                set_dpd(u16(d, 210+ 1));
                set_dvt(u16(d, 212+ 1));

                set_dvd(u16(d, 214+ 1));
                set_dvw(u16(d, 216+ 1));


                set_pfr(u16(d, 218+ 1));
                set_pfc(u16(d, 220+ 1));
                set_ddp(u16(d, 222+ 1));
                set_dds(u16(d, 224+ 1));
                set_park_start_time(bpt::not_a_date_time);
                set_pns(u16(d, 226+ 1));
                set_pto(u32(d, 228+ 1));
                set_pth(u32(d, 232+ 1));
                set_psa(u8(d, 236+ 1));
                set_psi(u32(d, 237+ 1));
                set_pfm(u8(d, 241+ 1));
                set_pft(u16(d, 242+ 1));
                set_pfd(u16(d, 244+ 1));
                set_pfv(u16(d, 246+ 1));
                set_pdt(u16(d, 248+ 1));
                set_psm(u8(d, 250+ 1));
                set_psd(u16(d, 251+ 1));
                set_pst(u16(d, 253+ 1));
                set_psv(u16(d, 255+ 1));
                set_psw(u16(d, 257+ 1));
                set_ascent_start_time(bpt::not_a_date_time);
                set_asn(u16(d, 259+ 1));
                set_arr(u16(d, 261+ 1));
                set_apm(u16(d, 263+ 1));
                set_apn(u16(d, 265+ 1));
                set_apf(u16(d, 267+ 1));
                set_asi(u32(d, 269+ 1));
                set_asd(u16(d, 273+ 1));
                set_add(u16(d, 275+ 1));
                set_adt(u16(d, 277+ 1));
                set_arc(u16(d, 279+ 1));
                set_ahr(u16(d, 281+ 1));
                set_ahd(u16(d, 283+ 1));
                set_surface_start_time(bpt::not_a_date_time);
                set_sfn(u16(d, 285+ 1));
                set_sdc(i16(d, 287+ 1));
                set_sgt(u16(d, 289+ 1));
                set_set(u16(d, 291+ 1));
                set_sbm(u8(d, 293+ 1));
                set_sbt(u16(d, 294+ 1));
                set_sdt(u32(d, 296+ 1));
                set_ssa(u16(d, 300+ 1));
                set_ssi(u32(d, 302+ 1));
                set_sds(u16(d, 306+ 1));
                set_sam(u8(d, 308+ 1));
                set_sax(u16(d, 309+ 1));
                set_san(u16(d, 311+ 1));
                set_sao(u16(d, 313+ 1));
                set_sap(u16(d, 315+ 1));
                set_spt(u16(d, 317+ 1));
                set_ssp(u16(d, 319+ 1));

                break;

            case 0x04036100: // ALAMO Software version 0x04036100 (2017 S2h test float 10108).

                if(data.size() != 327) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 327 bytes, but received "
                       << data.size()
                       << " bytes!";
#                   if defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG
                    lwrn() << os.str();
#                   else
                    EXC_THROW(os.str());
#                   endif // defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG
                }

                /* Config. - General: */

                set_msg_version(u8(d, 8));
                set_sensor_type(u8(d, 9));
                set_msn(u16(d, 10));
                set_dtd(u16(d, 12));
                set_mds(u16(d, 14));
                set_apt(u8(d, 16));
                set_btm(u8(d, 17));
                set_xas(u8(d, 18));
                set_cps(u8(d, 19));
                set_cst(u8(d, 20));
                set_ctf(u16(d, 21));
                set_ctl(u16(d, 23));
                set_ctu(u16(d, 25));

                set_csp(u32(d, 27));
                cpb_[0] = u16(d, 31);
                cpb_[1] = u16(d, 33);
                cpb_[2] = u16(d, 35);
                cpn_[0] = u16(d, 37);
                cpn_[1] = u16(d, 39);
                cpn_[2] = u16(d, 41);
                cpp_[0] = u16(d, 43);
                cpp_[1] = u16(d, 45);
                cpp_[2] = u16(d, 47);
                set_mtd(u16(d, 49 + 4));
                set_mtt(u32(d, 51 + 4));
                set_mam(u8(d, 55 + 4));
                set_mat(u16(d, 56 + 4));
                set_aca(u16(d, 58 + 4));
                set_acg(u16(d, 60 + 4));
                set_acr(u16(d, 62 + 4));
                set_pnb(u16(d, 64 + 4));
                set_pnc(u16(d, 66 + 4));
                set_pxc(u16(d, 68 + 4));
                set_pbl(u16(d, 70 + 4));
                set_pbh(u16(d, 72 + 4));
                set_pp0(u16(d, 74 + 4));
                set_pp1(u16(d, 76 + 4));
                set_pvl(u16(d, 78 + 4));
                set_pvh(u16(d, 80 + 4));
                set_pv0(u16(d, 82 + 4));
                set_pv5(u16(d, 84 + 4));
                set_pc0(u16(d, 86 + 4));
                set_pc1(u16(d, 88 + 4));

                set_icm(u8(d, 90 + 4));
                set_imm(u16(d, 91 + 4));
                set_ica0(u16(d, 93 + 4));
                set_ica1(u16(d, 95 + 4));
                set_icb0(u16(d, 97 + 4));
                set_icb1(u16(d, 99 + 4));
                set_imt0(u16(d, 101 + 4));
                set_imt1(u16(d, 103 + 4));
                set_ilt0(u16(d, 105 + 4));
                set_ilt1(u16(d, 107 + 4));
                set_icn(u16(d, 109 + 4));
                set_icx(u16(d, 111 + 4));
                set_ict(i16(d, 113 + 4));
                set_ics(u16(d, 115 + 4));
                set_iss0(u8(d, 117 + 4));
                set_iss1(u8(d, 118 + 4));
                set_ist(u16(d, 119 + 5));
                set_iso(u16(d, 121 + 5));
                set_isp(u8(d, 123 + 5));
                set_iue0(u8(d, 124 + 5));
                set_iue1(u8(d, 125 + 5));
                set_icd0(u16(d, 126 + 5));
                set_icd1(u16(d, 128 + 5));
                set_icr0(u32(d, 130 + 5));
                set_icr1(u32(d, 134 + 5));
                set_icg0(u8(d, 138 + 5));
                set_icg1(u8(d, 139 + 5));
                set_isa(u8(d, 140 + 5));
                set_isd(u16(d, 141 + 5));
                set_iat(u16(d, 143 + 5));
                set_isv(u16(d, 145 + 5));
                set_isw(u16(d, 147 + 5));
                set_ico0(u16(d, 149 + 5));
                set_ico1(u16(d, 151 + 5));
                set_icf(u32(d, 153 + 5));
                set_icw(u32(d, 157 + 5));

                /* Descent state configuration: */

                set_descend_start_time(bpt::not_a_date_time);
                set_dns(u16(d, 161 + 5));
                set_dra(u16(d, 163 + 5));
                set_dsd(u16(d, 165 + 5));
                set_dst(u16(d, 167 + 5));
                set_ddf(u16(d, 169 + 5));
                set_ddc(u16(d, 171 + 5));
                set_dsi(u32(d, 173 + 5));
                set_ddd(u16(d, 177 + 5));
                set_ddt(u16(d, 179 + 5));
                set_dft(u16(d, 181 + 5));
                set_dcm(u8(d, 183 + 5));
                set_dal(u8(d, 184 + 5));
                set_dct(u16(d, 185 + 5));
                set_dca(u8(d, 187 + 5));
                set_drc(u16(d, 188 + 5));
                set_drt(u16(d, 190 + 5));
                set_dcx(u16(d, 192 + 5));
                set_dcp(u16(d, 194 + 5));
                set_dcf(u16(d, 196 + 5));
                set_dcd(u16(d, 198 + 5));
                set_dcr(u16(d, 200 + 5));
                set_dsw(u16(d, 202 + 5));
                set_duc(u16(d, 204 + 5));
                set_dpt(u16(d, 206 + 5));
                set_dut(u16(d, 208 + 5));
                set_dpd(u16(d, 210 + 5));
                set_dvt(u16(d, 212 + 5));

                set_dvd(u16(d, 214 + 5));
                set_dvw(u16(d, 216 + 5));


                set_pfr(u16(d, 218 + 5));
                set_pfc(u16(d, 220 + 5));
                set_ddp(u16(d, 222 + 5));
                set_dds(u16(d, 224 + 5));
                set_park_start_time(bpt::not_a_date_time);
                set_pns(u16(d, 226 + 5));
                set_pto(u32(d, 228 + 5));
                set_pth(u32(d, 232 + 5));
                set_psa(u8(d, 236 + 5));
                set_psi(u32(d, 237 + 5));
                set_pfm(u8(d, 241 + 5));
                set_pft(u16(d, 242 + 5));
                set_pfd(u16(d, 244 + 5));
                set_pfv(u16(d, 246 + 5));
                set_pdt(u16(d, 248 + 5));
                set_psm(u8(d, 250 + 5));
                set_psd(u16(d, 251 + 5));
                set_pst(u16(d, 253 + 5));
                set_psv(u16(d, 255 + 5));
                set_psw(u16(d, 257 + 5));
                set_ascent_start_time(bpt::not_a_date_time);
                set_asn(u16(d, 259 + 5));
                set_arr(u16(d, 261 + 5));
                set_apm(u16(d, 263 + 5));
                set_apn(u16(d, 265 + 5));
                set_apf(u16(d, 267 + 5));
                set_asi(u32(d, 269 + 5));
                set_asd(u16(d, 273 + 5));
                set_add(u16(d, 275 + 5));
                set_adt(u16(d, 277 + 5));
                set_arc(u16(d, 279 + 5));
                set_ahr(u16(d, 281 + 5));
                set_ahd(u16(d, 283 + 5));
                set_surface_start_time(bpt::not_a_date_time);
                set_sfn(u16(d, 285 + 5));
                set_sdc(i16(d, 287 + 5));
                set_sgt(u16(d, 289 + 5));
                set_set(u16(d, 291 + 5));
                set_sbm(u8(d, 293 + 5));
                set_sbt(u16(d, 294 + 5));
                set_sdt(u32(d, 296 + 5));
                set_ssa(u16(d, 300 + 5));
                set_ssi(u32(d, 302 + 5));
                set_sds(u16(d, 306 + 5));
                set_sam(u8(d, 308 + 5));
                set_sax(u16(d, 309 + 5));
                set_san(u16(d, 311 + 5));
                set_sao(u16(d, 313 + 5));
                set_sap(u16(d, 315 + 5));
                set_spt(u16(d, 317 + 5));
                set_ssp(u16(d, 319 + 5));

                break;

            case 0x04046100: // ALAMO Software version 0x04046100 (2017 Hurricane Floats).

                if(data.size() != 328) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 328 bytes, but received "
                       << data.size()
                       << " bytes!";
#                   if defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG
                    lwrn() << os.str();
#                   else
                    EXC_THROW(os.str());
#                   endif // defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG
                }

                /* Config. - General: */

                set_msg_version(u8(d, 8));
                set_sensor_type(u8(d, 9));
                set_msn(u16(d, 10));
                set_dtd(u16(d, 12));
                set_mds(u16(d, 14));
                set_apt(u8(d, 16));
                set_btm(u8(d, 17));
                set_xas(u8(d, 18));
                set_cps(u8(d, 19));
                set_cst(u8(d, 20));
                set_ctf(u16(d, 21));
                set_ctl(u16(d, 23));
                set_ctu(u16(d, 25));
                // set_ctc(u8(d, 27))

                set_csp(u32(d, 27 + 1));
                set_cpb1(u16(d, 31 + 1));
                set_cpb2(u16(d, 33 + 1));
                set_cpb3(u16(d, 35 + 1));
                set_cpn1(u16(d, 37 + 1));
                set_cpn2(u16(d, 39 + 1));
                set_cpn3(u16(d, 41 + 1));
                set_cpp1(u16(d, 43 + 1));
                set_cpp2(u16(d, 45 + 1));
                set_cpp3(u16(d, 47 + 1));
                // set_crd
                // set_crs


                set_mtd(u16(d, 49 + 5));  // good
                set_mtt(u32(d, 51 + 5));
                set_mam(u8(d, 55 + 5));
                set_mat(u16(d, 56 + 5));
//                set_mat2(u16(d, 56 + 5));
//                set_map(u16(d, 56 + 5));
                set_aca(u16(d, 58 + 5));
                set_acg(u16(d, 60 + 5));
                set_acr(u16(d, 62 + 5));
                set_pnb(u16(d, 64 + 5));
                set_pnc(u16(d, 66 + 5));
                set_pxc(u16(d, 68 + 5));
                set_pbl(u16(d, 70 + 5));
                set_pbh(u16(d, 72 + 5));
                set_pp0(u16(d, 74 + 5));
                set_pp1(u16(d, 76 + 5));
                set_pvl(u16(d, 78 + 5));
                set_pvh(u16(d, 80 + 5));
                set_pv0(u16(d, 82 + 5));
                set_pv5(u16(d, 84 + 5));
                set_pc0(u16(d, 86 + 5));
                set_pc1(u16(d, 88 + 5));

                /* Ice Configuration */

                set_icm(u8(d, 90 + 5));
                set_imm(u16(d, 91 + 5));
                set_ica0(u16(d, 93 + 5));
                set_ica1(u16(d, 95 + 5));
                set_icb0(u16(d, 97 + 5));
                set_icb1(u16(d, 99 + 5));
                set_imt0(u16(d, 101 + 5));
                set_imt1(u16(d, 103 + 5));
                set_ilt0(u16(d, 105 + 5));
                set_ilt1(u16(d, 107 + 5));
                set_icn(u16(d, 109 + 5));
                set_icx(u16(d, 111 + 5));
                set_ict(i16(d, 113 + 5));
                set_ics(u16(d, 115 + 5));
                set_iss0(u8(d, 117 + 5));
                set_iss1(u8(d, 118 + 5));
                set_ist(u16(d, 119 + 6));
                set_iso(u16(d, 121 + 6));
                set_isp(u8(d, 123 + 6));
                set_iue0(u8(d, 124 + 6));
                set_iue1(u8(d, 125 + 6));
                set_icd0(u16(d, 126 + 6));
                set_icd1(u16(d, 128 + 6));
                set_icr0(u32(d, 130 + 6));
                set_icr1(u32(d, 134 + 6));
                set_icg0(u8(d, 138 + 6));
                set_icg1(u8(d, 139 + 6));
                set_isa(u8(d, 140 + 6));
                set_isd(u16(d, 141 + 6));
                set_iat(u16(d, 143 + 6));
                set_isv(u16(d, 145 + 6));
                set_isw(u16(d, 147 + 6));
                set_ico0(u16(d, 149 + 6));
                set_ico1(u16(d, 151 + 6));
                set_icf(u32(d, 153 + 6));
                set_icw(u32(d, 157 + 6));

                /* Descent state configuration: */

                set_descend_start_time(bpt::not_a_date_time);
                set_dns(u16(d, 161 + 6));
                set_dra(u16(d, 163 + 6));
                set_dsd(u16(d, 165 + 6));
                set_dst(u16(d, 167 + 6));
                set_ddf(u16(d, 169 + 6));
                set_ddc(u16(d, 171 + 6));
                set_dsi(u32(d, 173 + 6));
                set_ddd(u16(d, 177 + 6));
                set_ddt(u16(d, 179 + 6));
                set_dft(u16(d, 181 + 6));
                set_dcm(u8(d, 183 + 6));
                set_dal(u8(d, 184 + 6));
                set_dct(u16(d, 185 + 6));
                set_dca(u8(d, 187 + 6));
                set_drc(u16(d, 188 + 6));
                set_drt(u16(d, 190 + 6));
                set_dcx(u16(d, 192 + 6));
                set_dcp(u16(d, 194 + 6));
                set_dcf(u16(d, 196 + 6));
                set_dcd(u16(d, 198 + 6));
                set_dcr(u16(d, 200 + 6));
                set_dsw(u16(d, 202 + 6));
                set_duc(u16(d, 204 + 6));
                set_dpt(u16(d, 206 + 6));
                set_dut(u16(d, 208 + 6));
                set_dpd(u16(d, 210 + 6));
                set_dvt(u16(d, 212 + 6));

                set_dvd(u16(d, 214 + 6));
                set_dvw(u16(d, 216 + 6));


                set_pfr(u16(d, 218 + 6));
                set_pfc(u16(d, 220 + 6));
                set_ddp(u16(d, 222 + 6));
                set_dds(u16(d, 224 + 6));
                set_park_start_time(bpt::not_a_date_time);
                set_pns(u16(d, 226 + 6));
                set_pto(u32(d, 228 + 6));
                set_pth(u32(d, 232 + 6));
                // set_ptd(u32(d, 236 + 10))
                set_psa(u8(d, 236 + 6));
                set_psi(u32(d, 237 + 6));
                set_pfm(u8(d, 241 + 6));
                set_pft(u16(d, 242 + 6));
                set_pfd(u16(d, 244 + 6));
                set_pfv(u16(d, 246 + 6));
                set_pdt(u16(d, 248 + 6));
                set_psm(u8(d, 250 + 6));
                set_psd(u16(d, 251 + 6));
                set_pst(u16(d, 253 + 6));
                set_psv(u16(d, 255 + 6));
                set_psw(u16(d, 257 + 6));
                set_ascent_start_time(bpt::not_a_date_time);
                set_asn(u16(d, 259 + 6));
                set_arr(u16(d, 261 + 6));
                set_apm(u16(d, 263 + 6));
                set_apn(u16(d, 265 + 6));
                set_apf(u16(d, 267 + 6));
                set_asi(u32(d, 269 + 6));
                set_asd(u16(d, 273 + 6));
                set_add(u16(d, 275 + 6));
                set_adt(u16(d, 277 + 6));
                set_arc(u16(d, 279 + 6));
                set_ahr(u16(d, 281 + 6));
                set_ahd(u16(d, 283 + 6));
                set_surface_start_time(bpt::not_a_date_time);
                set_sfn(u16(d, 285 + 6));
                set_sdc(i16(d, 287 + 6));
                set_sgt(u16(d, 289 + 6));
                set_set(u16(d, 291 + 6));
                set_sbm(u8(d, 293 + 6));
                set_sbt(u16(d, 294 + 6));
                set_sdt(u32(d, 296 + 6));
                set_ssa(u16(d, 300 + 6));
                set_ssi(u32(d, 302 + 6));
                set_sds(u16(d, 306 + 6));
                set_sam(u8(d, 308 + 6));
                set_sax(u16(d, 309 + 6));
                set_san(u16(d, 311 + 6));
                set_sao(u16(d, 313 + 6));
                set_sap(u16(d, 315 + 6));
                set_spt(u16(d, 317 + 6));
                set_ssp(u16(d, 319 + 6));

                break;

            case 0x04056300: // ALAMO Software version 0x04056300 (2017 S2h test float 10108).

                if(data.size() != 328) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 328 bytes, but received "
                       << data.size()
                       << " bytes!";
#                   if defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG
                    lwrn() << os.str();
#                   else
                    EXC_THROW(os.str());
#                   endif // defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG
                }

                /* General Configuration: */

                set_msg_version(u8(d, 8));
                set_sensor_type(u8(d, 9));
                set_msn(u16(d, 10));
                set_dtd(u16(d, 12));
                set_mds(u16(d, 14));
                set_apt(u8(d, 16));
                set_btm(u8(d, 17));
                set_xas(u8(d, 18));
                set_cps(u8(d, 19));
                set_cst(u8(d, 20));
                set_ctf(u16(d, 21));
                set_ctl(u16(d, 23));
                set_ctu(u16(d, 25));
                // set_ctc(u8(d, 27))

                set_csp(u32(d, 27 + 1));
                set_cpb1(u16(d, 31 + 1));
                set_cpb2(u16(d, 33 + 1));
                set_cpb3(u16(d, 35 + 1));
                set_cpn1(u16(d, 37 + 1));
                set_cpn2(u16(d, 39 + 1));
                set_cpn3(u16(d, 41 + 1));
                set_cpp1(u16(d, 43 + 1));
                set_cpp2(u16(d, 45 + 1));
                set_cpp3(u16(d, 47 + 1));
                // set_crd
                // set_crs


                set_mtd(u16(d, 49 + 5));
                set_mtt(u32(d, 51 + 5));
                set_mam(u8(d, 55 + 5));
//                set_mat1(u16(d, 56 + 5));
//                set_mat2(u16(d, 56 + 5));
//                set_map(u16(d, 56 + 5));
                set_aca(u16(d, 58 + 9));
                set_acg(u16(d, 60 + 9));
                set_acr(u16(d, 62 + 9));
                set_pnb(u16(d, 64 + 9));
                set_pnc(u16(d, 66 + 9));
                set_pxc(u16(d, 68 + 9));
                set_pbl(u16(d, 70 + 9));
                set_pbh(u16(d, 72 + 9));
                set_pp0(u16(d, 74 + 9));
                set_pp1(u16(d, 76 + 9));
                set_pvl(u16(d, 78 + 9));
                set_pvh(u16(d, 80 + 9));
                set_pv0(u16(d, 82 + 9));
                set_pv5(u16(d, 84 + 9));
                set_pc0(u16(d, 86 + 9));
                set_pc1(u16(d, 88 + 9));

                /* Ice Configuration */

                set_icm(u8(d, 90 + 9));
                set_imm(u16(d, 91 + 9));
                set_ica0(u16(d, 93 + 9));
                set_ica1(u16(d, 95 + 9));
                set_icb0(u16(d, 97 + 9));
                set_icb1(u16(d, 99 + 9));
                set_imt0(u16(d, 101 + 9));
                set_imt1(u16(d, 103 + 9));
                set_ilt0(u16(d, 105 + 9));
                set_ilt1(u16(d, 107 + 9));
                set_icn(u16(d, 109 + 9));
                set_icx(u16(d, 111 + 9));
                set_ict(i16(d, 113 + 9));
                set_ics(u16(d, 115 + 9));
                set_iss0(u8(d, 117 + 9));
                set_iss1(u8(d, 118 + 9));
                set_ist(u16(d, 119 + 10));
                set_iso(u16(d, 121 + 10));
                set_isp(u8(d, 123 + 10));
                set_iue0(u8(d, 124 + 10));
                set_iue1(u8(d, 125 + 10));
                set_icd0(u16(d, 126 + 10));
                set_icd1(u16(d, 128 + 10));
                set_icr0(u32(d, 130 + 10));
                set_icr1(u32(d, 134 + 10));
                set_icg0(u8(d, 138 + 10));
                set_icg1(u8(d, 139 + 10));
                set_isa(u8(d, 140 + 10));
                set_isd(u16(d, 141 + 10));
                set_iat(u16(d, 143 + 10));
                set_isv(u16(d, 145 + 10));
                set_isw(u16(d, 147 + 10));
                set_ico0(u16(d, 149 + 10));
                set_ico1(u16(d, 151 + 10));
                set_icf(u32(d, 153 + 10));
                set_icw(u32(d, 157 + 10));

                /* Descent state configuration: */

                set_descend_start_time(bpt::not_a_date_time);
                set_dns(u16(d, 161 + 10));
                set_dra(u16(d, 163 + 10));
                set_dsd(u16(d, 165 + 10));
                set_dst(u16(d, 167 + 10));
                set_ddf(u16(d, 169 + 10));
                set_ddc(u16(d, 171 + 10));
                set_dsi(u32(d, 173 + 10));
                set_ddd(u16(d, 177 + 10));
                set_ddt(u16(d, 179 + 10));
                set_dft(u16(d, 181 + 10));
                set_dcm(u8(d, 183 + 10));
                set_dal(u8(d, 184 + 10));
                set_dct(u16(d, 185 + 10));
                set_dca(u8(d, 187 + 10));
                set_drc(u16(d, 188 + 10));
                set_drt(u16(d, 190 + 10));
                set_dcx(u16(d, 192 + 10));
                set_dcp(u16(d, 194 + 10));
                set_dcf(u16(d, 196 + 10));
                set_dcd(u16(d, 198 + 10));
                set_dcr(u16(d, 200 + 10));
                set_dsw(u16(d, 202 + 10));
                set_duc(u16(d, 204 + 10));
                set_dpt(u16(d, 206 + 10));
                set_dut(u16(d, 208 + 10));
                set_dpd(u16(d, 210 + 10));
                set_dvt(u16(d, 212 + 10));

                set_dvd(u16(d, 214 + 10));
                set_dvw(u16(d, 216 + 10));


                set_pfr(u16(d, 218 + 10));
                set_pfc(u16(d, 220 + 10));
                set_ddp(u16(d, 222 + 10));
                set_dds(u16(d, 224 + 10));
                set_park_start_time(bpt::not_a_date_time);
                set_pns(u16(d, 226 + 10));
                set_pto(u32(d, 228 + 10));
                set_pth(u32(d, 232 + 10));
                // set_ptd(u32(d, 236 + 10))
                set_psa(u8(d, 236 + 14));
                set_psi(u32(d, 237 + 14));
                set_pfm(u8(d, 241 + 14));
                set_pft(u16(d, 242 + 14));
                set_pfd(u16(d, 244 + 14));
                set_pfv(u16(d, 246 + 14));
                set_pdt(u16(d, 248 + 14));
                set_psm(u8(d, 250 + 14));
                set_psd(u16(d, 251 + 14));
                set_pst(u16(d, 253 + 14));
                set_psv(u16(d, 255 + 14));
                set_psw(u16(d, 257 + 14));
                set_ascent_start_time(bpt::not_a_date_time);
                set_asn(u16(d, 259 + 14));
                set_arr(u16(d, 261 + 14));
                set_apm(u16(d, 263 + 14));
                set_apn(u16(d, 265 + 14));
                set_apf(u16(d, 267 + 14));
                set_asi(u32(d, 269 + 14));
                set_asd(u16(d, 273 + 14));
                set_add(u16(d, 275 + 14));
                set_adt(u16(d, 277 + 14));
                set_arc(u16(d, 279 + 14));
                set_ahr(u16(d, 281 + 14));
                set_ahd(u16(d, 283 + 14));
                set_surface_start_time(bpt::not_a_date_time);
                set_sfn(u16(d, 285 + 14));
                set_sdc(i16(d, 287 + 14));
                set_sgt(u16(d, 289 + 14));
                set_set(u16(d, 291 + 14));
                set_sbm(u8(d, 293 + 14));
                set_sbt(u16(d, 294 + 14));
                set_sdt(u32(d, 296 + 14));
                set_ssa(u16(d, 300 + 14));
                set_ssi(u32(d, 302 + 14));
                set_sds(u16(d, 306 + 14));
                set_sam(u8(d, 308 + 14));
                set_sax(u16(d, 309 + 14));
                set_san(u16(d, 311 + 14));
                set_sao(u16(d, 313 + 14));
//                set_sap(u16(d, 315 + 14));
//                set_spt(u16(d, 317 + 14));
//                set_ssp(u16(d, 319 + 14));

                break;

            case 0x04066100: // ALAMO Software version 0x04066100 (2017 S2h test float 10108).

                if(data.size() != 328) {
                    ostringstream os;
                    os << "Invalid X-Message: Expected 328 bytes, but received "
                       << data.size()
                       << " bytes!";
#                   if defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG
                    lwrn() << os.str();
#                   else
                    EXC_THROW(os.str());
#                   endif // defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG
                }

                /* General Configuration: */

                set_msg_version(u8(d, 8));
                set_sensor_type(u8(d, 9));
                set_msn(u16(d, 10));
                set_dtd(u16(d, 12));
                set_mds(u16(d, 14));
                set_apt(u8(d, 16));
                set_btm(u8(d, 17));
                set_xas(u8(d, 18));
                set_cps(u8(d, 19));
                set_cst(u8(d, 20));
                set_ctf(u16(d, 21));
                set_ctl(u16(d, 23));
                set_ctu(u16(d, 25));
                // set_ctc(u8(d, 27))

                set_csp(u32(d, 27 + 1));
                set_cpb1(u16(d, 31 + 1));
                set_cpb2(u16(d, 33 + 1));
                set_cpb3(u16(d, 35 + 1));
                set_cpn1(u16(d, 37 + 1));
                set_cpn2(u16(d, 39 + 1));
                set_cpn3(u16(d, 41 + 1));
                set_cpp1(u16(d, 43 + 1));
                set_cpp2(u16(d, 45 + 1));
                set_cpp3(u16(d, 47 + 1));
                // set_crd
                // set_crs


                set_mtd(u16(d, 49 + 5));
                set_mtt(u32(d, 51 + 5));
                set_mam(u8(d, 55 + 5));
//                set_mat1(u16(d, 56 + 5));
//                set_mat2(u16(d, 56 + 5));
//                set_map(u16(d, 56 + 5));
                set_aca(u16(d, 58 + 9));
                set_acg(u16(d, 60 + 9));
                set_acr(u16(d, 62 + 9));
                set_pnb(u16(d, 64 + 9));
                set_pnc(u16(d, 66 + 9));
                set_pxc(u16(d, 68 + 9));
                set_pbl(u16(d, 70 + 9));
                set_pbh(u16(d, 72 + 9));
                set_pp0(u16(d, 74 + 9));
                set_pp1(u16(d, 76 + 9));
                set_pvl(u16(d, 78 + 9));
                set_pvh(u16(d, 80 + 9));
                set_pv0(u16(d, 82 + 9));
                set_pv5(u16(d, 84 + 9));
                set_pc0(u16(d, 86 + 9));
                set_pc1(u16(d, 88 + 9));

                /* Ice Configuration */

                set_icm(u8(d, 90 + 9));
                set_imm(u16(d, 91 + 9));
                set_ica0(u16(d, 93 + 9));
                set_ica1(u16(d, 95 + 9));
                set_icb0(u16(d, 97 + 9));
                set_icb1(u16(d, 99 + 9));
                set_imt0(u16(d, 101 + 9));
                set_imt1(u16(d, 103 + 9));
                set_ilt0(u16(d, 105 + 9));
                set_ilt1(u16(d, 107 + 9));
                set_icn(u16(d, 109 + 9));
                set_icx(u16(d, 111 + 9));
                set_ict(i16(d, 113 + 9));
                set_ics(u16(d, 115 + 9));
                set_iss0(u8(d, 117 + 9));
                set_iss1(u8(d, 118 + 9));
                set_ist(u16(d, 119 + 10));
                set_iso(u16(d, 121 + 10));
                set_isp(u8(d, 123 + 10));
                set_iue0(u8(d, 124 + 10));
                set_iue1(u8(d, 125 + 10));
                set_icd0(u16(d, 126 + 10));
                set_icd1(u16(d, 128 + 10));
                set_icr0(u32(d, 130 + 10));
                set_icr1(u32(d, 134 + 10));
                set_icg0(u8(d, 138 + 10));
                set_icg1(u8(d, 139 + 10));
                set_isa(u8(d, 140 + 10));
                set_isd(u16(d, 141 + 10));
                set_iat(u16(d, 143 + 10));
                set_isv(u16(d, 145 + 10));
                set_isw(u16(d, 147 + 10));
                set_ico0(u16(d, 149 + 10));
                set_ico1(u16(d, 151 + 10));
                set_icf(u32(d, 153 + 10));
                set_icw(u32(d, 157 + 10));

                /* Descent state configuration: */

                set_descend_start_time(bpt::not_a_date_time);
                set_dns(u16(d, 161 + 10));
                set_dra(u16(d, 163 + 10));
                set_dsd(u16(d, 165 + 10));
                set_dst(u16(d, 167 + 10));
                set_ddf(u16(d, 169 + 10));
                set_ddc(u16(d, 171 + 10));
                set_dsi(u32(d, 173 + 10));
                set_ddd(u16(d, 177 + 10));
                set_ddt(u16(d, 179 + 10));
                set_dft(u16(d, 181 + 10));
                set_dcm(u8(d, 183 + 10));
                set_dal(u8(d, 184 + 10));
                set_dct(u16(d, 185 + 10));
                set_dca(u8(d, 187 + 10));
                set_drc(u16(d, 188 + 10));
                set_drt(u16(d, 190 + 10));
                set_dcx(u16(d, 192 + 10));
                set_dcp(u16(d, 194 + 10));
                set_dcf(u16(d, 196 + 10));
                set_dcd(u16(d, 198 + 10));
                set_dcr(u16(d, 200 + 10));
                set_dsw(u16(d, 202 + 10));
                set_duc(u16(d, 204 + 10));
                set_dpt(u16(d, 206 + 10));
                set_dut(u16(d, 208 + 10));
                set_dpd(u16(d, 210 + 10));
                set_dvt(u16(d, 212 + 10));

                set_dvd(u16(d, 214 + 10));
                set_dvw(u16(d, 216 + 10));


                set_pfr(u16(d, 218 + 10));
                set_pfc(u16(d, 220 + 10));
                set_ddp(u16(d, 222 + 10));
                set_dds(u16(d, 224 + 10));
                set_park_start_time(bpt::not_a_date_time);
                set_pns(u16(d, 226 + 10));
                set_pto(u32(d, 228 + 10));
                set_pth(u32(d, 232 + 10));
                // set_ptd(u32(d, 236 + 10))
                set_psa(u8(d, 236 + 14));
                set_psi(u32(d, 237 + 14));
                set_pfm(u8(d, 241 + 14));
                set_pft(u16(d, 242 + 14));
                set_pfd(u16(d, 244 + 14));
                set_pfv(u16(d, 246 + 14));
                set_pdt(u16(d, 248 + 14));
                set_psm(u8(d, 250 + 14));
                set_psd(u16(d, 251 + 14));
                set_pst(u16(d, 253 + 14));
                set_psv(u16(d, 255 + 14));
                set_psw(u16(d, 257 + 14));
                set_ascent_start_time(bpt::not_a_date_time);
                set_asn(u16(d, 259 + 14));
                set_arr(u16(d, 261 + 14));
                set_apm(u16(d, 263 + 14));
                set_apn(u16(d, 265 + 14));
                set_apf(u16(d, 267 + 14));
                set_asi(u32(d, 269 + 14));
                set_asd(u16(d, 273 + 14));
                set_add(u16(d, 275 + 14));
                set_adt(u16(d, 277 + 14));
                set_arc(u16(d, 279 + 14));
                set_ahr(u16(d, 281 + 14));
                set_ahd(u16(d, 283 + 14));
                set_surface_start_time(bpt::not_a_date_time);
                set_sfn(u16(d, 285 + 14));
                set_sdc(i16(d, 287 + 14));
                set_sgt(u16(d, 289 + 14));
                set_set(u16(d, 291 + 14));
                set_sbm(u8(d, 293 + 14));
                set_sbt(u16(d, 294 + 14));
                set_sdt(u32(d, 296 + 14));
                set_ssa(u16(d, 300 + 14));
                set_ssi(u32(d, 302 + 14));
                set_sds(u16(d, 306 + 14));
                set_sam(u8(d, 308 + 14));
                set_sax(u16(d, 309 + 14));
                set_san(u16(d, 311 + 14));
                set_sao(u16(d, 313 + 14));
//                set_sap(u16(d, 315 + 14));
//                set_spt(u16(d, 317 + 14));
//                set_ssp(u16(d, 319 + 14));

                break;

            case 0x05006100: // ALAMO Software version 0x05006100 (2017 Alto test float 10109).

              if(data.size() != 328) {
                  ostringstream os;
                  os << "Invalid X-Message: Expected 328 bytes, but received "
                     << data.size()
                     << " bytes!";
#                   if defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG
                  lwrn() << os.str();
#                   else
                  EXC_THROW(os.str());
#                   endif // defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG
              }

              /* General Configuration: */

              set_msg_version(u8(d, 8));
              set_sensor_type(u8(d, 9));
              set_msn(u16(d, 10));
              set_dtd(u16(d, 12));
              set_mds(u16(d, 14));
              set_apt(u8(d, 16));
              set_btm(u8(d, 17));
              set_xas(u8(d, 18));
              set_cps(u8(d, 19));
              set_cst(u8(d, 20));
              set_ctf(u16(d, 21));
              set_ctl(u16(d, 23));
              set_ctu(u16(d, 25));
              // set_ctc(u8(d, 27))

              set_csp(u32(d, 27 + 1));
              set_cpb1(u16(d, 31 + 1));
              set_cpb2(u16(d, 33 + 1));
              set_cpb3(u16(d, 35 + 1));
              set_cpn1(u16(d, 37 + 1));
              set_cpn2(u16(d, 39 + 1));
              set_cpn3(u16(d, 41 + 1));
              set_cpp1(u16(d, 43 + 1));
              set_cpp2(u16(d, 45 + 1));
              set_cpp3(u16(d, 47 + 1));
              // set_crd
              // set_crs


              set_mtd(u16(d, 49 + 5));
              set_mtt(u32(d, 51 + 5));
              set_mam(u8(d, 55 + 5));
//                set_mat1(u16(d, 56 + 5));
//                set_mat2(u16(d, 56 + 5));
//                set_map(u16(d, 56 + 5));
              set_aca(u16(d, 58 + 9));
              set_acg(u16(d, 60 + 9));
              set_acr(u16(d, 62 + 9));
              set_pnb(u16(d, 64 + 9));
              set_pnc(u16(d, 66 + 9));
              set_pxc(u16(d, 68 + 9));
              set_pbl(u16(d, 70 + 9));
              set_pbh(u16(d, 72 + 9));
              set_pp0(u16(d, 74 + 9));
              set_pp1(u16(d, 76 + 9));
              set_pvl(u16(d, 78 + 9));
              set_pvh(u16(d, 80 + 9));
              set_pv0(u16(d, 82 + 9));
              set_pv5(u16(d, 84 + 9));
              set_pc0(u16(d, 86 + 9));
              set_pc1(u16(d, 88 + 9));

              /* Ice Configuration */

              set_icm(u8(d, 90 + 9));
              set_imm(u16(d, 91 + 9));
              set_ica0(u16(d, 93 + 9));
              set_ica1(u16(d, 95 + 9));
              set_icb0(u16(d, 97 + 9));
              set_icb1(u16(d, 99 + 9));
              set_imt0(u16(d, 101 + 9));
              set_imt1(u16(d, 103 + 9));
              set_ilt0(u16(d, 105 + 9));
              set_ilt1(u16(d, 107 + 9));
              set_icn(u16(d, 109 + 9));
              set_icx(u16(d, 111 + 9));
              set_ict(i16(d, 113 + 9));
              set_ics(u16(d, 115 + 9));
              set_iss0(u8(d, 117 + 9));
              set_iss1(u8(d, 118 + 9));
              set_ist(u16(d, 119 + 10));
              set_iso(u16(d, 121 + 10));
              set_isp(u8(d, 123 + 10));
              set_iue0(u8(d, 124 + 10));
              set_iue1(u8(d, 125 + 10));
              set_icd0(u16(d, 126 + 10));
              set_icd1(u16(d, 128 + 10));
              set_icr0(u32(d, 130 + 10));
              set_icr1(u32(d, 134 + 10));
              set_icg0(u8(d, 138 + 10));
              set_icg1(u8(d, 139 + 10));
              set_isa(u8(d, 140 + 10));
              set_isd(u16(d, 141 + 10));
              set_iat(u16(d, 143 + 10));
              set_isv(u16(d, 145 + 10));
              set_isw(u16(d, 147 + 10));
              set_ico0(u16(d, 149 + 10));
              set_ico1(u16(d, 151 + 10));
              set_icf(u32(d, 153 + 10));
              set_icw(u32(d, 157 + 10));

              /* Descent state configuration: */

              set_descend_start_time(bpt::not_a_date_time);
              set_dns(u16(d, 161 + 10));
              set_dra(u16(d, 163 + 10));
              set_dsd(u16(d, 165 + 10));
              set_dst(u16(d, 167 + 10));
              set_ddf(u16(d, 169 + 10));
              set_ddc(u16(d, 171 + 10));
              set_dsi(u32(d, 173 + 10));
              set_ddd(u16(d, 177 + 10));
              set_ddt(u16(d, 179 + 10));
              set_dft(u16(d, 181 + 10));
              set_dcm(u8(d, 183 + 10));
              set_dal(u8(d, 184 + 10));
              set_dct(u16(d, 185 + 10));
              set_dca(u8(d, 187 + 10));
              set_drc(u16(d, 188 + 10));
              set_drt(u16(d, 190 + 10));
              set_dcx(u16(d, 192 + 10));
              set_dcp(u16(d, 194 + 10));
              set_dcf(u16(d, 196 + 10));
              set_dcd(u16(d, 198 + 10));
              set_dcr(u16(d, 200 + 10));
              set_dsw(u16(d, 202 + 10));
              set_duc(u16(d, 204 + 10));
              set_dpt(u16(d, 206 + 10));
              set_dut(u16(d, 208 + 10));
              set_dpd(u16(d, 210 + 10));
              set_dvt(u16(d, 212 + 10));

              set_dvd(u16(d, 214 + 10));
              set_dvw(u16(d, 216 + 10));


              set_pfr(u16(d, 218 + 10));
              set_pfc(u16(d, 220 + 10));
              set_ddp(u16(d, 222 + 10));
              set_dds(u16(d, 224 + 10));
              set_park_start_time(bpt::not_a_date_time);
              set_pns(u16(d, 226 + 10));
              set_pto(u32(d, 228 + 10));
              set_pth(u32(d, 232 + 10));
              // set_ptd(u32(d, 236 + 10))
              set_psa(u8(d, 236 + 14));
              set_psi(u32(d, 237 + 14));
              set_pfm(u8(d, 241 + 14));
              set_pft(u16(d, 242 + 14));
              set_pfd(u16(d, 244 + 14));
              set_pfv(u16(d, 246 + 14));
              set_pdt(u16(d, 248 + 14));
              set_psm(u8(d, 250 + 14));
              set_psd(u16(d, 251 + 14));
              set_pst(u16(d, 253 + 14));
              set_psv(u16(d, 255 + 14));
              set_psw(u16(d, 257 + 14));
              set_ascent_start_time(bpt::not_a_date_time);
              set_asn(u16(d, 259 + 14));
              set_arr(u16(d, 261 + 14));
              set_apm(u16(d, 263 + 14));
              set_apn(u16(d, 265 + 14));
              set_apf(u16(d, 267 + 14));
              set_asi(u32(d, 269 + 14));
              set_asd(u16(d, 273 + 14));
              set_add(u16(d, 275 + 14));
              set_adt(u16(d, 277 + 14));
              set_arc(u16(d, 279 + 14));
              set_ahr(u16(d, 281 + 14));
              set_ahd(u16(d, 283 + 14));
              set_surface_start_time(bpt::not_a_date_time);
              set_sfn(u16(d, 285 + 14));
              set_sdc(i16(d, 287 + 14));
              set_sgt(u16(d, 289 + 14));
              set_set(u16(d, 291 + 14));
              set_sbm(u8(d, 293 + 14));
              set_sbt(u16(d, 294 + 14));
              set_sdt(u32(d, 296 + 14));
              set_ssa(u16(d, 300 + 14));
              set_ssi(u32(d, 302 + 14));
              set_sds(u16(d, 306 + 14));
              set_sam(u8(d, 308 + 14));
              set_sax(u16(d, 309 + 14));
              set_san(u16(d, 311 + 14));
              set_sao(u16(d, 313 + 14));

              break;

            default: { // Unsupported ALAMO software version.

                ostringstream os;
                os << "Unsupported ALAMO Mission Parameters message version: 0x"
                   << hex
                   << setw(8)
                   << setfill('0')
                   << fw_version()
                   << "!";
                EXC_THROW(os.str());
            }

                break;
            }

#           if defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG

            ldbg() << "Mission configuration listing string: "
                    << endl << GenerateListingString(0, 0, 0);

#           endif // defined(XMSG_DEBUG_ALAMO_MISS_GFG) && XMSG_DEBUG_ALAMO_MISS_GFG

        }CATCH_BLOCK("Failed to decode ALAMO mission configuration message!")

    }

    /** Generate a listing entry string from this ALAMO mission configuration message object.
     *
     *  @param[in]      whoi        The unique WHOI instrument identifier or serial number.
     *  @param[in]      dive        The dive or cycle number of this ALAMO mission configuration
     *                              message object.
     *  @param[in]      idx         The X-Message index of this ALAMO mission configuration message
     *                              object.
     *
     *  @return         A listing entry string from this ALAMO mission configuration message object.
     */
    const string GenerateListingString(int whoi, int dive, int idx) const {

        int midx = 0;
        ostringstream os;

        try {

            switch(fw_version()) {
            case 0x02006200: // ALAMO Software version 0x02006200.
            case 0x02006300: // ALAMO Software version 0x02006300.
            case 0x02006700: // ALAMO Software version 0x02006700.
            case 0x02006800: // ALAMO Software version 0x02006800.
            {

                // Header lines.
                static const format fmt1("#   %5s %4s %4s %16s");
                static const format fmt3("mis %05d %4d %4d       0x%08X");
                os << format(fmt1) % "SN" % "dive" % "idx" % "SWVer" << endl;
                os << format(fmt3) % whoi % dive % idx % fw_version() << endl;

                // Minor index 0.
                static const format fmt4("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s");
                static const format fmt5("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s");
                static const format fmt6("mis %05d %4d %4d %4d %11d %11d %11d %11d %11d");
                os << format(fmt4) % "SN" % "dive" % "idx" % "midx" % "MSN" % "DTD" % "MDS" % "APT"
                           % "XAS"
                   << endl;
                os << format(fmt5) % "" % "" % "" % "" % "" % "(dBar)" % "(dBar)" % "" % "" << endl;
                os << format(fmt6) % whoi % dive % idx % midx++ // midx
                           % msn() % dtd() % mds() % (int) apt() % (int) xas()
                   << endl;

                // Minor index 1.
                static const format fmt7("#   %5s %4s %4s %4s %11s %11s");
                static const format fmt8("#    %5s %4s %4s %4s %11s %11s");
                static const format fmt9("mis %05d %4d %4d %4d %11d %11.3f");
                os << "# --- Start-of-Mission Trigger ---------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt7) % "SN" % "dive" % "idx" % "midx" % "MTD" % "MTT" << endl;
                os << format(fmt8) % "" % "" % "" % "" % "(dBar)" % "(sec)" << endl;
                os << format(fmt9) % whoi % dive % idx % midx++ // midx
                           % mtd() % mtt_sec()
                   << endl;

                // Minor index 2.
                static const format fmt10("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s");
                static const format fmt11("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s");
                static const format fmt12("mis %05d %4d %4d %4d %11d %11d %11d %11d %11d");
                os << "# --- Wave-Field Measurement -----------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt10) % "SN" % "dive" % "idx" % "midx" % "MAM" % "MAT" % "ACA" % "ACG"
                           % "ACR"
                   << endl;
                os << format(fmt11) % "" % "" % "" % "" % "" % "(sec)" % "(g/FSR)" % "(°/sec/FSR)"
                           % "(Hz)"
                   << endl;
                os << format(fmt12) % whoi % dive % idx % midx++ // midx
                           % (int) mam() % mat() % aca() % acg() % acr()
                   << endl;

                // Minor index 3.
                static const format fmt13("#   %5s %4s %4s %4s %11s %11s %11s");
                static const format fmt14("#    %5s %4s %4s %4s %11s %11s %11s");
                static const format fmt15("mis %05d %4d %4d %4d %11.2f %11.2f %11.2f");
                os << "# --- Built-In Test (BIT) --------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt13) % "SN" % "dive" % "idx" % "midx" % "PNB" % "PNC" % "PXC"
                   << endl;
                os << format(fmt14) % "" % "" % "" % "" % "(V)" % "(psi)" % "(psi)" << endl;
                os << format(fmt15) % whoi % dive % idx % midx++ // midx
                           % pnb_v() % pnc_psi() % pxc_psi()
                   << endl;

                // Minor index 4.
                static const format fmt16("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt17("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt18("mis %05d %4d %4d %4d %11d %11d %11d %11d %11d %11d");
                os << "# --- Calibration ----------------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt16) % "SN" % "dive" % "idx" % "midx" % "PBL" % "PBH" % "PP0" % "PP1"
                           % "PVL" % "PVH"
                   << endl;
                os << format(fmt17) % "" % "" % "" % "" % "(LSB/12V)" % "(LSB/14V)" % "(LSB/0A)"
                           % "(LSB/1)A" % "(LSB/12V)" % "(LSB/14V)"
                   << endl;
                os << format(fmt18) % whoi % dive % idx % midx++ // midx
                           % pbl() % pbh() % pp0() % pp1() % pvl() % pvh()
                   << endl;

                // Minor index 5.
                static const format fmt19("#   %5s %4s %4s %4s %11s %11s %11s %11s");
                static const format fmt20("#    %5s %4s %4s %4s %11s %11s %11s %11s");
                static const format fmt21("mis %05d %4d %4d %4d %11d %11d %11d %11d");
                os << format(fmt19) % "SN" % "dive" % "idx" % "midx" % "PV0" % "PV5" % "PC0" % "PC1"
                   << endl;
                os << format(fmt20) % "" % "" % "" % "" % "(LSB/0inHg)" % "(LSB/5inHg)" % "(LSB/0A)"
                           % "(LSB/1A)"
                   << endl;
                os << format(fmt21) % whoi % dive % idx % midx++ // midx
                           % pv0() % pv5() % pc0() % pc1()
                   << endl;

                // Minor index 6.
                static const format fmt22("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt23("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt24("mis %05d %4d %4d %4d %11d %11d %11.3f %11d %11d %11d");
                os << "# --- Profile Configuration ------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt22) % "SN" % "dive" % "idx" % "midx" % "CPS" % "CST" % "CSP"
                           % "CPB1" % "CPB2" % "CPB3"
                   << endl;
                os << format(fmt23) % "" % "" % "" % "" % "" % "" % "(sec)" % "(dBar)" % "(dBar)"
                           % "(dBar)"
                   << endl;
                os << format(fmt24) % whoi % dive % idx % midx++ // midx
                           % (int) cps() % (int) cst() % csp_sec() % cpb1() % cpb2() % cpb3()
                   << endl;

                // Minor index 7.
                static const format fmt25("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt26("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt27(
                        "mis %05d %4d %4d %4d %11.1f %11.1f %11.1f %11.3f %11.3f %11.3f");
                os << format(fmt25) % "SN" % "dive" % "idx" % "midx" % "CPN1" % "CPN2" % "CPN3"
                           % "CPP1" % "CPP2" % "CPP3"
                   << endl;
                os << format(fmt26) % "" % "" % "" % "" % "(dBar)" % "(dBar)" % "(dBar)" % "(sec)"
                           % "(sec)" % "(sec)"
                   << endl;
                os << format(fmt27) % whoi % dive % idx
                           % midx++ // midx
                           % cpn1_dbar() % cpn2_dbar() % cpn3_dbar() % cpp1_sec() % cpp2_sec()
                           % cpp3_sec()
                   << endl;

                // Minor index 8.
                static const format fmt28("#   %5s %4s %4s %4s %11s %11s %11s");
                static const format fmt29("#    %5s %4s %4s %4s %11s %11s %11s");
                static const format fmt30("mis %05d %4d %4d %4d %11d %11d %11d");
                os << "# --- Diagnostic Dive ------------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt28) % "SN" % "dive" % "idx" % "midx" % "DDP" % "DDS" % "DDF"
                   << endl;
                os << format(fmt29) % "" % "" % "" % "" % "(dBar)" % "(sec)" % "(sec)" << endl;
                os << format(fmt30) % whoi % dive % idx % midx++ // midx
                           % ddp() % dds() % ddf()
                   << endl;

                // Minor index 9.
                static const format fmt31("#   %5s %4s %4s %4s %11s %11s %11s %11s");
                static const format fmt32("#    %5s %4s %4s %4s %11s %11s %11s %11s");
                static const format fmt33("mis %05d %4d %4d %4d %11d %11d %11d %11d");
                os << "# --- Descent - General ----------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt31) % "SN" % "dive" % "idx" % "midx" % "DNS" % "DTD" % "PFR" % "PFC"
                   << endl;
                os << format(fmt32) % "" % "" % "" % "" % "" % "(dBar)" % "(mL/min)" % "(mL/dBar)"
                   << endl;
                os << format(fmt33) % whoi % dive % idx % midx++ // midx
                           % dns() % dtd() % pfr() % pfc()
                   << endl;

                // Minor index 10.
                static const format fmt34("#   %5s %4s %4s %4s %11s %11s");
                static const format fmt35("#    %5s %4s %4s %4s %11s %11s");
                static const format fmt36("mis %05d %4d %4d %4d %11d %11d");
                os << "# --- Descent - Stage 1 (start) --------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt34) % "SN" % "dive" % "idx" % "midx" % "DSD" % "DST" << endl;
                os << format(fmt35) % "" % "" % "" % "" % "(dBar)" % "(sec)" << endl;
                os << format(fmt36) % whoi % dive % idx % midx++ // midx
                           % dsd() % dst()
                   << endl;

                // Minor index 11.
                static const format fmt37("#   %5s %4s %4s %4s %11s %11s %11s %11s");
                static const format fmt38("#    %5s %4s %4s %4s %11s %11s %11s %11s");
                static const format fmt39("mis %05d %4d %4d %4d %11d %11d %11d %11.3f");
                os << "# --- Descent - Stage 2 (fall) ---------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt37) % "SN" % "dive" % "idx" % "midx" % "DFT" % "DDD" % "DDT" % "DSI"
                   << endl;
                os << format(fmt38) % "" % "" % "" % "" % "(sec)" % "(dBar)" % "(sec)" % "(sec)"
                   << endl;
                os << format(fmt39) % whoi % dive % idx % midx++ // midx
                           % dft() % ddd() % ddt() % dsi_sec()
                   << endl;

                // Minor index 12.
                static const format fmt40("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt41("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt42("mis %05d %4d %4d %4d %11d %11d %11d %11d %11d %11d");
                os << "# --- Descent - Stage 3 (calibration) --------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt40) % "SN" % "dive" % "idx" % "midx" % "DCT" % "DCM" % "DAL" % "DCA"
                           % "DSW" % "DUC"
                   << endl;
                os << format(fmt41) % "" % "" % "" % "" % "(sec)" % "" % "" % "" % "(sec)" % ""
                   << endl;
                os << format(fmt42) % whoi % dive % idx % midx++ // midx
                           % dct() % (int) dcm() % (int) dal() % (int) dca() % dsw() % duc()
                   << endl;

                // Minor index 13.
                static const format fmt43("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt44("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt45("mis %05d %4d %4d %4d %11d %11d %11d %11d %11d %11d");
                os << format(fmt43) % "SN" % "dive" % "idx" % "midx" % "DPT" % "DUT" % "DVT" % "DCX"
                           % "DCP" % "DCF"
                   << endl;
                os << format(fmt44) % "" % "" % "" % "" % "(sec)" % "(sec)" % "(sec)" % "(sec)"
                           % "(sec)" % "(sec)"
                   << endl;
                os << format(fmt45) % whoi % dive % idx % midx++ // midx
                           % dpt() % dut() % dvt() % dcx() % dcp() % dcf()
                   << endl;

                // Minor index 14.
                static const format fmt46("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt47("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt48("mis %05d %4d %4d %4d %11d %11d  0x%08X %11d %11.3f %11d");
                os << "# --- Park/Drift - General -------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt46) % "SN" % "dive" % "idx" % "midx" % "PNS" % "PTO" % "PTH" % "PSA"
                           % "PSI" % "PFM"
                   << endl;
                os << format(fmt47) % "" % "" % "" % "" % "" % "(sec)" % "" % "" % "(sec)" % ""
                   << endl;
                os << format(fmt48) % whoi % dive % idx % midx++ // midx
                           % pns() % pto() % pth() % (int) psa() % psi_sec() % (int) pfm()
                   << endl;

                // Minor index 15.
                static const format fmt49("#   %5s %4s %4s %4s %11s %11s");
                static const format fmt50("#    %5s %4s %4s %4s %11s %11s");
                static const format fmt51("mis %05d %4d %4d %4d %11d %11d");
                os << format(fmt49) % "SN" % "dive" % "idx" % "midx" % "PFD" % "PDT" << endl;
                os << format(fmt50) % "" % "" % "" % "" % "(dBar)" % "(sec)" << endl;
                os << format(fmt51) % whoi % dive % idx % 15 % pfd() % pdt() << endl;

                // Minor index 16.
                static const format fmt52("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s");
                static const format fmt53("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s");
                static const format fmt54("mis %05d %4d %4d %4d %11s %11s %11s %11s %11s");
                os << "# --- Park/Drift - Seek ----------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt52) % "SN" % "dive" % "idx" % "midx" % "PSM" % "PST" % "PSD" % "PSV"
                           % "PSW"
                   << endl;
                os << format(fmt53) % "" % "" % "" % "" % "" % "(sec)" % "(dBar)" % "(sec)"
                           % "(sec)"
                   << endl;
                os << format(fmt54) % whoi % dive % idx % midx++ // midx
                           % (int) psm() % pst() % psd() % psv() % psw()
                   << endl;

                // Minor index 17.
                static const format fmt55("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt56("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt57("mis %05d %4d %4d %4d %11d %11.2f %11d %11d %11d %11.3f");
                os << "# --- Ascent/Profile -------------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt55) % "SN" % "dive" % "idx" % "midx" % "ASN" % "ARR" % "APM" % "APN"
                           % "APF" % "ASI"
                   << endl;
                os << format(fmt56) % "" % "" % "" % "" % "" % "(m/sec)" % "(sec)" % "(sec)"
                           % "(sec)" % "(sec)"
                   << endl;
                os << format(fmt57) % whoi % dive % idx % midx++ // midx
                           % asn() % arr_m_per_s() % apm() % apn() % apf() % asi_sec()
                   << endl;

                // Minor index 18.
                static const format fmt58("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt59("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt60("mis %05d %4d %4d %4d %11d %11d %11d %11d %11.3f %11d");
                os << format(fmt58) % "SN" % "dive" % "idx" % "midx" % "ASD" % "ADD" % "ADT" % "ARC"
                           % "AHR" % "AHD"
                   << endl;
                os << format(fmt59) % "" % "" % "" % "" % "(dBar)" % "(dBar)" % "(sec)" % "(?)"
                           % "(m/sec)" % "(dBar)"
                   << endl;
                os << format(fmt60) % whoi % dive % idx % midx++ // midx
                           % asd() % add() % adt() % arc() % ahr_m_per_s() % ahd()
                   << endl;

                // Minor index 19.
                static const format fmt61("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt62("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt63("mis %05d %4d %4d %4d %11d %11d %11d %11d %11d %11.3f");
                os << "# --- Surface --------------------------------------------------------------"
                   "------------------------"
                   << endl;

                os << format(fmt61) % "SN" % "dive" % "idx" % "midx" % "SFN" % "SGT" % "SET" % "SDT"
                           % "SSA" % "SSI"
                   << endl;
                os << format(fmt62) % "" % "" % "" % "" % "" % "(min)" % "(min)" % "(?)" % ""
                           % "(sec)"
                   << endl;
                os << format(fmt63) % whoi % dive % idx % midx++ // midx
                           % sfn() % sgt() % set() % sdt() % ssa() % ssi_sec()
                   << endl;

                // Minor index 20.
                static const format fmt64("#   %5s %4s %4s %4s %11s %11s");
                static const format fmt65("#    %5s %4s %4s %4s %11s %11s");
                static const format fmt66("mis %05d %4d %4d %4d %11d %11d");

                os << format(fmt64) % "SN" % "dive" % "idx" % "midx" % "SDS" % "SPT" << endl;
                os << format(fmt65) % "" % "" % "" % "" % "(sec)" % "(sec)" << endl;
                os << format(fmt66) % whoi % dive % idx % midx++ // midx
                           % sds() % spt()
                   << endl;

                // Minor index 21.
                static const format fmt67("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s");
                static const format fmt68("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s");
                static const format fmt69("mis %05d %4d %4d %4d %11d %11.3f %11.3f %11d %11d");
                os << "# --- Surface - Air Pump ---------------------------------------------------"
                   "------------------------"
                   << endl;

                os << format(fmt67) % "SN" % "dive" % "idx" % "midx" % "SAM" % "SAX" % "SAN" % "SAO"
                           % "SAP"
                   << endl;
                os << format(fmt68) % "" % "" % "" % "" % "" % "(psi)" % "(psi)" % "(sec)" % "(sec)"
                   << endl;
                os << format(fmt69) % whoi % dive % idx % midx++ // midx
                           % (int) sam() % sax_psi() % san_psi() % sao() % sap()
                   << endl;

                // Minor index 22.
                static const format fmt70("#   %5s %4s %4s %4s %11s %11s");
                static const format fmt71("#    %5s %4s %4s %4s %11s %11s");
                static const format fmt72("mis %05d %4d %4d %4d %11d %11d");
                os << "# --- Surface - Abort/Beacon Mode ------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt70) % "SN" % "dive" % "idx" % "midx" % "SBM" % "SBT" << endl;
                os << format(fmt71) % "" % "" % "" % "" % "" % "(sec)" << endl;
                os << format(fmt72) % whoi % dive % idx % midx++ // midx
                           % (int) sbm() % sbt()
                   << endl;
            }

                break;

            case 0x02006900: // ALAMO Firmware version 0x02006900.
            case 0x02036200: // ALAMO Firmware version 0x02036200.
            case 0x02046200: // ALAMO Firmware version 0x02046200.
            case 0x02056200: // ALAMO Firmware version 0x02056200.
            case 0x03006500: // ALAMO Firmware version 0x03006500.
            case 0x03016700: // ALAMO Software version 0x03016700 (Solo-II Hybrid test float #10083).
            case 0x03026100: // ALAMO Software version 0x03026100 (2016 NASCar floats).
            case 0x04016100: // ALAMO Software version 0x04016100 (2016 NASCar floats).

            {

                {   // Header lines.
                    static const format fmt1("#   %5s %4s %4s %16s");
                    static const format fmt3("mis %05d %4d %4d       0x%08X");
                    os << format(fmt1) % "SN" % "dive" % "idx" % "SWVer" << endl;
                    os << format(fmt3) % whoi % dive % idx % fw_version() << endl;
                }

                static const format fmt4("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s");
                static const format fmt5("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s");
                static const format fmt6("mis %05d %4d %4d %4d %11d %11d %11d %11d %11d");
                os << format(fmt4) % "SN" % "dive" % "idx" % "midx" % "MSN" % "DTD" % "MDS" % "APT"
                           % "XAS"
                   << endl;
                os << format(fmt5) % "" % "" % "" % "" % "" % "(dBar)" % "(dBar)" % "" % "" << endl;
                os << format(fmt6) % whoi % dive % idx % midx++ // midx
                           % msn() % dtd() % mds() % (int) apt() % (int) xas()
                   << endl;

                static const format fmt7("#   %5s %4s %4s %4s %11s %11s");
                static const format fmt8("#    %5s %4s %4s %4s %11s %11s");
                static const format fmt9("mis %05d %4d %4d %4d %11d %11.3f");
                os << "# --- Start-of-Mission Trigger ---------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt7) % "SN" % "dive" % "idx" % "midx" % "MTD" % "MTT" << endl;
                os << format(fmt8) % "" % "" % "" % "" % "(dBar)" % "(sec)" << endl;
                os << format(fmt9) % whoi % dive % idx % midx++ // midx
                           % mtd() % mtt_sec()
                   << endl;

                static const format fmt28("#   %5s %4s %4s %4s %11s %11s %11s");
                static const format fmt29("#    %5s %4s %4s %4s %11s %11s %11s");
                static const format fmt30("mis %05d %4d %4d %4d %11d %11d %11d");
                os << "# --- Diagnostic Dive ------------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt28) % "SN" % "dive" % "idx" % "midx" % "DDP" % "DDS" % "DDF"
                   << endl;
                os << format(fmt29) % "" % "" % "" % "" % "(dBar)" % "(sec)" % "(sec)" << endl;
                os << format(fmt30) % whoi % dive % idx % midx++ // midx
                           % ddp() % dds() % ddf()
                   << endl;

                static const format fmt13("#   %5s %4s %4s %4s %11s %11s %11s");
                static const format fmt14("#    %5s %4s %4s %4s %11s %11s %11s");
                static const format fmt15("mis %05d %4d %4d %4d %11.2f %11.2f %11.2f");
                os << "# --- Built-In Test (BIT) --------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt13) % "SN" % "dive" % "idx" % "midx" % "PNB" % "PNC" % "PXC"
                   << endl;
                os << format(fmt14) % "" % "" % "" % "" % "(V)" % "(psi)" % "(psi)" << endl;
                os << format(fmt15) % whoi % dive % idx % midx++ // midx
                           % pnb_v() % pnc_psi() % pxc_psi()
                   << endl;

                static const format fmt16("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt17("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt18("mis %05d %4d %4d %4d %11d %11d %11d %11d %11d %11d");
                os << "# --- Calibration ----------------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt16) % "SN" % "dive" % "idx" % "midx" % "PBL" % "PBH" % "PP0" % "PP1"
                           % "PVL" % "PVH"
                   << endl;
                os << format(fmt17) % "" % "" % "" % "" % "(LSB/12V)" % "(LSB/14V)" % "(LSB/0A)"
                           % "(LSB/1A)" % "(LSB/12V)" % "(LSB/14V)"
                   << endl;
                os << format(fmt18) % whoi % dive % idx % midx++ // midx
                           % pbl() % pbh() % pp0() % pp1() % pvl() % pvh()
                   << endl;

                // Minor index 5.
                static const format fmt19("#   %5s %4s %4s %4s %11s %11s %11s %11s");
                static const format fmt20("#    %5s %4s %4s %4s %11s %11s %11s %11s");
                static const format fmt21("mis %05d %4d %4d %4d %11d %11d %11d %11d");
                os << format(fmt19) % "SN" % "dive" % "idx" % "midx" % "PV0" % "PV5" % "PC0" % "PC1"
                   << endl;
                os << format(fmt20) % "" % "" % "" % "" % "(LSB/0inHg)" % "(LSB/5inHg)" % "(LSB/0A)"
                           % "(LSB/1A)"
                   << endl;
                os << format(fmt21) % whoi % dive % idx % midx++ // midx
                           % pv0() % pv5() % pc0() % pc1()
                   << endl;

                static const format fmt22("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt23("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt24("mis %05d %4d %4d %4d %11d %11d %11.3f %11d %11d %11d");
                os << "# --- Profile Configuration ------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt22) % "SN" % "dive" % "idx" % "midx" % "CPS" % "CST" % "CSP"
                           % "CPB1" % "CPB2" % "CPB3"
                   << endl;
                os << format(fmt23) % "" % "" % "" % "" % "" % "" % "(sec)" % "(dBar)" % "(dBar)"
                           % "(dBar)"
                   << endl;
                os << format(fmt24) % whoi % dive % idx % midx++ // midx
                           % (int) cps() % (int) cst() % csp_sec() % cpb1() % cpb2() % cpb3()
                   << endl;

                static const format fmt25("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt26("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt27(
                        "mis %05d %4d %4d %4d %11.1f %11.1f %11.1f %11.3f %11.3f %11.3f");
                os << format(fmt25) % "SN" % "dive" % "idx" % "midx" % "CPN1" % "CPN2" % "CPN3"
                           % "CPP1" % "CPP2" % "CPP3"
                   << endl;
                os << format(fmt26) % "" % "" % "" % "" % "(dBar)" % "(dBar)" % "(dBar)" % "(sec)"
                           % "(sec)" % "(sec)"
                   << endl;
                os << format(fmt27) % whoi % dive % idx
                           % midx++ // midx
                           % cpn1_dbar() % cpn2_dbar() % cpn3_dbar() % cpp1_sec() % cpp2_sec()
                           % cpp3_sec()
                   << endl;

                static const format fmt73("#   %5s %4s %4s %4s "
                        "%11s %11s %11s");
                static const format fmt74("#    %5s %4s %4s %4s "
                        "%11s %11s %11s");
                static const format fmt75("mis %05d %4d %4d %4d "
                        "%11d %11d %11d");
                os << format(fmt73) % "SN" % "dive" % "idx" % "midx" //
                           % "CTF" % "CTL" % "CTU"
                   << endl;
                os << format(fmt74) % "" % "" % "" % "" //
                           % "(dBar)" % "(dBar)" % "(dBar)"
                   << endl;
                os << format(fmt75) % whoi % dive % idx % midx++ // midx
                           % ctf() % ctl() % ctu()
                   << endl;

                static const format fmt31("#   %5s %4s %4s %4s %11s %11s %11s %11s");
                static const format fmt32("#    %5s %4s %4s %4s %11s %11s %11s %11s");
                static const format fmt33("mis %05d %4d %4d %4d %11d %11d %11d %11d");
                os << "# --- Descent - General ----------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt31) % "SN" % "dive" % "idx" % "midx" % "DNS" % "DTD" % "PFR" % "PFC"
                   << endl;
                os << format(fmt32) % "" % "" % "" % "" % "" % "(dBar)" % "(mL/min)" % "(mL/dBar)"
                   << endl;
                os << format(fmt33) % whoi % dive % idx % midx++ // midx
                           % dns() % dtd() % pfr() % pfc()
                   << endl;

                static const format fmt34("#   %5s %4s %4s %4s %11s %11s");
                static const format fmt35("#    %5s %4s %4s %4s %11s %11s");
                static const format fmt36("mis %05d %4d %4d %4d %11d %11d");
                os << "# --- Descent - Stage 1 (start) --------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt34) % "SN" % "dive" % "idx" % "midx" % "DSD" % "DST" << endl;
                os << format(fmt35) % "" % "" % "" % "" % "(dBar)" % "(sec)" << endl;
                os << format(fmt36) % whoi % dive % idx % midx++ // midx
                           % dsd() % dst()
                   << endl;

                static const format fmt37("#   %5s %4s %4s %4s %11s %11s %11s %11s");
                static const format fmt38("#    %5s %4s %4s %4s %11s %11s %11s %11s");
                static const format fmt39("mis %05d %4d %4d %4d %11d %11d %11d %11.3f");
                os << "# --- Descent - Stage 2 (fall) ---------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt37) % "SN" % "dive" % "idx" % "midx" % "DFT" % "DDD" % "DDT" % "DSI"
                   << endl;
                os << format(fmt38) % "" % "" % "" % "" % "(sec)" % "(dBar)" % "(sec)" % "(sec)"
                   << endl;
                os << format(fmt39) % whoi % dive % idx % midx++ // midx
                           % dft() % ddd() % ddt() % dsi_sec()
                   << endl;

                static const format fmt40("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt41("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt42("mis %05d %4d %4d %4d %11d %11d %11d %11d %11d %11d");
                os << "# --- Descent - Stage 3 (calibration) --------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt40) % "SN" % "dive" % "idx" % "midx" % "DCT" % "DCM" % "DAL" % "DCA"
                           % "DSW" % "DUC"
                   << endl;
                os << format(fmt41) % "" % "" % "" % "" % "(sec)" % "" % "" % "" % "(sec)" % ""
                   << endl;
                os << format(fmt42) % whoi % dive % idx % midx++ // midx
                           % dct() % (int) dcm() % (int) dal() % (int) dca() % dsw() % duc()
                   << endl;

                static const format fmt43("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt44("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt45("mis %05d %4d %4d %4d %11d %11d %11d %11d %11d %11d");
                os << format(fmt43) % "SN" % "dive" % "idx" % "midx" % "DPT" % "DUT" % "DVT" % "DCX"
                           % "DCP" % "DCF"
                   << endl;
                os << format(fmt44) % "" % "" % "" % "" % "(sec)" % "(sec)" % "(sec)" % "(sec)"
                           % "(sec)" % "(sec)"
                   << endl;
                os << format(fmt45) % whoi % dive % idx % midx++ // midx
                           % dpt() % dut() % dvt() % dcx() % dcp() % dcf()
                   << endl;

                static const format fmt46("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt47("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt48("mis %05d %4d %4d %4d %11d %11d  0x%08X %11d %11.3f %11d");
                os << "# --- Park/Drift - General -------------------------------------------------"
                   "------------------------"
                   << endl;
                os << format(fmt46) % "SN" % "dive" % "idx" % "midx" % "PNS" % "PTO" % "PTH" % "PSA"
                           % "PSI" % "PFM"
                   << endl;
                os << format(fmt47) % "" % "" % "" % "" % "" % "(sec)" % "" % "" % "(sec)" % ""
                   << endl;
                os << format(fmt48) % whoi % dive % idx % midx++ // midx
                           % pns() % pto() % pth() % (int) psa() % psi_sec() % (int) pfm()
                   << endl;

                static const format fmt49("#   %5s %4s %4s %4s %11s %11s");
                static const format fmt50("#    %5s %4s %4s %4s %11s %11s");
                static const format fmt51("mis %05d %4d %4d %4d %11d %11d");
                os << format(fmt49) % "SN" % "dive" % "idx" % "midx" % "PFD" % "PDT" << endl;
                os << format(fmt50) % "" % "" % "" % "" % "(dBar)" % "(sec)" << endl;
                os << format(fmt51) % whoi % dive % idx % midx++ // midx
                           % pfd() % pdt()
                   << endl;

                static const format fmt52("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s");
                static const format fmt53("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s");
                static const format fmt54("mis %05d %4d %4d %4d %11s %11s %11s %11s %11s");
                os << "# --- Park/Drift - Seek ----------------------------------------------------"
                   "------------------------"
                   << endl << format(fmt52) % "SN" % "dive" % "idx" % "midx" % "PSM" % "PST" % "PSD"
                           % "PSV" % "PSW"
                   << endl << format(fmt53) % "" % "" % "" % "" % "" % "(sec)" % "(dBar)" % "(sec)"
                           % "(sec)"
                   << endl << format(fmt54) % whoi % dive % idx % midx++ // midx
                           % (int) psm() % pst() % psd() % psv() % psw()
                   << endl;

                static const format fmt55("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt56("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
                static const format fmt57("mis %05d %4d %4d %4d %11d %11.2f %11d %11d %11d %11.3f");
                os << "# --- Ascent/Profile -------------------------------------------------------"
                   "------------------------"
                   << endl << format(fmt55) % "SN" % "dive" % "idx" % "midx" % "ASN" % "ARR" % "APM"
                           % "APN" % "APF" % "ASI"
                   << endl << format(fmt56) % "" % "" % "" % "" % "" % "(m/sec)" % "(sec)" % "(sec)"
                           % "(sec)" % "(sec)"
                   << endl << format(fmt57) % whoi % dive % idx % midx++  // midx
                           % asn() % arr_m_per_s() % apm() % apn() % apf() % asi_sec()
                   << endl;

                {
                    static const format fmt1("#   %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt2("#    %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt3("mis %05d %4d %4d %4d %11d "
                            "%11d %11d "
                            "%11d %11.3f "
                            "%11d");
                    os << format(fmt1) % "SN" % "dive" % "idx" % "midx"  //
                               % "ASD" % "ADD" //
                               % "ADT" % "ARC" //
                               % "AHR" % "AHD"
                       << endl << format(fmt2) % "" % "" % "" % "" //
                               % "(dBar)" % "(dBar)" //
                               % "(sec)" % "(?)" //
                               % "(m/sec)" % "(dBar)"
                       << endl << format(fmt3) % whoi % dive % idx % midx++ // midx
                               % asd() % add() //
                               % adt() % arc() //
                               % ahr_m_per_s() % ahd()
                       << endl;
                }

                {
                    static const format fmt1("#   %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt2("#    %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%12s %11s");
                    static const format fmt3("mis %05d %4d %4d %4d "
                            "%11d      0x%04X "
                            "%11d %11d "
                            "%11.2f %11d");
                    os << "# --- Ascent - Ice-Detection -------------------------------------------"
                       "----------------------------"
                       << endl << format(fmt1) % "SN" % "dive" % "idx" % "midx" //
                               % "ICM" % "IMM" //
                               % "ICN" % "ICX" //
                               % "ICT" % "ICS"
                       << endl << format(fmt2) % "" % "" % "" % "" //
                               % "" % "" //
                               % "(dBar)" % "(dBar)" //
                               % "(degC)" % "(sec)"
                       << endl << format(fmt3) % whoi % dive % idx % midx++ // midx
                               % (int) icm() % imm() //
                               % icn() % icx() //
                               % ict_degc() % ics()
                       << endl;
                }

                {
                    static const format fmt1("#   %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt2("#    %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt3("mis %05d %4d %4d %4d "
                            "%11d %11d "
                            "%11.2f %11d "
                            " 0x%08X  0x%08X");
                    os << format(fmt1) % "SN" % "dive" % "idx" % "midx" //
                               % "ISP" % "ISM" //
                               % "IST" % "ISO" //
                               % "ICF" % "ICW"
                       << endl << format(fmt2) % "" % "" % "" % "" //
                               % "" % "" //
                               % "(PSU)" % "(sec)" //
                               % "" % "" //
                       << endl << format(fmt3) % whoi % dive % idx % midx++ // midx
                               % (int) isp() % (int) ism() //
                               % ist_psu() % iso() //
                               % icf() % icw()
                       << endl;
                }

                {
                    static const format fmt1("#   %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt2("#    %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt3("mis %05d %4d %4d %4d "
                            "%11d %11d "
                            "%11d %11d "
                            "%11d %11d");
                    os << "# --- Ascent - Ice-Detection (Summer) ----------------------------------"
                       "----------------------------"
                       << endl << format(fmt1) % "SN" % "dive" % "idx" % "midx" //
                               % "ISS0" % "ICA0" //
                               % "ICR0" % "ICD0" //
                               % "ICO0" % "ICG0" //
                       << endl << format(fmt2) % "" % "" % "" % "" //
                               % "" % "" //
                               % "(min)" % "(dBar)" //
                               % "(sec)" % "" //
                       << endl << format(fmt3) % whoi % dive % idx % midx++ // midx
                               % (int) iss0() % ica0() //
                               % icr0() % icd0() //
                               % ico0() % (int) icg0()
                       << endl;
                }

                {
                    static const format fmt1("#   %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt2("#    %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt3("mis %05d %4d %4d %4d "
                            "%11d %11d "
                            "%11d %11d");
                    os << format(fmt1) % "SN" % "dive" % "idx" % "midx" //
                               % "IUE0" % "ICB" //
                               % "IMT0" % "ILT0" //
                       << endl << format(fmt2) % "" % "" % "" % "" //
                               % "" % "" //
                               % "(%)" % "(sec)" //
                       << endl << format(fmt3) % whoi % dive % idx % midx++ // midx
                               % (int) iue0() % icb0() //
                               % imt0() % ilt0() //
                       << endl;
                }

                {
                    static const format fmt1("#   %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt2("#    %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt3("mis %05d %4d %4d %4d "
                            "%11d %11d "
                            "%11d %11d "
                            "%11d %11d");
                    os << "# --- Ascent - Ice-Detection (Winter) ----------------------------------"
                       "----------------------------"
                       << endl << format(fmt1) % "SN" % "dive" % "idx" % "midx" //
                               % "ISS1" % "ICA1" //
                               % "ICR1" % "ICD1" //
                               % "ICO1" % "ICG1" //
                       << endl << format(fmt2) % "" % "" % "" % "" //
                               % "" % "" //
                               % "(min)" % "(dBar)" //
                               % "(sec)" % "" //
                       << endl << format(fmt3) % whoi % dive % idx % midx++ // midx
                               % (int) iss1() % ica1() //
                               % icr1() % icd1() //
                               % ico1() % (int) icg1()
                       << endl;
                }

                {
                    static const format fmt1("#   %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt2("#    %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt3("mis %05d %4d %4d %4d "
                            "%11d %11d "
                            "%11d %11d");
                    os << format(fmt1) % "SN" % "dive" % "idx" % "midx" //
                               % "IUE1" % "ICB1" //
                               % "IMT1" % "ILT1" //
                       << endl << format(fmt2) % "" % "" % "" % "" //
                               % "" % "" //
                               % "(%)" % "(sec)" //
                       << endl << format(fmt3) % whoi % dive % idx % midx++ // midx
                               % (int) iue1() % icb1() //
                               % imt1() % ilt1() //
                       << endl;
                }

                {
                    static const format fmt1("#   %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt2("#    %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s %11s");
                    static const format fmt3("mis %05d %4d %4d %4d "
                            "%11d %11d "
                            "%11d %11d "
                            "%11d %11.3f");
                    os << "# --- Surface --------------------------------------------------------------"
                       "------------------------"
                       << endl
                       << format(fmt1) % "SN" % "dive" % "idx" % "midx" //
                               % "SFN" % "SGT" //
                               % "SET" % "SDT" //
                               % "SSA" % "SSI"
                       << endl
                       << format(fmt2) % "" % "" % "" % "" //
                               % "" % "(min)" //
                               % "(min)" % "(?)" //
                               % "" % "(sec)"
                       << endl
                       << format(fmt3) % whoi % dive % idx % midx++ // midx
                               % sfn() % sgt() //
                               % set() % sdt() //
                               % ssa() % ssi_sec()
                       << endl;

                }

                {
                    static const format fmt1("#   %5s %4s %4s %4s "
                            "%11s %11s");
                    static const format fmt2("#    %5s %4s %4s %4s "
                            "%11s %11s");
                    static const format fmt3("mis %05d %4d %4d %4d "
                            "%11d %11d");
                    os << format(fmt1) % "SN" % "dive" % "idx" % "midx" //
                               % "SDS" % "SPT"
                       << endl;
                    os << format(fmt2) % "" % "" % "" % "" //
                               % "(sec)" % "(sec)"
                       << endl;
                    os << format(fmt3) % whoi % dive % idx % midx++ // midx
                               % sds() % spt()
                       << endl;
                }

                {
                    static const format fmt1("#   %5s %4s %4s %4s "
                            "%11s %11s %11s "
                            "%11s %11s");
                    static const format fmt2("#    %5s %4s %4s %4s "
                            "%11s %11s %11s "
                            "%11s %11s");
                    static const format fmt3("mis %05d %4d %4d %4d "
                            "%11d %11.3f %11.3f "
                            "%11d %11d");
                    os << "# --- Surface - Air Pump -----------------------------------------------"
                       "----------------------------"
                       << endl << format(fmt1) % "SN" % "dive" % "idx" % "midx" //
                               % "SAM" % "SAX" % "SAN" //
                               % "SAO" % "SAP" //
                       << endl << format(fmt2) % "" % "" % "" % "" //
                               % "" % "(psi)" % "(psi)" //
                               % "(sec)" % "(sec)"
                       << endl << format(fmt3) % whoi % dive % idx % midx++ // midx
                               % (int) sam() % sax_psi() % san_psi() //
                               % sao() % sap()
                       << endl;
                }

                {
                    static const format fmt1("#   %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s");
                    static const format fmt2("#    %5s %4s %4s %4s "
                            "%11s %11s "
                            "%11s %11s "
                            "%11s");
                    static const format fmt3("mis %05d %4d %4d %4d "
                            "%11d %11d "
                            "%11d %11d "
                            "%11d");
                    os << "# --- Surface - Wave-Field Measurement -------------------------------------"
                       "------------------------"
                       << endl
                       << format(fmt1) % "SN" % "dive" % "idx" % "midx" //
                               % "MAM" % "MAT" //
                               % "ACA" % "ACG" //
                               % "ACR"
                       << endl
                       << format(fmt2) % "" % "" % "" % "" //
                               % "" % "(sec)" //
                               % "(g/FSR)" % "(°/sec/FSR)" //
                               % "(Hz)"
                       << endl
                       << format(fmt3) % whoi % dive % idx % midx++ // midx
                               % (int) mam() % mat() //
                               % aca() % acg() //
                               % acr()
                       << endl;
                }

                {
                    static const format fmt1("#   %5s %4s %4s %4s "
                            "%11s %11s");
                    static const format fmt2("#    %5s %4s %4s %4s "
                            "%11s %11s");
                    static const format fmt3("mis %05d %4d %4d %4d "
                            "%11d %11d");
                    os << "# --- Surface - Abort/Beacon Mode ------------------------------------------"
                       "------------------------"
                       << endl
                       << format(fmt1) % "SN" % "dive" % "idx" % "midx" //
                               % "SBM" % "SBT"
                       << endl
                       << format(fmt2) % "" % "" % "" % "" //
                               % "" % "(sec)"
                       << endl
                       << format(fmt3) % whoi % dive % idx % midx++ // midx
                               % (int) sbm() % sbt()
                       << endl;
                }

            }

                break;

            case 0x04026200: // 2017 PAR floats
            case 0x04036100: // ALAMO Software version 0x04036100 (2017 S2h test float 10108).
            case 0x04046100: // ALAMO Software version 0x04046100 (2017 Hurricane Floats).
            case 0x04056300: // ALAMO Software version 0x04056300 (2017 S2h test float 10108).
            case 0x04066100: // ALAMO Software version 0x04066100 (2017 S2h test float 10108).

            {   int stype;
                string stype_str;
                switch(stype = sensor_type()) {
                case 0:     stype_str = "RBR";                          break;
                case 1:     stype_str = "SBE";                          break;
                default:    stype_str = "unknown"; stype = -99;         break; }
                {   // Header line.
                    static const format fmt1("#   %5s %4s %4s %16s %11s %11s        %s");
                    static const format fmt3("MIS %05d %4d %4d             0x%02X "                                     " 0x%08X "      "%11d "         "     # %s"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "idx"     % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
                    static const string str2("#");
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % idx       % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
                }
                os << "# --- General --------------------------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s"          "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s"          "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d"          "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "MSN"         % "MDS"         % "APT"         % "XAS"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(dBar)"      % ""            % ""            ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % msn()         % mds()         % apt()         %  xas()        << endl;
                }
                os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11.3f"        );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "MTD"         % "MTT"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % mtd()         % mtt_sec()     << endl;
                }
                os << "# --- Diagnostic Dive ------------------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "DDP"         % "DDS"         % "DDF"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(sec)"       % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % ddp()         % dds()         % ddf()         << endl;
                }
                os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11.2f "       "%11.2f "       "%11.2f"        );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PNB"         % "PNC"         % "PXC"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(V)"         % "(psi)"       % "(psi)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % pnb_v()       % pnc_psi()     % pxc_psi()     << endl;
                }
                os << "# --- Calibration Coefficients ---------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "        "%11s "         "%11s "         "%11s "         "%11s "         "%11s"           );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "        "%11s "         "%11s "         "%11s "         "%11s "         "%11s"           );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "        "%11d "         "%11d "         "%11d "         "%11d "         "%11d"           );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PBL"         % "PBH"         % "PP0"         % "PP1"         % "PVL"         % "PVH"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(LSB/12V)"   % "(LSB/14V)"   % "(LSB/0A)"    % "(LSB/1A)"    % "(LSB/12V)"   % "(LSB/14V)"   ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % pbl()         % pbh()         % pp0()         % pp1()         % pvl()         % pvh()         << endl;
                }
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PV0"         % "PV5"         % "PC0"         % "PC1"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(LSB/0inHg)" % "(LSB/5inHg)" % "(LSB/0A)"    % "(LSB/1A)"    ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % pv0()         % pv5()         % pc0()         % pc1()         << endl;
                }
                os << "# --- Profile --------------------------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11.3f "       "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "CPS"         % "CST"         % "CSP"         % "CTF"         % "CTL"         % "CTU"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(sec)"       % "(dBar)"      % "(dBar)"      % "(dBar)"      ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) cps()   % (int) cst()   % csp_sec()     % ctf()         % ctl()         % ctu()         << endl;
                }
                os << "# --- Profile - Binning Breakpoints ----------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "CPB1"        % "CPB2"        % "CPB3"        ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(dBar)"      % "(dBar)"      ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % cpb1()        % cpb2()        % cpb3()        << endl;
                }
                os << "# --- Profile - Binning Configuration --------------------------------------------------------------"            << endl;
                switch(sensor_type()) {
                case 0: { // RBR
                    static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "        "%11s "          "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "        "%11s "          "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11.1f "      "%11.1f "        "%11.1f "       "%11.3f "       "%11.3f "       "%11.3f"        );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "CPN1"        % "CPN2"        % "CPN3"        % "CPP1"        % "CPP2"        % "CPP3"        ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(dBar)"      % "(dBar)"      % "(sec)"       % "(sec)"       % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % cpn1_dbar()   % cpn2_dbar()   % cpn3_dbar()   % cpp1_sec()    % cpp2_sec()    % cpp3_sec()    << endl;
                }
                    break;
                case 1:  { // SBE
                    static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "CPN1"        % "CPN2"        % "CPN3"        % "CPP1"        % "CPP2"        % "CPP3"        ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(n/a)"       % "(n/a)"       % "(n/a)"       % "(dBar)"      % "(dBar)"      % "(dBar)"      ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % cpn1()        % cpn2()        % cpn3()        % cpp1()        % cpp2()        % cpp3()        << endl;
                }
                    break;
                default:
                    static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "CPN1"        % "CPN2"        % "CPN3"        % "CPP1"        % "CPP2"        % "CPP3"        ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % ""            % ""            % ""            % ""            ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % cpn1()        % cpn2()        % cpn3()        % cpp1()        % cpp2()        % cpp3()        << endl;
                    break;
                }
                os << "# --- Pump Characteristics -------------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PFR"         % "PFC"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(mL/min)"    % "(mL/dBar)"   ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % pfr()         % pfc()         << endl;
                }
                os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "        "%11s"           );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "        "%11s"           );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "        "%11.3f"         );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "DNS"         % "DTD"         % "DDD"         % "DDT"        % "DSI"          ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(dBar)"      % "(dBar)"      % "(sec)"      % "(sec)"        ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % dns()         % dtd()         % ddd()         % ddt()        % dsi_sec()      << endl;
                }
                os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "DSD"         % "DST"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % dsd()         % dst()         << endl;
                }
                os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "DFT"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % dft()         << endl;
                }
                os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "DCT"         % "DCM"         % "DAL"         % "DCA"         % "DSW"         % "DUC"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(sec)"       % ""            % ""            % ""            % "(sec)"       % ""            ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % dct()         % (int) dcm()   % (int) dal()   % (int) dca()   % dsw()         % duc()         << endl;
                }
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "DPT"         % "DUT"         % "DVT"         % "DCX"         % "DCP"         % "DCF"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(sec)"       % "(sec)"       % "(sec)"       % "(sec)"       % "(sec)"       % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % dpt()         % dut()         % dvt()         % dcx()         % dcp()         % dcf()         << endl;
                }
                os << "# --- Park/Drift - General -------------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         " 0x%08X "      "%11d "         "%11.3f "       "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PNS"         % "PTO"         % "PTH"         % "PSA"         % "PSI"         % "PFM"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(sec)"       % ""            % ""            % "(sec)"       % ""            ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % pns()         % pto()         % pth()         % (int) psa()   % psi_sec()     % (int) pfm()   << endl;
                }
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PFD"         % "PDT"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % pfd()         % pdt()         << endl;
                }
                os << "# --- Park/Drift - Seek ----------------------------------------------------------------------------"            << endl;
                {
                    static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PSM"         % "PST"         % "PSD"         % "PSV"         % "PSW"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(sec)"       % "(dBar)"      % "(sec)"       % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) psm()   % pst()         % psd()         % psv()         % psw()         << endl;
                }
                os << "# --- Ascent/Profile -------------------------------------------------------------------------------"            << endl;
                {
                    static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11.2f "       "%11d "         "%11d "         "%11d "         "%11.3f"        );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "ASN"         % "ARR"         % "APM"         % "APN"         % "APF"         % "ASI"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(m/sec)"     % "(sec)"       % "(sec)"       % "(sec)"       % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % asn()         % arr_m_per_s() % apm()         % apn()         % apf()         % asi_sec()     << endl;
                }
                {
                    static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11.3f "       "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "ASD"         % "ADD"         % "ADT"         % "ARC"         % "AHR"         % "AHD"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(dBar)"      % "(sec)"       % "(?)"         % "(m/sec)"     % "(dBar)"      ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % asd()         % add()         % adt()         % arc()         % ahr_m_per_s() % ahd()         << endl;
                }
                os << "# --- Ascent - Ice-Detection -----------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%12s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "     0x%04X "  "%11d "         "%11d "         "%11.2f "       "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "ICM"         % "IMM"         % "ICN"         % "ICX"         % "ICT"         % "ICS"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(dBar)"      % "(dBar)"      % "(degC)"        % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) icm()   % imm()         % icn()         % icx()         % ict_degc()    % ics()         << endl;
                }
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11.2f "       "%11d "         " 0x%08X "      " 0x%08X"       );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "ISP"         % "ISM"         % "IST"         % "ISO"         % "ICF"         % "ICW"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(PSU)"       % "(sec)"       % ""            % ""            ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) isp()   % (int) ism()   % ist_psu()     % iso()         % icf()         % icw()         << endl;
                }
                os << "# --- Ascent - Ice-Detection (Summer) --------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "ISS0"        % "ICA0"        % "ICR0"        % "ICD0"        % "ICO0"        % "ICG0"        ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(min)"       % "(dBar)"      % "(sec)"       % ""            ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) iss0()  % ica0()        % icr0()        % icd0()        % ico0()        % (int) icg0()  << endl;
                }
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "IUE0"        % "ICB"         % "IMT0"        % "ILT0"        ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(%)"         % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) iue0()  % icb0()        % imt0()        % ilt0()        << endl;
                }
                os << "# --- Ascent - Ice-Detection (Winter) --------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "ISS1"        % "ICA1"        % "ICR1"        % "ICD1"        % "ICO1"        % "ICG1"        ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(min)"       % "(dBar)"      % "(sec)"       % ""            ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) iss1()  % ica1()        % icr1()        % icd1()        % ico1()        % (int) icg1()  << endl;
                }
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "IUE1"        % "ICB1"        % "IMT1"        % "ILT1"        ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(%)"         % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) iue1()  % icb1()        % imt1()        % ilt1()        << endl;
                }
                os << "# --- Surface --------------------------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11.3f"        );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "SFN"         % "SGT"         % "SET"         % "SDT"         % "SSA"         % "SSI"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(min)"       % "(min)"       % "(?)"         % ""            % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % sfn()         % sgt()         % set()         % sdt()         % ssa()         % ssi_sec()     << endl;
                }
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "SDS"         % "SPT"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(sec)"       % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % sds()         % spt()         << endl;
                }
                os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11.3f "       "%11.3f "       "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "SAM"         % "SAX"         % "SAN"         % "SAO"         % "SAP"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(psi)"       % "(psi)"       % "(sec)"       % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) sam()   % sax_psi()     % san_psi()     % sao()         % sap()         << endl;
                }
                os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "MAM"         % "MAT"         % "ACA"         % "ACG"         % "ACR"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(sec)"       % "(g/FSR)"     % "(°/sec/FSR)" % "(Hz)"        ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) mam()   % mat()         % aca()         % acg()         % acr()         << endl;
                }
                os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
                {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s"          );
                    static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s"          );
                    static const format fmt3("mis %05d %4d %4d %4d "                                                    "%11d "         "%11d"          );
                    static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "SBM"         % "SBT"         ));
                    static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(sec)"       ));
                    os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) sbm()   % sbt()         << endl;
                }

            }

                break;

            case 0x05006100: // ALAMO Software version 0x05006100 (2017 Alto test float 10109).

              break;


            default: {          // Unsupported ALAMO software version.
                ostringstream os;
                os << "Unsupported ALAMO Mission Parameters message version: 0x"
                   << hex
                   << setw(8)
                   << setfill('0')
                   << fw_version()
                   << "!";
                EXC_THROW(os.str());
            }

                break;

            }

        }CATCH_BLOCK("Failed to generate listing string for ALAMO mission parameters object!")

        return os.str();
    }

public:

    /* Public Accessor Methods: */

    uint16_t aca() const {
        return aca_;
    }

    uint16_t acg() const {
        return acg_;
    }

    uint16_t acr() const {
        return acr_;
    }

    uint16_t add() const {
        return add_;
    }

    uint16_t adt() const {
        return adt_;
    }

    uint16_t ahd() const {
        return ahd_;
    }

    uint16_t ahr() const {
        return ahr_;
    }

    uint16_t apf() const {
        return apf_;
    }

    uint16_t apm() const {
        return apm_;
    }

    uint16_t apn() const {
        return apn_;
    }

    unsigned int apt() const {
        return apt_;
    }

    uint16_t arc() const {
        return arc_;
    }

    uint16_t arr() const {
        return arr_;
    }

    const ptime& ascent_start_time() const {
        return ascent_start_time_;
    }

    uint16_t asd() const {
        return asd_;
    }

    uint32_t asi() const {
        return asi_;
    }

    uint16_t asn() const {
        return asn_;
    }

    uint16_t cpp1() const {
        return cpp_[0];
    }

    uint16_t cpp2() const {
        return cpp_[1];
    }

    uint16_t cpp3() const {
        return cpp_[2];
    }

    uint8_t cps() const {
        return cps_;
    }

    uint32_t csp() const {
        return csp_;
    }

    uint8_t cst() const {
        return cst_;
    }

    uint16_t ctf() const {
        return ctf_;
    }

    uint16_t ctl() const {
        return ctl_;
    }

    uint16_t ctu() const {
        return ctu_;
    }

    uint8_t dal() const {
        return dal_;
    }

    uint8_t dca() const {
        return dca_;
    }

    uint16_t dcd() const {
        return dcd_;
    }

    uint16_t dcf() const {
        return dcf_;
    }

    uint8_t dcm() const {
        return dcm_;
    }

    uint16_t dcp() const {
        return dcp_;
    }

    uint16_t dcr() const {
        return dcr_;
    }

    uint16_t dct() const {
        return dct_;
    }

    uint16_t dcx() const {
        return dcx_;
    }

    uint16_t ddc() const {
        return ddc_;
    }

    uint16_t ddd() const {
        return ddd_;
    }

    uint16_t ddf() const {
        return ddf_;
    }

    uint16_t ddp() const {
        return ddp_;
    }

    uint16_t dds() const {
        return dds_;
    }

    uint16_t ddt() const {
        return ddt_;
    }

    const ptime& descend_start_time() const {
        return descend_start_time_;
    }

    uint16_t dft() const {
        return dft_;
    }

    uint16_t dns() const {
        return dns_;
    }

    uint16_t dpd() const {
        return dpd_;
    }

    uint16_t dpt() const {
        return dpt_;
    }

    uint8_t dra() const {
        return dra_;
    }

    uint16_t drc() const {
        return drc_;
    }

    uint16_t drt() const {
        return drt_;
    }

    uint16_t dsd() const {
        return dsd_;
    }

    uint32_t dsi() const {
        return dsi_;
    }

    uint16_t dst() const {
        return dst_;
    }

    uint16_t dsw() const {
        return dsw_;
    }

    uint16_t dtd() const {
        return dtd_;
    }

    uint16_t duc() const {
        return duc_;
    }

    uint16_t dut() const {
        return dut_;
    }

    uint16_t dvd() const {
        return dvd_;
    }

    uint16_t dvw() const {
        return dvw_;
    }

    uint16_t dvt() const {
        return dvt_;
    }

    uint16_t fos() const {
        return fos_;
    }

    uint8_t fta() const {
        return fta_;
    }

    uint8_t ftm() const {
        return ftm_;
    }

    uint16_t fto() const {
        return fto_;
    }

    uint8_t mam() const {
        return mam_;
    }

    uint16_t mat() const {
        return mat_;
    }

    uint16_t mds() const {
        return mds_;
    }

    unsigned int msg_version() const {
        return msg_version_;
    }

    int16_t msn() const {
        return msn_;
    }

    uint16_t mtd() const {
        return mtd_;
    }

    uint16_t mtt() const {
        return mtt_;
    }

    const ptime& park_start_time() const {
        return park_start_time_;
    }

    uint16_t pbh() const {
        return pbh_;
    }

    uint16_t pbl() const {
        return pbl_;
    }

    uint16_t pc0() const {
        return pc0_;
    }

    uint16_t pc1() const {
        return pc1_;
    }

    uint16_t pdt() const {
        return pdt_;
    }

    uint16_t pfc() const {
        return pfc_;
    }

    uint16_t pfd() const {
        return pfd_;
    }

    uint8_t pfm() const {
        return pfm_;
    }

    uint16_t pfr() const {
        return pfr_;
    }

    uint16_t pft() const {
        return pft_;
    }

    uint16_t pfv() const {
        return pfv_;
    }

    uint16_t pnb() const {
        return pnb_;
    }

    uint16_t pnc() const {
        return pnc_;
    }

    uint16_t pns() const {
        return pns_;
    }

    uint16_t pp0() const {
        return pp0_;
    }

    uint16_t pp1() const {
        return pp1_;
    }

    uint8_t psa() const {
        return psa_;
    }

    uint16_t psd() const {
        return psd_;
    }

    uint32_t psi() const {
        return psi_;
    }

    uint8_t psm() const {
        return psm_;
    }

    uint16_t pst() const {
        return pst_;
    }

    uint16_t psv() const {
        return psv_;
    }

    uint16_t psw() const {
        return psw_;
    }

    uint32_t pth() const {
        return pth_;
    }

    uint32_t pto() const {
        return pto_;
    }

    uint16_t pv0() const {
        return pv0_;
    }

    uint16_t pv5() const {
        return pv5_;
    }

    uint16_t pvh() const {
        return pvh_;
    }

    uint16_t pvl() const {
        return pvl_;
    }

    uint16_t pxc() const {
        return pxc_;
    }

    uint8_t sam() const {
        return sam_;
    }

    uint16_t san() const {
        return san_;
    }

    uint16_t sao() const {
        return sao_;
    }

    uint16_t sap() const {
        return sap_;
    }

    uint16_t sax() const {
        return sax_;
    }

    uint8_t sbm() const {
        return sbm_;
    }

    uint16_t sbt() const {
        return sbt_;
    }

    int16_t sdc() const {
        return sdc_;
    }

    uint16_t sds() const {
        return sds_;
    }

    uint32_t sdt() const {
        return sdt_;
    }

    int sensor_type() const {
        return sensor_type_;
    }

    uint16_t set() const {
        return set_;
    }

    uint16_t sfn() const {
        return sfn_;
    }

    uint16_t sgt() const {
        return sgt_;
    }

    uint32_t fw_version() const {
        return fw_version_;
    }

    uint16_t spt() const {
        return spt_;
    }

    uint16_t ssa() const {
        return ssa_;
    }

    uint32_t ssi() const {
        return ssi_;
    }

    uint16_t ssp() const {
        return ssp_;
    }

    const ptime& surface_start_time() const {
        return surface_start_time_;
    }

    int xas() const {
        return xas_;
    }

    template<typename T = float> T pnb_v() const {
        return (T) pnb() / 100.;
    }

    template<typename T = float> T pnc_psi() const {
        return (T) pnc() / 100.;
    }

    template<typename T = float> T pxc_psi() const {
        return (T) pxc() / 100.;
    }

    template<typename T = float> T mtt_sec() const {
        return (T) mtt() / 1000.;
    }

    template<typename T = float> T csp_sec() const {
        return (T) csp() / 1000.;
    }

    uint16_t cpb1() const {
        return cpb_[0];
    }

    uint16_t cpb2() const {
        return cpb_[1];
    }

    uint16_t cpb3() const {
        return cpb_[2];
    }

    uint16_t cpn1() const {
        return cpn_[0];
    }

    uint16_t cpn2() const {
        return cpn_[1];
    }

    uint16_t cpn3() const {
        return cpn_[2];
    }

    uint32_t icf() const {
        return icf_;
    }

    uint8_t icg0() const {
        return icg_[0];
    }

    uint8_t icg1() const {
        return icg_[1];
    }

    uint8_t icm() const {
        return icm_;
    }

    uint16_t icn() const {
        return icn_;
    }

    uint16_t ics() const {
        return ics_;
    }

    int16_t ict() const {
        return ict_;
    }

    uint32_t icw() const {
        return icw_;
    }

    uint16_t icx() const {
        return icx_;
    }

    uint16_t imm() const {
        return imm_;
    }

    uint16_t iso() const {
        return iso_;
    }

    uint8_t isp() const {
        return isp_;
    }

    uint16_t ist() const {
        return ist_;
    }

    uint16_t iat() const {
        return iat_;
    }

    uint16_t isd() const {
        return isd_;
    }

    uint16_t ica0() const {
        return ica_[0];
    }

    uint16_t icb0() const {
        return icb_[0];
    }

    uint16_t icd0() const {
        return icd_[0];
    }

    uint16_t ilt0() const {
        return ilt_[0];
    }

    uint16_t imt0() const {
        return imt_[0];
    }

    uint8_t iss0() const {
        return iss_[0];
    }

    uint8_t iue0() const {
        return iue_[0];
    }

    uint16_t ico0() const {
        return ico_[0];
    }

    uint32_t icr0() const {
        return icr_[0];
    }

    uint16_t ica1() const {
        return ica_[1];
    }

    uint16_t icb1() const {
        return icb_[1];
    }

    uint16_t icd1() const {
        return icd_[1];
    }

    uint16_t ilt1() const {
        return ilt_[1];
    }

    uint16_t imt1() const {
        return imt_[1];
    }

    uint8_t isa() const {
        return isa_;
    }

    uint16_t isv() const {
        return isv_;
    }

    uint16_t isw() const {
        return isw_;
    }

    uint8_t iss1() const {
        return iss_[1];
    }

    uint8_t iue1() const {
        return iue_[1];
    }

    uint16_t ico1() const {
        return ico_[1];
    }

    uint32_t icr1() const {
        return icr_[1];
    }

    uint8_t ism() const {
        return ism_;
    }

    uint8_t btm() const {
        return btm_;
    }

    template<typename T = float> T cpn1_dbar() const {
        return (T) cpn1() / 10.;
    }

    template<typename T = float> T cpn2_dbar() const {
        return (T) cpn2() / 10.;
    }

    template<typename T = float> T cpn3_dbar() const {
        return (T) cpn3() / 10.;
    }

    template<typename T = float> T cpp1_sec() const {
        return (T) cpp1() / 1000.;
    }

    template<typename T = float> T cpp2_sec() const {
        return (T) cpp2() / 1000.;
    }

    template<typename T = float> T cpp3_sec() const {
        return (T) cpp3() / 1000.;
    }

    template<typename T = float> T dra_m_per_s() const {
        return (T) dra() / 100.;
    }

    template<typename T = float> T dsi_sec() const {
        return (T) dsi() / 1000.;
    }

    template<typename T = float> T psi_sec() const {
        return (T) psi() / 1000.;
    }

    template<typename T = float> T arr_m_per_s() const {
        return (T) arr() / 100.;
    }

    template<typename T = float> T asi_sec() const {
        return (T) asi() / 1000.;
    }

    template<typename T = float> T ahr_m_per_s() const {
        return (T) ahr() / 1000.;
    }

    template<typename T = float> T ssi_sec() const {
        return (T) ssi() / 1000.;
    }

    template<typename T = float> T san_psi() const {
        return (T) san() / 1000.;
    }

    template<typename T = float> T sax_psi() const {
        return (T) sax() / 1000.;
    }

    template<typename T = float> T ict_degc() const {
        return (T) ict() / 100.;
    }

    template<typename T = float> T ist_psu() const {
        return (T) ist() / 100.;
    }

protected:

    /* Protected Mutator Methods: */

    void set_aca(uint16_t aca) {
        aca_ = aca;
    }

    void set_acg(uint16_t acg) {
        acg_ = acg;
    }

    void set_acr(uint16_t acr) {
        acr_ = acr;
    }

    void set_add(uint16_t add) {
        add_ = add;
    }

    void set_adt(uint16_t adt) {
        adt_ = adt;
    }

    void set_ahd(uint16_t ahd) {
        ahd_ = ahd;
    }

    void set_ahr(uint16_t ahr) {
        ahr_ = ahr;
    }

    void set_apf(uint16_t apf) {
        apf_ = apf;
    }

    void set_apm(uint16_t apm) {
        apm_ = apm;
    }

    void set_apn(uint16_t apn) {
        apn_ = apn;
    }

    void set_apt(uint8_t apt) {
        apt_ = apt;
    }

    void set_arc(uint16_t arc) {
        arc_ = arc;
    }

    void set_arr(uint16_t arr) {
        arr_ = arr;
    }

    void set_ascent_start_time(const ptime& ascent_start_time) {
        ascent_start_time_ = ascent_start_time;
    }

    void set_asd(uint16_t asd) {
        asd_ = asd;
    }

    void set_asi(uint32_t asi) {
        asi_ = asi;
    }

    void set_asn(uint16_t asn) {
        asn_ = asn;
    }

    void set_btm(uint8_t btm) {
        btm_ = btm;
    }

    void set_cpb1(uint16_t cpb) {
        cpb_[0] = cpb;
    }

    void set_cpb2(uint16_t cpb) {
        cpb_[1] = cpb;
    }

    void set_cpb3(uint16_t cpb) {
        cpb_[2] = cpb;
    }


    void set_cpp1(uint16_t cpp) {
        cpp_[0] = cpp;
    }

    void set_cpp2(uint16_t cpp) {
        cpp_[1] = cpp;
    }

    void set_cpp3(uint16_t cpp) {
        cpp_[2] = cpp;
    }


    void set_cpn1(uint16_t cpn) {
        cpn_[0] = cpn;
    }

    void set_cpn2(uint16_t cpn) {
        cpn_[1] = cpn;
    }

    void set_cpn3(uint16_t cpn) {
        cpn_[2] = cpn;
    }


    void set_cps(uint8_t cps) {
        cps_ = cps;
    }

    void set_csp(uint32_t csp) {
        csp_ = csp;
    }

    void set_cst(uint8_t cst) {
        cst_ = cst;
    }

    void set_ctf(uint16_t ctf) {
        ctf_ = ctf;
    }

    void set_ctl(uint16_t ctl) {
        ctl_ = ctl;
    }

    void set_ctu(uint16_t ctu) {
        ctu_ = ctu;
    }

    void set_dal(uint8_t dal) {
        dal_ = dal;
    }

    void set_dca(uint8_t dca) {
        dca_ = dca;
    }

    void set_dcd(uint16_t dcd) {
        dcd_ = dcd;
    }

    void set_dcf(uint16_t dcf) {
        dcf_ = dcf;
    }

    void set_dcm(uint8_t dcm) {
        dcm_ = dcm;
    }

    void set_dcp(uint16_t dcp) {
        dcp_ = dcp;
    }

    void set_dcr(uint16_t dcr) {
        dcr_ = dcr;
    }

    void set_dct(uint16_t dct) {
        dct_ = dct;
    }

    void set_dcx(uint16_t dcx) {
        dcx_ = dcx;
    }

    void set_ddc(uint16_t ddc) {
        ddc_ = ddc;
    }

    void set_ddd(uint16_t ddd) {
        ddd_ = ddd;
    }

    void set_ddf(uint16_t ddf) {
        ddf_ = ddf;
    }

    void set_ddp(uint16_t ddp) {
        ddp_ = ddp;
    }

    void set_dds(uint16_t dds) {
        dds_ = dds;
    }

    void set_ddt(uint16_t ddt) {
        ddt_ = ddt;
    }

    void set_descend_start_time(const ptime& descend_start_time) {
        descend_start_time_ = descend_start_time;
    }

    void set_dft(uint16_t dft) {
        dft_ = dft;
    }

    void set_dns(uint16_t dns) {
        dns_ = dns;
    }

    void set_dpd(uint16_t dpd) {
        dpd_ = dpd;
    }

    void set_dpt(uint16_t dpt) {
        dpt_ = dpt;
    }

    void set_dra(uint8_t dra) {
        dra_ = dra;
    }

    void set_drc(uint16_t drc) {
        drc_ = drc;
    }

    void set_drt(uint16_t drt) {
        drt_ = drt;
    }

    void set_dsd(uint16_t dsd) {
        dsd_ = dsd;
    }

    void set_dsi(uint32_t dsi) {
        dsi_ = dsi;
    }

    void set_dst(uint16_t dst) {
        dst_ = dst;
    }

    void set_dsw(uint16_t dsw) {
        dsw_ = dsw;
    }

    void set_dtd(uint16_t dtd) {
        dtd_ = dtd;
    }

    void set_duc(uint16_t duc) {
        duc_ = duc;
    }

    void set_dut(uint16_t dut) {
        dut_ = dut;
    }

    void set_dvd(uint16_t dvd) {
        dvd_ = dvd;
    }

    void set_dvt(uint16_t dvt) {
        dvt_ = dvt;
    }

    void set_dvw(uint16_t dvw) {
            dvw_ = dvw;
        }

    void set_fos(uint16_t fos) {
        fos_ = fos;
    }

    void set_fta(uint8_t fta) {
        fta_ = fta;
    }

    void set_ftm(uint8_t ftm) {
        ftm_ = ftm;
    }

    void set_fto(uint16_t fto) {
        fto_ = fto;
    }

    void set_iat(uint16_t iat) {
        iat_ = iat;
    }

    void set_isd(uint16_t isd) {
        isd_ = isd;
    }

    void set_mam(uint8_t mam) {
        mam_ = mam;
    }

    void set_mat(uint16_t mat) {
        mat_ = mat;
    }

    void set_mds(uint16_t mds) {
        mds_ = mds;
    }

    void set_msg_version(uint8_t msg_version) {
        msg_version_ = msg_version;
    }

    void set_msn(int16_t msn) {
        msn_ = msn;
    }

    void set_mtd(uint16_t mtd) {
        mtd_ = mtd;
    }

    void set_mtt(uint16_t mtt) {
        mtt_ = mtt;
    }

    void set_park_start_time(const ptime& park_start_time) {
        park_start_time_ = park_start_time;
    }

    void set_pbh(uint16_t pbh) {
        pbh_ = pbh;
    }

    void set_pbl(uint16_t pbl) {
        pbl_ = pbl;
    }

    void set_pc0(uint16_t pc0) {
        pc0_ = pc0;
    }

    void set_pc1(uint16_t pc1) {
        pc1_ = pc1;
    }

    void set_pdt(uint16_t pdt) {
        pdt_ = pdt;
    }

    void set_pfc(uint16_t pfc) {
        pfc_ = pfc;
    }

    void set_pfd(uint16_t pfd) {
        pfd_ = pfd;
    }

    void set_pfm(uint8_t pfm) {
        pfm_ = pfm;
    }

    void set_pfr(uint16_t pfr) {
        pfr_ = pfr;
    }

    void set_pft(uint16_t pft) {
        pft_ = pft;
    }

    void set_pfv(uint16_t pfv) {
        pfv_ = pfv;
    }

    void set_pnb(uint16_t pnb) {
        pnb_ = pnb;
    }

    void set_pnc(uint16_t pnc) {
        pnc_ = pnc;
    }

    void set_pns(uint16_t pns) {
        pns_ = pns;
    }

    void set_pp0(uint16_t pp0) {
        pp0_ = pp0;
    }

    void set_pp1(uint16_t pp1) {
        pp1_ = pp1;
    }

    void set_psa(uint8_t psa) {
        psa_ = psa;
    }

    void set_psd(uint16_t psd) {
        psd_ = psd;
    }

    void set_psi(uint32_t psi) {
        psi_ = psi;
    }

    void set_psm(uint8_t psm) {
        psm_ = psm;
    }

    void set_pst(uint16_t pst) {
        pst_ = pst;
    }

    void set_psv(uint16_t psv) {
        psv_ = psv;
    }

    void set_psw(uint16_t psw) {
        psw_ = psw;
    }

    void set_pth(uint32_t pth) {
        pth_ = pth;
    }

    void set_pto(uint32_t pto) {
        pto_ = pto;
    }

    void set_pv0(uint16_t pv0) {
        pv0_ = pv0;
    }

    void set_pv5(uint16_t pv5) {
        pv5_ = pv5;
    }

    void set_pvh(uint16_t pvh) {
        pvh_ = pvh;
    }

    void set_pvl(uint16_t pvl) {
        pvl_ = pvl;
    }

    void set_pxc(uint16_t pxc) {
        pxc_ = pxc;
    }

    void set_sam(uint8_t sam) {
        sam_ = sam;
    }

    void set_san(uint16_t san) {
        san_ = san;
    }

    void set_sao(uint16_t sao) {
        sao_ = sao;
    }

    void set_sap(uint16_t sap) {
        sap_ = sap;
    }

    void set_sax(uint16_t sax) {
        sax_ = sax;
    }

    void set_sbm(uint8_t sbm) {
        sbm_ = sbm;
    }

    void set_sbt(uint16_t sbt) {
        sbt_ = sbt;
    }

    void set_sdc(int16_t sdc) {
        sdc_ = sdc;
    }

    void set_sds(uint16_t sds) {
        sds_ = sds;
    }

    void set_sdt(uint32_t sdt) {
        sdt_ = sdt;
    }

    void set_sensor_type(uint8_t sensor_type) {
        sensor_type_ = sensor_type;
    }

    void set_set(uint16_t set) {
        set_ = set;
    }

    void set_sfn(uint16_t sfn) {
        sfn_ = sfn;
    }

    void set_sgt(uint16_t sgt) {
        sgt_ = sgt;
    }

    void set_fw_version(uint32_t fw_version) {
        fw_version_ = fw_version;
    }

    void set_spt(uint16_t spt) {
        spt_ = spt;
    }

    void set_ssa(uint16_t ssa) {
        ssa_ = ssa;
    }

    void set_ssi(uint32_t ssi) {
        ssi_ = ssi;
    }

    void set_ssp(uint16_t ssp) {
        ssp_ = ssp;
    }

    void set_surface_start_time(const ptime& surface_start_time) {
        surface_start_time_ = surface_start_time;
    }

    void set_xas(uint8_t xas) {
        xas_ = xas;
    }

    void set_icf(uint32_t icf) {
        icf_ = icf;
    }

    void set_icm(uint8_t icm) {
        icm_ = icm;
    }

    void set_icn(uint16_t icn) {
        icn_ = icn;
    }

    void set_ics(uint16_t ics) {
        ics_ = ics;
    }

    void set_ict(int16_t ict) {
        ict_ = ict;
    }

    void set_icw(uint32_t icw) {
        icw_ = icw;
    }

    void set_icx(uint16_t icx) {
        icx_ = icx;
    }

    void set_imm(uint16_t imm) {
        imm_ = imm;
    }

    void set_iso(uint16_t iso) {
        iso_ = iso;
    }

    void set_isp(uint8_t isp) {
        isp_ = isp;
    }

    void set_ist(uint16_t ist) {
        ist_ = ist;
    }

    void set_ica0(uint16_t ica) {
        ica_[0] = ica;
    }

    void set_icb0(uint16_t icb) {
        icb_[0] = icb;
    }

    void set_icd0(uint16_t icd) {
        icd_[0] = icd;
    }

    void set_icg0(uint8_t icg) {
        icg_[0] = icg;
    }

    void set_ico0(uint16_t ico) {
        ico_[0] = ico;
    }

    void set_icr0(uint32_t icr) {
        icr_[0] = icr;
    }

    void set_ilt0(uint16_t ilt) {
        ilt_[0] = ilt;
    }

    void set_imt0(uint16_t imt) {
        imt_[0] = imt;
    }

    void set_isa(uint8_t isa) {
        isa_ = isa;
    }

    void set_isv(uint16_t isv) {
        isv_ = isv;
    }

    void set_isw(uint16_t isw) {
        isw_ = isw;
    }

    void set_iss0(uint8_t iss) {
        iss_[0] = iss;
    }

    void set_iue0(uint8_t iue) {
        iue_[0] = iue;
    }

    void set_ica1(uint16_t ica) {
        ica_[1] = ica;
    }

    void set_icb1(uint16_t icb) {
        icb_[1] = icb;
    }

    void set_icd1(uint16_t icd) {
        icd_[1] = icd;
    }

    void set_icg1(uint8_t icg) {
        icg_[1] = icg;
    }

    void set_ico1(uint16_t ico) {
        ico_[1] = ico;
    }

    void set_icr1(uint32_t icr) {
        icr_[1] = icr;
    }

    void set_ilt1(uint16_t ilt) {
        ilt_[1] = ilt;
    }

    void set_imt1(uint16_t imt) {
        imt_[1] = imt;
    }

    void set_iss1(uint8_t iss) {
        iss_[1] = iss;
    }

    void set_iue1(uint8_t iue) {
        iue_[1] = iue;
    }

    void set_ism(uint8_t ism) {
        ism_ = ism;
    }
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XALAMOMISSIONCONFIGURATION_H_ */
