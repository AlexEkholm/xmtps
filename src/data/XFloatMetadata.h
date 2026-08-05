/** The @ref argo::iridium::xfloat::XFloatMetadata class header file.
 *
 *  This is the header file for the @ref argo::iridium::xfloat::XFloatMetadata class. This class
 *  contains the metadata for a single instrument. The metadata object includes information about
 *  the instrument including details about deployment, principal investigators, and the instrument
 *  type.  During application initialization, the metadata files are read from the local
 *  filesystem, and used to construct one metadata object per metadata file. Additionally,
 *  the metadata is persisted to the SQL database.
 *
 *  This file contains the class definition and the majority of the functionality of the class.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Aug 18, 2014 4:25:34 PM
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

#ifndef XFLOATMETADATA_H_
#define XFLOATMETADATA_H_

/* Includes: */

#include "XFloatDefinitions.h"

namespace argo {
namespace iridium {
namespace xfloat {

/* Class Definition: */

/** The instrument metadata class.
 *
 *  This class contains the metadata for a single instrument. The metadata object includes
 *  information about the instrument including details about deployment, principal investigators,
 *  and the instrument type.  During application initialization, the metadata files are read from
 *  the local filesystem, and used to construct one metadata object per metadata file.
 *  Additionally, the metadata is persisted to the SQL database.
 *
 *  @todo   Devise a method to actively check for (1) changes in the metadata files and (2) newly
 *          created metadata files. Load the metadata from the new or modified files and assign
 *          them to the corresponding float object. If necessary, retroactively process the mail
 *          messages for these instruments. Persist updates to the SQL database as necessary.
 */
class XFloatMetadata:
        public util::file::FileSystemUtils,
        public argo::iridium::Iridium,
        public util::mysql::LocalPersistentEntity,
        protected util::position::Position {

    /* Friends Classes: */

    friend class XFloat;

    /* Friend Functions: */

    /** Friend function for setting the value of some of the static configuration options that are
     *  members of this class.
     */
    friend options_description util::config::LocalConfiguration::GenerateConfigurationFileOptionsDescription();

    friend istream& operator>>(istream& in, XFloatMetadata& metadata);

private:

    /* Static Data Members: */

    /** The path to metadata directory on the local filesystem.
     *
     *  This path indicates the directory containing the instrument metadata files. The value of
     *  this field is set during application initialization as a result of parsing the
     *  command-line arguments and configuration file arguments.
     *
     *  After the run-time configuration settings are parsed, but before the completion of
     *  application initialization, the metadata files are read from this directory and used to
     *  generate one instance of this class for each metadata file read. The resulting object are
     *  assigned to the corresponding instrument objects (@ref XFloat), and are save to the SQL
     *  database.
     *
     */
    static path metadata_directory_path_;

    /** The SQL table description object.
     *
     *  This SQL table description provides the details needed to map this object to a relational
     *  database table. It utilizes the accessor methods of the class and the details of the SQL
     *  table column to (1) create an SQL INSERT statement to save the object into the database,
     *  (2) populate one or more objects' fields as a result of an SQL SELECT query, or (3) query
     *  the SQL database to access one or more of the values that are stored the the SQL table
     *  corresponding to this object.
     */
    static const util::mysql::SqlTableDescription<XFloatMetadata> sql_table_description_;

private:

    /* Dynamic Data Members: */

    /** The instrument comment(s).
     *
     */
    string comment_;

    /* Dynamic Data Members - Deployment/Launch: */

    /** The name of the instrument deployer(s).
     *
     */
    string deployer_;

    /** The address of the instrument deployer(s).
     *
     */
    string deployer_address_;

    /** The unique identifier of the deployment or launch cruise.
     *
     */
    string deployment_cruiseId_;

    /** The status of the accompanying date/time of the instrument deployment or launch.
     *
     */
    string deployment_datetime_status_;

    /** The deployment or launch platform.
     *
     */
    string deployment_platform_;

    /** The status of the accompanying date/time of the instrument deployment or launch.
     *
     */
    string deployment_position_status_;

    /** The deployment or launch type.
     *
     */
    string deployment_type_;

    /* Dynamic Data Members - Other: */

    /** The instrument manufacturer.
     *
     */
    string manufacturer_;

    /** The path to the metadata file on the local filesystem.
     *
     */
    string local_path_;

    string instrument_type_;

    /* Dynamic Data Members - Principal Investigators: */

    /** The names of the principal investigator(s).
     *
     */
    string principal_investigators_;

    /** The address of the principal investigator(s).
     *
     */
    string principal_investigators_address_;

    /** The originating country of the instrument.
     *
     */
    string principal_investigators_originating_country_;

    /* Dynamic Data Members - WMO Identifiers: */

    /** The WMO instrument type identifier.
     *
     *  @note           This value corresponds to the WMO reference table 1770.
     *
     */
    int wmo_instrument_type_;

    /** The WMO recorder type identifier.
     *
     *  @note           This value corresponds to the WMO reference table 4770.
     *
     */
    int wmo_recorder_type_;

protected:

    /* Protected Member Functions */

    /** List the paths to all metadata files on the local filesystem.
     *
     *  @throw          util::log::MessageException if an error occurs while listing the metadata
     *                  paths.
     *
     *  @return         A vector containing the paths to the metadata files on the local filesystem.
     */
    static vector<path> ListMetadataFiles() {

        vector<path> paths;

        try {
            // List all files in the metadata directory.
            if(!is_directory(metadata_directory_path_))
                EXC_THROW("Invalid metadata path: " + metadata_directory_path_.string());

            vector<path> files(ListFiles(metadata_directory_path_));

            // Copy paths to files matching the regex.
            copy_if(files.begin(), files.end(), back_inserter(paths), [](const path & p) {

                return regex_match(p.string(),
                        regex(".*/[0-9]{4,5}_[0-9]{4,6}\\.meta", regex::extended));
            });

        }CATCH_BLOCK("Failed to get paths to metadata files!")

        return paths;
    }

    /** Read instrument metadata from a file on the local filesystem.
     *
     *  @param[in]      path        The path to the metadata file on the local filesystem.
     *
     *  @throw          util::log::MessageException if the file does not exist or extraction of
     *                  metadata from the file fails.
     */
    void LoadFile(const path& path) {

        try {

            // Check if file exists.
            if(!exists(path))
                EXC_THROW("File does not exist!");

            ldbg() << "Loading instrument metadata from file: " << path.string() << endl;

            // Set local path.
            set_local_path(path.string());

            // Read metadata from stream.
            ifstream ifs(path);
            ifs >> *this;
            ifs.close();

        }CATCH_NOTHROW("Failed to extract metadata from file: " + path.string() + "!")
    }

    /** Load all metadata from the files on the local filesystem.
     *
     *  @throw          util::log::MessageException if an error occurs while loading the metadata from
     *                  the files on the local filesystem.
     *
     *  @return         A vector of shared pointers to the metadata objects constructed from the
     *                  metadata files on the local filesystem.
     */
    static vector<shared_ptr<XFloatMetadata> > LoadMetadataFromFiles() {

        vector<shared_ptr<XFloatMetadata> > md;

        try {

            vector<path> paths(ListMetadataFiles());
            for_each(paths.begin(), paths.end(), [&](const path& path) {

                md.push_back(make_shared<XFloatMetadata>(path));
            });

        }CATCH_BLOCK("Failed to load metadata!")

        return md;
    }

    /** Set the instrument comment(s).
     *
     *  @param[in]  comment     The instrument comment(s).
     */
    void set_comment(const string& comment) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        comment_ = comment;
    }

    /** Set the name of the instrument deployer(s).
     *
     *  @param[in]  deployer    The name of the instrument deployer(s).
     */
    void set_deployer(const string& deployer) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        deployer_ = deployer;
    }

    /** Set the address of the instrument deployer(s).
     *
     *  @param[in]  address    The address of the instrument deployer(s).
     */
    void set_deployer_address(const string& address) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        deployer_address_ = address;
    }

    /** Set the unique identifier of the deployment or launch cruise.
     *
     *  @param[in]  id          The unique identifier of the deployment or launch cruise.
     */
    void set_deployment_cruise_id(const string& id) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        deployment_cruiseId_ = id;
    }

    /** Set the date/time of the deployment.
     *
     *  @param[in]  dt          The date/time of the deployment.
     */
    void set_deployment_datetime(const ptime& dt) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        set_position_datetime(dt);
    }

    /** Set the status of the accompanying date/time of the instrument deployment or launch.
     *
     *  @param[in]  stat        The status of the accompanying date/time of the instrument
     *                          deployment or launch.
     */
    void set_deployment_datetime_status(const string& stat) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        deployment_datetime_status_ = stat;
    }

    /** Set the deployment position latitude, in decimal degrees North.
     *
     *  @param[in]  lat         The deployment position latitude, in decimal degrees North.
     */
    void set_deployment_latitude(double lat) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        set_position_latitude(lat);
    }

    /** Set the deployment position longitude, in decimal degrees East.
     *
     *  @param[in]  lon         The deployment position longitude, in decimal degrees East.
     */
    void set_deployment_longitude(double lon) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        set_position_longitude(lon);
    }

    /** Set the deployment or launch platform.
     *
     *  @param[in]  platform    The deployment or launch platform.
     */
    void set_deployment_platform(const string& platform) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        deployment_platform_ = platform;
    }

    /** Set the status of the accompanying date/time of the instrument deployment or launch.
     *
     *  @param[in]  status      The status of the accompanying date/time of the instrument
     *                          deployment or launch.
     */
    void set_deployment_position_status(const string& status) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        deployment_position_status_ = status;
    }

    void set_deployment_position_valid(bool valid) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        set_position_is_valid(valid);
    }

    /** Set the deployment or launch type.
     *
     *  @param[in]  type        The deployment or launch type.
     */
    void set_deployment_type(const string& type) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        deployment_type_ = type;
    }


    void set_instrument_type(const string& instrument_type) {
      instrument_type_ = instrument_type;
    }

    /** Set the path to the metadata file on the local filesystem.
     *
     *  @param[in]  local_path  The path to the metadata file on the local filesystem.
     */
    void set_local_path(const string& local_path) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        local_path_ = local_path;
    }

    /** Set the instrument manufacturer.
     *
     *  @param[in]  man         The instrument manufacturer.
     */
    void set_manufacturer(const string& man) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        manufacturer_ = man;
    }

    /** Set the names of the principal investigator(s).
     *
     *  @param[in]  pis         The names of the principal investigator(s).
     */
    void set_principal_investigators(const string& pis) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        principal_investigators_ = pis;
    }

    /** Set the address of the principal investigator(s).
     *
     *  @param[in]  addr        The address of the principal investigator(s).
     */
    void set_principal_investigators_address(const string& addr) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        principal_investigators_address_ = addr;
    }

    /** Set the originating country of the instrument.
     *
     *  @param[in]  country     The originating country of the instrument.
     */
    void set_principal_investigators_originating_country(const string& country) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        principal_investigators_originating_country_ = country;
    }

    /** Set the WMO instrument type identifier.
     *
     *  @param[in]  type        The WMO instrument type identifier.
     */
    void set_wmo_instrument_type(int type) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        wmo_instrument_type_ = type;
    }

    /** Set the WMO recorder type identifier.
     *
     *  @param[in]  type        The WMO recorder type identifier.
     */
    void set_wmo_recorder_type(int type) {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        wmo_recorder_type_ = type;
    }

public:

    /* Public Member Functions: */

    /** The default constructor.
     *
     */
    XFloatMetadata() :
                    Iridium(),
                    deployer_("unknown"),
                    deployer_address_("unknown"),
                    deployment_cruiseId_("unknown"),
                    deployment_datetime_status_("unknown"),
                    deployment_platform_("unknown"),
                    deployment_position_status_("unknown"),
                    deployment_type_("unknown"),
                    manufacturer_("unknown"),
                    principal_investigators_("Steven Jayne, Breck Owens, P.E. Robbins"),
                    principal_investigators_address_("WHOI, Woods Hole, MA"),
                    principal_investigators_originating_country_("USA"),
                    wmo_instrument_type_(XFLT_ID_UNKNOWN),
                    wmo_recorder_type_(XFLT_ID_UNKNOWN) {

        /* Do nothing. */
    }

    /** The file constructor.
     *
     *  Reads instrument metadata from a file on the local filesystem.
     *
     *  @param[in]      path        The path to the metadata file on the local filesystem.
     *
     *  @throw          util::log::MessageException if the file does not exist or extraction of
     *                  metadata from the file fails.
     */
    XFloatMetadata(const path& path) :
                    Iridium(),
                    deployer_("unknown"),
                    deployer_address_("unknown"),
                    deployment_cruiseId_("unknown"),
                    deployment_datetime_status_("unknown"),
                    deployment_platform_("unknown"),
                    deployment_position_status_("unknown"),
                    deployment_type_("unknown"),
                    manufacturer_("unknown"),
                    principal_investigators_("Steven Jayne, Breck Owens, P.E. Robbins"),
                    principal_investigators_address_("WHOI, Woods Hole, MA"),
                    principal_investigators_originating_country_("USA"),
                    wmo_instrument_type_(XFLT_ID_UNKNOWN),
                    wmo_recorder_type_(XFLT_ID_UNKNOWN) {
        try {

            // Extract data from the file at path.
            LoadFile(path);

        }CATCH_BLOCK("Failed to construct Metadata object!")
    }

    /** The destructor.
     *
     */
    ~XFloatMetadata() {

        /* Do nothing. */
    }

    /** Get the instrument comment(s).
     *
     *  @return         The instrument comment(s).
     */
    string comment() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return comment_;
    }

    /** Get the name of the instrument deployer(s).
     *
     *  @return         The name of the instrument deployer(s).
     */
    string deployer() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return deployer_;
    }

    /** Get the address of the instrument deployer(s).
     *
     *  @return         The address of the instrument deployer(s).
     */
    string deployer_address() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return deployer_address_;
    }

    /** Get the unique identifier of the deployment or launch cruise.
     *
     *  @return         The unique identifier of the deployment or launch cruise.
     */
    string deployment_cruise_id() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return deployment_cruiseId_;
    }

    /** Get the date/time of the deployment.
     *
     *  @return         The date/time of the deployment.
     */
    ptime deployment_datetime() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return position_datetime();
    }

    /** Get the latitude of the deployment position, in decimal degrees North.
     *
     *  @return         The latitude of the deployment position, in decimal degrees North.
     */
    double deployment_latitude() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return position_latitude();
    }

    /** Get the longitude of the deployment position, in decimal degrees East.
     *
     *  @return         The longitude of the deployment position, in decimal degrees East.
     */
    double deployment_longitude() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return position_longitude();
    }

    /** Get the status of the accompanying date/time of the instrument deployment or launch.
     *
     *  @return         The status of the accompanying date/time of the instrument deployment or
     *                  launch.
     */
    string deployment_datetime_status() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return deployment_datetime_status_;
    }

    /** Get the deployment or launch platform.
     *
     *  @return         The deployment or launch platform.
     */
    string deployment_platform() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return deployment_platform_;
    }

    /** Get the status of the accompanying date/time of the instrument deployment or launch.
     *
     *  @return         The status of the accompanying date/time of the instrument deployment or
     *                  launch.
     */
    string deployment_position_status() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return deployment_position_status_;
    }

    string deployment_position_as_iso6709_string() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return postition_as_iso6709_string();
    }

    /** Get the deployment or launch type.
     *
     *  @return         The deployment or launch type.
     */
    string deployment_type() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return deployment_type_;
    }

    bool is_deployment_position_valid() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return position_is_valid();
    }

    /** Get the path to the metadata file on the local filesystem.
     *
     *  @return         The path to the metadata file on the local filesystem.
     */
    string local_path() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return local_path_;
    }

    /** Get the instrument manufacturer.
     *
     *  @return         The instrument manufacturer.
     */
    string manufacturer() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return manufacturer_;
    }

    /** Get the names of the principal investigator(s).
     *
     *  @return         The names of the principal investigator(s).
     */
    string principal_investigators() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return principal_investigators_;
    }

    /** Get the address of the principal investigator(s).
     *
     *  @return         The address of the principal investigator(s).
     */
    string principal_investigators_address() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return principal_investigators_address_;
    }

    /** Get the originating country of the instrument.
     *
     *  @return         The originating country of the instrument.
     */
    string principal_investigators_originating_country() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return principal_investigators_originating_country_;
    }

    /** Save this object to the database.
     *
     *  @throw      util::log::MessageException if an error occurs while saving this object to the
     *              database.
     *
     */
    void SaveObjectToDatabase() const {

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            ldbg() << "Saving metadata object to database...";

            sql_table_description_.SaveObjectToDatabase(*this);

        }CATCH_BLOCK("Failed to save metadata to database!")
    }

    /** Get the WMO instrument type identifier.
     *
     *  @return         The WMO instrument type identifier.
     */
    int wmo_instrument_type() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return wmo_instrument_type_;
    }

    /** Get the WMO recorder type identifier.
     *
     *  @return         The WMO recorder type identifier.
     */
    int wmo_recorder_type() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return wmo_recorder_type_;
    }

    const string& instrument_type() const {
      return instrument_type_;
    }

};

/* Function Prototypes: */

istream& operator>>(istream& in, XFloatMetadata& metadata);

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */
#endif /* XFLOATMETADATA_H_ */
