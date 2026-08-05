/** The @ref argo::iridium::xfloat::XFloatDiveData class header file.
 *
 *  This is the header file for the @ref argo::iridium::xfloat::XFloatDiveData class. This class
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
 *  This file contains the class definition and the majority of the class functionality.
 *
 *  @file
 *
 * 	@version    0.6
 *
 * 	@date       Jul 3, 2014 8:56:05 AM
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

#ifndef XFLOATDIVEDATA_H_
#define XFLOATDIVEDATA_H_

/* Includes: */

#include "XFloatDefinitions.h"  // local

namespace argo {
namespace iridium {
namespace xfloat {

/* Class Definitions: */

// forward declarations
class XFloat;
class XFloatMailMessage;

/** The instrument or float dive data class.
 *
 *  This class contains all of the mail messages and corresponding decoded telemetry data for an
 *  instrument for a single dive. Each time a mail message is received by the telemetry decoder
 *  thread, @ref XFloatTelemetryDecoderService, the corresponding dive data object is retrieved
 *  and the mail message is assigned to it. Then, the binary data from the attached Iridium Short
 *  Burst Data (SBD) message is decoded by calling @ref DecodeXMsgData(). As a result of decoding
 *  the binary message data, an object corresponding to the message type is created and its field
 *  are populated from the decoded data in the binary message. The newly created object will be
 *  of a type that inherits from @ref util::xmsg::XMessage. A shared reference to the object is
 *  assigned to the dive data object.
 *
 *  This class contains the functionality for writing the formatted intermediate and PHY listing
 *  files to the local filesystem. Each time a new message is received, the entire intermediate
 *  listing file will be written to the local filesystem, overwriting the previous listing file,
 *  if it exists. The PHY file is written only when the dive data object is determined to contain
 *  the complete data for dive, and only if the instrument has been deployed i.e. it has a
 *  metadata file with valid DAC and WMO identifiers.
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
 *  @todo   Devise a scheme to ensure that the dive data objects do not remain in memory
 *          indefinitely if one or more message for the dive or cycle are not received and the dive
 *          data object is not completed.
 */
class XFloatDiveData:
    public Iridium,
    protected util::date::DateUtils,
    protected util::file::FileSystemUtils,
    protected util::mysql::LocalPersistentEntity/*,
    public util::netcdf::NetcdfUtils*/ {

    /* Friends Classes: */

    friend class XFloat;

    friend class XFloatTelemetryDecoderService;

    /* Friend Functions: */

    /** Friend function for populating some of the static data members of this class from the
     *  configuration file.
     */
    friend options_description util::config::LocalConfiguration::GenerateConfigurationFileOptionsDescription();

    /** Friend function for creating an X-Float object using the private constructor and a shared
     *  pointer to that object.
     */
    friend shared_ptr<XFloatDiveData> std::make_shared<XFloatDiveData>();

  private:

    /* Static Const Data Members: */

    /** The NetCDF referecne date/time.
     *
     *  This field is used as the reference date/time for generation of NetCDF files where dates
     *  are represented as days since 00:00Z on this date and the fractional remainder represents
     *  the time-of-day. The value of this field represents the date/time 1950-01-01T00:00:00Z.
     */
    static const ptime netcdf_reference_datetime_;

    /** The SQL table description object.
     *
     *  This SQL table description provides the details needed to map this object to a relational
     *  database table. It utilizes the accessor methods of the class and the details of the SQL
     *  table column to (1) create an SQL INSERT statement to save the object into the database,
     *  (2) populate one or more objects' fields as a result of an SQL SELECT query, or (3) query
     *  the SQL database to access one or more of the values that are stored the the SQL table
     *  corresponding to this object.
     */
    static const util::mysql::SqlTableDescription<XFloatDiveData> sql_table_description_;

  private:

    /* Static Data Members: */

    /** The global intermediate-listing-file-generation-is-enabled flag.
     *
     *  The value of this field determines whether or not any intermediate listing files will be
     *  written to the local filesystem. If TRUE, the intermediate listing files will be written
     *  as new messages are received by the application. Otherwise, if FALSE, no intermediate
     *  listing files will be written to the local filesystem.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static bool intermediate_listing_file_generation_is_enabled_;

    /** The intermediate listing directory path on the local filesystem.
     *
     *  The value of this field determines the base directory to which intermediate listing files
     *  will be written. Intermediate listing files will be written to subdirectories of this path,
     *  which are organized by the unique instrument identifiers. Intermediate listing files will
     *  be written each time a new message is received from an instrument.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static path intermediate_listing_directory_path_;

    /** The global NetCDF-file-generation-is-enabled flag.
     *
     *  The value of this field determines whether or not any NetCDF files will be written to the
     *  local filesystem. If TRUE, then NetCDF files will be written each time a complete set of
     *  dive data is received from a deployed instrument and NetCDF file generation is enabled for
     *  that instrument. Otherwise, if FALSE, no NetCDF files will be written to the local
     *  filesystem.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static bool netcdf_file_generation_is_enabled_;

    /** The NetCDF directory path on the local filesystem.
     *
     *  The value of this field determines the base directory to which NetCDF files will be
     *  written. NetCDF files will be written to subdirectories of this path, which are organized
     *  by the unique instrument identifiers. NetCDF files will be written each time a new message
     *  is received from an instrument that completes all the necessary data for that dive and
     *  NetCDF file generation is enabled for that instrument.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static path netcdf_directory_path_;

    /** The global PHY-file-generation-is-enabled flag.
     *
     *  The value of this field determines whether or not any PHY files will be written to the
     *  local filesystem. If TRUE, then PHY files will be written each time a complete set of dive
     *  data is received from a deployed instrument and PHY file generation is enabled for that
     *  instrument. Otherwise, if FALSE, no PHY files will be written to the local filesystem.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static bool phy_file_generation_is_enabled_;

    /** The PHY directory path on the local filesystem.
     *
     *  The value of this field determines the base directory to which PHY files will be written.
     *  PHY files will be written to subdirectories of this path, which are organized by the
     *  unique instrument identifiers. PHY files will be written each time a new message is
     *  received from an instrument that completes all the necessary data for that dive and PHY
     *  file generation is enabled for that instrument.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static path phy_directory_path_;

    /** The global surface-pressure-offset-correction-is-enabled-in-intermediate-listing-files flag.
     *
     *  The value of this flag determines whether the surface pressure offset correction is applied
     *  to all of the pressure values in the intermediate listing files. If TRUE, the surface
     *  pressure offset correction will be applied to the intermediate listing files. Otherwise,
     *  the pressure offset correction will not be applied.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static bool pressure_offset_correction_enabled_list_file_;

    /** The global surface-pressure-offset-correction-is-enabled-in-NetCDF-files flag.
     *
     *  The value of this flag determines whether the surface pressure offset correction is applied
     *  to all of the pressure values in the NetCDF files. If TRUE, the surface pressure offset
     *  correction will be applied to the NetCDF files. Otherwise, the pressure offset correction
     *  will not be applied.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static bool pressure_offset_correction_enabled_netcdf_file_;

    /** The global surface-pressure-offset-correction-is-enabled-in-PHY-files flag.
     *
     *  The value of this flag determines whether the surface pressure offset correction is applied
     *  to all of the pressure values in the PHY files. If TRUE, the surface pressure offset
     *  correction will be applied to the PHY files. Otherwise, the pressure offset correction
     *  will not be applied.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static bool pressure_offset_correction_enabled_phy_file_;

  private:

    /* Data Members: */

    /** The enumerated Argo instrument type identifier.
     *
     *  This field is used to identify the specific type of instrument i.e. Solo-II, Deep Argo,
     *  or ALAMO/Hurricane floats. The value of this field may affect the behavior of some
     *  aspects of the application. For example, the manner in which some Iridium Short Burst Data
     *  (SBD) messages are decoded is dependent on the specific type of instrument.
     */
    XArgoInstrumentType_t argo_instrument_type_;

    /** The dive-data-is-complete flag.
     *
     *  The value of this field indicates whether the dive data object is complete. The
     *  completeness of the dive data is determined by \ref CheckCompleteness() which is invoked
     *  after each new mail message is decoded and assigned to this dive data object.
     *
     *  The dive data is determined to be complete if the dive data object meets all of the
     *  following criteria: it contains a complete set of profile data, the sensor gains and
     *  offsets for this dive are known (i.e. the Argo data message has been received), it
     *  contains a set of mail messages with contiguous packet identifiers (PIDs) starting at
     *  PID=0, and it contains a valid position for the profile. These criteria is determined by
     *  the functions \ref HasCompleteProfileData(), \ref SensorGainsAndOffsetsAreKnown(),
     *  \ref HasContiguousPids(), \ref HasValidPosition(), respectively.
     */
    bool complete_;

    /** The dive or cycle number of this dive data object.
     *
     *  This field is used to index the dive data object for a given instrument. Together, the
     *  unique instrument identifier and the dive or cycle number provide a unique index of the
     *  dive data object.
     */
    int id_dive_;

    /** The date/time of transmission of the first Iridium message sent by the instrument for this
     *  dive.
     *
     *  This field is updated as a result of assigning a mail message to this dive data object by
     *  calling the function \ref AssignMailMessage(const shared_ptr<XFloatMailMessage>& x).
     */
    ptime iridium_position_datetime_first_;

    /** The date/time of transmission of the last Iridium message sent by the instrument for this
     *  dive.
     *
     *  This field is updated as a result of assigning a mail message to this dive data object by
     *  calling the function \ref AssignMailMessage(const shared_ptr<XFloatMailMessage>& x).
     */
    ptime iridium_position_datetime_last_;

    /** The mail messages for this dive data object.
     *
     *  This vector contains a set of shared reference to the mail messages that contain the
     *  telemetry data for this dive. Each time a mail message is received by the telemetry
     *  decoder thread, it is assigned to the corresponding dive data object before the thread
     *  goes on to decode the message.
     *
     *  This set of mail message may be used to reprocess the telemetry data for a dive, if
     *  necessary. For example, if a new Argo message arrives with different sensor gain and
     *  offset values than the previous dive, then the sensor data messages may be reprocessed
     *  using the new sensor gain and offset values.
     */
    static_vector<shared_ptr<XFloatMailMessage>, XMSG_MAX_MAIL> mail_messages_;

    /** The number of SBD messages that have been received for this dive cycle.
     *
     */
    uint16_t num_sbd_msgs_;

    /** The parent float or instrument object of this dive data object.
     *
     *  This field provides a shared back-reference to the instrument object that owns this dive
     *  data object. This field specifies the any-to-one or many-to-one relationship between dive
     *  data object and instrument objects.
     *
     *  Every dive data object must have a parent instrument object. In other words, value of this
     *  field should never be NULL.
     */
    shared_ptr<XFloat> parent_instrument_;

    /* Data Members - NetCDF: */

    /** The NetCDF dimensions.
     *
     */
//    map<string, NcDim> nc_dims_;

    /** The NetCDF file path.
     *
     */
    path netcdf_file_path_;

    /** The NetCDF-file-is-updated flag.
     *
     */
    bool netcdf_file_updated_;

    /* Data Members - Derived Fields - Pressure Offset: */

    /** The computed surface pressure offset for this dive.
     *
     */
    float pres_offs_;

    /** The number of dives used to compute the surface pressure offset.
     *
     */
    int pres_offs_num_dives_;

    /** The number of samples used to compute the surface pressure offset.
     *
     */
    int pres_offs_num_samples_;

    /** The surface-pressure offset-is-valid flag.
     *
     */
    bool pres_offs_valid_;

    /* Data Members - Derived Fields - Surface Pressure: */

    /** The maximum reported surface pressure.
     *
     */
    float surface_pres_max_;

    /** The mean surface pressure.
     *
     */
    float surface_pres_mean_;

    /** The minimum reported surface pressure.
     *
     */
    float surface_pres_min_;

    /** The number of surface pressure samples reported.
     *
     */
    int surface_pres_num_samples_;

    /** The surface-pressure-is-valid flag.
     *
     */
    bool surface_pres_valid_;

    /** The surface pressure variance.
     *
     */
    float surface_pres_variance_;

    /* Data Members - Files: */

    /** The path to the intermediate listing file for this dive data on the local filesystem.
     *
     *  This field is set as a result of writing the intermediate listing file i.e. by calling
     *  @ref WriteListingFile(). This path is relative to the listing directory, which is
     *  specified by the static data member @ref intermediate_listing_directory_path_.
     */
    path listing_file_path_;

    /** The listing-file-is-updated flag.
     *
     */
    bool listing_file_updated_;

    /** The path to the PHY file for this dive data on the local filesystem.
     *
     *  This field is set as a result of writing the PHY listing file i.e. by calling @ref
     *  WritePhyFile(). This path is relative to the PHY listing directory, which is specified by
     *  the static data member, @ref intermediate_listing_directory_path_.
     */
    path phy_file_path_;

    /** The phy-file-is-updated flag.
     *
     */
    bool phy_file_updated_;

    /* Dynamic Data Members - X-Message Data: */

    /** A vector of pointers to the acoustic position messages.
     *
     *  @note           This message type is used by MIZ floats only.
     *  @note           This vector is sorted upon insertion by order of non-decreasing message
     *                  indices.
     */
    shared_ptr<x::XAcousticPosition> xmsg_acoustic_positions_;

    /** A pointer to the Argo data message. */
    shared_ptr<x::XArgo> xmsg_argo_data_;

    /** The composite binned profile data object.
     *
     *  This object contains all of the binned profile data for this dive/cycle.
     */
    x::XCompositeProfile xmsg_composite_profile_binned_;

    /** The composite unbinned profile data object.
     *
     *  This object contains all of the unbinned profile data for this dive/cycle.
     */
    x::XCompositeHiResProfile xmsg_composite_profile_unbinned_;

    /** The composite drift data object.
     *
     *  This object contains all of the drift data for this dive/cycle.
     *
     */
    x::XCompositeDrift xmsg_composite_drift_data_;

    /** A pointer to the engineering abort message. */
    shared_ptr<x::XEngAbort> xmsg_engineering_data_abort_;

    /** A pointer the the engineering ascent message.
     *
     *  @note           This message type is used by ALAMO floats only.
     */
    shared_ptr<x::XEngAscent> xmsg_engineering_data_ascent_;

    /** A pointer to the engineering BIT failed message.
     *
     */
    shared_ptr<x::XEngBITFail> xmsg_engineering_data_bit_fail_;

    /** A pointer to the engineering BIT passed message.
     *
     */
    shared_ptr<x::XEngBITPass> xmsg_engineering_data_bit_passed_;

    /** A pointer to the engineering descent message.
     *
     *  @note           This message type is used by ALAMO floats only.
     */
    shared_ptr<x::XEngDescent> xmsg_engineering_data_descent_;

    /** A pointer to the engineering diagnostic or startup message.
     *
     */
    shared_ptr<x::XEngDiagnostic> xmsg_engineering_data_diagnostic_dive_;

    /** A pointer to the engineering drift message.
     *
     *  @note           This message type is used by ALAMO floats only.
     */
    shared_ptr<x::XEngDrift> xmsg_engineering_data_drift_;

    /** A pointer to the ice-detection engineering message.
     *
     */
    shared_ptr<x::XEngIce> xmsg_engineering_data_ice_;

    /** A pointer to the standard dive-cycle engineering message.
     *
     */
    shared_ptr<x::XEngStandard> xmsg_engineering_data_standard_dive_;

    /** A pointer to the engineering surface message.
     *
     *  @note           This message type is used by ALAMO floats only.
     */
    shared_ptr<x::XEngSurface> xmsg_engineering_data_surface_;

    /** A static vector of pointers to the fall time-series messages.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message
     *                  indices.
     */
    static_vector<shared_ptr<x::XFall>, XMSG_MAX_FALL> xmsg_fall_time_series_;

    /** A static vector of pointers to the GPS position message(s).
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message
     *                  indices.
     */
    static_vector<shared_ptr<x::XGPS>, XMSG_MAX_GPS> xmsg_gps_positions_;

    /** A vector of pointer to mission configuration command upload echo-back messages.
     *
     *  @note           These messages are unordered.
     */
    vector<shared_ptr<x::XMissionCmdEcho> > xmsg_mission_cmd_echoback_;

    /** The MFA mission configruation object.
     *
     */
    x::XMfaMissionCfg xmsg_mission_cfg_;

    /** A static vector of pointers to the pump time-series messages.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message
     *                  indices.
     */
    static_vector<shared_ptr<x::XPump>, XMSG_MAX_PUMP> xmsg_pump_time_series_;

    /** A static vector of pointers to the fall time-series messages.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message
     *                  indices.
     */
    static_vector<shared_ptr<x::XRise>, XMSG_MAX_RISE> xmsg_rise_time_series_;

    /** The surface pressure time-series object.
     *
     */
    x::XCompositeSurfaceData xmsg_surface_time_series_;

    /** Valve operation messages.
     *
     */
    static_vector<shared_ptr<x::XValve>, XMSG_MAX_VALVE> xmsg_valve_data_;

    /** A static vector of pointers to the surface wave field data messages.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message
     *                  indices.
     */
    shared_ptr<x::XWave> xmsg_wave_field_;

  public:

    /* Public Static Member Functions*/

    /** Generate a human-readable hex string.
     *
     *  @param[in]  data  The message data.
     *
     *  @return    A human readable hex representation of the data.
     */
    static string GenerateReadableHex(const vector<uint8_t>& data);

  public:

    /* Public Member Functions: */

    XFloatDiveData();

    XFloatDiveData(const shared_ptr<XFloat>& parent, int dive);

    ~XFloatDiveData();

    bool CheckCompleteness();

    int DecodeMailMessageData(shared_ptr<XFloatMailMessage>& msg);

    void DecodeXMsgData(
        const vector<uint8_t>& data,
        shared_ptr<vector<uint8_t> > xmsg_id_types = shared_ptr<vector<uint8_t> >())
            ATTR_NO_EXCEPTIONS;

    void GetGainsAndOffsets(
        uint16_t& pg,
        uint16_t& po,
        uint16_t& tg,
        uint16_t& to,
        uint16_t& sg,
        uint16_t& so) const;

    void GetSurfaceGainsAndOffsets(
        uint16_t& pg,
        uint16_t& po,
        uint16_t& tg,
        uint16_t& to,
        uint16_t& sg,
        uint16_t& so) const;

    float ComputePressureOffsetFromSurfaceTimeSeries(unsigned int max_num_dives);

    string GenerateListingString() const;

    string GeneratePhyString() const;

    string GeneratePressureCorrectionListingString() const;

    shared_ptr<x::XGPS> GetGpsPositionEndOfDive() const;

    shared_ptr<x::XGPS> GetGpsPositionStartOfDive() const;

    shared_ptr<x::XGPS> GetGpsProfilePosition() const;

    bool GpsPositionIsValidProfilePosition(const shared_ptr<x::XGPS> x) const;

    bool HasDiveData() const;

    bool HasDriftData() const;

    bool HasEngineeringData() const;

    bool HasFallRiseData() const;

    bool HasCompleteBinnedProfileData() const;

    bool HasCompleteProfileData() const;

    bool HasCompleteUnbinnedProfileData() const;

    bool HasContiguousMomsns() const;

    bool HasContiguousPids() const;

    bool HasPositionData() const;

    bool HasBinnedProfileData() const;

    bool HasBinnedProfileSalinityData() const;

    bool HasUnbinnedProfileData() const;

    bool HasUnbinnedProfileSalinityData() const;

    bool HasProfileSalinityData() const;

    bool HasValidGpsPosition() const;

    bool HasValidIridiumPosition() const;

    bool HasValidPosition() const;

    bool HasWaveData() const;

    void ResetData();

    bool SensorGainsAndOffsetsAreKnown() const;

    void WriteListingFile();

//    void WriteNetCDFFile();

    void WritePhyFile();

  private:

    /* Private Member Functions: */


    shared_ptr<x::XCompositeFall> GenerateCompositeFallTimeSeriesData() const;

    shared_ptr<x::XCompositeIridiumPosition> GenerateCompositeIridiumPosition() const;

    shared_ptr<x::XCompositePump> GenerateCompositePumpTimeSeriesData() const;

    shared_ptr<x::XCompositeRise> GenerateCompositeRiseTimeSeriesData() const;

    shared_ptr<x::XCompositeValve> GenerateCompositeValveData() const;

    vector<int> GenerateOrderedPidList() const;

    vector<int> GenerateOrderedMomsnList() const;

    void UpdateDerivedSurfaceDataFields() {

      // Sensor gains & offsets for surface data.
      uint16_t spg, spo, ssg, sso, stg, sto;

      try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        ldbg() << "Updating derived surface data fields...";

        if(!SensorGainsAndOffsetsAreKnown()) {

          set_surface_pres_max(XMSG_SENS_BAD_PRES);
          set_surface_pres_mean(XMSG_SENS_BAD_PRES);
          set_surface_pres_min(XMSG_SENS_BAD_PRES);
          set_surface_pres_valid(false);
          set_surface_pres_variance(XMSG_SENS_BAD_PRES);

          return;

        }

        // Get surface gains & offsets.
        GetSurfaceGainsAndOffsets(spg, spo, stg, sto, ssg, sso);

        // Handle missing data.
        if(xmsg_surface_time_series_.IsEmpty()) {

          set_surface_pres_max(XMSG_SENS_BAD_PRES);
          set_surface_pres_mean(XMSG_SENS_BAD_PRES);
          set_surface_pres_min(XMSG_SENS_BAD_PRES);
          set_surface_pres_valid(false);
          set_surface_pres_variance(XMSG_SENS_BAD_PRES);

          return;

        }

        if(xmsg_surface_time_series_.PresMeanAsFloat(spg, spo) > 5.
            || xmsg_surface_time_series_.PresMeanAsFloat(spg, spo) < -5.) {
          set_surface_pres_max(XMSG_SENS_BAD_PRES);
          set_surface_pres_mean(XMSG_SENS_BAD_PRES);
          set_surface_pres_min(XMSG_SENS_BAD_PRES);
          set_surface_pres_valid(false);
          set_surface_pres_variance(XMSG_SENS_BAD_PRES);

          return;

        }

        // Compute derived surface data fields.
        set_surface_pres_max(xmsg_surface_time_series_.PresMaxAsFloat(spg, spo));
        set_surface_pres_mean(xmsg_surface_time_series_.PresMeanAsFloat(spg, spo));
        set_surface_pres_min(xmsg_surface_time_series_.PresMinAsFloat(spg, spo));
        set_surface_pres_num_samples(xmsg_surface_time_series_.pres_num_samples());
        set_surface_pres_variance(xmsg_surface_time_series_.PresVarAsFloat(spg));

        // Determine surface pressure validity
        if(!xmsg_surface_time_series_.IsDataComplete()) {

          ldbg() << "Surface data incomplete.";

          set_surface_pres_valid(false);

          return;

        }

        if(!HasValidGpsPosition()) {

          ldbg() << "Surface data invalid. No GPS position.";

          set_surface_pres_valid(false);

          return;

        }

        ldbg() << "Surface data valid.";

        set_surface_pres_valid(true);

        // TODO Check status of ice-detection.

      }CATCH_BLOCK("Failed to update derived surface data fields!")
    }

  private:

    /* Private NetCDF Member Functions: */

//    map<string, NcDim> NC_GenerateDimensions(NcGroup& nc_group);
//
//    void NC_GenerateGlobalAttrs(NcGroup& nc_group) const;
//
//    void NC_GenerateVariables(NcGroup& nc_group);

  public:

    /* Public SQL Database Member Functions: */

    static void DeleteFromDatabase(int id_instrument) {

      ostringstream os;

      try {

        // Delete GPS objects.
        x::XGPS::DeleteFromDatabase(id_instrument);

        // Delete dive data objects;
        os << "id_instrument = " << id_instrument;
        sql_table_description_.DeleteObjectsFromDatabase(os.str());

      }CATCH_BLOCK("Failed to delete dive data object(s) from database!")

    }

    static void DeleteObjectsFromDatabase(const string& where) {

      try {

        sql_table_description_.DeleteObjectsFromDatabase(where);

      }CATCH_BLOCK("Failed to delete GPS objects from database!")

    }

    vector<shared_ptr<XFloatMailMessage>> GetDiveDataMailMessagesFromDatabase();

    static shared_ptr<XFloatDiveData> GetDiveDataObjectFromDatabase(
        const shared_ptr<XFloat>& x,
        int dive);

    /** Save this dive data object to the SQL database.
     *
     */
    void SaveObjectToDatabase() const {

      using namespace x;

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

    static shared_ptr<XFloatDiveData> SelectObjectFromDatabase(
        const string& where = "",
        const string& groupBy = "",
        const string& orderBy = "",
        const unsigned int& offset = 0) {

      try {

        return sql_table_description_.SelectObjectFromDatabase(where, groupBy, orderBy, offset);

      }CATCH_BLOCK("Failed to query XFloat objects from the database!")

      return make_shared<XFloatDiveData>();
    }

    static vector<shared_ptr<XFloatDiveData> > SelectObjectsFromDatabase(
        const string& where = "",
        const string& groupBy = "",
        const string& orderBy = "",
        const unsigned int& limit = 0,
        const unsigned int& offset = 0) {

      try {

        return sql_table_description_.SelectObjectsFromDatabase(
            where,
            groupBy,
            orderBy,
            limit,
            offset);

      }CATCH_BLOCK("Failed to query XFloat objects from the database!")

      return vector<shared_ptr<XFloatDiveData> >();
    }

    template<typename TT> static TT SelectValueFromDatabase(
        const string& what,
        const string& where = "",
        const string& groupBy = "",
        const string& orderBy = "",
        const unsigned int& offset = 0) {

      return sql_table_description_.SelectValueFromDatabase<TT>(
          what,
          where,
          groupBy,
          orderBy,
          offset);
    }

  public:

    /* Public Accessor Methods: */

    /** Get the enumerated Argo instrument type identifier.
     *
     *  @return         The enumerated Argo instrument type identifier.
     */
    XArgoInstrumentType_t argo_instrument_type() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return argo_instrument_type_;
    }

    /** Get the dive or cycle number of this dive data object.
     *
     *  @return         The dive or cycle number of this dive data object.
     */
    int id_dive() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return id_dive_;
    }

    /** Get the path to the intermediate listing file on the local filesystem.
     *
     *  @return         The path to the intermediate listing file on the local filesystem.
     */
    const path& intermediate_listing_file_path() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return listing_file_path_;
    }

    const ptime& iridium_position_datetime_first() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return iridium_position_datetime_first_;
    }

    const ptime& iridium_position_datetime_last() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return iridium_position_datetime_last_;
    }

    bool is_complete() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return complete_;
    }

    bool is_listing_file_updated() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return listing_file_updated_;
    }

    bool is_netcdf_file_updated() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return netcdf_file_updated_;
    }

    bool is_phy_file_updated() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return phy_file_updated_;
    }

    bool is_pres_offs_valid() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return pres_offs_valid_;
    }

    bool is_surface_pres_valid() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return surface_pres_valid_;
    }

    uint16_t num_sbd_msgs() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return num_sbd_msgs_;
    }

    const path& netcdf_file_path() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return netcdf_file_path_;
    }

    shared_ptr<XFloat> parent_instrument() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return parent_instrument_;
    }

    /** Get the path to the PHY listing file on the local filesystem.
     *
     *  @return         The path to the PHY listing file on the local filesystem.
     */
    const path& phy_file_path() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return phy_file_path_;
    }

    float surface_pres_max() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return surface_pres_max_;
    }

    float surface_pres_mean() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return surface_pres_mean_;
    }

    float surface_pres_min() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return surface_pres_min_;
    }

    float surface_pres_variance() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return surface_pres_variance_;
    }

    float pres_offs() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return pres_offs_;
    }

    int pres_offs_num_dives() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return pres_offs_num_dives_;
    }

    int pres_offs_num_samples() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return pres_offs_num_samples_;
    }

    int surface_pres_num_samples() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return surface_pres_num_samples_;
    }

  private:

    /* Private Mutator Functions: */



    void AssignArgoData(const shared_ptr<x::XArgo>& x);

    /** Assign a drift pressure message to this dive data object.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message indices.
     *
     *  @param[in]      x       A pointer to the drift pressure message to assign.
     */
    void AssignDriftPressure(const shared_ptr<x::XDriftPressure>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_composite_drift_data_.AssignPressureMessage(x);

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign a drift salinity message to this dive data object.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message indices.
     *
     *  @param[in]      x       A pointer to the drift salinity message to assign.
     */
    void AssignDriftSalinity(const shared_ptr<x::XDriftSalinity>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_composite_drift_data_.AssignSalinityMessage(x);

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign a drift temperature message to this dive data object.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message indices.
     *
     *  @param[in]      x       A pointer to the drift temperature message to assign.
     */
    void AssignDriftTemperatre(const shared_ptr<x::XDriftTemperature>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_composite_drift_data_.AssignTemperatureMessage(x);

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign an engineering abort/beacon message to this dive data object.
     *
     *  @param[in]      x       A pointer to the engineering abort/beacon message to assign.
     */
    void AssignEngineeringAbort(const shared_ptr<x::XEngAbort>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_engineering_data_abort_ = x;

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign an engineering ascent message to this dive data object.
     *
     *  @note           This message type is used by ALAMO floats only.
     *
     *  @param[in]      x       A pointer to the XXengineering ascentX message to assign.
     */
    void AssignEngineeringAscent(const shared_ptr<x::XEngAscent>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_engineering_data_ascent_ = x;

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign an engineering BIT failed message to this dive data object.
     *
     *  @param[in]      x       A pointer to the engineering BIT failed message to assign.
     */
    void AssignEngineeringBitFail(const shared_ptr<x::XEngBITFail>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_engineering_data_bit_fail_ = x;

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign an engineering BIT passed message to this dive data object.
     *
     *  @param[in]      x       A pointer to the engineering BIT passed message to assign.
     */
    void AssignEngineeringBitPass(const shared_ptr<x::XEngBITPass>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_engineering_data_bit_passed_ = x;

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign an engineering descent message to this dive data object.
     *
     *  @note           This message type is used by ALAMO floats only.
     *
     *  @param[in]      x       A pointer to the engineering descent message to assign.
     */
    void AssignEngineeringDescent(const shared_ptr<x::XEngDescent>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_engineering_data_descent_ = x;

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign an engineering diagnostic dive message to this dive data object.
     *
     *  @param[in]      x       A pointer to the engineering diagnostic dive message to assign.
     */
    void AssignEngineeringDiagnostic(const shared_ptr<x::XEngDiagnostic>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_engineering_data_diagnostic_dive_ = x;

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign an engineering drift/park message to this dive data object.
     *
     *  @note           This message type is used by ALAMO floats only.
     *
     *  @param[in]      x       A pointer to the engineering drift/park message to assign.
     */
    void AssignEngineeringDrift(const shared_ptr<x::XEngDrift>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_engineering_data_drift_ = x;

      // Update file status.
      set_listing_file_updated(false);

    }

    void AssignEngineeringIce(const shared_ptr<x::XEngIce>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_engineering_data_ice_ = x;

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign an engineering standard dive message to this dive data object.
     *
     *  @param[in]      x       A pointer to the engineering standard dive message to assign.
     */
    void AssignEngineeringStandard(const shared_ptr<x::XEngStandard>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_engineering_data_standard_dive_ = x;

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign an engineering surface message to this dive data object.
     *
     *  @note           This message type is used by ALAMO floats only.
     *
     *  @param[in]      x       A pointer to the engineering surface message to assign.
     */
    void AssignEngineeringSurface(const shared_ptr<x::XEngSurface>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_engineering_data_surface_ = x;

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign a fall time-series message to this dive data object.
     *
     *  @note       This vector is sorted upon insertion by order of non-decreasing message indices.
     *
     *  @param[in]  x       A pointer to the fall time-series message to assign.
     */
    void AssignFallTimeSeries(const shared_ptr<x::XFall>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      // Iterate to emplacement position.
      auto it = xmsg_fall_time_series_.begin();
      for(; it < xmsg_fall_time_series_.end(); ++it)

        // Check for duplicate messages.
        if(x->xmsg_index() == (*it)->xmsg_index())

          // Duplicate message encountered.
          EXC_THROW("Duplicate X-Message index encountered!");

        else if(x->xmsg_index() < (*it)->xmsg_index())

          // Emplacement position found.
          break;

      // Emplace element at position.
      xmsg_fall_time_series_.emplace(it, x);

      // Update file status.
      set_listing_file_updated(false);

    }

    void AssignGps(shared_ptr<x::XGPS>& x);

    int AssignMailMessage(const shared_ptr<XFloatMailMessage>& msg);

    /** Assign a ground-station command echo-back message to this dive data object.
     *
     *  @param[in]      x       A pointer to the ground-station command echo-back message to assign.
     */
    void AssignMissionCmdEchoback(const shared_ptr<x::XMissionCmdEcho>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_mission_cmd_echoback_.push_back(x);

      // Update file status.
      set_listing_file_updated(false);

    }

    void AssignParentInstrument(const shared_ptr<XFloat>& flt);

    /** Assign a bin-averaged profile pressure message to this dive data object.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message
     *                  indices.
     *
     *  @param[in]      x       A pointer to the bin-averaged profile pressure message to assign.
     */
    void AssignProfilePressureBinned(const shared_ptr<x::XProfilePressure>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_composite_profile_binned_.AssignPressureMessage(x);

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign an unbinned profile pressure message to this dive data object.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message
     *                  indices.
     *
     *  @param[in]      x       A pointer to the unbinned profile pressure message to assign.
     */
    void AssignProfilePressureUnbinned(const shared_ptr<x::XHiResPressure>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_composite_profile_unbinned_.AssignPressureMessage(x);

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign a bin-averaged profile salinity message to this dive data object.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message
     *                  indices.
     *
     *  @param[in]      x       A pointer to the bin-averaged profile salinity message to assign.
     */
    void AssignProfileSalinityBinned(const shared_ptr<x::XProfileSalinity>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_composite_profile_binned_.AssignSalinityMessage(x);

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign an unbinned profile salinity message to this dive data object.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message
     *                  indices.
     *
     *  @param[in]      x       A pointer to the unbinned profile salinity message to assign.
     */
    void AssignProfileSalinityUnbinned(const shared_ptr<x::XHiResSalinity>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_composite_profile_unbinned_.AssignSalinityMessage(x);

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign a bin-averaged profile temperature message to this dive data object.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message
     *                  indices.
     *
     *  @param[in]      x       A pointer to the bin-averaged profile temperature message to assign.
     */
    void AssignProfileTemperatureBinned(const shared_ptr<x::XProfileTemperature>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_composite_profile_binned_.AssignTemperatureMessage(x);

      // Update file status.
      set_listing_file_updated(false);

    }

    void AssignAuxTemperatureBinned(const shared_ptr<x::XProfileTemperature>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_composite_profile_binned_.AssignAuxTemperatureMessage(x);

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign an unbinned profile temperature message to this dive data object.
     *
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message
     *                  indices.
     *
     *  @param[in]      x       A pointer to the unbinned profile temperature message to assign.
     */
    void AssignProfileTemperatureUnbinned(const shared_ptr<x::XHiResTemperature>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_composite_profile_unbinned_.AssignTemperatureMessage(x);

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign a pump time-series message to this dive data object.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message indices.
     *
     *  @param[in]      x       A pointer to the pump time-series message to assign.
     */
    void AssignPumpTimeSeries(const shared_ptr<x::XPump>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      // Iterate to emplacement position.
      auto it = xmsg_pump_time_series_.begin();
      for(; it < xmsg_pump_time_series_.end(); ++it)

        // Check for duplicate messages.
        if(x->xmsg_index() == (*it)->xmsg_index())

          // Duplicate message encountered.
          EXC_THROW("Duplicate X-Message index encountered!");

        else if(x->xmsg_index() < (*it)->xmsg_index())

          // Emplacement position found.
          break;

      // Emplace element at position.
      xmsg_pump_time_series_.emplace(it, x);

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign a rise time-series message to this dive data object.
     *
     *  @note           This vector is sorted upon insertion by order of non-decreasing message indices.
     *
     *  @param[in]      x       A pointer to the rise time-series message to assign.
     */
    void AssignRiseTimeSeries(const shared_ptr<x::XRise>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      // Iterate to emplacement position.
      auto it = xmsg_rise_time_series_.begin();
      for(; it < xmsg_rise_time_series_.end(); ++it)

        // Check for duplicate messages.
        if(x->xmsg_index() == (*it)->xmsg_index())

          // Duplicate message encountered.
          EXC_THROW("Duplicate X-Message index encountered!");

        else if(x->xmsg_index() < (*it)->xmsg_index())

          // Emplacement position found.
          break;

      // Emplace element at position.
      xmsg_rise_time_series_.emplace(it, x);

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Assign a surface pressure time-series message to this dive data object.
     *
     *  @note           This message type is used by ALAMO floats only.
     *  @note           This vector is sorted upon insertion by order of non-decreasing message indices.
     *
     *  @param[in]      x       A pointer to the surface pressure time-series message to assign.
     */
    void AssignSurfaceTimeSeries(const shared_ptr<x::XSurfaceData>& x) {

      try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        // Assign the message.
        xmsg_surface_time_series_.AssignMessage(x);

        // Update file status.
        set_listing_file_updated(false);

      }CATCH_BLOCK("Failed to assign surface time-series data message!")

    }

    void AssignValveData(const shared_ptr<x::XValve>& x) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      // Iterate to emplacement position.
      auto it = xmsg_valve_data_.begin();
      for(; it < xmsg_valve_data_.end(); ++it)

        // Check for duplicate messages.
        if(x->xmsg_index() == (*it)->xmsg_index())

          // Duplicate message encountered.
          EXC_THROW("Duplicate X-Message index encountered!");

        else if(x->xmsg_index() < (*it)->xmsg_index())

          // Emplacement position found.
          break;

      // Emplace element at position.
      xmsg_valve_data_.emplace(it, x);

      // Update file status.
      set_listing_file_updated(false);

    }

    /** Set the enumerated Argo instrument type identifier.
     *
     *  @param[in]  type        The enumerated Argo instrument type identifier.
     */
    void set_argo_instrument_type(XArgoInstrumentType_t type) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      argo_instrument_type_ = type;
    }

    void set_complete(bool complete) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      complete_ = complete;
    }

    /** Set the dive or cycle number of this dive data object.
     *
     *  @param[in]  id_dive     The dive or cycle number.
     */
    void set_id_dive(int id_dive) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      id_dive_ = id_dive;
    }

    /** Set the path to the intermediate listing file.
     *
     *  @param[in]  file_path   The path to the intermediate listing file.
     */
    void set_intermediate_listing_file_path(const path& file_path) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      listing_file_path_ = file_path;
    }

    void set_iridium_position_datetime_first(const ptime& iridium_position_datetime_first) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      iridium_position_datetime_first_ = iridium_position_datetime_first;
    }

    void set_iridium_position_datetime_last(const ptime& iridium_position_datetime_last) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      iridium_position_datetime_last_ = iridium_position_datetime_last;
    }

    void set_listing_file_updated(bool listing_file_updated) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      listing_file_updated_ = listing_file_updated;
    }

    void set_netcdf_file_path(const path& netcdf_file_path) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      netcdf_file_path_ = netcdf_file_path;
    }

    void set_netcdf_file_updated(bool netcdf_file_updated = true) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      netcdf_file_updated_ = netcdf_file_updated;
    }

    void set_num_sbd_msgs(uint16_t num_sbd_msgs) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      num_sbd_msgs_ = num_sbd_msgs;
    }

    /** Set the path to the PHY listing file.
     *
     *  @param[in]  file_path   The path to the PHY listing file.
     */
    void set_phy_file_path(const path& file_path) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      phy_file_path_ = file_path;
    }

    void set_phy_file_updated(bool phy_file_updated) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      phy_file_updated_ = phy_file_updated;
    }

    void set_surface_pres_max(float surface_pres_max) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      surface_pres_max_ = surface_pres_max;
    }

    void set_surface_pres_mean(float surface_pres_mean) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      surface_pres_mean_ = surface_pres_mean;
    }

    void set_surface_pres_min(float surface_pres_min) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      surface_pres_min_ = surface_pres_min;
    }

    void set_surface_pres_valid(bool surface_pres_valid) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      surface_pres_valid_ = surface_pres_valid;
    }

    void set_surface_pres_variance(float surface_pres_variance) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      surface_pres_variance_ = surface_pres_variance;
    }

    void set_pres_offs(float pres_offs) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      pres_offs_ = pres_offs;
    }

    void set_pres_offs_num_dives(int pres_offs_num_dives) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      pres_offs_num_dives_ = pres_offs_num_dives;
    }

    void set_pres_offs_num_samples(int pres_offs_num_samples) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      pres_offs_num_samples_ = pres_offs_num_samples;
    }

    void set_pres_offs_valid(bool pres_offs_valid) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      pres_offs_valid_ = pres_offs_valid;
    }

    void set_surface_pres_num_samples(int surface_pres_num_samples) {
      surface_pres_num_samples_ = surface_pres_num_samples;
    }


};

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */

#endif /* XFLOATDIVEDATA_H_ */
