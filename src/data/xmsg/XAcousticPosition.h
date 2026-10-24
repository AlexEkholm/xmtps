/** The @ref util::xmsg::XAcousticPosition class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 1, 2014 1:56:04 PM
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

#ifndef XACOUSTICPOSITION_H_
#define XACOUSTICPOSITION_H_

/* Includes */

#include "XMessage.h"

namespace bpt = boost::posix_time;

/* Definitions */

namespace util {
namespace xmsg {

using bpt::ptime;



/* Class Definitions */

/** An acoustic reception (CACST) record.
 *
 *  An instance of this class represents a single acoustic signal received by the instrument,
 *  decoded from an acoustic position message (see @ref XAcousticPosition): its counter, time of
 *  arrival (seconds and microseconds), source identifier, signal power, PSK error code, input
 *  and output signal-to-noise ratios, and the index of the listening window it was received in.
 */
class XAcousticCacst {

    friend class XAcousticPosition;


  private:

    uint32_t counter_;

    /** Time of arrival, in UNIX time
     *
     */
    uint32_t toa_sec_;

    /** Microsecond of arrival time.
     *
     */
    uint32_t toa_usec_;

    /** Matched filter peak value, in dB.
     *
     */
    uint32_t mfd_power_;

    /** Acoustic message source ID.
     *
     */
    int16_t src_;

    /** PSK error code enumeration.
     *
     */
    uint16_t psk_error_code_;

    /** Input SNR, in 1/10 dB.
     *
     */
    int32_t snr_in_;

    /** SNR at output of equalizer, in 1/10 dB.
     *
     */
    int32_t snr_out_;

    int window_idx_;

  public:

    /** Default constructor.
     *
     */
    XAcousticCacst() :
            counter_(UINT32_MAX),
            toa_sec_(UINT32_MAX),
            toa_usec_(UINT32_MAX),
            mfd_power_(0),
            src_(INT16_MIN),
            psk_error_code_(0),
            snr_in_(0),
            snr_out_(0),
            window_idx_(-1) {
      /* Do nothing. */
    }

    /** Construct from message data.
     *
     *  @param[in]      data        The binary message data to decode.
     */
    XAcousticCacst(const vector<uint8_t>& data) : XAcousticCacst()  {
      DecodeXMsgData(data);
    }

    /** Destructor.
     *
     */
    ~XAcousticCacst() {
      /* Do nothing. */
    }

    /** Decode the acoustic position CACST X-Message from the binary message data.
     *
     *  @param[in]      data        The binary message data to decode.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {

          // Check minimum message size.
          static const int EXPECTED_SIZE = 33;
          if((int) data.size() != EXPECTED_SIZE) {
            ostringstream msg;
            msg << "Invalid XMessage: expected " << EXPECTED_SIZE << " bytes, but only received "
                << data.size() << " bytes!";
            BOOST_THROW_EXCEPTION(log::MessageException(msg));
          }

          const uint8_t* d = data.data();

          set_counter(        u32(d,  4));
          set_toa_sec(        u32(d,  8));
          set_toa_usec(       u32(d, 12));
          set_mfd_power(      u32(d, 16));
          set_src(            u16(d, 20));
          set_psk_error_code( u16(d, 22));
          set_snr_in(         u32(d, 24));
          set_snr_out(        u32(d, 28));

        }CATCH_BLOCK("Failed to decode acoustic position CACST X-Message!")
    }

  public:

    /* Helper functions: */

    ptime toa_ptime() const {
      return bpt::from_time_t(toa_sec());
    }


    string toa_iso8601_string() const {

      static const string not_a_datetime_string_extended = "0000-00-00T00:00:00.000000Z";

      ostringstream os;

      ptime p = toa_ptime();

      if(p.is_not_a_date_time() || toa_sec() == UINT32_MAX || toa_usec() > 999999) {
        os << not_a_datetime_string_extended;
      } else {
        os << bpt::to_iso_extended_string(p) << ".";
        os << format("%06dZ") % toa_usec();
      }

      return os.str();
    }

    float snr_in_as_float() {
      return (float) snr_in() / 10.;
    }

    float snr_out_as_float() {
      return (float) snr_out() / 10.;
    }


  public:

    /* Mutators and Accessors: */

    uint32_t counter() const {
      return counter_;
    }

    void set_counter(uint32_t counter) {
      counter_ = counter;
    }

    uint32_t mfd_power() const {
      return mfd_power_;
    }

    void set_mfd_power(uint32_t mfd_power) {
      mfd_power_ = mfd_power;
    }

    uint16_t psk_error_code() const {
      return psk_error_code_;
    }

    void set_psk_error_code(uint16_t psk_error_code) {
      psk_error_code_ = psk_error_code;
    }


    uint32_t toa_sec() const {
      return toa_sec_;
    }

    void set_toa_sec(uint32_t toa_sec) {
      toa_sec_ = toa_sec;
    }

    uint32_t toa_usec() const {
      return toa_usec_;
    }

    void set_toa_usec(uint32_t toa_usec) {
      toa_usec_ = toa_usec;
    }

    int32_t snr_in() const {
      return snr_in_;
    }

    void set_snr_in(int32_t snr_in) {
      snr_in_ = snr_in;
    }

    int32_t snr_out() const {
      return snr_out_;
    }

    void set_snr_out(int32_t snr_out) {
      snr_out_ = snr_out;
    }

    int16_t src() const {
      return src_;
    }

    void set_src(int16_t src) {
      src_ = src;
    }

    int window_idx() const {
      return window_idx_;
    }

    void set_window_idx(int idx) {
      window_idx_ = idx;
    }
};

/** An acoustic listening window.
 *
 *  An instance of this class represents a period during which the instrument listened for
 *  acoustic signals, decoded from an acoustic position message (see @ref XAcousticPosition): its
 *  start and end times, the pressure, temperature and salinity at the start and end, its index,
 *  and the number of receptions (see @ref XAcousticCacst) that fall within it.
 */
class XAcousticWindow {

    friend class XAcousticPosition;

  private:

    /** Start and end of window timestamps, in UNIX time.
     *
     */
    uint32_t ts_end_, ts_start_;

    /** CTD triplet at end of window.
     *
     */
    uint32_t p_end_, t_end_, s_end_;

    /** CTD triplet at start of window.
     *
     */
    uint32_t p_start_, t_start_, s_start_;

    int idx_;

    int rx_count_;

  public:

    /** Default constructor.
     *
     */
    XAcousticWindow() :
            ts_start_(UINT32_MAX),
            ts_end_(0),
            p_end_(UINT32_MAX),
            t_end_(UINT32_MAX),
            s_end_(UINT32_MAX),
            p_start_(UINT32_MAX),
            t_start_(UINT32_MAX),
            s_start_(UINT32_MAX),
            idx_(-1),
            rx_count_(0) {
      /* Do nothing. */
    }

    /** Construct from message data.
     *
     *  @param[in]      data        The binary message data to decode.
     */
    XAcousticWindow(const vector<uint8_t>& data) : XAcousticWindow() {
      DecodeXMsgData(data);
    }


    /** Destructor
     *
     */
    ~XAcousticWindow() {
      /* Do nothing. */
    }


    /** Decode the acoustic position CACST X-Message from the binary message data.
     *
     *  @param[in]      data        The binary message data to decode.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {

          // Check minimum message size.
          static const int EXPECTED_SIZE = 37;
          if((int) data.size() != EXPECTED_SIZE) {
            ostringstream msg;
            msg << "Invalid XMessage: expected " << EXPECTED_SIZE << " bytes, but only received "
                << data.size() << " bytes!";
            BOOST_THROW_EXCEPTION(log::MessageException(msg));
          }

          const uint8_t* d = data.data();

          set_ts_start(     u32(d,  4));
          set_p_start(      u32(d,  8));
          set_t_start(      u32(d, 12));
          set_s_start(      u32(d, 16));
          set_ts_end(       u32(d, 20));
          set_p_end(        u32(d, 24));
          set_t_end(        u32(d, 28));
          set_s_end(        u32(d, 32));

        }CATCH_BLOCK("Failed to decode acoustic window X-Message!")
    }

  public:

    /* Helper functions: */

    ptime ts_end_ptime() const {
      return bpt::from_time_t(ts_end());
    }

    string ts_end_iso8601_string() const {

      static const string not_a_datetime_string = "0000-00-00T00:00:00Z";

      ostringstream os;

      ptime p = ts_end_ptime();

      if(p.is_not_a_date_time()) {
        os << not_a_datetime_string;
      } else {
        os << bpt::to_iso_extended_string(p) << "Z";
      }

      return os.str();
    }

    ptime ts_start_ptime() const {
      return bpt::from_time_t(ts_start());
    }

    string ts_start_iso8601_string() const {

      static const string not_a_datetime_string = "0000-00-00T00:00:00Z";

      ostringstream os;

      ptime p = ts_start_ptime();

      if(p.is_not_a_date_time()) {
        os << not_a_datetime_string;
      } else {
        os << bpt::to_iso_extended_string(p) << "Z";
      }

      return os.str();
    }

  public:

    /* Mutators and Accessors: */

    uint32_t p_end() const {
      return p_end_;
    }

    void set_p_end(uint32_t p_end) {
      p_end_ = p_end;
    }

    uint32_t p_start() const {
      return p_start_;
    }

    void set_p_start(uint32_t p_start) {
      p_start_ = p_start;
    }

    uint32_t s_end() const {
      return s_end_;
    }

    void set_s_end(uint32_t s_end) {
      s_end_ = s_end;
    }

    uint32_t s_start() const {
      return s_start_;
    }

    void set_s_start(uint32_t s_start) {
      s_start_ = s_start;
    }

    uint32_t t_end() const {
      return t_end_;
    }

    void set_t_end(uint32_t t_end) {
      t_end_ = t_end;
    }

    uint32_t t_start() const {
      return t_start_;
    }

    void set_t_start(uint32_t t_start) {
      t_start_ = t_start;
    }

    uint32_t ts_end() const {
      return ts_end_;
    }

    void set_ts_end(uint32_t ts_end) {
      ts_end_ = ts_end;
    }

    uint32_t ts_start() const {
      return ts_start_;
    }

    void set_ts_start(uint32_t ts_start) {
      ts_start_ = ts_start;
    }

    int idx() const {
      return idx_;
    }

    void set_idx(int idx) {
      idx_ = idx;
    }

    int rx_count() const {
      return rx_count_;
    }

    void set_rx_count(int rx_count) {
      rx_count_ = rx_count;
    }
};

/** Class representing an acoustic position X-Message.
 *
 *  @note       This message type was added in X-Message version 2.0.
 *  @note       This message type is used by marginal ice-zone (MIZ) floats only.
 *
 */
class XAcousticPosition:
        public util::xmsg::XMessage {

  private:

    vector<shared_ptr<XAcousticCacst>> acoustic_cacst_;

    vector<shared_ptr<XAcousticWindow>> acoustic_window_;

  public:

    /* Public member functions: */

    /** Parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XAcousticPosition(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XMessage(dive, whoi, data) {
        DecodeXMsgData(data);    // Decode message.
    }

    /** Destructor.
     *
     */
    ~XAcousticPosition() {
        /* Do nothing. */
    }

    /** Decode the acoustic position X-Message from the binary message data.
     *
     *  @param[in]      data        The binary message data to decode.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

        try {

            // Check message.
            const uint8_t* d = CheckXMsgData(data, XMSG_TYPE_ID_MIZ_ACOUSTIC, -1);

            /* Decode message based on subtype: */

            uint8_t subtype_id = u08(d, 3);

            switch (subtype_id) {

              case 3: // window

                acoustic_window_.push_back(make_shared<XAcousticWindow>(data));

                break;

              case 5: // CACST

                acoustic_cacst_.push_back(make_shared<XAcousticCacst>(data));

                break;

              default:
                break;
            }

            /** @todo   Implement the functionality for decoding the acoustic position message.
             *
             */

        }CATCH_BLOCK("Failed to decode acoustic position X-Message!")
    }

    /** Generate a listing string representation of the acoustic position message.
     *
     *  @return     A listing string representation of the acoustic position message.
     */
    const string GenerateListingString() const {
        ostringstream os;

        try {



          /** @todo   Implement the functionality for generating a listing string for the
           *          acoustic position message.
           *
           */
          os << "# --- Acoustic Listening Windows ------------------------------------------------"
              "-------------------"
             << endl;

          const format fmt3_h(
              "#   %5s %4s %4s %4s"
              " %20s %20s"
              );
          os << format(fmt3_h)
              % "SN" % "dive" % "midx" % "widx"
              % "start-date/time" % "end-date/time"
              << endl;
          os << format(fmt3_h)
              % "" % "" % "" % ""
              % "(UTC)" % "(UTC)"
              << endl;

          const format fmt3(
              "APS %5d %4d %4d %4d"
              " %20s %20s "
              );

          for_each(
              acoustic_window_.begin(),
              acoustic_window_.end(),
              [&](const shared_ptr<XAcousticWindow> window) {

            os << format(fmt3)
                % id_instrument() % id_dive() % 3 % window->idx()
                % window->ts_start_iso8601_string() % window->ts_end_iso8601_string()
                << endl;
          });



          os << "# --- Acoustic Communication Cycle Receive Statistics ---------------------------"
              "-------------------"
             << endl;

          const format fmt5_h(
              "#   %5s %4s %4s %4s"
              " %27s %6s %6s"
              " %6s %6s %6s");

          os << format(fmt5_h)
              % "SN" % "dive" % "midx" % "widx"
              % "date/time" % "src" % "MFDPwr"
              % "PSKErr" % "SNRIn" % "SNROut"
              << endl;
          os << format(fmt5_h)
              % "" % "" % "" % ""
              % "(UTC)" % "" % "(dB)"
              % "" % "(dB)" % "(dB)"

              << endl;

          const format fmt5(
              "APS %5d %4d %4d %4d"
              " %27s %6d %6d"
              " %6d %6.1f %6.1f");

          for_each(
              acoustic_cacst_.begin(),
              acoustic_cacst_.end(),
              [&](const shared_ptr<XAcousticCacst> cacst){

            os << format(fmt5)
                % id_instrument() % id_dive() % 5 % cacst->window_idx()
                % cacst->toa_iso8601_string() % (int)cacst->src() % (int)cacst->mfd_power()
                % cacst->psk_error_code() % cacst->snr_in_as_float() % cacst->snr_in_as_float()
                << endl;
          });

        } CATCH_BLOCK("Failed to generate listing string for acoustic position message!")

        return os.str();
    }

  public:

    /** Number the acoustic listening windows and assign each ping to its window.
     *
     *  Windows are numbered in order, starting at 0. Each ping (CACST record) whose time of
     *  arrival falls within a window's start and end times is given that window's index, and the
     *  window's receive count is incremented. A ping outside every window keeps its window index
     *  unchanged.
     *
     *  @throw      util::log::MessageException if an error occurs.
     */
    void AssignWindowIndices(void) {

      try {

        // Assign indices to windows.

        int idx = 0;
        for_each(
            acoustic_window_.begin(),
            acoustic_window_.end(),
            [&](shared_ptr<XAcousticWindow> window) {
          window->set_idx(idx++);
        });

        // Assign pings to windows.

        for(auto it_cacst = acoustic_cacst_.begin();
            it_cacst != acoustic_cacst_.end();
            it_cacst++) {
          shared_ptr<XAcousticCacst> cacst = *it_cacst;

          // Find which window this ping belongs to.

          for(auto it_w = acoustic_window_.begin(); it_w != acoustic_window_.end(); it_w++) {
            shared_ptr<XAcousticWindow> window = *it_w;

            uint32_t ts = cacst->toa_sec();

            if((ts >= window->ts_start()) && (ts <= window->ts_end())) {

              cacst->set_window_idx(window->idx());
              window->set_rx_count(window->rx_count()+1);
              break;

            }
          }
        }
      } CATCH_BLOCK("Failed to assign acoustic window indices.")
    }
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XACOUSTICPOSITION_H_ */
