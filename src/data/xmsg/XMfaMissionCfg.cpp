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

/* Includes: */

#include "XMfaMissionCfg.h"   // header

namespace util {
namespace xmsg {

/* Function Definitions: */

/** The default constructor.
 *
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
XMfaMissionCfg::XMfaMissionCfg() :
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
    aps_(XMSG_BAD_U16),
    arc_(XMSG_BAD_U16),
    arr_(XMSG_BAD_U16),
    asd_(XMSG_BAD_U16),
    asi_(XMSG_BAD_U32),
    asn_(XMSG_BAD_U16),
    btm_(XMSG_BAD_U08),
    cms_(XMSG_BAD_U08),
    cpb_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
    cpn_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
    cpp_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
    cps_(XMSG_BAD_U08),
    crd_(XMSG_BAD_U16),
    crs_(XMSG_BAD_U16),
    csp_(XMSG_BAD_U32),
    cst_(XMSG_BAD_U08),
    ctc_(XMSG_BAD_U08),
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
    dct_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
    dcx_(XMSG_BAD_U16),
    ddb_(XMSG_BAD_U08),
    ddc_(XMSG_BAD_U16),
    ddd_(XMSG_BAD_U16),
    ddf_(XMSG_BAD_U16),
    ddp_(XMSG_BAD_U16),
    dds_(XMSG_BAD_U16),
    ddt_(XMSG_BAD_U16),
    dft_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
    dns_(XMSG_BAD_U16),
    dpd_(XMSG_BAD_U16),
    dpt_(XMSG_BAD_U16),
    dra_(XMSG_BAD_U08),
    drc_(XMSG_BAD_U16),
    drt_(XMSG_BAD_U16),
    dsd_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
    dsi_(XMSG_BAD_U32),
    dst_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
    dsw_(XMSG_BAD_U16),
    dtd_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
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
    map_(XMSG_BAD_U16),
    mat_{ XMSG_BAD_U16,XMSG_BAD_U16 },
    mds_(XMSG_BAD_U16),
    mpc_(XMSG_BAD_U16),
    mps_(XMSG_BAD_U08),
    msg_data_(XMSG_MAX_MFA_MISS, vector<uint8_t>()),
    msg_version_(XMSG_BAD_U08),
    msn_(XMSG_BAD_I16),
    mtd_(XMSG_BAD_U16),
    mts_(XMSG_BAD_U32),
    mtt_(XMSG_BAD_U16),
    pbh_(XMSG_BAD_U16),
    pbl_(XMSG_BAD_U16),
    pc0_(XMSG_BAD_U16),
    pc1_(XMSG_BAD_U16),
    pdt_(XMSG_BAD_U16),
    pfc_(XMSG_BAD_U16),
    pfd_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
    pfm_{ XMSG_BAD_U08, XMSG_BAD_U08, XMSG_BAD_U08, },
    pfr_(XMSG_BAD_U16),
    pft_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
    pfv_{ XMSG_BAD_U16, XMSG_BAD_U16, XMSG_BAD_U16, },
    pnb_(XMSG_BAD_U16),
    pnc_(XMSG_BAD_U16),
    pns_(XMSG_BAD_U16),
    pp0_(XMSG_BAD_U16),
    pp1_(XMSG_BAD_U16),
    psa_(XMSG_BAD_U08),
    psd_(XMSG_BAD_U16),
    psi_(XMSG_BAD_U32),
    psm_(XMSG_BAD_U08),
    pss_(XMSG_BAD_U08),
    pst_(XMSG_BAD_U16),
    psv_(XMSG_BAD_U16),
    psw_(XMSG_BAD_U16),
    ptd_(XMSG_BAD_U32),
    pth_{ XMSG_BAD_U32, XMSG_BAD_U32, XMSG_BAD_U32, },
    pto_{ XMSG_BAD_U32, XMSG_BAD_U32, XMSG_BAD_U32, },
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
    xas_(XMSG_BAD_U08),
    wmt_(XMSG_BAD_U16),
    ppc_tau_(XMSG_BAD_FLT),
    ppc_alpha_(XMSG_BAD_U16),
    ppc_tdelay_(XMSG_BAD_U16),
    ppc_ctccoeff_(XMSG_BAD_U16),
    ppe_(XMSG_BAD_U08) {
  for(int i = 0; i < XMSG_MAX_MFA_MISS; ++i)
    msg_data_[i].reserve(340);
}
#pragma GCC diagnostic pop

/** The destructor.
 *
 */
XMfaMissionCfg::~XMfaMissionCfg() {
  /* Do nothing. */
}

/** Assign a mission configuration message.
 *
 *  @param[in]  data    The message.
 */
void XMfaMissionCfg::AssignMessage(const vector<uint8_t>& data) {

    uint8_t msg_idx;

    try {

      if(data.empty())  EXC_THROW("Zero-length message.");    // Check length

      /* Copy the message data: */

      if((msg_idx = data[0] & 0x0F) >= XMSG_MAX_MFA_MISS)     // Check index.
        EXC_THROW("Message index exceeds XMSG_MAX_MFA_MISS ("
                  + lexical_cast<string>(XMSG_MAX_MFA_MISS) + ").");
      if(!msg_data_[msg_idx].empty())
        msg_data_[msg_idx].clear();                           // Clear existing message.
      msg_data_[msg_idx].insert(                              // Insert data.
          msg_data_[msg_idx].end(),
          data.begin(),
          data.end());

      /* Decode the message: */

      switch(msg_idx) {

        case 0:

          set_fw_version(u32(data, 4));

          DecodeXMsgData(fw_version(), 0, msg_data_[0]);
          if(!msg_data_[1].empty())
            DecodeXMsgData(fw_version(), 1, msg_data_[1]);

          break;

        case 1:

          if(msg_data_[0].empty())
            break;

          DecodeXMsgData(fw_version(), 1, msg_data_[1]);

          break;

        default:

          break;
      }

    } CATCH_BLOCK("Failed to decode MFA mission configuration message!")

  }

/** Decode the mission configuration message data.
 *
 *  @param[in] fw_ver   The firmware version.
 *  @param[in] msg_idx  The message index.
 *  @param[in] data     The message data.
 */
void XMfaMissionCfg::DecodeXMsgData(uint32_t fw_ver, uint8_t msg_idx, const vector<uint8_t>& data) {

  try {

    const uint8_t* d = data.data();

    ldbg() << "Decoding MFA mission configuration message...";

    switch(fw_ver) {
      case 0x02006200: // ALAMO Software version 0x02006200.
      case 0x02006300: // ALAMO Software version 0x02006300.
      case 0x02006700: // ALAMO Software version 0x02006700.
      case 0x02006800: // ALAMO Software version 0x02006800.

        if(data.size() != 323) {
          ostringstream os;
          os << "Invalid X-Message: Expected 323 bytes, but received " << data.size() << " bytes!";
          EXC_THROW(os.str());
        }

        if(msg_idx == 0) {

          set_msn(u16(d, 8));
          set_dtd(u16(d, 10));
          set_mds(u16(d, 12));
          set_apt(u08(d, 14));
          set_xas(u08(d, 15));
          set_cps(u08(d, 16));
          set_cst(u08(d, 17));
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
          set_mam(u08(d, 46));
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
          set_dcm(u08(d, 125));
          set_dal(u08(d, 126));
          set_dct(u16(d, 127));
          set_dca(u08(d, 129));
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
          set_dvt(u16(d, 154));
          set_dpd(u16(d, 152));
          set_pfr(u16(d, 156));
          set_pfc(u16(d, 158));
          set_ddp(u16(d, 160));
          set_dds(u16(d, 162));
          set_park_start_time(s2000(d, 179));
          set_pns(u16(d, 183));
          set_pto(u32(d, 185));
          set_pth(u32(d, 189));
          set_psa(u08(d, 193));
          set_psi(u32(d, 194));
          set_pfm(u08(d, 198));
          set_pft(u16(d, 199));
          set_pfd(u16(d, 201));
          set_pfv(u16(d, 203));
          set_pdt(u16(d, 205));
          set_psm(u08(d, 207));
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
          set_sbm(u08(d, 283));
          set_sbt(u16(d, 284));
          set_sdt(u32(d, 286));
          set_ssa(u16(d, 290));
          set_ssi(u32(d, 292));
          set_sds(u16(d, 296));
          set_sam(u08(d, 298));
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

        if(data.size() != 322) {
          ostringstream os;
          os << "Invalid X-Message: Expected 323 bytes, but received " << data.size() << " bytes!";
          EXC_THROW(os.str());
        }

        if(msg_idx == 0) {

          set_msn(u16(d, 8));
          set_dtd(u16(d, 10));
          set_mds(u16(d, 12));
          set_apt(u08(d, 14));
          set_xas(u08(d, 15));
          set_cps(u08(d, 16));
          set_cst(u08(d, 17));
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
          set_mam(u08(d, 46 + 6));
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

          set_icm(u08(d, 87));
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
          set_iss0(u08(d, 114));
          set_iss1(u08(d, 115));
          set_ist(u16(d, 116));
          set_iso(u16(d, 118));
          set_isp(u08(d, 120));
          set_iue0(u08(d, 121));
          set_iue1(u08(d, 122));
          set_icd0(u16(d, 123));
          set_icd1(u16(d, 125));
          set_icr0(u32(d, 127));
          set_icr1(u32(d, 131));
          set_icg0(u08(d, 135));
          set_icg1(u08(d, 136));
          set_ico0(u16(d, 137));
          set_ico1(u16(d, 139));
          set_icf(u08(d, 141));
          set_icw(u08(d, 145));

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
          set_dcm(u08(d, 125 + 50));
          set_dal(u08(d, 126 + 50));
          set_dct(u16(d, 127 + 50));
          set_dca(u08(d, 129 + 50));
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
          set_psa(u08(d, 193 + 35));
          set_psi(u32(d, 194 + 35));
          set_pfm(u08(d, 198 + 35));
          set_pft(u16(d, 199 + 35));
          set_pfd(u16(d, 201 + 35));
          set_pfv(u16(d, 203 + 35));
          set_pdt(u16(d, 205 + 35));
          set_psm(u08(d, 207 + 35));
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
          set_sbm(u08(d, 283 + 10));
          set_sbt(u16(d, 284 + 10));
          set_sdt(u32(d, 286 + 10));
          set_ssa(u16(d, 290 + 10));
          set_ssi(u32(d, 292 + 10));
          set_sds(u16(d, 296 + 10));
          set_sam(u08(d, 298 + 10));
          set_sax(u16(d, 299 + 10));
          set_san(u16(d, 301 + 10));
          set_sao(u16(d, 303 + 10));
          set_sap(u16(d, 305 + 10));
          set_spt(u16(d, 307 + 10));

        }

        break;

      case 0x03006500: // ALAMO Software version 0x03006500.

        if(data.size() != 328) {
          ostringstream os;
          os << "Invalid X-Message: Expected 328 bytes, but received " << data.size() << " bytes!";
          EXC_THROW(os.str());
        }

        if(msg_idx == 0) {
          set_msn(u16(d, 8));
          set_dtd(u16(d, 10));
          set_mds(u16(d, 12));
          set_apt(u08(d, 14));
          set_xas(u08(d, 15));
          set_cps(u08(d, 16));
          set_cst(u08(d, 17));
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
          set_mam(u08(d, 46 + 6));
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

          set_icm(u08(d, 87));
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
          set_iss0(u08(d, 114));
          set_iss1(u08(d, 115));
          set_ist(u16(d, 116));
          set_iso(u16(d, 118));
          set_isp(u08(d, 120));
          set_iue0(u08(d, 121));
          set_iue1(u08(d, 122));
          set_icd0(u16(d, 123));
          set_icd1(u16(d, 125));
          set_icr0(u32(d, 127));
          set_icr1(u32(d, 131));
          set_icg0(u08(d, 135));
          set_icg1(u08(d, 136));

          set_ico0(u16(d, 137 + 9));
          set_ico1(u16(d, 139 + 9));
          set_icf(u08(d, 141 + 9));
          set_icw(u08(d, 145 + 9));

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
          set_dcm(u08(d, 125 + 59));
          set_dal(u08(d, 126 + 59));
          set_dct(u16(d, 127 + 59));
          set_dca(u08(d, 129 + 59));
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
          set_psa(u08(d, 193 + 44));
          set_psi(u32(d, 194 + 44));
          set_pfm(u08(d, 198 + 44));
          set_pft(u16(d, 199 + 44));
          set_pfd(u16(d, 201 + 44));
          set_pfv(u16(d, 203 + 44));
          set_pdt(u16(d, 205 + 44));
          set_psm(u08(d, 207 + 44));
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
          set_sbm(u08(d, 283 + 19));
          set_sbt(u16(d, 284 + 19));
          set_sdt(u32(d, 286 + 19));
          set_ssa(u16(d, 290 + 19));
          set_ssi(u32(d, 292 + 19));
          set_sds(u16(d, 296 + 19));
          set_sam(u08(d, 298 + 19));
          set_sax(u16(d, 299 + 19));
          set_san(u16(d, 301 + 19));
          set_sao(u16(d, 303 + 19));
          set_sap(u16(d, 305 + 19));
          set_spt(XMSG_BAD_U16);

        }

        break;

      case 0x03016700: // ALAMO Software version 0x03016700 (Solo-II Hybrid #10083).

        if(data.size() != 328) {
          ostringstream os;
          os << "Invalid X-Message: Expected 328 bytes, but received " << data.size() << " bytes!";
          EXC_THROW(os.str());
        }

        if(msg_idx == 0) {
          set_msn(u16(d, 8));
          set_dtd(u16(d, 10));
          set_mds(u16(d, 12));
          set_apt(u08(d, 14));
          set_xas(u08(d, 15));
          set_cps(u08(d, 16));
          set_cst(u08(d, 17));
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
          set_mam(u08(d, 46 + 6));
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

          set_icm(u08(d, 87));
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
          set_iss0(u08(d, 114));
          set_iss1(u08(d, 115));
          set_ist(u16(d, 116 + 1));
          set_iso(u16(d, 118 + 1));
          set_isp(u08(d, 120 + 1));
          set_iue0(u08(d, 121 + 1));
          set_iue1(u08(d, 122 + 1));
          set_icd0(u16(d, 123 + 1));
          set_icd1(u16(d, 125 + 1));
          set_icr0(u32(d, 127 + 1));
          set_icr1(u32(d, 131 + 1));

          set_icg0(u08(d, 135 + 1));
          set_icg1(u08(d, 136 + 1));
          set_ico0(u16(d, 137 + 10));
          set_ico1(u16(d, 139 + 10));
          set_icf(u08(d, 141 + 10));
          set_icw(u08(d, 145 + 10));

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
          set_dcm(u08(d, 125 + 60));
          set_dal(u08(d, 126 + 60));
          set_dct(u16(d, 127 + 60));
          set_dca(u08(d, 129 + 60));
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
          set_psa(u08(d, 193 + 45));
          set_psi(u32(d, 194 + 45));
          set_pfm(u08(d, 198 + 45));
          set_pft(u16(d, 199 + 45));
          set_pfd(u16(d, 201 + 45));
          set_pfv(u16(d, 203 + 45));
          set_pdt(u16(d, 205 + 45));
          set_psm(u08(d, 207 + 45));
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
          set_sbm(u08(d, 283 + 20));
          set_sbt(u16(d, 284 + 20));
          set_sdt(u32(d, 286 + 20));
          set_ssa(u16(d, 290 + 20));
          set_ssi(u32(d, 292 + 20));
          set_sds(u16(d, 296 + 20));
          set_sam(u08(d, 298 + 20));
          set_sax(u16(d, 299 + 20));
          set_san(u16(d, 301 + 20));
          set_sao(u16(d, 303 + 20));
          set_sap(u16(d, 305 + 20));

          // TODO: Implement additional parameters.
          set_spt(XMSG_BAD_U16);
        }
        break;

      case 0x03026100: // ALAMO Software version 0x03026100 (2016 NASCar floats).

        if(data.size() != 328) {
          ostringstream os;
          os << "Invalid X-Message: Expected 328 bytes, but received " << data.size() << " bytes!";
          EXC_THROW(os.str());
        }

        if(msg_idx == 0) {
          set_msn(u16(d, 8));
          set_dtd(u16(d, 10));
          set_mds(u16(d, 12));
          set_apt(u08(d, 14));
          set_btm(u08(d, 15));
          set_xas(u08(d, 15 + 1));
          set_cps(u08(d, 16 + 1));
          set_cst(u08(d, 17 + 1));
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
          set_mam(u08(d, 46 + 6 + 1));
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

          set_icm(u08(d, 87 + 1));
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
          set_iss0(u08(d, 114 + 1));
          set_iss1(u08(d, 115 + 1));
          set_ist(u16(d, 116 + 1 + 1));
          set_iso(u16(d, 118 + 1 + 1));
          set_isp(u08(d, 120 + 1 + 1));
          set_iue0(u08(d, 121 + 1 + 1));
          set_iue1(u08(d, 122 + 1 + 1));
          set_icd0(u16(d, 123 + 1 + 1));
          set_icd1(u16(d, 125 + 1 + 1));
          set_icr0(u32(d, 127 + 1 + 1));
          set_icr1(u32(d, 131 + 1 + 1));

          set_icg0(u08(d, 135 + 1 + 1));
          set_icg1(u08(d, 136 + 1 + 1));
          set_ico0(u16(d, 137 + 10 + 1));
          set_ico1(u16(d, 139 + 10 + 1));
          set_icf(u08(d, 141 + 10 + 1));
          set_icw(u08(d, 145 + 10 + 1));

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
          set_dcm(u08(d, 125 + 61));
          set_dal(u08(d, 126 + 61));
          set_dct(u16(d, 127 + 61));
          set_dca(u08(d, 129 + 61));
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
          set_psa(u08(d, 193 + 46));
          set_psi(u32(d, 194 + 46));
          set_pfm(u08(d, 198 + 46));
          set_pft(u16(d, 199 + 46));
          set_pfd(u16(d, 201 + 46));
          set_pfv(u16(d, 203 + 46));
          set_pdt(u16(d, 205 + 46));
          set_psm(u08(d, 207 + 46));
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
          set_sbm(u08(d, 283 + 21));
          set_sbt(u16(d, 284 + 21));
          set_sdt(u32(d, 286 + 21));
          set_ssa(u16(d, 290 + 21));
          set_ssi(u32(d, 292 + 21));
          set_sds(u16(d, 296 + 21));
          set_sam(u08(d, 298 + 21));
          set_sax(u16(d, 299 + 21));
          set_san(u16(d, 301 + 21));
          set_sao(u16(d, 303 + 21));
          set_sap(XMSG_BAD_U16);

          // TODO: Implement additional parameters.
          set_spt(XMSG_BAD_U16);

        }

        break;

      case 0x04016100: // ALAMO Software version 0x04016100 (2016 NASCar floats).

        if(data.size() != 328) {
          ostringstream os;
          os << "Invalid X-Message: Expected 328 bytes, but received " << data.size() << " bytes!";
          EXC_THROW(os.str());
        }

        if(msg_idx == 0) {
          set_msn(u16(d, 8));
          set_dtd(u16(d, 10));
          set_mds(u16(d, 12));
          set_apt(u08(d, 14));
          set_btm(u08(d, 15));
          set_xas(u08(d, 15 + 1));
          set_cps(u08(d, 16 + 1));
          set_cst(u08(d, 17 + 1));
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
          set_mam(u08(d, 46 + 6 + 1));
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

          set_icm(u08(d, 87 + 1));
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
          set_iss0(u08(d, 114 + 1));
          set_iss1(u08(d, 115 + 1));
          set_ist(u16(d, 116 + 1 + 1));
          set_iso(u16(d, 118 + 1 + 1));
          set_isp(u08(d, 120 + 1 + 1));
          set_iue0(u08(d, 121 + 1 + 1));
          set_iue1(u08(d, 122 + 1 + 1));
          set_icd0(u16(d, 123 + 1 + 1));
          set_icd1(u16(d, 125 + 1 + 1));
          set_icr0(u32(d, 127 + 1 + 1));
          set_icr1(u32(d, 131 + 1 + 1));

          set_icg0(u08(d, 135 + 1 + 1));
          set_icg1(u08(d, 136 + 1 + 1));
          set_ico0(u16(d, 137 + 10 + 1));
          set_ico1(u16(d, 139 + 10 + 1));
          set_icf(u08(d, 141 + 10 + 1));
          set_icw(u08(d, 145 + 10 + 1));

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
          set_dcm(u08(d, 125 + 61));
          set_dal(u08(d, 126 + 61));
          set_dct(u16(d, 127 + 61));
          set_dca(u08(d, 129 + 61));
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
          set_psa(u08(d, 193 + 46));
          set_psi(u32(d, 194 + 46));
          set_pfm(u08(d, 198 + 46));
          set_pft(u16(d, 199 + 46));
          set_pfd(u16(d, 201 + 46));
          set_pfv(u16(d, 203 + 46));
          set_pdt(u16(d, 205 + 46));
          set_psm(u08(d, 207 + 46));
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
          set_sbm(u08(d, 283 + 21));
          set_sbt(u16(d, 284 + 21));
          set_sdt(u32(d, 286 + 21));
          set_ssa(u16(d, 290 + 21));
          set_ssi(u32(d, 292 + 21));
          set_sds(u16(d, 296 + 21));
          set_sam(u08(d, 298 + 21));
          set_sax(u16(d, 299 + 21));
          set_san(u16(d, 301 + 21));
          set_sao(u16(d, 303 + 21));
          set_sap(XMSG_BAD_U16);

          // TODO: Implement additional parameters.
          set_spt(XMSG_BAD_U16);
        }
        break;

      case 0x04026200: // PAR floats

        if(msg_idx == 0) {

          if(data.size() != 323) {
            ostringstream os;
            os << "Invalid X-Message: Expected 323 bytes, but received " << data.size() << " bytes!";
  #         if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            {
              lwrn() << os.str();
            }
  #         else
            {
              EXC_THROW(os.str());
            }
  #         endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn(u16(d, 10));
          set_dtd(u16(d, 12));
          set_mds(u16(d, 14));
          set_apt(u08(d, 16));
          set_btm(u08(d, 17));
          set_xas(u08(d, 18));
          set_cps(u08(d, 19));
          set_cst(u08(d, 20));
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
          set_mam(u08(d, 55));
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

          set_icm(u08(d, 90));
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
          set_iss0(u08(d, 117));
          set_iss1(u08(d, 118));

          set_ist(u16(d, 119 + 1));
          set_iso(u16(d, 121 + 1));
          set_isp(u08(d, 123 + 1));
          set_iue0(u08(d, 124 + 1));
          set_iue1(u08(d, 125 + 1));
          set_icd0(u16(d, 126 + 1));
          set_icd1(u16(d, 128 + 1));
          set_icr0(u32(d, 130 + 1));
          set_icr1(u32(d, 134 + 1));
          set_icg0(u08(d, 138 + 1));
          set_icg1(u08(d, 139 + 1));
          set_isa(u08(d, 140 + 1));
          set_isd(u16(d, 141 + 1));
          set_iat(u16(d, 143 + 1));
          set_isv(u16(d, 145 + 1));
          set_isw(u16(d, 147 + 1));
          set_ico0(u16(d, 149 + 1));
          set_ico1(u16(d, 151 + 1));
          set_icf(u32(d, 153 + 1));
          set_icw(u32(d, 157 + 1));

          /* Descent state configuration: */

          set_descend_start_time(bpt::not_a_date_time);
          set_dns(u16(d, 161 + 1));
          set_dra(u16(d, 163 + 1));
          set_dsd(u16(d, 165 + 1));
          set_dst(u16(d, 167 + 1));
          set_ddf(u16(d, 169 + 1));
          set_ddc(u16(d, 171 + 1));
          set_dsi(u32(d, 173 + 1));
          set_ddd(u16(d, 177 + 1));
          set_ddt(u16(d, 179 + 1));
          set_dft(u16(d, 181 + 1));
          set_dcm(u08(d, 183 + 1));
          set_dal(u08(d, 184 + 1));
          set_dct(u16(d, 185 + 1));
          set_dca(u08(d, 187 + 1));
          set_drc(u16(d, 188 + 1));
          set_drt(u16(d, 190 + 1));
          set_dcx(u16(d, 192 + 1));
          set_dcp(u16(d, 194 + 1));
          set_dcf(u16(d, 196 + 1));
          set_dcd(u16(d, 198 + 1));
          set_dcr(u16(d, 200 + 1));
          set_dsw(u16(d, 202 + 1));
          set_duc(u16(d, 204 + 1));
          set_dpt(u16(d, 206 + 1));
          set_dut(u16(d, 208 + 1));
          set_dpd(u16(d, 210 + 1));
          set_dvt(u16(d, 212 + 1));

          set_dvd(u16(d, 214 + 1));
          set_dvw(u16(d, 216 + 1));

          set_pfr(u16(d, 218 + 1));
          set_pfc(u16(d, 220 + 1));
          set_ddp(u16(d, 222 + 1));
          set_dds(u16(d, 224 + 1));
          set_park_start_time(bpt::not_a_date_time);
          set_pns(u16(d, 226 + 1));
          set_pto(u32(d, 228 + 1));
          set_pth(u32(d, 232 + 1));
          set_psa(u08(d, 236 + 1));
          set_psi(u32(d, 237 + 1));
          set_pfm(u08(d, 241 + 1));
          set_pft(u16(d, 242 + 1));
          set_pfd(u16(d, 244 + 1));
          set_pfv(u16(d, 246 + 1));
          set_pdt(u16(d, 248 + 1));
          set_psm(u08(d, 250 + 1));
          set_psd(u16(d, 251 + 1));
          set_pst(u16(d, 253 + 1));
          set_psv(u16(d, 255 + 1));
          set_psw(u16(d, 257 + 1));
          set_ascent_start_time(bpt::not_a_date_time);
          set_asn(u16(d, 259 + 1));
          set_arr(u16(d, 261 + 1));
          set_apm(u16(d, 263 + 1));
          set_apn(u16(d, 265 + 1));
          set_apf(u16(d, 267 + 1));
          set_asi(u32(d, 269 + 1));
          set_asd(u16(d, 273 + 1));
          set_add(u16(d, 275 + 1));
          set_adt(u16(d, 277 + 1));
          set_arc(u16(d, 279 + 1));
          set_ahr(u16(d, 281 + 1));
          set_ahd(u16(d, 283 + 1));
          set_surface_start_time(bpt::not_a_date_time);
          set_sfn(u16(d, 285 + 1));
          set_sdc(i16(d, 287 + 1));
          set_sgt(u16(d, 289 + 1));
          set_set(u16(d, 291 + 1));
          set_sbm(u08(d, 293 + 1));
          set_sbt(u16(d, 294 + 1));
          set_sdt(u32(d, 296 + 1));
          set_ssa(u16(d, 300 + 1));
          set_ssi(u32(d, 302 + 1));
          set_sds(u16(d, 306 + 1));
          set_sam(u08(d, 308 + 1));
          set_sax(u16(d, 309 + 1));
          set_san(u16(d, 311 + 1));
          set_sao(u16(d, 313 + 1));
          set_sap(u16(d, 315 + 1));
          set_spt(u16(d, 317 + 1));
          set_ssp(u16(d, 319 + 1));
        }
        break;

      case 0x04036100: // ALAMO Software version 0x04036100 (2017 S2h test float 10108).

        if(msg_idx == 0) {

          if(data.size() != 327) {
            ostringstream os;
            os << "Invalid X-Message: Expected 327 bytes, but received " << data.size() << " bytes!";
  #         if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            {
              lwrn() << os.str();
            }
  #         else
            {
              EXC_THROW(os.str());
            }
  #         endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn(u16(d, 10));
          set_dtd(u16(d, 12));
          set_mds(u16(d, 14));
          set_apt(u08(d, 16));
          set_btm(u08(d, 17));
          set_xas(u08(d, 18));
          set_cps(u08(d, 19));
          set_cst(u08(d, 20));
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
          set_mam(u08(d, 55 + 4));
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

          set_icm(u08(d, 90 + 4));
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
          set_iss0(u08(d, 117 + 4));
          set_iss1(u08(d, 118 + 4));
          set_ist(u16(d, 119 + 5));
          set_iso(u16(d, 121 + 5));
          set_isp(u08(d, 123 + 5));
          set_iue0(u08(d, 124 + 5));
          set_iue1(u08(d, 125 + 5));
          set_icd0(u16(d, 126 + 5));
          set_icd1(u16(d, 128 + 5));
          set_icr0(u32(d, 130 + 5));
          set_icr1(u32(d, 134 + 5));
          set_icg0(u08(d, 138 + 5));
          set_icg1(u08(d, 139 + 5));
          set_isa(u08(d, 140 + 5));
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
          set_dcm(u08(d, 183 + 5));
          set_dal(u08(d, 184 + 5));
          set_dct(u16(d, 185 + 5));
          set_dca(u08(d, 187 + 5));
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
          set_psa(u08(d, 236 + 5));
          set_psi(u32(d, 237 + 5));
          set_pfm(u08(d, 241 + 5));
          set_pft(u16(d, 242 + 5));
          set_pfd(u16(d, 244 + 5));
          set_pfv(u16(d, 246 + 5));
          set_pdt(u16(d, 248 + 5));
          set_psm(u08(d, 250 + 5));
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
          set_sbm(u08(d, 293 + 5));
          set_sbt(u16(d, 294 + 5));
          set_sdt(u32(d, 296 + 5));
          set_ssa(u16(d, 300 + 5));
          set_ssi(u32(d, 302 + 5));
          set_sds(u16(d, 306 + 5));
          set_sam(u08(d, 308 + 5));
          set_sax(u16(d, 309 + 5));
          set_san(u16(d, 311 + 5));
          set_sao(u16(d, 313 + 5));
          set_sap(u16(d, 315 + 5));
          set_spt(u16(d, 317 + 5));
          set_ssp(u16(d, 319 + 5));
        }
        break;

      case 0x04046100: // ALAMO Software version 0x04046100 (2017 Hurricane Floats).



        if(msg_idx == 0) {

          if(data.size() != 328) {
            ostringstream os;
            os << "Invalid X-Message: Expected 328 bytes, but received " << data.size() << " bytes!";
  #         if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            {
              lwrn() << os.str();
            }
  #         else
            {
              EXC_THROW(os.str());
            }
  #         endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn(u16(d, 10));
          set_dtd(u16(d, 12));
          set_mds(u16(d, 14));
          set_apt(u08(d, 16));
          set_btm(u08(d, 17));
          set_xas(u08(d, 18));
          set_cps(u08(d, 19));
          set_cst(u08(d, 20));
          set_ctf(u16(d, 21));
          set_ctl(u16(d, 23));
          set_ctu(u16(d, 25));
          // set_ctc(u08(d, 27))

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
          set_mam(u08(d, 55 + 5));
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

          set_icm(u08(d, 90 + 5));
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
          set_iss0(u08(d, 117 + 5));
          set_iss1(u08(d, 118 + 5));
          set_ist(u16(d, 119 + 6));
          set_iso(u16(d, 121 + 6));
          set_isp(u08(d, 123 + 6));
          set_iue0(u08(d, 124 + 6));
          set_iue1(u08(d, 125 + 6));
          set_icd0(u16(d, 126 + 6));
          set_icd1(u16(d, 128 + 6));
          set_icr0(u32(d, 130 + 6));
          set_icr1(u32(d, 134 + 6));
          set_icg0(u08(d, 138 + 6));
          set_icg1(u08(d, 139 + 6));
          set_isa(u08(d, 140 + 6));
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
          set_dcm(u08(d, 183 + 6));
          set_dal(u08(d, 184 + 6));
          set_dct(u16(d, 185 + 6));
          set_dca(u08(d, 187 + 6));
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
          set_psa(u08(d, 236 + 6));
          set_psi(u32(d, 237 + 6));
          set_pfm(u08(d, 241 + 6));
          set_pft(u16(d, 242 + 6));
          set_pfd(u16(d, 244 + 6));
          set_pfv(u16(d, 246 + 6));
          set_pdt(u16(d, 248 + 6));
          set_psm(u08(d, 250 + 6));
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
          set_sbm(u08(d, 293 + 6));
          set_sbt(u16(d, 294 + 6));
          set_sdt(u32(d, 296 + 6));
          set_ssa(u16(d, 300 + 6));
          set_ssi(u32(d, 302 + 6));
          set_sds(u16(d, 306 + 6));
          set_sam(u08(d, 308 + 6));
          set_sax(u16(d, 309 + 6));
          set_san(u16(d, 311 + 6));
          set_sao(u16(d, 313 + 6));
          set_sap(u16(d, 315 + 6));
          set_spt(u16(d, 317 + 6));
          set_ssp(u16(d, 319 + 6));
        }
        break;

      case 0x04056300: // ALAMO Software version 0x04056300 (2017 S2h test float 10108).

        if(msg_idx == 0) {

          if(data.size() != 328) {
            ostringstream os;
            os << "Invalid X-Message: Expected 328 bytes, but received " << data.size() << " bytes!";
  #         if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            {
              lwrn() << os.str();
            }
  #         else
            {
              EXC_THROW(os.str());
            }
  #         endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_dtd( u16(d, 12));
          set_mds( u16(d, 14));
          set_apt( u08(d, 16));
          set_btm( u08(d, 17));
          set_xas( u08(d, 18));
          set_cps( u08(d, 19));
          set_cst( u08(d, 20));
          set_ctf( u16(d, 21));
          set_ctl( u16(d, 23));
          set_ctu( u16(d, 25));
          set_ctc( u08(d, 27));
          set_csp( u32(d, 27 + 1));
          set_cpb1(u16(d, 31 + 1));
          set_cpb2(u16(d, 33 + 1));
          set_cpb3(u16(d, 35 + 1));
          set_cpn1(u16(d, 37 + 1));
          set_cpn2(u16(d, 39 + 1));
          set_cpn3(u16(d, 41 + 1));
          set_cpp1(u16(d, 43 + 1));
          set_cpp2(u16(d, 45 + 1));
          set_cpp3(u16(d, 47 + 1));
          set_crd( u16(d, 50));
          set_crs( u16(d, 52));

          set_mtd(u16(d, 49 + 5));
          set_mtt(u32(d, 51 + 5));
          set_mam(u08(d, 55 + 5));
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

          set_icm(u08(d, 90 + 9));
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
          set_iss0(u08(d, 117 + 9));
          set_iss1(u08(d, 118 + 9));
          set_ist(u16(d, 119 + 10));
          set_iso(u16(d, 121 + 10));
          set_isp(u08(d, 123 + 10));
          set_iue0(u08(d, 124 + 10));
          set_iue1(u08(d, 125 + 10));
          set_icd0(u16(d, 126 + 10));
          set_icd1(u16(d, 128 + 10));
          set_icr0(u32(d, 130 + 10));
          set_icr1(u32(d, 134 + 10));
          set_icg0(u08(d, 138 + 10));
          set_icg1(u08(d, 139 + 10));
          set_isa(u08(d, 140 + 10));
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
          set_dcm(u08(d, 183 + 10));
          set_dal(u08(d, 184 + 10));
          set_dct(u16(d, 185 + 10));
          set_dca(u08(d, 187 + 10));
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
          set_psa(u08(d, 236 + 14));
          set_psi(u32(d, 237 + 14));
          set_pfm(u08(d, 241 + 14));
          set_pft(u16(d, 242 + 14));
          set_pfd(u16(d, 244 + 14));
          set_pfv(u16(d, 246 + 14));
          set_pdt(u16(d, 248 + 14));
          set_psm(u08(d, 250 + 14));
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
          set_sbm(u08(d, 293 + 14));
          set_sbt(u16(d, 294 + 14));
          set_sdt(u32(d, 296 + 14));
          set_ssa(u16(d, 300 + 14));
          set_ssi(u32(d, 302 + 14));
          set_sds(u16(d, 306 + 14));
          set_sam(u08(d, 308 + 14));
          set_sax(u16(d, 309 + 14));
          set_san(u16(d, 311 + 14));

        } else if(msg_idx == 1) {

          set_sao(u16(d, 3));
          set_sap(u16(d, 5));
          set_spt(u16(d, 7));
          set_ssp(u16(d, 9));

        }
        break;

      case 0x04066100: // ALAMO Software version 0x04066100 (2017 S2h test float 10108).

        if(msg_idx == 0) {

          if(data.size() != 328) {
            ostringstream os;
            os << "Invalid X-Message: Expected 328 bytes, but received " << data.size() << " bytes!";
  #         if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            {
              lwrn() << os.str();
            }
  #         else
            {
              EXC_THROW(os.str());
            }
  #         endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn(u16(d, 10));
          set_dtd(u16(d, 12));
          set_mds(u16(d, 14));
          set_apt(u08(d, 16));
          set_btm(u08(d, 17));
          set_xas(u08(d, 18));
          set_cps(u08(d, 19));
          set_cst(u08(d, 20));
          set_ctf(u16(d, 21));
          set_ctl(u16(d, 23));
          set_ctu(u16(d, 25));
          // set_ctc(u08(d, 27))

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
          set_mam(u08(d, 55 + 5));
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

          set_icm(u08(d, 90 + 9));
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
          set_iss0(u08(d, 117 + 9));
          set_iss1(u08(d, 118 + 9));
          set_ist(u16(d, 119 + 10));
          set_iso(u16(d, 121 + 10));
          set_isp(u08(d, 123 + 10));
          set_iue0(u08(d, 124 + 10));
          set_iue1(u08(d, 125 + 10));
          set_icd0(u16(d, 126 + 10));
          set_icd1(u16(d, 128 + 10));
          set_icr0(u32(d, 130 + 10));
          set_icr1(u32(d, 134 + 10));
          set_icg0(u08(d, 138 + 10));
          set_icg1(u08(d, 139 + 10));
          set_isa(u08(d, 140 + 10));
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
          set_dcm(u08(d, 183 + 10));
          set_dal(u08(d, 184 + 10));
          set_dct(u16(d, 185 + 10));
          set_dca(u08(d, 187 + 10));
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
          set_psa(u08(d, 236 + 14));
          set_psi(u32(d, 237 + 14));
          set_pfm(u08(d, 241 + 14));
          set_pft(u16(d, 242 + 14));
          set_pfd(u16(d, 244 + 14));
          set_pfv(u16(d, 246 + 14));
          set_pdt(u16(d, 248 + 14));
          set_psm(u08(d, 250 + 14));
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
          set_sbm(u08(d, 293 + 14));
          set_sbt(u16(d, 294 + 14));
          set_sdt(u32(d, 296 + 14));
          set_ssa(u16(d, 300 + 14));
          set_ssi(u32(d, 302 + 14));
          set_sds(u16(d, 306 + 14));
          set_sam(u08(d, 308 + 14));
          set_sax(u16(d, 309 + 14));
          set_san(u16(d, 311 + 14));
          set_sao(u16(d, 313 + 14));
//                set_sap(u16(d, 315 + 14));
//                set_spt(u16(d, 317 + 14));
//                set_ssp(u16(d, 319 + 14));
        }
        break;

      case 0x05006100: // ALAMO Software version 0x05006100 (2017 Alto test float 11011).

        if(msg_idx == 0) {
          if(data.size() != 328) {
            ostringstream os;
            os << "Invalid X-Message: Expected 328 bytes, but received " << data.size() << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 64+1));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 97+1));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 248+1));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 303+1));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));
          set_pfv1(u16(d, 324+1));
//          set_pfv2(u16(d, 326));
//          set_pdt( u16(d, 324));
//          set_psm( u08(d, 326));

        } else if(msg_idx == 1) {
          if(data.size() != 79) {
            ostringstream os;
            os << "Invalid X-Message: Expected 79 bytes, but received " << data.size() << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_pfv2(u16(d, 3));
          set_pdt( u16(d, 5));
          set_psm( u08(d, 7));
          set_psd( u16(d, 3+5));
          set_pst( u16(d, 5+5));
          set_psv( u16(d, 7+5));
          set_psw( u16(d, 9+5));
          set_asn( u16(d, 11+5));
          set_arr( u16(d, 13+5));
          set_apm( u16(d, 15+5));
          set_apn( u16(d, 17+5));
          set_apf( u16(d, 19+5));
          set_asi( u32(d, 21+5));
          set_asd( u16(d, 25+5));
          set_add( u16(d, 27+5));
          set_adt( u16(d, 29+5));
          set_arc( u16(d, 31+5));
          set_ahr( u16(d, 33+5));
          set_ahd( u16(d, 35+5));
          set_sfn( u16(d, 37+5));
          set_sdc( i16(d, 39+5));
          set_sgt( u16(d, 41+5));
          set_set( u16(d, 43+5));
          set_sbm( u08(d, 45+5));
          set_sbt( u16(d, 46+5));
          set_sdt( u32(d, 48+5));
          set_ssa( u16(d, 52+5));
          set_ssi( u32(d, 54+5));
          set_sds( u16(d, 58+5));
          set_sam( u08(d, 60+5));
          set_sax( u16(d, 61+5));
          set_san( u16(d, 63+5));
          set_sao( u16(d, 65+5));
          set_sap( u16(d, 67+5));
          set_spt( u16(d, 69+5));
          set_ssp( u16(d, 71+5));
        }
        break;

      case 0x05006101: // ALAMO Software version 0x05006101 alternate (2017 Alto test float 10109).

        if(msg_idx == 0) {
          if(data.size() != 328) {
            ostringstream os;
            os << "Invalid X-Message: Expected 328 bytes, but received " << data.size() << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_xas( u08(d, 34));
          set_cps( u08(d, 35));
          set_cst( u08(d, 36));
          set_ctf( u16(d, 37));
          set_ctl( u16(d, 39));
          set_ctu( u16(d, 41));
          set_ctc( u08(d, 43));
          set_csp( u32(d, 44));
          set_cpb1(u16(d, 48));
          set_cpb2(u16(d, 50));
          set_cpb3(u16(d, 52));
          set_cpn1(u16(d, 54));
          set_cpn2(u16(d, 56));
          set_cpn3(u16(d, 58));
          set_cpp1(u16(d, 60));
          set_cpp2(u16(d, 62));
          set_cpp3(u16(d, 64));
          set_crd( u16(d, 66));
          set_crs( u16(d, 68));
          set_mtd( u16(d, 70));
          set_mtt( u32(d, 72));
          set_mts( u32(d, 76));
          set_mam( u08(d, 80));
          set_mat2(u16(d, 81));
//          set_mat2(u16(d, 83));
//          set_map( u16(d, 85));
          set_aca( u16(d, 87-4));
          set_acg( u16(d, 89-4));
          set_acr( u16(d, 91-4));
          set_pnb( u16(d, 93-4));
          set_pnc( u16(d, 95-4));
          set_pxc( u16(d, 97-4));
          set_pbl( u16(d, 99-4));
          set_pbh( u16(d, 101-4));
          set_pp0( u16(d, 103-4));
          set_pp1( u16(d, 105-4));
          set_pvl( u16(d, 107-4));
          set_pvh( u16(d, 109-4));
          set_pv0( u16(d, 111-4));
          set_pv5( u16(d, 113-4));
          set_pc0( u16(d, 115-4));
          set_pc1( u16(d, 117-4));

          /* Ice Configuration */

          set_icm( u08(d, 119-4));
          set_imm( u16(d, 120-4));
          set_ica0(u16(d, 122-4));
          set_ica1(u16(d, 124-4));
          set_icb0(u16(d, 126-4));
          set_icb1(u16(d, 128-4));
          set_imt0(u16(d, 130-4));
          set_imt1(u16(d, 132-4));
          set_ilt0(u16(d, 134-4));
          set_ilt1(u16(d, 136-4));
          set_icn( u16(d, 138-4));
          set_icx( u16(d, 140-4));
          set_ict( i16(d, 142-4));
          set_ics( u16(d, 144-4));
          set_iss0(u08(d, 146-4));
          set_iss1(u08(d, 147-4));
          set_ism( u08(d, 148-4));
          set_ist( u16(d, 149-4));
          set_iso( u16(d, 151-4));
          set_isp( u08(d, 153-4));
          set_iue0(u08(d, 154-4));
          set_iue1(u08(d, 155-4));
          set_icd0(u16(d, 156-4));
          set_icd1(u16(d, 158-4));
          set_icr0(u32(d, 160-4));
          set_icr1(u32(d, 164-4));
          set_icg0(u08(d, 168-4));
          set_icg1(u08(d, 169-4));
          set_isa( u08(d, 171-4));
          set_isd( u16(d, 171-4));
          set_iat( u16(d, 173-4));
          set_isv( u16(d, 175-4));
          set_isw( u16(d, 177-4));
          set_ico0(u16(d, 179-4));
          set_ico1(u16(d, 181-4));
          set_icf( u32(d, 183-4));
          set_icw( u32(d, 187-4));

          /* Descent state configuration: */

          set_dns( u16(d, 191-4));
          set_dra( u16(d, 193-4));
          set_dsd0(u16(d, 195-4));
          set_dsd1(u16(d, 197-4));
          set_dsd2(u16(d, 199-4));
          set_dst0(u16(d, 201-4));
          set_dst1(u16(d, 203-4));
          set_dst2(u16(d, 205-4));
          set_ddf( u16(d, 207-4));
          set_ddc( u16(d, 209-4));
          set_dsi( u32(d, 211-4));
          set_ddd( u16(d, 215-4));
          set_ddt( u16(d, 217-4));
          set_dft0(u16(d, 219-4));
          set_dft1(u16(d, 221-4));
          set_dft2(u16(d, 223-4));
          set_dcm( u08(d, 225-4));
          set_dal( u08(d, 226-4));
          set_dct0(u16(d, 227-4));
          set_dct1(u16(d, 229-4));
          set_dct2(u16(d, 231-4));
          set_dca( u08(d, 233-4));
          set_drc( u16(d, 234-4));
          set_drt( u16(d, 236-4));
          set_dcx( u16(d, 238-4));
          set_dcp( u16(d, 240-4));
          set_dcf( u16(d, 242-4));
          set_dcd( u16(d, 244-4));
          set_dcr( u16(d, 246-4));
          set_dsw( u16(d, 248-4));
          set_duc( u16(d, 250-4));
          set_dpt( u16(d, 252-4));
          set_dut( u16(d, 254-4));
          set_dpd( u16(d, 256-4));
          set_dvt( u16(d, 258-4));
          set_dvd( u16(d, 260-4));
          set_dvw( u16(d, 262-4));
          set_pfr( u16(d, 264-4));
          set_pfc( u16(d, 266-4));
          set_ddp( u16(d, 268-4));
          set_dds( u16(d, 270-4));
          set_pns( u16(d, 272-4));
          set_pto0(u32(d, 274-4));
          set_pto1(u32(d, 278-4));
          set_pto2(u32(d, 282-4));
          set_pth0(u32(d, 286-4));
          set_pth1(u32(d, 290-4));
          set_pth2(u32(d, 294-4));
          set_ptd( u32(d, 298-4));
          set_psa( u08(d, 302-4));
          set_psi( u32(d, 303-4));
          set_pfm0(u08(d, 307-4));
          set_pfm1(u08(d, 308-4));
          set_pfm2(u08(d, 309-4));
          set_pft0(u16(d, 310-4));
          set_pft1(u16(d, 312-4));
          set_pft2(u16(d, 314-4));
          set_pfd0(u16(d, 316-4));
          set_pfd1(u16(d, 318-4));
          set_pfd2(u16(d, 320-4));
          set_pfv0(u16(d, 322-4));
          set_pfv1(u16(d, 324-4));
          set_pfv2(u16(d, 326-4));

          set_pdt( u16(d, 324));
          set_psm( u08(d, 326));

        } else if(msg_idx == 1) {
          if(data.size() != 74) {
            ostringstream os;
            os << "Invalid X-Message: Expected 74 bytes, but received " << data.size() << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

//          set_pdt( u16(d, ));
//          set_psm( u08(d, ));
          set_psd( u16(d, 3));
          set_pst( u16(d, 5));
          set_psv( u16(d, 7));
          set_psw( u16(d, 9));
          set_asn( u16(d, 11));
          set_arr( u16(d, 13));
          set_apm( u16(d, 15));
          set_apn( u16(d, 17));
          set_apf( u16(d, 19));
          set_asi( u32(d, 21));
          set_asd( u16(d, 25));
          set_add( u16(d, 27));
          set_adt( u16(d, 29));
          set_arc( u16(d, 31));
          set_ahr( u16(d, 33));
          set_ahd( u16(d, 35));
          set_sfn( u16(d, 37));
          set_sdc( i16(d, 39));
          set_sgt( u16(d, 41));
          set_set( u16(d, 43));
          set_sbm( u08(d, 45));
          set_sbt( u16(d, 46));
          set_sdt( u32(d, 48));
          set_ssa( u16(d, 52));
          set_ssi( u32(d, 54));
          set_sds( u16(d, 58));
          set_sam( u08(d, 60));
          set_sax( u16(d, 61));
          set_san( u16(d, 63));
          set_sao( u16(d, 65));
          set_sap( u16(d, 67));
          set_spt( u16(d, 69));
          set_ssp( u16(d, 71));
        }
        break;

      case 0x06000000: // ALAMO Software version 0x06000000

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 64+1));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 97+1));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 248+1));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 303+1));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));
          set_pfv1(u16(d, 324+1));
//          set_pfv2(u16(d, 326));
//          set_pdt( u16(d, 324));
//          set_psm( u08(d, 326));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 81;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_pfv2(u16(d, 3));
          set_pdt( u16(d, 5));
          set_psm( u08(d, 7));
          set_psd( u16(d, 3+5));
          set_pst( u16(d, 5+5));
          set_psv( u16(d, 7+5));
          set_psw( u16(d, 9+5));
          set_asn( u16(d, 11+5));
          set_arr( u16(d, 13+5));
          set_apm( u16(d, 20));
          set_aps( u16(d, 22));
          set_apn( u16(d, 24));
          set_apf( u16(d, 19+7));
          set_asi( u32(d, 21+7));
          set_asd( u16(d, 25+7));
          set_add( u16(d, 27+7));
          set_adt( u16(d, 29+7));
          set_arc( u16(d, 31+7));
          set_ahr( u16(d, 33+7));
          set_ahd( u16(d, 35+7));
          set_sfn( u16(d, 37+7));
          set_sdc( i16(d, 39+7));
          set_sgt( u16(d, 41+7));
          set_set( u16(d, 43+7));
          set_sbm( u08(d, 45+7));
          set_sbt( u16(d, 46+7));
          set_sdt( u32(d, 48+7));
          set_ssa( u16(d, 52+7));
          set_ssi( u32(d, 54+7));
          set_sds( u16(d, 58+7));
          set_sam( u08(d, 60+7));
          set_sax( u16(d, 61+7));
          set_san( u16(d, 63+7));
          set_sao( u16(d, 65+7));
          set_sap( u16(d, 67+7));
          set_spt( u16(d, 69+7));
          set_ssp( u16(d, 71+7));
        }
        break;

      case 0x06010300: // ALAMO Software version 0x06000100 (2019 ALTO RBR/O2 test float 11060)

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 64+1));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 97+1));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 248+1));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 303+1));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));
          set_pfv1(u16(d, 324+1));
//          set_pfv2(u16(d, 326));
//          set_pdt( u16(d, 324));
//          set_psm( u08(d, 326));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 81;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_pfv2(u16(d, 3));
          set_pdt( u16(d, 5));
          set_psm( u08(d, 7));
          set_psd( u16(d, 3+5));
          set_pst( u16(d, 5+5));
          set_psv( u16(d, 7+5));
          set_psw( u16(d, 9+5));
          set_asn( u16(d, 11+5));
          set_arr( u16(d, 13+5));
          set_apm( u16(d, 20));
          set_aps( u16(d, 22));
          set_apn( u16(d, 24));
          set_apf( u16(d, 19+7));
          set_asi( u32(d, 21+7));
          set_asd( u16(d, 25+7));
          set_add( u16(d, 27+7));
          set_adt( u16(d, 29+7));
          set_arc( u16(d, 31+7));
          set_ahr( u16(d, 33+7));
          set_ahd( u16(d, 35+7));
          set_sfn( u16(d, 37+7));
          set_sdc( i16(d, 39+7));
          set_sgt( u16(d, 41+7));
          set_set( u16(d, 43+7));
          set_sbm( u08(d, 45+7));
          set_sbt( u16(d, 46+7));
          set_sdt( u32(d, 48+7));
          set_ssa( u16(d, 52+7));
          set_ssi( u32(d, 54+7));
          set_sds( u16(d, 58+7));
          set_sam( u08(d, 60+7));
          set_sax( u16(d, 61+7));
          set_san( u16(d, 63+7));
          set_sao( u16(d, 65+7));
          set_sap( u16(d, 67+7));
          set_spt( u16(d, 69+7));
          set_ssp( u16(d, 71+7));
        }
        break;

      case 0x06000100: // ALAMO Software version 0x06000100

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 64+1));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 97+1));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 248+1));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 303+1));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));
          set_pfv1(u16(d, 324+1));
//          set_pfv2(u16(d, 326));
//          set_pdt( u16(d, 324));
//          set_psm( u08(d, 326));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 81;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_pfv2(u16(d, 3));
          set_pdt( u16(d, 5));
          set_psm( u08(d, 7));
          set_psd( u16(d, 3+5));
          set_pst( u16(d, 5+5));
          set_psv( u16(d, 7+5));
          set_psw( u16(d, 9+5));
          set_asn( u16(d, 11+5));
          set_arr( u16(d, 13+5));
          set_apm( u16(d, 20));
          set_aps( u16(d, 22));
          set_apn( u16(d, 24));
          set_apf( u16(d, 19+7));
          set_asi( u32(d, 21+7));
          set_asd( u16(d, 25+7));
          set_add( u16(d, 27+7));
          set_adt( u16(d, 29+7));
          set_arc( u16(d, 31+7));
          set_ahr( u16(d, 33+7));
          set_ahd( u16(d, 35+7));
          set_sfn( u16(d, 37+7));
          set_sdc( i16(d, 39+7));
          set_sgt( u16(d, 41+7));
          set_set( u16(d, 43+7));
          set_sbm( u08(d, 45+7));
          set_sbt( u16(d, 46+7));
          set_sdt( u32(d, 48+7));
          set_ssa( u16(d, 52+7));
          set_ssi( u32(d, 54+7));
          set_sds( u16(d, 58+7));
          set_sam( u08(d, 60+7));
          set_sax( u16(d, 61+7));
          set_san( u16(d, 63+7));
          set_sao( u16(d, 65+7));
          set_sap( u16(d, 67+7));
          set_spt( u16(d, 69+7));
          set_ssp( u16(d, 71+7));
        }
        break;

      case 0x06040900: // ALAMO Software version 0x06040900 (v6.4.9: K. Wood's 2019 Arctic PAR floats)

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 64+1));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 97+1));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 248+1));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 303+1));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));
          set_pfv1(u16(d, 324+1));
//          set_pfv2(u16(d, 326));
//          set_pdt( u16(d, 324));
//          set_psm( u08(d, 326));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 81;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_pfv2(u16(d, 3));
          set_pdt( u16(d, 5));
          set_psm( u08(d, 7));
          set_psd( u16(d, 3+5));
          set_pst( u16(d, 5+5));
          set_psv( u16(d, 7+5));
          set_psw( u16(d, 9+5));
          set_asn( u16(d, 11+5));
          set_arr( u16(d, 13+5));
          set_apm( u16(d, 20));
          set_aps( u16(d, 22));
          set_apn( u16(d, 24));
          set_apf( u16(d, 19+7));
          set_asi( u32(d, 21+7));
          set_asd( u16(d, 25+7));
          set_add( u16(d, 27+7));
          set_adt( u16(d, 29+7));
          set_arc( u16(d, 31+7));
          set_ahr( u16(d, 33+7));
          set_ahd( u16(d, 35+7));
          set_sfn( u16(d, 37+7));
          set_sdc( i16(d, 39+7));
          set_sgt( u16(d, 48));
          set_set( u16(d, 50));
          set_sbm( u08(d, 52));
          set_sbt( u16(d, 53));
          set_sdt( u32(d, 55));
          set_ssa( u08(d, 59)); // ???
          set_ssi( u16(d, 60)); // ???
          // byte 62?
//          set_sds( u16(d, 58+7));
          set_sam( u08(d, 63));
          set_sax( u16(d, 64));
          set_san( u16(d, 66));
          set_sao( u16(d, 68));
          set_sap( u16(d, 70));
          set_spt( u16(d, 72));
          set_ssp( u16(d, 74));
        }
        break;

      case 0x06050100: // ALAMO Software version 0x06050100

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 65));  // OK
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 98));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 249));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 304));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));
          set_pfv1(u16(d, 324+1));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 77;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_pfv2(u16(d, 3));
          set_pdt( u16(d, 5));
          set_psm( u08(d, 7));
          set_psd( u16(d, 3+5));
          set_pst( u16(d, 5+5));
          set_psv( u16(d, 7+5));
          set_psw( u16(d, 9+5));
          set_asn( u16(d, 11+5));
          set_arr( u16(d, 13+5));
          set_apm( u16(d, 20));
          set_aps( u16(d, 22));
          set_apn( u16(d, 24));
          set_apf( u16(d, 19+7));
          set_asi( u32(d, 21+7));
          set_asd( u16(d, 25+7));
          set_add( u16(d, 27+7));
          set_adt( u16(d, 29+7));
          set_arc( u16(d, 31+7));
          set_ahr( u16(d, 33+7));
          set_ahd( u16(d, 35+7));
          set_sfn( u16(d, 37+7));
          set_sdc( i16(d, 39+7));
          set_sgt( u16(d, 41+7));
          set_set( u16(d, 43+7));
          set_sbm( u08(d, 45+7));
          set_sbt( u16(d, 46+7));
          set_ssa( u16(d, 55));
          set_ssi( u32(d, 57));
          set_sds( u16(d, 61));
          set_sam( u08(d, 63));
          set_sax( u16(d, 64));
          set_san( u16(d, 66));
          set_sao( u16(d, 68));
          set_sap( u16(d, 70));
          set_spt( u16(d, 72));
          set_ssp( u16(d, 74));
        }
        break;

      case 0x06057700: // ALAMO Software version 0x06057700 (v6.9.w: B. Sanabia's 2019 Hurricane floats w/ WIMS)

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 35));
          set_cps( u08(d, 36));
          set_cst( u08(d, 37));
          set_ctf( u16(d, 38));
          set_ctl( u16(d, 40));
          set_ctu( u16(d, 42));
          set_ctc( u08(d, 44));
          set_csp( u32(d, 45));
          set_cpb1(u16(d, 49));
          set_cpb2(u16(d, 51));
          set_cpb3(u16(d, 53));
          set_cpn1(u16(d, 55));
          set_cpn2(u16(d, 57));
          set_cpn3(u16(d, 59));
          set_cpp1(u16(d, 61));
          set_cpp2(u16(d, 63));
          set_cpp3(u16(d, 65));
          set_crd( u16(d, 67));
          set_crs( u16(d, 69));
          set_mtd( u16(d, 71));
          set_mtt( u32(d, 73));
          set_mts( u32(d, 77));
          set_mam( u08(d, 81));
          set_mat1(u16(d, 82));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 88));
          set_acg( u16(d, 90));
          set_acr( u16(d, 92));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 97+1));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 248+1));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 303+1));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 79;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }
          set_pfv1(u16(d, 3));
          set_pfv2(u16(d, 5));
          set_pdt( u16(d, 7));
          set_psm( u08(d, 9));
          set_psd( u16(d, 10));
          set_pst( u16(d, 12));
          set_psv( u16(d, 14));
          set_psw( u16(d, 16));
          set_asn( u16(d, 18));
          set_arr( u16(d, 20));
          set_apm( u16(d, 22));
          set_aps( u16(d, 24));
          set_apn( u16(d, 26));
          set_apf( u16(d, 28));
          set_asi( u32(d, 30));
          set_asd( u16(d, 34));
          set_add( u16(d, 36));
          set_adt( u16(d, 38));
          set_arc( u16(d, 40));
          set_ahr( u16(d, 42));
          set_ahd( u16(d, 44));
          set_sfn( u16(d, 46));
          set_sdc( i16(d, 48));
          set_sgt( u16(d, 50));
          set_set( u16(d, 52));
          set_sbm( u08(d, 54));
          set_sbt( u16(d, 55));
          set_ssa( u16(d, 57));
          set_ssi( u32(d, 59));
          set_sds( u16(d, 63));
          set_sam( u08(d, 65));
          set_sax( u16(d, 66));
          set_san( u16(d, 68));
          set_sao( u16(d, 70));
          set_sap( u16(d, 72));
          set_spt( u16(d, 74));
          set_ssp( u16(d, 76));
        }
        break;

      case 0x06080500: // ALAMO Software version 0x06080500 (2019 ALTO #11011 test float)

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 35));
          set_cps( u08(d, 36));
          set_cst( u08(d, 37));
          set_ctf( u16(d, 38));
          set_ctl( u16(d, 40));
          set_ctu( u16(d, 42));
          set_ctc( u08(d, 44));
          set_csp( u32(d, 45));
          set_cpb1(u16(d, 49));
          set_cpb2(u16(d, 51));
          set_cpb3(u16(d, 53));
          set_cpn1(u16(d, 55));
          set_cpn2(u16(d, 57));
          set_cpn3(u16(d, 59));
          set_cpp1(u16(d, 61));
          set_cpp2(u16(d, 63));
          set_cpp3(u16(d, 65));
          set_crd( u16(d, 67));
          set_crs( u16(d, 69));
          set_mtd( u16(d, 71));
          set_mtt( u32(d, 73));
          set_mts( u32(d, 77));
          set_mam( u08(d, 81));
          set_mat1(u16(d, 82));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 88));
          set_acg( u16(d, 90));
          set_acr( u16(d, 92));
          //tsp0
          //tsp1
          //tsn0
          //tsn1
          set_pnb( u16(d, 93+3));
          set_pnc( u16(d, 95+3));
          set_pxc( u16(d, 97+3));
          set_pbl( u16(d, 99+3));
          set_pbh( u16(d, 101+3));
          set_pp0( u16(d, 103+3));
          set_pp1( u16(d, 105+3));
          set_pvl( u16(d, 107+3));
          set_pvh( u16(d, 109+3));
          set_pv0( u16(d, 111+3));
          set_pv5( u16(d, 113+3));
          set_pc0( u16(d, 115+3));
          set_pc1( u16(d, 117+3));

          /* Ice Configuration */

          set_icm( u08(d, 119+3));
          set_imm( u16(d, 120+3));
          set_ica0(u16(d, 122+3));
          set_ica1(u16(d, 124+3));
          set_icb0(u16(d, 126+3));
          set_icb1(u16(d, 128+3));
          set_imt0(u16(d, 130+3));
          set_imt1(u16(d, 132+3));
          set_ilt0(u16(d, 134+3));
          set_ilt1(u16(d, 136+3));
          set_icn( u16(d, 138+3));
          set_icx( u16(d, 140+3));
          set_ict( i16(d, 142+3));
          set_ics( u16(d, 144+3));
          set_iss0(u08(d, 146+3));
          set_iss1(u08(d, 147+3));
          set_ism( u08(d, 148+3));
          set_ist( u16(d, 149+3));
          set_iso( u16(d, 151+3));
          set_isp( u08(d, 153+3));
          set_iue0(u08(d, 154+3));
          set_iue1(u08(d, 155+3));
          set_icd0(u16(d, 156+3));
          set_icd1(u16(d, 158+3));
          set_icr0(u32(d, 160+3));
          set_icr1(u32(d, 164+3));
          set_icg0(u08(d, 168+3));
          set_icg1(u08(d, 169+3));
          set_isa( u08(d, 171+3));
          set_isd( u16(d, 171+3));
          set_iat( u16(d, 173+3));
          set_isv( u16(d, 175+3));
          set_isw( u16(d, 177+3));
          set_ico0(u16(d, 179+3));
          set_ico1(u16(d, 181+3));
          set_icf( u32(d, 183+3));
          set_icw( u32(d, 187+3));

          /* Descent state configuration: */

          set_dns( u16(d, 191+3));
          set_dra( u16(d, 193+3));
          set_dsd0(u16(d, 195+3));
          set_dsd1(u16(d, 197+3));
          set_dsd2(u16(d, 199+3));
          set_dst0(u16(d, 201+3));
          set_dst1(u16(d, 203+3));
          set_dst2(u16(d, 205+3));
          set_ddf( u16(d, 207+3));
          set_ddc( u16(d, 209+3));
          set_dsi( u32(d, 211+3));
          set_ddd( u16(d, 215+3));
          set_ddt( u16(d, 217+3));
          set_dft0(u16(d, 219+3));
          set_dft1(u16(d, 221+3));
          set_dft2(u16(d, 223+3));
          set_dcm( u08(d, 225+3));
          set_dal( u08(d, 226+3));
          set_dct0(u16(d, 227+3));
          set_dct1(u16(d, 229+3));
          set_dct2(u16(d, 231+3));
          set_dca( u08(d, 233+3));
          set_drc( u16(d, 234+3));
          set_drt( u16(d, 236+3));
          set_dcx( u16(d, 238+3));
          set_dcp( u16(d, 240+3));
          set_dcf( u16(d, 242+3));
          set_dcd( u16(d, 244+3));
          set_dcr( u16(d, 246+3));
          set_dsw( u16(d, 248+3));
          set_duc( u16(d, 250+3));
          set_dpt( u16(d, 252+3));
          set_dut( u16(d, 254+3));
          set_dpd( u16(d, 256+3));
          set_dvt( u16(d, 258+3));
          set_dvd( u16(d, 260+3));
          set_dvw( u16(d, 262+3));
          set_pfr( u16(d, 264+3));
          set_pfc( u16(d, 266+3));
          set_ddp( u16(d, 268+3));
          set_dds( u16(d, 270+3));
          set_pns( u16(d, 272+3));
          set_pto0(u32(d, 274+3));
          set_pto1(u32(d, 278+3));
          set_pto2(u32(d, 282+3));
          set_pth0(u32(d, 286+3));
          set_pth1(u32(d, 290+3));
          set_pth2(u32(d, 294+3));
          set_ptd( u32(d, 298+3));
          set_psa( u08(d, 302+3));
          set_psi( u32(d, 303+3));
          set_pfm0(u08(d, 307+3));
          set_pfm1(u08(d, 308+3));
          set_pfm2(u08(d, 309+3));
          set_pft0(u16(d, 310+3));
          set_pft1(u16(d, 312+3));
          set_pft2(u16(d, 314+3));
          set_pfd0(u16(d, 316+3));
          set_pfd1(u16(d, 318+3));
          set_pfd2(u16(d, 320+3));
          set_pfv0(u16(d, 322+3));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 79;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }
          set_pfv1(u16(d, 3));
          set_pfv2(u16(d, 5));
          set_pdt( u16(d, 7));
          set_psm( u08(d, 9));
          set_psd( u16(d, 10));
          set_pst( u16(d, 12));
          set_psv( u16(d, 14));
          set_psw( u16(d, 16));
          set_asn( u16(d, 18));
          set_arr( u16(d, 20));
          set_apm( u16(d, 22));
          set_aps( u16(d, 24));
          set_apn( u16(d, 26));
          set_apf( u16(d, 28));
          set_asi( u32(d, 30));
          set_asd( u16(d, 34));
          set_add( u16(d, 36));
          set_adt( u16(d, 38));
          set_arc( u16(d, 40));
          set_ahr( u16(d, 42));
          set_ahd( u16(d, 44));
          set_sfn( u16(d, 46));
          set_sdc( i16(d, 48));
          set_sgt( u16(d, 50));
          set_set( u16(d, 52));
          set_sbm( u08(d, 54));
          set_sbt( u16(d, 55));
          set_ssa( u16(d, 57));
          set_ssi( u32(d, 59));
          set_sds( u16(d, 63));
          set_sam( u08(d, 65));
          set_sax( u16(d, 66));
          set_san( u16(d, 68));
          set_sao( u16(d, 70));
          set_sap( u16(d, 72));
          set_spt( u16(d, 74));
          set_ssp( u16(d, 76));
        }
        break;

      case 0x07020200: // ALAMO Software version 0x07020200 (2020 ALTO #11011 test float)
      case 0x07020300: // ALAMO Software version 0x07020300 (2020 ALTO #11011 test float)
      case 0x07020400: // ALAMO Software version 0x07020400 (2020 ALTO #11011 test float)
      case 0x07020500: // ALAMO Software version 0x07020500 (2020 ALTO #11011 test float)
      case 0x07020600: // ALAMO Software version 0x07020600 (2020 ALTO #11011 test float)
      case 0x07020700: // ALAMO Software version 0x07020700 (2020 ALTO #11011 test float)
      case 0x07020800: // ALAMO Software version 0x07020800 (2020 ALTO #11011 test float)
      case 0x07020900: // ALAMO Software version 0x07020900 (2020 ALTO #11011 test float)
      case 0x07030000: // ALAMO Software version 0x07020900 (2020 ALTO RBR/ODO #11060 test float)
      case 0x07030500: // ALAMO Software version 0x07020900 (2020 ALTO RBR/ODO #11060 test float)

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 65));  // OK
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 98));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 249));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 304));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));
          set_pfv1(u16(d, 324+1));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 76;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_pfv2(u16(d,  3));
          set_pdt( u16(d,  5));
          set_psm( u08(d,  7));
          set_psd( u16(d,  8));
          set_pst( u16(d, 10));
          set_psv( u16(d, 12));
          set_psw( u16(d, 14));
          set_pss( u08(d, 16));
          set_asn( u16(d, 17));
          set_arr( u16(d, 19));
          set_apm( u16(d, 21));
          set_aps( u16(d, 23));
          set_apn( u16(d, 25));
          set_apf( u16(d, 19+8));
          set_asi( u32(d, 21+8));
          set_asd( u16(d, 25+8));
          set_add( u16(d, 27+8));
          set_adt( u16(d, 29+8));
          set_arc( u16(d, 31+8));
          set_ahr( u16(d, 33+8));
          set_ahd( u16(d, 35+8));
          set_sfn( u16(d, 37+8));
          set_sdc( i16(d, 39+8));
          set_sgt( u16(d, 41+8));
          set_set( u16(d, 43+8));
          set_sbm( u08(d, 45+8));
          set_sbt( u16(d, 46+8));
          set_ssa( u16(d, 55+1));
          set_ssi( u32(d, 57+1));
          set_sds( u16(d, 61+1));
          set_sam( u08(d, 63+1));
          set_sax( u16(d, 64+1));
          set_san( u16(d, 66+1));
          set_sao( u16(d, 68+1));
          set_spt( u16(d, 70+1));
          set_ssp( u16(d, 72+1));
        }
        break;

      case 0x08000000: // ALAMO Software version 0x08000000
      case 0x08000001: // ALAMO Software version 0x08000001
      case 0x08000100: // ALAMO Software version 0x08000100
      case 0x08000101: // ALAMO Software version 0x08000101 (2020 Hurricane floats)
      case 0x08000200: // ALAMO Software version 0x08000200
      case 0x08000300: // ALAMO Software version 0x08000300
      case 0x08000400: // ALAMO Software version 0x08000400
      case 0x08000500: // ALAMO Software version 0x08000500
      case 0x08010001: // ALAMO Software version 0x08010001 (2021 Hurricane floats)
      case 0x0A020201:


        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 65));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 98));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 249));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 304));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));
          set_pfv1(u16(d, 324+1));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 78;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_pfv2(u16(d,  3));
          set_pdt( u16(d,  5));
          set_psm( u08(d,  7));
          set_psd( u16(d,  8));
          set_pst( u16(d, 10));
          set_psv( u16(d, 12));
          set_psw( u16(d, 14));
          set_pss( u08(d, 16));
          set_asn( u16(d, 17));
          set_arr( u16(d, 19));
          set_apm( u16(d, 21));
          set_aps( u16(d, 23));
          set_apn( u16(d, 25));
          set_apf( u16(d, 19+8));
          set_asi( u32(d, 21+8));
          set_asd( u16(d, 25+8));
          set_add( u16(d, 27+8));
          set_adt( u16(d, 29+8));
          set_arc( u16(d, 31+8));
          set_ahr( u16(d, 33+8));
          set_ahd( u16(d, 35+8));
          set_sfn( u16(d, 37+8));
          set_sdc( i16(d, 39+8));
          set_sgt( u16(d, 41+8));
          set_set( u16(d, 43+8));
          set_sbm( u08(d, 45+8));
          set_sbt( u16(d, 46+8));
          set_ssa( u16(d, 55+1));
          set_ssi( u32(d, 57+1));
          set_sds( u16(d, 61+1));
          set_sam( u08(d, 63+1));
          set_sax( u16(d, 64+1));
          set_san( u16(d, 66+1));
          set_sao( u16(d, 68+1));
          set_spt( u16(d, 70+1));
          set_ssp( u16(d, 72+1));
          set_wmt( u16(d, 75));
        }
        break;

      case 0x08010000: // ALAMO Software version 0x08010000
      case 0x08020000: // ALAMO Software version 0x08020000
      case 0x09000000: // ALAMO Software version 0x09000000
      case 0x0A000000: // ALAMO Software version 0x0A000000
      case 0x0A000100: // ALAMO Software version 0x0A000100
      case 0x0A020000: // ALAMO Software version 10.2.0
      case 0x0A020100: // ALAMO Software version 10.2.1
      case 0x0A020200: // ALAMO Software version 10.2.2
      case 0x0A030000: // ALAMO Software version 10.3.0
      case 0x0A030100: // ALAMO Software version 10.3.1

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 65));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 98));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 249));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 304));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));
          set_pfv1(u16(d, 324+1));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 76;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_pfv2(u16(d,  3));
          set_pdt( u16(d,  5));
          set_psm( u08(d,  7));
          set_psd( u16(d,  8));
          set_pst( u16(d, 10));
          set_psv( u16(d, 12));
          set_psw( u16(d, 14));
          set_pss( u08(d, 16));
          set_asn( u16(d, 17));
          set_arr( u16(d, 19));
          set_apm( u16(d, 21));
          set_aps( u16(d, 23));
          set_apn( u16(d, 25));
          set_apf( u16(d, 19+8));
          set_asi( u32(d, 21+8));
          set_asd( u16(d, 25+8));
          set_add( u16(d, 27+8));
          set_adt( u16(d, 29+8));
          set_arc( u16(d, 31+8));
          set_ahr( u16(d, 33+8));
          set_ahd( u16(d, 35+8));
          set_sfn( u16(d, 37+8));
          set_sdc( i16(d, 39+8));
          set_sgt( u16(d, 41+8));
          set_set( u16(d, 43+8));
          set_sbm( u08(d, 45+8));
          set_sbt( u16(d, 46+8));
          set_ssa( u16(d, 55+1));
          set_ssi( u32(d, 57+1));
          set_sds( u16(d, 61+1));
          set_sam( u08(d, 63+1));
          set_sax( u16(d, 64+1));
          set_san( u16(d, 66+1));
          set_sao( u16(d, 68+1));
          set_spt( u16(d, 70+1));
          set_ssp( u16(d, 72+1));
        }
        break;

      case 0x0A050000: // ALAMO Software version 10.5.0.0
      case 0x0A050001: // ALAMO Software version 10.5.0.1
      case 0x0A050002: // ALAMO Software version 10.5.0.2
      case 0x0A050003: // ALAMO Software version 10.5.0.3
      case 0x0A050004: // ALAMO Software version 10.5.0.4
      case 0x0A060000: // ALAMO Software version 10.6.0.0
      case 0x0A060001: // ALAMO Software version 10.6.0.1
      case 0x0A060002: // ALAMO Software version 10.6.0.2
      case 0x0A060003: // ALAMO Software version 10.6.0.3
      case 0x0A060004: // ALAMO Software version 10.6.0.4
      case 0x0A060100: // ALAMO Software version 10.6.1.0
      case 0x0A060101: // ALAMO Software version 10.6.1.1
      case 0x0A060102: // ALAMO Software version 10.6.1.2
      case 0x0A060103: // ALAMO Software version 10.6.1.3
      case 0x0A060104: // ALAMO Software version 10.6.1.4
      case 0x0A060133: // ALAMO Software version 10.6.1.33 Micro-ALTO 10.6.1


        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 65));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 98));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 249));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 304));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));
          set_pfv1(u16(d, 324+1));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 93;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_pfv2(u16(d,  3));
          set_pdt( u16(d,  5));
          set_psm( u08(d,  7));
          set_psd( u16(d,  8));
          set_pst( u16(d, 10));
          set_psv( u16(d, 12));
          set_psw( u16(d, 14));
          set_pss( u08(d, 16));
          set_asn( u16(d, 17));
          set_arr( u16(d, 19));
          set_apm( u16(d, 21));
          set_aps( u16(d, 23));
          set_apn( u16(d, 25));
          set_apf( u16(d, 19+8));
          set_asi( u32(d, 21+8));
          set_asd( u16(d, 25+8));
          set_add( u16(d, 27+8));
          set_adt( u16(d, 29+8));
          set_arc( u16(d, 31+8));
          set_ahr( u16(d, 33+8));
          set_ahd( u16(d, 35+8));
          set_sfn( u16(d, 37+8));
          set_sdc( i16(d, 39+8));
          set_sgt( u16(d, 41+8));
          set_set( u16(d, 43+8));
          set_sbm( u08(d, 45+8));
          set_sbt( u16(d, 46+8));
          set_ssa( u16(d, 55+1));
          set_ssi( u32(d, 57+1));
          set_sds( u16(d, 61+1));
          set_sam( u08(d, 63+1));
          set_sax( u16(d, 64+1));
          set_san( u16(d, 66+1));
          set_sao( u16(d, 68+1));
          set_spt( u16(d, 70+1));
          set_ssp( u16(d, 72+1));
          set_ppc_tau(f32(d, 75));
          set_ppc_alpha(f32(d, 79));
          set_ppc_tdelay(f32(d, 83));
          set_ppc_ctccoeff(f32(d, 87));
          set_ppe(u08(d, 91));
        }
        break;

      case 0x0A060231: // ALAMO Software version 10.6.2.31 (WIMS)

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 65));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 98));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 249));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 304));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));
          set_pfv1(u16(d, 324+1));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 95;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_pfv2(u16(d,  3));
          set_pdt( u16(d,  5));
          set_psm( u08(d,  7));
          set_psd( u16(d,  8));
          set_pst( u16(d, 10));
          set_psv( u16(d, 12));
          set_psw( u16(d, 14));
          set_pss( u08(d, 16));
          set_asn( u16(d, 17));
          set_arr( u16(d, 19));
          set_apm( u16(d, 21));
          set_aps( u16(d, 23));
          set_apn( u16(d, 25));
          set_apf( u16(d, 19+8));
          set_asi( u32(d, 21+8));
          set_asd( u16(d, 25+8));
          set_add( u16(d, 27+8));
          set_adt( u16(d, 29+8));
          set_arc( u16(d, 31+8));
          set_ahr( u16(d, 33+8));
          set_ahd( u16(d, 35+8));
          set_sfn( u16(d, 37+8));
          set_sdc( i16(d, 39+8));
          set_sgt( u16(d, 41+8));
          set_set( u16(d, 43+8));
          set_sbm( u08(d, 45+8));
          set_sbt( u16(d, 46+8));
          set_ssa( u16(d, 55+1));
          set_ssi( u32(d, 57+1));
          set_sds( u16(d, 61+1));
          set_sam( u08(d, 63+1));
          set_sax( u16(d, 64+1));
          set_san( u16(d, 66+1));
          set_sao( u16(d, 68+1));
          set_spt( u16(d, 70+1));
          set_ssp( u16(d, 72+1));
          set_ppc_tau(f32(d, 75));
          set_ppc_alpha(f32(d, 79));
          set_ppc_tdelay(f32(d, 83));
          set_ppc_ctccoeff(f32(d, 87));
          set_ppe(u08(d, 91));
          set_wmt(u16(d, 92));
        }
        break;

      case 0x0A060200: // ALAMO Software version 10.6.2.0
      case 0x0A060201: // ALAMO Software version 10.6.2.1
      case 0x0A060202: // ALAMO Software version 10.6.2.2
      case 0x0A060203: // ALAMO Software version 10.6.2.3
      case 0x0A060204: // ALAMO Software version 10.6.2.4
      case 0x0A060233: // ALAMO Software version 10.6.2.4
      case 0x0A060300: // ALAMO Software version 10.6.2.0
      case 0x0A060301: // ALAMO Software version 10.6.2.1
      case 0x0A060302: // ALAMO Software version 10.6.2.2
      case 0x0A060303: // ALAMO Software version 10.6.2.3
      case 0x0A060304: // ALAMO Software version 10.6.2.4
      case 0x0A060333: // ALAMO Software version 10.6.2.4

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 328;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 65));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
          set_pnb( u16(d, 93+1));
          set_pnc( u16(d, 95+1));
          set_pxc( u16(d, 98));
          set_pbl( u16(d, 99+1));
          set_pbh( u16(d, 101+1));
          set_pp0( u16(d, 103+1));
          set_pp1( u16(d, 105+1));
          set_pvl( u16(d, 107+1));
          set_pvh( u16(d, 109+1));
          set_pv0( u16(d, 111+1));
          set_pv5( u16(d, 113+1));
          set_pc0( u16(d, 115+1));
          set_pc1( u16(d, 117+1));

          /* Ice Configuration */

          set_icm( u08(d, 119+1));
          set_imm( u16(d, 120+1));
          set_ica0(u16(d, 122+1));
          set_ica1(u16(d, 124+1));
          set_icb0(u16(d, 126+1));
          set_icb1(u16(d, 128+1));
          set_imt0(u16(d, 130+1));
          set_imt1(u16(d, 132+1));
          set_ilt0(u16(d, 134+1));
          set_ilt1(u16(d, 136+1));
          set_icn( u16(d, 138+1));
          set_icx( u16(d, 140+1));
          set_ict( i16(d, 142+1));
          set_ics( u16(d, 144+1));
          set_iss0(u08(d, 146+1));
          set_iss1(u08(d, 147+1));
          set_ism( u08(d, 148+1));
          set_ist( u16(d, 149+1));
          set_iso( u16(d, 151+1));
          set_isp( u08(d, 153+1));
          set_iue0(u08(d, 154+1));
          set_iue1(u08(d, 155+1));
          set_icd0(u16(d, 156+1));
          set_icd1(u16(d, 158+1));
          set_icr0(u32(d, 160+1));
          set_icr1(u32(d, 164+1));
          set_icg0(u08(d, 168+1));
          set_icg1(u08(d, 169+1));
          set_isa( u08(d, 171+1));
          set_isd( u16(d, 171+1));
          set_iat( u16(d, 173+1));
          set_isv( u16(d, 175+1));
          set_isw( u16(d, 177+1));
          set_ico0(u16(d, 179+1));
          set_ico1(u16(d, 181+1));
          set_icf( u32(d, 183+1));
          set_icw( u32(d, 187+1));

          /* Descent state configuration: */

          set_dns( u16(d, 191+1));
          set_dra( u16(d, 193+1));
          set_dsd0(u16(d, 195+1));
          set_dsd1(u16(d, 197+1));
          set_dsd2(u16(d, 199+1));
          set_dst0(u16(d, 201+1));
          set_dst1(u16(d, 203+1));
          set_dst2(u16(d, 205+1));
          set_ddf( u16(d, 207+1));
          set_ddc( u16(d, 209+1));
          set_dsi( u32(d, 211+1));
          set_ddd( u16(d, 215+1));
          set_ddt( u16(d, 217+1));
          set_dft0(u16(d, 219+1));
          set_dft1(u16(d, 221+1));
          set_dft2(u16(d, 223+1));
          set_dcm( u08(d, 225+1));
          set_dal( u08(d, 226+1));
          set_dct0(u16(d, 227+1));
          set_dct1(u16(d, 229+1));
          set_dct2(u16(d, 231+1));
          set_dca( u08(d, 233+1));
          set_drc( u16(d, 234+1));
          set_drt( u16(d, 236+1));
          set_dcx( u16(d, 238+1));
          set_dcp( u16(d, 240+1));
          set_dcf( u16(d, 242+1));
          set_dcd( u16(d, 244+1));
          set_dcr( u16(d, 246+1));
          set_dsw( u16(d, 249));
          set_duc( u16(d, 250+1));
          set_dpt( u16(d, 252+1));
          set_dut( u16(d, 254+1));
          set_dpd( u16(d, 256+1));
          set_dvt( u16(d, 258+1));
          set_dvd( u16(d, 260+1));
          set_dvw( u16(d, 262+1));
          set_pfr( u16(d, 264+1));
          set_pfc( u16(d, 266+1));
          set_ddp( u16(d, 268+1));
          set_dds( u16(d, 270+1));
          set_pns( u16(d, 272+1));
          set_pto0(u32(d, 274+1));
          set_pto1(u32(d, 278+1));
          set_pto2(u32(d, 282+1));
          set_pth0(u32(d, 286+1));
          set_pth1(u32(d, 290+1));
          set_pth2(u32(d, 294+1));
          set_ptd( u32(d, 298+1));
          set_psa( u08(d, 302+1));
          set_psi( u32(d, 304));
          set_pfm0(u08(d, 307+1));
          set_pfm1(u08(d, 308+1));
          set_pfm2(u08(d, 309+1));
          set_pft0(u16(d, 310+1));
          set_pft1(u16(d, 312+1));
          set_pft2(u16(d, 314+1));
          set_pfd0(u16(d, 316+1));
          set_pfd1(u16(d, 318+1));
          set_pfd2(u16(d, 320+1));
          set_pfv0(u16(d, 322+1));
          set_pfv1(u16(d, 324+1));

        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 93;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_pfv2(u16(d,  3));
          set_pdt( u16(d,  5));
          set_psm( u08(d,  7));
          set_psd( u16(d,  8));
          set_pst( u16(d, 10));
          set_psv( u16(d, 12));
          set_psw( u16(d, 14));
          set_pss( u08(d, 16));
          set_asn( u16(d, 17));
          set_arr( u16(d, 19));
          set_apm( u16(d, 21));
          set_aps( u16(d, 23));
          set_apn( u16(d, 25));
          set_apf( u16(d, 19+8));
          set_asi( u32(d, 21+8));
          set_asd( u16(d, 25+8));
          set_add( u16(d, 27+8));
          set_adt( u16(d, 29+8));
          set_arc( u16(d, 31+8));
          set_ahr( u16(d, 33+8));
          set_ahd( u16(d, 35+8));
          set_sfn( u16(d, 37+8));
          set_sdc( i16(d, 39+8));
          set_sgt( u16(d, 41+8));
          set_set( u16(d, 43+8));
          set_sbm( u08(d, 45+8));
          set_sbt( u16(d, 46+8));
          set_ssa( u16(d, 55+1));
          set_ssi( u32(d, 57+1));
          set_sds( u16(d, 61+1));
          set_sam( u08(d, 63+1));
          set_sax( u16(d, 64+1));
          set_san( u16(d, 66+1));
          set_sao( u16(d, 68+1));
          set_spt( u16(d, 70+1));
          set_ssp( u16(d, 72+1));
          set_ppc_tau(f32(d, 75));
          set_ppc_alpha(f32(d, 79));
          set_ppc_tdelay(f32(d, 83));
          set_ppc_ctccoeff(f32(d, 87));
          set_ppe(u08(d, 91));
        }
        break;

      case 0x0A070101: // ALAMO Software version 10.7.1.1

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 327;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 65));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
//          set_pnb( u16(d, 93+1));
//          set_pnc( u16(d, 95+1));
//          set_pxc( u16(d, 98));
          set_pbl( u16(d, 99-5));
          set_pbh( u16(d, 101-5));
          set_pp0( u16(d, 103-5));
          set_pp1( u16(d, 105-5));
          set_pvl( u16(d, 107-5));
          set_pvh( u16(d, 109-5));
          set_pv0( u16(d, 111-5));
          set_pv5( u16(d, 113-5));
          set_pc0( u16(d, 115-5));
          set_pc1( u16(d, 117-5));

          /* Ice Configuration */

          set_icm( u08(d, 119-5));
          set_imm( u16(d, 120-5));
          set_ica0(u16(d, 122-5));
          set_ica1(u16(d, 124-5));
          set_icb0(u16(d, 126-5));
          set_icb1(u16(d, 128-5));
          set_imt0(u16(d, 130-5));
          set_imt1(u16(d, 132-5));
          set_ilt0(u16(d, 134-5));
          set_ilt1(u16(d, 136-5));
          set_icn( u16(d, 138-5));
          set_icx( u16(d, 140-5));
          set_ict( i16(d, 142-5));
          set_ics( u16(d, 144-5));
          set_iss0(u08(d, 146-5));
          set_iss1(u08(d, 147-5));
          set_ism( u08(d, 148-5));
          set_ist( u16(d, 149-5));
          set_iso( u16(d, 151-5));
          set_isp( u08(d, 153-5));
          set_iue0(u08(d, 154-5));
          set_iue1(u08(d, 155-5));
          set_icd0(u16(d, 156-5));
          set_icd1(u16(d, 158-5));
          set_icr0(u32(d, 160-5));
          set_icr1(u32(d, 164-5));
          set_icg0(u08(d, 168-5));
          set_icg1(u08(d, 169-5));
          set_isa( u08(d, 171-5));
          set_isd( u16(d, 171-5));
          set_iat( u16(d, 173-5));
          set_isv( u16(d, 175-5));
          set_isw( u16(d, 177-5));
          set_ico0(u16(d, 179-5));
          set_ico1(u16(d, 181-5));
          set_icf( u32(d, 183-5));
          set_icw( u32(d, 187-5));

          /* Descent state configuration: */

          set_dns( u16(d, 191-5));
          set_dra( u16(d, 193-5));
          set_dsd0(u16(d, 195-5));
          set_dsd1(u16(d, 197-5));
          set_dsd2(u16(d, 199-5));
          set_dst0(u16(d, 201-5));
          set_dst1(u16(d, 203-5));
          set_dst2(u16(d, 205-5));
          set_ddf( u16(d, 207-5));
          set_ddc( u16(d, 209-5));
          set_dsi( u32(d, 211-5));
          set_ddd( u16(d, 215-5));
          set_ddt( u16(d, 217-5));
          set_dft0(u16(d, 219-5));
          set_dft1(u16(d, 221-5));
          set_dft2(u16(d, 223-5));
          set_dcm( u08(d, 225-5));
          set_dal( u08(d, 226-5));
          set_dct0(u16(d, 227-5));
          set_dct1(u16(d, 229-5));
          set_dct2(u16(d, 231-5));
          set_dca( u08(d, 233-5));
          set_drc( u16(d, 234-5));
          set_drt( u16(d, 236-5));
          set_dcx( u16(d, 238-5));
          set_dcp( u16(d, 240-5));
          set_dcf( u16(d, 242-5));
          set_dcd( u16(d, 244-5));
          set_dcr( u16(d, 246-5));
          set_dsw( u16(d, 243));
          set_duc( u16(d, 250-5));
          set_dpt( u16(d, 252-5));
          set_dut( u16(d, 254-5));
          set_dpd( u16(d, 256-5));
          set_dvt( u16(d, 258-5));
          set_dvd( u16(d, 260-5));
          set_dvw( u16(d, 262-5));
          set_pfr( u16(d, 264-5));
          set_pfc( u16(d, 266-5));
          set_ddp( u16(d, 268-5));
          set_dds( u16(d, 270-5));
          set_pns( u16(d, 272-5));
          set_pto0(u32(d, 274-5));
          set_pto1(u32(d, 278-5));
          set_pto2(u32(d, 282-5));
          set_pth0(u32(d, 286-5));
          set_pth1(u32(d, 290-5));
          set_pth2(u32(d, 294-5));
          set_ptd( u32(d, 298-5));
          set_psa( u08(d, 302-5));
          set_psi( u32(d, 298));
          set_pfm0(u08(d, 307-5));
          set_pfm1(u08(d, 308-5));
          set_pfm2(u08(d, 309-5));
          set_pft0(u16(d, 310-5));
          set_pft1(u16(d, 312-5));
          set_pft2(u16(d, 314-5));
          set_pfd0(u16(d, 316-5));
          set_pfd1(u16(d, 318-5));
          set_pfd2(u16(d, 320-5));
          set_pfv0(u16(d, 322-5));
          set_pfv1(u16(d, 324-5));
          set_pfv2(u16(d, 321));
          set_pdt( u16(d, 323));
          set_psm( u08(d, 325));


        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 88;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          /* parameter index - 323 */

//          set_pfv2(u16(d,  3));
//          set_pdt( u16(d,  5));
//          set_psm( u08(d,  7));
          set_psd( u16(d,           3));
          set_pst( u16(d,           5));
          set_psv( u16(d,           7));
          set_psw( u16(d,           9));
          set_pss( u08(d,           11));
          set_asn( u16(d,           12));
          set_arr( u16(d,           14));
          set_apm( u16(d,           16));
          set_aps( u16(d,           18));
          set_apn( u16(d,           20));
          set_apf( u16(d,           22));
          set_asi( u32(d,           24));
          set_asd( u16(d,           28));
          set_add( u16(d,           30));
          set_adt( u16(d,           32));
          set_arc( u16(d,           34));
          set_ahr( u16(d,           36));
          set_ahd( u16(d,           38));
          set_sfn( u16(d,           40));
          set_sdc( i16(d,           42));
          set_sgt( u16(d,           44));
          set_set( u16(d,           46));
          set_sbm( u08(d,           48));
          set_sbt( u16(d,           49));
          set_ssa( u16(d,           51));
          set_ssi( u32(d,           53));
          set_sds( u16(d,           57));
          set_sam( u08(d,           59));
          set_sax( u16(d,           60));
          set_san( u16(d,           62));
          set_sao( u16(d,           64));
          set_spt( u16(d,           66));
          set_ssp( u16(d,           68));
//          set_wmt(u16(d,            70));
//          set_ppc_tau(f32(d,        72));
//          set_ppc_alpha(f32(d,      76));
//          set_ppc_tdelay(f32(d,     80));
//          set_ppc_ctccoeff(f32(d,   84));
//          set_ppe(u08(d,            88));

        }
        break;


      case 0x0A080004: // ALAMO Software version 10.8.0.4 (SBE)
      case 0x0A0C0001: // ALAMO Software version 10.12.0.1 (RBR)
      case 0x0A0C0032: // ALAMO/MM2 Software version 10.12.0.32

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 327;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 65));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
//          set_pnb( u16(d, 93+1));
//          set_pnc( u16(d, 95+1));
//          set_pxc( u16(d, 98));
          set_pbl( u16(d, 99-5));
          set_pbh( u16(d, 101-5));
          set_pp0( u16(d, 103-5));
          set_pp1( u16(d, 105-5));
          set_pvl( u16(d, 107-5));
          set_pvh( u16(d, 109-5));
          set_pv0( u16(d, 111-5));
          set_pv5( u16(d, 113-5));
          set_pc0( u16(d, 115-5));
          set_pc1( u16(d, 117-5));

          /* Ice Configuration */

          set_icm( u08(d, 119-5));
          set_imm( u16(d, 120-5));
          set_ica0(u16(d, 122-5));
          set_ica1(u16(d, 124-5));
          set_icb0(u16(d, 126-5));
          set_icb1(u16(d, 128-5));
          set_imt0(u16(d, 130-5));
          set_imt1(u16(d, 132-5));
          set_ilt0(u16(d, 134-5));
          set_ilt1(u16(d, 136-5));
          set_icn( u16(d, 138-5));
          set_icx( u16(d, 140-5));
          set_ict( i16(d, 142-5));
          set_ics( u16(d, 144-5));
          set_iss0(u08(d, 146-5));
          set_iss1(u08(d, 147-5));
          set_ism( u08(d, 148-5));
          set_ist( u16(d, 149-5));
          set_iso( u16(d, 151-5));
          set_isp( u08(d, 153-5));
          set_iue0(u08(d, 154-5));
          set_iue1(u08(d, 155-5));
          set_icd0(u16(d, 156-5));
          set_icd1(u16(d, 158-5));
          set_icr0(u32(d, 160-5));
          set_icr1(u32(d, 164-5));
          set_icg0(u08(d, 168-5));
          set_icg1(u08(d, 169-5));
          set_isa( u08(d, 171-5));
          set_isd( u16(d, 171-5));
          set_iat( u16(d, 173-5));
          set_isv( u16(d, 175-5));
          set_isw( u16(d, 177-5));
          set_ico0(u16(d, 179-5));
          set_ico1(u16(d, 181-5));
          set_icf( u32(d, 183-5));
          set_icw( u32(d, 187-5));

          /* Descent state configuration: */

          set_dns( u16(d, 191-5));
          set_dra( u16(d, 193-5));
          set_dsd0(u16(d, 195-5));
          set_dsd1(u16(d, 197-5));
          set_dsd2(u16(d, 199-5));
          set_dst0(u16(d, 201-5));
          set_dst1(u16(d, 203-5));
          set_dst2(u16(d, 205-5));
          set_ddf( u16(d, 207-5));
          set_ddc( u16(d, 209-5));
          set_dsi( u32(d, 211-5));
          set_ddd( u16(d, 215-5));
          set_ddt( u16(d, 217-5));
          set_dft0(u16(d, 219-5));
          set_dft1(u16(d, 221-5));
          set_dft2(u16(d, 223-5));
          set_dcm( u08(d, 225-5));
          set_dal( u08(d, 226-5));
          set_dct0(u16(d, 227-5));
          set_dct1(u16(d, 229-5));
          set_dct2(u16(d, 231-5));
          set_dca( u08(d, 233-5));
          set_drc( u16(d, 234-5));
          set_drt( u16(d, 236-5));
          set_dcx( u16(d, 238-5));
          set_dcp( u16(d, 240-5));
          set_dcf( u16(d, 242-5));
          set_dcd( u16(d, 244-5));
          set_dcr( u16(d, 246-5));
          set_dsw( u16(d, 243));
          set_duc( u16(d, 250-5));
          set_dpt( u16(d, 252-5));
          set_dut( u16(d, 254-5));
          set_dpd( u16(d, 256-5));
          set_dvt( u16(d, 258-5));
          set_dvd( u16(d, 260-5));
          set_dvw( u16(d, 262-5));
          set_pfr( u16(d, 264-5));
          set_pfc( u16(d, 266-5));
          set_ddp( u16(d, 268-5));
          set_dds( u16(d, 270-5));
          set_pns( u16(d, 272-5));
          set_pto0(u32(d, 274-5));
          set_pto1(u32(d, 278-5));
          set_pto2(u32(d, 282-5));
          set_pth0(u32(d, 286-5));
          set_pth1(u32(d, 290-5));
          set_pth2(u32(d, 294-5));
          set_ptd( u32(d, 298-5));
          set_psa( u08(d, 302-5));
          set_psi( u32(d, 298));
          set_pfm0(u08(d, 307-5));
          set_pfm1(u08(d, 308-5));
          set_pfm2(u08(d, 309-5));
          set_pft0(u16(d, 310-5));
          set_pft1(u16(d, 312-5));
          set_pft2(u16(d, 314-5));
          set_pfd0(u16(d, 316-5));
          set_pfd1(u16(d, 318-5));
          set_pfd2(u16(d, 320-5));
          set_pfv0(u16(d, 322-5));
          set_pfv1(u16(d, 324-5));
          set_pfv2(u16(d, 321));
          set_pdt( u16(d, 323));
          set_psm( u08(d, 325));


        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 71;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          /* parameter index - 323 */

//          set_pfv2(u16(d,  3));
//          set_pdt( u16(d,  5));
//          set_psm( u08(d,  7));
          set_psd( u16(d,           3));
          set_pst( u16(d,           5));
          set_psv( u16(d,           7));
          set_psw( u16(d,           9));
          set_pss( u08(d,           11));
          set_asn( u16(d,           12));
          set_arr( u16(d,           14));
          set_apm( u16(d,           16));
          set_aps( u16(d,           18));
          set_apn( u16(d,           20));
          set_apf( u16(d,           22));
          set_asi( u32(d,           24));
          set_asd( u16(d,           28));
          set_add( u16(d,           30));
          set_adt( u16(d,           32));
          set_arc( u16(d,           34));
          set_ahr( u16(d,           36));
          set_ahd( u16(d,           38));
          set_sfn( u16(d,           40));
          set_sdc( i16(d,           42));
          set_sgt( u16(d,           44));
          set_set( u16(d,           46));
          set_sbm( u08(d,           48));
          set_sbt( u16(d,           49));
          set_ssa( u16(d,           51));
          set_ssi( u32(d,           53));
          set_sds( u16(d,           57));
          set_sam( u08(d,           59));
          set_sax( u16(d,           60));
          set_san( u16(d,           62));
          set_sao( u16(d,           64));
          set_spt( u16(d,           66));
          set_ssp( u16(d,           68));
          set_wmt(u16(d,            70));
          set_ppc_tau(f32(d,        72));
          set_ppc_alpha(f32(d,      76));
          set_ppc_tdelay(f32(d,     80));
          set_ppc_ctccoeff(f32(d,   84));
          set_ppe(u08(d,            88));

        }
        break;


      case 0x0A080031: // ALAMO Software version 10.8.0.31 (WIMS)
      case 0x0A0A0031: // ALAMO Software version 10.10.0.31 (WIMS)
      case 0x0A0C0131: // ALAMO Software version 10.12.1.31 (WIMS)

        if(msg_idx == 0) {

          static const unsigned int EXPECTED_SIZE = 327;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));
          set_msn( u16(d, 10));
          set_mps( u08(d, 12));
          set_mpc( u16(d, 13));
          set_mnm0(u16(d, 15));
          set_mnm1(u16(d, 17));
          set_mnm2(u16(d, 19));
          set_mns0(u08(d, 21));
          set_mns1(u08(d, 22));
          set_mns2(u08(d, 23));
          set_dtd0(u16(d, 24));
          set_dtd1(u16(d, 26));
          set_dtd2(u16(d, 28));
          set_mds( u16(d, 30));
          set_apt( u08(d, 32));
          set_btm( u08(d, 33));
          set_ddb( u08(d, 34));
          set_xas( u08(d, 34+1));
          set_cps( u08(d, 35+1));
          set_cst( u08(d, 36+1));
          set_ctf( u16(d, 37+1));
          set_ctl( u16(d, 39+1));
          set_ctu( u16(d, 41+1));
          set_ctc( u08(d, 43+1));
          set_csp( u32(d, 44+1));
          set_cpb1(u16(d, 48+1));
          set_cpb2(u16(d, 50+1));
          set_cpb3(u16(d, 52+1));
          set_cpn1(u16(d, 54+1));
          set_cpn2(u16(d, 56+1));
          set_cpn3(u16(d, 58+1));
          set_cpp1(u16(d, 60+1));
          set_cpp2(u16(d, 62+1));
          set_cpp3(u16(d, 65));
          set_crd( u16(d, 66+1));
          set_crs( u16(d, 68+1));
          set_mtd( u16(d, 70+1));
          set_mtt( u32(d, 72+1));
          set_mts( u32(d, 76+1));
          set_mam( u08(d, 80+1));
          set_mat1(u16(d, 81+1));
          set_mat2(u16(d, 84));
          set_map( u16(d, 86));
          set_aca( u16(d, 87+1));
          set_acg( u16(d, 89+1));
          set_acr( u16(d, 91+1));
//          set_pnb( u16(d, 93+1));
//          set_pnc( u16(d, 95+1));
//          set_pxc( u16(d, 98));
          set_pbl( u16(d, 99-5));
          set_pbh( u16(d, 101-5));
          set_pp0( u16(d, 103-5));
          set_pp1( u16(d, 105-5));
          set_pvl( u16(d, 107-5));
          set_pvh( u16(d, 109-5));
          set_pv0( u16(d, 111-5));
          set_pv5( u16(d, 113-5));
          set_pc0( u16(d, 115-5));
          set_pc1( u16(d, 117-5));

          /* Ice Configuration */

          set_icm( u08(d, 119-5));
          set_imm( u16(d, 120-5));
          set_ica0(u16(d, 122-5));
          set_ica1(u16(d, 124-5));
          set_icb0(u16(d, 126-5));
          set_icb1(u16(d, 128-5));
          set_imt0(u16(d, 130-5));
          set_imt1(u16(d, 132-5));
          set_ilt0(u16(d, 134-5));
          set_ilt1(u16(d, 136-5));
          set_icn( u16(d, 138-5));
          set_icx( u16(d, 140-5));
          set_ict( i16(d, 142-5));
          set_ics( u16(d, 144-5));
          set_iss0(u08(d, 146-5));
          set_iss1(u08(d, 147-5));
          set_ism( u08(d, 148-5));
          set_ist( u16(d, 149-5));
          set_iso( u16(d, 151-5));
          set_isp( u08(d, 153-5));
          set_iue0(u08(d, 154-5));
          set_iue1(u08(d, 155-5));
          set_icd0(u16(d, 156-5));
          set_icd1(u16(d, 158-5));
          set_icr0(u32(d, 160-5));
          set_icr1(u32(d, 164-5));
          set_icg0(u08(d, 168-5));
          set_icg1(u08(d, 169-5));
          set_isa( u08(d, 171-5));
          set_isd( u16(d, 171-5));
          set_iat( u16(d, 173-5));
          set_isv( u16(d, 175-5));
          set_isw( u16(d, 177-5));
          set_ico0(u16(d, 179-5));
          set_ico1(u16(d, 181-5));
          set_icf( u32(d, 183-5));
          set_icw( u32(d, 187-5));

          /* Descent state configuration: */

          set_dns( u16(d, 191-5));
          set_dra( u16(d, 193-5));
          set_dsd0(u16(d, 195-5));
          set_dsd1(u16(d, 197-5));
          set_dsd2(u16(d, 199-5));
          set_dst0(u16(d, 201-5));
          set_dst1(u16(d, 203-5));
          set_dst2(u16(d, 205-5));
          set_ddf( u16(d, 207-5));
          set_ddc( u16(d, 209-5));
          set_dsi( u32(d, 211-5));
          set_ddd( u16(d, 215-5));
          set_ddt( u16(d, 217-5));
          set_dft0(u16(d, 219-5));
          set_dft1(u16(d, 221-5));
          set_dft2(u16(d, 223-5));
          set_dcm( u08(d, 225-5));
          set_dal( u08(d, 226-5));
          set_dct0(u16(d, 227-5));
          set_dct1(u16(d, 229-5));
          set_dct2(u16(d, 231-5));
          set_dca( u08(d, 233-5));
          set_drc( u16(d, 234-5));
          set_drt( u16(d, 236-5));
          set_dcx( u16(d, 238-5));
          set_dcp( u16(d, 240-5));
          set_dcf( u16(d, 242-5));
          set_dcd( u16(d, 244-5));
          set_dcr( u16(d, 246-5));
          set_dsw( u16(d, 243));
          set_duc( u16(d, 250-5));
          set_dpt( u16(d, 252-5));
          set_dut( u16(d, 254-5));
          set_dpd( u16(d, 256-5));
          set_dvt( u16(d, 258-5));
          set_dvd( u16(d, 260-5));
          set_dvw( u16(d, 262-5));
          set_pfr( u16(d, 264-5));
          set_pfc( u16(d, 266-5));
          set_ddp( u16(d, 268-5));
          set_dds( u16(d, 270-5));
          set_pns( u16(d, 272-5));
          set_pto0(u32(d, 274-5));
          set_pto1(u32(d, 278-5));
          set_pto2(u32(d, 282-5));
          set_pth0(u32(d, 286-5));
          set_pth1(u32(d, 290-5));
          set_pth2(u32(d, 294-5));
          set_ptd( u32(d, 298-5));
          set_psa( u08(d, 302-5));
          set_psi( u32(d, 298));
          set_pfm0(u08(d, 307-5));
          set_pfm1(u08(d, 308-5));
          set_pfm2(u08(d, 309-5));
          set_pft0(u16(d, 310-5));
          set_pft1(u16(d, 312-5));
          set_pft2(u16(d, 314-5));
          set_pfd0(u16(d, 316-5));
          set_pfd1(u16(d, 318-5));
          set_pfd2(u16(d, 320-5));
          set_pfv0(u16(d, 322-5));
          set_pfv1(u16(d, 324-5));
          set_pfv2(u16(d, 321));
          set_pdt( u16(d, 323));
          set_psm( u08(d, 325));


        } else if(msg_idx == 1) {

          static const unsigned int EXPECTED_SIZE = 90;

          if(data.size() != EXPECTED_SIZE) {
            ostringstream os;
            os << "Invalid X-Message: Expected "
               << EXPECTED_SIZE
               << " bytes, but received "
               << data.size()
               << " bytes!";
#           if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
            lwrn() << os.str();
#           else
            EXC_THROW(os.str());
#           endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          }

          /* parameter index - 323 */

//          set_pfv2(u16(d,  3));
//          set_pdt( u16(d,  5));
//          set_psm( u08(d,  7));
          set_psd( u16(d,           3));
          set_pst( u16(d,           5));
          set_psv( u16(d,           7));
          set_psw( u16(d,           9));
          set_pss( u08(d,           11));
          set_asn( u16(d,           12));
          set_arr( u16(d,           14));
          set_apm( u16(d,           16));
          set_aps( u16(d,           18));
          set_apn( u16(d,           20));
          set_apf( u16(d,           22));
          set_asi( u32(d,           24));
          set_asd( u16(d,           28));
          set_add( u16(d,           30));
          set_adt( u16(d,           32));
          set_arc( u16(d,           34));
          set_ahr( u16(d,           36));
          set_ahd( u16(d,           38));
          set_sfn( u16(d,           40));
          set_sdc( i16(d,           42));
          set_sgt( u16(d,           44));
          set_set( u16(d,           46));
          set_sbm( u08(d,           48));
          set_sbt( u16(d,           49));
          set_ssa( u16(d,           51));
          set_ssi( u32(d,           53));
          set_sds( u16(d,           57));
          set_sam( u08(d,           59));
          set_sax( u16(d,           60));
          set_san( u16(d,           62));
          set_sao( u16(d,           64));
          set_spt( u16(d,           66));
          set_ssp( u16(d,           68));
          set_wmt(u16(d,            70));
          set_ppc_tau(f32(d,        72));
          set_ppc_alpha(f32(d,      76));
          set_ppc_tdelay(f32(d,     80));
          set_ppc_ctccoeff(f32(d,   84));
          set_ppe(u08(d,            88));

        }
        break;




      default: { // Unsupported ALAMO software version.


        if(msg_idx == 0) {

          set_msg_version(u08(d, 8));
          set_sensor_type(u08(d, 9));

        }

        ostringstream os;
        os << "Unsupported ALAMO Mission Parameters message version: 0x"
           << hex
           << setw(8)
           << setfill('0')
           << fw_version()
           << "!";
#         if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          lwrn() << os.str();
#         else
          EXC_THROW(os.str());
#         endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG


      }

        break;
    }

#   if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
    ldbg() << "Mission configuration listing string: " << endl << GenerateListingString(0, 0, 0);
#   endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG

  }CATCH_BLOCK(
      "Failed to decode mission configuration message for message index "
          + lexical_cast<string>((int )msg_idx) + "!")

}

/** Generate the mission configuration message listing string.
 *
 *  @param[in] whoi   The instrument identifier.
 *  @param[in] dive   The dive cycle number
 *  @param[in] idx
 *
 *  @return
 */
const string XMfaMissionCfg::GenerateListingString(int whoi, int dive, int idx) const {

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
          static const format fmt3("mis %5d %4d %4d       0x%08X");
          os << format(fmt1) % "SN" % "dive" % "idx" % "SWVer" << endl;
          os << format(fmt3) % whoi % dive % idx % fw_version() << endl;

          // Minor index 0.
          static const format fmt4("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s");
          static const format fmt5("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s");
          static const format fmt6("mis %5d %4d %4d %4d %11d %11d %11d %11d %11d");
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
          static const format fmt9("mis %5d %4d %4d %4d %11d %11.3f");
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
          static const format fmt12("mis %5d %4d %4d %4d %11d %11d %11d %11d %11d");
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
          static const format fmt15("mis %5d %4d %4d %4d %11.2f %11.2f %11.2f");
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
          static const format fmt18("mis %5d %4d %4d %4d %11d %11d %11d %11d %11d %11d");
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
          static const format fmt21("mis %5d %4d %4d %4d %11d %11d %11d %11d");
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
          static const format fmt24("mis %5d %4d %4d %4d %11d %11d %11.3f %11d %11d %11d");
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
                  "mis %5d %4d %4d %4d %11.1f %11.1f %11.1f %11.3f %11.3f %11.3f");
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
          static const format fmt30("mis %5d %4d %4d %4d %11d %11d %11d");
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
          static const format fmt33("mis %5d %4d %4d %4d %11d %11d %11d %11d");
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
          static const format fmt36("mis %5d %4d %4d %4d %11d %11d");
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
          static const format fmt39("mis %5d %4d %4d %4d %11d %11d %11d %11.3f");
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
          static const format fmt42("mis %5d %4d %4d %4d %11d %11d %11d %11d %11d %11d");
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
          static const format fmt45("mis %5d %4d %4d %4d %11d %11d %11d %11d %11d %11d");
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
          static const format fmt48("mis %5d %4d %4d %4d %11d %11d  0x%08X %11d %11.3f %11d");
          os << "# --- Drift/Park - General -------------------------------------------------"
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
          static const format fmt51("mis %5d %4d %4d %4d %11d %11d");
          os << format(fmt49) % "SN" % "dive" % "idx" % "midx" % "PFD" % "PDT" << endl;
          os << format(fmt50) % "" % "" % "" % "" % "(dBar)" % "(sec)" << endl;
          os << format(fmt51) % whoi % dive % idx % 15 % pfd() % pdt() << endl;

          // Minor index 16.
          static const format fmt52("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s");
          static const format fmt53("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s");
          static const format fmt54("mis %5d %4d %4d %4d %11s %11s %11s %11s %11s");
          os << "# --- Drift/Park - Seek ----------------------------------------------------"
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
          static const format fmt57("mis %5d %4d %4d %4d %11d %11.2f %11d %11d %11d %11.3f");
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
                     % asn() % arr_mps() % apm() % apn() % apf() % asi_sec()
             << endl;

          // Minor index 18.
          static const format fmt58("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
          static const format fmt59("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
          static const format fmt60("mis %5d %4d %4d %4d %11d %11d %11d %11d %11.3f %11d");
          os << format(fmt58) % "SN" % "dive" % "idx" % "midx" % "ASD" % "ADD" % "ADT" % "ARC"
                     % "AHR" % "AHD"
             << endl;
          os << format(fmt59) % "" % "" % "" % "" % "(dBar)" % "(dBar)" % "(sec)" % "(?)"
                     % "(m/sec)" % "(dBar)"
             << endl;
          os << format(fmt60) % whoi % dive % idx % midx++ // midx
                     % asd() % add() % adt() % arc() % ahr_mps() % ahd()
             << endl;

          // Minor index 19.
          static const format fmt61("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
          static const format fmt62("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s %11s");
          static const format fmt63("mis %5d %4d %4d %4d %11d %11d %11d %11d %11d %11.3f");
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
          static const format fmt66("mis %5d %4d %4d %4d %11d %11d");

          os << format(fmt64) % "SN" % "dive" % "idx" % "midx" % "SDS" % "SPT" << endl;
          os << format(fmt65) % "" % "" % "" % "" % "(sec)" % "(sec)" << endl;
          os << format(fmt66) % whoi % dive % idx % midx++ // midx
                     % sds() % spt()
             << endl;

          // Minor index 21.
          static const format fmt67("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s");
          static const format fmt68("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s");
          static const format fmt69("mis %5d %4d %4d %4d %11d %11.3f %11.3f %11d %11d");
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
          static const format fmt72("mis %5d %4d %4d %4d %11d %11d");
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
              static const format fmt3("mis %5d %4d %4d       0x%08X");
              os << format(fmt1) % "SN" % "dive" % "idx" % "SWVer" << endl;
              os << format(fmt3) % whoi % dive % idx % fw_version() << endl;
          }

          static const format fmt4("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s");
          static const format fmt5("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s");
          static const format fmt6("mis %5d %4d %4d %4d %11d %11d %11d %11d %11d");
          os << format(fmt4) % "SN" % "dive" % "idx" % "midx" % "MSN" % "DTD" % "MDS" % "APT"
                     % "XAS"
             << endl;
          os << format(fmt5) % "" % "" % "" % "" % "" % "(dBar)" % "(dBar)" % "" % "" << endl;
          os << format(fmt6) % whoi % dive % idx % midx++ // midx
                     % msn() % dtd() % mds() % (int) apt() % (int) xas()
             << endl;

          static const format fmt7("#   %5s %4s %4s %4s %11s %11s");
          static const format fmt8("#    %5s %4s %4s %4s %11s %11s");
          static const format fmt9("mis %5d %4d %4d %4d %11d %11.3f");
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
          static const format fmt30("mis %5d %4d %4d %4d %11d %11d %11d");
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
          static const format fmt15("mis %5d %4d %4d %4d %11.2f %11.2f %11.2f");
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
          static const format fmt18("mis %5d %4d %4d %4d %11d %11d %11d %11d %11d %11d");
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
          static const format fmt21("mis %5d %4d %4d %4d %11d %11d %11d %11d");
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
          static const format fmt24("mis %5d %4d %4d %4d %11d %11d %11.3f %11d %11d %11d");
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
                  "mis %5d %4d %4d %4d %11.1f %11.1f %11.1f %11.3f %11.3f %11.3f");
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
          static const format fmt75("mis %5d %4d %4d %4d "
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
          static const format fmt33("mis %5d %4d %4d %4d %11d %11d %11d %11d");
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
          static const format fmt36("mis %5d %4d %4d %4d %11d %11d");
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
          static const format fmt39("mis %5d %4d %4d %4d %11d %11d %11d %11.3f");
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
          static const format fmt42("mis %5d %4d %4d %4d %11d %11d %11d %11d %11d %11d");
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
          static const format fmt45("mis %5d %4d %4d %4d %11d %11d %11d %11d %11d %11d");
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
          static const format fmt48("mis %5d %4d %4d %4d %11d %11d  0x%08X %11d %11.3f %11d");
          os << "# --- Drift/Park - General -------------------------------------------------"
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
          static const format fmt51("mis %5d %4d %4d %4d %11d %11d");
          os << format(fmt49) % "SN" % "dive" % "idx" % "midx" % "PFD" % "PDT" << endl;
          os << format(fmt50) % "" % "" % "" % "" % "(dBar)" % "(sec)" << endl;
          os << format(fmt51) % whoi % dive % idx % midx++ // midx
                     % pfd() % pdt()
             << endl;

          static const format fmt52("#   %5s %4s %4s %4s %11s %11s %11s %11s %11s");
          static const format fmt53("#    %5s %4s %4s %4s %11s %11s %11s %11s %11s");
          static const format fmt54("mis %5d %4d %4d %4d %11s %11s %11s %11s %11s");
          os << "# --- Drift/Park - Seek ----------------------------------------------------"
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
          static const format fmt57("mis %5d %4d %4d %4d %11d %11.2f %11d %11d %11d %11.3f");
          os << "# --- Ascent/Profile -------------------------------------------------------"
             "------------------------"
             << endl << format(fmt55) % "SN" % "dive" % "idx" % "midx" % "ASN" % "ARR" % "APM"
                     % "APN" % "APF" % "ASI"
             << endl << format(fmt56) % "" % "" % "" % "" % "" % "(m/sec)" % "(sec)" % "(sec)"
                     % "(sec)" % "(sec)"
             << endl << format(fmt57) % whoi % dive % idx % midx++  // midx
                     % asn() % arr_mps() % apm() % apn() % apf() % asi_sec()
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
              static const format fmt3("mis %5d %4d %4d %4d %11d "
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
                         % ahr_mps() % ahd()
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
              static const format fmt3("mis %5d %4d %4d %4d "
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
              static const format fmt3("mis %5d %4d %4d %4d "
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
              static const format fmt3("mis %5d %4d %4d %4d "
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
              static const format fmt3("mis %5d %4d %4d %4d "
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
              static const format fmt3("mis %5d %4d %4d %4d "
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
              static const format fmt3("mis %5d %4d %4d %4d "
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
              static const format fmt3("mis %5d %4d %4d %4d "
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
              static const format fmt3("mis %5d %4d %4d %4d "
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
              static const format fmt3("mis %5d %4d %4d %4d "
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
              static const format fmt3("mis %5d %4d %4d %4d "
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
              static const format fmt3("mis %5d %4d %4d %4d "
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

      case 0x04026200: // ALAMO Software version 0x04036100 (2017 PAR floats)
      case 0x04036100: // ALAMO Software version 0x04036100 (2017 S2h test float 10108).
      case 0x04046100: // ALAMO Software version 0x04046100 (2017 Hurricane Floats).
      case 0x04066100: // ALAMO Software version 0x04066100 (2017 S2h test float 10108).

      {   int stype;
          string stype_str;
          switch(stype = sensor_type()) {
          case 0:     stype_str = "RBR";                          break;
          case 1:     stype_str = "SBE";                          break;
          default:    stype_str = "unknown"; stype = -99;         break; }
          {   // Header line.
              static const format fmt1("#   %5s %4s %4s %16s %11s %11s        %s");
              static const format fmt3("MIS %5d %4d %4d             0x%02X "                                     " 0x%08X "      "%11d "         "     # %s"          );
              static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "idx"     % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
              static const string str2("#");
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % idx       % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
          }
          os << "# --- General --------------------------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s"          "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s"          "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d"          "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "MSN"         % "MDS"         % "APT"         % "XAS"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(dBar)"      % ""            % ""            ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % msn()         % mds()         % apt()         %  xas()        << endl;
          }
          os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11.3f"        );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "MTD"         % "MTT"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % mtd()         % mtt_sec()     << endl;
          }
          os << "# --- Diagnostic Dive ------------------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "DDP"         % "DDS"         % "DDF"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(sec)"       % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % ddp()         % dds()         % ddf()         << endl;
          }
          os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11.2f "       "%11.2f "       "%11.2f"        );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PNB"         % "PNC"         % "PXC"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(V)"         % "(psi)"       % "(psi)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % pnb_v()       % pnc_psi()     % pxc_psi()     << endl;
          }
          os << "# --- Calibration Coefficients ---------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "        "%11s "         "%11s "         "%11s "         "%11s "         "%11s"           );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "        "%11s "         "%11s "         "%11s "         "%11s "         "%11s"           );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "        "%11d "         "%11d "         "%11d "         "%11d "         "%11d"           );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PBL"         % "PBH"         % "PP0"         % "PP1"         % "PVL"         % "PVH"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(LSB/12V)"   % "(LSB/14V)"   % "(LSB/0A)"    % "(LSB/1A)"    % "(LSB/12V)"   % "(LSB/14V)"   ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % pbl()         % pbh()         % pp0()         % pp1()         % pvl()         % pvh()         << endl;
          }
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PV0"         % "PV5"         % "PC0"         % "PC1"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(LSB/0inHg)" % "(LSB/5inHg)" % "(LSB/0A)"    % "(LSB/1A)"    ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % pv0()         % pv5()         % pc0()         % pc1()         << endl;
          }
          os << "# --- Profile --------------------------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11.3f "       "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "CPS"         % "CST"         % "CSP"         % "CTF"         % "CTL"         % "CTU"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(sec)"       % "(dBar)"      % "(dBar)"      % "(dBar)"      ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) cps()   % (int) cst()   % csp_sec()     % ctf()         % ctl()         % ctu()         << endl;
          }
          os << "# --- Profile - Binning Breakpoints ----------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "CPB1"        % "CPB2"        % "CPB3"        ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(dBar)"      % "(dBar)"      ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % cpb1()        % cpb2()        % cpb3()        << endl;
          }
          os << "# --- Profile - Binning Configuration --------------------------------------------------------------"            << endl;
          switch(sensor_type()) {
          case 0: { // RBR
              static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "        "%11s "          "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "        "%11s "          "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11.1f "      "%11.1f "        "%11.1f "       "%11.3f "       "%11.3f "       "%11.3f"        );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "CPN1"        % "CPN2"        % "CPN3"        % "CPP1"        % "CPP2"        % "CPP3"        ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(dBar)"      % "(dBar)"      % "(sec)"       % "(sec)"       % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % cpn1_dbar()   % cpn2_dbar()   % cpn3_dbar()   % cpp1_sec()    % cpp2_sec()    % cpp3_sec()    << endl;
          }
              break;
          case 1:  { // SBE
              static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "CPN1"        % "CPN2"        % "CPN3"        % "CPP1"        % "CPP2"        % "CPP3"        ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(n/a)"       % "(n/a)"       % "(n/a)"       % "(dBar)"      % "(dBar)"      % "(dBar)"      ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % cpn1()        % cpn2()        % cpn3()        % cpp1()        % cpp2()        % cpp3()        << endl;
          }
              break;
          default:
              static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "CPN1"        % "CPN2"        % "CPN3"        % "CPP1"        % "CPP2"        % "CPP3"        ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % ""            % ""            % ""            % ""            ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % cpn1()        % cpn2()        % cpn3()        % cpp1()        % cpp2()        % cpp3()        << endl;
              break;
          }
          os << "# --- Pump Characteristics -------------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PFR"         % "PFC"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(mL/min)"    % "(mL/dBar)"   ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % pfr()         % pfc()         << endl;
          }
          os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "        "%11s"           );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "        "%11s"           );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "        "%11.3f"         );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "DNS"         % "DTD"         % "DDD"         % "DDT"        % "DSI"          ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(dBar)"      % "(dBar)"      % "(sec)"      % "(sec)"        ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % dns()         % dtd()         % ddd()         % ddt()        % dsi_sec()      << endl;
          }
          os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "DSD"         % "DST"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % dsd()         % dst()         << endl;
          }
          os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "DFT"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % dft()         << endl;
          }
          os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "DCT"         % "DCM"         % "DAL"         % "DCA"         % "DSW"         % "DUC"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(sec)"       % ""            % ""            % ""            % "(sec)"       % ""            ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % dct()         % (int) dcm()   % (int) dal()   % (int) dca()   % dsw()         % duc()         << endl;
          }
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "DPT"         % "DUT"         % "DVT"         % "DCX"         % "DCP"         % "DCF"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(sec)"       % "(sec)"       % "(sec)"       % "(sec)"       % "(sec)"       % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % dpt()         % dut()         % dvt()         % dcx()         % dcp()         % dcf()         << endl;
          }
          os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         " 0x%08X "      "%11d "         "%11.3f "       "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PNS"         % "PTO"         % "PTH"         % "PSA"         % "PSI"         % "PFM"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(sec)"       % ""            % ""            % "(sec)"       % ""            ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % pns()         % pto()         % pth()         % (int) psa()   % psi_sec()     % (int) pfm()   << endl;
          }
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PFD"         % "PDT"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % pfd()         % pdt()         << endl;
          }
          os << "# --- Drift/Park - Seek ----------------------------------------------------------------------------"            << endl;
          {
              static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "PSM"         % "PST"         % "PSD"         % "PSV"         % "PSW"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(sec)"       % "(dBar)"      % "(sec)"       % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) psm()   % pst()         % psd()         % psv()         % psw()         << endl;
          }
          os << "# --- Ascent/Profile -------------------------------------------------------------------------------"            << endl;
          {
              static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11.2f "       "%11d "         "%11d "         "%11d "         "%11.3f"        );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "ASN"         % "ARR"         % "APM"         % "APN"         % "APF"         % "ASI"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(m/sec)"     % "(sec)"       % "(sec)"       % "(sec)"       % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % asn()         % arr_mps() % apm()         % apn()         % apf()         % asi_sec()     << endl;
          }
          {
              static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11.3f "       "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "ASD"         % "ADD"         % "ADT"         % "ARC"         % "AHR"         % "AHD"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(dBar)"      % "(dBar)"      % "(sec)"       % "(?)"         % "(m/sec)"     % "(dBar)"      ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % asd()         % add()         % adt()         % arc()         % ahr_mps() % ahd()         << endl;
          }
          os << "# --- Ascent - Ice-Detection -----------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%12s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "     0x%04X "  "%11d "         "%11d "         "%11.2f "       "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "ICM"         % "IMM"         % "ICN"         % "ICX"         % "ICT"         % "ICS"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(dBar)"      % "(dBar)"      % "(degC)"        % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) icm()   % imm()         % icn()         % icx()         % ict_degc()    % ics()         << endl;
          }
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11.2f "       "%11d "         " 0x%08X "      " 0x%08X"       );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "ISP"         % "ISM"         % "IST"         % "ISO"         % "ICF"         % "ICW"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(PSU)"       % "(sec)"       % ""            % ""            ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) isp()   % (int) ism()   % ist_psu()     % iso()         % icf()         % icw()         << endl;
          }
          os << "# --- Ascent - Ice-Detection (Summer) --------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "ISS0"        % "ICA0"        % "ICR0"        % "ICD0"        % "ICO0"        % "ICG0"        ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(min)"       % "(dBar)"      % "(sec)"       % ""            ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) iss0()  % ica0()        % icr0()        % icd0()        % ico0()        % (int) icg0()  << endl;
          }
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "IUE0"        % "ICB"         % "IMT0"        % "ILT0"        ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(%)"         % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) iue0()  % icb0()        % imt0()        % ilt0()        << endl;
          }
          os << "# --- Ascent - Ice-Detection (Winter) --------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "ISS1"        % "ICA1"        % "ICR1"        % "ICD1"        % "ICO1"        % "ICG1"        ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(min)"       % "(dBar)"      % "(sec)"       % ""            ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) iss1()  % ica1()        % icr1()        % icd1()        % ico1()        % (int) icg1()  << endl;
          }
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "IUE1"        % "ICB1"        % "IMT1"        % "ILT1"        ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % ""            % "(%)"         % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) iue1()  % icb1()        % imt1()        % ilt1()        << endl;
          }
          os << "# --- Surface --------------------------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d "         "%11.3f"        );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "SFN"         % "SGT"         % "SET"         % "SDT"         % "SSA"         % "SSI"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(min)"       % "(min)"       % "(?)"         % ""            % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % sfn()         % sgt()         % set()         % sdt()         % ssa()         % ssi_sec()     << endl;
          }
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "SDS"         % "SPT"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % "(sec)"       % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % sds()         % spt()         << endl;
          }
          os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11.3f "       "%11.3f "       "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "SAM"         % "SAX"         % "SAN"         % "SAO"         % "SAP"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(psi)"       % "(psi)"       % "(sec)"       % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) sam()   % sax_psi()     % san_psi()     % sao()         % sap()         << endl;
          }
          os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s "         "%11s "         "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d "         "%11d "         "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "MAM"         % "MAT"         % "ACA"         % "ACG"         % "ACR"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(sec)"       % "(g/FSR)"     % "(°/sec/FSR)" % "(Hz)"        ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) mam()   % mat()         % aca()         % acg()         % acr()         << endl;
          }
          os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
          {   static const format fmt1("#   %5s %4s %4s %4s "                                                     "%11s "         "%11s"          );
              static const format fmt2("#    %5s %4s %4s %4s "                                                    "%11s "         "%11s"          );
              static const format fmt3("mis %5d %4d %4d %4d "                                                    "%11d "         "%11d"          );
              static const string str1(boost::str(format(fmt1)    % "SN"  % "dive"    % "idx"     % "midx"        % "SBM"         % "SBT"         ));
              static const string str2(boost::str(format(fmt2)    % ""    % ""        % ""        % ""            % ""            % "(sec)"       ));
              os << str1 << endl << str2 << endl << format(fmt3)  % whoi  % dive      % idx       % midx++        % (int) sbm()   % sbt()         << endl;
          }

      }

          break;

      case 0x04056300: // ALAMO Software version 0x04056300 (2017 S2h test float 10108).

      {
              static const format fmt_d(                          "%3d " "%-5s " "%10d     "  "%-12s " "# %s");
              static const format fmt_f1(                         "%3d " "%-5s " "%12.01f   " "%-12s " "# %s");
              static const format fmt_f2(                         "%3d " "%-5s " "%13.02f  "  "%-12s " "# %s");
              static const format fmt_f3(                         "%3d " "%-5s " "%14.03f "   "%-12s " "# %s");
              static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%14s "      "%-12s " "  %s");
              static const format fmt_pre("m   " "%5d " "%4d " );
              static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value    "   % "units"   % "desc"));
              //                          #      SN dive idx param      value     units          desc
              static const string str_h2("#      -- ---- --- -----      -----     -----          ----");

              int stype;
              string stype_str;
              string lpre = boost::str(format(fmt_pre)  % whoi % dive);

              switch(stype = sensor_type()) {
              case 0:     stype_str = "RBR";                          break;
              case 1:     stype_str = "SBE";                          break;
              default:    stype_str = "unknown"; stype = -99;         break; }
              { // Header line.
                static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
                static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
                static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
                static const string str2("#");
                os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
              }
              os << "# --- General --------------------------------------------------------------------------------------" << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
              os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
              os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
              os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
              os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
              os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
              os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
              os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
              os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
              os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
              os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
              os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
              switch(sensor_type()) {
              case 0:  // RBR
                os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
                os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
                os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
                os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
                os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
                os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
                break;
              case 1:   // SBE
                os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
                os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
                os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
                os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
                os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
                os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
                break;
              default:
                os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
                os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
                os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
                os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
                os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
                os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
                break;
              }
              os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
              os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
              os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
              os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
              os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DTD"  % dtd()       % "[dBar]"      % "target drift/park pressure"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
              os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
              os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "DSD"  % dsd()       % "[dBar]"      % "first pump pressure"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DST"  % dst()       % "[sec]"       % "first pump timeout"  << endl;
              os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "DFT"  % dft()       % "[sec]"       % "first pump duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DCT"  % dct()       % "[sec]"       % "wait duration after first pump"  << endl;
              os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
              os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
              os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
              os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
              os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PTH"  % pth()       % "[-]"         % "hour-of-day trigger mask"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PTO"  % pto()       % "[sec]"       % "drift/park duration trigger"  << endl;
              os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "PFM"  % pfm()       % "[-]"         % "fall mode0"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PFD"  % pfd()       % "[dBar]"      % "target profile pressure"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PFT"  % pft()       % "[sec]"       % "fall timeout"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "PFV"  % pfv()       % "[sec]"       % "valve open duration"  << endl;
              os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
              os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
              os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
              os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
              os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
              os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
              os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
              os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
              os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
              os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
              os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
              os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
              os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
              os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
              os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
              os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
              os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
              os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
              os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "SAP"  % sap()       % "[sec]"       % "maximum air pump duration"  << endl;
              os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
              os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
              os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
              os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
              os << str_h1 << endl << str_h2 << endl;
              os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
              os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;

            }

      break;

      case 0x05006100: // ALAMO Software version 0x05006100 (2017 Alto test float 11011).
      case 0x05006101: // ALAMO Software version 0x05006101 alternate (2017 Alto test float 10109).

      {
        static const format fmt_d(                          "%3d " "%-5s " "%10d     "  "%-12s " "# %s");
        static const format fmt_f1(                         "%3d " "%-5s " "%12.01f   " "%-12s " "# %s");
        static const format fmt_f2(                         "%3d " "%-5s " "%13.02f  "  "%-12s " "# %s");
        static const format fmt_f3(                         "%3d " "%-5s " "%14.03f "   "%-12s " "# %s");
        static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%14s "      "%-12s " "  %s");
        static const format fmt_pre("m   " "%5d " "%4d " );
        static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value    "   % "units"   % "desc"));
        //                          #      SN dive idx param      value     units          desc
        static const string str_h2("#      -- ---- --- -----      -----     -----          ----");

        int stype;
        string stype_str;
        string lpre = boost::str(format(fmt_pre)  % whoi % dive);

        switch(stype = sensor_type()) {
        case 0:     stype_str = "RBR";                          break;
        case 1:     stype_str = "SBE";                          break;
        default:    stype_str = "unknown"; stype = -99;         break; }
        { // Header line.
          static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
          static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
          static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
          static const string str2("#");
          os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
        }
        os << "# --- General --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
        os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
        os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
        os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MTC"  % mtc()       % "[-]"         % "trigger mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
        os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
        os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
        os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
        os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
        switch(sensor_type()) {
        case 0:  // RBR
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
          break;
        case 1:   // SBE
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
          break;
        default:
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
          break;
        }
        os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
        os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
        os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
        os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
        os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
        os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
        os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
        os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
        os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
        os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
        os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
        os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
        os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
        os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
        os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
        os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
        os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAP"  % sap()       % "[sec]"       % "maximum air pump duration"  << endl;
        os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
        os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
        os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;

      }
        break;

      case 0x06000000: // ALAMO Software version 0x06000000

      case 0x06000100: // ALAMO Software version 0x06000100
      case 0x06010300: // ALAMO Software version 0x06010300 (2019 ALTO RBR/O2 test float)
      case 0x06040900: // ALAMO Software version 0x06040900 (v6.4.9: K. Wood's 2019 Arctic PAR floats)

      {
        static const format fmt_d(                          "%3d " "%-5s " "%10d     "  "%-12s " "# %s");
        static const format fmt_f1(                         "%3d " "%-5s " "%12.01f   " "%-12s " "# %s");
        static const format fmt_f2(                         "%3d " "%-5s " "%13.02f  "  "%-12s " "# %s");
        static const format fmt_f3(                         "%3d " "%-5s " "%14.03f "   "%-12s " "# %s");
        static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%14s "      "%-12s " "  %s");
        static const format fmt_pre("m   " "%5d " "%4d " );
        static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value    "   % "units"   % "desc"));
        //                          #      SN dive idx param      value     units          desc
        static const string str_h2("#      -- ---- --- -----      -----     -----          ----");

        int stype;
        string stype_str;
        string lpre = boost::str(format(fmt_pre)  % whoi % dive);

        switch(stype = sensor_type()) {
        case 0:     stype_str = "RBR";                          break;
        case 1:     stype_str = "SBE";                          break;
        default:    stype_str = "unknown"; stype = -99;         break; }
        { // Header line.
          static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
          static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
          static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
          static const string str2("#");
          os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
        }
        os << "# --- General --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
        os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
        os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
        os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MTC"  % mtc()       % "[-]"         % "trigger mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
        os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
        os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
        os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
        os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
        switch(sensor_type()) {
        case 0:  // RBR
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
          break;
        case 1:   // SBE
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
          break;
        default:
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
          break;
        }
        os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
        os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
        os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
        os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
        os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
        os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
        os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
        os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
        os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
        os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
        os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
        os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
        os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APS"  % aps()       % "[sec]"       % "initial pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
        os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
        os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
        os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
        os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAP"  % sap()       % "[sec]"       % "maximum air pump duration"  << endl;
        os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
        os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
        os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;

      }
        break;
      case 0x06050100: // ALAMO Software version 0x06050100

      {
        static const format fmt_d(                          "%3d " "%-5s " "%10d     "  "%-12s " "# %s");
        static const format fmt_f1(                         "%3d " "%-5s " "%12.01f   " "%-12s " "# %s");
        static const format fmt_f2(                         "%3d " "%-5s " "%13.02f  "  "%-12s " "# %s");
        static const format fmt_f3(                         "%3d " "%-5s " "%14.03f "   "%-12s " "# %s");
        static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%14s "      "%-12s " "  %s");
        static const format fmt_pre("m   " "%5d " "%4d " );
        static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value    "   % "units"   % "desc"));
        //                          #      SN dive idx param      value     units          desc
        static const string str_h2("#      -- ---- --- -----      -----     -----          ----");

        int stype;
        string stype_str;
        string lpre = boost::str(format(fmt_pre)  % whoi % dive);

        switch(stype = sensor_type()) {
        case 0:     stype_str = "RBR";                          break;
        case 1:     stype_str = "SBE";                          break;
        default:    stype_str = "unknown"; stype = -99;         break; }
        { // Header line.
          static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
          static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
          static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
          static const string str2("#");
          os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
        }
        os << "# --- General --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
        os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
        os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
        os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MTC"  % mtc()       % "[-]"         % "trigger mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
        os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
        os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
        os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
        os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
        switch(sensor_type()) {
        case 0:  // RBR
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
          break;
        case 1:   // SBE
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
          break;
        default:
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
          break;
        }
        os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
        os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
        os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
        os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
        os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
        os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
        os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
        os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
        os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
        os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
        os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
        os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
        os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APS"  % aps()       % "[sec]"       % "initial pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
        os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
        os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
        os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
        os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAP"  % sap()       % "[sec]"       % "maximum air pump duration"  << endl;
        os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
        os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
        os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;

      }
        break;

      case 0x06057700: // ALAMO Software version 0x06057700 (v6.9.w: B. Sanabia's 2019 Hurricane floats w/ WIMS)

      {
        static const format fmt_d(                          "%3d " "%-5s " "%10d     "  "%-12s " "# %s");
        static const format fmt_f1(                         "%3d " "%-5s " "%12.01f   " "%-12s " "# %s");
        static const format fmt_f2(                         "%3d " "%-5s " "%13.02f  "  "%-12s " "# %s");
        static const format fmt_f3(                         "%3d " "%-5s " "%14.03f "   "%-12s " "# %s");
        static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%14s "      "%-12s " "  %s");
        static const format fmt_pre("m   " "%5d " "%4d " );
        static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value    "   % "units"   % "desc"));
        //                          #      SN dive idx param      value     units          desc
        static const string str_h2("#      -- ---- --- -----      -----     -----          ----");

        int stype;
        string stype_str;
        string lpre = boost::str(format(fmt_pre)  % whoi % dive);

        switch(stype = sensor_type()) {
        case 0:     stype_str = "RBR";                          break;
        case 1:     stype_str = "SBE";                          break;
        default:    stype_str = "unknown"; stype = -99;         break; }
        { // Header line.
          static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
          static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
          static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
          static const string str2("#");
          os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
        }
        os << "# --- General --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
        os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
        os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
        os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MTC"  % mtc()       % "[-]"         % "trigger mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
        os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
        os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
        os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
        os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
        switch(sensor_type()) {
        case 0:  // RBR
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
          break;
        case 1:   // SBE
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
          break;
        default:
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
          break;
        }
        os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
        os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
        os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
        os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
        os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
        os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
        os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
        os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
        os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
        os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
        os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
        os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
        os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APS"  % aps()       % "[sec]"       % "initial pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
        os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
        os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
        os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
        os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAP"  % sap()       % "[sec]"       % "maximum air pump duration"  << endl;
        os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
        os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "WMT"  % wmt()       % "[sec]"       % "WIMS timeout"  << endl;
        os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;

      }
        break;

      case 0x06080500: // ALAMO Software version 0x06080500 (2019 ALTO #11011 test float)

      {
        static const format fmt_d(                          "%3d " "%-5s " "%10d     "  "%-12s " "# %s");
        static const format fmt_f1(                         "%3d " "%-5s " "%12.01f   " "%-12s " "# %s");
        static const format fmt_f2(                         "%3d " "%-5s " "%13.02f  "  "%-12s " "# %s");
        static const format fmt_f3(                         "%3d " "%-5s " "%14.03f "   "%-12s " "# %s");
        static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%14s "      "%-12s " "  %s");
        static const format fmt_pre("m   " "%5d " "%4d " );
        static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value    "   % "units"   % "desc"));
        //                          #      SN dive idx param      value     units          desc
        static const string str_h2("#      -- ---- --- -----      -----     -----          ----");

        int stype;
        string stype_str;
        string lpre = boost::str(format(fmt_pre)  % whoi % dive);

        switch(stype = sensor_type()) {
        case 0:     stype_str = "RBR";                          break;
        case 1:     stype_str = "SBE";                          break;
        default:    stype_str = "unknown"; stype = -99;         break; }
        { // Header line.
          static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
          static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
          static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
          static const string str2("#");
          os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
        }
        os << "# --- General --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
        os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
        os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
        os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MTC"  % mtc()       % "[-]"         % "trigger mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
        os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
        os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
        os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
        os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
        switch(sensor_type()) {
        case 0:  // RBR
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
          break;
        case 1:   // SBE
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
          break;
        default:
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
          break;
        }
        os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
        os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
        os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
        os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
        os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
        os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
        os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
        os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
        os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
        os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
        os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
        os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
        os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APS"  % aps()       % "[sec]"       % "initial pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
        os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
        os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
        os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
        os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAP"  % sap()       % "[sec]"       % "maximum air pump duration"  << endl;
        os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
        os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
        os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;

      }
        break;

      case 0x07020200: // ALAMO Software version 0x07020200 (2020 ALTO #11011 test float)
      case 0x07020300: // ALAMO Software version 0x07020300 (2020 ALTO #11011 test float)
      case 0x07020400: // ALAMO Software version 0x07020400 (2020 ALTO #11011 test float)
      case 0x07020500: // ALAMO Software version 0x07020500 (2020 ALTO #11011 test float)
      case 0x07020600: // ALAMO Software version 0x07020600 (2020 ALTO #11011 test float)
      case 0x07020700: // ALAMO Software version 0x07020700 (2020 ALTO #11011 test float)
      case 0x07020800: // ALAMO Software version 0x07020800 (2020 ALTO #11011 test float)
      case 0x07020900: // ALAMO Software version 0x07020900 (2020 ALTO #11011 test float)
      case 0x07030000: // ALAMO Software version 0x07020900 (2020 ALTO RBR/ODO #11060 test float)
      case 0x07030500: // ALAMO Software version 0x07020900 (2020 ALTO RBR/ODO #11060 test float)

      {
        static const format fmt_d(                          "%3d " "%-5s " "%10d     "  "%-12s " "# %s");
        static const format fmt_f1(                         "%3d " "%-5s " "%12.01f   " "%-12s " "# %s");
        static const format fmt_f2(                         "%3d " "%-5s " "%13.02f  "  "%-12s " "# %s");
        static const format fmt_f3(                         "%3d " "%-5s " "%14.03f "   "%-12s " "# %s");
        static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%14s "      "%-12s " "  %s");
        static const format fmt_pre("m   " "%5d " "%4d " );
        static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value    "   % "units"   % "desc"));
        //                          #      SN dive idx param      value     units          desc
        static const string str_h2("#      -- ---- --- -----      -----     -----          ----");

        int stype;
        string stype_str;
        string lpre = boost::str(format(fmt_pre)  % whoi % dive);

        switch(stype = sensor_type()) {
        case 0:     stype_str = "RBR";                          break;
        case 1:     stype_str = "SBE";                          break;
        default:    stype_str = "unknown"; stype = -99;         break; }
        { // Header line.
          static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
          static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
          static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
          static const string str2("#");
          os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
        }
        os << "# --- General --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
        os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
        os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
        os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
        os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
        os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
        os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
        os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
        switch(sensor_type()) {
        case 0:  // RBR
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
          break;
        case 1:   // SBE
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
          break;
        default:
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
          break;
        }
        os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
        os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
        os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
        os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
        os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
        os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
        os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
        os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
        os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSS"  % pss()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
        os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
        os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
        os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
        os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APS"  % aps()       % "[sec]"       % "initial pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
        os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
        os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
        os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
        os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
        os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
        os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
        os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;

      }
        break;

      case 0x08000000: // ALAMO Software version 0x08000000
      case 0x08000001: // ALAMO Software version 0x08000001
      case 0x08000100: // ALAMO Software version 0x08000100
      case 0x08000101: // ALAMO Software version 0x08000101 (2020 Hurricane floats)
      case 0x08000200: // ALAMO Software version 0x08000200
      case 0x08000300: // ALAMO Software version 0x08000300
      case 0x08000400: // ALAMO Software version 0x08000400
      case 0x08000500: // ALAMO Software version 0x08000500
      case 0x08010001: // ALAMO Software version 0x08010001 (2021 Hurricane Floats)

      {
        static const format fmt_d(                          "%3d " "%-5s " "%10d     "  "%-12s " "# %s");
        static const format fmt_f1(                         "%3d " "%-5s " "%12.01f   " "%-12s " "# %s");
        static const format fmt_f2(                         "%3d " "%-5s " "%13.02f  "  "%-12s " "# %s");
        static const format fmt_f3(                         "%3d " "%-5s " "%14.03f "   "%-12s " "# %s");
        static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%14s "      "%-12s " "  %s");
        static const format fmt_pre("m   " "%5d " "%4d " );
        static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value    "   % "units"   % "desc"));
        //                          #      SN dive idx param      value     units          desc
        static const string str_h2("#      -- ---- --- -----      -----     -----          ----");

        int stype;
        string stype_str;
        string lpre = boost::str(format(fmt_pre)  % whoi % dive);

        switch(stype = sensor_type()) {
        case 0:     stype_str = "RBR";                          break;
        case 1:     stype_str = "SBE";                          break;
        default:    stype_str = "unknown"; stype = -99;         break; }
        { // Header line.
          static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
          static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
          static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
          static const string str2("#");
          os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
        }
        os << "# --- General --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
        os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
        os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
        os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
        os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
        os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
        os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
        os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
        switch(sensor_type()) {
        case 0:  // RBR
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
          break;
        case 1:   // SBE
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
          break;
        default:
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
          break;
        }
        os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
        os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
        os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
        os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
        os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
        os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
        os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
        os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
        os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSS"  % pss()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
        os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
        os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
        os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
        os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APS"  % aps()       % "[sec]"       % "initial pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
        os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
        os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
        os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
        os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
        os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
        os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "WST"  % wmt()       % "[sec]"       % "WIMS timeout duration"  << endl;
        os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;

      }
        break;

      case 0x08010000: // ALAMO Software version 0x08010000
      case 0x09000000: // ALAMO Software version 0x09000000
      case 0x0A000000: // ALAMO Software version 0x0A000000
      case 0x0A000100: // ALAMO Software version 0x0A000100
      case 0x0A020000: // ALAMO Software version 10.2.0
      case 0x0A020100: // ALAMO Software version 10.2.1
      case 0x0A020200: // ALAMO Software version 10.2.2
      case 0x0A030000: // ALAMO Software version 10.3.0
      case 0x0A030100: // ALAMO Software version 10.3.1



      {
        static const format fmt_d(                          "%3d " "%-5s " "%10d     "  "%-12s " "# %s");
        static const format fmt_f1(                         "%3d " "%-5s " "%12.01f   " "%-12s " "# %s");
        static const format fmt_f2(                         "%3d " "%-5s " "%13.02f  "  "%-12s " "# %s");
        static const format fmt_f3(                         "%3d " "%-5s " "%14.03f "   "%-12s " "# %s");
        static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%14s "      "%-12s " "  %s");
        static const format fmt_pre("m   " "%5d " "%4d " );
        static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value    "   % "units"   % "desc"));
        //                          #      SN dive idx param      value     units          desc
        static const string str_h2("#      -- ---- --- -----      -----     -----          ----");

        int stype;
        string stype_str;
        string lpre = boost::str(format(fmt_pre)  % whoi % dive);

        switch(stype = sensor_type()) {
        case 0:     stype_str = "RBR";                          break;
        case 1:     stype_str = "SBE";                          break;
        default:    stype_str = "unknown"; stype = -99;         break; }
        { // Header line.
          static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
          static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
          static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
          static const string str2("#");
          os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
        }
        os << "# --- General --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
        os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
        os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
        os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
        os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
        os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
        os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
        os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
        switch(sensor_type()) {
        case 0:  // RBR
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
          break;
        case 1:   // SBE
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
          break;
        default:
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
          break;
        }
        os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
        os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
        os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
        os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
        os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
        os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
        os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
        os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
        os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSS"  % pss()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
        os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
        os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
        os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
        os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APS"  % aps()       % "[sec]"       % "initial pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
        os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
        os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
        os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
        os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
        os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
        os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
//        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
//        os << str_h1 << endl << str_h2 << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "WST"  % wmt()       % "[sec]"       % "WIMS timeout duration"  << endl;
        os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;

      }
        break;

      case 0x0A060231: // ALAMO Software version 10.6.2.31 (WIMS)

      {
             static const format fmt_d(                          "%3d " "%-5s " "%10d      "  "%-12s " "# %s");
             static const format fmt_f1(                         "%3d " "%-5s " "%12.01f    " "%-12s " "# %s");
             static const format fmt_f2(                         "%3d " "%-5s " "%13.02f   "  "%-12s " "# %s");
             static const format fmt_f3(                         "%3d " "%-5s " "%14.03f  "   "%-12s " "# %s");
             static const format fmt_f4(                         "%3d " "%-5s " "%15.04f "    "%-12s " "# %s");
             static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%15s "       "%-12s " "  %s");
             static const format fmt_pre("m   " "%5d " "%4d " );
             static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value     "   % "units"   % "desc"));
             //                          #      SN dive idx param      value      units          desc
             static const string str_h2("#      -- ---- --- -----      -----      -----          ----");

             int stype;
             string stype_str;
             string lpre = boost::str(format(fmt_pre)  % whoi % dive);

             switch(stype = sensor_type()) {
             case 0:     stype_str = "RBR";                          break;
             case 1:     stype_str = "SBE";                          break;
             default:    stype_str = "unknown"; stype = -99;         break; }
             { // Header line.
               static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
               static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
               static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
               static const string str2("#");
               os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
             }
             os << "# --- General --------------------------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
             os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
             os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
             os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
             os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
             os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
             os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
             os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
             os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
             os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
             os << "# --- Profile - Dynamic Salinity Correction --------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PPE"  % (int)ppe()     % "[-]"      % "dynamic correction mode"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "PPC0" % ppc_alpha()    % "[sec]"    % "tau: conductivity-temperature lag"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "PPC1" % ppc_tau()      % "[-]"      % "alpha: short-term thermal mass correction" << endl;
             os << lpre << format(fmt_f3)    % midx++  % "PPC2" % ppc_tdelay()   % "[sec]"    % "tdelay: water-cell lag"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "PPC3" % ppc_ctccoeff() % "[-]"      % "ctccoeff: long-term thermal mass correction"  << endl;
             os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
             switch(sensor_type()) {
             case 0:  // RBR
               os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
               os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
               os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
               os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
               break;
             case 1:   // SBE
               os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
               break;
             default:
               os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
               break;
             }
             os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
             os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
             os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
             os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
             os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
             os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
             os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
             os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
             os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PSS"  % pss()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
             os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
             os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
             os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
             os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
             os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "APS"  % aps()       % "[sec]"       % "initial pump on duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
             os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
             os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
             os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
             os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
             os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
             os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
             os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
             os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
             os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
             os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
             os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
             os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
     //        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
     //        os << str_h1 << endl << str_h2 << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "WST"  % wmt()       % "[sec]"       % "WIMS timeout duration"  << endl;
             os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;
             os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "WST"  % wmt()       % "[sec]"       % "WIMS timeout duration"  << endl;


           }
             break;

      case 0x0A050000: // ALAMO Software version 10.5.0.0
      case 0x0A050001: // ALAMO Software version 10.5.0.1
      case 0x0A050002: // ALAMO Software version 10.5.0.2
      case 0x0A050003: // ALAMO Software version 10.5.0.3
      case 0x0A050004: // ALAMO Software version 10.5.0.4
      case 0x0A060000: // ALAMO Software version 10.6.0.0
      case 0x0A060001: // ALAMO Software version 10.6.0.1
      case 0x0A060002: // ALAMO Software version 10.6.0.2
      case 0x0A060003: // ALAMO Software version 10.6.0.3
      case 0x0A060004: // ALAMO Software version 10.6.0.4
      case 0x0A060100: // ALAMO Software version 10.6.1.0
      case 0x0A060101: // ALAMO Software version 10.6.1.1
      case 0x0A060102: // ALAMO Software version 10.6.1.2
      case 0x0A060103: // ALAMO Software version 10.6.1.3
      case 0x0A060104: // ALAMO Software version 10.6.1.4
      case 0x0A060133: // ALAMO Software version 10.6.1.33 Micro-ALTO 10.6.1
      case 0x0A060200: // ALAMO Software version 10.6.2.0
      case 0x0A060201: // ALAMO Software version 10.6.2.1
      case 0x0A060202: // ALAMO Software version 10.6.2.2
      case 0x0A060203: // ALAMO Software version 10.6.2.3
      case 0x0A060204: // ALAMO Software version 10.6.2.4
      case 0x0A060233: // ALAMO Software version 10.6.2.4
      case 0x0A060300: // ALAMO Software version 10.6.2.0
      case 0x0A060301: // ALAMO Software version 10.6.2.1
      case 0x0A060302: // ALAMO Software version 10.6.2.2
      case 0x0A060303: // ALAMO Software version 10.6.2.3
      case 0x0A060304: // ALAMO Software version 10.6.2.4
      case 0x0A060333: // ALAMO Software version 10.6.2.4

      {
        static const format fmt_d(                          "%3d " "%-5s " "%10d      "  "%-12s " "# %s");
        static const format fmt_f1(                         "%3d " "%-5s " "%12.01f    " "%-12s " "# %s");
        static const format fmt_f2(                         "%3d " "%-5s " "%13.02f   "  "%-12s " "# %s");
        static const format fmt_f3(                         "%3d " "%-5s " "%14.03f  "   "%-12s " "# %s");
        static const format fmt_f4(                         "%3d " "%-5s " "%15.04f "    "%-12s " "# %s");
        static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%15s "       "%-12s " "  %s");
        static const format fmt_pre("m   " "%5d " "%4d " );
        static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value     "   % "units"   % "desc"));
        //                          #      SN dive idx param      value      units          desc
        static const string str_h2("#      -- ---- --- -----      -----      -----          ----");

        int stype;
        string stype_str;
        string lpre = boost::str(format(fmt_pre)  % whoi % dive);

        switch(stype = sensor_type()) {
        case 0:     stype_str = "RBR";                          break;
        case 1:     stype_str = "SBE";                          break;
        default:    stype_str = "unknown"; stype = -99;         break; }
        { // Header line.
          static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
          static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
          static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
          static const string str2("#");
          os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
        }
        os << "# --- General --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
        os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
        os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
        os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
        os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
        os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
        os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
//        switch(sensor_type()) {
//        case 0:  // RBR
//          os << "# --- Profile - Dynamic Salinity Correction --------------------------------------------------------" << endl;
//          os << str_h1 << endl << str_h2 << endl;
//          os << lpre << format(fmt_d)     % midx++  % "PPE"  % (int)ppe()     % "[-]"      % "dynamic correction mode"  << endl;
//          os << lpre << format(fmt_f3)    % midx++  % "PPC0" % ppc_alpha()    % "[sec]"    % "tau: conductivity-temperature lag"  << endl;
//          os << lpre << format(fmt_f3)    % midx++  % "PPC1" % ppc_tau()      % "[-]"      % "alpha: short-term thermal mass correction" << endl;
//          os << lpre << format(fmt_f3)    % midx++  % "PPC2" % ppc_tdelay()   % "[sec]"    % "tdelay: water-cell lag"  << endl;
//          os << lpre << format(fmt_f3)    % midx++  % "PPC3" % ppc_ctccoeff() % "[-]"      % "ctccoeff: long-term thermal mass correction"  << endl;
//          break;
//        case 1:   // SBE
//          break;
//        default:
//          break;
//        }
        os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
        switch(sensor_type()) {
        case 0:  // RBR
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
          break;
        case 1:   // SBE
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
          break;
        default:
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
          break;
        }
        os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
        os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
        os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
        os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
        os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
        os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
        os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
        os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
        os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSS"  % pss()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
        os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
        os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
        os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
        os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APS"  % aps()       % "[sec]"       % "initial pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
        os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
        os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
        os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
        os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
        os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
        os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
//        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
//        os << str_h1 << endl << str_h2 << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "WST"  % wmt()       % "[sec]"       % "WIMS timeout duration"  << endl;
        os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;

      }
        break;

      case 0x0A070101: // ALAMO Software version 10.7.1.1
      case 0x0A080004: // ALAMO Software version 10.8.0.4 (SBE)
      case 0x0A0C0001: // ALAMO Software version 10.12.0.1 (RBR)
      case 0x0A0C0032: // ALAMO/MM2 Software version 10.12.0.32

      {
        static const format fmt_d(                          "%3d " "%-5s " "%10d      "  "%-12s " "# %s");
        static const format fmt_f1(                         "%3d " "%-5s " "%12.01f    " "%-12s " "# %s");
        static const format fmt_f2(                         "%3d " "%-5s " "%13.02f   "  "%-12s " "# %s");
        static const format fmt_f3(                         "%3d " "%-5s " "%14.03f  "   "%-12s " "# %s");
        static const format fmt_f4(                         "%3d " "%-5s " "%15.04f "    "%-12s " "# %s");
        static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%15s "       "%-12s " "  %s");
        static const format fmt_pre("m   " "%5d " "%4d " );
        static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value     "   % "units"   % "desc"));
        //                          #      SN dive idx param      value      units          desc
        static const string str_h2("#      -- ---- --- -----      -----      -----          ----");

        int stype;
        string stype_str;
        string lpre = boost::str(format(fmt_pre)  % whoi % dive);

        switch(stype = sensor_type()) {
        case 0:     stype_str = "RBR";                          break;
        case 1:     stype_str = "SBE";                          break;
        default:    stype_str = "unknown"; stype = -99;         break; }
        { // Header line.
          static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
          static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
          static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
          static const string str2("#");
          os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
        }
        os << "# --- General --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
        os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
//        os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
//        os << str_h1 << endl << str_h2 << endl;
//        os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
//        os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
//        os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
        os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
        os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
        os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
        os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
//        switch(sensor_type()) {
//        case 0:  // RBR
//          os << "# --- Profile - Dynamic Salinity Correction --------------------------------------------------------" << endl;
//          os << str_h1 << endl << str_h2 << endl;
//          os << lpre << format(fmt_d)     % midx++  % "PPE"  % (int)ppe()     % "[-]"      % "dynamic correction mode"  << endl;
//          os << lpre << format(fmt_f3)    % midx++  % "PPC0" % ppc_alpha()    % "[sec]"    % "tau: conductivity-temperature lag"  << endl;
//          os << lpre << format(fmt_f3)    % midx++  % "PPC1" % ppc_tau()      % "[-]"      % "alpha: short-term thermal mass correction" << endl;
//          os << lpre << format(fmt_f3)    % midx++  % "PPC2" % ppc_tdelay()   % "[sec]"    % "tdelay: water-cell lag"  << endl;
//          os << lpre << format(fmt_f3)    % midx++  % "PPC3" % ppc_ctccoeff() % "[-]"      % "ctccoeff: long-term thermal mass correction"  << endl;
//          break;
//        case 1:   // SBE
//          break;
//        default:
//          break;
//        }
        os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
        switch(sensor_type()) {
        case 0:  // RBR
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
          os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
          break;
        case 1:   // SBE
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
          break;
        default:
          os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
          break;
        }
        os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
        os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
        os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
        os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
        os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
        os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
        os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
        os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
        os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSS"  % pss()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
        os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
        os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
        os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
        os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APS"  % aps()       % "[sec]"       % "initial pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
        os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
        os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
        os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
        os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
        os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
        os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
        os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
        os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
        os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
        os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
//        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
//        os << str_h1 << endl << str_h2 << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
//        os << lpre << format(fmt_d)     % midx++  % "WST"  % wmt()       % "[sec]"       % "WIMS timeout duration"  << endl;
        os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
        os << str_h1 << endl << str_h2 << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
        os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;

      }
        break;


      case 0x0A080031: // ALAMO Software version 10.8.0.31 (WIMS)
      case 0x0A0A0031: // ALAMO Software version 10.10.0.31 (WIMS)
      case 0x0A0C0131: // ALAMO Software version 10.12.1.31 (WIMS)
            

      {
             static const format fmt_d(                          "%3d " "%-5s " "%10d      "  "%-12s " "# %s");
             static const format fmt_f1(                         "%3d " "%-5s " "%12.01f    " "%-12s " "# %s");
             static const format fmt_f2(                         "%3d " "%-5s " "%13.02f   "  "%-12s " "# %s");
             static const format fmt_f3(                         "%3d " "%-5s " "%14.03f  "   "%-12s " "# %s");
             static const format fmt_f4(                         "%3d " "%-5s " "%15.04f "    "%-12s " "# %s");
             static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%15s "       "%-12s " "  %s");
             static const format fmt_pre("m   " "%5d " "%4d " );
             static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value     "   % "units"   % "desc"));
             //                          #      SN dive idx param      value      units          desc
             static const string str_h2("#      -- ---- --- -----      -----      -----          ----");

             int stype;
             string stype_str;
             string lpre = boost::str(format(fmt_pre)  % whoi % dive);

             switch(stype = sensor_type()) {
             case 0:     stype_str = "RBR";                          break;
             case 1:     stype_str = "SBE";                          break;
             default:    stype_str = "unknown"; stype = -99;         break; }
             { // Header line.
               static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
               static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
               static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
               static const string str2("#");
               os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
             }
             os << "# --- General --------------------------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
             os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
//             os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
//             os << str_h1 << endl << str_h2 << endl;
//             os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
//             os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
//             os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
             os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
             os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
             os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
             os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
             os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
             os << "# --- Profile - Dynamic Salinity Correction --------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PPE"  % (int)ppe()     % "[-]"      % "dynamic correction mode"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "PPC0" % ppc_alpha()    % "[sec]"    % "tau: conductivity-temperature lag"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "PPC1" % ppc_tau()      % "[-]"      % "alpha: short-term thermal mass correction" << endl;
             os << lpre << format(fmt_f3)    % midx++  % "PPC2" % ppc_tdelay()   % "[sec]"    % "tdelay: water-cell lag"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "PPC3" % ppc_ctccoeff() % "[-]"      % "ctccoeff: long-term thermal mass correction"  << endl;
             os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
             switch(sensor_type()) {
             case 0:  // RBR
               os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
               os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
               os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
               os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
               break;
             case 1:   // SBE
               os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
               break;
             default:
               os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
               os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
               break;
             }
             os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
             os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
             os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
             os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
             os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
             os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
             os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
             os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
             os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PSS"  % pss()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
             os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
             os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
             os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
             os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
             os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "APS"  % aps()       % "[sec]"       % "initial pump on duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
             os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
             os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
             os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
             os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
             os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
             os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
             os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
             os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
             os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
             os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
             os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
             os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
             os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
     //        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
     //        os << str_h1 << endl << str_h2 << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
     //        os << lpre << format(fmt_d)     % midx++  % "WST"  % wmt()       % "[sec]"       % "WIMS timeout duration"  << endl;
             os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
             os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;
             os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
             os << str_h1 << endl << str_h2 << endl;
             os << lpre << format(fmt_d)     % midx++  % "WST"  % wmt()       % "[sec]"       % "WIMS timeout duration"  << endl;


           }
             break;


      case 0xFFFFFFFF:

        os << "# Incomplete mission parameters." << endl;

        break;

      default: {          // Unsupported ALAMO software version.
          ostringstream os;
          os << "Unsupported ALAMO Mission Parameters message version: 0x"
             << hex
             << setw(8)
             << setfill('0')
             << fw_version()
             << "!";
#         if defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
          lwrn() << os.str();
#         else
          EXC_THROW(os.str());
#         endif // defined(XMSG_DEBUG_MFA_MISS_GFG) && XMSG_DEBUG_MFA_MISS_GFG
      }

      {
          static const format fmt_d(                          "%3d " "%-5s " "%10d     "  "%-12s " "# %s");
          static const format fmt_f1(                         "%3d " "%-5s " "%12.01f   " "%-12s " "# %s");
          static const format fmt_f2(                         "%3d " "%-5s " "%13.02f  "  "%-12s " "# %s");
          static const format fmt_f3(                         "%3d " "%-5s " "%14.03f "   "%-12s " "# %s");
          static const format fmt_h(  "#   " "%5s "  "%4s "   "%3s " "%-5s " "%14s "      "%-12s " "  %s");
          static const format fmt_pre("m   " "%5d " "%4d " );
          static const string str_h1(boost::str(format(fmt_h)  % "SN"  % "dive"   % "idx"  % "param" % "value    "   % "units"   % "desc"));
          //                          #      SN dive idx param      value     units          desc
          static const string str_h2("#      -- ---- --- -----      -----     -----          ----");

          int stype;
          string stype_str;
          string lpre = boost::str(format(fmt_pre)  % whoi % dive);

          switch(stype = sensor_type()) {
          case 0:     stype_str = "RBR";                          break;
          case 1:     stype_str = "SBE";                          break;
          default:    stype_str = "unknown"; stype = -99;         break; }
          { // Header line.
            static const format fmt1("#   %5s %4s %16s %11s %11s        %s");
            static const format fmt3("MIS %5d %4d             0x%02X "                               " 0x%08X "      "%11d "         "     # %s"          );
            static const string str1(boost::str(format(fmt1)    % "SN"   % "dive"   % "CfgMsgVer"   % "FWVer"       % "sensor-type" % "sensor-type-name"   ));
            static const string str2("#");
            os << str1 << endl << str2 << endl << format(fmt3)  % whoi   % dive     % msg_version() % fw_version()  % sensor_type() % stype_str     << endl;
          }
          os << "# --- General --------------------------------------------------------------------------------------" << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "MSN"  % msn()       % "[-]"         % "dive cycle number"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "APT"  % apt()       % "[-]"         % "auto-transmit mission parameters"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "BTM"  % btm()       % "[-]"         % "bluetooth mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "MDS"  % mds()       % "[dBar]"      % "maximum depth"                     << endl;
          os << lpre << format(fmt_d)     % midx++  % "XAS"  % xas()       % "[-]"         % "profile data encoding method"      << endl;
          os << "# --- Built-In Test (BIT) --------------------------------------------------------------------------" << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_f2)    % midx++  % "PNB"  % pnb_v()     % "[V]"         % "battery voltage threshold"  << endl;
          os << lpre << format(fmt_f2)    % midx++  % "PNC"  % pnc_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
          os << lpre << format(fmt_f2)    % midx++  % "PXC"  % pxc_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
          os << "# --- Calibration Coefficients ---------------------------------------------------------------------" << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "PBL"  % pbl()       % "[LSB@12V]"   % "battery voltage"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PBH"  % pbh()       % "[LSB@14V]"   % "battery voltage"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PP0"  % pp0()       % "[LSB@0A]"    % "pump current"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PP1"  % pp1()       % "[LSB@1A]"    % "pump current"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PVL"  % pvl()       % "[LSB@12V]"   % "regulator voltage"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PVH"  % pvh()       % "[LSB@14V]"   % "regulator voltage"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PV0"  % pv0()       % "[LSB@0inHg]" % "internal vacuum"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PV5"  % pv5()       % "[LSB@5inHg]" % "internal vacuum"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PC0"  % pc0()       % "[LSB@0A]"    % "regulator current"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PC1"  % pc1()       % "[LSB@1A]"    % "regulator current"  << endl;
          os << "# --- Start-of-Mission Trigger ---------------------------------------------------------------------" << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "MTD"  % mtd()       % "[dBar]"      % "pressure threshold"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "MTS"  % mts()       % "[sec]"       % "start-of-mission timeout"  << endl;
          os << lpre << format(fmt_f3)    % midx++  % "MTT"  % mtt_sec()   % "[sec]"       % "sample interval"  << endl;
          os << "# --- Diagnostic Dive ------------------------------------------------------------------------------" << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "DDB"  % ddb()       % "[-]"         % "bypass mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DDC"  % ddc()       % "[sec]"       % ""  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DDP"  % ddp()       % "[dBar]"      % "target pressure"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DDS"  % dds()       % "[sec]"       % "descent timeout"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DDF"  % ddf()       % "[sec]"       % "pump duration"  << endl;
          os << "# --- Mission Phase --------------------------------------------------------------------------------" << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "MPS"  % mps()       % "[-]"         % "current phase"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "MPC"  % mpc()       % "[cycles]"    % "current phase cycle number"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "MNM0" % mnm0()      % "[cycles]"    % "no. dive cycles, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "MNM1" % mnm1()      % "[cycles]"    % "no. dive cycles, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "MNM2" % mnm2()      % "[cycles]"    % "no. dive cycles, phase 2"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "MNS0" % mns0()      % "[-]"         % "next mission phase, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "MNS1" % mns1()      % "[-]"         % "next mission phase, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "MNS2" % mns2()      % "[-]"         % "next mission phase, phase 2"  << endl;
          os << "# --- Profile --------------------------------------------------------------------------------------" << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "CPS"  % cps()       % "[-]"         % "profiling mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "CST"  % cst()       % "[-]"         % "sampling mode"  << endl;
          os << lpre << format(fmt_f3)    % midx++  % "CSP"  % csp_sec()   % "[sec]"       % "continuous sampling period"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "CTC"  % ctc()       % "[-]"         % "aux. temperature channel mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "CTF"  % ctf()       % "[dBar]"      % "profile cut-off pressure"  << endl;
          os << "# --- Profile - Dynamic Salinity Correction --------------------------------------------------------" << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "PPE"  % (int)ppe()     % "[-]"      % "dynamic correction mode"  << endl;
          os << lpre << format(fmt_f3)    % midx++  % "PPC0" % ppc_alpha()    % "[s]"      % "tau: conductivity-temperature lag"  << endl;
          os << lpre << format(fmt_f3)    % midx++  % "PPC1" % ppc_tau()      % "[-]"      % "alpha: short-term thermal mass correction" << endl;
          os << lpre << format(fmt_f3)    % midx++  % "PPC2" % ppc_tdelay()   % "[s]"      % "tdelay: water-cell lag"  << endl;
          os << lpre << format(fmt_f3)    % midx++  % "PPC3" % ppc_ctccoeff() % "[-]"      % "ctccoeff: long-term thermal mass correction"  << endl;
          os << "# --- Profile - Binning Configuration --------------------------------------------------------------" << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "CPB1" % cpb1()      % "[dBar]"      % "interval 1 pressure boundary"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "CPB2" % cpb2()      % "[dBar]"      % "interval 2 pressure boundary"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "CPB3" % cpb3()      % "[dBar]"      % "interval 3 pressure boundary"  << endl;
          switch(sensor_type()) {
          case 0:  // RBR
            os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1_dbar() % "[dBar]"      % "interval 1 bin size"       << endl;
            os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2_dbar() % "[dBar]"      % "interval 2 bin size"       << endl;
            os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3_dbar() % "[dBar]"      % "interval 3 bin size"       << endl;
            os << lpre << format(fmt_f3)  % midx++  % "CPP1" % cpp1_sec()  % "[sec]"       % "interval 1 sample period"  << endl;
            os << lpre << format(fmt_f3)  % midx++  % "CPP2" % cpp2_sec()  % "[sec]"       % "interval 2 sample period"  << endl;
            os << lpre << format(fmt_f3)  % midx++  % "CPP3" % cpp3_sec()  % "[sec]"       % "interval 3 sample period"  << endl;
            break;
          case 1:   // SBE
            os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[dBar]"      % "interval 1 bin size"       << endl;
            os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[dBar]"      % "interval 2 bin size"       << endl;
            os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[dBar]"      % "interval 3 bin size"       << endl;
            os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[dBar]"      % "interval 1 sample interval"  << endl;
            os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[dBar]"      % "interval 2 sample interval"  << endl;
            os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[dBar]"      % "interval 3 sample interval"  << endl;
            break;
          default:
            os << lpre << format(fmt_d)   % midx++  % "CPN1" % cpn1()      % "[-]"            % ""  << endl;
            os << lpre << format(fmt_d)   % midx++  % "CPN2" % cpn2()      % "[-]"         % ""  << endl;
            os << lpre << format(fmt_d)   % midx++  % "CPN3" % cpn3()      % "[-]"         % ""  << endl;
            os << lpre << format(fmt_d)   % midx++  % "CPP1" % cpp1()      % "[-]"         % ""  << endl;
            os << lpre << format(fmt_d)   % midx++  % "CPP2" % cpp2()      % "[-]"         % ""  << endl;
            os << lpre << format(fmt_d)   % midx++  % "CPP3" % cpp3()      % "[-]"         % ""  << endl;
            break;
          }
          os << "# --- Profile - Raw Sampling -----------------------------------------------------------------------" << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "CRD"  % crd()       % "[dBar]"      % "threshold pressure"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "CRS"  % crs()       % "[samples]"   % "maximum samples"  << endl;
          os << "# --- Profile - Surface Offset Correction ----------------------------------------------------------" << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "CTL"  % ctl()       % "[dBar]"      % "lower limit"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "CTU"  % ctu()       % "[dBar]"      % "upper limit"  << endl;
          os << "# --- Pump Characteristics -------------------------------------------------------------------------" << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFR"  % pfr()       % "[mL/min]"    % "flow rate"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFC"  % pfc()       % "[uL/dBar]"   % "pressure correction"  << endl;
          os << "# --- Descent - General ----------------------------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "DNS"  % dns()       % "[-]"         % ""  << endl;
          os << lpre << format(fmt_f3)    % midx++  % "DSI"  % dsi_sec()   % "[sec]"       % "sleep interval"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DTD0" % dtd0()      % "[dBar]"      % "target drift/park pressure, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DTD1" % dtd1()      % "[dBar]"      % "target drift/park pressure, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DTD2" % dtd2()      % "[dBar]"      % "target drift/park pressure, phase 2"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DCR"  % dcr()       % "[sec]"       % ""  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DPD"  % dpd()       % "[sec]"       % ""  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DRA"  % dra()       % "[m/s]"       % ""  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DRT"  % dra()       % "[sec]"       % ""  << endl;
          os << "# --- Descent - Fall Time-Series -------------------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "DDD"  % ddd()       % "[dBar]"      % "maximum pressure interval"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DDT"  % ddt()       % "[sec]"       % "maximum time interval"     << endl;
          os << "# --- Descent - Stage 1 (Descent to First-Pump Pressure) -------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "DSD0" % dsd0()      % "[dBar]"      % "first pump pressure, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DSD1" % dsd1()      % "[dBar]"      % "first pump pressure, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DSD2" % dsd2()      % "[dBar]"      % "first pump pressure, phase 2"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DST0" % dst0()      % "[sec]"       % "first pump timeout, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DST1" % dst1()      % "[sec]"       % "first pump timeout, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DST2" % dst2()      % "[sec]"       % "first pump timeout, phase 2"  << endl;
          os << "# --- Descent - Stage 2 (First Descent Pump) -------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "DFT0" % dft0()      % "[sec]"       % "first pump duration, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DFT1" % dft1()      % "[sec]"       % "first pump duration, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DFT2" % dft2()      % "[sec]"       % "first pump duration, phase 2"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DCT0" % dct0()      % "[sec]"       % "wait duration after first pump, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DCT1" % dct1()      % "[sec]"       % "wait duration after first pump, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DCT2" % dct2()      % "[sec]"       % "wait duration after first pump, phase 2"  << endl;
          os << "# --- Descent - Stage 3 (Seek to Drift/Park Pressure) ----------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "DCM"  % dcm()       % "[-]"         % "seek mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DAL"  % dal()       % "[-]"         % "seek algorithm"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DCA"  % dca()       % "[seeks]"     % "maximum seeks"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DCF"  % dcf()       % "[sec]"       % "minimum valve time"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DCP"  % dcp()       % "[sec]"       % "minimum pump time"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DCX"  % dcx()       % "[sec]"       % "maximum pump time"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DPT"  % dpt()       % "[sec]"       % "100m pump time"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DUT"  % dut()       % "[sec]"       % "100m valve time"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DSW"  % dsw()       % "[sec]"       % "seek interval"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DUC"  % duc()       % "[%]"         % "dampening factor"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DVT"  % dvt()       % "[sec]"       % "velocity measurement delay"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DVD"  % dvd()       % "[dBar]"      % "pump time update, min. pres. delta"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "DVW"  % dvw()       % "[sec]"       % "pump time update delay duration"  << endl;
          os << "# --- Drift/Park - General -------------------------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "PNS"  % pns()       % "[-]"         % ""  << endl;
          os << lpre << format(fmt_f3)    % midx++  % "PSI"  % psi_sec()   % "[sec]"       % "sleep interval"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PSA"  % psa()       % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PSS"  % pss()       % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PDT"  % pdt()       % "[sec]"       % "sample interval"  << endl;
          os << "# --- Drift/Park - Seek to Drift/Park Pressure -----------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "PSM"  % psm()       % "[-]"         % "seek mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PST"  % pst()       % "[sec]"       % "seek interval"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PSD"  % psd()       % "[dBar]"      % "minimum seek adjustment"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PSV"  % psv()       % "[sec]"       % "velocity measurement delay"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PSW"  % psw()       % "[sec]"       % "wait duration after seek"  << endl;
          os << "# --- Drift/Park - Start of Profile Trigger --------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "PTD"  % ptd()       % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PTH0" % pth0()      % "[-]"         % "hour-of-day trigger mask, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PTH1" % pth1()      % "[-]"         % "hour-of-day trigger mask, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PTH2" % pth2()      % "[-]"         % "hour-of-day trigger mask, phase 2"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PTO0" % pto0()      % "[sec]"       % "drift/park duration trigger, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PTO1" % pto1()      % "[sec]"       % "drift/park duration trigger, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PTO2" % pto2()      % "[sec]"       % "drift/park duration trigger, phase 2"  << endl;
          os << "# --- Drift/Park - Fall to Profile Pressure --------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFM0" % pfm0()      % "[-]"         % "fall mode, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFM1" % pfm1()      % "[-]"         % "fall mode, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFM2" % pfm2()      % "[-]"         % "fall mode, phase 2"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFD0" % pfd0()      % "[dBar]"      % "target profile pressure, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFD1" % pfd1()      % "[dBar]"      % "target profile pressure, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFD2" % pfd2()      % "[dBar]"      % "target profile pressure, phase 2"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFT0" % pft0()      % "[sec]"       % "fall timeout, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFT1" % pft1()      % "[sec]"       % "fall timeout, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFT2" % pft2()      % "[sec]"       % "fall timeout, phase 2"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFV0" % pfv0()      % "[sec]"       % "valve open duration, phase 0"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFV1" % pfv1()      % "[sec]"       % "valve open duration, phase 1"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "PFV2" % pfv2()      % "[sec]"       % "valve open duration, phase 2"  << endl;
          os << "# --- Ascent/Profile - General ---------------------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "ASN"  % asn()       % "[-]"         % ""  << endl;
          os << lpre << format(fmt_f3)    % midx++  % "ASI"  % asi_sec()   % "[sec]"       % "sleep interval"  << endl;
          os << lpre << format(fmt_f2)    % midx++  % "ARR"  % arr_mps()   % "[m/s]"       % "target ascent rate"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "APM"  % apm()       % "[sec]"       % "maximum pump duration"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "APS"  % aps()       % "[sec]"       % "initial pump on duration"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "APN"  % apn()       % "[sec]"       % "pump on duration"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "APF"  % apf()       % "[sec]"       % "pump off duration"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ASD"  % asd()       % "[dBar]"      % "surface pressure threshold"  << endl;
          os << lpre << format(fmt_f1)    % midx++  % "ARC"  % arc_sec()   % "[sec]"       % "sample delay correction"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "AHD"  % ahd()       % "[dBar]"      % "pressure halt threshold"  << endl;
          os << lpre << format(fmt_f3)    % midx++  % "AHR"  % ahr_mps()   % "[m/s]"       % "rise rate halt threshold"  << endl;
          os << "# --- Ascent/Profile - Rise Time-Series ------------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "ADD"  % add()       % "[dBar]"      % "maximum pressure interval"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ADT"  % adt()       % "[sec]"       % "maximum time interval"      << endl;
          os << "# --- Ascent/Profile - Ice-Detection ---------------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICM"  % icm()       % "[-]"         % "ice-detection mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "IMM"  % imm()       % "[-]"         % "month mask"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICN"  % icn()       % "[dBar]"      % "temperature test lower pressure boundary"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICX"  % icx()       % "[dBar]"      % "temperature test upper pressure boundary"  << endl;
          os << lpre << format(fmt_f2)    % midx++  % "ICT"  % ict_degc()  % "[degC]"      % "threshold temperature"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICS"  % ics()       % "[sec]"       % "temperature test sample interval"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ISP"  % isp()       % "[-]"         % "multiple surfacing profile mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ISM"  % ism()       % "[-]"         % "surfacing criteria mode"  << endl;
          os << lpre << format(fmt_f2)    % midx++  % "IST"  % ist_psu()   % "[PSU]"       % "surfacing salinity threshold"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ISO"  % iso()       % "[sec]"       % "surfacing attempt duration"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICF"  % icf()       % "[-]"         % "ice-free month mask"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICW"  % icw()       % "[-]"         % "iced-over month mask"  << endl;
          os << "# --- Ascent/Profile - Ice-Detection (Summer) ------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "ISS0" % iss0()      % "[-]"         % "multiple surfacing mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICA0" % ica0()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICR0" % icr0()      % "[min]"       % "hold pressure duration"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICD0" % icd0()      % "[dBar]"      % "turnaround hold pressure"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICO0" % ico0()      % "[sec]"       % "turnaround valve duration"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICG0" % icg0()      % "[-]"         % "turnaround seek mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "IUE0" % iue0()      % "[-]"         % "urgency mode exit criteria"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICB0" % icb0()      % "[cycles]"    % "multiple surfacing cycle interval"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "IMT0" % imt0()      % "[%]"         % "storage urgency threshold"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ILT0" % ilt0()      % "[days]"      % "last comms urgency threshold"  << endl;
          os << "# --- Ascent/Profile - Ice-Detection (Winter) ------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "ISS1" % iss1()      % "[-]"         % "multiple surfacing mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICA1" % ica1()      % "[attempts]"  % "maximum surfacing attempts"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICR1" % icr1()      % "[min]"       % "hold pressure duration"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICD1" % icd1()      % "[dBar]"      % "turnaround hold pressure"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICO1" % ico1()      % "[sec]"       % "turnaround valve duration"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICG1" % icg1()      % "[-]"         % "turnaround seek mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "IUE1" % iue1()      % "[-]"         % "urgency mode exit criteria"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ICB1" % icb1()      % "[cycles]"    % "multiple surfacing cycle interval" << endl;
          os << lpre << format(fmt_d)     % midx++  % "IMT1" % imt1()      % "[%]"         % "storage urgency threshold"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "ILT1" % ilt1()      % "[days]"      % "last comms urgency threshold"  << endl;
          os << "# --- Surface - General ----------------------------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "SFN"  % sfn()       % "[-]"         % ""  << endl;
          os << lpre << format(fmt_f3)    % midx++  % "SSI"  % ssi_sec()   % "[sec]"       % "sleep interval"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "SDC"  % sdc()       % "[dBar]"      % "surface pressure adjustment"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "SDT"  % sdt()       % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)     % midx++  % "SSA"  % ssa()       % "[-]"         % ""  << endl;
          os << lpre << format(fmt_d)     % midx++  % "SSP"  % ssp()       % "[sec]"       % "oil pump run duration at surface" << endl;
          os << "# --- Surface - Air Pump ---------------------------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "SAM"  % sam()       % "[-]"         % "air pump mode"  << endl;
          os << lpre << format(fmt_f3)    % midx++  % "SAN"  % san_psi()   % "[PSI]"       % "minimum air bladder pressure"  << endl;
          os << lpre << format(fmt_f3)    % midx++  % "SAX"  % sax_psi()   % "[PSI]"       % "maximum air bladder pressure"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "SAO"  % sao()       % "[sec]"       % "air pump duration"  << endl;
          os << "# --- Surface - Sampling ---------------------------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "SDS"  % sds()       % "[sec]"       % "sample interval"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "SPT"  % spt()       % "[sec]"       % "sampling duration"  << endl;
          os << "# --- Surface - Sat./Comm. -------------------------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "SGT"  % sgt()       % "[min]"       % "maximum GPS duration"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "SET"  % set()       % "[min]"       % "maximum Iridium duration"  << endl;
  //        os << "# --- Surface - Wave-Field Measurement -------------------------------------------------------------"            << endl;
  //        os << str_h1 << endl << str_h2 << endl;
  //        os << lpre << format(fmt_d)     % midx++  % "MAM"  % mam()       % "[-]"         % "measurement mode mask"  << endl;
  //        os << lpre << format(fmt_d)     % midx++  % "MAP"  % map()       % "[sec]"       % "measurement duration"  << endl;
  //        os << lpre << format(fmt_d)     % midx++  % "MAT1" % mat1()      % "[intervals]" % "measurement intervals"  << endl;
  //        os << lpre << format(fmt_d)     % midx++  % "MAT2" % mat2()      % "[sec]"       % "measurement interval duration"  << endl;
  //        os << lpre << format(fmt_d)     % midx++  % "ACA"  % aca()       % "[g/FSR]"     % "accelerometer sensitivity"  << endl;
  //        os << lpre << format(fmt_d)     % midx++  % "ACG"  % acg()       % "[dps/FSR]"   % "gyroscope sensitivity"  << endl;
  //        os << lpre << format(fmt_d)     % midx++  % "ACR"  % acr()       % "[Hz]"        % "sample rate"  << endl;
  //        os << lpre << format(fmt_d)     % midx++  % "WST"  % wmt()       % "[sec]"       % "WIMS timeout duration"  << endl;
          os << "# --- Surface - Abort/Beacon Mode ------------------------------------------------------------------"            << endl;
          os << str_h1 << endl << str_h2 << endl;
          os << lpre << format(fmt_d)     % midx++  % "SBM"  % sbm()       % "[-]"         % "beacon mode"  << endl;
          os << lpre << format(fmt_d)     % midx++  % "SBT"  % sbt()       % "[sec]"       % "beacon interval"  << endl;

        }
        break;
      }

  }CATCH_BLOCK("Failed to generate listing string for ALAMO mission parameters object!")

  return os.str();
}


} /* namespace xmsg */
} /* namespace util */
