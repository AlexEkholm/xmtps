/** The @ref util::xmsg::XWaveSpectralSample class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jun 19, 2019 1:18:26 PM
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

#ifndef DATA_XMSG_COMMON_XWAVESPECTRALSAMPLE_H_
#define DATA_XMSG_COMMON_XWAVESPECTRALSAMPLE_H_

/* Includes */

// local
#include "XDefinitions.h"

/* Definitions: */

namespace util {
namespace xmsg {

/* Types: */

/** The wave spectral sample class.
 *
 *  An instance of this class represents the acceleration spectra and co-/quadrature spectra at a
 *  single frequency, decoded from a wave X-message (see @ref XWave).
 */
class XWaveSpectralSample {

    /* Friends: */

    friend class XWave;

  private:

    static const double SPECTRA_FILL_VALUE;

    static const double SPECTRA_M2_MAX;

    static const double SPECTRA_M2_MIN;

  private:

    float freq_;
    int32_t c_xx_;
    int32_t c_xy_;
    int32_t c_yy_;
    int32_t c_zz_;
    int32_t q_zx_;
    int32_t q_zy_;

  public:

    XWaveSpectralSample() ATTR_NO_EXCEPTIONS :
    freq_(-99),
    c_xx_(INT32_MIN),
    c_xy_(INT32_MIN),
    c_yy_(INT32_MIN),
    c_zz_(INT32_MIN),
    q_zx_(INT32_MIN),
    q_zy_(INT32_MIN) {
      /* Do nothing. */
    }

    ~XWaveSpectralSample() {
      /* Do nothing. */
    }

  protected:

    template<typename T = double> T c_xx_g2hz(T scale) const {
      if(c_xx_ == INT32_MIN) return XWaveSpectralSample::SPECTRA_FILL_VALUE;
//      if(!scale)             return XWaveSpectralSample::SPECTRA_FILL_VALUE;
      return ((T) c_xx_ / (T) scale);
    }

    template<typename T = double> T c_xy_g2hz(T scale) const {
      if(c_xy_ == INT32_MIN) return XWaveSpectralSample::SPECTRA_FILL_VALUE;
//      if(!scale)             return XWaveSpectralSample::SPECTRA_FILL_VALUE;
      return ((T) c_xy_ / (T) scale);
    }

    template<typename T = double> T c_yy_g2hz(T scale) const {
      if(c_yy_ == INT32_MIN) return XWaveSpectralSample::SPECTRA_FILL_VALUE;
//      if(!scale)             return XWaveSpectralSample::SPECTRA_FILL_VALUE;
      return ((T) c_yy_ / (T) scale);
    }

    template<typename T = double> T c_zz_g2hz(T scale) const {
      if(c_zz_ == INT32_MIN) return XWaveSpectralSample::SPECTRA_FILL_VALUE;
//      if(!scale)             return XWaveSpectralSample::SPECTRA_FILL_VALUE;
      return ((T) c_zz_ / (T) scale);
    }

    template<typename T = double> T q_zx_g2hz(T scale) const {
      if(q_zx_ == INT32_MIN) return XWaveSpectralSample::SPECTRA_FILL_VALUE;
//      if(!scale)             return XWaveSpectralSample::SPECTRA_FILL_VALUE;
      return ((T) q_zx_ / (T) scale);
    }

    template<typename T = double> T q_zy_g2hz(T scale) const {
      if(q_zy_ == INT32_MIN) return XWaveSpectralSample::SPECTRA_FILL_VALUE;
//      if(!scale)             return XWaveSpectralSample::SPECTRA_FILL_VALUE;
      return ((T) q_zy_ / (T) scale);
    }

    template<typename T = double> T c_xx_m2hz(T scale, T freq) const {

      const T g_squared = (T) 9.80665 * 9.80665;
      T two_pi_f;
      T tmp;

      if(c_xx_ == INT32_MIN) return SPECTRA_FILL_VALUE;
//      if(scale == 0)         return SPECTRA_FILL_VALUE;
      if(freq <= 0)          return XWaveSpectralSample::SPECTRA_FILL_VALUE;    // DC bin

      two_pi_f = (T) 2 * 3.14159265359 * freq;
      tmp = g_squared * c_xx_g2hz(scale) / two_pi_f / two_pi_f / two_pi_f / two_pi_f;

      tmp = (tmp > SPECTRA_M2_MAX ? SPECTRA_M2_MAX : tmp);

      return tmp;
    }

    template<typename T = double> T c_xy_m2hz(T scale, T freq) const {

      const T g_squared = (T) 9.80665 * 9.80665;
      T two_pi_f;
      T tmp;

      if(c_xy_ == INT32_MIN) return XWaveSpectralSample::SPECTRA_FILL_VALUE;
//      if(!scale)             return XWaveSpectralSample::SPECTRA_FILL_VALUE;
      if(freq <= 0)          return XWaveSpectralSample::SPECTRA_FILL_VALUE;    // DC bin

      two_pi_f = (T) 2 * 3.14159265359 * freq;
      tmp = g_squared * c_xy_g2hz(scale) / two_pi_f / two_pi_f / two_pi_f / two_pi_f;

      tmp = (tmp > SPECTRA_M2_MAX ? SPECTRA_M2_MAX : tmp);
      tmp = (tmp < SPECTRA_M2_MIN ? SPECTRA_M2_MIN : tmp);

      return tmp;
    }

    template<typename T = double> T c_yy_m2hz(T scale,T freq) const {

      const T g_squared = (T) 9.80665 * 9.80665;
      T two_pi_f;
      T tmp;

      if(c_yy_ == INT32_MIN) return XWaveSpectralSample::SPECTRA_FILL_VALUE;
//      if(!scale)             return XWaveSpectralSample::SPECTRA_FILL_VALUE;
      if(freq <= 0)          return XWaveSpectralSample::SPECTRA_FILL_VALUE;    // DC bin

      two_pi_f = (T) 2 * 3.14159265359 * freq;
      tmp = g_squared * c_yy_g2hz(scale) / two_pi_f / two_pi_f / two_pi_f / two_pi_f;

      tmp = (tmp > SPECTRA_M2_MAX ? SPECTRA_M2_MAX : tmp);

      return tmp;
    }

    template<typename T = double> T c_zz_m2hz(T scale,T freq) const {

      const T g_squared = (T) 9.80665 * 9.80665;
      T two_pi_f;
      T tmp;

      if(c_zz_ == INT32_MIN) return XWaveSpectralSample::SPECTRA_FILL_VALUE;
//      if(!scale)             return XWaveSpectralSample::SPECTRA_FILL_VALUE;
      if(freq <= 0)          return XWaveSpectralSample::SPECTRA_FILL_VALUE;    // DC bin

      two_pi_f = (T) 2 * 3.14159265359 * freq;
      tmp = g_squared * c_zz_g2hz(scale) / two_pi_f / two_pi_f / two_pi_f / two_pi_f;

      tmp = (tmp > SPECTRA_M2_MAX ? SPECTRA_M2_MAX : tmp);

      return tmp;
    }

    template<typename T = double> T q_zx_m2hz(T scale,T freq) const {

      const T g_squared = (T) 9.80665 * 9.80665;
      T two_pi_f;
      T tmp;

      if(q_zx_ == INT32_MIN) return XWaveSpectralSample::SPECTRA_FILL_VALUE;
//      if(!scale)             return XWaveSpectralSample::SPECTRA_FILL_VALUE;
      if(freq <= 0)          return XWaveSpectralSample::SPECTRA_FILL_VALUE;    // DC bin

      two_pi_f = (T) 2 * 3.14159265359 * freq;
      tmp = g_squared * q_zx_g2hz(scale) / two_pi_f / two_pi_f / two_pi_f / two_pi_f;

      tmp = (tmp > SPECTRA_M2_MAX ? SPECTRA_M2_MAX : tmp);
      tmp = (tmp < SPECTRA_M2_MIN ? SPECTRA_M2_MIN : tmp);

      return tmp;
    }

    template<typename T = double> T q_zy_m2hz(T scale,T freq) const {

      const T g_squared = (T) 9.80665 * 9.80665;
      T two_pi_f;
      T tmp;

      if(q_zy_ == INT32_MIN) return XWaveSpectralSample::SPECTRA_FILL_VALUE;
//      if(!scale)             return XWaveSpectralSample::SPECTRA_FILL_VALUE;
      if(freq <= 0)          return XWaveSpectralSample::SPECTRA_FILL_VALUE;    // DC bin

      two_pi_f = (T) 2 * 3.14159265359 * freq;
      tmp = g_squared * q_zy_g2hz(scale) / two_pi_f / two_pi_f / two_pi_f / two_pi_f;

      tmp = (tmp > SPECTRA_M2_MAX ? SPECTRA_M2_MAX : tmp);
      tmp = (tmp < SPECTRA_M2_MIN ? SPECTRA_M2_MIN : tmp);

      return tmp;
    }
};

/* Functions: */

} /* namespace xmsg */
} /* namespace util */

#endif /* DATA_XMSG_COMMON_XWAVESPECTRALSAMPLE_H_ */
