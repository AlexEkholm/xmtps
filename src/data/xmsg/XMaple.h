/** The @ref util::xmsg::XMaple class header file.
 *
 *  @file
 *
 *  @author     Mr. Alexander K. Ekholm <aekholm@whoi.edu>,
 *              Engineer I,
 *              Department of Physical Oceanography,
 *              Woods Hole Oceanographic Institution
 *
 *  @copyright  2026 Woods Hole Oceanographic Institution.
 *              Released under the GNU General Public License.
 *
 *  This is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 *  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *  General Public License for more details. You should have received a copy of the GNU General
 *  Public License along with this software. If not, please visit http://www.gnu.org/licenses/.
 */

#ifndef XMAPLE_H_
#define XMAPLE_H_

/* Includes */

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <map>

#include "XMessage.h"

namespace util {
namespace xmsg {

/* Class Definitions */

/** A file sent by a guest subsystem (the Rockland MAPLe microstructure board of micro-ALTO floats),
 *  rebuilt from its guest file fragment X-messages (0xF1), and its decoded content.
 *
 *  The float reads each processed MAPLe file of the ascent (the raw tm.dat is not sent) and sends it
 *  in fragments, each with the file's byte offset and a "more" flag. A file is complete when its
 *  fragments run contiguously from offset 0 and the last one has "more" clear. The MAPLe files are
 *  text with a '%' header and trailer (spec xmessage-format.md C.8):
 *
 *  - Signal-mean files (tm_<channel>.eng): ASCII rows, one per processing record; each column is
 *    sent as its first value then first differences, and is summed here.
 *  - Band-averaged spectra (tm_<channel>_logavg.eng): binary rows of the signal variance and the
 *    band averages: little-endian float32, or, with a "%scaleoff: (scale,offset)" header, one byte b
 *    per value standing for 10^(b / scale - offset).
 *  - Motor files (tm_<channel>_motors.eng): ASCII rows "onoff sample".
 */
class XMapleFile {

  public:

    /** A fragment of the file. */
    struct Fragment {
        bool more;                  //!< More fragments follow this one.
        int version;                //!< Fragment encoding version (0 or 1).
        vector<uint8_t> data;       //!< The file bytes.
    };

    /** The kind of file, from its name. */
    enum Kind { KIND_UNKNOWN, KIND_MEAN, KIND_SPECTRA, KIND_MOTORS };

    /** Construct an empty file.
     *
     *  @param[in]  device_id   The guest device identifier (5: MAPLe on expansion port J5).
     *  @param[in]  file_id     The file identifier within the device.
     */
    XMapleFile(int device_id, int file_id) :
            device_id_(device_id), file_id_(file_id), duplicates_(0), conflicts_(0),
            complete_(false), parsed_(false), channel_(-1), nfft_(0), navg_(0),
            sample_rate_(NAN), effective_sample_rate_(NAN), scale_(NAN), offset_(NAN) {
      /* Do nothing. */
    }

    /** Add a fragment.
     *
     *  A fragment at an offset already held is a retransmission and is ignored; it is counted as a
     *  conflict if its bytes differ.
     *
     *  @param[in]  offset      The byte offset of the fragment in the file.
     *  @param[in]  fragment    The fragment.
     */
    void AddFragment(uint32_t offset, const Fragment& fragment) {
      auto it = fragments_.find(offset);
      if(it != fragments_.end()) {
        ++duplicates_;
        if(it->second.data != fragment.data || it->second.more != fragment.more)
          ++conflicts_;
        return;
      }
      fragments_[offset] = fragment;
      parsed_ = false;
    }

    /** Rebuild the file from its fragments and decode it. Call after adding fragments. */
    void Assemble() {
      content_.clear();
      complete_ = false;
      uint32_t next = 0;
      bool last_more = true;
      for(auto it = fragments_.begin(); it != fragments_.end(); ++it) {
        if(it->first != next)
          break;
        content_.insert(content_.end(), it->second.data.begin(), it->second.data.end());
        next += it->second.data.size();
        last_more = it->second.more;
      }
      complete_ = !fragments_.empty() && !last_more && next == ExpectedSize();
      Parse();
    }

    /** The file name, from the MRV file identifier table of the MAPLe board (device 5). */
    string name() const {
      static const char* names[] = {
          "", "tm.dat",
          "tm_shear.eng", "tm_shear_logavg.eng", "tm_shear_motors.eng",
          "tm_shear1.eng", "tm_shear1_logavg.eng", "tm_shear1_motors.eng",
          "tm_shear2.eng", "tm_shear2_logavg.eng", "tm_shear2_motors.eng",
          "tm_temp.eng", "tm_temp_logavg.eng", "tm_temp_motors.eng",
          "tm_temp1.eng", "tm_temp1_logavg.eng", "tm_temp1_motors.eng",
          "tm_temp2.eng", "tm_temp2_logavg.eng", "tm_temp2_motors.eng" };
      if(device_id_ == 5 && file_id_ >= 1 && file_id_ <= 19)
        return names[file_id_];
      return (format("dev%d-file%d") % device_id_ % file_id_).str();
    }

    /** The kind of file, from its name. */
    Kind kind() const {
      const string n = name();
      if(device_id_ != 5 || n.size() < 4 || n.compare(n.size() - 4, 4, ".eng") != 0)
        return KIND_UNKNOWN;
      if(EndsWith(n, "_logavg.eng"))
        return KIND_SPECTRA;
      if(EndsWith(n, "_motors.eng"))
        return KIND_MOTORS;
      return KIND_MEAN;
    }

    /** The size the fragments give: the end of the last fragment, or 0 without fragments. */
    uint32_t ExpectedSize() const {
      if(fragments_.empty())
        return 0;
      auto it = fragments_.rbegin();
      return it->first + it->second.data.size();
    }

    /** The number of bytes held (the sum of the fragment sizes). */
    uint32_t BytesHeld() const {
      uint32_t n = 0;
      for(const auto& f : fragments_)
        n += f.second.data.size();
      return n;
    }

    /** The highest fragment encoding version seen. */
    int Version() const {
      int v = -1;
      for(const auto& f : fragments_)
        v = std::max(v, f.second.version);
      return v;
    }

    /** Get a header or trailer value; an empty string when absent. */
    string Field(const string& key) const {
      auto it = fields_.find(key);
      return it == fields_.end() ? string() : it->second;
    }

    /** Get a numeric header or trailer value; NaN when absent or not a number. */
    double FieldNumber(const string& key) const {
      const string v = Field(key);
      if(v.empty())
        return NAN;
      char* end = nullptr;
      const double x = strtod(v.c_str(), &end);
      return end == v.c_str() ? NAN : x;
    }

    /** Format a MAPLe date/time ("MM DD YYY hh mm ss ms", years since 1900) as ISO 8601 with
     *  milliseconds; "0000-00-00T00:00:00.000Z" when absent or invalid. */
    static string FormatMapleTime(const string& v) {
      int mo, dd, yy, hh, mi, ss, ms;
      if(sscanf(v.c_str(), "%d %d %d %d %d %d %d", &mo, &dd, &yy, &hh, &mi, &ss, &ms) == 7
          && mo >= 1 && mo <= 12 && dd >= 1 && dd <= 31 && yy >= 70 && yy < 300 && hh >= 0
          && hh < 24 && mi >= 0 && mi < 60 && ss >= 0 && ss < 61 && ms >= 0 && ms < 1000)
        return (format("%04d-%02d-%02dT%02d:%02d:%02d.%03dZ") % (yy + 1900) % mo % dd % hh % mi
                % ss % ms).str();
      return "0000-00-00T00:00:00.000Z";
    }

    /** The processing record length in samples (navg * nfft): the spacing of the signal-mean and
     *  spectra rows. 0 when unknown. */
    long RecordSamples() const {
      return (long) navg_ * nfft_;
    }

    /** The time of the middle of record ridx, in seconds after the start time; NaN when unknown.
     *
     *  A record is 2 * navg + 1 FFT blocks of nfft samples overlapping by half (the bits of the
     *  shear mask), starting at sample ridx * navg * nfft, so its middle is (navg + 1) * nfft / 2
     *  samples later. */
    double RecordTime(size_t ridx) const {
      if(RecordSamples() <= 0 || !(effective_sample_rate_ > 0))
        return NAN;
      return (ridx * (double) RecordSamples() + (navg_ + 1) * nfft_ / 2.0) / effective_sample_rate_;
    }

    /** The time of a sample, in seconds after the start time; NaN when unknown. */
    double SampleTime(double sample) const {
      if(!(effective_sample_rate_ > 0))
        return NAN;
      return sample / effective_sample_rate_;
    }

    /* Accessors */

    int device_id() const { return device_id_; }
    int file_id() const { return file_id_; }
    size_t num_fragments() const { return fragments_.size(); }
    int duplicates() const { return duplicates_; }
    int conflicts() const { return conflicts_; }
    bool complete() const { return complete_; }
    bool parsed() const { return parsed_; }
    int channel() const { return channel_; }
    const string& channel_name() const { return channel_name_; }
    int nfft() const { return nfft_; }
    int navg() const { return navg_; }
    double sample_rate() const { return sample_rate_; }
    double effective_sample_rate() const { return effective_sample_rate_; }
    double scale() const { return scale_; }
    double offset() const { return offset_; }
    const vector<string>& columns() const { return columns_; }
    const vector<vector<double> >& rows() const { return rows_; }
    const vector<std::pair<int, int> >& logmap() const { return logmap_; }
    const string& parse_error() const { return parse_error_; }

  private:

    static bool EndsWith(const string& s, const string& suffix) {
      return s.size() >= suffix.size()
          && s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    static string Trim(const string& s) {
      const size_t a = s.find_first_not_of(" \t\r\n");
      if(a == string::npos)
        return string();
      const size_t b = s.find_last_not_of(" \t\r\n");
      return s.substr(a, b - a + 1);
    }

    /** Read "%key: value" lines from text into fields_. */
    void ReadFields(const string& text) {
      size_t p = 0;
      while(p < text.size()) {
        size_t e = text.find('\n', p);
        if(e == string::npos)
          e = text.size();
        const string line = text.substr(p, e - p);
        p = e + 1;
        if(line.empty() || line[0] != '%')
          continue;
        const size_t c = line.find(':');
        if(c == string::npos)
          continue;
        fields_[Trim(line.substr(1, c - 1))] = Trim(line.substr(c + 1));
      }
    }

    /** Decode a complete MAPLe .eng file: header, body rows and trailer. */
    void Parse() {
      parsed_ = false;
      parse_error_.clear();
      fields_.clear();
      columns_.clear();
      rows_.clear();
      logmap_.clear();
      channel_ = -1;
      channel_name_.clear();
      nfft_ = navg_ = 0;
      sample_rate_ = effective_sample_rate_ = scale_ = offset_ = NAN;

      if(!complete_ || kind() == KIND_UNKNOWN)
        return;

      const string text(content_.begin(), content_.end());

      // The header ends with the %start line; the trailer starts at the last "\n%stop:".
      const size_t start = text.find("%start:");
      if(start == string::npos) {
        parse_error_ = "no %start line";
        return;
      }
      size_t body_begin = text.find('\n', start);
      body_begin = body_begin == string::npos ? text.size() : body_begin + 1;
      size_t body_end = text.rfind("\n%stop:");
      if(body_end == string::npos || body_end < body_begin - 1) {
        parse_error_ = "no %stop line";
        return;
      }
      if(body_end < body_begin)
        body_end = body_begin;      // Empty body: "%start: ...\n%stop: ...".
      ReadFields(text.substr(0, body_begin));
      ReadFields(text.substr(body_end));

      // Channel number and name: "%channel: 0 shear".
      {
        istringstream is(Field("channel"));
        if(!(is >> channel_ >> channel_name_))
          channel_ = -1;
      }
      nfft_ = (int) lround(std::isnan(FieldNumber("nfft")) ? 0 : FieldNumber("nfft"));
      navg_ = (int) lround(std::isnan(FieldNumber("navg")) ? 0 : FieldNumber("navg"));
      sample_rate_ = FieldNumber("samplerate");
      effective_sample_rate_ = FieldNumber("effectivesamplerate");

      // Columns, without their "(scale,offset)" annotations.
      {
        istringstream is(Field("columns"));
        string c;
        while(is >> c) {
          const size_t paren = c.find('(');
          columns_.push_back(paren == string::npos ? c : c.substr(0, paren));
        }
      }

      // Log-average bin map: "0-0,1-1,2-3,...".
      {
        istringstream is(Field("logmap"));
        string item;
        while(std::getline(is, item, ',')) {
          int lo, hi;
          if(sscanf(item.c_str(), "%d-%d", &lo, &hi) == 2)
            logmap_.push_back(std::make_pair(lo, hi));
        }
      }

      // Scale and offset of 8-bit spectra: "(25.6,2.0)".
      {
        const string so = Field("scaleoff");
        double sc, of;
        if(!so.empty() && sscanf(so.c_str(), " (%lf,%lf)", &sc, &of) == 2 && std::fabs(sc) > 0) {
          scale_ = sc;
          offset_ = of;
        }
      }

      if(columns_.empty()) {
        parse_error_ = "no %columns line";
        return;
      }

      const size_t ncol = columns_.size();

      if(kind() == KIND_SPECTRA) {

        // Binary rows: float32 little-endian, or one byte per value with %scaleoff.
        const bool bytes = !std::isnan(scale_);
        const size_t width = bytes ? 1 : 4, row_size = ncol * width;
        const size_t nbody = body_end - body_begin, nrow = nbody / row_size;
        if(nbody % row_size)
          parse_error_ = (format("spectra body of %d bytes is not a whole number of %d-byte rows")
                          % nbody % row_size).str();
        const uint8_t* b = content_.data() + body_begin;
        for(size_t r = 0; r < nrow; ++r) {
          vector<double> row(ncol);
          for(size_t c = 0; c < ncol; ++c) {
            const uint8_t* v = b + r * row_size + c * width;
            if(bytes) {
              row[c] = pow(10.0, v[0] / scale_ - offset_);
            } else {
              const uint32_t u = (uint32_t) v[0] | ((uint32_t) v[1] << 8)
                  | ((uint32_t) v[2] << 16) | ((uint32_t) v[3] << 24);
              float f;
              memcpy(&f, &u, sizeof(f));
              row[c] = f;
            }
          }
          rows_.push_back(row);
        }

      } else {

        // ASCII rows of integers; signal-mean columns are first differences.
        const bool differenced = kind() == KIND_MEAN;
        vector<double> sum(ncol, 0);
        istringstream is(text.substr(body_begin, body_end - body_begin));
        string line;
        while(std::getline(is, line)) {
          istringstream ls(line);
          vector<double> row;
          double x;
          while(ls >> x)
            row.push_back(x);
          if(row.empty())
            continue;
          if(row.size() != ncol) {
            parse_error_ = (format("row %d has %d values, expected %d") % rows_.size()
                            % row.size() % ncol).str();
            row.resize(ncol, NAN);
          }
          if(differenced)
            for(size_t c = 0; c < ncol; ++c)
              row[c] = (sum[c] += row[c]);
          rows_.push_back(row);
        }
      }

      parsed_ = true;
    }

  private:

    int device_id_;
    int file_id_;
    std::map<uint32_t, Fragment> fragments_;
    int duplicates_;
    int conflicts_;
    vector<uint8_t> content_;
    bool complete_;
    bool parsed_;
    string parse_error_;
    std::map<string, string> fields_;
    int channel_;
    string channel_name_;
    int nfft_;
    int navg_;
    double sample_rate_;
    double effective_sample_rate_;
    double scale_;
    double offset_;
    vector<string> columns_;
    vector<vector<double> > rows_;
    vector<std::pair<int, int> > logmap_;
};

/** The guest file fragment X-messages (0xF1) of a dive: the MAPLe microstructure files of a
 *  micro-ALTO float (spec xmessage-format.md C.8).
 *
 *  Blob layout (MRV, "fileBlobber"): the ID 0xF1 and the size; the high nibble of the size word is
 *  the fragment encoding version.
 *
 *  - Version 0: device ID (1 byte), file ID (1 byte), more flag and offset (3 bytes), data, ';'.
 *  - Version 1: device ID in the top 3 bits and file ID in the low 5 bits of one byte, more flag and
 *    offset (3 bytes), data, ';'.
 *
 *  The more flag is the top bit of the big-endian 24-bit word; the low 23 bits are the offset of
 *  the fragment's data in the file.
 */
class XMaple:
        public XMessage {

  public:

    /** Parameterized constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XMaple(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
            XMessage(dive, whoi, data), num_blobs_(0) {
      DecodeXMsgData(data);
    }

    /** Destructor. */
    ~XMaple() {
      /* Do nothing. */
    }

    /** Decode a guest file fragment blob and add it to its file.
     *
     *  @param[in]      data        The binary message data to decode.
     *
     *  @throw          util::log::MessageException if the blob is invalid.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {

      try {

        const uint8_t* d = CheckXMsgData(data, XMSG_TYPE_ID_GUEST_FILE, 8);
        const size_t len = 0x0FFF & u16(d, 1);
        const int version = d[1] >> 4;

        int device_id = 0, file_id = 0;
        size_t mo_at = 0;
        if(version == 0) {
          if(len < 9)
            EXC_THROW("Guest file fragment (version 0) too short: " + lexical_cast<string>(len)
                      + " bytes, at least 9 required!");
          device_id = d[3];
          file_id = d[4];
          mo_at = 5;
        } else if(version == 1) {
          device_id = d[3] >> 5;
          file_id = d[3] & 0x1F;
          mo_at = 4;
        } else {
          EXC_THROW("Unknown guest file fragment version " + lexical_cast<string>(version) + "!");
        }

        const uint32_t mo = u24(d, mo_at);
        XMapleFile::Fragment f;
        f.more = (mo & 0x800000) != 0;
        f.version = version;
        f.data.assign(d + mo_at + 3, d + len - 1);

        const int key = (device_id << 8) | file_id;
        auto it = files_.find(key);
        if(it == files_.end())
          it = files_.insert(std::make_pair(key, XMapleFile(device_id, file_id))).first;
        it->second.AddFragment(mo & 0x7FFFFF, f);
        it->second.Assemble();
        ++num_blobs_;

      }CATCH_BLOCK("Failed to decode guest file fragment (0xF1) message!")
    }

    /** The files, by (device ID << 8 | file ID). */
    const std::map<int, XMapleFile>& files() const {
      return files_;
    }

    /** Generate the listing lines: MSF (files), MSC (channels), MSB (spectral bands), msm
     *  (signal mean), msv (spectra) and MSX (motor events). See spec listing-file-format.md.
     *
     *  @return     The listing string.
     */
    const string GenerateListingString() const {
      ostringstream os;

      try {

        const int sn = id_instrument(), dv = id_dive();

        os << "# --- Guest Files ----------------------------------------------------------------"
              "------------------" << endl;
        os << "#      SN dive dev fid ver nFrag    bytes complete name" << endl;
        for(const auto& kv : files_) {
          const XMapleFile& f = kv.second;
          os << format("MSF %5d %4d %3d %3d %3d %5d %8d %8s %s") % sn % dv % f.device_id()
                % f.file_id() % f.Version() % f.num_fragments() % f.BytesHeld()
                % (f.complete() ? "yes" : "no") % f.name();
          if(f.conflicts())
            os << format(" # %d retransmitted fragment(s) differ") % f.conflicts();
          if(!f.parse_error().empty())
            os << " # " << f.parse_error();
          os << endl;
        }

        // Group the decoded files by channel name.
        std::map<string, const XMapleFile*> mean, spectra, motors;
        vector<string> channels;
        for(const auto& kv : files_) {
          const XMapleFile& f = kv.second;
          if(!f.parsed() || f.channel_name().empty())
            continue;
          const string& c = f.channel_name();
          if(std::find(channels.begin(), channels.end(), c) == channels.end())
            channels.push_back(c);
          (f.kind() == XMapleFile::KIND_SPECTRA ? spectra
           : f.kind() == XMapleFile::KIND_MOTORS ? motors : mean)[c] = &f;
        }
        if(channels.empty())
          return os.str();
        std::sort(channels.begin(), channels.end(), [&](const string& a, const string& b) {
          return Ref(a, mean, spectra, motors)->channel() < Ref(b, mean, spectra, motors)->channel()
              || (Ref(a, mean, spectra, motors)->channel() == Ref(b, mean, spectra, motors)->channel()
                  && a < b);
        });

        os << "# --- Channels -------------------------------------------------------------------"
              "------------------" << endl;
        os << "#      SN dive ch name    nfft navg    fs  fsEff     GD      S  dsThr dsPass"
              "  nMean  nSpec   samples  despiked goodBlk motorBlk               start-date/time"
              "                stop-date/time  comment" << endl;
        os << "#                                    (Hz)   (Hz)  (sec)         (std)"
              << endl;
        for(const string& c : channels) {
          const XMapleFile* r = Ref(c, mean, spectra, motors);
          const XMapleFile* m = mean.count(c) ? mean[c] : nullptr;
          const XMapleFile* s = spectra.count(c) ? spectra[c] : nullptr;
          os << format("MSC %5d %4d %2d %-7s %4d %4d %5s %6s %6s %6s %6s %6s %6d %6d %9s %9s"
                       " %7s %8s %29s %29s  %s")
                % sn % dv % r->channel() % c % r->nfft() % r->navg()
                % Num(r->sample_rate(), "%.0f") % Num(r->effective_sample_rate(), "%.2f")
                % Num(r->FieldNumber("GD"), "%.4f") % Num(r->FieldNumber("S"), "%.4f")
                % Num(r->FieldNumber("despikethreshold"), "%.2f")
                % Num(r->FieldNumber("despikepasses"), "%.0f")
                % (m ? (int) m->rows().size() : -1) % (s ? (int) s->rows().size() : -1)
                % Num(r->FieldNumber("samplesprocessed"), "%.0f")
                % Num(r->FieldNumber("totaldespike"), "%.0f")
                % Num(r->FieldNumber("goodblocks"), "%.0f")
                % Num(r->FieldNumber("motordroppedblocks"), "%.0f")
                % XMapleFile::FormatMapleTime(r->Field("start"))
                % XMapleFile::FormatMapleTime(r->Field("stop"))
                % r->Field("comment")
             << endl;
        }

        // Band definitions of each channel's spectra.
        bool any_spectra = false;
        for(const string& c : channels)
          any_spectra = any_spectra || spectra.count(c);
        if(any_spectra) {
          os << "# --- Band-Averaged Spectra: Bands -----------------------------------------------"
                "------------------" << endl;
          os << "#      SN dive ch name    band  kLo  kHi     freq    fLo    fHi" << endl;
          os << "#                                            (Hz)   (Hz)   (Hz)" << endl;
          for(const string& c : channels) {
            if(!spectra.count(c))
              continue;
            const XMapleFile* s = spectra[c];
            const double df = s->nfft() > 0 ? s->effective_sample_rate() / s->nfft() : NAN;
            for(size_t b = 0; b < s->logmap().size(); ++b) {
              const int lo = s->logmap()[b].first, hi = s->logmap()[b].second;
              // MAPLe bin k is the frequency (k + 1) * fsEff / nfft (bin 0 is the first above DC).
              // Centre frequency from the column name (cf_<Hz>), else the band's mean frequency.
              double fc = ((lo + hi) * 0.5 + 1) * df;
              if(b + 1 < s->columns().size() && s->columns()[b + 1].compare(0, 3, "cf_") == 0)
                fc = atof(s->columns()[b + 1].c_str() + 3);
              os << format("MSB %5d %4d %2d %-7s %4d %4d %4d %8s %6s %6s") % sn % dv
                    % s->channel() % c % b % lo % hi % Num(fc, "%.2f")
                    % Num((lo + 0.5) * df, "%.2f") % Num((hi + 1.5) * df, "%.2f") << endl;
            }
          }
        }

        // Signal mean rows.
        for(const string& c : channels) {
          if(!mean.count(c))
            continue;
          const XMapleFile* m = mean[c];
          os << "# --- Signal Mean: " << c << " " << string(std::max(0, 80 - (int) c.size()), '-')
             << endl;
          os << "#      SN dive ch name     ridx     time      sigMean  despiked    shearMask"
             << endl;
          os << "#                                 (+sec)     (counts)" << endl;
          const int imask = Column(*m, "shearmask"), idesp = Column(*m, "despikepoints"),
              imean = Column(*m, "sigmean");
          for(size_t r = 0; r < m->rows().size(); ++r) {
            const vector<double>& row = m->rows()[r];
            os << format("msm %5d %4d %2d %-7s %5d %8s %12s %9s %12s") % sn % dv % m->channel() % c
                  % r % Num(m->RecordTime(r), "%+.1f")
                  % Num(imean >= 0 ? row[imean] : NAN, "%.0f")
                  % Num(idesp >= 0 ? row[idesp] : NAN, "%.0f")
                  % (imask >= 0 && !std::isnan(row[imask])
                     ? (format("0x%05X") % (long) row[imask]).str() : string("-1"))
               << endl;
          }
        }

        // Spectra rows.
        for(const string& c : channels) {
          if(!spectra.count(c))
            continue;
          const XMapleFile* s = spectra[c];
          os << "# --- Band-Averaged Spectra: " << c << " "
             << string(std::max(0, 70 - (int) c.size()), '-') << endl;
          os << "#      SN dive ch name     ridx     time     sigVar";
          for(size_t b = 1; b < s->columns().size(); ++b)
            os << format(" %10s") % (format("S%d") % (b - 1)).str();
          os << endl;
          os << "#                                 (+sec) (counts^2)" << endl;
          os << "#   S<n>: band-averaged spectrum of band n (MSB lines), in counts^2/Hz." << endl;
          for(size_t r = 0; r < s->rows().size(); ++r) {
            os << format("msv %5d %4d %2d %-7s %5d %8s") % sn % dv % s->channel() % c % r
                  % Num(s->RecordTime(r), "%+.1f");
            for(double v : s->rows()[r])
              os << format(" %10s") % Num(v, "%.4e");
            os << endl;
          }
        }

        // Motor events.
        bool any_motors = false;
        for(const string& c : channels)
          any_motors = any_motors || (motors.count(c) && !motors[c]->rows().empty());
        if(any_motors) {
          os << "# --- Motor Events ---------------------------------------------------------------"
                "------------------" << endl;
          os << "#      SN dive ch name    eidx onoff    sample     time" << endl;
          os << "#                                                (+sec)" << endl;
          for(const string& c : channels) {
            if(!motors.count(c))
              continue;
            const XMapleFile* x = motors[c];
            const int ionoff = Column(*x, "onoff"), isample = Column(*x, "sample");
            for(size_t r = 0; r < x->rows().size(); ++r) {
              const vector<double>& row = x->rows()[r];
              const double sample = isample >= 0 ? row[isample] : NAN;
              os << format("MSX %5d %4d %2d %-7s %4d %5s %9s %8s") % sn % dv % x->channel() % c
                    % r % Num(ionoff >= 0 ? row[ionoff] : NAN, "%.0f") % Num(sample, "%.0f")
                    % Num(x->SampleTime(sample), "%+.1f") << endl;
            }
          }
        }

      }CATCH_BLOCK("Failed to generate listing string for guest file fragment messages!")

      return os.str();
    }

  private:

    /** A file of a channel to take the header values from: spectra, else mean, else motors. */
    static const XMapleFile* Ref(const string& c, const std::map<string, const XMapleFile*>& mean,
                                 const std::map<string, const XMapleFile*>& spectra,
                                 const std::map<string, const XMapleFile*>& motors) {
      if(spectra.count(c))
        return spectra.at(c);
      if(mean.count(c))
        return mean.at(c);
      return motors.at(c);
    }

    /** The index of a column by name, or -1. */
    static int Column(const XMapleFile& f, const string& name) {
      for(size_t i = 0; i < f.columns().size(); ++i)
        if(f.columns()[i] == name)
          return (int) i;
      return -1;
    }

    /** Format a number, or "NaN" when it is not finite. */
    static string Num(double x, const char* fmt) {
      if(!std::isfinite(x))
        return "NaN";
      return (format(fmt) % x).str();
    }

    std::map<int, XMapleFile> files_;
    int num_blobs_;
};

} /* namespace xmsg */
} /* namespace util */
#endif /* XMAPLE_H_ */
