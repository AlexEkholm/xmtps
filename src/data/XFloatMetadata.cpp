/** The @ref argo::iridium::xfloat::XFloatMetadata class source file.
 *
 *  This is the source file for the @ref argo::iridium::xfloat::XFloatMetadata class. This class
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
/* Includes: */

// header
#include "XFloatMetadata.h"

namespace argo {
namespace iridium {
namespace xfloat {

/* Static Variables: */

path XFloatMetadata::metadata_directory_path_;

const util::mysql::SqlTableDescription<XFloatMetadata> XFloatMetadata::sql_table_description_(
        "XFloatMetadata",

        {

        sql_coli(XFloatMetadata, "id_dac", &XFloatMetadata::id_dac, &XFloatMetadata::set_id_dac),

        sql_coll(XFloatMetadata, "id_imei", &XFloatMetadata::id_imei, &XFloatMetadata::set_id_imei),

        sql_coli(
                XFloatMetadata,
                "id_instrument",
                &XFloatMetadata::id_instrument,
                &XFloatMetadata::set_id_instrument,
                true),

        sql_coli(XFloatMetadata, "id_wmo", &XFloatMetadata::id_wmo, &XFloatMetadata::set_id_wmo),

        sql_cols(
                XFloatMetadata,
                "deployer",
                &XFloatMetadata::deployer,
                &XFloatMetadata::set_deployer),

        sql_cols(
                XFloatMetadata,
                "deployer_address",
                &XFloatMetadata::deployer_address,
                &XFloatMetadata::set_deployer_address),

        sql_cols(
                XFloatMetadata,
                "deployment_cruise_id",
                &XFloatMetadata::deployment_cruise_id,
                &XFloatMetadata::set_deployment_cruise_id),

        sql_cols(
                XFloatMetadata,
                "deployment_datetime_status",
                &XFloatMetadata::deployment_datetime_status,
                &XFloatMetadata::set_deployment_datetime_status),

        sql_colt(
                XFloatMetadata,
                "deployment_datetime",
                &XFloatMetadata::deployment_datetime,
                &XFloatMetadata::set_deployment_datetime),

        sql_cold(
                XFloatMetadata,
                "deployment_latitude",
                &XFloatMetadata::deployment_latitude,
                &XFloatMetadata::set_deployment_latitude),

        sql_cold(
                XFloatMetadata,
                "deployment_longitude",
                &XFloatMetadata::deployment_longitude,
                &XFloatMetadata::set_deployment_longitude),

        sql_cols(
                XFloatMetadata,
                "deployment_platform",
                &XFloatMetadata::deployment_platform,
                &XFloatMetadata::set_deployment_platform),

        sql_cols(
                XFloatMetadata,
                "deployment_position_status",
                &XFloatMetadata::deployment_position_status,
                &XFloatMetadata::set_deployment_position_status),

        sql_colb(
                XFloatMetadata,
                "deployment_position_valid",
                &XFloatMetadata::is_deployment_position_valid,
                &XFloatMetadata::set_deployment_position_valid),

        sql_cols(
                XFloatMetadata,
                "deployment_type",
                &XFloatMetadata::deployment_type,
                &XFloatMetadata::set_deployment_type),

        sql_cols(
                XFloatMetadata,
                "manufacturer",
                &XFloatMetadata::manufacturer,
                &XFloatMetadata::set_manufacturer),

        sql_cols(XFloatMetadata, "comment", &XFloatMetadata::comment, &XFloatMetadata::set_comment),

        sql_cols(
                XFloatMetadata,
                "local_path",
                &XFloatMetadata::local_path,
                &XFloatMetadata::set_local_path),

        sql_cols(
                XFloatMetadata,
                "principal_investigators",
                &XFloatMetadata::principal_investigators,
                &XFloatMetadata::set_principal_investigators),

        sql_cols(
                XFloatMetadata,
                "principal_investigators_address",
                &XFloatMetadata::principal_investigators_address,
                &XFloatMetadata::set_principal_investigators_address),

        sql_cols(
                XFloatMetadata,
                "principal_investigators_originating_country",
                &XFloatMetadata::principal_investigators_originating_country,
                &XFloatMetadata::set_principal_investigators_originating_country),

        sql_coli(
                XFloatMetadata,
                "wmo_instrument_type",
                &XFloatMetadata::wmo_instrument_type,
                &XFloatMetadata::set_wmo_instrument_type),

        sql_coli(
                XFloatMetadata,
                "wmo_recorder_type",
                &XFloatMetadata::wmo_recorder_type,
                &XFloatMetadata::set_wmo_recorder_type)

        });

/* Function Definitions: */

/** Stream extraction operator overload function for the @ref XFloatMetadata class.
 *
 *  @param[in]      in          The input stream.
 *  @param[in]      metadata    The metadata object.
 *
 *  @throw          util::log::MessageException if the stream extraction fails.
 *
 *  @return         The reference to the input stream, @p istream.
 */
istream& operator>>(istream& in, XFloatMetadata& metadata) {

    string line;                    // The input stream line buffer.
    vector<string> unknown_fields;  // List of unsupported metadata fields.


    try {

        // Iterate over the input stream.
        while(getline(in, line)) {

            // Ignore lines less that 40 characters.
            if(line.length() < 40)
                continue;

            // Get field and value strings.
            string field = line.substr(0, 40), val = line.substr(40);
            trim(field);
            trim(val);

            ldbg() << field << ": " << val;



            // Ignore empty value strings.
            if(val.empty())
                continue;

            // Parse the value, according to the field string.
            try {

                /** @todo   Implement the functionality for parsing additional metadata field from
                 *          stream
                 *
                 */

                if(regex_match(field, regex("internal ID number", regex::extended))) {

                    try {

                        metadata.set_id_dac(lexical_cast<int>(val));

                    }CATCH_BLOCK("Failed to parse DAC ID!")

                } else if(regex_match(field, regex("WMO ID number", regex::extended))) {

                    try {

                        metadata.set_id_wmo(lexical_cast<int>(val));

                    }CATCH_BLOCK("Failed to parse WMO ID!")

                } else if(regex_match(field, regex("WMO instrument type.*", regex::extended))) {

                    try {

                        metadata.set_wmo_instrument_type(lexical_cast<int>(val));

                    }CATCH_BLOCK("Failed to parse WMO instrument type!")

                } else if(regex_match(field, regex("WMO recorder type.*", regex::extended))) {

                    try {

                        metadata.set_wmo_recorder_type(lexical_cast<int>(val));

                    }CATCH_BLOCK("Failed to parse WMO recorder type!")

                } else if(regex_match(field, regex("launch time.*", regex::extended))) {

                    try {

                        // Parse deployment date/time [dd mm yyyy hh mm].
                        cmatch cm;
                        if(regex_match(
                                val.c_str(),
                                cm,
                                regex("(\\d+) (\\d+) (\\d+) (\\d+) (\\d+)", regex::extended))) {

                            int yr = lexical_cast<int>(cm[3].str());
                            int mo = lexical_cast<int>(cm[2].str());
                            int dy = lexical_cast<int>(cm[1].str());
                            int hr = lexical_cast<int>(cm[4].str());
                            int mn = lexical_cast<int>(cm[5].str());

                            // Check the date/time values.
                            if(yr > 2000 && yr < 2100 && mo > 0 && mo < 13 && dy > 0 && dy < 32
                                    && hr >= 0 && hr < 24 && mn >= 0 && mn < 60) {

                                // Date/time values are valid.
                                metadata.set_deployment_datetime(
                                        ptime(date(yr, mo, dy), time_duration(hr, mn, 0)));

                            } else {

                                // Date/time values are NOT valid.
                                metadata.set_deployment_datetime(not_a_date_time);

                            }

                        } else {

                            // Failed to match regex!
                            lwrn() << "Failed to match instrument deployment date/time value "
                                      << "string: "
                                      << val;
                            metadata.set_deployment_datetime(not_a_date_time);
                        }

                    }CATCH_BLOCK("Failed to match instrument deployment date/time value "
                            "string: " + val)

                } else if(regex_match(field, regex("status of launch time", regex::extended))) {

                    try {

                        metadata.set_deployment_datetime_status(val);

                    }CATCH_BLOCK("Failed to parse deployment date/time status!")

                } else if(regex_match(field, regex("launch position.*", regex::extended))) {

                    try {

                        // Parse deployment position [lat latm lon lonm].
                        cmatch cm;
                        if(regex_match(
                                val.c_str(),
                                cm,
                                regex(
                                        "([+-]?\\d*\\.?\\d*) ([+-]?\\d*\\.?\\d*) "
                                                "([+-]?\\d*\\.?\\d*) ([+-]?\\d*\\.?\\d*)",
                                        regex::extended))) {

                            double lat = lexical_cast<double>(cm[1].str())
                                    + lexical_cast<double>(cm[2].str()) / 60.;
                            double lon = lexical_cast<double>(cm[3].str())
                                    + lexical_cast<double>(cm[4].str()) / 60.;

                            // Check if the deployment position is valid.
                            if(util::position::PositionUtils::IsPositionValid(lat, lon)) {
                                metadata.set_deployment_latitude(lat);
                                metadata.set_deployment_longitude(lon);
                                metadata.set_position_is_valid(true);
                            } else {
                                metadata.set_deployment_latitude(UTIL_POS_INVALID_LAT);
                                metadata.set_deployment_longitude(UTIL_POS_INVALID_LON);
                                metadata.set_position_is_valid(false);
                            }

                        } else {

                            // Failed to match regex!
                            lwrn() << "Failed to match instrument deployment location value "
                                      << "string: "
                                      << val;
                            metadata.set_deployment_latitude(XFLT_POS_INVALID_LAT);
                            metadata.set_deployment_longitude(XFLT_POS_INVALID_LON);
                        }

                    }CATCH_BLOCK("Failed to extract instrument deployment location value "
                            "string: " + val)

                } else if(regex_match(field, regex("status of launch position", regex::extended))) {

                    metadata.set_deployment_position_status(val);

                } else if(regex_match(field, regex("float manufacturer", regex::extended))) {

                    metadata.set_manufacturer(val);

                } else if(regex_match(field, regex("float serial number", regex::extended))) {

                    try {

                        metadata.set_id_instrument(lexical_cast<int>(val));

                    }CATCH_BLOCK("Failed to parse float serial number!")

                } else if(regex_match(field, regex("PI", regex::extended))) {

                    metadata.set_principal_investigators(val);

                } else if(regex_match(
                        field,
                        regex("principal investigator address", regex::extended))) {

                    metadata.set_principal_investigators_address(val);

                } else if(regex_match(field, regex("originating country", regex::extended))) {

                    metadata.set_principal_investigators_originating_country(val);

                } else if(regex_match(field, regex("float deployer", regex::extended))) {

                    metadata.set_deployer(val);

                } else if(regex_match(field, regex("float deployer address", regex::extended))) {

                    metadata.set_deployer_address(val);

                } else if(regex_match(field, regex("deployment type", regex::extended))) {

                    metadata.set_deployment_type(val);

                } else if(regex_match(field, regex("deployment platform", regex::extended))) {

                    metadata.set_deployment_platform(val);

                } else if(regex_match(field, regex("deployment cruise id", regex::extended))) {

                    metadata.set_deployment_cruise_id(val);

                } else if(regex_match(field, regex("comment", regex::extended))) {

                    metadata.set_comment(val);

                } else if(regex_match(field, regex("instrument type", regex::extended))) {

                    metadata.set_instrument_type(val);

                } else {

                    ostringstream os;

                    os << "\"" << field << " = " << val << "\"";

                    unknown_fields.push_back(os.str());

                }

            }CATCH_BLOCK("Failed to extract metadata from field: '" + field + "' = '" + val + "'")

        }

        if(!unknown_fields.empty()) {

            ostringstream os;
            os << "Encountered " << unknown_fields.size() << " unsupported metadata fields:";

            for_each(unknown_fields.begin(), unknown_fields.end(), [&](const string& field) {

                os << " " << field;

            });

            lwrn() << os.str();

        }

    }CATCH_BLOCK("Failed to extract metadata from input stream!")

    return in;
}

}
}
}

