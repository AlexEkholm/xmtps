/** The @ref argo::iridium::xfloat::XFloatDiveData class source file.
 *
 *  This is the source file for the @ref argo::iridium::xfloat::XFloatDiveData class. This class
 *  contains all of the mail messages and corresponding decoded telemetry data for an instrument
 *  for a single dive. Each time a mail message is received by the telemetry decoder thread, @ref
 *  argo::iridium::xfloat::XFloatTelemetryDecoderService, the corresponding dive data object is
 *  retrieved and the mail message is assigned to it. Then, the binary data from the attached
 *  Iridium Short Burst Data (SBD) message is decoded. As a result of decoding the binary message
 *  data, an object corresponding to the message type is created and its field  are populated from
 *  the decoded data in the binary message. The newly created object will be  of a type that
 *  inherits from @ref util::xmsg::XMessage. A shared reference to the object is assigned to the
 *  dive data object.
 *
 *  This class contains the functionality for writing the formatted intermediate and PHY listing
 *  files to the local filesystem. Each time a new message is received, the entire intermediate
 *  listing file will be written to the local filesystem, overwriting the previous listing file,
 *  if it exists. The PHY file is written only when the dive data object is determined to contain
 *  the complete data for dive, and only if the instrument has been deployed i.e. it has a
 *  metadata file with valid DAC and WMO identifiers.
 *
 *  This file contains the static data members of the class.
 *
 * 	@file
 *
 * 	@version	0.5
 *
 * 	@date     	Jul 3, 2014 8:56:05 AM
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

#include "XFloatDiveData.h"   // header
#include "XFloat.h"           // parent float (GetGpsProfilePosition())

namespace argo {
namespace iridium {
namespace xfloat {

/* Static Variables: */

bool XFloatDiveData::intermediate_listing_file_generation_is_enabled_(false);

path XFloatDiveData::intermediate_listing_directory_path_;

bool XFloatDiveData::netcdf_file_generation_is_enabled_(false);

path XFloatDiveData::netcdf_directory_path_;

const ptime XFloatDiveData::netcdf_reference_datetime_(
    boost::gregorian::date(1950, boost::gregorian::Jan, 1),
    boost::posix_time::time_duration(0, 0, 0));

bool XFloatDiveData::phy_file_generation_is_enabled_(false);

path XFloatDiveData::phy_directory_path_;

bool XFloatDiveData::pressure_offset_correction_enabled_list_file_(true);

bool XFloatDiveData::pressure_offset_correction_enabled_netcdf_file_(true);

bool XFloatDiveData::pressure_offset_correction_enabled_phy_file_(true);

const util::mysql::SqlTableDescription<XFloatDiveData> XFloatDiveData::sql_table_description_(

    "XFloatDive",

    {

      sql_coli(XFloatDiveData, "id_dac", &XFloatDiveData::id_dac, &XFloatDiveData::set_id_dac),

      sql_coli(
          XFloatDiveData,
          "id_dive",
          &XFloatDiveData::id_dive,
          &XFloatDiveData::set_id_dive,
          true),

      sql_coll(XFloatDiveData, "id_imei", &XFloatDiveData::id_imei, &XFloatDiveData::set_id_imei),

      sql_coli(
          XFloatDiveData,
          "id_instrument",
          &XFloatDiveData::id_instrument,
          &XFloatDiveData::set_id_instrument,
          true),

      sql_coli(XFloatDiveData, "id_wmo", &XFloatDiveData::id_wmo, &XFloatDiveData::set_id_wmo),

      sql_colb(
          XFloatDiveData,
          "complete",
          &XFloatDiveData::is_complete,
          &XFloatDiveData::set_complete),

      sql_colp(
          XFloatDiveData,
          "intermediate_listing_file_path",
          &XFloatDiveData::intermediate_listing_file_path,
          &XFloatDiveData::set_intermediate_listing_file_path),

      sql_colt(
          XFloatDiveData,
          "iridium_position_datetime_first",
          &XFloatDiveData::iridium_position_datetime_first,
          &XFloatDiveData::set_iridium_position_datetime_first),

      sql_colt(
          XFloatDiveData,
          "iridium_position_datetime_last",
          &XFloatDiveData::iridium_position_datetime_last,
          &XFloatDiveData::set_iridium_position_datetime_last),

      sql_colp(
          XFloatDiveData,
          "phy_file_path",
          &XFloatDiveData::phy_file_path,
          &XFloatDiveData::set_phy_file_path),

      sql_cu16(
          XFloatDiveData,
          "num_sbd_msgs",
          &XFloatDiveData::num_sbd_msgs,
          &XFloatDiveData::set_num_sbd_msgs),

      sql_colf(
          XFloatDiveData,
          "pres_offs",
          &XFloatDiveData::pres_offs,
          &XFloatDiveData::set_pres_offs),

      sql_coli(
          XFloatDiveData,
          "pres_offs_num_dives",
          &XFloatDiveData::pres_offs_num_dives,
          &XFloatDiveData::set_pres_offs_num_dives),

      sql_coli(
          XFloatDiveData,
          "pres_offs_num_samples",
          &XFloatDiveData::pres_offs_num_samples,
          &XFloatDiveData::set_pres_offs_num_samples),

      sql_colb(
          XFloatDiveData,
          "pres_offs_valid",
          &XFloatDiveData::is_pres_offs_valid,
          &XFloatDiveData::set_pres_offs_valid),

      sql_colf(
          XFloatDiveData,
          "surface_pres_max",
          &XFloatDiveData::surface_pres_max,
          &XFloatDiveData::set_surface_pres_max),

      sql_colf(
          XFloatDiveData,
          "surface_pres_mean",
          &XFloatDiveData::surface_pres_mean,
          &XFloatDiveData::set_surface_pres_mean),

      sql_colf(
          XFloatDiveData,
          "surface_pres_min",
          &XFloatDiveData::surface_pres_min,
          &XFloatDiveData::set_surface_pres_min),

      sql_colb(
          XFloatDiveData,
          "surface_pres_valid",
          &XFloatDiveData::is_surface_pres_valid,
          &XFloatDiveData::set_surface_pres_valid),

      sql_colf(
          XFloatDiveData,
          "surface_pres_variance",
          &XFloatDiveData::surface_pres_variance,
          &XFloatDiveData::set_surface_pres_variance),

    });

/* Function Definitions: */

/** The default constructor.
 *
 *  @exception      None. Never throws.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
XFloatDiveData::XFloatDiveData() :
        Iridium(),
        argo_instrument_type_(XARGO_INSRUMENT_TYPE_UNKNOWN),
        complete_(false),
        id_dive_(XFLT_DIVE_UNKNOWN),
        iridium_position_datetime_first_(not_a_date_time),
        iridium_position_datetime_last_(not_a_date_time),
        num_sbd_msgs_(0),
        pres_offs_(0),
        pres_offs_num_dives_(-1),
        pres_offs_num_samples_(-1),
        pres_offs_valid_(false),
        surface_pres_max_(XMSG_SENS_BAD_PRES),
        surface_pres_mean_(XMSG_SENS_BAD_PRES),
        surface_pres_min_(XMSG_SENS_BAD_PRES),
        surface_pres_num_samples_(-1),
        surface_pres_valid_(false),
        surface_pres_variance_(XMSG_SENS_BAD_PRES),
        listing_file_updated_(false),
        netcdf_file_updated_(false),
        phy_file_updated_(false),
        xmsg_surface_time_series_() { /* Do nothing. */ }
#pragma GCC diagnostic pop

/** The destructor.
 *
 */
XFloatDiveData::~XFloatDiveData() { /* Do nothing. */ }

/** Is this dive data object complete?
 *
 *  This function determines if the dive data is complete and sets the \ref complete_ flag
 *  accordingly. The completeness of the dive data is determined by \ref CheckCompleteness()
 *  which is invoked after each new mail message is decoded and assigned to this dive data
 *  object.
 *
 *  The dive data is determined to be complete if the dive data object meets all of the
 *  following criteria: it contains a complete set of profile data, the sensor gains and
 *  offsets for this dive are known (i.e. the Argo data message has been received), it
 *  contains a set of mail messages with contiguous packet identifiers (PIDs) starting at
 *  PID=0, and it contains a valid position for the profile. These criteria is determined by
 *  the functions \ref HasCompleteProfileData(), \ref SensorGainsAndOffsetsAreKnown(),
 *  \ref HasContiguousPids(), \ref HasValidPosition(), respectively.
 *
 *  @return     TRUE if all data has been received for this dive, or false otherwise.
 */
bool XFloatDiveData::CheckCompleteness() {

  bool complete = false;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    set_complete(
        complete = (HasCompleteProfileData() && SensorGainsAndOffsetsAreKnown()
            && HasContiguousPids() && HasValidPosition()));

  }CATCH_BLOCK("Failed to check dive data completeness!")

  return complete;
}

string XFloatDiveData::GeneratePressureCorrectionListingString() const {

   ostringstream os;
   int midx = 0;

   try {

     {
       const format fmt("#   %5s %4s %4s "
           "%4s %5s "
           "%5s "
           "%15s "
           "%8s %8s %8s %8s");
       os << format(fmt) % "SN" % "dive" % "midx" //
              % "nSam" % "nDive" //
              % "valid" //
              % "presOffset" //
              % "sfcPmin" % "sfcPmean" % "sfcPmax" % "sfcPstd"
          << endl;
     }

     {
       const format fmt("#   %5s %4s %4s "
           "%4s %5s "
           "%5s "
           "%15s "
           "%8s %8s %8s %8s");
       os << format(fmt) % "" % "" % "" //
              % "" % "" //
              % "" //
              % "(dBar)" //
              % "(dBar)" % "(dBar)" % "(dBar)" % "(dBar)"
          << endl;
     }

     {
       const format fmt("PRC %5d %4d %4s "
           "%4d %5d "
           "%5d "
           "%15s "
           "%8s %8s %8s %8s");
       // This dive's uncorrected surface pressure statistics; the minimum excludes outliers.
       const string sfc_pres_std = surface_pres_variance() < 0 ? "-9.999"
           : (format("%.3f") % sqrt(surface_pres_variance())).str();
       os << format(fmt) % id_instrument() % id_dive() % midx++ //
           % pres_offs_num_samples() % pres_offs_num_dives() //
           % (int) is_pres_offs_valid() //
           % XMsgFormatPressure(pres_offs()) //
           % XMsgFormatPressure(surface_pres_min()) % XMsgFormatPressure(surface_pres_mean()) //
           % XMsgFormatPressure(surface_pres_max()) % sfc_pres_std;
     }

   }CATCH_BLOCK("Failed to generate pressure correction listing string!")

   return os.str();

 }

/** Get the end-of-dive GPS message.
 *
 *  This function returns a pointer to the GPS message that represents the position of the
 *  profile.
 *
 *  @return         A pointer to the end-of-dive GPS message, or to a newly constructed empty
 *                  GPS message if one does not exist.
 */
shared_ptr<x::XGPS> XFloatDiveData::GetGpsPositionEndOfDive() const {

  using namespace x;

  shared_ptr<XGPS> x;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    // Iterate over GPS messages.
    for_each(
        xmsg_gps_positions_.begin(),
        xmsg_gps_positions_.end(),
        [&](const shared_ptr<XGPS>& gps) {

          // Check the GPS message type.
          if(gps->id_xmsg_type() == XMSG_TYPE_ID_GPS_DIAGNOSTIC
              || gps->id_xmsg_type() == XMSG_TYPE_ID_GPS_ENDOFDIVE) {

            // Update the GPS position.
            if(!x || x->position_datetime() == not_a_date_time
                || gps->position_datetime() > x->position_datetime()) {
              x = gps;
            }
          }
        });

    // Create an empty placeholder GPS message.
    if(!x)
      x = make_shared<XGPS>(
          id_dive(),
          id_instrument(),
          id_dive() == 0 ? XMSG_TYPE_ID_GPS_DIAGNOSTIC : XMSG_TYPE_ID_GPS_ENDOFDIVE);

  }CATCH_BLOCK("Failed to get end-of-dive GPS message!")

  return x;
}

shared_ptr<x::XGPS> XFloatDiveData::GetGpsPositionStartOfDive() const {

  using namespace x;

  shared_ptr<XGPS> x;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    // Iterate over GPS messages.
    for_each(
        xmsg_gps_positions_.begin(),
        xmsg_gps_positions_.end(),
        [&](const shared_ptr<XGPS>& gps) {

          // Check the GPS message type.
          if(gps->id_xmsg_type() == XMSG_TYPE_ID_GPS_STARTOFDIVE) {

            // Update the GPS position.
            if(!x || x->position_datetime() == not_a_date_time
                || gps->position_datetime() > x->position_datetime()) {
              x = gps;
            }
          }
        });

    // Create an empty placeholder GPS message.
    if(!x)
      x = make_shared<XGPS>(id_dive(), id_instrument(), XMSG_TYPE_ID_GPS_STARTOFDIVE);

  }CATCH_BLOCK("Failed to get end-of-dive GPS message!")

  return x;
}

string XFloatDiveData::GenerateReadableHex(const vector<uint8_t>& data) {

  static const format fmt_char(" %2c");
  static const format fmt_hex(" %02X");
  static const format fmt_int(" %02d");
  static const format fmt_line(" 0x%04X (%04u) | 0x ");

  size_t byte_count = 0;
  ostringstream readable_data_ostream;

  try {

    // Print the header lines.
    readable_data_ostream << "====================================================================="
        "======================================================" << endl;
    readable_data_ostream << "               | 0x";
    for(size_t i = 0; i < 32; i++)
      readable_data_ostream << (i % 4 == 0 ? " " : "") << format(fmt_hex) % i;
    readable_data_ostream << endl << "               |   ";
    for(size_t i = 0; i < 32; i++)
      readable_data_ostream << (i % 4 == 0 ? " " : "") << format(fmt_int) % i;
    readable_data_ostream << endl;
    readable_data_ostream << "---------------------------------------------------------------------"
        "------------------------------------------------------" << endl;

    // Print the bytes in a readable format.
    for(vector<uint8_t>::const_iterator data_iterator = data.begin();
        data_iterator < data.end();) {

      ostringstream readable_line_char;
      ostringstream readable_line_hex;

      // Iterate over bytes in groups of 32.
      for(size_t i = 0; i < 32 && data_iterator < data.end(); i++) {
        if(i == 0) {
          readable_line_hex << format(fmt_line) % byte_count % byte_count;
          readable_line_char << "               |    ";
        } else if(byte_count % 4 == 0) {
          readable_line_hex << " ";
          readable_line_char << " ";
        }

        readable_line_hex << format(fmt_hex) % (int) *data_iterator;

        if(isprint(*data_iterator))
          readable_line_char << format(fmt_char) % (char) *data_iterator;
        else
          readable_line_char << format(fmt_char) % ".";

        data_iterator++;
        byte_count++;
      }

      readable_data_ostream << readable_line_hex.str()
                            << endl
                            << readable_line_char.str()
                            << endl;

    }
    readable_data_ostream << "====================================================================="
        "======================================================" << endl;


  }CATCH_BLOCK("Failed to generate readable hex string of vector data!");

  return readable_data_ostream.str();
}

shared_ptr<x::XGPS> XFloatDiveData::GetGpsProfilePosition() const {

  using namespace x;

  shared_ptr<XGPS> x;

  try {

    // Check if there is a good end-of-dive GPS position.
    if(GpsPositionIsValidProfilePosition(x = GetGpsPositionEndOfDive()))
      return x;

    // Query the database for the start-of-dive position for the next dive cycle.
    vector<shared_ptr<XGPS> > gps_positions;
    if(util::mysql::LocalMysqlConfiguration::is_sql_enabled())
      gps_positions = XGPS::SelectObjectsFromDatabase(
            "id_instrument = " + lexical_cast<string>(id_instrument()) + " AND id_dive = "
                + lexical_cast<string>(id_dive() + 1) + " AND id_type = "
                + lexical_cast<string>((int) XMSG_TYPE_ID_GPS_STARTOFDIVE)
                + " AND valid = 1");

    // Without the SQL database, take it from the next dive read in this run, if any.
    else if(shared_ptr<XFloatDiveData> next_dive = parent_instrument()->GetCachedDiveData(id_dive() + 1)) {

      lock_guard<recursive_mutex> guard(next_dive->mutex_);
      for(const shared_ptr<XGPS>& gps : next_dive->xmsg_gps_positions_)
        if(gps && gps->id_xmsg_type() == XMSG_TYPE_ID_GPS_STARTOFDIVE && gps->position_is_valid())
          gps_positions.push_back(gps);
    }

    vector<shared_ptr<XGPS> >::iterator gps_postion_itr;
    for(gps_postion_itr = gps_positions.begin(); gps_postion_itr < gps_positions.end();
        ++gps_postion_itr) {
      if(GpsPositionIsValidProfilePosition(*gps_postion_itr))
        return (*gps_postion_itr);
    }

  }CATCH_BLOCK("Failed to get profile position GPS message!")

  return x;
}

/** Generate a composite fall time-series data for this dive.
 *
 *  @throw          util::log::MessageException if an error occurs while generating the composite
 *                  data.
 *
 *  @return         The The composite fall time-series data for this dive.
 */
shared_ptr<x::XCompositeFall> XFloatDiveData::GenerateCompositeFallTimeSeriesData() const {

  using namespace x;

  shared_ptr<x::XCompositeFall> composite_fall;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    ldbg() << "Generating composite fall time-series data...";
    composite_fall = make_shared<XCompositeFall>();
    composite_fall->AppendAllElements< XMSG_MAX_FALL>(xmsg_fall_time_series_);

  }CATCH_BLOCK("Failed to generate composite fall time-series from dive data!")

  return composite_fall;
}


/** Get the composite pump time-series data for this dive.
 *
 *  @throw          util::log::MessageException if an error occurs while generating the composite
 *                  data.
 *
 *  @return         The composite pump time-series data for this dive.
 */
shared_ptr<x::XCompositePump> XFloatDiveData::GenerateCompositePumpTimeSeriesData() const {

  using namespace x;

  shared_ptr<x::XCompositePump> composite_pump;

  try {
    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    ldbg() << "Generating composite pump time-series...";
    composite_pump = make_shared<XCompositePump>();
    composite_pump->AppendAllElements< XMSG_MAX_PUMP>(xmsg_pump_time_series_);
  }CATCH_BLOCK("Failed to generate composite pump time-series from dive data!")

  return composite_pump;
}

/** Get the composite rise time-series data for this dive.
 *
 *  @throw          util::log::MessageException if an error occurs while generating the composite
 *                  data.
 *
 *  @return         The composite rise time-series data for this dive.
 */
shared_ptr<x::XCompositeRise> XFloatDiveData::GenerateCompositeRiseTimeSeriesData() const {

  using namespace x;

  shared_ptr<XCompositeRise> composite_rise;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    ldbg() << "Generating composite rise time-series...";
    composite_rise = make_shared<XCompositeRise>();
    composite_rise->AppendAllElements< XMSG_MAX_RISE>(xmsg_rise_time_series_);

  }CATCH_BLOCK("Failed to generate composite rise from dive data!")

  return composite_rise;
}

shared_ptr<x::XCompositeValve> XFloatDiveData::GenerateCompositeValveData() const {

  using namespace x;

  shared_ptr<XCompositeValve> composite_valve;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    ldbg() << "Generating composite valve data object...";
    composite_valve = make_shared<XCompositeValve>();
    composite_valve->AppendAllElements< XMSG_MAX_VALVE>(xmsg_valve_data_);

  }CATCH_BLOCK("Failed to generate composite valve data object from dive data!")

  return composite_valve;
}

/** Does this dive data object have any binned profile data?
 *
 *  @return         TRUE if this dive data object has any binned profile data, or FALSE otherwise.
 */
bool XFloatDiveData::HasBinnedProfileData() const {

  // Get an exclusive lock.
  lock_guard<recursive_mutex> guard(mutex_);

  return !xmsg_composite_profile_binned_.IsEmpty();
}

bool XFloatDiveData::HasBinnedProfileSalinityData() const {

  // Get an exclusive lock.
  lock_guard<recursive_mutex> guard(mutex_);

  return xmsg_composite_profile_binned_.num_saln_samples() > 0;
}

/** Does this dive have complete binned profile data?
 *
 *  @note   The conditions that need to be met in order for the profile data for this dive to
 *          be considered complete are:
 *          - @f$N_{pres} > 0@f$, and
 *          - @f$N_{pres} = N_{temp}@f$, and
 *          - @f$[(N_{saln = 0})@f$ or @f$(N_{saln} = N_{pres})]@f$, and
 *          - pres., temp., and saln. message each have contiguously incrementing message
 *              indices.
 *
 *  @return TRUE if this dive has complete profile data, or FALSE otherwise.
 */
bool XFloatDiveData::HasCompleteBinnedProfileData() const {
  return xmsg_composite_profile_binned_.IsDataComplete();
}

/** Does this dive have complete binned or unbinned profile data?
 *
 *  @note   The conditions that need to be met in order for the profile data for this dive to
 *          be considered complete are:
 *          - @f$N_{pres} > 0@f$, and
 *          - @f$N_{pres} = N_{temp}@f$, and
 *          - @f$[(N_{saln = 0})@f$ or @f$(N_{saln} = N_{pres})]@f$, and
 *          - pres., temp., and saln. message each have contiguously incrementing message
 *              indices.
 *
 *  @return TRUE if this dive has complete profile data, or FALSE otherwise.
 */
bool XFloatDiveData::HasCompleteProfileData() const {
  return HasCompleteBinnedProfileData() || HasCompleteUnbinnedProfileData();
}

/** Does this dive have complete unbinned profile data?
 *
 *  @note   The conditions that need to be met in order for the profile data for this dive to
 *          be considered complete are:
 *          - @f$N_{pres} > 0@f$, and
 *          - @f$N_{pres} = N_{temp}@f$, and
 *          - @f$[(N_{saln = 0})@f$ or @f$(N_{saln} = N_{pres})]@f$, and
 *          - pres., temp., and saln. message each have contiguously incrementing message
 *              indices.
 *
 *  @return TRUE if this dive has complete profile data, or FALSE otherwise.
 */
bool XFloatDiveData::HasCompleteUnbinnedProfileData() const {
  return xmsg_composite_profile_unbinned_.IsDataComplete();
}

/** Do the messages that comprise this dive data object have contiguously increasing MOMSN
 *  numbers?
 *
 *  @note   This method will still return TRUE if there are Iridium message with duplicate
 *          MOMSNs, as long as no numbers are missing from the sequence.
 *
 *  @throw  util::log::MessageException if an error occurs.
 *
 *  @return TRUE if the mail message have contiguously incrementing MOMSNs, or FALSE otherwise.
 */
bool XFloatDiveData::HasContiguousMomsns() const {

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    vector<int> momsns(GenerateOrderedMomsnList());
    switch(momsns.size()) {
      case 0: return false;
      case 1: return true;
      default:
        // Check for each MOMSN in the sequence
        int minMomsn = momsns.at(0), maxMomsn = momsns.at(momsns.size() - 1);
        for(int i = minMomsn; i <= maxMomsn; i++)
          if(find(momsns.begin(), momsns.end(), i) == momsns.end())
            return false;
        return true;
    }

  }CATCH_BLOCK("Failed to check for contiguous MOMSNs!")

  return false;
}

/** Do the messages that comprise this dive data object have contiguously increasing packet index
 *  identifiers (PIDs) starting with zero?
 *
 *  @note           This method will still return TRUE if there are Iridium message with duplicate
 *                  PIDs, as long as no numbers are missing from the sequence.
 *
 *  @throw          util::log::MessageException if an error occurs.
 *
 *  @return         TRUE if the mail message have contiguously increasing packet index identifiers
 *                  (PIDs), or FALSE otherwise.
 */    //
bool XFloatDiveData::HasContiguousPids() const {

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    vector<int> pids(GenerateOrderedPidList());
    switch(pids.size()) {
      case 0: return false;
      case 1: return (pids.at(0) == 0);
      default:                // Check for each PID.
        int maxPid = pids.at(pids.size() - 1);
        for(int i = 0; i <= maxPid; i++)
          if(find(pids.begin(), pids.end(), i) == pids.end())
            return false;
        return true;
    }

  }CATCH_BLOCK("Failed to check for contiguous PIDs!")

  return false;
}

/** Does this dive data object have any dive data?
 *
 *  @return         TRUE if this dive data object has any dive data, or FALSE otherwise.
 */
bool XFloatDiveData::HasDiveData() const {

  // Get an exclusive lock.
  lock_guard<recursive_mutex> guard(mutex_);

  return HasDriftData() || HasCompleteProfileData() || HasFallRiseData()
      || !xmsg_surface_time_series_.IsEmpty() || HasWaveData();
}

/** Does this dive data object have any drift data?
 *
 *  @return         TRUE if this dive data object has any profile data, or FALSE otherwise.
 */
bool XFloatDiveData::HasDriftData() const {

  // Get an exclusive lock.
  lock_guard<recursive_mutex> guard(mutex_);

  return !xmsg_composite_drift_data_.IsEmpty();
}

/** Does this dive data object have any engineering data?
 *
 *  @return         TRUE if this dive data object has any engineering data, or FALSE otherwise.
 */
bool XFloatDiveData::HasEngineeringData() const {

  // Get an exclusive lock.
  lock_guard<recursive_mutex> guard(mutex_);

  return xmsg_engineering_data_abort_ || xmsg_engineering_data_ascent_
      || xmsg_engineering_data_bit_fail_ || xmsg_engineering_data_bit_passed_
      || xmsg_engineering_data_descent_ || xmsg_engineering_data_diagnostic_dive_
      || xmsg_engineering_data_drift_ || xmsg_engineering_data_standard_dive_
      || xmsg_engineering_data_surface_;
}

/** Does this dive data object have any rise or fall data?
 *
 *  @return         TRUE if this dive data object has any rise or fall data, or FALSE otherwise.
 */
bool XFloatDiveData::HasFallRiseData() const {

  // Get an exclusive lock.
  lock_guard<recursive_mutex> guard(mutex_);

  return !xmsg_fall_time_series_.empty() || !xmsg_rise_time_series_.empty();
}

/** Does this dive data object have any position data?
 *
 *  @return         TRUE if this dive data object has any position data, or FALSE otherwise.
 */
bool XFloatDiveData::HasPositionData() const {

  // Get an exclusive lock.
  lock_guard<recursive_mutex> guard(mutex_);

  return !xmsg_gps_positions_.empty() || !mail_messages_.empty();
}

bool XFloatDiveData::HasProfileSalinityData() const {

  return (HasBinnedProfileSalinityData() || HasUnbinnedProfileData());
}

/** Does this dive data object have any unbinned profile data?
 *
 *  @return         TRUE if this dive data object has any unbinned profile data, or FALSE otherwise.
 */
bool XFloatDiveData::HasUnbinnedProfileData() const {

  // Get an exclusive lock.
  lock_guard<recursive_mutex> guard(mutex_);

  return !xmsg_composite_profile_unbinned_.IsEmpty();
}

bool XFloatDiveData::HasUnbinnedProfileSalinityData() const {

  // Get an exclusive lock.
  lock_guard<recursive_mutex> guard(mutex_);

  return xmsg_composite_profile_unbinned_.num_saln_samples() > 0;
}

/** Does this dive data object have a valid GPS position for this dive's profile data?
 *
 *  This function checks if the dive data object has a valid GPS position that accurately
 *  represents the position and date/time of the profile data for this dive.
 *
 *  @return         TRUE if this dive data object has a valid GPS position, or FALSE otherwise.
 */
bool XFloatDiveData::HasValidGpsPosition() const {

  using namespace x;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    // Check if there is a valid end-of-dive GPS position for this dive.
    shared_ptr<XGPS> x(GetGpsProfilePosition());

    // Check the GPS position.
    return GpsPositionIsValidProfilePosition(x);

  }CATCH_BLOCK("Failure encountered during check of dive data GPS message validity")

  return false;
}

/** Does this dive data object have a valid Iridium position?
 *
 *  This function checks if the dive data object has a valid Iridium position that accurately
 *  represents the position and date/time of the profile data for this dive cycle.
 *
 *  @return         TRUE if this dive data object has a valid composite Iridium position, or
 *                  FALSE otherwise.
 */
bool XFloatDiveData::HasValidIridiumPosition() const {

  /** @todo   Implement the functionality to determine if a dive data object has a valid
   *          Iridium position that accurately represents the position and date/time of the
   *          profile data for this dive cycle.
   */

  return false;
}

/** Does this dive data object have a valid position for this dive's profile data?
 *
 *  Check if we have either a valid GPS position, or a valid composite Iridium position to
 *  represent the profile location.
 *
 *  @return         TRUE if this dive data object has a valid position for this dive's profile data,
 *                  or FALSE otherwise.
 */ //
bool XFloatDiveData::HasValidPosition() const {
  return HasValidGpsPosition() || HasValidIridiumPosition();
}

bool XFloatDiveData::HasWaveData() const {
  return xmsg_wave_field_ && xmsg_wave_field_->segment_len();
}

//map<string, NcDim> XFloatDiveData::NC_GenerateDimensions(NcGroup& nc_group) {
//
//  static const format nc_dim_string_format("STRING%d");
//  static const string nc_dim_datetime_name("DATE_TIME"), nc_dim_n_levels_name("N_LEVELS"),
//      nc_dim_n_param_name("N_PARAM"), nc_dim_n_prof_name("N_PROF");
//  static const size_t nc_dim_datetime_size(14);
//
//  ostringstream os;
//
//  string nc_dim_name;
//  size_t nc_dim_size;
//
//  try {
//
//    // Get an exclusive lock.
//    lock_guard<recursive_mutex> guard(mutex_);
//
//    // Clear existing dimensions.
//    nc_dims_.clear();
//
//    // Add the "DATETIME" dimension.
//    nc_dims_.insert(
//        pair<string, NcDim>(
//            nc_dim_datetime_name,
//            nc_group.addDim(nc_dim_datetime_name, nc_dim_datetime_size)));
//
//    //  Add the "N_LEVELS" dimension.
//    nc_dim_size = max(
//        (xmsg_composite_profile_binned_.IsEmpty() ?
//            0 :
//            xmsg_composite_profile_binned_.max_num_samples()),
//        (xmsg_composite_profile_unbinned_.IsEmpty() ?
//            0 :
//            xmsg_composite_profile_unbinned_.max_num_samples()));
//    nc_dims_.insert(
//        pair<string, NcDim>(
//            nc_dim_n_levels_name,
//            nc_group.addDim(nc_dim_n_levels_name, nc_dim_size)));
//
//    // Add the "N_PARAM" dimension.
//    nc_dim_size = HasProfileSalinityData() ? 3 : 2;
//    nc_dims_.insert(
//        pair<string, NcDim>(
//            nc_dim_n_param_name,
//            nc_group.addDim(nc_dim_n_param_name, nc_dim_size)));
//
//    // Add the "N_PROF" dimension.
//    nc_dim_size = std::min(
//        (HasBinnedProfileData() ? 1 : 0) + (HasUnbinnedProfileData() ? 1 : 0),
//        1);
//    nc_dims_.insert(
//        pair<string, NcDim>(
//            nc_dim_n_prof_name,
//            nc_group.addDim(nc_dim_n_prof_name, nc_dim_size)));
//
//    // Add the "STRINGn" dimensions.
//    for(nc_dim_size = 2; nc_dim_size <= 256; nc_dim_size *= 2) {
//
//      // Create the dimension name.
//      os << format(nc_dim_string_format) % nc_dim_size;
//      nc_dim_name = os.str();
//
//      // Clear the output stream
//      os.str("");
//      os.clear();
//
//      // Add the dimension
//      nc_dims_.insert(
//          pair<string, NcDim>(nc_dim_name, nc_group.addDim(nc_dim_name, nc_dim_size)));
//
//    }
//
//  }CATCH_BLOCK("Failed to generate NC dimensions!")
//
//  return nc_dims_;
//
//}

//void XFloatDiveData::NC_GenerateGlobalAttrs(NcGroup& nc_group) const {
//
//  static const format fmt_title("Profile Data for ALAMO Float #%04d, Dive Cycle %d");
//
//  ostringstream os;
//  string att_str;
//
//  try {
//
//    // title
//    os << format(fmt_title) % id_instrument() % id_dive();
//    att_str = os.str();
//    os.str("");
//    os.clear();
//    nc_group.putAtt("title", att_str);
//
//    // institution
//    nc_group.putAtt("institution", "Woods Hole Oceanographic Institution");
//
//    // source
//    nc_group.putAtt("source", string("ALAMO float"));
//
//    // history
//    nc_group.putAtt(
//        "history",
//        DateUtils::GenerateCurrentUtcTimeIso8601String() + " creation");
//
//    // references
//    nc_group.putAtt("references", "http://www.argodatamgt.org/Documentation");
//
//    // comment
//    nc_group.putAtt("comment", "This file was created by " PROG_NAME " v" PROG_VERS ".");
//
//    // user_manual_version
//    nc_group.putAtt("user_manual_version", "3.2");
//
//    // Conventions
//    nc_group.putAtt("Conventions", "Argo-3.0 CF-1.6");
//
//    // featureType
//    nc_group.putAtt("featureType", "trajectoryProfile");
//
//    // comment_on_resolution
//    nc_group.putAtt("comment_on_resolution", "");
//
//  }CATCH_NOTHROW("Failed to generate NetCDF global attributes!")
//
//}

//void XFloatDiveData::NC_GenerateVariables(NcGroup& nc_group) {
//
//  uint16_t pg;
//  uint16_t po;
//  uint16_t tg;
//  uint16_t to;
//  uint16_t sg;
//  uint16_t so;
//
//  size_t i, j;
//
//  NcVar nc_var;
//
//  ostringstream os;
//
//  double sample_buf[XMSG_SENS_MAX_SAMPLES];
//  double juld;
//  double fill_value;
//
//  string str;
//
//  vector<size_t> startp, countp;
//
//  try {
//
//    // Get an exclusive lock.
//    lock_guard<recursive_mutex> guard(mutex_);
//
//    // Get dimensions.
//    NcDim nc_dim_date_time(nc_dims_.at("DATE_TIME"));
//    NcDim nc_dim_n_levels(nc_dims_.at("N_LEVELS"));
//    NcDim nc_dim_n_param(nc_dims_.at("N_PARAM"));
//    NcDim nc_dim_n_prof(nc_dims_.at("N_PROF"));
//    NcDim nc_dim_string2(nc_dims_.at("STRING2"));
//    NcDim nc_dim_string4(nc_dims_.at("STRING4"));
//    NcDim nc_dim_string8(nc_dims_.at("STRING8"));
//    NcDim nc_dim_string16(nc_dims_.at("STRING16"));
//    NcDim nc_dim_string32(nc_dims_.at("STRING32"));
//    NcDim nc_dim_string64(nc_dims_.at("STRING64"));
//    NcDim nc_dim_string256(nc_dims_.at("STRING256"));
//
//    /* Instrument Identifiers - Platform (WMO) Number: */
//
//    try {
//
//      // Add variable.
//      nc_var = nc_group.addVar("PLATFORM_NUMBER", netCDF::ncChar, vector<NcDim>( {
//        nc_dim_n_prof,
//        nc_dim_string8 }));
//
//      // Add variable attribute(s).
//      nc_var.putAtt("_FillValue", netCDF::ncChar, 1, " ");
//      nc_var.putAtt(
//          "long_name",
//          "World Meteorological Organization (WMO) unique  instrument identifier");
//      nc_var.putAtt("conventions", "WMO unique instrument identifier: A9IIIII");
//
//      // Add variable value(s).
//      static const format fmt_platform_number("%-8d");
//      os << format(fmt_platform_number) % id_wmo();
//      str = os.str();
//      countp = {1, 8};
//      startp = {0, 0};
//      for(i = 0; i < nc_dim_n_prof.getSize(); ++i) {
//
//        startp.at(0) = i;
//        nc_var.putVar(startp, countp, str.c_str());
//
//      }
//
//      // Clear stream.
//      os.str("");
//      os.clear();
//
//    }CATCH_BLOCK("Failed to add PLATFORM_NUMBER variable!")
//
//    /* Instrument Identifiers - Float Serial Number: */
//
//    try {
//
//      // Add variable.
//      nc_var = nc_group.addVar("FLOAT_SERIAL_NO", netCDF::ncChar, vector<NcDim>( {
//        nc_dim_n_prof,
//        nc_dim_string32 }));
//
//      // Add variable attribute(s).
//      nc_var.putAtt("_FillValue", netCDF::ncChar, 1, " ");
//      nc_var.putAtt(
//          "long_name",
//          "Unique  instrument identifier (e.g. instrument serial number)");
//
//      // Add variable value(s).
//      static const format fmt_float_serial_no("%-32d");
//      os << format(fmt_float_serial_no) % id_instrument();
//      str = os.str();
//      countp = {1, 32};
//      startp = {0, 0};
//      for(i = 0; i < nc_dim_n_prof.getSize(); ++i) {
//
//        startp.at(0) = i;
//        nc_var.putVar(startp, countp, str.c_str());
//
//      }
//
//      // Clear stream.
//      os.str("");
//      os.clear();
//
//    }CATCH_BLOCK("Failed to add FLOAT_SERIAL_NO variable!")
//
//    /* Instrument Identifiers - Dive Cycle Number: */
//
//    try {
//
//      // Add variable.
//      nc_var = nc_group.addVar("CYCLE_NUMBER", netCDF::ncInt, vector<NcDim>( {
//        nc_dim_n_prof }));
//
//      // Add variable attribute(s).
//      nc_var.putAtt("_FillValue", netCDF::ncInt, 99999);
//      nc_var.putAtt("long_name", "Instrument dive cycle number");
//      nc_var.putAtt(
//          "conventions",
//          "0...N, 0 : launch cycle (if exists), 1 : first complete cycle");
//
//      // Add variable value(s).
//      startp = {0};
//      for(i = 0; i < nc_dim_n_prof.getSize(); ++i) {
//
//        startp.at(0) = i;
//        nc_var.putVar(startp, id_dive());
//
//      }
//
//    }CATCH_BLOCK("Failed to add CYCLE_NUMBER variable!")
//
//    /* Instrument Identifiers -  Station Parameters: */
//
//    try {
//
//      // Add variable.
//      nc_var = nc_group.addVar("STATION_PARAMETERS", netCDF::ncChar, vector<NcDim>( {
//        nc_dim_n_prof,
//        nc_dim_n_param,
//        nc_dim_string16 }));
//
//      // Add variable attribute(s).
//      nc_var.putAtt("_FillValue", netCDF::ncChar, 1, " ");
//      nc_var.putAtt("long_name", "List of available parameters for the station");
//      nc_var.putAtt("conventions", "Argo reference table 3");
//
//      // Add variable value(s).
//      static const char * station_parameters[3] = {
//        "PRES            ",
//        "TEMP            ",
//        "PSAL            " };
//      countp = {1, 1, 16};
//      startp = {0, 0, 0};
//      for(i = 0; i < nc_dim_n_prof.getSize(); ++i) {
//
//        startp.at(0) = i;
//
//        for(j = 0; j < nc_dim_n_param.getSize(); ++j) {
//
//          startp.at(1) = j;
//          nc_var.putVar(startp, countp, station_parameters[j]);
//
//        }
//      }
//
//    }CATCH_BLOCK("Failed to add STATION_PARAMETERS variable!")
//
//    /* TODO General - Date Creation: */
//
//    try {
//
//    }CATCH_BLOCK("Failed to add XXX variable!")
//
//    /* TODO General - Date update: */
//
//    try {
//
//    }CATCH_BLOCK("Failed to add XXX variable!")
//
//    /* TODO General - Reference Date/Time: */
//
//    try {
//
//      // Add variable.
//      nc_var = nc_group.addVar("REFERENCE_DATE_TIME", netCDF::ncChar, vector<NcDim>( {
//        nc_dim_date_time }));
//
//      // Add variable attribute(s).
//      nc_var.putAtt("_FillValue", netCDF::ncChar, 1, " ");
//      nc_var.putAtt("conventions", "ISO8601 date/time string: YYYYMMDDhhmmss");
//      nc_var.putAtt("long_name", "Date of reference for Julian days");
//
//      // Add variable value(s).
//      str = GenerateIso8601String(netcdf_reference_datetime_, false);
//      str = str.substr(0, 8) + str.substr(9, 6);
//      countp = {nc_dim_date_time.getSize()};
//      startp = {0};
//      nc_var.putVar(startp, countp, str.c_str());
//
//    }CATCH_BLOCK("Failed to add REFERENCE_DATE_TIME variable!")
//
//    /* TODO Geolocation: */
//
//    // Get profile position.
//    shared_ptr<x::XGPS> gps_prof(GetGpsProfilePosition());
//    if(gps_prof) {
//
//      /* Geolocation - Julian Day: */
//
//      try {
//
//
//
//        fill_value = 999999.;
//
//        // Add variable.
//        nc_var = nc_group.addVar("JULD", netCDF::ncDouble, vector<NcDim>( {
//          nc_dim_n_prof }));
//
//        // Add variable attribute(s).
//        nc_var.putAtt("_FillValue", netCDF::ncDouble, fill_value);
//        nc_var.putAtt(
//            "conventions",
//            "Relative julian days with decimal part (as parts of day)");
//        nc_var.putAtt(
//            "long_name",
//            "Julian day (UTC) of the station relative to REFERENCE_DATE_TIME");
//        nc_var.putAtt("units", "days since 1950-01-01T00:00:00Z");
//
//        // Add variable value(s).
//        juld = fill_value;
//        if(xmsg_argo_data_
//            && !xmsg_argo_data_->ascent_state_start_time().is_not_a_date_time()) {
//
//          // Get Argo data start-of-ascent timestamp.
//          juld = GenerateJulianDay(
//              xmsg_argo_data_->ascent_state_start_time(),
//              netcdf_reference_datetime_);
//
//        }
//
//        startp = {0};
//        for(i = 0; i < nc_dim_n_prof.getSize(); ++i) {
//
//          startp.at(0) = i;
//          nc_var.putVar(startp, juld);
//
//        }
//
//      }CATCH_BLOCK("Failed to add JULD variable!")
//
//      /* Geolocation - Location Julian Day: */
//
//      try {
//
//
//        fill_value = 999999.;
//
//        // Add variable.
//        nc_var = nc_group.addVar("JULD_LOCATION", netCDF::ncDouble, vector<NcDim>( {
//          nc_dim_n_prof }));
//
//        // Add variable attribute(s).
//        nc_var.putAtt("_FillValue", netCDF::ncDouble, fill_value);
//        nc_var.putAtt(
//            "conventions",
//            "Relative julian days with decimal part (as parts of day)");
//        nc_var.putAtt(
//            "long_name",
//            "Julian day (UTC) of the location relative to REFERENCE_DATE_TIME");
//        nc_var.putAtt("units", "days since 1950-01-01T00:00:00Z");
//
//        // Add variable value(s).
//        startp = {0};
//        juld =
//            (gps_prof->position_datetime().is_not_a_date_time()) ?
//                (fill_value) :
//                (GenerateJulianDay(
//                    gps_prof->position_datetime(),
//                    netcdf_reference_datetime_));
//
//        for(i = 0; i < nc_dim_n_prof.getSize(); ++i) {
//
//          startp.at(0) = i;
//          nc_var.putVar(startp, juld);
//
//        }
//
//      }CATCH_BLOCK("Failed to add JULD_LOCATION variable!")
//
//      /* Geolocation - Latitude: */
//
//      try {
//
//        fill_value = 99999;
//
//        // Add variable.
//        nc_var = nc_group.addVar("LATITUDE", netCDF::ncDouble, vector<NcDim>( {
//          nc_dim_n_prof }));
//
//        // Add variable attribute(s).
//        nc_var.putAtt("_FillValue", netCDF::ncDouble, fill_value);
//        nc_var.putAtt("axis", "Y");
//        nc_var.putAtt("long_name", "Latitude of the station, best estimate");
//        nc_var.putAtt("standard_name", "latitude");
//        nc_var.putAtt("units", "degree_north");
//        nc_var.putAtt("valid_max", netCDF::ncDouble, 90.);
//        nc_var.putAtt("valid_min", netCDF::ncDouble, -90.);
//
//        // Add variable value(s).
//        startp = {0};
//        for(i = 0; i < nc_dim_n_prof.getSize(); ++i) {
//
//          startp.at(0) = i;
//          nc_var.putVar(startp, gps_prof->position_latitude());
//
//        }
//
//      }CATCH_BLOCK("Failed to add LATITUDE variable!")
//
//      /* Geolocation - Longitude: */
//
//      try {
//
//        fill_value = 99999;
//
//        // Add variable.
//        nc_var = nc_group.addVar("LONGITUDE", netCDF::ncDouble, vector<NcDim>( {
//          nc_dim_n_prof }));
//
//        // Add variable attribute(s).
//        nc_var.putAtt("_FillValue", netCDF::ncDouble, fill_value);
//        nc_var.putAtt("axis", "X");
//        nc_var.putAtt("long_name", "Longitude of the station, best estimate");
//        nc_var.putAtt("standard_name", "longitude");
//        nc_var.putAtt("units", "degree_east");
//        nc_var.putAtt("valid_max", netCDF::ncDouble, 180.);
//        nc_var.putAtt("valid_min", netCDF::ncDouble, -180.);
//
//        // Add variable value(s).
//        startp = {0};
//        for(i = 0; i < nc_dim_n_prof.getSize(); ++i) {
//
//          startp.at(0) = i;
//          nc_var.putVar(startp, gps_prof->position_longitude());
//
//        }
//
//      }CATCH_BLOCK("Failed to add LONGITUDE variable!")
//    }
//
//    /* TODO Profile Data: */
//
//    fill_value = 99999;
//
//    // Get sensor data gains and offsets
//    GetGainsAndOffsets(pg, po, tg, to, sg, so);
//
//    float pres_correction = 0;
//
//    if(is_pres_offs_valid() && pressure_offset_correction_enabled_netcdf_file_)
//
//      // Get pressure offset correction.
//      pres_correction = pres_offs();
//
//    /* TODO Profile Data - PRES: */
//
//    try {
//
//      // Add variable.
//      nc_var = nc_group.addVar("PRES", netCDF::ncDouble, vector<NcDim>( {
//        nc_dim_n_prof,
//        nc_dim_n_levels }));
//
//      // Add variable attribute(s).
//      nc_var.putAtt("_FillValue", netCDF::ncDouble, fill_value);
//      nc_var.putAtt("long_name", "Sea water pressure, equals 0 at sea-level");
//      nc_var.putAtt("standard_name", "sea_water_pressure");
//      nc_var.putAtt("units", "decibar");
//      nc_var.putAtt("valid_max", netCDF::ncDouble, 12000.);
//      nc_var.putAtt("valid_min", netCDF::ncDouble, 0.);
//
//      // Add variable value(s).
//      countp = {1, nc_dim_n_levels.getSize()};
//      startp = {0, 0};
//
//      if(HasBinnedProfileData()) {
//
//        std::fill_n(sample_buf, XMSG_SENS_MAX_SAMPLES, fill_value);
//        xmsg_composite_profile_binned_.CopyPressureDataToArray(sample_buf,
//        XMSG_SENS_MAX_SAMPLES, pg, po, pres_correction, fill_value);
//        nc_var.putVar(startp, countp, sample_buf);
//
//        startp.at(0)++;
//
//        }
//
////                if(HasUnbinnedProfileData()) {
////                    std::fill_n(sample_buf, XMSG_SENS_MAX_SAMPLES, fill_value);
////                    xmsg_composite_profile_unbinned_.CopyPressureDataToArray(sample_buf,
////                    XMSG_SENS_MAX_SAMPLES, pg, po, pres_correction, fill_value);
////                    nc_var.putVar(startp, countp, sample_buf);
////
//////                    startp.at(0)++;
////                }
//
//      }
//    CATCH_BLOCK("Failed to add PRES variable!")
//
//    /* TODO Profile Data - TEMP: */
//
//    try {
//
//      // Add variable.
//      nc_var = nc_group.addVar("TEMP", netCDF::ncDouble, vector<NcDim>( {
//        nc_dim_n_prof,
//        nc_dim_n_levels }));
//
//      // Add variable attribute(s).
//      nc_var.putAtt("_FillValue", netCDF::ncDouble, fill_value);
//      nc_var.putAtt("long_name", "Sea temperature in-situ ITS-90 scale");
//      nc_var.putAtt("standard_name", "sea_water_temperature");
//      nc_var.putAtt("units", "degree_Celsius");
//      nc_var.putAtt("valid_max", netCDF::ncDouble, 40.);
//      nc_var.putAtt("valid_min", netCDF::ncDouble, -2.5);
//
//      // Add variable value(s).
//      countp = {1, nc_dim_n_levels.getSize()};
//      startp = {0, 0};
//
//      if(HasBinnedProfileData()) {
//
//        std::fill_n(sample_buf, XMSG_SENS_MAX_SAMPLES, fill_value);
//        xmsg_composite_profile_binned_.CopyTemperatureDataToArray(sample_buf,
//        XMSG_SENS_MAX_SAMPLES, tg, to, fill_value);
//        nc_var.putVar(startp, countp, sample_buf);
//
//        startp.at(0)++;
//
//        }
//
////                if(HasUnbinnedProfileData()) {
////
////                    std::fill_n(sample_buf, XMSG_SENS_MAX_SAMPLES, fill_value);
////                    xmsg_composite_profile_unbinned_.CopyTemperatureDataToArray(sample_buf,
////                    XMSG_SENS_MAX_SAMPLES, tg, to, fill_value);
////                    nc_var.putVar(startp, countp, sample_buf);
////
//////                    startp.at(0)++;
////                }
//
//      }
//    CATCH_BLOCK("Failed to add TEMP variable!")
//
//    /* TODO Profile Data - PSAL: */
//
//    if(HasProfileSalinityData())
//      try {
//
//        // Add variable.
//        nc_var = nc_group.addVar("PSAL", netCDF::ncDouble, vector<NcDim>( {
//          nc_dim_n_prof,
//          nc_dim_n_levels }));
//
//        // Add variable attribute(s).
//        nc_var.putAtt("_FillValue", netCDF::ncDouble, fill_value);
////                nc_var.putAtt("axis", "Y");
//        nc_var.putAtt("long_name", "Practical salinity");
//        nc_var.putAtt("standard_name", "sea_water_salinity");
//        nc_var.putAtt("units", "psu");
//        nc_var.putAtt("valid_max", netCDF::ncDouble, 41.);
//        nc_var.putAtt("valid_min", netCDF::ncDouble, 2.);
//
//        // Add variable value(s).
//        countp = {1, nc_dim_n_levels.getSize()};
//        startp = {0, 0};
//
//        if(HasBinnedProfileData()) {
//
//          std::fill_n(sample_buf, XMSG_SENS_MAX_SAMPLES, fill_value);
//          xmsg_composite_profile_binned_.CopySalinityDataToArray(sample_buf,
//          XMSG_SENS_MAX_SAMPLES, sg, so, fill_value);
//          nc_var.putVar(startp, countp, sample_buf);
//
//          startp.at(0)++;
//
//          }
//        }
//      CATCH_BLOCK("Failed to add PSAL variable!")
//
//    /** @todo Implement NC variable generation for XFloatDiveData
//     *
//     */
//
//  }CATCH_BLOCK("Failed to generate NC variables!")
//}

void XFloatDiveData::ResetData() {

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    set_complete(false);
    set_iridium_position_datetime_first(not_a_date_time);
    set_iridium_position_datetime_last(not_a_date_time);
    mail_messages_.clear();
    set_num_sbd_msgs(0);
    xmsg_acoustic_positions_.reset();
    xmsg_argo_data_.reset();
    xmsg_composite_profile_binned_.Reset();
    xmsg_composite_profile_unbinned_.Reset();
    xmsg_composite_drift_data_.Reset();
    xmsg_engineering_data_abort_.reset();
    xmsg_engineering_data_ascent_.reset();
    xmsg_engineering_data_bit_fail_.reset();
    xmsg_engineering_data_bit_passed_.reset();
    xmsg_engineering_data_descent_.reset();
    xmsg_engineering_data_diagnostic_dive_.reset();
    xmsg_engineering_data_drift_.reset();
    xmsg_engineering_data_ice_.reset();
    xmsg_engineering_data_standard_dive_.reset();
    xmsg_engineering_data_surface_.reset();
    xmsg_fall_time_series_.clear();
    xmsg_gps_positions_.clear();
    xmsg_mission_cmd_echoback_.clear();
    xmsg_extended_records_.clear();
    xmsg_wims_parameters_.clear();
    xmsg_pump_time_series_.clear();
    xmsg_rise_time_series_.clear();
    xmsg_surface_time_series_.Reset();
    xmsg_valve_data_.clear();

  }CATCH_BLOCK("Failed to reset dive data!")

}

//void XFloatDiveData::WriteNetCDFFile() {
//
//  if(!netcdf_file_generation_is_enabled_)
//    return;
//
//  path target_directory;
//
//  try {
//
//    // Get an exclusive lock.
//    lock_guard<recursive_mutex> guard(mutex_);
//
//    // Check if the NetCDF file requires update.
//    if(is_netcdf_file_updated()) {
//
////                ldbg() << "NetCDF file is up to date.";
//      return;
//
//    }
//
//    {   // Get listing directory.
//      ostringstream os;
//      os << format("%s/%04d") % netcdf_directory_path_.string() % id_instrument();
//      target_directory = GetDirectoryPathFromString(os.str(), true);
//    }
//
//    {   // Set local path
//      ostringstream os;
//      os << format("/R%04d_%04d.nc") % id_instrument() % id_dive();
//      set_netcdf_file_path(target_directory += os.str());
//    }
//
//    {   // Write file.
//      linf() << "Writing NetCDF file: " << netcdf_file_path() << "...";
//
//      // Lock the NetCDF mutex.
//      lock_guard<recursive_mutex> guard(netcdf_mutex_);
//
//      NcFile nc_file(
//          netcdf_file_path().string(),
//          NcFile::FileMode::replace,
//          NcFile::FileFormat::nc4);
//      NC_GenerateDimensions(nc_file);
//      NC_GenerateGlobalAttrs(nc_file);
//      NC_GenerateVariables(nc_file);
//
//      // Clear NC dims when complete.
//      nc_dims_.clear();
//    }
//
//    // Update the NetCDF file status flag.
//    set_netcdf_file_updated();
//
//  }CATCH_BLOCK("Failed to write NetCDF file!")
//
//}

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */

