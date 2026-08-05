/** The XXX file.
 *
 *  @file
 *
 *  @version    XXX
 *
 *  @date       Oct 9, 2017 2:34:36 PM
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

#ifndef DATA_XMSG_XMFAMISSIONCFG_H_
#define DATA_XMSG_XMFAMISSIONCFG_H_

/* Includes: */

#include "../../utility/DateUtils.h"    // local libs
#include "../../utility/Log.h"
#include "common/XDefinitions.h"

namespace util {
namespace xmsg {

/* Types: */

/** The XXX class.
 *
 */
class XMfaMissionCfg:
    public util::date::DateUtils {

  private:  /* Private Data Members: */

    /** The mission configuration messages' raw data.
     *
     */
    static_vector<vector<uint8_t>, XMSG_MAX_MFA_MISS> msg_data_;

    /** The software version identifier.
     *
     */
    uint32_t fw_version_;

    /** The message version identifier.
     *
     */
    uint8_t msg_version_;

    /** The sensor type identifier.
     *
     */
    uint8_t sensor_type_;

    /* Data Members - General Configuration: */

    /** The mission, dive, or cycle number for this mission configuration.
     *
     */
    int16_t msn_;

    /** The target descent/drift/park pressure, in dBar.
     *
     */
    uint16_t dtd_[3];

    /** The maximum descent/fall/profile pressure, in dBar.
     *
     */
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
    uint32_t mtt_;

    uint32_t mts_;

    /* Data Members - Mission Phase */

    /** Current mission phase.
     *
     */
    uint8_t mps_;

    /** Cycle count for current mission phase.
     *e
     */
    uint16_t mpc_;

    /** Number of cycles for mission phase
     *
     */
    uint16_t mnm_[3];

    /** Next mission phase.
     *
     */
    uint8_t mns_[3];

    /* Data Members - Ice Configuration */

    /** Ice-detection mode.
     *
     */
    uint8_t icm_;

    /** Ice-detection -- month mask.
     *
     */
    uint16_t imm_;

    /** Ice-detection -- surfacing achieved criteria mode.
     *
     */
    uint8_t ism_;

    /** Ice-detection -- surface attempts.
     *
     */
    uint16_t ica_[2];

    /** Ice-detection -- cycles between surface attempts.
     *
     */
    uint16_t icb_[2];

    /** Ice-detection -- percentage used storage urgency criteria.
     *
     */
    uint16_t imt_[2];

    /** Ice-detection -- duration since last successful comms urgency criteria, in days.
     *
     */
    uint16_t ilt_[2];

    /** Ice-detection -- temperature threshold test upper pressure boundary, in dBar.
     *
     */
    uint16_t icn_;

    /** Ice-detection -- temperature threshold test lower pressure boundary, in dBar.
     *
     */
    uint16_t icx_;

    /** Ice-detection -- temperature threshold test temperature, in 1/100 degC.
     *
     */
    int16_t ict_;

    /** Ice-detection -- temperature threshold test sample interval, in seconds.
     *
     */
    uint16_t ics_;

    /** Ice-detection -- multiple surface attempts mode.
     *
     */
    uint8_t iss_[2];

    /** Ice-detection -- surface achieved salinity threshold, in 1/10 PSU.
     *
     */
    uint16_t ist_;

    /** Ice-detection -- duration between multiple surfacing attempts, in seconds.
     *
     */
    uint16_t iso_;

    /** Ice-detection -- multiple surfacing profile mode.
     *
     */
    uint8_t isp_;

    /** Ice-detection -- urgency mode exit criteria.
     *
     */
    uint8_t iue_[2];

    /** Ice-detection -- hold pressure, in dBar.
     *
     */
    uint16_t icd_[2];

    /** Ice-detection -- .
     *
     */
    uint32_t icr_[2];

    /** Ice-detection -- seek mode.
     *
     */
    uint8_t icg_[2];

    /** Ice-detection -- .
     *
     */
    uint8_t isa_;

    /** Ice-detection -- .
     *
     */
    uint16_t isd_;

    /** Ice-detection -- .
     *
     */
    uint16_t iat_;

    /** Ice-detection -- .
     *
     */
    uint16_t isv_;

    /** Ice-detection -- .
     *
     */
    uint16_t isw_;

    /** Ice-detection -- ice-detection hold-pressure duration, in minutes.
     *
     */
    uint16_t ico_[2];

    /** Ice-detection -- ice-free month mask.
     *
     */
    uint32_t icf_;

    /** Ice-detection -- iced-over month mask.
     *
     */
    uint32_t icw_;

    /* Data Members - IMU Configuration */

    /** The motion processing unit (MPU) data acquisition mode mask.
     *
     */
    uint8_t mam_;

    /** The motion processing unit (MPU) sampling duration, in seconds.
     *
     */
    uint16_t mat_[2];

    uint16_t map_;

    /** The motion processing unit (MPU) accelerometer sensitivity scale factor identifier.
     *
     */
    uint16_t aca_;

    /** The motion processing unit (MPU) gyroscope sensitivity scale factor identifier.
     *
     */
    uint16_t acg_;

    /** The motion processing unit (MPU) sample rate, in Hz.
     *
     */
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

    /** The auxiliary temperature channel mode.
     *
     *  @note     Applies to RBR CTDs only.
     */
    uint8_t ctc_;

    /** The profile cutoff pressure.
     *
     *  @todo   Determine gain for profile cutoff pressure field.
     *
     *  @note   This field was added in firmware version 0x02006900.
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

    /** The CTD profiling mode.
     *
     */
    uint8_t cms_;

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

    /** The CTD raw sampling threshold, in dBar.
     *
     */
    uint16_t crd_;

    /** The maximum number of raw samples to collect above the raw sampling threshold.
     *
     */
    uint16_t crs_;


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
    uint16_t dsd_[3];

    /** The descent state start timeout, in seconds.
     *
     */
    uint16_t dst_[3];

    uint16_t ddf_;

    /** Diagnostic dive bypass mode.
     *
     */
    uint8_t ddb_;

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
    uint16_t dft_[3];

    uint8_t dcm_;

    uint8_t dal_;

    uint16_t dct_[3];

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

    uint32_t ptd_;

    /** The park/drift state timeout, in seconds.
     *
     */
    uint32_t pto_[3];

    /** The bit-encoded park/drift state hour timeout trigger(s).
     *
     */
    uint32_t pth_[3];

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
    uint8_t pfm_[3];

    uint16_t pft_[3];

    /** The park/drift state fall depth (i.e. profile depth), in dBar.
     *
     */
    uint16_t pfd_[3];

    uint16_t pfv_[3];

    /** The park/drift state data storage interval/timeout, in seconds.
     *
     */
    uint16_t pdt_;

    uint8_t psm_;

    uint16_t psd_;

    uint8_t pss_;

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

    uint16_t aps_;

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

    /** WIMS timeout during surface state, in seconds.
     *
     */
    uint16_t wmt_;


    /* Data Members - RBR Post Processing */

    float ppc_tau_;

    float ppc_alpha_;

    float ppc_tdelay_;

    float ppc_ctccoeff_;

    uint8_t ppe_;



  public:   /* Public Member Functions: */

    XMfaMissionCfg();
    ~XMfaMissionCfg();
    void AssignMessage(const vector<uint8_t>& data);
    void DecodeXMsgData(uint32_t fw_version, uint8_t msg_idx, const vector<uint8_t>& data);
    const string GenerateListingString(int whoi, int dive, int idx) const;

  public:   /* Public Accessor Methods: */

    const ptime& ascent_start_time() const { return ascent_start_time_; }
    const ptime& descend_start_time() const { return descend_start_time_; }
    const ptime& park_start_time() const { return park_start_time_; }
    const ptime& surface_start_time() const { return surface_start_time_; }
    float ahr_mps() const { return (float) ahr() / 1000.; }
    float arc_sec() const { return (float) arc() / 1000.; }
    float arr_mps() const { return (float) arr() / 100.; }
    float asi_sec() const { return (float) asi() / 1000.; }
    float cpn1_dbar() const { return (float) cpn1() / 10.; }
    float cpn2_dbar() const { return (float) cpn2() / 10.; }
    float cpn3_dbar() const { return (float) cpn3() / 10.; }
    float cpp1_sec() const { return (float) cpp1() / 1000.; }
    float cpp2_sec() const { return (float) cpp2() / 1000.; }
    float cpp3_sec() const { return (float) cpp3() / 1000.; }
    float csp_sec() const { return (float) csp() / 1000.; }
    float dra_mps() const { return (float) dra() / 100.; }
    float dsi_sec() const { return (float) dsi() / 1000.; }
    float ict_degc() const { return (float) ict() / 100; }
    float ist_psu() const { return (float) ist() / 100; }
    float mtt_sec() const { return (float) mtt() / 1000; }
    float pnb_v() const { return (float) pnb() / 100; }
    float pnc_psi() const { return (float) pnc() / 100; }
    float psi_sec() const { return (float) psi() / 1000; }
    float pxc_psi() const { return (float) pxc() / 100; }
    float san_psi() const { return (float) san() / 1000; }
    float sax_psi() const { return (float) sax() / 1000; }
    float ssi_sec() const { return (float) ssi() / 1000; }
    int ict() const { return ict_; }
    int msn() const { return msn_; }
    int sdc() const { return sdc_; }
    unsigned int aca() const { return aca_; }
    unsigned int acg() const { return acg_; }
    unsigned int acr() const { return acr_; }
    unsigned int add() const { return add_; }
    unsigned int adt() const { return adt_; }
    unsigned int ahd() const { return ahd_; }
    unsigned int ahr() const { return ahr_; }
    unsigned int apf() const { return apf_; }
    unsigned int apm() const { return apm_; }
    unsigned int apn() const { return apn_; }
    unsigned int apt() const { return apt_; }
    unsigned int arc() const { return arc_; }
    unsigned int arr() const { return arr_; }
    unsigned int asd() const { return asd_; }
    unsigned int asi() const { return asi_; }
    unsigned int asn() const { return asn_; }
    unsigned int btm() const { return btm_; }
    unsigned int cpb1() const { return cpb_[0]; }
    unsigned int cpb2() const { return cpb_[1]; }
    unsigned int cpb3() const { return cpb_[2]; }
    unsigned int cpn1() const { return cpn_[0]; }
    unsigned int cpn2() const { return cpn_[1]; }
    unsigned int cpn3() const { return cpn_[2]; }
    unsigned int cpp1() const { return cpp_[0]; }
    unsigned int cpp2() const { return cpp_[1]; }
    unsigned int cpp3() const { return cpp_[2]; }
    unsigned int cps() const { return cps_; }
    unsigned int crd() const { return crd_; }
    unsigned int crs() const { return crs_; }
    unsigned int csp() const { return csp_; }
    unsigned int cst() const { return cst_; }
    unsigned int ctc() const { return ctc_; }
    unsigned int ctf() const { return ctf_; }
    unsigned int ctl() const { return ctl_; }
    unsigned int ctu() const { return ctu_; }
    unsigned int dal() const { return dal_; }
    unsigned int dca() const { return dca_; }
    unsigned int dcd() const { return dcd_; }
    unsigned int dcf() const { return dcf_; }
    unsigned int dcm() const { return dcm_; }
    unsigned int dcp() const { return dcp_; }
    unsigned int dcr() const { return dcr_; }
    unsigned int dct() const { return dct_[0]; }
    unsigned int dct0() const { return dct_[0]; }
    unsigned int dct1() const { return dct_[1]; }
    unsigned int dct2() const { return dct_[2]; }
    unsigned int dcx() const { return dcx_; }
    unsigned int ddc() const { return ddc_; }
    unsigned int ddd() const { return ddd_; }
    unsigned int ddf() const { return ddf_; }
    unsigned int ddp() const { return ddp_; }
    unsigned int dds() const { return dds_; }
    unsigned int ddt() const { return ddt_; }
    unsigned int dft() const { return dft_[0]; }
    unsigned int dft0() const { return dft_[0]; }
    unsigned int dft1() const { return dft_[1]; }
    unsigned int dft2() const { return dft_[2]; }
    unsigned int dns() const { return dns_; }
    unsigned int dpd() const { return dpd_; }
    unsigned int dpt() const { return dpt_; }
    unsigned int dra() const { return dra_; }
    unsigned int drc() const { return drc_; }
    unsigned int drt() const { return drt_; }
    unsigned int dsd() const { return dsd_[0]; }
    unsigned int dsd0() const { return dsd_[0]; }
    unsigned int dsd1() const { return dsd_[1]; }
    unsigned int dsd2() const { return dsd_[2]; }
    unsigned int dsi() const { return dsi_; }
    unsigned int dst() const { return dst_[0]; }
    unsigned int dst0() const { return dst_[0]; }
    unsigned int dst1() const { return dst_[1]; }
    unsigned int dst2() const { return dst_[2]; }
    unsigned int dsw() const { return dsw_; }
    unsigned int dtd() const { return dtd_[0]; }
    unsigned int dtd0() const { return dtd_[0]; }
    unsigned int dtd1() const { return dtd_[1]; }
    unsigned int dtd2() const { return dtd_[2]; }
    unsigned int duc() const { return duc_; }
    unsigned int dut() const { return dut_; }
    unsigned int dvd() const { return dvd_; }
    unsigned int dvt() const { return dvt_; }
    unsigned int dvw() const { return dvw_; }
    unsigned int fos() const { return fos_; }
    unsigned int fta() const { return fta_; }
    unsigned int ftm() const { return ftm_; }
    unsigned int fto() const { return fto_; }
    unsigned int fw_version() const { return fw_version_; }
    unsigned int iat() const { return iat_; }
    unsigned int ica0() const { return ica_[0]; }
    unsigned int ica1() const { return ica_[1]; }
    unsigned int icb0() const { return icb_[0]; }
    unsigned int icb1() const { return icb_[1]; }
    unsigned int icd0() const { return icd_[0]; }
    unsigned int icd1() const { return icd_[1]; }
    unsigned int icf() const { return icf_; }
    unsigned int icg0() const { return icg_[0]; }
    unsigned int icg1() const { return icg_[1]; }
    unsigned int icm() const { return icm_; }
    unsigned int icn() const { return icn_; }
    unsigned int ico0() const { return ico_[0]; }
    unsigned int ico1() const { return ico_[1]; }
    unsigned int icr0() const { return icr_[0]; }
    unsigned int icr1() const { return icr_[1]; }
    unsigned int ics() const { return ics_; }
    unsigned int icw() const { return icw_; }
    unsigned int icx() const { return icx_; }
    unsigned int ilt0() const { return ilt_[0]; }
    unsigned int ilt1() const { return ilt_[1]; }
    unsigned int imm() const { return imm_; }
    unsigned int imt0() const { return imt_[0]; }
    unsigned int imt1() const { return imt_[1]; }
    unsigned int isa() const { return isa_; }
    unsigned int isd() const { return isd_; }
    unsigned int ism() const { return ism_; }
    unsigned int iso() const { return iso_; }
    unsigned int isp() const { return isp_; }
    unsigned int iss0() const { return iss_[0]; }
    unsigned int iss1() const { return iss_[1]; }
    unsigned int ist() const { return ist_; }
    unsigned int isv() const { return isv_; }
    unsigned int isw() const { return isw_; }
    unsigned int iue0() const { return iue_[0]; }
    unsigned int iue1() const { return iue_[1]; }
    unsigned int mam() const { return mam_; }
    unsigned int map() const { return map_; }
    unsigned int mat() const { return mat_[1]; }
    unsigned int mat1() const { return mat_[0]; }
    unsigned int mat2() const { return mat_[1]; }
    unsigned int mds() const { return mds_; }
    unsigned int mnm0() const { return mnm_[0]; }
    unsigned int mnm1() const { return mnm_[1]; }
    unsigned int mnm2() const { return mnm_[2]; }
    unsigned int mns0() const { return mns_[0]; }
    unsigned int mns1() const { return mns_[1]; }
    unsigned int mns2() const { return mns_[2]; }
    unsigned int mpc() const { return mpc_; }
    unsigned int mps() const { return mps_; }
    unsigned int msg_version() const { return msg_version_; }
    unsigned int mtd() const { return mtd_; }
    unsigned int mts() const { return mts_; }
    unsigned int mtt() const { return mtt_; }
    unsigned int pbh() const { return pbh_; }
    unsigned int pbl() const { return pbl_; }
    unsigned int pc0() const { return pc0_; }
    unsigned int pc1() const { return pc1_; }
    unsigned int pdt() const { return pdt_; }
    unsigned int pfc() const { return pfc_; }
    unsigned int pfd() const { return pfd_[0]; }
    unsigned int pfd0() const { return pfd_[0]; }
    unsigned int pfd1() const { return pfd_[1]; }
    unsigned int pfd2() const { return pfd_[2]; }
    unsigned int pfm() const { return pfm_[0]; }
    unsigned int pfm0() const { return pfm_[0]; }
    unsigned int pfm1() const { return pfm_[1]; }
    unsigned int pfm2() const { return pfm_[2]; }
    unsigned int pfr() const { return pfr_; }
    unsigned int pft() const { return pft_[0]; }
    unsigned int pft0() const { return pft_[0]; }
    unsigned int pft1() const { return pft_[1]; }
    unsigned int pft2() const { return pft_[2]; }
    unsigned int pfv() const { return pfv_[0]; }
    unsigned int pfv0() const { return pfv_[0]; }
    unsigned int pfv1() const { return pfv_[1]; }
    unsigned int pfv2() const { return pfv_[2]; }
    unsigned int pnb() const { return pnb_; }
    unsigned int pnc() const { return pnc_; }
    unsigned int pns() const { return pns_; }
    unsigned int pp0() const { return pp0_; }
    unsigned int pp1() const { return pp1_; }
    unsigned int psa() const { return psa_; }
    unsigned int psd() const { return psd_; }
    unsigned int psi() const { return psi_; }
    unsigned int psm() const { return psm_; }
    unsigned int pss() const { return pss_; }
    unsigned int pst() const { return pst_; }
    unsigned int psv() const { return psv_; }
    unsigned int psw() const { return psw_; }
    unsigned int ptd() const { return ptd_; }
    unsigned int pth() const { return pth_[0]; }
    unsigned int pth0() const { return pth_[0]; }
    unsigned int pth1() const { return pth_[1]; }
    unsigned int pth2() const { return pth_[2]; }
    unsigned int pto() const { return pto_[0]; }
    unsigned int pto0() const { return pto_[0]; }
    unsigned int pto1() const { return pto_[1]; }
    unsigned int pto2() const { return pto_[2]; }
    unsigned int pv0() const { return pv0_; }
    unsigned int pv5() const { return pv5_; }
    unsigned int pvh() const { return pvh_; }
    unsigned int pvl() const { return pvl_; }
    unsigned int pxc() const { return pxc_; }
    unsigned int sam() const { return sam_; }
    unsigned int san() const { return san_; }
    unsigned int sao() const { return sao_; }
    unsigned int sap() const { return sap_; }
    unsigned int sax() const { return sax_; }
    unsigned int sbm() const { return sbm_; }
    unsigned int sbt() const { return sbt_; }
    unsigned int sds() const { return sds_; }
    unsigned int sdt() const { return sdt_; }
    unsigned int sensor_type() const { return sensor_type_; }
    unsigned int set() const { return set_; }
    unsigned int sfn() const { return sfn_; }
    unsigned int sgt() const { return sgt_; }
    unsigned int spt() const { return spt_; }
    unsigned int ssa() const { return ssa_; }
    unsigned int ssi() const { return ssi_; }
    unsigned int ssp() const { return ssp_; }
    unsigned int xas() const { return xas_; }
    unsigned int ddb() const { return ddb_; }
    unsigned int aps() const { return aps_; }
    unsigned int wmt() const { return wmt_; }

    float ppc_alpha() const {
      return ppc_alpha_;
    }

    float ppc_ctccoeff() const {
      return ppc_ctccoeff_;
    }

    float ppc_tau() const {
      return ppc_tau_;
    }

    float ppc_tdelay() const {
      return ppc_tdelay_;
    }

    uint8_t ppe() const {
      return ppe_;
    }


  private:    /* Private Mutator Methods: */

    void set_aca(uint16_t aca) { aca_ = aca; }
    void set_acg(uint16_t acg) { acg_ = acg; }
    void set_acr(uint16_t acr) { acr_ = acr; }
    void set_add(uint16_t add) { add_ = add; }
    void set_adt(uint16_t adt) { adt_ = adt; }
    void set_ahd(uint16_t ahd) { ahd_ = ahd; }
    void set_ahr(uint16_t ahr) { ahr_ = ahr; }
    void set_apf(uint16_t apf) { apf_ = apf; }
    void set_apm(uint16_t apm) { apm_ = apm; }
    void set_apn(uint16_t apn) { apn_ = apn; }
    void set_apt(uint8_t apt) { apt_ = apt; }
    void set_arc(uint16_t arc) { arc_ = arc; }
    void set_arr(uint16_t arr) { arr_ = arr; }
    void set_ascent_start_time(const ptime& ascent_start_time) { ascent_start_time_ = ascent_start_time; }
    void set_asd(uint16_t asd) { asd_ = asd; }
    void set_asi(uint32_t asi) { asi_ = asi; }
    void set_asn(uint16_t asn) { asn_ = asn; }
    void set_btm(uint8_t btm) { btm_ = btm; }
    void set_cpb1(uint16_t cpb1) { cpb_[0] = cpb1; }
    void set_cpb2(uint16_t cpb2) { cpb_[1] = cpb2; }
    void set_cpb3(uint16_t cpb3) { cpb_[2] = cpb3; }
    void set_cpn1(uint16_t cpn1) { cpn_[0] = cpn1; }
    void set_cpn2(uint16_t cpn2) { cpn_[1] = cpn2; }
    void set_cpn3(uint16_t cpn3) { cpn_[2] = cpn3; }
    void set_cpp1(uint16_t cpp1) { cpp_[0] = cpp1; }
    void set_cpp2(uint16_t cpp2) { cpp_[1] = cpp2; }
    void set_cpp3(uint16_t cpp3) { cpp_[2] = cpp3; }
    void set_cps(uint8_t cps) { cps_ = cps; }
    void set_crd(uint16_t crd) { crd_ = crd; }
    void set_crs(uint16_t crs) { crs_ = crs; }
    void set_csp(uint32_t csp) { csp_ = csp; }
    void set_cst(uint8_t cst) { cst_ = cst; }
    void set_ctc(uint8_t ctc) { ctc_ = ctc; }
    void set_ctf(uint16_t ctf) { ctf_ = ctf; }
    void set_ctl(uint16_t ctl) { ctl_ = ctl; }
    void set_ctu(uint16_t ctu) { ctu_ = ctu; }
    void set_dal(uint8_t dal) { dal_ = dal; }
    void set_dca(uint8_t dca) { dca_ = dca; }
    void set_dcd(uint16_t dcd) { dcd_ = dcd; }
    void set_dcf(uint16_t dcf) { dcf_ = dcf; }
    void set_dcm(uint8_t dcm) { dcm_ = dcm; }
    void set_dcp(uint16_t dcp) { dcp_ = dcp; }
    void set_dcr(uint16_t dcr) { dcr_ = dcr; }
    void set_dct(uint16_t dct) { dct_[0] = dct; }
    void set_dct0(uint16_t dct) { dct_[0] = dct; }
    void set_dct1(uint16_t dct) { dct_[1] = dct; }
    void set_dct2(uint16_t dct) { dct_[2] = dct; }
    void set_dcx(uint16_t dcx) { dcx_ = dcx; }
    void set_ddc(uint16_t ddc) { ddc_ = ddc; }
    void set_ddd(uint16_t ddd) { ddd_ = ddd; }
    void set_ddf(uint16_t ddf) { ddf_ = ddf; }
    void set_ddp(uint16_t ddp) { ddp_ = ddp; }
    void set_dds(uint16_t dds) { dds_ = dds; }
    void set_ddt(uint16_t ddt) { ddt_ = ddt; }
    void set_descend_start_time(const ptime& descend_start_time) { descend_start_time_ = descend_start_time; }
    void set_dft(uint16_t dft) { dft_[0] = dft; }
    void set_dft0(uint16_t dft) { dft_[0] = dft; }
    void set_dft1(uint16_t dft) { dft_[1] = dft; }
    void set_dft2(uint16_t dft) { dft_[2] = dft; }
    void set_dns(uint16_t dns) { dns_ = dns; }
    void set_dpd(uint16_t dpd) { dpd_ = dpd; }
    void set_dpt(uint16_t dpt) { dpt_ = dpt; }
    void set_dra(uint8_t dra) { dra_ = dra; }
    void set_drc(uint16_t drc) { drc_ = drc; }
    void set_drt(uint16_t drt) { drt_ = drt; }
    void set_dsd(uint16_t dsd) { dsd_[0] = dsd; }
    void set_dsd0(uint16_t dsd) { dsd_[0] = dsd; }
    void set_dsd1(uint16_t dsd) { dsd_[1] = dsd; }
    void set_dsd2(uint16_t dsd) { dsd_[2] = dsd; }
    void set_dsi(uint32_t dsi) { dsi_ = dsi; }
    void set_dst(uint16_t dst) { dst_[0] = dst; }
    void set_dst0(uint16_t dst) { dst_[0] = dst; }
    void set_dst1(uint16_t dst) { dst_[1] = dst; }
    void set_dst2(uint16_t dst) { dst_[2] = dst; }
    void set_dsw(uint16_t dsw) { dsw_ = dsw; }
    void set_dtd(uint16_t dtd) { dtd_[0] = dtd; }
    void set_dtd0(uint16_t dtd) { dtd_[0] = dtd; }
    void set_dtd1(uint16_t dtd) { dtd_[1] = dtd; }
    void set_dtd2(uint16_t dtd) { dtd_[2] = dtd; }
    void set_duc(uint16_t duc) { duc_ = duc; }
    void set_dut(uint16_t dut) { dut_ = dut; }
    void set_dvd(uint16_t dvd) { dvd_ = dvd; }
    void set_dvt(uint16_t dvt) { dvt_ = dvt; }
    void set_dvw(uint16_t dvw) { dvw_ = dvw; }
    void set_fos(uint16_t fos) { fos_ = fos; }
    void set_fta(uint8_t fta) { fta_ = fta; }
    void set_ftm(uint8_t ftm) { ftm_ = ftm; }
    void set_fto(uint16_t fto) { fto_ = fto; }
    void set_fw_version(uint32_t fw_version) { fw_version_ = fw_version; }
    void set_iat(uint16_t iat) { iat_ = iat; }
    void set_ica0(uint16_t ica0) { ica_[0] = ica0; }
    void set_ica1(uint16_t ica1) { ica_[1] = ica1; }
    void set_icb0(uint16_t icb0) { icb_[0] = icb0; }
    void set_icb1(uint16_t icb1) { icb_[1] = icb1; }
    void set_icd0(uint16_t icd0) { icd_[0] = icd0; }
    void set_icd1(uint16_t icd1) { icd_[1] = icd1; }
    void set_icf(uint32_t icf) { icf_ = icf; }
    void set_icg0(uint16_t icg0) { icg_[0] = icg0; }
    void set_icg1(uint16_t icg1) { icg_[1] = icg1; }
    void set_icm(uint8_t icm) { icm_ = icm; }
    void set_icn(uint16_t icn) { icn_ = icn; }
    void set_ico0(uint16_t ico0) { ico_[0] = ico0; }
    void set_ico1(uint16_t ico1) { ico_[1] = ico1; }
    void set_icr0(uint16_t icr0) { icr_[0] = icr0; }
    void set_icr1(uint16_t icr1) { icr_[1] = icr1; }
    void set_ics(uint16_t ics) { ics_ = ics; }
    void set_ict(int16_t ict) { ict_ = ict; }
    void set_icw(uint32_t icw) { icw_ = icw; }
    void set_icx(uint16_t icx) { icx_ = icx; }
    void set_ilt0(uint16_t ilt0) { ilt_[0] = ilt0; }
    void set_ilt1(uint16_t ilt1) { ilt_[1] = ilt1; }
    void set_imm(uint16_t imm) { imm_ = imm; }
    void set_imt0(uint16_t imt0) { imt_[0] = imt0; }
    void set_imt1(uint16_t imt1) { imt_[1] = imt1; }
    void set_isa(uint8_t isa) { isa_ = isa; }
    void set_isd(uint16_t isd) { isd_ = isd; }
    void set_ism(uint8_t ism) { ism_ = ism; }
    void set_iso(uint16_t iso) { iso_ = iso; }
    void set_isp(uint8_t isp) { isp_ = isp; }
    void set_iss0(uint16_t iss0) { iss_[0] = iss0; }
    void set_iss1(uint16_t iss1) { iss_[1] = iss1; }
    void set_ist(uint16_t ist) { ist_ = ist; }
    void set_isv(uint16_t isv) { isv_ = isv; }
    void set_isw(uint16_t isw) { isw_ = isw; }
    void set_iue0(uint16_t iue0) { iue_[0] = iue0; }
    void set_iue1(uint16_t iue1) { iue_[1] = iue1; }
    void set_mam(uint8_t mam) { mam_ = mam; }
    void set_map(uint16_t map) { map_ = map; }
    void set_mat(uint16_t mat) { mat_[0] = mat; }
    void set_mat1(uint16_t mat) { mat_[0] = mat; }
    void set_mat2(uint16_t mat) { mat_[1] = mat; }
    void set_mds(uint16_t mds) { mds_ = mds; }
    void set_mnm0(uint16_t mnm) { mnm_[0] = mnm; }
    void set_mnm1(uint16_t mnm) { mnm_[1] = mnm; }
    void set_mnm2(uint16_t mnm) { mnm_[2] = mnm; }
    void set_mns0(uint8_t mns) { mns_[0] = mns; }
    void set_mns1(uint8_t mns) { mns_[1] = mns; }
    void set_mns2(uint8_t mns) { mns_[2] = mns; }
    void set_mpc(uint16_t mpc) { mpc_ = mpc; }
    void set_mps(uint8_t mps) { mps_ = mps; }
    void set_msg_version(uint8_t msg_version) { msg_version_ = msg_version; }
    void set_msn(int16_t msn) { msn_ = msn; }
    void set_mtd(uint16_t mtd) { mtd_ = mtd; }
    void set_mts(uint32_t mts) { mts_ = mts; }
    void set_mtt(uint32_t mtt) { mtt_ = mtt; }
    void set_park_start_time(const ptime& park_start_time) { park_start_time_ = park_start_time; }
    void set_pbh(uint16_t pbh) { pbh_ = pbh; }
    void set_pbl(uint16_t pbl) { pbl_ = pbl; }
    void set_pc0(uint16_t pc0) { pc0_ = pc0; }
    void set_pc1(uint16_t pc1) { pc1_ = pc1; }
    void set_pdt(uint16_t pdt) { pdt_ = pdt; }
    void set_pfc(uint16_t pfc) { pfc_ = pfc; }
    void set_pfd(uint16_t pfd) { pfd_[0] = pfd; }
    void set_pfd0(uint16_t pfd) { pfd_[0] = pfd; }
    void set_pfd1(uint16_t pfd) { pfd_[1] = pfd; }
    void set_pfd2(uint16_t pfd) { pfd_[2] = pfd; }
    void set_pfm(uint8_t pfm) { pfm_[0] = pfm; }
    void set_pfm0(uint8_t pfm) { pfm_[0] = pfm; }
    void set_pfm1(uint8_t pfm) { pfm_[1] = pfm; }
    void set_pfm2(uint8_t pfm) { pfm_[2] = pfm; }
    void set_pfr(uint16_t pfr) { pfr_ = pfr; }
    void set_pft(uint16_t pft) { pft_[0] = pft; }
    void set_pft0(uint16_t pft) { pft_[0] = pft; }
    void set_pft1(uint16_t pft) { pft_[1] = pft; }
    void set_pft2(uint16_t pft) { pft_[2] = pft; }
    void set_pfv(uint16_t pfv) { pfv_[0] = pfv; }
    void set_pfv0(uint16_t pfv) { pfv_[0] = pfv; }
    void set_pfv1(uint16_t pfv) { pfv_[1] = pfv; }
    void set_pfv2(uint16_t pfv) { pfv_[2] = pfv; }
    void set_pnb(uint16_t pnb) { pnb_ = pnb; }
    void set_pnc(uint16_t pnc) { pnc_ = pnc; }
    void set_pns(uint16_t pns) { pns_ = pns; }
    void set_pp0(uint16_t pp0) { pp0_ = pp0; }
    void set_pp1(uint16_t pp1) { pp1_ = pp1; }
    void set_psa(uint8_t psa) { psa_ = psa; }
    void set_psd(uint16_t psd) { psd_ = psd; }
    void set_psi(uint32_t psi) { psi_ = psi; }
    void set_psm(uint8_t psm) { psm_ = psm; }
    void set_pss(uint8_t pss) { pss_ = pss; }
    void set_pst(uint16_t pst) { pst_ = pst; }
    void set_psv(uint16_t psv) { psv_ = psv; }
    void set_psw(uint16_t psw) { psw_ = psw; }
    void set_ptd(uint32_t ptd) { ptd_= ptd; }
    void set_pth(uint32_t pth) { pth_[0] = pth; }
    void set_pth0(uint32_t pth) { pth_[0] = pth; }
    void set_pth1(uint32_t pth) { pth_[1] = pth; }
    void set_pth2(uint32_t pth) { pth_[2] = pth; }
    void set_pto(uint32_t pto) { pto_[0] = pto; }
    void set_pto0(uint32_t pto) { pto_[0] = pto; }
    void set_pto1(uint32_t pto) { pto_[1] = pto; }
    void set_pto2(uint32_t pto) { pto_[2] = pto; }
    void set_pv0(uint16_t pv0) { pv0_ = pv0; }
    void set_pv5(uint16_t pv5) { pv5_ = pv5; }
    void set_pvh(uint16_t pvh) { pvh_ = pvh; }
    void set_pvl(uint16_t pvl) { pvl_ = pvl; }
    void set_pxc(uint16_t pxc) { pxc_ = pxc; }
    void set_sam(uint8_t sam) { sam_ = sam; }
    void set_san(uint16_t san) { san_ = san; }
    void set_sao(uint16_t sao) { sao_ = sao; }
    void set_sap(uint16_t sap) { sap_ = sap; }
    void set_sax(uint16_t sax) { sax_ = sax; }
    void set_sbm(uint8_t sbm) { sbm_ = sbm; }
    void set_sbt(uint16_t sbt) { sbt_ = sbt; }
    void set_sdc(int16_t sdc) { sdc_ = sdc; }
    void set_sds(uint16_t sds) { sds_ = sds; }
    void set_sdt(uint32_t sdt) { sdt_ = sdt; }
    void set_sensor_type(uint8_t sensor_type) { sensor_type_ = sensor_type; }
    void set_set(uint16_t set) { set_ = set; }
    void set_sfn(uint16_t sfn) { sfn_ = sfn; }
    void set_sgt(uint16_t sgt) { sgt_ = sgt; }
    void set_spt(uint16_t spt) { spt_ = spt; }
    void set_ssa(uint16_t ssa) { ssa_ = ssa; }
    void set_ssi(uint32_t ssi) { ssi_ = ssi; }
    void set_ssp(uint16_t ssp) { ssp_ = ssp; }
    void set_surface_start_time(const ptime& surface_start_time) { surface_start_time_ = surface_start_time; }
    void set_ddb(uint8_t ddb) { ddb_ = ddb; }
    void set_xas(uint8_t xas) { xas_ = xas; }
    void set_aps(uint16_t aps) { aps_ = aps; }
    void set_wmt(uint16_t wmt) { wmt_ = wmt; }

    void set_ppc_alpha(float ppc_alpha) {
      ppc_alpha_ = ppc_alpha;
    }

    void set_ppc_ctccoeff(float ppc_ctccoeff) {
      ppc_ctccoeff_ = ppc_ctccoeff;
    }

    void set_ppc_tau(float ppc_tau) {
      ppc_tau_ = ppc_tau;
    }

    void set_ppc_tdelay(float ppc_tdelay) {
      ppc_tdelay_ = ppc_tdelay;
    }

    void set_ppe(uint8_t ppe) {
      ppe_ = ppe;
    }


};

} /* namespace xmsg */
} /* namespace util */

#endif /* DATA_XMSG_XMFAMISSIONCFG_H_ */
