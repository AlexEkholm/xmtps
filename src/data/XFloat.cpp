/** The @ref argo::iridium::xfloat::XFloat class source file.
 *
 *  This is the source file for the @ref argo::iridium::xfloat::XFloat class. This class
 *  represents a single instrument or float. Each instance of this contains information about an
 *  instrument including unique identifiers, sensor gains and offsets, metadata, and the
 *  instrument type. It also contains a dive-or-cycle-number-to-dive-data-object map that
 *  contains all of the incomplete dive data objects that belong to the instrument, uniquely
 *  indexed by dive or cycle number. As the application receives the telemetry data from the
 *  instrument, the data is decoded and assigned to the corresponding dive data object. Once it
 *  is determined that the data is complete for a single dive, the intermediate and PHY listing
 *  files are written to the local filesystem, the dive data object is persisted to the SQL
 *  database, and then the shared reference to the dive data object is released, thereby purging
 *  the dive data from the program's memory.
 *
 *  At run-time, at most one float object will exist for each instrument. Each float object is
 *  accessed and/or manipulated by the worker threads through a shared reference. A shared
 *  reference to all float objects is contained in the instrument-identifier-to-instrument-object
 *  map which is indexed by the unique instrument identifier. During application initialization,
 *  the map is populated as a result of querying all the known instruments from the SQL database.
 *  Each time a float object is created, it is added to the map. During application termination,
 *  the daemon thread iterates over the contents of the map and persists each of the float objects
 *  to the SQL database.
 *
 *  This file contains the static data members and some of the member functions of the @ref
 *  argo::iridium::xfloat::XFloat class. Several of the member functions of the @ref
 *  argo::iridium::xfloat::XFloatDiveData class are also defined in this file.
 *
 *  @file
 *
 *  @version    0.6
 *
 * 	@date       May 12, 2014 4:10:33 PM
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

// header
#include "XFloat.h"

namespace argo {
namespace iridium {
namespace xfloat {

/* Static Variables: */

flat_map<int, shared_ptr<XFloat> > XFloat::id2float_map_;

recursive_mutex XFloat::id2float_map_mutex_;

vector<unsigned int> XFloat::instrument_ids_to_enable_phy_generation_;

vector<unsigned int> XFloat::instrument_ids_to_disable_phy_generation_;

bool XFloat::kml_file_generation_is_enabled_;

path XFloat::kml_directory_path_;

const util::mysql::SqlTableDescription<XFloat> XFloat::sql_table_description_(
    "XFloat",
    {

      sql_cou8(
          XFloat,
          "argo_instrument_type",
          &XFloat::argo_instrument_type,
          &XFloat::set_argo_instrument_type),

      sql_coli(
          XFloat,
          "id_dac",
          &XFloat::id_dac,
          &XFloat::set_id_dac),

      sql_coll(
          XFloat,
          "id_imei",
          &XFloat::id_imei,
          &XFloat::set_id_imei),

      sql_coli(
          XFloat,
          "id_instrument",
          &XFloat::id_instrument,
          &XFloat::set_id_instrument,
          true),

      sql_coli(
          XFloat,
          "id_wmo",
          &XFloat::id_wmo,
          &XFloat::set_id_wmo),

      sql_cu16(
          XFloat,
          "gain_pressure",
          &XFloat::gain_pressure,
          &XFloat::set_gain_pressure),

      sql_cu16(
          XFloat,
          "gain_pressure_surface",
          &XFloat::gain_pressure_surface,
          &XFloat::set_gain_pressure_surface),

      sql_cu16(
          XFloat,
          "gain_salinity",
          &XFloat::gain_salinity,
          &XFloat::set_gain_salinity),

      sql_cu16(
          XFloat,
          "gain_salinity_surface",
          &XFloat::gain_salinity_surface,
          &XFloat::set_gain_salinity_surface),

      sql_cu16(
          XFloat,
          "gain_temperature",
          &XFloat::gain_temperature,
          &XFloat::set_gain_temperature),

      sql_cu16(
          XFloat,
          "gain_temperature_surface",
          &XFloat::gain_temperature_surface,
          &XFloat::set_gain_temperature_surface),

      sql_colt(
          XFloat,
          "latest_gps_position_datetime",
          &XFloat::latest_gps_position_datetime,
          &XFloat::set_latest_gps_position_datetime),

      sql_coli(
          XFloat,
          "latest_gps_position_dive_number",
          &XFloat::latest_gps_position_dive_number,
          &XFloat::set_latest_gps_position_dive_number),

      sql_cold(
          XFloat,
          "latest_gps_position_latitude",
          &XFloat::latest_gps_position_latitude,
          &XFloat::set_latest_gps_position_latitude),

      sql_cold(
          XFloat,
          "latest_gps_position_longitude",
          &XFloat::latest_gps_position_longitude,
          &XFloat::set_latest_gps_position_longitude),

      sql_coli(
          XFloat,
          "latest_iridium_position_cep_radius",
          &XFloat::latest_iridium_position_cep_radius,
          &XFloat::set_latest_iridium_position_cep_radius),

      sql_colt(
          XFloat,
          "latest_iridium_position_datetime",
          &XFloat::latest_iridium_position_datetime,
          &XFloat::set_latest_iridium_position_datetime),

      sql_coli(
          XFloat,
          "latest_iridium_position_dive_number",
          &XFloat::latest_iridium_position_dive_number,
          &XFloat::set_latest_iridium_position_dive_number),

      sql_cold(
          XFloat,
          "latest_iridium_position_latitude",
          &XFloat::latest_iridium_position_latitude,
          &XFloat::set_latest_iridium_position_latitude),

      sql_cold(
          XFloat,
          "latest_iridium_position_longitude",
          &XFloat::latest_iridium_position_longitude,
          &XFloat::set_latest_iridium_position_longitude),

      sql_coli(
          XFloat,
          "latest_sbd_momsn",
          &XFloat::latest_sbd_momsn,
          &XFloat::set_latest_sbd_momsn),

      sql_coli(
          XFloat,
          "latest_sbd_mtmsn",
          &XFloat::latest_sbd_mtmsn,
          &XFloat::set_latest_sbd_mtmsn),

      sql_colb(
          XFloat,
          "netcdf_file_generation_is_enabled",
          &XFloat::netcdf_file_generation_is_enabled,
          &XFloat::set_netcdf_file_generation_is_enabled),

      sql_cu16(
          XFloat,
          "offset_pressure",
          &XFloat::offset_pressure,
          &XFloat::set_offset_pressure),

      sql_cu16(
          XFloat,
          "offset_pressure_surface",
          &XFloat::offset_pressure_surface,
          &XFloat::set_offset_pressure_surface),

      sql_cu16(
          XFloat,
          "offset_salinity",
          &XFloat::offset_salinity,
          &XFloat::set_offset_salinity),

      sql_cu16(
          XFloat,
          "offset_salinity_surface",
          &XFloat::offset_salinity_surface,
          &XFloat::set_offset_salinity_surface),

      sql_cu16(
          XFloat,
          "offset_temperature",
          &XFloat::offset_temperature,
          &XFloat::set_offset_temperature),

      sql_cu16(
          XFloat,
          "offset_temperature_surface",
          &XFloat::offset_temperature_surface,
          &XFloat::set_offset_temperature_surface),

      sql_colb(
          XFloat,
          "phy_file_generation_is_enabled",
          &XFloat::phy_file_generation_is_enabled,
          &XFloat::set_phy_file_generation_is_enabled),

      sql_colt(
          XFloat,
          "start_of_mission_datetime",
          &XFloat::start_of_mission_datetime,
          &XFloat::set_start_of_mission_datetime),

      sql_cou8(
          XFloat,
          "xmsg_version_major",
          &XFloat::xmsg_version_major,
          &XFloat::set_xmsg_version_major),

      sql_cou8(
          XFloat,
          "xmsg_version_minor",
          &XFloat::xmsg_version_minor,
          &XFloat::set_xmsg_version_minor)

    });

bool XFloat::status_file_generation_is_enabled_;

path XFloat::status_directory_path_;

/* Function Definitions: */

/** The parameterized constructor.
 *
 *  @param[in]  imei            The unique International Mobile Equipment Identifier (IMEI).
 *  @param[in]  whoi            The unique WHOI identifier.
 *  @param[in]  wmo             The unique World Meteorological Organization (WMO) identifier.
 *  @param[in]  dac             The unique Data Assembly Center (DAC) identifier.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
XFloat::XFloat(int whoi, long long imei, int wmo, int dac) :
        Iridium(whoi, imei, wmo, dac),
        argo_instrument_type_(XARGO_INSRUMENT_TYPE_UNKNOWN),
        fw_version_(XMSG_BAD_U32),
        gain_pressure_(XFLT_GAIN_INVALID),
        gain_pressure_surface_(XFLT_GAIN_INVALID),
        gain_salinity_(XFLT_GAIN_INVALID),
        gain_salinity_surface_(XFLT_GAIN_INVALID),
        gain_temperature_(XFLT_GAIN_INVALID),
        gain_temperature_surface_(XFLT_GAIN_INVALID),
        kml_file_updated_(false),
        kml_document_updated_(false),
        latest_gps_position_datetime_(not_a_date_time),
        latest_gps_position_dive_number_(XFLT_DIVE_UNKNOWN),
        latest_gps_position_latitude_(XFLT_POS_INVALID_LAT),
        latest_gps_position_longitude_(XFLT_POS_INVALID_LON),
        latest_iridium_position_cep_radius_(-1),
        latest_iridium_position_datetime_(not_a_date_time),
        latest_iridium_position_dive_number_(XFLT_DIVE_UNKNOWN),
        latest_iridium_position_latitude_(XFLT_POS_INVALID_LAT),
        latest_iridium_position_longitude_(XFLT_POS_INVALID_LON),
        latest_iridium_sbd_momsn_(XFLT_ID_UNKNOWN),
        latest_iridium_sbd_mtmsn_(XFLT_ID_UNKNOWN),
        latest_iridium_startup_position_datetime_(not_a_date_time),
        latest_iridium_startup_sbd_momsn_(XFLT_ID_UNKNOWN),
        netcdf_file_generation_is_enabled_(false),
        offset_pressure_(XFLT_OFFS_INVALID),
        offset_pressure_surface_(XFLT_OFFS_INVALID),
        offset_salinity_(XFLT_OFFS_INVALID),
        offset_salinity_surface_(XFLT_OFFS_INVALID),
        offset_temperature_(XFLT_OFFS_INVALID),
        offset_temperature_surface_(XFLT_OFFS_INVALID),
        phy_file_generation_is_enabled_(false),
        status_file_updated_(false),
        start_of_mission_datetime_(not_a_date_time),
        xmsg_version_major_(XMSG_VERSION_UNKNOWN),
        xmsg_version_minor_(XMSG_VERSION_UNKNOWN) {
  /* Do nothing. */
}
#pragma GCC diagnostic pop

/** The default constructor.
 *
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
XFloat::XFloat() :
        Iridium(),
        argo_instrument_type_(XARGO_INSRUMENT_TYPE_UNKNOWN),
        fw_version_(XMSG_BAD_U32),
        gain_pressure_(XFLT_GAIN_INVALID),
        gain_pressure_surface_(XFLT_GAIN_INVALID),
        gain_salinity_(XFLT_GAIN_INVALID),
        gain_salinity_surface_(XFLT_GAIN_INVALID),
        gain_temperature_(XFLT_GAIN_INVALID),
        gain_temperature_surface_(XFLT_GAIN_INVALID),
        kml_file_updated_(false),
        kml_document_updated_(false),
        latest_gps_position_datetime_(not_a_date_time),
        latest_gps_position_dive_number_(XFLT_DIVE_UNKNOWN),
        latest_gps_position_latitude_(XFLT_POS_INVALID_LAT),
        latest_gps_position_longitude_(XFLT_POS_INVALID_LON),
        latest_iridium_position_cep_radius_(-1),
        latest_iridium_position_datetime_(not_a_date_time),
        latest_iridium_position_dive_number_(XFLT_DIVE_UNKNOWN),
        latest_iridium_position_latitude_(XFLT_POS_INVALID_LAT),
        latest_iridium_position_longitude_(XFLT_POS_INVALID_LON),
        latest_iridium_sbd_momsn_(XFLT_ID_UNKNOWN),
        latest_iridium_sbd_mtmsn_(XFLT_ID_UNKNOWN),
        latest_iridium_startup_position_datetime_(not_a_date_time),
        latest_iridium_startup_sbd_momsn_(XFLT_ID_UNKNOWN),
        netcdf_file_generation_is_enabled_(false),
        offset_pressure_(XFLT_OFFS_INVALID),
        offset_pressure_surface_(XFLT_OFFS_INVALID),
        offset_salinity_(XFLT_OFFS_INVALID),
        offset_salinity_surface_(XFLT_OFFS_INVALID),
        offset_temperature_(XFLT_OFFS_INVALID),
        offset_temperature_surface_(XFLT_OFFS_INVALID),
        phy_file_generation_is_enabled_(false),
        start_of_mission_datetime_(not_a_date_time),
        status_file_updated_(false),
        xmsg_version_major_(XMSG_VERSION_UNKNOWN),
        xmsg_version_minor_(XMSG_VERSION_UNKNOWN) {
  /* Do nothing. */
}
#pragma GCC diagnostic pop

/** The destructor.
 *
 */
XFloat::~XFloat() { /* Do nothing. */ }

/** Generate the Google Earth KML master document.
 *
 *  @return     The Google Earth KML master document.
 */
kmldom::DocumentPtr XFloat::GenerateKmlMasterDocument() {

  using kmldom::DocumentPtr;
  using kmldom::FolderPtr;

  DocumentPtr document;
  FolderPtr folder_active;
  FolderPtr folder_archive;

  shared_ptr<XFloat> flt;
  DocumentPtr flt_document;
  ptime latest_report;
  ptime now(DateUtils::GetCurrentUtcTime());

  try {

    /* Create KML Document: */

    document = GetKmlFactory()->CreateDocument();
    document->set_name("WHOI ALAMO/Hurricane Floats");

    // Create the document style.
    SetAllDefaultKmlSyles(document);

    // Create folders.
    folder_active = GetKmlFactory()->CreateFolder();
    folder_active->set_name("active");
    folder_active->set_visibility(true);
    folder_archive = GetKmlFactory()->CreateFolder();
    folder_archive->set_name("archive");
    folder_archive->set_visibility(false);

    /* Create Instrument Features: */

    vector<shared_ptr<XFloat> > floats(GetAllCachedFloats());
    for(auto it = floats.begin(); it != floats.end(); ++it) {

      // Get latest report date/time
      latest_report = (flt = *it)->latest_iridium_position_datetime();

      if(latest_report + boost::gregorian::date_duration(30) > now) {
        if((flt_document = flt->GenerateKmlDocument(false, true))) {
          // Add to active folder.
          flt_document->set_visibility(true);
          folder_active->add_feature(flt_document);
        }
      } else {
        if((flt_document = flt->GenerateKmlDocument(false, false))) {
          // Add to archive folder.
          flt_document->set_visibility(false);
          folder_archive->add_feature(flt_document);
        }
      }
    }

    // Add features to document.
    document->add_feature(folder_active);
    document->add_feature(folder_archive);

  } CATCH_BLOCK("Failed to generate KML master document object!")

  return document;

}

/** Generate the Google Earth KML master string.
 *
 *  @return     The Google Earth KML master string.
 */
string XFloat::GenerateKmlMasterString() {

  using kmldom::DocumentPtr;
  using kmldom::KmlPtr;
  using kmldom::SerializePretty;

  try {

    DocumentPtr document(GenerateKmlMasterDocument());

    /* Create the KML String. */

    KmlPtr kml(GetKmlFactory()->CreateKml());
    kml->set_feature(document);

    return SerializePretty(kml);

  } CATCH_BLOCK("Failed to generate KML master document string!")

  return string();
}

/** Write the KML master file.
 *
 */
void XFloat::WriteKmlMasterFile() {

  using namespace kmldom;
  using namespace kmlengine;

  path target_directory;

  // Check if KML file generation is enabled.
  if(!kml_file_generation_is_enabled_)
    return;

  try {

    // Get the KML directory.
    target_directory = GetDirectoryPathFromString(kml_directory_path_.string(),
    true);

    // Set KML file local path.
    path master_kml_file_path(target_directory += string("/master.kmz"));

    // Write  file.
    linf() << "Writing KML master file: " << master_kml_file_path << "...";
    KmzFile::WriteKmz(master_kml_file_path.c_str(), GenerateKmlMasterString().c_str());

  } CATCH_BLOCK("Failed to write KML master file!")
}

/** Get all float objects currently stored in the cache.
 *
 *  @return   A vector of shared pointers to the cached float objects.
 */
vector<shared_ptr<XFloat> > XFloat::GetAllCachedFloats() {

  vector<shared_ptr<XFloat> > floats;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(id2float_map_mutex_);

    // Copy the pointers to the instrument objects.
    for_each(
        id2float_map_.begin(),
        id2float_map_.end(),
        [&](const std::pair<int, shared_ptr<XFloat> > & pair) {
          floats.push_back(std::get<1>(pair));
        });

  } CATCH_BLOCK("Failed to get all cached instrument objects!")

  return floats;
}

/** Get a float object from the instrument identifier.
 *
 *  This function returns a shared reference to the float object identified by the unique
 *  instrument identifier. First, the function checks to see if the
 *  instrument-identifier-to-instrument-object map, @ref id2float_map_, is empty. It the map
 *  is empty, then the query the known float object from the SQL database and used the result
 *  of the query to populate the map.
 *
 *  Next, the function checks to see if the map contains the float object with the given
 *  instrument identifier. If it does not, then a new float object is created and added to the
 *  map. Finally, the shared reference to float object is returned.
 *
 *  @exception  util::log::MessageException if an error occurs while attempting to retrieve
 *              the float object with the give instrument identifier.
 *
 *  @note       This function places a lock guard on the
 *              instrument-identifier-to-instrument-object map mutual exclusion (mutex) lock,
 *              @ref id2float_map_mutex_, thereby locking the
 *              instrument-identifier-to-instrument-object map, @ref id2float_map_, and
 *              preventing access or modification of the map by any other thread. Any thread
 *              that does attempt to access or modify the map will block until the lock is
 *              released.
 *
 *  @param[in]  instrument_id       The unique instrument identifier.
 *
 *  @return     A shared reference to the float object with the given instrument identifier,
 *              @c instrument_id.
 */
shared_ptr<XFloat> XFloat::GetFloat(int instrument_id) {

  shared_ptr<XFloat> x;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(id2float_map_mutex_);

    // Check if local cache is empty.
    if(id2float_map_.empty())
      LoadLocalCacheFromDatabase();

    // Return a pointer the float of interest.
    x = id2float_map_[instrument_id] ?
        id2float_map_[instrument_id] :
        id2float_map_[instrument_id] = make_shared<XFloat>(instrument_id);

  } CATCH_BLOCK(
      "Failed to get XFloat object for instrument_id = " + lexical_cast<string>(instrument_id)
          + "!")

  return x;
}

/** Get the floats that the user has specified to disable PHY file generation.
 *
 *  @return A vector of floats that the user has specified to disable PHY file generation.
 */
const vector<unsigned int>& XFloat::instrument_ids_to_disable_phy_generation() {
  return instrument_ids_to_disable_phy_generation_;
}

/** Get the floats that the user has specified to enable PHY file generation.
 *
 *  @return A vector of floats that the user has specified to enable PHY file generation.
 */
const vector<unsigned int>& XFloat::instrument_ids_to_enable_phy_generation() {
  return instrument_ids_to_enable_phy_generation_;
}

/** Load all persistent instrument data from the database and use this data to populate the local
 *  cache.
 *
 *  @throw          util::log::MessageException if an error occurs while querying the database
 *                  or populating the local cache.
 */
void XFloat::LoadLocalCacheFromDatabase() {

  namespace attr = blg::attributes;

  try {

    lout() << "Loading persistent instrument data from the SQL database...";

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(id2float_map_mutex_);

    // Populate cache of XFloat objects from database.
    vector<shared_ptr<XFloat> > all(SelectAllObjectsFromDatabase());

    for_each(all.begin(), all.end(), [](shared_ptr<XFloat> x) {

      if(x) {

        // Set the global logger attributes.
        BOOST_LOG_SCOPED_THREAD_ATTR("WhoiID", attr::constant<int>(x->id_instrument()));
        BOOST_LOG_SCOPED_THREAD_ATTR("DacID", attr::constant<int>(x->id_dac()));
        BOOST_LOG_SCOPED_THREAD_ATTR("WmoID", attr::constant<int>(x->id_wmo()));

        // Add the instrument object to the id-to-instrument map.
        id2float_map_[x->id_instrument()] = x;

        linf() << "Loaded instrument data from database.";
      }
    });

    linf() << "Done. Successfully loaded persistent instrument data.";

  } CATCH_BLOCK("Failed to load local XFloat object cache from database!")
}

/** Load the instrument metadata.
 *
 *  This function loads the instruments' metadata from the metadata files on the local
 *  filesystem. It is called by the daemon thread during application initialization. A vector
 *  of metadata objects is created from the metadata files by calling @ref
 *  XFloatMetadata::LoadMetadataFromFiles(). The function then iterates over the metadata
 *  files and assigns each to the corresponding float object before persisting the metadata
 *  object to the SQL database.
 *
 *  @exception  util::log::MessageException if an error occurs while loading the instrument
 *              metadata from the files on the local file system.
 *
 *  @note       Metadata files that specify a DAC identifier of 9999 or -99 are used as place
 *              holders for instruments that have not yet been assigned a DAC identifier.
 *              Therefore, metadata files or objects specifying a DAC id less than 1, equal
 *              to 9999, or greater than 99998 will be discarded.
 */
void XFloat::LoadMetadataFromFiles() {

  namespace attr = blg::attributes;

  try {

    lout() << "Loading metadata from the local metadata files...";

    // Load metadata and assign to corresponding XFloat object.
    vector<shared_ptr<XFloatMetadata> > md(XFloatMetadata::LoadMetadataFromFiles());
    for_each(md.begin(), md.end(), [](shared_ptr<XFloatMetadata>& meta) {

      if(meta && meta->id_dac() > 0 && meta->id_dac() != 9999 && meta->id_dac() < 99999) {

        BOOST_LOG_SCOPED_THREAD_ATTR("WhoiID", attr::constant<int>(meta->id_instrument()));
        BOOST_LOG_SCOPED_THREAD_ATTR("DacID", attr::constant<int>(meta->id_dac()));
        BOOST_LOG_SCOPED_THREAD_ATTR("WmoID", attr::constant<int>(meta->id_wmo()));

        shared_ptr<XFloat> parent(GetFloat(meta->id_instrument()));
        parent->set_metadata(meta);
        parent->SaveObjectToDatabase();
        lout() << "Loaded instrument metadata.";

      }

    });

    linf() << "Done. Successfully loaded metadata from the local metadata files.";

  } CATCH_BLOCK("Failed to load instrument metadata!")

}

/** Save the local cache of X-Float objects to the database.
 *
 *  @throw          util::log::MessageException if an error occurs.
 */
void XFloat::SaveLocalCacheToDatabase() {
  try {
    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(id2float_map_mutex_);

    // Iterate over local cache.
    lout() << "Saving local cache of instrument data to database...";

    // Save each float separately, so one failure does not stop the remaining floats being saved.
    for_each(
        id2float_map_.begin(),
        id2float_map_.end(),
        [](const tuple<int, shared_ptr<XFloat> >& flt) {
          try {
            if(get<1>(flt))
              get<1>(flt)->SaveObjectToDatabase();
          } CATCH_NOTHROW("Failed to save instrument #" << get<0>(flt) << " to database!")
        });

    linf() << "Done. Local data saved successfully.";
  } CATCH_BLOCK("Failed to save local cache of X-Float objects!")
}

/** Select all XFloat objects from this SQL database table.
 *
 *  @param[in]      groupBy     The SQL select statement group-by clause.
 *  @param[in]      orderBy     The SQL select statement order-by clause.
 *  @param[in]      limit       The SQL select statement result set row limit value.
 *  @param[in]      offset      The SQL select statement result set row offset.
 *
 *  @throw          util::log::MessageException if an error occurs while querying the database.
 *
 *  @return         A vector of shared pointers to the object selected from the database.
 */
vector<shared_ptr<XFloat> > XFloat::SelectAllObjectsFromDatabase(
    const string& groupBy,
    const string& orderBy,
    const unsigned int& limit,
    const unsigned int& offset) {

  try {

    return sql_table_description_.SelectAllObjectsFromDatabase(groupBy, orderBy, limit, offset);

  } CATCH_BLOCK("Failed to query instrument objects from the database!")

  return vector<shared_ptr<XFloat> >();
}

/** Get a single XFloat object from the database.
 *
 *  @param[in]      where       The SQL select statement where-condition clause.
 *  @param[in]      groupBy     The SQL select statement group-by clause.
 *  @param[in]      orderBy     The SQL select statement order-by clause.
 *  @param[in]      offset      The SQL select statement result set row offset.
 *
 *  @throw          util::log::MessageException if an error occurs while querying the database.
 *
 *  @return         A shared pointer to the object selected from the database.
 */
shared_ptr<XFloat> XFloat::SelectObjectFromDatabase(
    const string& where,
    const string& groupBy,
    const string& orderBy,
    const unsigned int& offset) {

  try {

    return sql_table_description_.SelectObjectFromDatabase(where, groupBy, orderBy, offset);

  } CATCH_BLOCK("Failed to query instrument objects from the database!")

  return make_shared<XFloat>();
}

/** Query float objects from the SQL database.
 *
 *  This function is used to query the SQL database table corresponding to the float class
 *  and construct a vector of float objects, populated from the results of the SQL query.
 *
 *  @exception  util::log::MessageException if an error occurs while querying the database.
 *              or constructing the float objects from the result of the query.
 *
 *  @param[in]  where       The SQL select statement "where-condition" clause.
 *  @param[in]  groupBy     The SQL select statement "group-by expression" clause.
 *  @param[in]  orderBy     The SQL select statement "order-by expression" clause.
 *  @param[in]  limit       The SQL select statement result set row limit value.
 *  @param[in]  offset      The SQL select statement result set row offset value.
 *
 *  @return     A vector of shared references to the float objects constructed from the
 *              results of the SQL query.
 */
vector<shared_ptr<XFloat> > XFloat::SelectObjectsFromDatabase(
    const string& where,
    const string& groupBy,
    const string& orderBy,
    const unsigned int& limit,
    const unsigned int& offset) {

  try {

    return sql_table_description_.SelectObjectsFromDatabase(where, groupBy, orderBy, limit, offset);

  } CATCH_BLOCK("Failed to query instrument objects from the database!")

  return vector<shared_ptr<XFloat> >();
}

/** Select a value from the SQL database table that contains the persisted float objects.
 *
 *  This function may be used to select a singular value from the table that comprises the
 *  persisted float objects. For example, if an identifier is unknown, it is unnecessary to
 *  query the database to construct and entire float object to recover the identifier. Rather,
 *  this function may be used to recover the identifier without constructing a new object in
 *  the process.
 *
 *  @exception  util::log::MessageException if the SQL database query fails or if an error
 *              occurs while attempting to interpret the result of the query.
 *
 *  @note       The result set of this query is always limited to a single result. That is,
 *              the result set row limit value is automatically set to one. If more than one
 *              row in the database table satisfy the "where condition" clause of the select
 *              statement, then only the value of the first result will be returned.
 *
 *  @param[in]  what        The SQL select statement "what" clause. This clause may only
 *                          contain a singular result e.g. the name of the SQL database table
 *                          column of interest.
 *  @param[in]  where       The SQL select statement "where condition" clause.
 *  @param[in]  groupBy     The SQL select statement "group-by expression" clause.
 *  @param[in]  orderBy     The SQL select statement "order-by expression" clause.
 *  @param[in]  offset      The SQL select statement result set row offset value.
 *
 *  @return     The selected value from the SQL database table.
 */
template<typename TT> TT XFloat::SelectValueFromDatabase(
    const string& what,
    const string& where,
    const string& groupBy,
    const string& orderBy,
    const unsigned int& offset) {
  return sql_table_description_.SelectValueFromDatabase<TT>(what, where, groupBy, orderBy, offset);
}

template bool XFloat::SelectValueFromDatabase<bool>(
    const string& what,
    const string& where,
    const string& groupBy,
    const string& orderBy,
    const unsigned int& offset);

template double XFloat::SelectValueFromDatabase<double>(
    const string& what,
    const string& where,
    const string& groupBy,
    const string& orderBy,
    const unsigned int& offset);

template float XFloat::SelectValueFromDatabase<float>(
    const string& what,
    const string& where,
    const string& groupBy,
    const string& orderBy,
    const unsigned int& offset);

template int XFloat::SelectValueFromDatabase<int>(
    const string& what,
    const string& where,
    const string& groupBy,
    const string& orderBy,
    const unsigned int& offset);

template int16_t XFloat::SelectValueFromDatabase<int16_t>(
    const string& what,
    const string& where,
    const string& groupBy,
    const string& orderBy,
    const unsigned int& offset);

template long XFloat::SelectValueFromDatabase<long>(
    const string& what,
    const string& where,
    const string& groupBy,
    const string& orderBy,
    const unsigned int& offset);

template uint16_t XFloat::SelectValueFromDatabase<uint16_t>(
    const string& what,
    const string& where,
    const string& groupBy,
    const string& orderBy,
    const unsigned int& offset);

template uint8_t XFloat::SelectValueFromDatabase<uint8_t>(
    const string& what,
    const string& where,
    const string& groupBy,
    const string& orderBy,
    const unsigned int& offset);

/** Perform XFloat startup tasks.
 *
 *  If the user has specified that PHY file generation should be enabled or disabled for one or
 *  more floats, then PHY file generation flag will be updated by this function.
 */
void XFloat::StartupTasks() {

  shared_ptr<XFloat> inst;

  try {

    /* Change PHY file generations for specified instruments */

    try {
      for_each(
          instrument_ids_to_enable_phy_generation().begin(),
          instrument_ids_to_enable_phy_generation().end(),
          [&](unsigned int id) {

            lout() << "Enabling PHY file generation for instrument #" << id;

            inst = XFloat::GetFloat(id);
            inst->set_phy_file_generation_is_enabled(true);
            inst->SaveObjectToDatabase();
          });

    } CATCH_BLOCK("Failed to enable PHY file generation for specified instruments!")

    try {
      for_each(
          instrument_ids_to_disable_phy_generation().begin(),
          instrument_ids_to_disable_phy_generation().end(),
          [&](unsigned int id) {

            lout() << "Disabling PHY file generation for instrument #" << id;

            inst = XFloat::GetFloat(id);
            inst->set_phy_file_generation_is_enabled(false);
            inst->SaveObjectToDatabase();
          });

    } CATCH_BLOCK("Failed to disable PHY file generation for specified instruments!")
  } CATCH_BLOCK("Failed to run XFloat startup tasks!")
}

kmldom::DocumentPtr XFloat::GenerateKmlDocument(bool set_default_kml_style, bool visibility) const {

  using namespace kmldom;
  using x::XGPS;

  DocumentPtr document;

  try {

    shared_ptr<XFloatMetadata> meta(metadata());

    // Get GPS positions.
    vector<shared_ptr<XGPS> > gps_positions(GetGpsPositions());

    /* Create KML Document: */

    document = GetKmlFactory()->CreateDocument();
    document->set_name(lexical_cast<string>(id_instrument()));
    document->set_description(
        "WHOI ALAMO/Hurricane Float #" + lexical_cast<string>(id_instrument()));
    document->set_id(lexical_cast<string>(id_instrument()));

    // Create the document style.
    if(set_default_kml_style)
      SetDefaultKmlStyle(document, id_instrument());

    /* Create Placemark: Deployment Position */

    if(meta && meta->is_deployment_position_valid()) {

      // Create coordinates.
      CoordinatesPtr deployment_position_coordinates = GetKmlFactory()->CreateCoordinates();
      deployment_position_coordinates->add_latlngalt(
          meta->deployment_latitude(),
          meta->deployment_longitude(),
          2000);

      // Create point.
      PointPtr deployment_position_point = GetKmlFactory()->CreatePoint();
      deployment_position_point->set_coordinates(deployment_position_coordinates);
      deployment_position_point->set_altitudemode(kmldom::ALTITUDEMODE_RELATIVETOGROUND);
      deployment_position_point->set_extrude(true);
      deployment_position_point->set_altitudemode(ALTITUDEMODE_RELATIVETOGROUND);

      // Create deployment position placemark.
      PlacemarkPtr deployment_position_placemark = GetKmlFactory()->CreatePlacemark();
      deployment_position_placemark->set_name(
          lexical_cast<string>(id_instrument()) + " deployment position");
      deployment_position_placemark->set_geometry(deployment_position_point);
      deployment_position_placemark->set_styleurl(
          "#" + GetTriangleKmlPlacemarkStyleId(id_instrument()));
      deployment_position_placemark->set_visibility(false);

      document->add_feature(deployment_position_placemark);

    }

    /* Create Placemark: Trajectory Path */

    // Create the trajectory coordinates.
    CoordinatesPtr trajectory_coordinates = GetKmlFactory()->CreateCoordinates();
    for_each(gps_positions.begin(), gps_positions.end(), [&](const shared_ptr<XGPS>& gps_position) {
      trajectory_coordinates->add_latlng(
          gps_position->position_latitude(),
          gps_position->position_longitude());
    });

    // Create the trajectory line string.
    LineStringPtr trajectory_line_string = GetKmlFactory()->CreateLineString();
    trajectory_line_string->set_coordinates(trajectory_coordinates);
    trajectory_line_string->set_altitudemode(ALTITUDEMODE_RELATIVETOGROUND);

    // Create the trajectory placemark.
    PlacemarkPtr trajectory_placemark = GetKmlFactory()->CreatePlacemark();
    trajectory_placemark->set_name("trajectory");
    trajectory_placemark->set_geometry(trajectory_line_string);
    trajectory_placemark->set_styleurl("#" + GetDefaltKmlPathStyleId(id_instrument()));
    trajectory_placemark->set_visibility(visibility);

    // Add the trajectory placemark to the document.
    document->add_feature(trajectory_placemark);

    /* Create Placemark: Latest Position */

    if(!gps_positions.empty()) {

      // Create latest position coordinates.
      shared_ptr<XGPS> latest_gps_position = gps_positions.at(gps_positions.size() - 1);
      CoordinatesPtr latest_gps_position_coordinates = GetKmlFactory()->CreateCoordinates();
      latest_gps_position_coordinates->add_latlng(
          latest_gps_position->position_latitude(),
          latest_gps_position->position_longitude());

      // Create latest position description.

      // Create latest position point.
      PointPtr latest_postion_point = GetKmlFactory()->CreatePoint();
      latest_postion_point->set_coordinates(latest_gps_position_coordinates);
      latest_postion_point->set_altitudemode(ALTITUDEMODE_RELATIVETOGROUND);

      // Create latest position placemark.
      PlacemarkPtr latest_gps_position_placemark = GetKmlFactory()->CreatePlacemark();
      latest_gps_position_placemark->set_name(lexical_cast<string>(id_instrument()));
//                latest_gps_position_placemark->set_description("latest position");
      latest_gps_position_placemark->set_geometry(latest_postion_point);
      latest_gps_position_placemark->set_styleurl(
          "#" + GetDefaltKmlPlacemarkStyleId(id_instrument()));
      latest_gps_position_placemark->set_visibility(visibility);

      // Add latest position placemark to document.
      document->add_feature(latest_gps_position_placemark);

      // Create latest position description.
      ostringstream os;
//                os << "<![CDATA[";
      os << "<strong>"
         << "Most Recent Profile Position for WHOI ALAMO/Hurricane Float #"
         << id_instrument()
         << "</strong>"
         << endl;
      os << "<br />";
      os << "<p>"
         << "Profile #"
         << latest_gps_position->id_dive()
         << " reported on "
         << bpt::to_iso_extended_string(latest_gps_position->position_datetime())
         << "Z at "
         << latest_gps_position->postition_as_iso6709_string()
//         << "."
//         << id_instrument()
         << "</p>";
//                os << "]]>";
      latest_gps_position_placemark->set_description(os.str());
    }

    /* Create Placemarks: Profile Positions */

    // Create profile positions folder.
    FolderPtr folder = GetKmlFactory()->CreateFolder();
    folder->set_name("surface positions");

    // Iterate over the GPS positions.
    for_each(gps_positions.begin(), gps_positions.end(), [&](const shared_ptr<XGPS>& gps_position) {

      if(gps_position &&
          (gps_position->id_type() == 0 || gps_position->id_type() == 2)) {

        // Create coordinates.
        CoordinatesPtr position_coordinates =
        GetKmlFactory()->CreateCoordinates();
        position_coordinates->add_latlng(
            gps_position->position_latitude(),
            gps_position->position_longitude());

        // Create point.
        PointPtr position_point = GetKmlFactory()->CreatePoint();
        position_point->set_coordinates(position_coordinates);
        position_point->set_altitudemode(ALTITUDEMODE_RELATIVETOGROUND);

        // Create latest position placemark.
        PlacemarkPtr position_placemark = GetKmlFactory()->CreatePlacemark();
        ostringstream position_name;
        position_name
        << gps_position->id_dive()
        << ":"
        << bpt::to_iso_string(gps_position->position_datetime()).substr(0, 13)
        << "Z";
        position_placemark->set_name(position_name.str());
        position_placemark->set_geometry(position_point);
        position_placemark->set_styleurl(
            "#" + GetSmallKmlPlacemarkStyleId(id_instrument()));
        position_placemark->set_visibility(false);

        // Add position to folder.
        folder->add_feature(position_placemark);
      }
    });

    // Add folder to document.
    document->add_feature(folder);

  } CATCH_BLOCK("Failed to generate KML document object!")

  return document;
}

/** Generate the Google Earth KML string for this float.
 *
 *  @return     The Google Earth KML string for this float.
 */
string XFloat::GenerateKmlString() {

  using kmldom::DocumentPtr;
  using kmldom::KmlPtr;
  using kmldom::SerializePretty;

  try {

    // Update the KML document, if necessary.
    if(!is_kml_document_updated()) {
      set_kml_document(GenerateKmlDocument(true, true));
      set_kml_document_updated();
    }

    DocumentPtr document(kml_document());

    /* Create the KML String. */

    KmlPtr kml(GetKmlFactory()->CreateKml());
    kml->set_feature(document);

    return SerializePretty(kml);

  } CATCH_BLOCK("Failed to generate KML document string!")

  return string();
}

string XFloat::GenerateStatusString() const {
  ostringstream os;

  try {
    static const format fmt("%-39s %s");

    shared_ptr<XFloatMetadata> meta(metadata());

    /* Instrument Identifiers:  */

    os << format(fmt) % "ID_INST" % lexical_cast<string>(id_instrument()) << endl;
    os << format(fmt) % "ID_DAC" % lexical_cast<string>(id_dac()) << endl;
    os << format(fmt) % "ID_WMO" % lexical_cast<string>(id_wmo()) << endl;
    os << format(fmt) % "ID_IMEI" % lexical_cast<string>(id_imei()) << endl;

    /* File Generation: */

    os << format(fmt) % "PHY_GENERATION_ENABLED" % (phy_file_generation_is_enabled() ? "1" : "0")
       << endl;

    /* Deployment: */

    os << format(fmt) % "DEPLOYMENT_DATETIME"
           % (bpt::to_iso_extended_string(meta ? meta->deployment_datetime() : bpt::not_a_date_time))
       << endl;
    os << format(fmt) % "DEPLOYMENT_DATETIME_STATUS"
           % (meta ? meta->deployment_datetime_status() : "unknown")
       << endl;
    os << format(fmt) % "DEPLOYMENT_POSITION"
           % (meta ? meta->deployment_position_as_iso6709_string() : "+99.999999+999.999999")
       << endl;
    os << format(fmt) % "DEPLOYMENT_POSITION_STATUS"
           % (meta ? meta->deployment_position_status() : "unknown")
       << endl;
    os << format(fmt) % "DEPLOYMENT_CRUISE_ID" % (meta ? meta->deployment_cruise_id() : "unknown")
       << endl;
    os << format(fmt) % "DEPLOYMENT_PLATFORM" % (meta ? meta->deployment_platform() : "unknown")
       << endl;
    os << format(fmt) % "DEPLOYMENT_TYPE" % (meta ? meta->deployment_type() : "unknown") << endl;
    os << format(fmt) % "DEPLOYER" % (meta ? meta->deployer() : "unknown") << endl;
    os << format(fmt) % "DEPLOYER_ADDRESS" % (meta ? meta->deployer_address() : "unknown") << endl;

    os << format(fmt) % "LATEST_GPS_DIVE" % lexical_cast<string>(latest_gps_position_dive_number())
       << endl;
    os << format(fmt) % "LATEST_GPS_DATETIME"
           % bpt::to_iso_extended_string(latest_gps_position_datetime())
       << endl;
    os << format(fmt) % "LATEST_GPS_POSITION" % latest_gps_position_as_iso6709_string() << endl;

    os << format(fmt) % "LATEST_IRIDIUM_DIVE"
           % lexical_cast<string>(latest_iridium_position_dive_number())
       << endl;
    os << format(fmt) % "LATEST_IRIDIUM_DATETIME"
           % bpt::to_iso_extended_string(latest_iridium_position_datetime())
       << endl;
    os << format(fmt) % "LATEST_IRIDIUM_POSITION" % latest_iridium_position_as_iso6709_string()
       << endl;
    os << format(fmt) % "LATEST_SBD_MOMSN" % lexical_cast<string>(latest_sbd_momsn()) << endl;
    os << format(fmt) % "LATEST_SBD_MTMSN" % lexical_cast<string>(latest_sbd_mtmsn()) << endl;

//            os << format(fmt) % "" % lexical_cast<string>() << endl;
//            os << format(fmt) % "" % lexical_cast<string>() << endl;
//            os << format(fmt) % "" % lexical_cast<string>() << endl;

    os << format(fmt) % "START_OF_MISS_DATETIME"
           % bpt::to_iso_extended_string(start_of_mission_datetime())
       << endl;

    os << format(fmt) % "WMO_INSTRUMENT_TYPE"
           % (meta ? lexical_cast<string>(meta->wmo_instrument_type()) : "unknown")
       << endl;
    os << format(fmt) % "WMO_RECORDER_TYPE"
           % (meta ? lexical_cast<string>(meta->wmo_recorder_type()) : "unknown")
       << endl;

    os << format(fmt) % "XMSG_VERSION_MAJOR" % lexical_cast<string>((int) xmsg_version_major())
       << endl;
    os << format(fmt) % "XMSG_VERSION_MINOR" % lexical_cast<string>((int) xmsg_version_minor())
       << endl;

//            os << format(fmt) % "" % lexical_cast<string>() << endl;

  } CATCH_BLOCK("Failed to generate instrument status string!")

  return os.str();
}

/** Get all dive data objects for this float.
 *
 *  @return A vector of shared pointers to all dive data objects for this float.
 */
vector<shared_ptr<XFloatDiveData> > XFloat::GetAllDiveData() {

  shared_ptr<XFloatDiveData> dive_data;
  vector<shared_ptr<XFloatDiveData> > dive_datas;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    for(auto it = dive2data_map_.begin(); it < dive2data_map_.end(); ++it)
      if((dive_data = it->second))
        dive_datas.push_back(dive_data);

  } CATCH_BLOCK("Failed to get dive data objects for instrument!")

  return dive_datas;
}

/** Get the dive data object from the dive or cycle number.
 *
 *  This function returns a shared reference to the dive data object for this instrument
 *  identified by the unique dive or cycle number. First, the function checks if the dive data
 *  object exists in the dive-or-cycle-number-to-dive-data-object map, @ref dive2data_map_.
 *  If it does, then a shared reference to the object is returned.
 *
 *  Otherwise, if the dive data object does not exist in the map, the function attempts to
 *  query the SQL database, and populate a dive data object from the result of the SQL query.
 *  If successful, then the newly populated object is added to the map and a shared reference
 *  to the object is returned.
 *
 *  Finally, if the dive data object also does not exist in the SQL database, then a new dive
 *  data object will be constructed. The newly constructed dive data object is added to the
 *  dive-or-cycle-number-to-dive-data-object map, @ref dive2data_map_, and then persisted to
 *  the SQL database. A shared reference to the new object is returned.
 *
 *  @exception  util::log::MessageException if an error occurs while attempting to retrieve
 *              or construct the dive data object.
 *
 *  @note       This object will be locked for the scope of the execution of this function.
 *              This function places a lock guard on the mutual exclusion (mutex) lock,
 *              thereby locking the float object and preventing access or modification of the
 *              object by any other thread. Any thread that does attempt to access or modify
 *              this object will block until the lock is released.
 *
 *  @param[in]  dive        The cycle or dive number of interest.
 *  @param[in]  local_only  If TRUE, then the dive data object will only be loaded from the local
 *                          cache. Otherwise, the dive data object will be fetched from the SQL
 *                          database if necessary.
 *
 *  @return     The dive data object for a given dive.
 */
shared_ptr<XFloatDiveData> XFloat::GetDiveData(int dive, bool local_only) {

  namespace attr = blg::attributes;

  shared_ptr<XFloatDiveData> x;

  try {
    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    // Set scoped thread log attributes.
    BOOST_LOG_SCOPED_THREAD_ATTR("WhoiID", attr::constant<int>(id_instrument()));
    BOOST_LOG_SCOPED_THREAD_ATTR("DacID", attr::constant<int>(id_dac()));
    BOOST_LOG_SCOPED_THREAD_ATTR("WmoID", attr::constant<int>(id_wmo()));
    BOOST_LOG_SCOPED_THREAD_ATTR("DiveID", attr::constant<int>(dive));

    ldbg() << "Loading dive data object...";

    // Check if this dive data object exists in the local cache.
    if(dive2data_map_[dive]) {
      dbg1() << "Loaded dive data object from local cache.";
      return dive2data_map_[dive];
    }

    // Get the parent float object
    shared_ptr<XFloat> xfloat = GetFloat(id_instrument());

    if(!local_only) {

      // Query from the database.
      if((x = XFloatDiveData::GetDiveDataObjectFromDatabase(xfloat, dive))) {

        dbg1() << "Loaded dive data object from database. Fetching mail messages "
               "from database...";

        // Fetch existing messages and reprocess them.
        vector<shared_ptr<XFloatMailMessage> > msgs(x->GetDiveDataMailMessagesFromDatabase());
        x->set_iridium_position_datetime_first(not_a_date_time);
        x->set_iridium_position_datetime_last(not_a_date_time);
        x->set_num_sbd_msgs(0);
        for_each(msgs.begin(), msgs.end(), [&](shared_ptr<XFloatMailMessage>& msg) {

          // Set scoped thread log attributes.
            BOOST_LOG_SCOPED_THREAD_ATTR("MsgUID", attr::constant<int>(msg->id_msg_uid()));
            BOOST_LOG_SCOPED_THREAD_ATTR("PcktID", attr::constant<int>(msg->id_sbd_packet()));
            BOOST_LOG_SCOPED_THREAD_ATTR("MomsnID", attr::constant<int>(msg->id_sbd_momsn()));

            dbg2() << "Loaded mail message from database.";

            // Decode the mail message.
            x->DecodeMailMessageData(msg);
          });

        return (dive2data_map_[dive] = x);
      }
    }

    // Create a new dive data object.
    dbg1() << "Creating new dive data object...";
    x = dive2data_map_[dive] = make_shared<XFloatDiveData>(xfloat, dive);

  } CATCH_BLOCK("Failed to get dive data object for dive!")

  return x;
}

vector<shared_ptr<x::XGPS> > XFloat::GetGpsPositions() const {

  using x::XGPS;

  try {

    ostringstream where;
    where << "id_instrument = " << id_instrument() << " AND valid = 1 AND id_type <= 3";

    if(!start_of_mission_datetime().is_not_a_date_time())
      where << " AND datetime > '" << bpt::to_iso_string(start_of_mission_datetime()) << "'";

    shared_ptr<XFloatMetadata> meta(metadata());
    if(meta && !meta->deployment_datetime().is_not_a_date_time())
      where << " AND datetime > '" << bpt::to_iso_string(meta->deployment_datetime()) << "'";

    return XGPS::SelectObjectsFromDatabase(where.str(), "", "datetime ASC");

  } CATCH_BLOCK("Failed to get GPS position objects for instrument!")

  return vector<shared_ptr<x::XGPS> >();
}

/** Get the listing directory path for this float.
 *
 *  @return   The listing directory path for this float.
 */
path XFloat::GetListingDirectoryPath() const {

  static const format fmt("%s/%04d");

  ostringstream os;
  path path;

  try {

    os << format(fmt) % XFloatDiveData::intermediate_listing_directory_path_.string()
        % id_instrument();
    path = GetDirectoryPathFromString(os.str(), true);

  } CATCH_BLOCK("Failed to get listing file path!")

  return path;
}

/** Reset the dive data object for the given dive or cycle number.
 *
 *  This function releases the shared reference to the dive data object. It is called by the
 *  telemetry decoder thread once it determines that the dive data object is complete and no
 *  longer needed. Once all the shared references to the dive data object are released, then
 *  the object will be purged from memory.
 *
 *  @exception  None. Never throws.
 *
 *  @note       This object will be locked for the scope of the execution of this function.
 *              This function places a lock guard on the mutual exclusion (mutex) lock,
 *              thereby locking the float object and preventing access or modification of the
 *              object by any other thread. Any thread that does attempt to access or modify
 *              this object will block until the lock is released.
 *
 *  @param[in]  dive        The dive or cycle number of the dive data object to reset.
 */
void XFloat::ResetCachedDiveData(int dive) ATTR_NO_EXCEPTIONS {

  shared_ptr<XFloatDiveData> data;

  try {

//            // Reset the dive data object for the given dive.
//            if(dive2data_map_[dive]) {
//
//                // Get an exclusive lock.
//                lock_guard<recursive_mutex> guard(mutex_);
//
//                // Reset the pointer.
//                dive2data_map_[dive].reset();
//
//            }

//            // Wait for all other instances to be released.
//            for(size_t i = 0; i < 60 && data.use_count() > 1; ++i) {
//
//                boost::this_thread::sleep(boost::posix_time::time_duration(0, 0, 1));
//                ldbg() << "data.use_count(): " << (int) data.use_count();
//
//            }

  }CATCH_NOTHROW("Failed to reset dive data object for dive" + lexical_cast<string>(dive) + "!")
}

/** Reset the dive data objects for this float.
 *
 */
void XFloat::ResetDiveData() {

  vector<int> dive_nums;

  try {

    { // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      for_each(
          dive2data_map_.begin(),
          dive2data_map_.end(),
          [&](const pair< int, shared_ptr <XFloatDiveData> >& dive_pair) {
            dive_nums.push_back(dive_pair.first);
          });
    }

    // Reset cached dive data.
    for_each(dive_nums.begin(), dive_nums.end(), [&](int dive_num) {
      ResetCachedDiveData(dive_num);
    });

  } CATCH_BLOCK("Failed to reset instrument data!")

}

/** Save this float object to the database.
 *
 *  This function is used to persist the float object to the SQL database. If the float object
 *  has a non-NULL reference to a metadata object, then the metadata object will be persisted
 *  to the database as well.
 *
 *  @exception  util::log::MessageException if an error occurs while saving this float object
 *              or its metadata object to the database.
 *
 *  @note       This object will be locked for the scope of the execution of this function.
 *              This function places a lock guard on the mutual exclusion (mutex) lock,
 *              thereby locking the float object and preventing access or modification of the
 *              object by any other thread. Any thread that does attempt to access or modify
 *              this object will block until the lock is released.
 */
void XFloat::SaveObjectToDatabase() const {

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    ldbg() << "Saving instrument data to database...";
    sql_table_description_.SaveObjectToDatabase(*this);
    if(metadata_)
      metadata_->SaveObjectToDatabase();

  } CATCH_BLOCK("Failed to save instrument object to the database!")
}

/** Write the Google Earth KML file for this float.
 *
 *  @return   TRUE if the file was written, or FALSE if KML file generation is disabled or the KML
 *            file is already up-to-date.
 */
bool XFloat::WriteKmlFile() {

  using namespace kmldom;
  using namespace kmlengine;

  path target_directory;

  // Check if KML file generation is enabled.
  if(!kml_file_generation_is_enabled_)
    return false;

  try {

    // Check KML file status.
    if(is_kml_file_updated())

      return false;

    // Get the KML directory.
    target_directory = GetDirectoryPathFromString(kml_directory_path_.string(), true);

    // Set KML file local path.
    ostringstream os;
    os << format("/%04d.kmz") % id_instrument();
    set_kml_file_path(target_directory += os.str());

    // Write  file.
    linf() << "Writing KML file: " << kml_file_path() << "...";
    KmzFile::WriteKmz(kml_file_path().c_str(), GenerateKmlString().c_str());

    // Update KML file status.
    set_kml_file_updated();

  } CATCH_BLOCK("Failed to write KML file!")

  return true;
}

/** Write the status file for this float.
 *
 */
void XFloat::WriteStatusFile() {

  path target_directory;

  // Check if status file generation is enabled.
  if(!status_file_generation_is_enabled_)
    return;

  try {

    // Check file status.
    if(is_status_file_updated())

      return;

    // Get the status file directory.
    target_directory = GetDirectoryPathFromString(status_directory_path_.string(),
    true);

    // Set status file local path.
    ostringstream os;
    os << format("/stat%04d.txt") % id_instrument();
    set_status_file_path(target_directory += os.str());

    // Write  file.
    linf() << "Writing status file: " << status_file_path() << "...";
    ofstream of(status_file_path());
    of << GenerateStatusString();
    of.flush();
    of.close();

    // Update file status.
    set_status_file_updated();

  } CATCH_BLOCK("Failed to write instrument status file!")
}

/* Function Definitions - XFloatDiveData: */

/** Parameterized constructor.
 *
 *  @param[in]      parent      A pointer to the float object that owns this dive data.
 *  @param[in]      dive        The cycle or dive number of this dive.
 *
 *  @throw          util::log::MessageException if an error occurs during construction, or if
 *                  xFloat is NULL.
 */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreorder"
XFloatDiveData::XFloatDiveData(const shared_ptr<XFloat>& parent, int dive) :
        Iridium(
            parent ? parent->id_instrument() : XFLT_ID_UNKNOWN,
            parent ? parent->id_imei() : XFLT_ID_UNKNOWN,
            parent ? parent->id_wmo() : XFLT_ID_UNKNOWN,
            parent ? parent->id_dac() : XFLT_ID_UNKNOWN),
        argo_instrument_type_(XARGO_INSRUMENT_TYPE_UNKNOWN),
        complete_(false),
        id_dive_(dive),
        iridium_position_datetime_first_(not_a_date_time),
        iridium_position_datetime_last_(not_a_date_time),
        num_sbd_msgs_(0),
        pres_offs_(0.0),
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
        xmsg_mission_cfg_(),
        xmsg_surface_time_series_() {

  try {

    // Check the pointer to the instrument object.
    if(!parent)
      EXC_THROW("Null pointer to parent X-Float object!");

    // Assign the parent instrument object.
    AssignParentInstrument(parent);

  } CATCH_BLOCK("Failed to construct dive data object!")
}
#pragma GCC diagnostic pop

/** Assign an Argo data message to this dive data object.
 *
 *  @param[in]      x       A pointer to the Argo data message to assign.
 */
void XFloatDiveData::AssignArgoData(const shared_ptr<x::XArgo>& x) {

  {   // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    xmsg_argo_data_ = x;

    // Update file status.
    set_listing_file_updated(false);
  }

  parent_instrument_->set_argo_instrument_type((int) x->argo_instrument_type());
  parent_instrument_->set_gain_pressure(x->gain_pressure());
  parent_instrument_->set_gain_pressure_surface(x->gain_pressure_surface());
  parent_instrument_->set_gain_salinity(x->gain_salinity());
  parent_instrument_->set_gain_salinity_surface(x->gain_salinity_surface());
  parent_instrument_->set_gain_temperature(x->gain_temperature());
  parent_instrument_->set_gain_temperature_surface(x->gain_temperature_surface());
  parent_instrument_->set_offset_pressure(x->offset_pressure());
  parent_instrument_->set_offset_pressure_surface(x->offset_pressure_surface());
  parent_instrument_->set_offset_salinity(x->offset_salinity());
  parent_instrument_->set_offset_salinity_surface(x->offset_salinity_surface());
  parent_instrument_->set_offset_temperature(x->offset_temperature());
  parent_instrument_->set_offset_temperature_surface(x->offset_temperature_surface());
  parent_instrument_->set_xmsg_version_major(x->x_version_major());
  parent_instrument_->set_xmsg_version_minor(x->x_version_minor());
  parent_instrument_->SaveObjectToDatabase();

}

/** Assign a GPS message to this dive data object.
 *
 *  @note           If the GPS message is the startup/BIT GPS message, then the position date/time
 *                  will be used to set the parent start-of-mission time.
 *
 *  @param[in]      x           The GPS message to set.
 */
void XFloatDiveData::AssignGps(shared_ptr<x::XGPS>& x) {

  using namespace x;

  // Get the instrument object.
  shared_ptr<XFloat> inst(parent_instrument());

  try {

    // Double check if the position is valid.
    if(x->position_is_valid()) {

      // Check the position time-stamps.
      if(x->position_datetime() > iridium_position_datetime_last()
          || x->position_datetime() > GetCurrentUtcTime()) {

        // The position is NOT valid.
        x->set_position_is_valid(false);
        x->set_position_datetime(not_a_date_time);
        x->set_position_latitude(XFLT_POS_INVALID_LAT);
        x->set_position_longitude(XFLT_POS_INVALID_LON);
      }

    }

    try {

      // Update the start-of-mission date/time, if possible.
      if(x->id_xmsg_type() == XMSG_TYPE_ID_GPS_BITEST
          && !x->position_datetime().is_not_a_date_time()
          && (inst->start_of_mission_datetime().is_not_a_date_time()
              || inst->start_of_mission_datetime() < x->position_datetime())) {

        // Update the start-of-mission date/time and save the object to the database.
        inst->set_start_of_mission_datetime(x->position_datetime());
        inst->SaveObjectToDatabase();
      }

    }CATCH_NOTHROW("Failed to update instrument start-of-mission date/time!")

    try {

      // Update the latest GPS position, date/time, etc.
      if(x->position_is_valid()
          && (inst->latest_gps_position_datetime().is_not_a_date_time()
              || inst->latest_gps_position_datetime() < x->position_datetime())) {
        inst->set_latest_gps_position_datetime(x->position_datetime());
        inst->set_latest_gps_position_dive_number(x->id_dive());
        inst->set_latest_gps_position_latitude(x->position_latitude());
        inst->set_latest_gps_position_longitude(x->position_longitude());
      }

    }CATCH_NOTHROW("Failed to update latest position information for instrument!")

    // Check the ID.
    if(x->id_xmsg_type() > 5)
      EXC_THROW("Invalid GPS message identifier!");

    { /* Assign the GPS message to this dive: */

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      // Iterate to emplacement position.
      auto it = xmsg_gps_positions_.begin();
      for(; it < xmsg_gps_positions_.end(); ++it)

        if(x->id_xmsg_type() < (*it)->id_xmsg_type())

          // Emplacement position found.
          break;

      // Emplace element at position.
      xmsg_gps_positions_.emplace(it, x);

      // Update file status.
      set_listing_file_updated(false);
    }

  } CATCH_BLOCK("Failed to assign GPS message to dive data object!")
}

/** Set the shared pointer to the parent float object that owns this dive data.
 *
 *  @note           This dive data object will be locked for the scope of this function.
 *
 *  @param[in]      parent      The shared pointer to the parent float object to set.
 */
void XFloatDiveData::AssignParentInstrument(const shared_ptr<XFloat>& parent) {

  try {

    {   // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      // Check and assign pointer.
      if(!(parent_instrument_ = parent))
        EXC_THROW("Pointer to parent XFloat object is NULL!");
    }

    // Check that WHOI id matches parent XFloat object.
    if(id_instrument() != parent->id_instrument()) {
      ostringstream os;
      os << "Instrument serial number mismatch! Expected serial number "
         << id_instrument()
         << ", but received "
         << parent->id_instrument();
      EXC_THROW(os.str());
    }

    // Ensure proper unique identifiers.
    if(id_dac() == XFLT_ID_UNKNOWN)
      set_id_dac(parent->id_dac());
    if(id_imei() == XFLT_ID_UNKNOWN)
      set_id_imei(parent->id_imei());
    if(id_wmo() == XFLT_ID_UNKNOWN)
      set_id_wmo(parent->id_wmo());

  } CATCH_BLOCK("Failed to assign XFloat parent object to XFloatDiveData object!")
}

/** Save this dive data object to the SQL database.
 *
 *  The parent instrument object is saved first, so that its row exists before the dive row that
 *  references it. Otherwise, the dive data for a float that is not yet in the database would fail
 *  the foreign key constraint.
 *
 *  @throw      util::log::MessageException if an error occurs while saving the dive data or GPS
 *              message objects.
 */
void XFloatDiveData::SaveObjectToDatabase() const {

  using x::XGPS;

  /* Save the parent instrument without holding this object's lock, so the lock order stays
   * instrument-then-dive, as in XFloat::GetDiveData(). A failure here is logged, and the dive is
   * still saved, since the instrument row may already exist. */
  try {

    shared_ptr<XFloat> inst(parent_instrument());
    if(inst)
      inst->SaveObjectToDatabase();

  }CATCH_NOTHROW("Failed to save parent instrument object to database before dive!")

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    ldbg() << "Saving dive data object to database...";

    sql_table_description_.SaveObjectToDatabase(*this);

  }CATCH_BLOCK("Failed to save dive data object to database!")

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    for_each(
        xmsg_gps_positions_.begin(),
        xmsg_gps_positions_.end(),
        [&](const shared_ptr<XGPS>& x) {
          if(x) x->SaveObjectToDatabase();
        });

  }CATCH_BLOCK("Failed to save GPS message objects to database for dive!")
}

/** Decode the SBD data of a mail message into this dive data object.
 *
 *  Messages transmitted more than 15 minutes before the instrument's start-of-mission or
 *  deployment date/time are ignored. Otherwise, the mail message is assigned to this dive (see
 *  @ref AssignMailMessage()), its SBD data is decoded into X-messages (see @ref DecodeXMsgData()),
 *  the X-message type identifiers are stored in the mail message, and the parent instrument's
 *  IMEI and latest Iridium position are updated.
 *
 *  A message with a duplicate message ID is not assigned or decoded. A retransmission (a message
 *  whose SBD data is identical to one already assigned to this dive) is assigned, but its SBD data
 *  is not decoded again.
 *
 *  @param[in]  msg     The mail message to decode.
 *
 *  @throw      util::log::MessageException if @p msg is NULL or an error occurs while decoding.
 *
 *  @return     1 if the message is ignored because it was transmitted before the start-of-mission
 *              or deployment date/time, or 0 otherwise (including duplicates and retransmissions).
 */
int XFloatDiveData::DecodeMailMessageData(shared_ptr<XFloatMailMessage>& msg) {

  namespace attr = blg::attributes;

  static const time_duration window(0, 15, 0);

  shared_ptr<vector<uint8_t> > xmsg_id_types = make_shared<vector<uint8_t> >();
  shared_ptr<XFloatMetadata> metadata;

  try {

    if(!msg)
      EXC_THROW("Null pointer to mail message object!");

    // Set scoped thread log attributes.
    BOOST_LOG_SCOPED_THREAD_ATTR("WhoiID", attr::constant<int>(msg->id_instrument()));
    BOOST_LOG_SCOPED_THREAD_ATTR("DacID", attr::constant<int>(msg->id_dac()));
    BOOST_LOG_SCOPED_THREAD_ATTR("WmoID", attr::constant<int>(msg->id_wmo()));
    BOOST_LOG_SCOPED_THREAD_ATTR("DiveID", attr::constant<int>(msg->id_dive()));
    BOOST_LOG_SCOPED_THREAD_ATTR("MsgUID", attr::constant<int>(msg->id_msg_uid()));
    BOOST_LOG_SCOPED_THREAD_ATTR("MomsnID", attr::constant<int>(msg->id_sbd_momsn()));
    BOOST_LOG_SCOPED_THREAD_ATTR("PcktID", attr::constant<int>(msg->id_sbd_packet()));

    // Skip messages that were transmitted before the start-of-mission date/time.
    if(!parent_instrument()->start_of_mission_datetime().is_not_a_date_time()) {

      if(!msg->iridium_position_datetime().is_not_a_date_time()
          && (parent_instrument()->start_of_mission_datetime() - window)
              > msg->iridium_position_datetime()) {

        lwrn() << "Ignoring message transmitted before the start-of-mission date/time.";

        return 1;

      } else if(!msg->msg_header_datetime().is_not_a_date_time()
          && (parent_instrument()->start_of_mission_datetime() - window)
              > msg->msg_header_datetime()) {

        lwrn() << "Ignoring message transmitted before the start-of-mission date/time.";

        return 1;
      }
    }

    // Skip messages that were transmitted before the deployment date/time.
    metadata = parent_instrument()->metadata();
    if(metadata && !metadata->deployment_datetime().is_not_a_date_time()
        && !msg->iridium_position_datetime().is_not_a_date_time()
        && (metadata->deployment_datetime() - window) > msg->iridium_position_datetime()) {

      lwrn() << "Ignoring message transmitted before the deployment date/time.";

      return 1;
    }

    // Assign the mail message.
    switch(AssignMailMessage(msg)) {

      case 1:

        // Ignore duplicate mail messages.
        return 0;

      case 2:

        // Do not decode the SBD data of a retransmitted message again.
        break;

      default:

        // Decode the message data.
        DecodeXMsgData(msg->sbd_data(), xmsg_id_types);

        // Assign the xmsg_id_types.
        msg->set_xmsg_id_types(xmsg_id_types);

        break;
    }

    // Ensure IMEI is correct.
    if(parent_instrument()->id_imei() != msg->id_imei())
      parent_instrument()->set_id_imei(msg->id_imei());

    // Update the latest Iridium/SBD position, date/time, etc.
    if(msg->iridium_position_is_valid()
        && (parent_instrument()->latest_iridium_position_datetime().is_not_a_date_time()
            || parent_instrument()->latest_iridium_position_datetime()
                < msg->iridium_position_datetime())) {

      parent_instrument()->set_latest_iridium_position_cep_radius(
          msg->iridium_position_cep_radius());
      parent_instrument()->set_latest_iridium_position_datetime(msg->iridium_position_datetime());
      parent_instrument()->set_latest_iridium_position_dive_number(msg->id_dive());
      parent_instrument()->set_latest_iridium_position_latitude(msg->iridium_position_latitude());
      parent_instrument()->set_latest_iridium_position_longitude(msg->iridium_position_longitude());
      parent_instrument()->set_latest_sbd_momsn(msg->id_sbd_momsn());
      parent_instrument()->set_latest_sbd_mtmsn(msg->id_sbd_mtmsn());

    }

  } CATCH_BLOCK("Failed to decode mail message data!")

  return 0;
}

/** Decode an X-Message from the binary message data.
 *
 *  @exception  None. Never throws.
 *
 *  @param[in]  data            The binary message data to decode.
 *  @param[out] xmsg_id_types   A list of the x-message type identifiers contained in the @c data
 *                              message.
 */
void XFloatDiveData::DecodeXMsgData(
    const vector<uint8_t>& data,
    shared_ptr<vector<uint8_t> > xmsg_id_types) ATTR_NO_EXCEPTIONS {

  namespace attr = blg::attributes;
  using namespace x;

  size_t xlen = 0;
  vector<uint8_t> xdata;
  unsigned int packet_idx = 1;
  uint8_t xmsg_id_type;

  try {

    linf() << "Decoding message data...";

    // Check data length.
    if(data.size() < 8) {
      ostringstream os;
      os << ": Invalid X-Message size! Expected at least 8 bytes, but received only "
         << data.size()
         << " bytes!";
      EXC_THROW(os.str());
    }

    /** @todo   Verify the X-Message checksum value  during the decoding of the X-Message
     *          data.
     */

    // Iterate over message bytes.
    for(size_t i = 0; i < data.size() - 4 /* Ignoring checksum. */; ++i) {

      if(i < 8)
        continue;

      // Append byte to message to be processed.
      xdata.push_back(data.at(i));

      // Check for a complete X-Message.
      if(xdata.size() < 3) {

        xlen = 0;
        continue;

      } else if(xdata.size() == 3) { // Get the sub-message size.

        xlen = 0x0FFF & u16(xdata.data(), 1);

      } else if(xdata.size() >= xlen) { // X-message received, now decode the message.

        try {

          // Get the message type identifier.
          if(xmsg_id_types) xmsg_id_types->push_back(xmsg_id_type = xdata.at(0));

          // Set the scoped thread log attribute.
          ostringstream osXid;
          osXid << "0x" << hex << noshowbase << setw(2) << setfill('0') << uppercase << (int) xmsg_id_type;
          BOOST_LOG_SCOPED_THREAD_ATTR("XmsgID", attr::constant<string>(osXid.str()));

          // Decode the message.
          ldbg() << "Decoding packet " << packet_idx++ << " data: " << endl << GenerateReadableHex(xdata);
          switch(xmsg_id_type) {
            case XMSG_TYPE_ID_GPS_DIAGNOSTIC:    // 0x00
            case XMSG_TYPE_ID_GPS_STARTOFDIVE:
            case XMSG_TYPE_ID_GPS_ENDOFDIVE:
            case XMSG_TYPE_ID_GPS_ABORT:
            case XMSG_TYPE_ID_GPS_BITEST:

              try {

                /* Decode GPS message: */

                linf() << "Decoding GPS message...";
                shared_ptr<XGPS> x(std::make_shared<XGPS>(id_dive(), id_instrument(), xdata));
                AssignGps(x);

              } CATCH_BLOCK("Failed to decode GPS message!")

              break;

            case XMSG_TYPE_ID_PRES0:    // 0x10
            case XMSG_TYPE_ID_PRES1:
            case XMSG_TYPE_ID_PRES2:
            case XMSG_TYPE_ID_PRES3:
            case XMSG_TYPE_ID_PRES4:
            case XMSG_TYPE_ID_PRES5:
            case XMSG_TYPE_ID_PRES6:
            case XMSG_TYPE_ID_PRES7:
            case XMSG_TYPE_ID_PRES8:
            case XMSG_TYPE_ID_PRES9:
            case XMSG_TYPE_ID_PRESA:
            case XMSG_TYPE_ID_PRESB:
            case XMSG_TYPE_ID_PRESC:
            case XMSG_TYPE_ID_PRESD:
            case XMSG_TYPE_ID_PRESE:
            case XMSG_TYPE_ID_PRESF:

              try {

                /* Decode profile pressure: */

                linf() << "Decoding profile pressure (bin-averaged) message...";
                AssignProfilePressureBinned(std::make_shared<XProfilePressure>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode profile pressure (bin-averaged) message!")

              break;

            case XMSG_TYPE_ID_TEMP0:    // 0x20
            case XMSG_TYPE_ID_TEMP1:
            case XMSG_TYPE_ID_TEMP2:
            case XMSG_TYPE_ID_TEMP3:
            case XMSG_TYPE_ID_TEMP4:
            case XMSG_TYPE_ID_TEMP5:
            case XMSG_TYPE_ID_TEMP6:
            case XMSG_TYPE_ID_TEMP7:
            case XMSG_TYPE_ID_TEMP8:
            case XMSG_TYPE_ID_TEMP9:
            case XMSG_TYPE_ID_TEMPA:
            case XMSG_TYPE_ID_TEMPB:
            case XMSG_TYPE_ID_TEMPC:
            case XMSG_TYPE_ID_TEMPD:
            case XMSG_TYPE_ID_TEMPE:
            case XMSG_TYPE_ID_TEMPF:

              try {

                /* Decode profile temperature: */

                linf() << "Decoding profile temperature (bin-averaged) message...";
                shared_ptr<XProfileTemperature> temp = std::make_shared<XProfileTemperature>(id_dive(), id_instrument(), xdata);
                if(temp->is_aux_channel()) AssignAuxTemperatureBinned(temp);
                else AssignProfileTemperatureBinned(temp);

              } CATCH_BLOCK("Failed to decode profile temperature (bin-averaged) message!")

              break;

            case XMSG_TYPE_ID_SALN0:    // 0x30
            case XMSG_TYPE_ID_SALN1:
            case XMSG_TYPE_ID_SALN2:
            case XMSG_TYPE_ID_SALN3:
            case XMSG_TYPE_ID_SALN4:
            case XMSG_TYPE_ID_SALN5:
            case XMSG_TYPE_ID_SALN6:
            case XMSG_TYPE_ID_SALN7:
            case XMSG_TYPE_ID_SALN8:
            case XMSG_TYPE_ID_SALN9:
            case XMSG_TYPE_ID_SALNA:
            case XMSG_TYPE_ID_SALNB:
            case XMSG_TYPE_ID_SALNC:
            case XMSG_TYPE_ID_SALND:
            case XMSG_TYPE_ID_SALNE:
            case XMSG_TYPE_ID_SALNF:

              try {

                /* Decode profile salinity: */

                linf() << "Decoding profile salinity (bin-averaged) message...";
                AssignProfileSalinityBinned(std::make_shared<XProfileSalinity>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode profile salinity (bin-averaged) message!")

              break;

            case XMSG_TYPE_ID_FALL0:    // 0x40
            case XMSG_TYPE_ID_FALL1:
            case XMSG_TYPE_ID_FALL2:
            case XMSG_TYPE_ID_FALL3:
            case XMSG_TYPE_ID_FALL4:
            case XMSG_TYPE_ID_FALL5:
            case XMSG_TYPE_ID_FALL6:
            case XMSG_TYPE_ID_FALL7:
            case XMSG_TYPE_ID_FALL8:
            case XMSG_TYPE_ID_FALL9:
            case XMSG_TYPE_ID_FALLA:
            case XMSG_TYPE_ID_FALLB:
            case XMSG_TYPE_ID_FALLC:
            case XMSG_TYPE_ID_FALLD:
            case XMSG_TYPE_ID_FALLE:
            case XMSG_TYPE_ID_FALLF:

              try {

                /* Decode fall data: */

                linf() << "Decoding fall time-series message...";
                AssignFallTimeSeries(std::make_shared<XFall>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode fall message!")

              break;

            case XMSG_TYPE_ID_RISE0:    // 0x50
            case XMSG_TYPE_ID_RISE1:
            case XMSG_TYPE_ID_RISE2:
            case XMSG_TYPE_ID_RISE3:
            case XMSG_TYPE_ID_RISE4:
            case XMSG_TYPE_ID_RISE5:
            case XMSG_TYPE_ID_RISE6:
            case XMSG_TYPE_ID_RISE7:
            case XMSG_TYPE_ID_RISE8:
            case XMSG_TYPE_ID_RISE9:
            case XMSG_TYPE_ID_RISEA:
            case XMSG_TYPE_ID_RISEB:
            case XMSG_TYPE_ID_RISEC:
            case XMSG_TYPE_ID_RISED:
            case XMSG_TYPE_ID_RISEE:
            case XMSG_TYPE_ID_RISEF:

              try {

                /* Decode rise data: */

                linf() << "Decoding rise time-series message...";
                AssignRiseTimeSeries(std::make_shared<XRise>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode rise message!")

              break;

            case XMSG_TYPE_ID_PUMP0:    // 0x60
            case XMSG_TYPE_ID_PUMP1:
            case XMSG_TYPE_ID_PUMP2:
            case XMSG_TYPE_ID_PUMP3:
            case XMSG_TYPE_ID_PUMP4:
            case XMSG_TYPE_ID_PUMP5:
            case XMSG_TYPE_ID_PUMP6:
            case XMSG_TYPE_ID_PUMP7:
            case XMSG_TYPE_ID_PUMP8:
            case XMSG_TYPE_ID_PUMP9:
            case XMSG_TYPE_ID_PUMPA:
            case XMSG_TYPE_ID_PUMPB:
            case XMSG_TYPE_ID_PUMPC:
            case XMSG_TYPE_ID_PUMPD:
            case XMSG_TYPE_ID_PUMPE:
            case XMSG_TYPE_ID_PUMPF:

              try {

                /* Decode pump data: */

                linf() << "Decoding pump time-series message...";
                AssignPumpTimeSeries(std::make_shared<XPump>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode pump message!")

              break;

            case XMSG_TYPE_ID_PRES_SURF0:    // 0x80
            case XMSG_TYPE_ID_PRES_SURF1:
            case XMSG_TYPE_ID_PRES_SURF2:
            case XMSG_TYPE_ID_PRES_SURF3:
            case XMSG_TYPE_ID_PRES_SURF4:
            case XMSG_TYPE_ID_PRES_SURF5:
            case XMSG_TYPE_ID_PRES_SURF6:
            case XMSG_TYPE_ID_PRES_SURF7:
            case XMSG_TYPE_ID_PRES_SURF8:
            case XMSG_TYPE_ID_PRES_SURF9:
            case XMSG_TYPE_ID_PRES_SURFA:
            case XMSG_TYPE_ID_PRES_SURFB:
            case XMSG_TYPE_ID_PRES_SURFC:
            case XMSG_TYPE_ID_PRES_SURFD:
            case XMSG_TYPE_ID_PRES_SURFE:
            case XMSG_TYPE_ID_PRES_SURFF:

              try {

                /* Decode surface pressure time-series data: */

                linf() << "Decoding surface pressure time-series message...";
                AssignSurfaceTimeSeries(std::make_shared<XSurfaceData>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode surface pressure message!")

              break;

            case XMSG_TYPE_ID_PRES_HI_RES0:    // 0x90
            case XMSG_TYPE_ID_PRES_HI_RES1:
            case XMSG_TYPE_ID_PRES_HI_RES2:
            case XMSG_TYPE_ID_PRES_HI_RES3:
            case XMSG_TYPE_ID_PRES_HI_RES4:
            case XMSG_TYPE_ID_PRES_HI_RES5:
            case XMSG_TYPE_ID_PRES_HI_RES6:
            case XMSG_TYPE_ID_PRES_HI_RES7:

              try {

                /* Decode high-resolution pressure data: */

                linf() << "Decoding profile pressure (unbinned) message...";
                AssignProfilePressureUnbinned(std::make_shared<XHiResPressure>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode profile pressure (unbinned) message!")

              break;

            case XMSG_TYPE_ID_PRES_DRIFT8:    // 0x98
            case XMSG_TYPE_ID_PRES_DRIFT9:
            case XMSG_TYPE_ID_PRES_DRIFTA:
            case XMSG_TYPE_ID_PRES_DRIFTB:
            case XMSG_TYPE_ID_PRES_DRIFTC:
            case XMSG_TYPE_ID_PRES_DRIFTD:
            case XMSG_TYPE_ID_PRES_DRIFTE:
            case XMSG_TYPE_ID_PRES_DRIFTF:

              try {

                /* Decode drift pressure: */

                linf() << "Decoding drift pressure message...";
                AssignDriftPressure(std::make_shared<XDriftPressure>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode drift pressure message!")

              break;

            case XMSG_TYPE_ID_TEMP_HI_RES0:    // 0xA0
            case XMSG_TYPE_ID_TEMP_HI_RES1:
            case XMSG_TYPE_ID_TEMP_HI_RES2:
            case XMSG_TYPE_ID_TEMP_HI_RES3:
            case XMSG_TYPE_ID_TEMP_HI_RES4:
            case XMSG_TYPE_ID_TEMP_HI_RES5:
            case XMSG_TYPE_ID_TEMP_HI_RES6:
            case XMSG_TYPE_ID_TEMP_HI_RES7:

              try {

                // Decode high-resolution temperature.
                linf() << "Decoding profile temperature (unbinned) message...";
                AssignProfileTemperatureUnbinned(
                    std::make_shared<XHiResTemperature>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode profile temperature (unbinned) message!")

              break;

            case XMSG_TYPE_ID_TEMP_DRIFT8:    // 0xA8
            case XMSG_TYPE_ID_TEMP_DRIFT9:
            case XMSG_TYPE_ID_TEMP_DRIFTA:
            case XMSG_TYPE_ID_TEMP_DRIFTB:
            case XMSG_TYPE_ID_TEMP_DRIFTC:
            case XMSG_TYPE_ID_TEMP_DRIFTD:
            case XMSG_TYPE_ID_TEMP_DRIFTE:
            case XMSG_TYPE_ID_TEMP_DRIFTF:

              try {

                // Decode drift temperature.
                linf() << "Decoding drift temperature message...";
                AssignDriftTemperatre(
                    std::make_shared<XDriftTemperature>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode drift temperature message!")

              break;

            case XMSG_TYPE_ID_SALN_HI_RES0:    // 0xB0
            case XMSG_TYPE_ID_SALN_HI_RES1:
            case XMSG_TYPE_ID_SALN_HI_RES2:
            case XMSG_TYPE_ID_SALN_HI_RES3:
            case XMSG_TYPE_ID_SALN_HI_RES4:
            case XMSG_TYPE_ID_SALN_HI_RES5:
            case XMSG_TYPE_ID_SALN_HI_RES6:
            case XMSG_TYPE_ID_SALN_HI_RES7:

              try {

                // Decode high-resolution salinity.
                linf() << "Decoding profile salinity (unbinned) message...";
                AssignProfileSalinityUnbinned(
                    std::make_shared<XHiResSalinity>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode profile salinity (unbinned) message!")

              break;

            case XMSG_TYPE_ID_SALN_DRIFT8:    // 0xB8
            case XMSG_TYPE_ID_SALN_DRIFT9:
            case XMSG_TYPE_ID_SALN_DRIFTA:
            case XMSG_TYPE_ID_SALN_DRIFTB:
            case XMSG_TYPE_ID_SALN_DRIFTC:
            case XMSG_TYPE_ID_SALN_DRIFTD:
            case XMSG_TYPE_ID_SALN_DRIFTE:
            case XMSG_TYPE_ID_SALN_DRIFTF:

              try {

                // Decode drift salinity.
                linf() << "Decoding drift salinity message...";
                AssignDriftSalinity(
                    std::make_shared<XDriftSalinity>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode drift salinity message!")

              break;



            case XMSG_TYPE_ID_ALAMO_WAVE_C1:
            case XMSG_TYPE_ID_ALAMO_WAVE_C3:

              try {

                // Decode ALAMO wave data message.
                linf() << "Decoding wave message...";
                if(!xmsg_wave_field_)
                  xmsg_wave_field_ = std::make_shared<XWave>(id_dive(), id_instrument(), xdata);
                else
                  xmsg_wave_field_->DecodeXMsgData(xdata);

                // TODO Apply correction for dumb stupid encoded data
                switch(parent_instrument()->fw_version()) {
                  case 0x0A060231: // ALAMO Software version 10.6.2.31 (WIMS)
                  case 0x0A080031: // ALAMO Software version 10.8.0.31 (WIMS)
                  case 0x0A090131:
                  case 0x0A0A0031: // ALAMO Software version 10.10.0.31 (WIMS)
                    xmsg_wave_field_->FixFuckup9000();
                    break;

                  default:
                    /* Do nothing. */
                    break;
                }

                set_listing_file_updated(false);

              } CATCH_BLOCK("Failed to decode ocean surface wave-field (ALAMO) message!")

              break;

            case XMSG_TYPE_ID_MIZ_ACOUSTIC:

              try {

                // Decode MIZ acoustic message.
                linf() << "Decoding acoustic position message...";
                if (!xmsg_acoustic_positions_)
                  xmsg_acoustic_positions_ = make_shared<XAcousticPosition>(id_dive(), id_instrument(), xdata);
                else
                  xmsg_acoustic_positions_->DecodeXMsgData(xdata);

                set_listing_file_updated(false);

              } CATCH_BLOCK("Failed to decode acoustic position (MIZ) message!")



              break;

            case XMSG_TYPE_ID_MISS_CONF0:    // 0xD0
            case XMSG_TYPE_ID_MISS_CONF1:
            case XMSG_TYPE_ID_MISS_CONF2:
            case XMSG_TYPE_ID_MISS_CONF3:
            case XMSG_TYPE_ID_MISS_CONF4:
            case XMSG_TYPE_ID_MISS_CONF5:
            case XMSG_TYPE_ID_MISS_CONF6:
            case XMSG_TYPE_ID_MISS_CONF7:

              try {

                // Decode mission configuration message.
                linf() << "Decoding mission configuration parameter message...";
                xmsg_mission_cfg_.AssignMessage(xdata);

                if(xmsg_id_type == XMSG_TYPE_ID_MISS_CONF0) {

                  // Update firmware version.
                  parent_instrument()->set_fw_version(xmsg_mission_cfg_.fw_version());
                }

                set_listing_file_updated(false);

              } CATCH_BLOCK("Failed to decode mission configuration message!")

              break;

            case XMSG_TYPE_ID_MISS_CMD_ECHO:    // 0xDE

              try {

                // Decode mission command echo message.
                linf() << "Decoding mission command echo message...";
                AssignMissionCmdEchoback(
                    std::make_shared<XMissionCmdEcho>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode ground-station upload command echo-back "
                  "message!")

              break;

            case XMSG_TYPE_ID_ENG_DIAGNOSTIC:    // 0xE0

              try {

                // Decode engineering message for diagnostic dive.
                linf() << "Decoding engineering message for diagnostic dive...";
                AssignEngineeringDiagnostic(
                    std::make_shared<XEngDiagnostic>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode engineering message for diagnostic dive!")

              break;

            case XMSG_TYPE_ID_ENG_DIVE:

              try {

                // Decode engineering message for standard dive.
                linf() << "Decoding engineering message for standard dive...";
                AssignEngineeringStandard(
                    std::make_shared<XEngStandard>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode engineering message for standard dive!")

              break;

            case XMSG_TYPE_ID_ENG_ABORT:

              try {

                // Decode engineering message for abort/beacon.
                linf() << "Decoding engineering message for abort/beacon mode...";
                AssignEngineeringAbort(
                    std::make_shared<XEngAbort>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode engineering message for abort/beacon mode!")

              break;

            case XMSG_TYPE_ID_ENG_BIT_PASS:

              try {

                // Decode engineering message for BIT passed.
                linf() << "Decoding engineering message for BIT passed...";
                AssignEngineeringBitPass(
                    std::make_shared<XEngBITPass>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode engineering message for BIT passed!")

              break;

            case XMSG_TYPE_ID_ENG_BIT_FAIL:

              try {

                // Decode engineering message for BIT failed.
                linf() << "Decoding engineering message for BIT failed...";
                AssignEngineeringBitFail(
                    std::make_shared<XEngBITFail>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode engineering message for BIT failed!")

              break;

            case XMSG_TYPE_ID_ENG_DESCENT:

              try {

                // Decode engineering descent message.
                linf() << "Decoding engineering descent message...";
                AssignEngineeringDescent(
                    std::make_shared<XEngDescent>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode engineering descent message!")

              break;

            case XMSG_TYPE_ID_ENG_PARK:

              try {

                // Decode engineering park message.
                linf() << "Decoding engineering drift/park message...";
                AssignEngineeringDrift(
                    std::make_shared<XEngDrift>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode engineering drift/park message!")

              break;

            case XMSG_TYPE_ID_ENG_ASCENT:

              try {

                // Decode engineering ascent message.
                linf() << "Decoding engineering ascent message...";
                AssignEngineeringAscent(
                    std::make_shared<XEngAscent>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode engineering ascent message!")

              break;

            case XMSG_TYPE_ID_ENG_SURFACE:

              try {

                // Decode engineering surface message.
                linf() << "Decoding engineering surface message...";
                AssignEngineeringSurface(
                    std::make_shared<XEngSurface>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode surface-state engineering message!")

              break;

            case XMSG_TYPE_ID_ENG_VALVE: // 0xEB

              try {

                // Decode valve data message.
                linf() << "Decoding valve data message...";
                AssignValveData(std::make_shared<XValve>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode valve data message!")

              break;

            case XMSG_TYPE_ID_ENG_ICE: // 0xEC

              try {

                // TODO Decode ice-detection engineering message message.
                linf() << "Decoding ice-detection engineering message...";
                AssignEngineeringIce(
                    std::make_shared<XEngIce>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode ice-detection engineering message!")

              break;

            case XMSG_TYPE_ID_ARGO_DATA: // 0xF0

              try {

                // Decode Argo data.
                linf() << "Decoding Argo data message...";
                AssignArgoData(std::make_shared<XArgo>(id_dive(), id_instrument(), xdata));

              } CATCH_BLOCK("Failed to decode Argo data message!")

              break;

            case XMSG_TYPE_ID_TEST:

              // Ignore test messages, if any.
              break;

            case XMSG_TYPE_ID_UNKNOWN:
            default:

              // Unknown X-Message ID
              lwrn() << "Unknown X message ID: 0x"
                     << hex
                     << setw(2)
                     << setfill('0')
                     << (int) xdata.at(0)
                     << "!";

              break;

          }
        }CATCH_NOTHROW("Failed to decode X-Message!")

        // Clear message buffer.
        xdata.clear();
      }
    }
  } CATCH_BLOCK("Failed to decode SBD data!")
}

/** Generate a listing entry string representation of this dive data object.
 *
 *  @throw          util::log::MessageException if an error occurs while generating the string
 *                  representation.
 *
 *  @return         A listing entry string representation of this dive data object.
 */
string XFloatDiveData::GenerateListingString() const {
  using namespace x;
  using util::date::DateUtils;

  ostringstream os;
  uint16_t pg, po, sg, so, tg, to,        // Sensor gains & offsets.
      spg, spo, ssg, sso, stg, sto;   // Sensor gains & offsets for surface data.
  float pres_correction = 0;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    // Check parent object.
    if(!parent_instrument())
      EXC_THROW("Null pointer to parent XFloat object!");

    // Get gains & offsets.
    GetGainsAndOffsets(pg, po, tg, to, sg, so);
    GetSurfaceGainsAndOffsets(spg, spo, stg, sto, ssg, sso);

    if(is_pres_offs_valid() && pressure_offset_correction_enabled_list_file_)

      // Get pressure offset correction.
      pres_correction = pres_offs();

    /* LISTING HEADER */

    try {

      {   // Print splash and header.
//        os << util::config::LocalConfiguration::splash_lis();
        static const format fmt("Intermediate Listing for Float #%d, Dive #%d");
        XMSG_HEADER(format(fmt) % id_instrument() % id_dive());
      }

      {   // Print processing details.
        static const format fmt("# %30s: %s");
        os << format(fmt) % "Created" % GenerateCurrentUtcTimeIso8601String() << endl;
        if(!intermediate_listing_file_path().empty())
          os << format(fmt) % "Local Path" % intermediate_listing_file_path() << endl;

        XMSG_SECTION("Dive Data Completeness");
        os << format(fmt) % "Dive Complete" % (is_complete() ? "yes" : "no") << endl;
        os << format(fmt) % "Complete Profile Data" % (HasCompleteProfileData() ? "yes" : "no")
           << endl;
        os << format(fmt) % "Contiguous MOMSNs" % (HasContiguousMomsns() ? "yes" : "no") << endl;
        os << format(fmt) % "Contiguous PIDs" % (HasContiguousPids() ? "yes" : "no") << endl;
        os << format(fmt) % "Sensor Gains & Offsets Known"
               % (SensorGainsAndOffsetsAreKnown() ? "yes" : "no")
           << endl;
        os << format(fmt) % "Valid Position" % (HasValidPosition() ? "yes" : "no") << endl;
      }

      {   // Print decoder version details.
        XMSG_SECTION("XMTPS Software Version");
//        static const format fmt1("#   %20s %20s %23s %30s");
        static const format fmt2("XSV %20s %20s %23s %30s");
//        os << format(fmt1) % "xmessage-version" % "xmtps-version" % "build-configuration"
//               % "build-date/time"
//           << endl;
        os << "#       xmessage-version        xmtps-version     build-configuration                build-date/time" << endl;
#           if defined(BUILD_DEBUG) && BUILD_DEBUG
        os << format(fmt2) % PROG_XVERS % PROG_VERS % "DEBUG" % BUILD_DATETIME << endl;
#           else
        os << format(fmt2) % PROG_XVERS % PROG_VERS % "RELEASE" % BUILD_DATETIME << endl;
#           endif
      }

      try {

        // Print listing details
        XMSG_SECTION("Listing Details");
        static const format fmt1("#   %4s %4s %6s %15s %7s");
//        os << format(fmt1) % "SN" % "dive" % "DAC-ID" % "IMEI" % "WMO-ID" << endl;
        os << "#      SN dive DAC-ID            IMEI  WMO-ID" << endl;
        static const format fmt2("LIS %5d %4d   %04d %015d %07d");
        os << format(fmt2) % id_instrument() % id_dive() % id_dac() % (id_imei() % 10000000)
               % id_wmo()
           << endl
           << endl;

      } CATCH_BLOCK("Failed to generate listing details section!")

    } CATCH_BLOCK("Failed to generate listing file header from dive data "
        "object!")

    /* IRIDIUM DATA */

    if(!mail_messages_.empty())

      try {

        // Print mail/SBD message data.
        XMSG_HEADER2("Iridium Short-Burst Data (SBD) Message(s)");
        os << XFloatMailMessage::GenerateListingComment() << endl;
        for_each(
            mail_messages_.begin(),
            mail_messages_.end(),
            [&](const shared_ptr<XFloatMailMessage>& msg) {
              if(msg) os << msg->GenerateListingString() << endl;
            });
        os << endl;

      } CATCH_BLOCK("Failed to generate SBD data listing entry from dive data object!")

    /* POSITION DATA */

    if(HasPositionData()) {

      try {
        XMSG_HEADER2("Position Data");

        if(xmsg_acoustic_positions_) {

          // Print acoustic position(s) (MIZ only).
          XMSG_SECTION("Acoustic Position");
          xmsg_acoustic_positions_->AssignWindowIndices();
          os << xmsg_acoustic_positions_->GenerateListingString();
        }

        if(!xmsg_gps_positions_.empty()) {

          // Print GPS messages.
          XMSG_SECTION("GPS");
          os << XGPS::GenerateListingComment() << endl;
          for_each(
              xmsg_gps_positions_.begin(),
              xmsg_gps_positions_.end(),
              [&](const shared_ptr<XGPS>& gps) {

                if(gps) os << gps->GenerateListingString() << endl;

              });
        }

        if(!mail_messages_.empty()) {

          // Print composite Iridium position.
          XMSG_SECTION("Iridium Position");
          os << GenerateCompositeIridiumPosition()->GenerateListingString(
                 parent_instrument_->id_instrument(),
                 id_dive())
             << endl;
        }

        os << endl;

      } CATCH_BLOCK("Failed to generate position data listing entry from dive data object!")
    }

    /* DIVE DATA */

    if(HasDiveData())

      try {

        // Print dive data.
        XMSG_HEADER2("Dive Data");

        if(xmsg_argo_data_)

          try {

            // Print Argo data.
            XMSG_SECTION("Argo Data");
            os << xmsg_argo_data_->GenerateListingString() << endl;

          }CATCH_NOTHROW("Failed to generate Argo data listing entry!")

        // Print pressure correction.
        try {

          XMSG_SECTION("Pressure Correction");
          os << GeneratePressureCorrectionListingString() << endl;

        } CATCH_BLOCK("Failed to generate pressure correction listing string entry!")

        if(!xmsg_fall_time_series_.empty())

          try {

            // Print fall messages.
            XMSG_SECTION("Fall Time-Series Data");
            os << GenerateCompositeFallTimeSeriesData()->GenerateListingString(
                   pg,
                   po,
                   sg,
                   so,
                   tg,
                   to,
                   pres_correction)
               << endl;

          }CATCH_NOTHROW("Failed to generate fall pressure time-series listing entry!")

        if(HasDriftData())

          try {

            // Print drift data.
            XMSG_SECTION("Drift Data");
            os << xmsg_composite_drift_data_.GenerateListingString(
                   id_instrument(),
                   id_dive(),
                   pg,
                   po,
                   sg,
                   so,
                   tg,
                   to,
                   pres_correction)
               << endl;

          }CATCH_NOTHROW("Failed to generate drift data listing entry!")

        if(HasBinnedProfileData())

          try {

            // Print profile data message(s).
            XMSG_SECTION("Profile Data (Binned)");
            os << xmsg_composite_profile_binned_.GenerateListingString(
                   id_instrument(),
                   id_dive(),
                   pg,
                   po,
                   sg,
                   so,
                   tg,
                   to,
                   pres_correction)
               << endl;

          }CATCH_NOTHROW("Failed to generate profile data listing entry!")

        if(HasUnbinnedProfileData())

          try {

            // Print profile data message(s).
            XMSG_SECTION("Profile Data (Unbinned)");
            os << xmsg_composite_profile_unbinned_.GenerateListingString(
                   id_instrument(),
                   id_dive(),
                   pg,
                   po,
                   sg,
                   so,
                   tg,
                   to,
                   pres_correction)
               << endl;

          }CATCH_NOTHROW("Failed to generate profile data listing entry!")

//        if(HasAuxTempData())
//
//          try {
//
//            // Print profile data message(s).
//            XMSG_SECTION("Auxiliary Temperature Channel");
//            os << GenerateCompositeAuxTemp()->GenerateListingString(
//                   id_instrument(),
//                   id_dive(),
//                   tg,
//                   to);
//
//          }CATCH_NOTHROW("Failed to generate profile data listing entry!")

        if(!xmsg_valve_data_.empty())

          try {

            // Print valve messages.
            XMSG_SECTION("Valve Data");
            os << GenerateCompositeValveData()->GenerateListingString(pg, po, pres_correction)
               << endl;

          }CATCH_NOTHROW("Failed to generate valve data listing entry!")

        if(!xmsg_pump_time_series_.empty())

          try {

            // Print pump messages.
            XMSG_SECTION("Pump Data");
            os << GenerateCompositePumpTimeSeriesData()->GenerateListingString(
                   pg,
                   po,
                   pres_correction)
               << endl;

          }CATCH_NOTHROW("Failed to generate pump data listing entry!")

        if(!xmsg_rise_time_series_.empty())

          try {

            // Print rise messages.
            XMSG_SECTION("Rise Time-Series Data");
            os << GenerateCompositeRiseTimeSeriesData()->GenerateListingString(
                   pg,
                   po,
                   sg,
                   so,
                   tg,
                   to,
                   pres_correction)
               << endl;

          }CATCH_NOTHROW("Failed to generate rise pressure time-series listing entry!")

        if(!xmsg_surface_time_series_.IsEmpty())

          try {

            // Print surface pressure time-series message(s).
            os << xmsg_surface_time_series_.GenerateListingString(
                   spg,
                   spo,
                   ssg,
                   sso,
                   stg,
                   sto,
                   pres_correction)
               << endl;

          }CATCH_NOTHROW("Failed to generate surface pressure time-series listing entry!")

        if(HasWaveData())

          try {

            // Print ocean surface wave message(s).
            XMSG_SECTION("Ocean Surface Wave-Field Data");
            os << xmsg_wave_field_->GenerateListingString() << endl;

          }CATCH_NOTHROW("Failed to generate wave data listing entry!")

        os << endl;

      }CATCH_NOTHROW("Failed to generate dive data listing entry from dive data object!")

    /* ENGINEERING DATA */

    if(HasEngineeringData())

      try {

        XMSG_HEADER2("Engineering Data");

        // Print abort engineering message.
        if(xmsg_engineering_data_abort_)

          try {

            XMSG_SECTION("Engineering (Abort)");
            os << xmsg_engineering_data_abort_->GenerateListingString() << endl;

          }CATCH_NOTHROW("Failed to generate abort engineering message entry!")

        // Print BIT fail engineering message.
        if(xmsg_engineering_data_bit_fail_)

          try {

            XMSG_SECTION("Engineering (BIT failed)");
            os << xmsg_engineering_data_bit_fail_->GenerateListingString(pg, po, sg, so, tg, to)
               << endl;

          }CATCH_NOTHROW("Failed to generate BIT failed engineering message entry!")

        // Print BIT passed engineering message.
        if(xmsg_engineering_data_bit_passed_)

          try {

            XMSG_SECTION("Engineering (BIT passed)");
            os << xmsg_engineering_data_bit_passed_->GenerateListingString(pg, po, sg, so, tg, to)
               << endl;

          }CATCH_NOTHROW("Failed to generate BIT passed engineering message entry!")

        // Print descent engineering message.
        if(xmsg_engineering_data_descent_)

          try {

            XMSG_SECTION("Engineering (Descent)");
            os << xmsg_engineering_data_descent_->GenerateListingString() << endl;

          }CATCH_NOTHROW("Failed to generate descent engineering message entry!")

        // Print diagnostic engineering message.
        if(xmsg_engineering_data_diagnostic_dive_)

          try {

            XMSG_SECTION("Engineering (Diagnostic Dive)");
            os << xmsg_engineering_data_diagnostic_dive_->GenerateListingString() << endl;

          }CATCH_NOTHROW("Failed to generate diagnostic dive engineering message entry!")

        // Print park/drift engineering message.
        if(xmsg_engineering_data_drift_)

          try {

            XMSG_SECTION("Engineering (Park/Drift)");
            os << xmsg_engineering_data_drift_->GenerateListingString(pg, po, sg, so, tg, to)
               << endl;

          }CATCH_NOTHROW("Failed to generate drift engineering message entry!")

        // Print ascent engineering message.
        if(xmsg_engineering_data_ascent_)

          try {

            XMSG_SECTION("Engineering (Ascent)");
            os << xmsg_engineering_data_ascent_->GenerateListingString(pg, po, sg, so, tg, to) << endl;

          }CATCH_NOTHROW("Failed to generate ascent engineering message entry!")

        // Print ice-detection engineering message.
        if(xmsg_engineering_data_ice_)

          try {

            XMSG_SECTION("Engineering (Ice)");
            os << xmsg_engineering_data_ice_->GenerateListingString(pg, po, sg, so, tg, to) << endl;

          }CATCH_NOTHROW("Failed to generate ice-detection engineering message entry")

        // Print standard engineering message.
        if(xmsg_engineering_data_standard_dive_)

          try {

            XMSG_SECTION("Engineering (Standard Dive)");
            os << xmsg_engineering_data_standard_dive_->GenerateListingString() << endl;

          }CATCH_NOTHROW("Failed to generate standard dive engineering message entry!")

        // print surface engineering message.
        if(xmsg_engineering_data_surface_)

          try {

            XMSG_SECTION("Engineering (Surface)");
            os << xmsg_engineering_data_surface_->GenerateListingString(pg, po, sg, so, tg, to)
               << endl;

          }CATCH_NOTHROW("Failed to generate surface engineering message entry!")

        os << endl;

      }CATCH_NOTHROW("Failed to generate engineering data listing entry from "
          "dive data object!")

    // Print mission command upload echo-back message(s).
    if(!xmsg_mission_cmd_echoback_.empty())

      try {

        XMSG_HEADER2("Ground Station Upload Command Echo-Back Messages");
        for_each(
            xmsg_mission_cmd_echoback_.begin(),
            xmsg_mission_cmd_echoback_.end(),
            [&](const shared_ptr<XMissionCmdEcho>& msg) {

              if(msg) os << msg->GenerateListingString() << endl;

            });

      }CATCH_NOTHROW("Failed to generate listing entry for ground station upload command "
          "echo-back message!")

    // Print mission configuration message(s).
    try {

      XMSG_HEADER2("Mission Configuration");
      os << xmsg_mission_cfg_.GenerateListingString(id_instrument(), id_dive(), 0) << endl;

    }CATCH_NOTHROW("Failed to generate mission configuration listing entry "
        "from dive data object!")

    // Print SBD message data hex:
    if(!mail_messages_.empty())
      try {
        XMSG_HEADER2("SBD Message Hex Data");
        for_each(
            mail_messages_.begin(),
            mail_messages_.end(),
            [&](const shared_ptr<XFloatMailMessage>& mail) {
              if(mail) os << mail->GenerateSbdHexListingString();
          });
        os << endl;
      } CATCH_NOTHROW("Failed to generate SBD hex listing string!")


  } CATCH_BLOCK("Failed to generate listing string from dive data!")

  return os.str();
}

/** Generate a PHY entry string representation of this dive data object.
 *
 *  @throw          util::log::MessageException if an error occurs while generating the string
 *                  representation.
 *
 *  @return         A phy entry string representation of this dive data object.
 */
string XFloatDiveData::GeneratePhyString() const {
  using namespace x;

  ostringstream os;
  uint16_t pg, po, sg, so, tg, to;

  float pres_correction = 0;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    // Check parent object.
    if(!parent_instrument_)
      EXC_THROW("Null pointer to parent XFloat object!");

    // Get gains & offsets.
    GetGainsAndOffsets(pg, po, tg, to, sg, so);

    if(is_pres_offs_valid() && pressure_offset_correction_enabled_phy_file_)

      // Get pressure offset correction.
      pres_correction = pres_offs();

    os << format("%-39s %d") % "INTERNAL ID NUMBER" % id_dac() << endl;
    os << format("%-39s %d") % "WMO ID NUMBER" % id_wmo() << endl;
    os << format("%-39s %d") % "PROFILE NUMBER" % id_dive() << endl;

    switch(parent_instrument_->argo_instrument_type()) {

      case XARGO_INSRUMENT_TYPE_SOLO2:

        /** @todo   Implement the functionality for generate a PHY string representation of a
         *          dive data object for a Solo-II float.
         */

        break;

      case XARGO_INSRUMENT_TYPE_DEEP_ARGO:

        /** @todo   Implement the functionality for generate a PHY string representation of a
         *          dive data object for a Deep Argo float.
         */
        break;

      case XARGO_INSRUMENT_TYPE_ALAMO: {

        shared_ptr<XFloatMetadata> md(parent_instrument_->metadata());

        XMSG_PHY(os, "INSTRUMENT TYPE", md->instrument_type());
        os << format("%-39s %d") % "WMO INSTRUMENT TYPE (TABLE 1770)" % md->wmo_instrument_type()
           << endl;
        os << format("%-39s %d") % "WMO RECORDER TYPE (TABLE 4770)" % md->wmo_recorder_type()
           << endl;
        XMSG_PHY(os, "POSITIONING_SYSTEM", "GPS");
        XMSG_PHY(os, "PI", md->principal_investigators());
        switch(md->wmo_instrument_type()) {

          case 866:

            XMSG_PHY(os, "NUMBER OF COLUMNS", "2");
            XMSG_PHY(os, "1. COLUMN", "PRESSURE (DBAR)");
            XMSG_PHY(os, "2. COLUMN", "TEMPERATURE-90 (DEG C)");
            break;

          case 867:   // ALAMO, RBR conductivity sensor.
          case 868:   // ALAMO, SBE conductivity sensor.
          case 873:   // ALTO, unspecified conductivity sensor.
          case 875:   // ALTO, RBR conductivity sensor.
          case 876:   // ALTO, SBE conductivity sensor.
            XMSG_PHY(os, "NUMBER OF COLUMNS", "3");
            XMSG_PHY(os, "1. COLUMN", "PRESSURE (DBAR)");
            XMSG_PHY(os, "2. COLUMN", "TEMPERATURE-90 (DEG C)");
            XMSG_PHY(os, "3. COLUMN", "SALINITY (PSU)");
            break;

          default:
            EXC_THROW("Unsupported WMO instrument type identifier.");
            break;

        }
      }

        break;

      case XARGO_INSRUMENT_TYPE_UNKNOWN:
      default:

        EXC_THROW("Invalid Argo instrument type identifier!");
        break;

    }

    // Position.
    os << GetGpsProfilePosition()->GeneratePhyString(mail_messages_.size());

    // Profile Data.
    if(HasCompleteBinnedProfileData())
      os << xmsg_composite_profile_binned_.GeneratePhyString(
             pg,
             po,
             sg,
             so,
             tg,
             to,
             pres_correction)
         << endl;
    else if(HasCompleteUnbinnedProfileData())
      os << xmsg_composite_profile_unbinned_.GeneratePhyString(
             pg,
             po,
             sg,
             so,
             tg,
             to,
             pres_correction)
         << endl;

  } CATCH_BLOCK("Failed to generate PHY string from dive data!")

  return os.str();
}

vector<shared_ptr<XFloatMailMessage>> XFloatDiveData::GetDiveDataMailMessagesFromDatabase() {

  static const time_duration window(0, 15, 0);

  shared_ptr<XFloat> inst;
  shared_ptr<XFloatMetadata> meta;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    // Check the instrument IDs.
    if(id_instrument() == XFLT_ID_UNKNOWN)
      EXC_THROW("The instrument identifier is unknown!");
    if(id_dive() == XFLT_DIVE_UNKNOWN)
      EXC_THROW("The dive cycle identifier is unknown!");

    // Get the instrument objects.
    inst = XFloat::GetFloat(id_instrument());
    if(inst)
      meta = inst->metadata();

    // Construct the "WHERE" clause of the SQL query.
    ostringstream where;
    where << "id_instrument = " << id_instrument();
    where << " AND id_dive = " << id_dive();
    where << " AND msg_is_valid = 1";

    if(inst && !inst->start_of_mission_datetime().is_not_a_date_time())
      where << " AND iridium_position_datetime >= '"
            << bpt::to_iso_string(inst->start_of_mission_datetime() - window)
            << "'";

    if(meta && !meta->deployment_datetime().is_not_a_date_time())
      where << " AND iridium_position_datetime >= '"
            << bpt::to_iso_string(meta->deployment_datetime() - window)
            << "'";

    return XFloatMailMessage::SelectObjectsFromDatabase(where.str());

  } CATCH_BLOCK("Failed to get mail messages for dive data object from database!")

  return vector<shared_ptr<XFloatMailMessage>>();
}

/** Get a dive data object for a given instrument and dive number from the database, if it exists.
 *
 *  @param[in]      xfloat      A shared pointer to the instrument object. Must not be NULL.
 *  @param[in]      dive        The dive number.
 *
 *  @throw          util::log::MessageException if an error occurs while querying the database or
 *                  xfloat is NULL.
 *
 *  @return         A shared pointer to the dive data object or NULL if a corresponding entry does
 *                  not exist in the database.
 */
shared_ptr<XFloatDiveData> XFloatDiveData::GetDiveDataObjectFromDatabase(
    const shared_ptr<XFloat>& xfloat,
    int dive) {

  shared_ptr<XFloatDiveData> x;
  int whoi = XFLT_ID_UNKNOWN;

  try {

    if(!xfloat)
      EXC_THROW("Pointer to xfloat object is NULL!");

    whoi = xfloat->id_instrument();

    // Query the object from the database.
    ostringstream where;
    where << "id_instrument = " << whoi << " AND id_dive = " << dive;
    if((x = SelectObjectFromDatabase(where.str())))
      x->AssignParentInstrument(xfloat);

  } CATCH_BLOCK(
      "Failed to get dive data object from the database for instrument #"
          + lexical_cast<string>(whoi) + ", dive #" + lexical_cast<string>(dive) + "!")
  return x;
}

/** Get the pressure, temperature, and salinity sensor gains and offsets.
 *
 *  @param[out]     pg          The pressure sensitivity gain factor, in dBar/LSB.
 *  @param[out]     po          The pressure offset, in dBar.
 *  @param[out]     tg          The temperature sensitivity gain factor, in degC/LSB.
 *  @param[out]     to          The temperature offset, in degC.
 *  @param[out]     sg          The salinity sensitivity scale factor, in PSU/LSB.
 *  @param[out]     so          The salinity offset, in PSU.
 */
void XFloatDiveData::GetGainsAndOffsets(
    uint16_t& pg,
    uint16_t& po,
    uint16_t& tg,
    uint16_t& to,
    uint16_t& sg,
    uint16_t& so) const {
  // Get an exclusive lock.
  lock_guard<recursive_mutex> guard(mutex_);

  if(xmsg_argo_data_) {
    pg = xmsg_argo_data_->gain_pressure();
    po = xmsg_argo_data_->offset_pressure();
    sg = xmsg_argo_data_->gain_salinity();
    so = xmsg_argo_data_->offset_salinity();
    tg = xmsg_argo_data_->gain_temperature();
    to = xmsg_argo_data_->offset_temperature();
//    } else if(parent_instrument_) {
//        pg = parent_instrument_->gain_pressure();
//        po = parent_instrument_->offset_pressure();
//        sg = parent_instrument_->gain_salinity();
//        so = parent_instrument_->offset_salinity();
//        tg = parent_instrument_->gain_temperature();
//        to = parent_instrument_->offset_temperature();
  } else {
    pg = XFLT_GAIN_INVALID;
    po = XFLT_GAIN_INVALID;
    sg = XFLT_GAIN_INVALID;
    so = XFLT_OFFS_INVALID;
    tg = XFLT_OFFS_INVALID;
    to = XFLT_OFFS_INVALID;
  }
}

/** Compute this dive's surface pressure offset from the surface time-series data.
 *
 *  The offset is the mean of the surface pressure means (see
 *  @ref UpdateDerivedSurfaceDataFields()) of this dive and up to @p max_num_dives preceding dives,
 *  counting only dives whose surface pressure is valid. Each dive counts equally, regardless of its
 *  number of samples. The result is stored in the pressure offset fields (@c pres_offs,
 *  @c pres_offs_num_dives, @c pres_offs_num_samples, @c pres_offs_valid), which the listing and
 *  PHY output use for the pressure offset correction.
 *
 *  The offset is only computed for instruments whose conductivity sensor needs the correction
 *  (WMO instrument types 866, 867 and 875). For SBE sensors (868, 873, 876) and unknown types the
 *  offset is left invalid; an unexpected type is logged as a warning. It is also left invalid if no
 *  dive has valid surface pressure data.
 *
 *  @note       Preceding dives are loaded with @ref XFloat::GetDiveData(), which may read them from
 *              the database.
 *
 *  @param[in]  max_num_dives   The maximum number of preceding dives to include (0 = none, and
 *                              the offset is not computed).
 *
 *  @throw      util::log::MessageException if an error occurs.
 *
 *  @return     Always 0; read the result from the pressure offset fields.
 */
float XFloatDiveData::ComputePressureOffsetFromSurfaceTimeSeries(unsigned int max_num_dives) {

  shared_ptr<XFloatDiveData> dive;
  shared_ptr<XFloat> xfloat;
  shared_ptr<XFloatMetadata> meta;
  int dive_idx;
  unsigned int num_dives = 0;
  unsigned int num_samples = 0;
  float pres_sum = 0;
  float val = 0;

  try {

    ldbg() << "Computing pressure offset for dive...";

    {
      lock_guard<recursive_mutex> guard(mutex_);  // Get an exclusive lock.
      set_pres_offs(0);                           // Reset pressure offset.
      set_pres_offs_num_dives(-1);
      set_pres_offs_num_samples(-1);
      set_pres_offs_valid(false);
    }

    // Check number of dives.
    if(!max_num_dives) {

      lwrn() << "Not computing surface pressure offset: " << "max_num_dives = 0.";
      return 0;
    }

    // Check instrument type.
    if(!(xfloat = parent_instrument()) || !(meta = xfloat->metadata())) {

      /* The instrument type is unknown. Do not compute surface pressure offset. */
      return 0.;
    }
    switch(meta->wmo_instrument_type()) {
      case XFLT_ID_UNKNOWN:

        /* Do not compute surface pressure offset for unknown float types. */
        return 0.;

      case 866:   // ALAMO, no coductivity sensor.
      case 867:   // ALAMO, RBR coductivity sensor.
      case 875:   // ALTO, RBR conductivity sensor.

        /* Do nothing (proceed with pressure offset computation). */
        break;

      case 868:   // ALAMO, SBE conductivity sensor.
      case 873:   // ALTO, unspecified conductivity sensor (all deployed units are SBE).
      case 876:   // ALTO, SBE conductivity sensor (corrects pressure onboard).

        /* Do not compute surface pressure offset for SBE floats. */
        return 0.;

      default:    // Unexpected float type

        /* Do not compute surface pressure offset for unknown float types. */
        lwrn() << "Not computing surface pressure offset. "
               << "Unknown WMO instrument type encountered: "
               << meta->wmo_instrument_type()
               << ".";
        return 0.;
    }

    // Collect data from current dive.
    if(is_surface_pres_valid()) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      ldbg() << "Acquiring surface pressure mean for dive current dive...";

      num_dives++;
      num_samples += surface_pres_num_samples();
      pres_sum += surface_pres_mean();

    }

    // Collect data from previous dive(s).
    for(unsigned int i = 1; i <= max_num_dives && (dive_idx = id_dive() - i) >= 0; ++i) {

      ldbg() << "Acquiring surface pressure mean for dive " << dive_idx << "...";

      // Get data for dive.
      dive = parent_instrument()->GetDiveData(dive_idx, false);

      // Check surface pressure data validity.
      if(!dive || !dive->is_surface_pres_valid()) {

        ldbg() << "Surface pressure data not valid for dive " << dive_idx << ".";
        continue;

      }

      // Collect dive data.
      num_dives++;
      num_samples += dive->surface_pres_num_samples();
      pres_sum += dive->surface_pres_mean();

    }

    // Ensure data was acquired.
    if(!num_dives) {

      lwrn() << "Pressure offset not computed. Zero valid dives found.";
      return 0;

    }

    if(!num_samples) {

      lwrn() << "Pressure offset not computed. Zero valid samples found.";
      return 0;

    }

    {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      // Compute pressure offset fields.
      set_pres_offs(pres_sum / num_dives);
      set_pres_offs_num_dives(num_dives);
      set_pres_offs_num_samples(num_samples);
      set_pres_offs_valid(true);

    }

  } CATCH_BLOCK("Failed to get pressure offset!")

  return val;

}

/** Get the pressure, temperature, and salinity sensor gains and offsets for surface data.
 *
 *  @param[out]     pg          The pressure sensitivity gain factor, in dBar/LSB.
 *  @param[out]     po          The pressure offset, in dBar.
 *  @param[out]     tg          The temperature sensitivity gain factor, in degC/LSB.
 *  @param[out]     to          The temperature offset, in degC.
 *  @param[out]     sg          The salinity sensitivity scale factor, in PSU/LSB.
 *  @param[out]     so          The salinity offset, in PSU.
 */
void XFloatDiveData::GetSurfaceGainsAndOffsets(
    uint16_t& pg,
    uint16_t& po,
    uint16_t& tg,
    uint16_t& to,
    uint16_t& sg,
    uint16_t& so) const {

  // Get an exclusive lock.
  lock_guard<recursive_mutex> guard(mutex_);

  if(xmsg_argo_data_) {
    pg = xmsg_argo_data_->gain_pressure_surface();
    po = xmsg_argo_data_->offset_pressure_surface();
    sg = xmsg_argo_data_->gain_salinity_surface();
    so = xmsg_argo_data_->offset_salinity_surface();
    tg = xmsg_argo_data_->gain_temperature_surface();
    to = xmsg_argo_data_->offset_temperature_surface();
//    } else if(parent_instrument_) {
//        pg = parent_instrument_->gain_pressure_surface();
//        po = parent_instrument_->offset_pressure_surface();
//        sg = parent_instrument_->gain_salinity_surface();
//        so = parent_instrument_->offset_salinity_surface();
//        tg = parent_instrument_->gain_temperature_surface();
//        to = parent_instrument_->offset_temperature_surface();
  } else {
    pg = XFLT_GAIN_INVALID;
    po = XFLT_GAIN_INVALID;
    sg = XFLT_GAIN_INVALID;
    so = XFLT_OFFS_INVALID;
    tg = XFLT_OFFS_INVALID;
    to = XFLT_OFFS_INVALID;
  }
}

bool XFloatDiveData::GpsPositionIsValidProfilePosition(const shared_ptr<x::XGPS> x) const {

  using namespace x;

  try {

    // Check for valid pointer, position, and timestamp.
    if(!x || !x->position_is_valid() || x->position_datetime().is_not_a_date_time()
        || x->position_datetime() > GetCurrentUtcTime())
      return false;

    // Continue based on message type.
    switch(x->id_xmsg_type()) {

      case XMSG_TYPE_ID_GPS_DIAGNOSTIC:

        // Check the dive numbers.
        if(x->id_dive() != 0 || id_dive() != 0)
          return false;

        // Check if timestamp is after transmission.
        if(x->position_datetime() > iridium_position_datetime_last())
          return false;

        break;

      case XMSG_TYPE_ID_GPS_STARTOFDIVE:

        // Check the dive numbers.
        if(x->id_dive() != id_dive() + 1)
          return false;

        break;

      case XMSG_TYPE_ID_GPS_ENDOFDIVE:

        // Check the dive numbers.
        if(x->id_dive() != id_dive())
          return false;

        // Check if timestamp is after transmission.
        if(x->position_datetime() > iridium_position_datetime_last())
          return false;

        break;

      case XMSG_TYPE_ID_GPS_ABORT:
      case XMSG_TYPE_ID_GPS_BITEST:
      default:

        return false;
    }

    // Check against start-of-mission date/time.
    ptime start_of_mission_datetime = parent_instrument()->start_of_mission_datetime();
    if(!start_of_mission_datetime.is_not_a_date_time()
        && x->position_datetime() < start_of_mission_datetime)
      return false;

  } CATCH_BLOCK("Failed to check if GPS position is valid profile position!")

  return true;

}

/** Are the sensor gains and offsets known for this dive data object.
 *
 *  @return         TRUE if the gains and offsets are known, or FALSE otherwise.
 */
bool XFloatDiveData::SensorGainsAndOffsetsAreKnown() const {

  // Get an exclusive lock.
  lock_guard<recursive_mutex> guard(mutex_);

  return (xmsg_argo_data_ != NULL)
  /*|| ((parent_instrument_) && parent_instrument_->gain_pressure() != XFLT_GAIN_INVALID
   && parent_instrument_->offset_pressure() != XFLT_OFFS_INVALID)*/;
}

/** Write the listing file for this dive.
 *
 *  @throw          util::log::MessageException if an error occurs while writing the listing file.
 *
 */
void XFloatDiveData::WriteListingFile() {

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    // Check listing file generation is enabled.
    if(!intermediate_listing_file_generation_is_enabled_)
      return;

    // Check the listing file status.
    if(is_listing_file_updated())
      return;

    // Get the listing file directory.
    path target_directory(parent_instrument()->GetListingDirectoryPath());

    // Check the listing file path.
    if(intermediate_listing_file_path().empty()) {

      // Set local path
      ostringstream os;
      static const format fmt("/lis%04d-%04d.txt");
      os << format(fmt) % id_instrument() % id_dive();
      set_intermediate_listing_file_path(target_directory += os.str());
    }

    {   // Write file.
      linf() << "Writing listing file: " << intermediate_listing_file_path() << "...";
      ofstream of(intermediate_listing_file_path());
      of << GenerateListingString();
      of.flush();
      of.close();
    }

    // Update listing file status.
    set_listing_file_updated(true);

  } CATCH_BLOCK("Failed to write listing file!")

}

/** Write the PHY file for this dive.
 *
 *  @throw          util::log::MessageException if an error occurs while writing the PHY file.
 *
 */
void XFloatDiveData::WritePhyFile() {

  path target_directory;

  try {

    // Get an exclusive lock.
    lock_guard<recursive_mutex> guard(mutex_);

    if(!phy_file_generation_is_enabled_ || !parent_instrument_
        || !parent_instrument_->phy_file_generation_is_enabled() || !parent_instrument_->metadata()
        || !is_complete())
      return;

    {   // Get phy directory.
      ostringstream os;
      os << format("%s/%04d") % phy_directory_path_.string() % id_instrument();
      target_directory = GetDirectoryPathFromString(os.str(), true);
    }

    {    // Set local path.
      ostringstream os;
      os << format("/%04d_%04d_%04d.phy") % id_dac() % id_instrument() % id_dive();
      set_phy_file_path(target_directory += os.str());
    }

    {    // Write file.
      linf() << "Writing PHY file: " << phy_file_path() << "...";
      ofstream of(phy_file_path());
      of << GeneratePhyString();
      of.flush();
      of.close();
    }

  } CATCH_BLOCK("Failed to write PHY file!")
}

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */
