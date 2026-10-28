/** The @ref util::xmsg::XWave class header file.
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

#ifndef XMOTION_H_
#define XMOTION_H_

/* Includes */

// local
#include "XMessageIndexable.h"

/* Definitions */

/** The default/invalid sensitivity scale
factor value for wave data.
 *
 */
#define XWAVE_INVALID_SF DBL_MAX

/** The number of available accelerometer sensitivity scale factors for raw data. */
#define XWAVE_SIZE_ASSF_RAW       (sizeof(util::xmsg::XWave::ASSF_RAW)/sizeof(double))

/** The number of available accelerometer sensitivity scale factors for spectral. */
#define XWAVE_SIZE_ASSF_SPECTRAL  (sizeof(util::xmsg::XWave::ASSF_SPECTRAL)/sizeof(double))

/** The number of available sampling frequencies. */
#define XWAVE_SIZE_FSAM           (sizeof(util::xmsg::XWave::FSAM)/sizeof(double))

/** The number of available gyroscope sensitivity scale factors. */
#define XWAVE_SIZE_GSSF           (sizeof(util::xmsg::XWave::GSSF)/sizeof(double))

/** The number of available magnetometer sensitivity scale factors. */
#define XWAVE_SIZE_MSSF           (sizeof(util::xmsg::XWave::MSSF)/sizeof(double))

namespace util {
namespace xmsg {

/* Type Definitions */

/** Wave data modes enumeration.
 *
 *  @note           Currently, only raw wave motion measurements are supported.
 *
 */
typedef enum XWaveMode {

    /** Raw data mode. */
    XWAVE_MODE_RAW = 0,

    /** Spectral data mode. */
    XWAVE_MODE_SPECTRAL = 1,

    /** Unknown/invalid data mode. */
    XWAVE_MODE_UNKNOWN = 99

} XWaveMode_t;

typedef enum XWaveSpectralAxis {

  XWAVE_SPECTRAL_AXIS_UNDEFINED = 0,
  XWAVE_SPECTRAL_AXIS_Z_PRIME   = 1,
  XWAVE_SPECTRAL_AXIS_Y_PRIME   = 2,
  XWAVE_SPECTRAL_AXIS_X_PRIME   = 3,
  XWAVE_SPECTRAL_AXIS_Z         = 4,
  XWAVE_SPECTRAL_AXIS_ZZ        = 5,
  XWAVE_SPECTRAL_AXIS_ZX        = 6,
  XWAVE_SPECTRAL_AXIS_ZY        = 7,
  XWAVE_SPECTRAL_AXIS_X         = 8,
  XWAVE_SPECTRAL_AXIS_XZ        = 9,
  XWAVE_SPECTRAL_AXIS_XX        = 10,
  XWAVE_SPECTRAL_AXIS_XY        = 11,
  XWAVE_SPECTRAL_AXIS_Y         = 12,
  XWAVE_SPECTRAL_AXIS_YZ        = 13,
  XWAVE_SPECTRAL_AXIS_YX        = 14,
  XWAVE_SPECTRAL_AXIS_YY        = 15,
  XWAVE_NUM_SPECTRAL_AXES       = 16,

} XWaveSpectralAxis_t;

typedef enum XWaveSpectralType {

  XWAVE_SPECTRAL_TYPE_UNDEFINED = 0,
  XWAVE_SPECTRAL_TYPE_CO        = 1,
  XWAVE_SPECTRAL_TYPE_QU        = 2,
  XWAVE_SPECTRAL_TYPE_RE        = 3,
  XWAVE_SPECTRAL_TYPE_IM        = 4,
  XWAVE_SPECTRAL_TYPE_MAG       = 5,
  XWAVE_SPECTRAL_TYPE_PHASE     = 6,
  XWAVE_NUM_SPECTRAL_TYPES      = 7,

} XWaveSpectralType_t;

/** Class representing measured ocean surface wave data X message.
 *
 */
class XWave: public XMessageIndexable {

private:

    /* Data Members */

    /** Available accelerometer sensitivity scale factors for raw wave data. */
    static const unsigned int ASSF_RAW[];

    /** Available accelerometer sensitivity scale factors for spectral wave data -- message version 1. */
    static const unsigned int ASSF_SPECTRAL_V1[];

    /** Available accelerometer sensitivity scale factors for spectral wave data -- message version 2. */
    static const unsigned int ASSF_SPECTRAL_V2[];

    /** Available ample rate frequencies. */
    static const double FSAM[];

    /** Available gyrscope sensitivity scale factors. */
    static const double GSSF[];

    /** Available magenetometer sensitivity scale factors. */
    static const double MSSF[];

private:

    /* Data Members */

    /** The accelerometer sensitivity scale factor, in LSB/g.
     *
     *  @note           Dividing accelerometer counts by this number produces this measured
     *                  acceleration, in g.
     */
    unsigned int assf_;

    /** Accelerometer software gain.
     *
     */
    uint16_t gain_accel_q3_;

    /** The ocean surface wave field sampling mode.
     *
     *  This field indicates whether this message contains raw wave motion measurements or
     *  spectrally encoded data.
     */
    XWaveMode_t sample_mode_;

    /** The base sample rate, in Hz.
     *
     */
    uint16_t sample_rate_base_;

    /** The sample rate divider, in Hz/Hz.
     *
     */
    uint16_t sample_rate_div_;

    /** The sample rate that sets the spectral bin spacing, in Hz.
     *
     *  This is the effective sample rate of the ocean surface wave measurements, or twice it for
     *  spectra that hold only every other bin (see @ref set_every_other_bin()).
     */
    float sample_rate_effective_;

    /** Whether the spectra hold only every other bin (see @ref set_every_other_bin()). */
    bool every_other_bin_;

    /** The power-of-two segment length for spectral data.
     *
     */
    uint16_t segment_len_;

    /** The power-of-two segment length for spectral data.
     *
     */
    uint16_t segment_len_pow_;

    /**
     *
     */
    uint8_t version_;

    /** The spectral data samples.
     *
     */
    vector<XWaveSpectralSample> wave_data_spectral_;

public:
    /* Member Functions */

    /** The parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        Binary surface pressure X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XWave(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XMessageIndexable(dive, whoi, data),
                    assf_(0),
                    gain_accel_q3_(8),
                    sample_mode_(XWAVE_MODE_UNKNOWN),
                    sample_rate_base_(0),
                    sample_rate_div_(0),
                    sample_rate_effective_(0),
                    every_other_bin_(false),
                    segment_len_(0),
                    version_(0xFF),
                    wave_data_spectral_() {
        DecodeXMsgData(data);    // Decode the wave data message.
    }

    /** The destructor.
     *
     */
    ~XWave() {
        /* Do noting. */
    }

    void DecodeXMsgData(const vector<uint8_t>& data);

    const string GenerateListingString() const;

    /** Get the effective spectral sensitivity scale factor, G_s, in LSB/(g^2/Hz).
     *
     *  G_s = 2^(30 - 2 log2(fsra)) * G_sa^2 * U * f_e^2 / (2 * slen^2), from the WIMS user manual
     *  (rev 3.15, "Scale Sensitivity"). assf_ holds 2^(15 - 2 log2(fsra)). G_sa is the
     *  acceleration software gain, or 1 when it is 0. U is the mean square of the firmware's Tukey
     *  window, 1 - (5 L / 4 + 1) / slen, where L = round((slen - 1) * twalp / 256) is the taper
     *  length, for wav.twalp = 64, which the message header does not include (U = 0.68359 for
     *  slen = 256). f_e is the true effective sample rate, not the doubled rate used for the bin
     *  spacing of every-other-bin spectra. For fsra 4, gaina 32 and slen 256, G_s is 35000 at
     *  f_e = 2.5 Hz and 140000 at f_e = 5 Hz.
     *
     *  @note           Up to 2026-10-27, this was 2^(15 - 2 log2(fsra)) * G_sa^2, the formula of
     *                  manual revisions up to 3.14, which ignores f_e, slen and the window.
     */
    double effective_ssf() const {
      static const unsigned int twalp = 64;
      const unsigned int n = segment_len();
      if(n < 2) return 0.0;                                            // no spectral bins
      const unsigned int taper = ((n - 1) * twalp + 128) / 256;        // L, halves rounded up
      const double u = taper ? 1.0 - (1.25 * taper + 1.0) / n : 1.0;
      const double gsa = gain_accel_q3_ ? gain_accel_q3_ / 8.0 : 1.0;
      const double fe = sample_rate_true();
      return assf_ * 32768.0 * gsa * gsa * u * fe * fe / (2.0 * n * n);
    }

    /** Get the true effective sample rate, srate / (srdiv + 1), in Hz. */
    double sample_rate_true() const {
      return (double)sample_rate_base_ / (sample_rate_div_ + 1);
    }

    /** Set whether the spectra hold only every other bin (MRV's 9000th data encoding fuckup).
     *
     *  For select firmware versions, only every other spectral value is returned. The second half
     *  of the spectra is all zeros. These are fixed by doubling the rate that sets the bin
     *  spacing; the scale factor still uses the true effective sample rate. Calling this again
     *  with the same value has no effect.
     *
     *  @param[in]  every_other_bin Whether the spectra hold only every other bin.
     */
    void set_every_other_bin(bool every_other_bin) {
      every_other_bin_ = every_other_bin;
      sample_rate_effective_ = (every_other_bin_ ? 2.0 : 1.0) * sample_rate_true();

      for(unsigned int i = 0; i < wave_data_spectral_.size(); ++i) {

        if(wave_data_spectral_[i].freq_ >= 0)             // bins not received keep their fill value
          wave_data_spectral_[i].freq_
              = sample_rate_effective_*i/segment_len();   // bin 0 is the DC bin
      }
    }

    /** Check whether the spectra look like they hold only every other bin.
     *
     *  For floats whose firmware version is not known, the every-other-bin encoding is recognized
     *  from the data: every bin in the upper half of the spectra has been received and is zero in
     *  all six components, and some bin in the lower half has a nonzero vertical auto-spectrum.
     *  Spectra that are truncated to a range of frequencies leave the upper bins unreceived, not
     *  zero, so they do not match.
     *
     *  @return     Whether the spectra look like they hold only every other bin.
     */
    bool LooksLikeEveryOtherBin() const {
      const unsigned int n = wave_data_spectral_.size();
      if(n < 4) return false;

      for(unsigned int i = n / 2; i < n; ++i) {
        const XWaveSpectralSample& s = wave_data_spectral_[i];
        if(s.c_xx_ || s.c_xy_ || s.c_yy_ || s.c_zz_ || s.q_zx_ || s.q_zy_) return false;
      }
      for(unsigned int i = 1; i < n / 2; ++i) {
        const int32_t c = wave_data_spectral_[i].c_zz_;
        if(c != 0 && c != INT32_MIN) return true;
      }
      return false;
    }

private:

    /** Get the number of spectral bins to decode from this spectral message.
     *
     *  The message holds two-byte bins between its header and its ';' terminator. Newer sensor
     *  firmware truncates the spectrum to a range of frequencies, so this may be fewer than the
     *  N/2 bins of the spectrum. Bins that would lie beyond the spectrum's N/2 bins, counting from
     *  the first bin index, are not decoded, and a warning is logged.
     *
     *  @param[in]  header_len  The length of the message header, in bytes (16 for spectral
     *                          versions 0 and 1, 18 for version 2).
     *  @param[in]  first_bin   The index of the first bin in this message.
     *
     *  @return     The number of bins to decode.
     */
    unsigned int NumSpectralBins(unsigned int header_len, unsigned int first_bin) const {

      const unsigned int size = xmsg_size_bytes();
      const unsigned int num_bins = size > header_len + 1 ? (size - header_len - 1) / 2 : 0;
      const unsigned int spectrum_bins = segment_len() >> 1;
      const unsigned int max_bins = first_bin < spectrum_bins ? spectrum_bins - first_bin : 0;

      if(num_bins > max_bins) {
        lwrn() << "Wave message has " << num_bins << " bins from bin " << first_bin
               << ", but the spectrum has only " << spectrum_bins
               << " bins; the bins beyond it are not decoded.";
        return max_bins;
      }

      return num_bins;
    }

public:

    /* Accessor Methods: */

    float gain_accel() const {
      return (float) gain_accel_q3_ / 8;
    }

    uint16_t segment_len() const {
      return segment_len_;
    }

    uint16_t segment_len_pow() const {
      return segment_len_pow_;
    }

public:

    /* Mutator Methods: */

    void set_segment_len(uint16_t segment_len) {
      segment_len_ = segment_len;
    }

    void set_segment_len_pow(uint16_t segment_len_pow) {
      set_segment_len(1 << (segment_len_pow_ = segment_len_pow));
    }
};

} /* namespace xmsg */
} /* namespace util */

#endif /* XMOTION_H_ */
