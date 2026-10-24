/** The @ref util::xmsg::XWave class source file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jun 12, 2014 9:23:09 AM
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


/* Includes  */

// header
#include "XWave.h"

namespace util {
namespace xmsg {

/* Static Variables */

const unsigned int XWave::ASSF_RAW[] = { 16384, 8192, 4096, 2048 };
const unsigned int XWave::ASSF_SPECTRAL_V1[] = { 8192, 2048, 512, 128 };
//const unsigned int XWave::ASSF_SPECTRAL_V2[] = { 4096, 1024, 256, 64 };
const unsigned int XWave::ASSF_SPECTRAL_V2[] =  { 8192, 2048, 512, 128 };
const double XWave::FSAM[] = { 1 };
const double XWave::GSSF[] = { 131.072, 65.536, 32.768, 16.384 };
const double XWave::MSSF[] = { (30.0 / 9.0) };


/* Function Definitions */

/** Generate a listing entry string from this ocean surface wave-field data message.
 *
 *  @return         A listing entry string from this ocean surface wave-field data message.
 */
const string XWave::GenerateListingString() const {
  ostringstream os;

  try {

    switch(version_) {

      case 1:
      case 2:
        switch(sample_mode_) {
          case XWAVE_MODE_RAW: {    // Print raw wave data.
    //        size_t sam = 0;
    //
    //        /* Print raw wave data listing comment: */
    //
    //        { static const format fmt(
    //                  "#   %5s %4s %3s %4s "
    //                  "%4s %4s %6s "
    //                  "%12s %12s %12s "
    //                  "%9s %9s %9s "
    //                  "%14s %14s %14s\n"
    //                  "#   %5s %4s %3s %4s "
    //                  "%4s %4s %6s "
    //                  "%12s %12s %12s "
    //                  "%10s %10s %10s "
    //                  "%13s %13s %13s");
    //          os << format(fmt)
    //                 % "SN"   % "dive"  % "idx"  % "code"
    //                 % "mode"  % "nSam"  % "fSam"
    //                 % "a_x"   % "a_y"   % "a_z"
    //                 % "B_x"   % "B_y"   % "B_z"
    //                 % "ω_γ"   % "ω_β"   % "ω_α"
    //                 % ""      % ""      % ""     % ""
    //                 % ""      % ""      % "(Hz)"
    //                 % "(g)"   % "(g)"   % "(g)"
    //                 % "(μT)"  % "(μT)"  % "(μT)"
    //                 % "(°/s)" % "(°/s)" % "(°/s)"
    //             << endl;
    //        }
    //
    //        /* Print header line: */
    //
    //        { static const format fmt("WAV %5d %4d %3d %4d %4d %4d %6.3f");
    //          os << format(fmt) % id_instrument() % id_dive() % (int) xmsg_index() % (int) phase_code_
    //              % (int) sample_mode_ % wave_data_raw_.size() % sample_rate_effective_;
    //        }
    //
    //        /* Print sample lines: */
    //
    //        { static const format fmt("wav %5d %4d %3d %4d %4d %4d %6.3f ");
    //
    //          /* Iterate over raw samples: */
    //
    //          for(vector<XWaveRawSample>::const_iterator it = wave_data_raw_.begin(); it < wave_data_raw_.end(); ++it) {
    //
    //            /* Print sample line: */
    //
    //            os << endl << format(fmt) % id_instrument() % id_dive() % (int) xmsg_index()
    //                    % (int) phase_code_ % (int) sample_mode_ % sam++ % sample_rate_effective_
    //                << it->GenerateListingString(assf_, gssf_, mssf_);
    //          }
    //        }
          }

            break;

          case XWAVE_MODE_SPECTRAL: {

            unsigned int essf = effective_ssf();

            { /* Print header lines: */

              static const format fmt( "WVS %5d %4d 0x%02X %4d %8.4f %4d %5d %8d %6.3f %8d" );
              os << "#      SN dive vers slen     fsam fbase fdiv     assf  again     essf" << endl;
              os << "#                            (Hz)  (Hz)      (LSB/g^2)       (LSB/g^2)" << endl;
              os << format(fmt) % id_instrument() % id_dive() % (int)version_ % segment_len()
                  % sample_rate_effective_ % sample_rate_base_ % (sample_rate_div_+1) % assf_
                  % gain_accel() % essf << endl;
              os << "#---------------------------------------------------------------------------------------------------" << endl;
              os << "#      SN dive  idx     freq         Szz         Syy         Sxx         Cxy         Qzx         Qzy" << endl;
              os << "#                       (Hz)    (g^2/Hz)    (g^2/Hz)    (g^2/Hz)    (g^2/Hz)    (g^2/Hz)    (g^2/Hz)";
            }

            /* Iterate over spectral samples: */

            unsigned int idx = 0;
            for(vector<XWaveSpectralSample>::const_iterator it = wave_data_spectral_.begin();
                it < wave_data_spectral_.end(); ++it) {

              /* Print sample line: */

              static const format fmt(
                  "wvs %5d %4d %4d %8.4f"
                  " %+11.5f %+11.5f %+11.5f %+11.5f %+11.5f %+11.5f");
                os << endl << format(fmt)
                   % id_instrument()  % id_dive() % idx++ % it->freq_
                   % it->c_zz_g2hz((double)essf)
                   % it->c_yy_g2hz((double)essf)
                   % it->c_xx_g2hz((double)essf)
                   % it->c_xy_g2hz((double)essf)
                   % it->q_zx_g2hz((double)essf)
                   % it->q_zy_g2hz((double)essf);
            }

             /* Print header lines: */

             os << endl
                 << "#---------------------------------------------------------------------------------------------------"
                 << endl
                 << "#      SN dive  idx     freq         Szz         Syy         Sxx         Cxy         Qzx         Qzy"
                 << endl
                 << "#                       (Hz)    (m^2/Hz)    (m^2/Hz)    (m^2/Hz)    (m^2/Hz)    (m^2/Hz)    (m^2/Hz)";


             /* Iterate over spectral samples: */

             idx = 0;
             for(vector<XWaveSpectralSample>::const_iterator it = wave_data_spectral_.begin();
                 it < wave_data_spectral_.end(); ++it) {

               /* Print sample line: */

               static const format fmt(
                   "wvm %5d %4d %4d %8.4f"
                   " %+11.4f %+11.4f %+11.4f %+11.4f %+11.4f %+11.4f");
                 os << endl << format(fmt)
                    % id_instrument()  % id_dive() % idx++ % it->freq_
                    % it->c_zz_m2hz((double)essf, (double)it->freq_)
                    % it->c_yy_m2hz((double)essf, (double)it->freq_)
                    % it->c_xx_m2hz((double)essf, (double)it->freq_)
                    % it->c_xy_m2hz((double)essf, (double)it->freq_)
                    % it->q_zx_m2hz((double)essf, (double)it->freq_)
                    % it->q_zy_m2hz((double)essf, (double)it->freq_);
             }
          }



            break;

          default:

            EXC_THROW("Invalid surface wave mode encountered!");

            break;

        }
      break;
    }

  }CATCH_BLOCK("Failed to generate listing string from ocean surface wave data object!")
  return os.str();
}

/** Decode the wave data message.
 *
 *  @param[in]      data            The byte array to decode.
 *
 *  @throw          log::MessageException on error.
 */
void XWave::DecodeXMsgData(const vector<uint8_t>& data) {

  try {

    // Check for valid X message ID, size, & terminator.
    const uint8_t* d = CheckXMsgData(data, -1, 17);

    // Get packet index.
//    set_xmsg_index(d[3]);

    /* Parse header */
    version_ = d[3];


    switch(version_) {
      case 0:
      case 1:

        sample_mode_ = XWAVE_MODE_SPECTRAL;

//        { uint8_t m = d[5];     //  Get mode.
//          if(m > 1) EXC_THROW("Invalid sample mode!");
//          sample_mode_ = static_cast<XWaveMode_t>(m);
//        }

        XWaveSpectralAxis_t axis;
        XWaveSpectralType_t type;
        uint16_t first_sample_idx;
        int32_t sample;

        axis = static_cast<XWaveSpectralAxis_t>(d[6]);
        type = static_cast<XWaveSpectralType_t>(d[7]);
        set_segment_len_pow(u08(d,8));
        sample_rate_base_ = (u16(d, 9));
        sample_rate_div_  = (u16(d,11));
        sample_rate_effective_ = (float)sample_rate_base_ / (sample_rate_div_ + 1);

        // Get accelerometer sensitivity.
        switch (d[13]) {
          case  2: assf_ = ASSF_SPECTRAL_V1[0]; break;
          case  4: assf_ = ASSF_SPECTRAL_V1[1]; break;
          case  8: assf_ = ASSF_SPECTRAL_V1[2]; break;
          case 16: assf_ = ASSF_SPECTRAL_V1[3]; break;
          default:
//            EXC_THROW("Invalid accelerometer sensitivity scale factor!");
            lerr() << "Invalid accelerometer sensitivity scale factor: " << d[13];
            assf_ = (int)ASSF_SPECTRAL_V1[1];
            break;
        }
        dbg1() << "accl. gain: " << assf_;

        first_sample_idx = u16(d,14);

        /* Reserve sample space: */

        if(wave_data_spectral_.size() < (unsigned int)(segment_len() >> 1))
          wave_data_spectral_.resize(segment_len() >> 1);

        /* Iterate over spectral samples: */

        for(unsigned int i = 0; i < (unsigned int)(segment_len() >> 1); ++i) {

          if((unsigned int)(i*2+17) >= (unsigned int)xmsg_size_bytes()) {
            lerr() << "wave sample index exceeds message size";
            break;
          }

          wave_data_spectral_.at(i+first_sample_idx).freq_
              = sample_rate_effective_*(i+first_sample_idx+1)/segment_len();

          switch(type) {
            case XWAVE_SPECTRAL_TYPE_UNDEFINED:

              EXC_THROW("Undefined spectral type!");
              break;

            case XWAVE_SPECTRAL_TYPE_CO:
              switch (axis) {
                case XWAVE_SPECTRAL_AXIS_ZZ:
                  sample = u16(d, i*2+16);
                  wave_data_spectral_.at(i+first_sample_idx).c_zz_ = sample;
                  break;

                case XWAVE_SPECTRAL_AXIS_XX:
                  sample = u16(d, i*2+16);
                  wave_data_spectral_.at(i+first_sample_idx).c_xx_ = sample;
                  break;

                case XWAVE_SPECTRAL_AXIS_YY:
                  sample = u16(d, i*2+16);
                  wave_data_spectral_.at(i+first_sample_idx).c_yy_ = sample;
                  break;

                case XWAVE_SPECTRAL_AXIS_XY:
                  sample = i16(d, i*2+16);
                  wave_data_spectral_.at(i+first_sample_idx).c_xy_ = sample;
                  break;

                default:
                  EXC_THROW("Unsupported axis for co-spectral density!");
                  break;
              }

              ldbg() << "sample " << setw(3) << i
                  << " (idx: " << setw(3) << (i*2+16) << ")"
                  << ": " << setw(6) << sample;
              break;

            case XWAVE_SPECTRAL_TYPE_QU:
              switch (axis) {
                case XWAVE_SPECTRAL_AXIS_ZX:
                  sample = i16(d, i*2+16);
                  wave_data_spectral_.at(i+first_sample_idx).q_zx_ = sample;
                  break;

                case XWAVE_SPECTRAL_AXIS_ZY:
                  sample = i16(d, i*2+16);
                  wave_data_spectral_.at(i+first_sample_idx).q_zy_ = sample;
                  break;

                default:
                  EXC_THROW("Unsupported axis for quadrature-spectral density!");
                  break;
              }

              ldbg() << "sample " << setw(5) << i << " (idx: " << (i*2+16) << ")" << ": " << sample;
              break;

            case XWAVE_SPECTRAL_TYPE_RE:
              EXC_THROW("Spectral type not supported!");

              break;

            case XWAVE_SPECTRAL_TYPE_IM:
              EXC_THROW("Spectral type not supported!");

              break;

            case XWAVE_SPECTRAL_TYPE_MAG:
              EXC_THROW("Spectral type not supported!");

              break;

            case XWAVE_SPECTRAL_TYPE_PHASE:
              EXC_THROW("Spectral type not supported!");

              break;

            default:
              EXC_THROW("Unknown spectral type!");
              break;
          }
        }

        ldbg() << "end deco wave";

        break;

      case 2:

        { uint8_t m = d[5];     //  Get mode.
          if(m > 1) {
            sample_mode_ = XWAVE_MODE_UNKNOWN;
            EXC_THROW("Invalid sample mode!");
          }
          sample_mode_ = static_cast<XWaveMode_t>(m);
        }

        switch(sample_mode_) {

          case XWAVE_MODE_RAW:{

//              // Get sample rate.
//              if(d[4] > XWAVE_SIZE_FSAM) EXC_THROW("Invalid sample rate!");
//              sample_rate_effective_ = FSAM[d[4]];
//              dbg1() << "sample rate: " << sample_rate_effective_;
//
//              // Get accelerometer sensitivity.
//              if(d[5] > XWAVE_SIZE_ASSF_RAW) EXC_THROW("Invalid accelerometer sensitivity scale factor!");
//              assf_ = ASSF_RAW[d[5]];
//              dbg1() << "accl. gain: " << assf_;
//
//              // Get gyroscope sensitivity.
//              if(d[6] > XWAVE_SIZE_GSSF) EXC_THROW("Invalid gyroscope sensitivity scale factor!");
//              gssf_ = GSSF[d[6]];
//              dbg1() << "gyro. gain: " << gssf_;
//
//              // Get magnetometer sensitivity.
//              if(d[7] > XWAVE_SIZE_MSSF)  EXC_THROW("Invalid magnetometer sensitivity scale factor!");
//              mssf_ = MSSF[d[7]];
//              dbg1() << "magn. gain: " << mssf_;
//
//              { // Get phase code
//                uint8_t c = (d[8] & 0x0F);
//                if(c < 1 || c > 14)
//                  EXC_THROW("Invalid phase code!");
//                phase_code_ = static_cast<XPhaseCode_t>(c);
//              }
//
//              // Iterate over raw motion samples.
//              for(size_t i = 0; 18 * i + 26 < xmsg_size_bytes(); ++i) {
//
//                XWaveRawSample wdata;
//
//                for(size_t j = 0; j < 3; j++) {
//                  wdata.accel[j] = i16(d, 18 * i + 2 * j + 9);
//                  wdata.omega[j] = i16(d, 18 * i + 2 * j + 15);
//                  wdata.magnt[j] = i16(d, 18 * i + 2 * j + 21);
//                }
//
//                wave_data_raw_.push_back(wdata);
//              }
          }

            break;

          case XWAVE_MODE_SPECTRAL: {

             XWaveSpectralAxis_t axis;
             XWaveSpectralType_t type;
             uint16_t first_sample_idx;
             int32_t sample;

             if(xmsg_size_bytes() < 19)
               EXC_THROW("Spectral wave message too short for its header!");

             axis = static_cast<XWaveSpectralAxis_t>(d[6]);
             type = static_cast<XWaveSpectralType_t>(d[7]);
             set_segment_len_pow(u08(d,8));
             sample_rate_base_ = (u16(d, 9));
             sample_rate_div_  = (u16(d,11));
             sample_rate_effective_ = (float)sample_rate_base_ / (sample_rate_div_ + 1);

             // Get accelerometer sensitivity.
             switch (d[13]) {
               case  2: assf_ = ASSF_SPECTRAL_V2[0]; break;
               case  4: assf_ = ASSF_SPECTRAL_V2[1]; break;
               case  8: assf_ = ASSF_SPECTRAL_V2[2]; break;
               case 16: assf_ = ASSF_SPECTRAL_V2[3]; break;
               default:
     //            EXC_THROW("Invalid accelerometer sensitivity scale factor!");
                 lwrn() << "Invalid accelerometer sensitivity scale factor: " << d[13];
                 assf_ = (int)ASSF_SPECTRAL_V2[1];
                 break;
             }
             dbg1() << "accl. gain: " << assf_;

             gain_accel_q3_ = u16(d,14);

             first_sample_idx = u16(d,16);

             /* Reserve sample space: */

             if(wave_data_spectral_.size() < (unsigned int)(segment_len() >> 1))
               wave_data_spectral_.resize(segment_len() >> 1);

             /* Iterate over spectral samples: */

             for(unsigned int i = 0; i < (unsigned int)(segment_len() >> 1); ++i) {

               if((unsigned int)(i*2+20) >= (unsigned int)xmsg_size_bytes()) {
                 lerr() << "wave sample index exceeds message size";
                 break;
               }

               wave_data_spectral_.at(i+first_sample_idx).freq_
                   = sample_rate_effective_*(i+first_sample_idx+1)/segment_len();

               switch(type) {
                 case XWAVE_SPECTRAL_TYPE_UNDEFINED:

                   EXC_THROW("Undefined spectral type!");
                   break;

                 case XWAVE_SPECTRAL_TYPE_CO:
                   switch (axis) {
                     case XWAVE_SPECTRAL_AXIS_ZZ:
                       sample = u16(d, i*2+18);
                       wave_data_spectral_.at(i+first_sample_idx).c_zz_ = sample;
                       break;

                     case XWAVE_SPECTRAL_AXIS_XX:
                       sample = u16(d, i*2+18);
                       wave_data_spectral_.at(i+first_sample_idx).c_xx_ = sample;
                       break;

                     case XWAVE_SPECTRAL_AXIS_YY:
                       sample = u16(d, i*2+18);
                       wave_data_spectral_.at(i+first_sample_idx).c_yy_ = sample;
                       break;

                     case XWAVE_SPECTRAL_AXIS_XY:
                       sample = i16(d, i*2+18);
                       wave_data_spectral_.at(i+first_sample_idx).c_xy_ = sample;
                       break;

                     default:
                       EXC_THROW("Unsupported axis for co-spectral density!");
                       break;
                   }

                   ldbg() << "sample " << setw(3) << i
                       << " (idx: " << setw(3) << (i*2+18) << ")"
                       << ": " << setw(6) << sample;
                   break;

                 case XWAVE_SPECTRAL_TYPE_QU:
                   switch (axis) {
                     case XWAVE_SPECTRAL_AXIS_ZX:
                       sample = i16(d, i*2+18);
                       wave_data_spectral_.at(i+first_sample_idx).q_zx_ = sample;
                       break;

                     case XWAVE_SPECTRAL_AXIS_ZY:
                       sample = i16(d, i*2+18);
                       wave_data_spectral_.at(i+first_sample_idx).q_zy_ = sample;
                       break;

                     default:
                       EXC_THROW("Unsupported axis for quadrature-spectral density!");
                       break;
                   }

                   ldbg() << "sample " << setw(5) << i << " (idx: " << (i*2+18) << ")" << ": " << sample;
                   break;

                 case XWAVE_SPECTRAL_TYPE_RE:
                   EXC_THROW("Spectral type not supported!");

                   break;

                 case XWAVE_SPECTRAL_TYPE_IM:
                   EXC_THROW("Spectral type not supported!");

                   break;

                 case XWAVE_SPECTRAL_TYPE_MAG:
                   EXC_THROW("Spectral type not supported!");

                   break;

                 case XWAVE_SPECTRAL_TYPE_PHASE:
                   EXC_THROW("Spectral type not supported!");

                   break;

                 default:
                   EXC_THROW("Unknown spectral type!");
                   break;
               }
             }

             ldbg() << "end deco wave";

          }

            break;

          default:

            break;

        }

        break;

      default:

        break;


    }




  }CATCH_BLOCK("Failed to decode XWave message!")
}

} /* namespace xmsg */
} /* namespace util */
