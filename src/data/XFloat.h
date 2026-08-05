/** The @ref argo::iridium::xfloat::XFloat class header file.
 *
 *  This is the header file for the @ref argo::iridium::xfloat::XFloat class. This class
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
 *  This file contains the @ref argo::iridium::xfloat::XFloat class definition and the majority
 *  of the functionality of the class.
 *
 *  @file
 *
 * 	@version    0.6
 *
 *  @date       May 12, 2014 4:10:48 PM
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

#ifndef XFLOAT_H_
#define XFLOAT_H_

/* Includes: */

#include "XFloatMailMessage.h"          // local
#include "XFloatMetadata.h"

namespace argo {
namespace iridium {
namespace xfloat {

/* Class Definition: */

class XFloatTelemetryDecoderService;    // forward declaration

/** The instrument or float class.
 *
 *  This class represents a single instrument or float. Each instance of this contains information
 *  about an instrument including unique identifiers, sensor gains and offsets, metadata, and the
 *  instrument type. It also contains a dive-or-cycle-number-to-dive-data-object, @ref
 *  dive2data_map_, map that contains all of the incomplete dive data objects that belong to the
 *  instrument, uniquely indexed by dive or cycle number. As the application receives the
 *  telemetry data from the instrument, the data is decoded and assigned to the corresponding dive
 *  data object. Once it is determined that the data is complete for a single dive, the
 *  intermediate and PHY listing files are written to the local filesystem, the dive data object
 *  is persisted to the SQL database, and then the shared reference to the dive data object is
 *  released, thereby purging the dive data from the program's memory.
 *
 *  At run-time, at most one float object will exist for each instrument. Each float object is
 *  accessed and/or manipulated by the worker threads through a shared reference. A shared
 *  reference to all float objects is contained in the instrument-identifier-to-instrument-object
 *  map, @ref id2float_map_, which is indexed by the unique instrument identifier. During
 *  application initialization, the map is populated as a result of querying all the known
 *  instruments from the SQL database. Each time a float object is created, it is added to the
 *  map. During application termination, the daemon thread iterates over the contents of the map
 *  and persists each of the float objects to the SQL database.
 *
 *  @note   This object is lockable. It contains a recursive mutual exclusion (mutex) lock in
 *          order to synchronize access to and modification of objects of this type by various
 *          worker threads. Once the object is locked, any other thread that attempts to access
 *          or modify the object will block until the initial lock is released. Since the mutex
 *          is recursive, the object may be locked multiple times by the same thread without
 *          causing the thread to block indefinitely.
 *
 *  @note   This class is mapped to an SQL database table by the @ref sql_table_description_
 *          object. This allows some or all of the field of this class to be persisted to the SQL
 *          database. Likewise, an object of this type may be constructed from the results of an
 *          SQL query to the corresponding table.
 *
 *  @todo   Devise a method to actively check for (1) changes in the metadata files and (2) newly
 *          created metadata files. Load the metadata from the new or modified files and assign
 *          them to the corresponding float object. If necessary, retroactively process the mail
 *          messages for these instruments. Persist updates to the SQL database as necessary.
 */
class XFloat:
    protected util::file::FileSystemUtils,
    public Iridium,
    public util::kml::KmlUtil,
    public util::mysql::LocalPersistentEntity,
    protected util::position::PositionUtils {

    /* Friends Classes: */

    friend class XFloatDiveData;

    friend class XFloatMailProcessingService;

    friend class XFloatTelemetryDecoderService;

    friend class XFloatPostProcessingService;

    /* Friend Functions: */

    /** Friend function for populating some of the static members of this class from the
     *  command-line options.
     */
    friend options_description util::config::LocalConfiguration::GenerateCommandLineOptionsDescription();

    /** Friend function for populating some of the static members of this class from the
     *  configuration file options.
     */
    friend options_description util::config::LocalConfiguration::GenerateConfigurationFileOptionsDescription();

    /** Friend function for performing startup tasks during initialization. */
    friend const util::config::LocalConfiguration& Initialize(int argc, char** argv);


  private:

    /* Private Static Data Members: */

    /** The instrument-identifier-to-instrument-object map.
     *
     *  This object is a flat map that contains shared references to all of the instances of this
     *  class. It acts as a static, local cache of the instrument data. The object maps the
     *  unique instrument identifier to the corresponding float object. During application
     *  initialization, this map is populated as a result of querying all the known instruments
     *  from the SQL database.
     *
     *  When a specific float object is requested by any of the threads, this map is searched for
     *  the instrument object first. If the float object does not already exist, then it is
     *  created and added to this map.
     *
     *  Upon application termination, all of the float objects in this map are persisted to the
     *  SQL database so that they may be again loaded into this map when the application is
     *  restarted.
     *
     *  Accesses and modifications to this map are synchronized using the recursive mutex @ref
     *  id2float_map_mutex_. The mutex provides an exclusive lock so that only one thread at a
     *  time may either access or modify this object.
     */
    static flat_map<int, std::shared_ptr<XFloat> > id2float_map_;

    /** The mutex lock for synchronizing access to the instrument-identifier-to-instrument-object
     *  map.
     *
     *  This mutual exclusion lock is used to ensure that accesses and modifications to the
     *  instrument-identifier-to-instrument-object map, @ref id2float_map_, are synchronized so
     *  that only one thread at a time may either access or modify this object. Because the mutex
     *  is recursive, it may be locked multiple times by the same thread without causing the
     *  thread to block indefinitely.
     */
    static recursive_mutex id2float_map_mutex_;

    /** The SQL table description object.
     *
     *  This SQL table description provides the details needed to map this object to a relational
     *  database table. It utilizes the accessor methods of the class and the details of the SQL
     *  table column to (1) create an SQL INSERT statement to save the object into the database,
     *  (2) populate one or more objects' fields as a result of an SQL SELECT query, or (3) query
     *  the SQL database to access one or more of the values that are stored the the SQL table
     *  corresponding to this object.
     */
    static const util::mysql::SqlTableDescription<XFloat> sql_table_description_;

  private:

    /* Private Static Data Members - GE KML Files: */

    /** The global KML-file-generation-is-enabled flag.
     *
     *  The value of this field determines whether or not any KML files will be written to the
     *  local filesystem. If TRUE, then KML files will be written each time a complete set of dive
     *  data is received from a deployed instrument. Otherwise, if FALSE, no KML files will be
     *  written to the local filesystem.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static bool kml_file_generation_is_enabled_;

    /** The KML file directory.
     *
     */
    static path kml_directory_path_;

  private:

    /* Private Static Data Members - Startup Tasks: */

    /** A list of instrument identifiers for which to enable PHY file generation.
     *
     *  On startup, the list of instruments contained by this vector will be used to set the
     *  \ref XFloatDiveData::phy_file_generation_is_enabled_ at the start of execution of the mail
     *  retrieval thread. Subsequently, the dive data for these instruments will be reprocessed in
     *  order generate PHY files for previous dives.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static vector<unsigned int> instrument_ids_to_enable_phy_generation_;

    /** A list of instrument identifiers for which to disable PHY file generation.
     *
     *  On startup, the list of instruments contained by this vector will be used to set the
     *  \ref XFloatDiveData::phy_file_generation_is_enabled_ at the start of execution of the mail
     *  retrieval thread.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static vector<unsigned int> instrument_ids_to_disable_phy_generation_;

  private:

    /* Private Static Data Members - Status Files: */

    /** The global status-file-generation-is-enabled flag.
     *
     *  If TRUE, then status files will be written.
     */
    static bool status_file_generation_is_enabled_;

    /** The global status file directory path.
     *
     *  All status files will be written to this directory.
     */
    static path status_directory_path_;

  private:

    /* Private Data Members: */

    /** The enumerated Argo instrument type identifier.
     *
     *  This field is used to identify the specific type of instrument i.e. Solo-II, Deep Argo,
     *  or ALAMO/Hurricane floats. The value of this field may affect the behavior of some
     *  aspects of the application. For example, the manner in which some Iridium Short Burst Data
     *  (SBD) messages are decoded is dependent on the specific type of instrument.
     */
    XArgoInstrumentType_t argo_instrument_type_;

    /** The dive-number-to-dive-data-object map.
     *
     *  This object is a flat map that contains shared references to all of the incomplete dive
     *  data objects that belong to this instrument. The dive data objects are uniquely indexed
     *  by the dive or cycle number. The map acts as a dynamic, local cache of the incomplete
     *  instrument dive data.
     *
     *  Each time a thread requests the dive data object from the float object using the @ref
     *  GetDiveData() function, the dive or cycle number is used to look up the dive data object
     *  in this map. If the corresponding dive data object does already exist in the map, it will
     *  be created and added to the map before the shared reference is returned.
     */
    flat_map<int, shared_ptr<XFloatDiveData> > dive2data_map_;

    /** The reported instrument firmware version.
     *
     */
    uint32_t fw_version_;

    /** The instrument metadata.
     *
     *  This is a shared reference to this instrument metadata object corresponding to this
     *  instrument. During application initialization the metadata object is constructed from
     *  the metadata file on the local filesystem. If there is no metadata file corresponding to
     *  to this instrument, then the reference will be NULL.
     *
     *  This object contains the metadata for this instrument. It is object is used, in part, to
     *  determine whether the instrument has been deployed, and consequently, whether or not a
     *  PHY file should be generated once a the complete data for a dive is received. If the
     *  instrument object does not have a corresponding metadata object, then PHY files will NOT
     *  be generated for this instrument
     *
     */
    shared_ptr<XFloatMetadata> metadata_;

    /** The date/time of startup of the instrument.
     *
     *  This field represents the most recent date/time that the instrument was started up. This
     *  field is used, in part, to validate incoming message from the instrument. Messages that
     *  have either and Iridium position or email header date/time that is subsequent to the value
     *  of this field will be discarded.
     *
     *  Generally, the value of this field is assigned from the date/time reported by the GPS
     *  message that is reported as a result of the instrument self-test when the instrument is
     *  started. However, if the instrument is unable to obtain a valid GPS fix, then the value
     *  of this field needs to be set by alternative means.
     *
     *  @todo       Implement an alternative method to assign the value of this field if the
     *              date/time reported by the start-of-mission GPS position is invalid or
     *              unavailable.
     */
    ptime start_of_mission_datetime_;

    /** The X-Message format major version number.
     *
     *  This field indicates the X-Message version used by this instrument's firmware. It affects
     *  the manner in which the X-Messages are decoded that reflect the iterative version of the
     *  X-Message format.
     */
    uint8_t xmsg_version_major_;

    /** The X-Message format minor version number.
     *
     *  This field indicates the X-Message version used by this instrument's firmware. It affects
     *  the manner in which the X-Messages are decoded that reflect the iterative version of the
     *  X-Message format.
     */
    uint8_t xmsg_version_minor_;

  private:

    /* Private Data Members - GE KML Files: */

    /** The GE KML document.
     *
     */
    kmldom::DocumentPtr kml_document_;

    /** The KML document is updated flag.
     *
     */
    bool kml_document_updated_;

    /** The KMLfile path.
     *
     */
    path kml_file_path_;

    /** The KML file is updated flag
     *
     */
    bool kml_file_updated_;

  private:

    /* Private Data Members - GPS: */

    /** The latest GPS position date/time.
     *
     */
    ptime latest_gps_position_datetime_;

    /** The latest GPS position dive number.
     *
     */
    int latest_gps_position_dive_number_;

    /** The latest GPS position latitude.
     *
     */
    double latest_gps_position_latitude_;

    /** The latest GPS position longitude.
     *
     */
    double latest_gps_position_longitude_;

  private:

    /* Private Data Members - Iridium: */

    /** The latest iridium position circular error probable radius.
     *
     */
    int latest_iridium_position_cep_radius_;

    /** The latest iridium position date/time.
     *
     */
    ptime latest_iridium_position_datetime_;

    /** The latest iridium position dive number.
     *
     */
    int latest_iridium_position_dive_number_;

    /** The latest iridium position latitude.
     *
     */
    double latest_iridium_position_latitude_;

    /** The latest iridium position longitude.
     *
     */
    double latest_iridium_position_longitude_;

    /** The latest iridium position Mobile-Originated Message Number (MOMSN).
     *
     */
    int latest_iridium_sbd_momsn_;

    /** The latest iridium position Mobile-Terminated Message Number (MTMSN).
     *
     */
    int latest_iridium_sbd_mtmsn_;

    /** The latest startup iridium position date/time.
     *
     */
    ptime latest_iridium_startup_position_datetime_;

    /** The latest startup iridium position Mobile-Originated Message Number (MOMSN).
     *
     */
    int latest_iridium_startup_sbd_momsn_;

  private:

    /* Private Data Members - NetCDF Files: */

    /** The NetCDF-file-generation-is-enabled flag.
     *
     */
    bool netcdf_file_generation_is_enabled_;

  private:

    /* Private Data Members - PHY Files: */

    /** The PHY-file-generation-enabled flag.
     *
     *  This field determines, in part, whether or not a PHY file will be generated once the
     *  complete data for a single dive is received from the instrument. If FALSE, then PHY files
     *  will NOT be generated for this instrument. Otherwise, if TRUE, PHY files for this
     *  instrument will be generated only if the other requisite conditions are met.
     */
    bool phy_file_generation_is_enabled_;

  private:

    /* Private Data Members - Status Files: */

    /** The status file path.
     *
     */
    path status_file_path_;

    /** The status-file-is-updated flag.
     *
     */
    bool status_file_updated_;

  private:

    /* Private Data Members - Sensor Gains and Offsets: */

    /** The pressure sensitivity gain factor, in dBar/LSB.
     *
     *  This field is used for decoding the drift or profile data encoded in the the Iridium Short
     *  Burst Data (SBD) messages. It along with the corresponding offset, is used to convert the
     *  raw sensor counts into scientific units.
     */
    uint16_t gain_pressure_;

    /** The pressure sensitivity gain factor for surface data, in dBar/LSB.
     *
     *  This field is used for decoding the surface data encoded in the the Iridium Short Burst
     *  Data (SBD) messages. It along with the corresponding offset, is used to convert the raw
     *  sensor counts into scientific units.
     */
    uint16_t gain_pressure_surface_;

    /** The salinity sensitivity scale factor, in PSU/LSB.
     *
     *  This field is used for decoding the drift or profile data encoded in the the Iridium Short
     *  Burst Data (SBD) messages. It along with the corresponding offset, is used to convert the
     *  raw sensor counts into scientific units.
     */
    uint16_t gain_salinity_;

    /** The salinity sensitivity scale factor for surface data, in PSU/LSB.
     *
     *  This field is used for decoding the surface data encoded in the the Iridium Short Burst
     *  Data (SBD) messages. It along with the corresponding offset, is used to convert the raw
     *  sensor counts into scientific units.
     */
    uint16_t gain_salinity_surface_;

    /** The temperature sensitivity gain factor, in degC/LSB.
     *
     *  This field is used for decoding the drift or profile data encoded in the the Iridium Short
     *  Burst Data (SBD) messages. It along with the corresponding offset, is used to convert the
     *  raw sensor counts into scientific units.
     */
    uint16_t gain_temperature_;

    /** The temperature sensitivity gain factor for surface data, in degC/LSB.
     *
     *  This field is used for decoding the surface data encoded in the the Iridium Short Burst
     *  Data (SBD) messages. It along with the corresponding offset, is used to convert the raw
     *  sensor counts into scientific units.
     */
    uint16_t gain_temperature_surface_;

    /** The pressure offset, in dBar.
     *
     *  This field is used for decoding the drift or profile data encoded in the the Iridium Short
     *  Burst Data (SBD) messages. It along with the corresponding gain, is used to convert the
     *  raw sensor counts into scientific units.
     */
    uint16_t offset_pressure_;

    /** The pressure offset for surface data, in dBar.
     *
     *  This field is used for decoding the surface data encoded in the the Iridium Short Burst
     *  Data (SBD) messages. It along with the corresponding gain, is used to convert the raw
     *  sensor counts into scientific units.
     */
    uint16_t offset_pressure_surface_;

    /** The salinity offset, in PSU.
     *
     *  This field is used for decoding the drift or profile data encoded in the the Iridium Short
     *  Burst Data (SBD) messages. It along with the corresponding gain, is used to convert the
     *  raw sensor counts into scientific units.
     */
    uint16_t offset_salinity_;

    /** The salinity offset for surface data, in PSU.
     *
     *  This field is used for decoding the surface data encoded in the the Iridium Short Burst
     *  Data (SBD) messages. It along with the corresponding gain, is used to convert the raw
     *  sensor counts into scientific units.
     */
    uint16_t offset_salinity_surface_;

    /** The temperature offset, in degC.
     *
     *  This field is used for decoding the drift or profile data encoded in the the Iridium Short
     *  Burst Data (SBD) messages. It along with the corresponding gain, is used to convert the
     *  raw sensor counts into scientific units.
     */
    uint16_t offset_temperature_;

    /** The temperature offset for surface data, in degC.
     *
     *  This field is used for decoding the surface data encoded in the the Iridium Short Burst
     *  Data (SBD) messages. It along with the corresponding gain, is used to convert the raw
     *  sensor counts into scientific units.
     */
    uint16_t offset_temperature_surface_;

  public:

    /* Public Static Member Functions */

    static vector<shared_ptr<XFloat> > GetAllCachedFloats();

    static shared_ptr<XFloat> GetFloat(int instrument_id);

    static void LoadLocalCacheFromDatabase();

    static void LoadMetadataFromFiles();

    static void SaveLocalCacheToDatabase();

    static shared_ptr<XFloat> SelectObjectFromDatabase(
        const string& where = "",
        const string& groupBy = "",
        const string& orderBy = "",
        const unsigned int& offset = 0);

    static vector<shared_ptr<XFloat> > SelectObjectsFromDatabase(
        const string& where = "",
        const string& groupBy = "",
        const string& orderBy = "",
        const unsigned int& limit = 0,
        const unsigned int& offset = 0);

    static vector<shared_ptr<XFloat> > SelectAllObjectsFromDatabase(
        const string& groupBy = "",
        const string& orderBy = "",
        const unsigned int& limit = 0,
        const unsigned int& offset = 0);

    template<typename TT> static TT SelectValueFromDatabase(
        const string& what,
        const string& where = "",
        const string& groupBy = "",
        const string& orderBy = "",
        const unsigned int& offset = 0);

    static void StartupTasks();

  private:

    /* Private Static Member Functions: */

    static kmldom::DocumentPtr GenerateKmlMasterDocument();

    static string GenerateKmlMasterString();

    static void WriteKmlMasterFile();

  public:

    /* Public Member Functions */

    XFloat(
        int whoi,
        long long imei = XFLT_ID_UNKNOWN,
        int wmo = XFLT_ID_UNKNOWN,
        int dac = XFLT_ID_UNKNOWN);

    XFloat();

    ~XFloat();

    vector<shared_ptr<XFloatDiveData> > GetAllDiveData();

    shared_ptr<XFloatDiveData> GetDiveData(int dive, bool load_from_local_cache_only = false);

    vector<shared_ptr<x::XGPS> > GetGpsPositions() const;

    path GetListingDirectoryPath() const;

    void ResetCachedDiveData(int dive) ATTR_NO_EXCEPTIONS;

    void ResetDiveData();

  private:

    /* Private Member Functions: */

    kmldom::DocumentPtr GenerateKmlDocument(
      bool set_default_kml_style = true,
      bool visibility = true) const;

    string GenerateKmlString();

    string GenerateStatusString() const;

    void SaveObjectToDatabase() const;

    bool WriteKmlFile();

    void WriteStatusFile();

  public:

    /* Public Static Accessor Methods: */

    static const vector<unsigned int>& instrument_ids_to_disable_phy_generation();

    static const vector<unsigned int>& instrument_ids_to_enable_phy_generation();

  public:

    /* Public Accessor Methods: */

    /** Get the enumerated Argo instrument type identifier.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The enumerated Argo instrument type identifier.
     */
    uint8_t argo_instrument_type() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return argo_instrument_type_;
    }

    /** Get the firmware version.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return   The firmware version.
     */
    uint32_t fw_version() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return fw_version_;
    }

    /** Get the pressure gain value, in dBar/LSB.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The pressure gain value, in dBar/LSB.
     */
    uint16_t gain_pressure() const ATTR_DEPRECATED {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return gain_pressure_;
    }

    /** Get the pressure gain value for surface data, in dBar/LSB.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The pressure gain value, in dBar/LSB.
     */
    uint16_t gain_pressure_surface() const ATTR_DEPRECATED {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return gain_pressure_surface_;
    }

    /** Get the salinity gain value, in PSU/LSB.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The salinity gain value, in PSU/LSB.
     */
    uint16_t gain_salinity() const ATTR_DEPRECATED {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return gain_salinity_;
    }

    /** Get the salinity gain value for surface data, in PSU/LSB.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The salinity gain value, in PSU/LSB.
     */
    uint16_t gain_salinity_surface() const ATTR_DEPRECATED {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return gain_salinity_surface_;
    }

    /** Get the temperature gain value, in degC/LSB.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The temperature gain value, in degC/LSB.
     */
    uint16_t gain_temperature() const ATTR_DEPRECATED {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return gain_temperature_;
    }

    /** Get the temperature gain value for surface data, in degC/LSB.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The temperature gain value, in degC/LSB.
     */
    uint16_t gain_temperature_surface() const ATTR_DEPRECATED {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return gain_temperature_surface_;
    }

    bool is_kml_document_updated() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return kml_document_updated_;
    }

    bool is_kml_file_updated() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return kml_file_updated_;

    }

    bool is_status_file_updated() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return status_file_updated_;

    }

    const kmldom::DocumentPtr& kml_document() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return kml_document_;
    }

    const path& kml_file_path() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return kml_file_path_;
    }

    int latest_gps_position_dive_number() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return latest_gps_position_dive_number_;
    }

    ptime latest_gps_position_datetime() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return latest_gps_position_datetime_;
    }

    double latest_gps_position_latitude() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return latest_gps_position_latitude_;
    }

    double latest_gps_position_longitude() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return latest_gps_position_longitude_;
    }

    string latest_gps_position_as_iso6709_string() const {
      return GenerateIso6709String(latest_gps_position_latitude(), latest_gps_position_longitude());
    }

    int latest_iridium_position_cep_radius() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return latest_iridium_position_cep_radius_;
    }

    ptime latest_iridium_position_datetime() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return latest_iridium_position_datetime_;
    }

    int latest_iridium_position_dive_number() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return latest_iridium_position_dive_number_;
    }

    double latest_iridium_position_latitude() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return latest_iridium_position_latitude_;
    }

    double latest_iridium_position_longitude() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return latest_iridium_position_longitude_;
    }

    string latest_iridium_position_as_iso6709_string() const {
      return GenerateIso6709String(
          latest_iridium_position_latitude(),
          latest_iridium_position_longitude());
    }

    const ptime& latest_iridium_startup_position_datetime() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return latest_iridium_startup_position_datetime_;
    }

    int latest_iridium_startup_sbd_momsn() const {
      return latest_iridium_startup_sbd_momsn_;
    }

    int latest_sbd_momsn() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return latest_iridium_sbd_momsn_;
    }

    int latest_sbd_mtmsn() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return latest_iridium_sbd_mtmsn_;
    }

    /** Get the metadata object for this instrument.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     A shared pointer to the metadata object for this instrument.
     */
    const shared_ptr<XFloatMetadata>& metadata() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return metadata_;
    }

    bool netcdf_file_generation_is_enabled() const {
      return netcdf_file_generation_is_enabled_;
    }

    /** Get the pressure offset value for this instrument, in dBar.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The pressure offset value for this instrument, in dBar.
     */
    uint16_t offset_pressure() const ATTR_DEPRECATED {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return offset_pressure_;
    }

    /** Get the pressure offset value for surface data, in dBar.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The pressure offset value for this instrument, in dBar.
     */
    uint16_t offset_pressure_surface() const ATTR_DEPRECATED {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return offset_pressure_surface_;
    }

    /** Get the salinity offset value for this instrument, in PSU.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The salinity offset value for this instrument, in PSU.
     */
    uint16_t offset_salinity() const ATTR_DEPRECATED {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return offset_salinity_;
    }

    /** Get the salinity offset value for surface data, in PSU.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The salinity offset value for this instrument, in PSU.
     */
    uint16_t offset_salinity_surface() const ATTR_DEPRECATED {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return offset_salinity_surface_;
    }

    /** Get the temperature offset value for this instrument, in degC.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The temperature offset value for this instrument, in degC.
     */
    uint16_t offset_temperature() const ATTR_DEPRECATED {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return offset_temperature_;
    }

    /** Get the temperature offset value for surface data, in degC.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The temperature offset value for this instrument, in degC.
     */
    uint16_t offset_temperature_surface() const ATTR_DEPRECATED {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return offset_temperature_surface_;
    }

    /** Is PHY file generation enabled?
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     TRUE if PHY file generation is enabled, or FALSE otherwise.
     */
    bool phy_file_generation_is_enabled() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return phy_file_generation_is_enabled_;
    }

    /** Get the date/time of the start-of-mission for this instrument.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The date/time of the start-of-mission for this instrument.
     */
    ptime start_of_mission_datetime() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return start_of_mission_datetime_;
    }

    const path& status_file_path() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return status_file_path_;
    }

    /** Get the X-Message major version identifier for this instrument.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The X-Message major version identifier for this instrument.
     */
    uint8_t xmsg_version_major() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return xmsg_version_major_;
    }

    /** Get the X-Message minor version identifier for this instrument.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The X-Message minor version identifier for this instrument.
     */
    uint8_t xmsg_version_minor() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return xmsg_version_minor_;
    }

  private:

    /* Private Mutator Methods: */

    /** Set the enumerated Argo instrument type identifier.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  argo_instrument_type    The enumerated Argo instrument type identifier to set.
     */
    void set_argo_instrument_type(uint8_t argo_instrument_type) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      argo_instrument_type_ = static_cast<XArgoInstrumentType_t>(argo_instrument_type);
    }

    /** Set the instrument firmware version.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  fw_version  The firmware version
     */
    void set_fw_version(uint32_t fw_version) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      fw_version_ = fw_version;
    }

    /** Set the instrument pressure gain value, in dBar/LSB.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  gain    The instrument pressure gain value to set, in dBar/LSB.
     */
    void set_gain_pressure(uint16_t gain) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      gain_pressure_ = gain;
    }

    /** Set the instrument pressure gain value for surface data, in dBar/LSB.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  gain    The instrument pressure gain value to set, in dBar/LSB.
     */
    void set_gain_pressure_surface(uint16_t gain) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      gain_pressure_surface_ = gain;
    }

    /** Set the instrument salinity gain value, in PSU/LSB.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  gain    The instrument salinity gain value to set, in PSU/LSB.
     */
    void set_gain_salinity(uint16_t gain) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      gain_salinity_ = gain;
    }

    /** Set the instrument salinity gain value for surface data, in PSU/LSB.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  gain    The instrument salinity gain value to set, in PSU/LSB.
     */
    void set_gain_salinity_surface(uint16_t gain) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      gain_salinity_surface_ = gain;
    }

    /** Set the instrument temperature gain value, in degC/LSB.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  gain    The instrument temperature gain value to set, in degC/LSB.
     */
    void set_gain_temperature(uint16_t gain) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      gain_temperature_ = gain;
    }

    /** Set the instrument temperature gain value for surface data, in degC/LSB.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  gain    The instrument temperature gain value to set, in degC/LSB.
     */
    void set_gain_temperature_surface(uint16_t gain) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      gain_temperature_surface_ = gain;
    }

    void set_kml_document(const kmldom::DocumentPtr& kml_document) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      kml_document_ = kml_document;
    }

    void set_kml_file_path(const path& kml_file_path) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      kml_file_path_ = kml_file_path;
    }

    void set_kml_file_updated(bool kml_file_updated = true) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      kml_file_updated_ = kml_file_updated;
    }

    void set_kml_document_updated(bool kml_document_updated = true) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      kml_document_updated_ = kml_document_updated;
    }

    void set_latest_gps_position_dive_number(int latest_gps_position_dive_number) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      latest_gps_position_dive_number_ = latest_gps_position_dive_number;
    }

    void set_latest_gps_position_datetime(const ptime& latest_gps_position_datetime) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      latest_gps_position_datetime_ = latest_gps_position_datetime;
    }

    void set_latest_gps_position_latitude(double latest_gps_position_latitude) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      latest_gps_position_latitude_ = latest_gps_position_latitude;
    }

    void set_latest_gps_position_longitude(double latest_gps_position_longitude) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      latest_gps_position_longitude_ = latest_gps_position_longitude;
    }

    void set_latest_iridium_position_cep_radius(int latest_iridium_position_cep_radius) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      latest_iridium_position_cep_radius_ = latest_iridium_position_cep_radius;
    }

    void set_latest_iridium_position_datetime(const ptime& latest_iridium_position_datetime) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      latest_iridium_position_datetime_ = latest_iridium_position_datetime;
    }

    void set_latest_iridium_position_dive_number(int latest_iridium_position_dive_number) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      latest_iridium_position_dive_number_ = latest_iridium_position_dive_number;
    }

    void set_latest_iridium_position_latitude(double latest_iridium_position_latitude) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      latest_iridium_position_latitude_ = latest_iridium_position_latitude;
    }

    void set_latest_iridium_position_longitude(double latest_iridium_position_longitude) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      latest_iridium_position_longitude_ = latest_iridium_position_longitude;
    }

    void set_latest_iridium_startup_position_datetime(
        const ptime& latest_iridium_startup_position_datetime) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      latest_iridium_startup_position_datetime_ = latest_iridium_startup_position_datetime;
    }

    void set_latest_iridium_startup_sbd_momsn(int latest_iridium_startup_sbd_momsn) {
      latest_iridium_startup_sbd_momsn_ = latest_iridium_startup_sbd_momsn;
    }

    void set_latest_sbd_momsn(int latest_sbd_momsn) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      latest_iridium_sbd_momsn_ = latest_sbd_momsn;
    }

    void set_latest_sbd_mtmsn(int latest_sbd_mtmsn) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      latest_iridium_sbd_mtmsn_ = latest_sbd_mtmsn;
    }

    /** Assign an instrument metadata object to this X-Float object.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  metadata                A shared pointer to the instrument metadata object to
     *                                      assign to this X-Float object.
     */
    void set_metadata(const shared_ptr<XFloatMetadata>& metadata) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      metadata_ = metadata;

      if(!metadata)
        return;

      // Update the metadata IMEI.
      if(metadata->id_imei() != id_imei())
        metadata->set_id_imei(id_imei());

      // Update float ID fields
      set_id_dac(metadata->id_dac());
      set_id_wmo(metadata->id_wmo());
    }

    void set_netcdf_file_generation_is_enabled(bool netcdf_file_generation_is_enabled) {
      netcdf_file_generation_is_enabled_ = netcdf_file_generation_is_enabled;
    }

    /** Set the instrument pressure offset value, in dBar.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  offs    The instrument pressure offset value to set, in dBar.
     */
    void set_offset_pressure(uint16_t offs) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      offset_pressure_ = offs;
    }

    /** Set the instrument pressure offset value for surface data, in dBar.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  offs    The instrument pressure offset value to set, in dBar.
     */
    void set_offset_pressure_surface(uint16_t offs) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      offset_pressure_surface_ = offs;
    }

    /** Set the instrument salinity offset value, in PSU.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  offs    The instrument salinity offset value to set, in PSU.
     */
    void set_offset_salinity(uint16_t offs) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      offset_salinity_ = offs;
    }

    /** Set the instrument salinity offset value for surface data, in PSU.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  offs    The instrument salinity offset value to set, in PSU.
     */
    void set_offset_salinity_surface(uint16_t offs) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      offset_salinity_surface_ = offs;
    }

    /** Set the instrument temperature offset value, in degC.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  offs    The instrument temperature offset value to set, in degC.
     */
    void set_offset_temperature(uint16_t offs) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      offset_temperature_ = offs;
    }

    /** Set the instrument temperature offset value for surface data, in degC.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  offs    The instrument temperature offset value to set, in degC.
     */
    void set_offset_temperature_surface(uint16_t offs) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      offset_temperature_surface_ = offs;
    }

    /** Set the PHY-file-generation-is-enabled flag.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  is_enabled              The PHY-file-generation-is-enabled flag to set.
     */
    void set_phy_file_generation_is_enabled(bool is_enabled) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      phy_file_generation_is_enabled_ = is_enabled;
    }

    /** Set the date/time of the start-of-mission for this instrument.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  datetime                The date/time of the start-of-mission for this
     *                                      instrument to set.
     */
    void set_start_of_mission_datetime(const ptime& datetime) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      start_of_mission_datetime_ = datetime;
    }

    void set_status_file_path(const path& path) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      status_file_path_ = path;
    }

    void set_status_file_updated(bool status_file_updated = true) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      status_file_updated_ = status_file_updated;
    }

    /** Set the X-Message major version number.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  xmsg_version_major      The X-Message major version number to set.
     */
    void set_xmsg_version_major(uint8_t xmsg_version_major) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_version_major_ = xmsg_version_major;
    }

    /** Set the X-Message minor version number.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @param[in]  xmsg_version_minor      The X-Message minor version number to set.
     */
    void set_xmsg_version_minor(uint8_t xmsg_version_minor) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_version_minor_ = xmsg_version_minor;
    }



};

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */

#endif /* XFLOAT_H_ */
