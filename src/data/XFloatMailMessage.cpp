/** The @ref argo::iridium::xfloat::XFloatMailMessage class source file.
 *
 *  This is the header file for the @ref argo::iridium::xfloat::XFloatMailMessage class. This
 *  class represents a single mail message received from the Iridium system, including fields
 *  that are parsed from the various parts of the mail message and the attached Iridium Short
 *  Burst Data (SBD) file. It is used to parse, validate, and write the mail message files to the
 *  local filesystem.
 *
 *  This file contains the static data members of the class and some of the class's functionality.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:31:19 PM
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

/* Includes */

// header
#include "XFloatMailMessage.h"

namespace argo {
namespace iridium {
namespace xfloat {

/* Static Variables */

string XFloatMailMessage::msg_header_valid_sender_regex_(XMAIL_DFLT_MSG_HEADER_VALID_SNDR_REGEX);

string XFloatMailMessage::msg_header_valid_subject_regex_(XMAIL_DFLT_MSG_HEADER_VALID_SUBJ_REGEX);

int XFloatMailMessage::raw_file_format_(XMAIL_DFLT_RAW_FILE_FORMAT);

bool XFloatMailMessage::raw_file_generation_is_enabled_(XMAIL_DFLT_RAW_FILE_GENERATRION_ENABLE);

path XFloatMailMessage::raw_directory_path_;

bool XFloatMailMessage::sbd_file_generation_is_enabled_(XMAIL_DFLT_SBD_FILE_GENERATRION_ENABLE);

string XFloatMailMessage::sbd_file_valid_name_regex_(XMAIL_DFLT_SBD_FILE_VALID_NAME_REGEX);

path XFloatMailMessage::sbd_directory_path_;

const util::mysql::SqlTableDescription<XFloatMailMessage> XFloatMailMessage::sql_table_description_(

        "XFloatMail",

        {

        sql_coli(
                XFloatMailMessage,
                "id_dive",
                &XFloatMailMessage::id_dive,
                &XFloatMailMessage::set_id_dive,
                true),


        sql_coll(
                XFloatMailMessage,
                "id_imei",
                &XFloatMailMessage::id_imei,
                &XFloatMailMessage::set_id_imei,
                true),

        sql_coli(
                XFloatMailMessage,
                "id_sbd_momsn",
                &XFloatMailMessage::id_sbd_momsn,
                &XFloatMailMessage::set_id_sbd_momsn,
                true),

        sql_coli(
                XFloatMailMessage,
                "id_sbd_packet",
                &XFloatMailMessage::id_sbd_packet,
                &XFloatMailMessage::set_id_sbd_packet,
                true),

        sql_cols(
                XFloatMailMessage,
                "id_msg",
                &XFloatMailMessage::id_msg,
                &XFloatMailMessage::set_id_msg,
                true),

        sql_coli(
                XFloatMailMessage,
                "id_instrument",
                &XFloatMailMessage::id_instrument,
                &XFloatMailMessage::set_id_instrument),

        sql_coll(
                XFloatMailMessage,
                "id_msg_uid",
                &XFloatMailMessage::id_msg_uid,
                &XFloatMailMessage::set_id_msg_uid),

        /* The mail body text and raw header text are not stored: they are only used while the mail
         * is parsed, and both are part of msg_raw_text. */

        sql_colb(
                XFloatMailMessage,
                "msg_body_is_valid",
                &XFloatMailMessage::msg_body_is_valid,
                &XFloatMailMessage::set_msg_body_is_valid),

        sql_colt(
                XFloatMailMessage,
                "msg_header_datetime",
                &XFloatMailMessage::msg_header_datetime,
                &XFloatMailMessage::set_msg_header_datetime),

        sql_cols(
                XFloatMailMessage,
                "msg_header_from",
                &XFloatMailMessage::msg_header_from,
                &XFloatMailMessage::set_msg_header_from),

        sql_cols(
                XFloatMailMessage,
                "msg_header_subject",
                &XFloatMailMessage::msg_header_subject,
                &XFloatMailMessage::set_msg_header_subject),

        sql_colb(
                XFloatMailMessage,
                "msg_header_is_valid",
                &XFloatMailMessage::msg_header_is_valid,
                &XFloatMailMessage::set_msg_header_is_valid),

        sql_cols(
                XFloatMailMessage,
                "msg_raw_text",
                &XFloatMailMessage::msg_raw_text,
                &XFloatMailMessage::set_msg_raw_text),

        sql_coli(
                XFloatMailMessage,
                "iridium_position_cep_radius",
                &XFloatMailMessage::iridium_position_cep_radius,
                &XFloatMailMessage::set_iridium_position_cep_radius),

        sql_colt(
                XFloatMailMessage,
                "iridium_position_datetime",
                &XFloatMailMessage::iridium_position_datetime,
                &XFloatMailMessage::set_iridium_position_datetime),

        sql_cold(
                XFloatMailMessage,
                "iridium_position_latitude",
                &XFloatMailMessage::iridium_position_latitude,
                &XFloatMailMessage::set_iridium_position_latitude),

        sql_cold(
                XFloatMailMessage,
                "iridium_position_longitude",
                &XFloatMailMessage::iridium_position_longitude,
                &XFloatMailMessage::set_iridium_position_longitude),

        sql_colb(
                XFloatMailMessage,
                "iridium_position_is_valid",
                &XFloatMailMessage::iridium_position_is_valid,
                &XFloatMailMessage::set_iridium_position_is_valid),

        sql_colv(
                XFloatMailMessage,
                "sbd_data",
                &XFloatMailMessage::sbd_data,
                &XFloatMailMessage::set_sbd_data),

        sql_cols(
                XFloatMailMessage,
                "sbd_file_name",
                &XFloatMailMessage::sbd_file_name,
                &XFloatMailMessage::set_sbd_file_name),

        sql_colp(
                XFloatMailMessage,
                "sbd_file_local_path",
                &XFloatMailMessage::sbd_file_local_path,
                &XFloatMailMessage::set_sbd_file_local_path),

        sql_coli(
                XFloatMailMessage,
                "id_sbd_mtmsn",
                &XFloatMailMessage::id_sbd_mtmsn,
                &XFloatMailMessage::set_id_sbd_mtmsn),

        sql_coli(
                XFloatMailMessage,
                "sbd_file_size_bytes",
                &XFloatMailMessage::sbd_file_size_bytes,
                &XFloatMailMessage::set_sbd_file_size_bytes),

        sql_coli(
                XFloatMailMessage,
                "sbd_session_status_code",
                &XFloatMailMessage::sbd_session_status_code,
                &XFloatMailMessage::set_sbd_session_status_code),

        sql_cols(
                XFloatMailMessage,
                "sbd_session_status_string",
                &XFloatMailMessage::sbd_session_status_string,
                &XFloatMailMessage::set_sbd_session_status_string),

        sql_colb(
                XFloatMailMessage,
                "sbd_data_is_valid",
                &XFloatMailMessage::sbd_data_is_valid,
                &XFloatMailMessage::set_sbd_data_is_valid),

        sql_colp(
                XFloatMailMessage,
                "raw_file_local_path",
                &XFloatMailMessage::raw_file_local_path,
                &XFloatMailMessage::set_raw_file_local_path),

        sql_colb(
                XFloatMailMessage,
                "msg_is_valid",
                &XFloatMailMessage::msg_is_valid,
                &XFloatMailMessage::set_msg_is_valid)

        });

/* Function Definitions */


/** Generate a list of ordered Iridium message PIDs for this dive.
 *
 *  @exception      util::log::MessageException if an error occurs.
 *
 *  @return         A vector of PIDs, sorted in non-decreasing order.
 */
vector<int> XFloatDiveData::GenerateOrderedPidList() const {
    vector<int> pids;
    vector<int>::iterator pids_itr;

    try {
        // Sorted emplace.
        for_each(
                mail_messages_.begin(),
                mail_messages_.end(),
                [&](const shared_ptr<XFloatMailMessage>& msg) {

                    for(pids_itr = pids.begin(); pids_itr < pids.end(); pids_itr++)
                    if(msg->id_sbd_packet() < *pids_itr)
                    break;
                    pids.emplace(pids_itr, msg->id_sbd_packet());
                });
    }CATCH_BLOCK("Failed to get PIDs from Iridium messages!")

    return pids;
}

/** Generate a list of ordered Iridium message MOMSNs for this dive.
 *
 *  @throw          util::log::MessageException if an error occurs.
 *
 *  @return         A vector of MOMSNs, sorted in non-decreasing order.
 */
vector<int> XFloatDiveData::GenerateOrderedMomsnList() const {

    vector<int> momsns;
    vector<int>::iterator momsns_itr;

    try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        for_each(
                mail_messages_.begin(),
                mail_messages_.end(),
                [&](const shared_ptr<XFloatMailMessage>& msg) {

                    // Sorted emplace.
                    for(momsns_itr = momsns.begin(); momsns_itr < momsns.end(); momsns_itr++)
                    if(msg->id_sbd_momsn() < *momsns_itr)
                    break;
                    momsns.emplace(momsns_itr, msg->id_sbd_momsn());

                });

    }CATCH_BLOCK("Failed to get MOMSNs from Iridium messages!")

    return momsns;
}

/** Assign a mail message to this dive data object.
 *
 *  @exception  util::log::MessageException if !x or an error occurs.
 *
 *  @note       This vector is sorted upon insertion by order of non-decreasing message indices.
 *
 *  @param[in]  x       The mail message to assign.
 *
 *  @return     0 on successful assignment of the mail message to this dive data object, 1 if
 *              a duplicate mail message UID is encountered, or 2 if the mail message was assigned
 *              but is a retransmission of an already assigned message (identical SBD data), so
 *              its SBD data should not be decoded again.
 */
int XFloatDiveData::AssignMailMessage(const shared_ptr<XFloatMailMessage>& x) {

    int status = 0;

    try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        // Check the message pointer.
        if(!x)
            EXC_THROW("NULL pointer to mail message object.");

        const vector<uint8_t> sbd_data(x->sbd_data());

        // Iterate to emplacement position.
        auto it = mail_messages_.begin();
        for(auto dup = mail_messages_.begin(); dup < mail_messages_.end(); ++dup)

            // Check for duplicate message IDs.
            if(x->id_msg() == (*dup)->id_msg()) {

                ldbg() << "Ignoring mail message with duplicate ID: " << x->id_msg();
                return 1;

            /* Floats sometimes resend a packet unchanged under a new MOMSN. The copy is still
             * assigned (it carries its own Iridium position), but decoding its SBD data again
             * would fail with duplicate X-Message indices. */
            } else if(!status && !sbd_data.empty() && sbd_data == (*dup)->sbd_data()) {

                ldbg() << "Mail message is a retransmission (MOMSN " << x->id_sbd_momsn()
                       << ", identical SBD data to MOMSN " << (*dup)->id_sbd_momsn() << ").";
                status = 2;

            }

        for(; it < mail_messages_.end(); ++it)
            if(x->id_sbd_momsn() < (*it)->id_sbd_momsn())

                // Emplacement position found.
                break;

        // Emplace element at position.
        mail_messages_.emplace(it, x);

        // Increment the number of SBD message for this dive cycle.
        num_sbd_msgs_++;

        // Update the first & last iridium position date/times.
        if(iridium_position_datetime_first().is_not_a_date_time()
                || x->iridium_position_datetime() < iridium_position_datetime_first())
            set_iridium_position_datetime_first(x->iridium_position_datetime());

        if(iridium_position_datetime_last().is_not_a_date_time()
                || x->iridium_position_datetime() > iridium_position_datetime_last())
            set_iridium_position_datetime_last(x->iridium_position_datetime());

    }CATCH_BLOCK("Failed to assign mail message to dive data object!")

    return status;
}


/** The stream insertion operator for the SBD data type.
 *
 *  @exception      util::log::MessageException of an error occurs.
 *
 *  @param[in]      os          The ostream object.
 *  @param[in]      data        The SBD data.
 *
 *  @return         The ostream object, @p os.
 */
ostream& operator<<(ostream& os, const XMailSbdData_t& data) {
    try {
        os << uppercase << setfill('0');
        for(size_t i = 0; i < data.size(); ++i) {
            if(i % 32 == 0)
                os << endl << "(" << dec << setw(3) << i << ") 0x ";
            else if(i % 4 == 0)
                os << " ";
            os << hex << setw(2) << (int) data.at(i);
        }
    }CATCH_BLOCK("Failed to generate RAW mail message string!")

    return os;
}

/** The stream insertion operator overload function for @ref XFloatMailMessage.
 *
 *  Generates and inserts a RAW mail string representation of the mail message.
 *
 *  @exception      util::log::MessageException of an error occurs.
 *
 *  @param[in]      os          The ostream object.
 *  @param[in]      mail        The mail message object.
 *
 *  @return         The ostream object, @p os.
 */
ostream& operator<<(ostream& os, const XFloatMailMessage& mail) {
    using util::date::DateUtils;

    try {

        os << util::config::LocalConfiguration::splash_lis();

        switch(XFloatMailMessage::raw_file_format_) {
        case XMAIL_RAW_FORMAT_STANDARD:

            XMSG_HEADER2("Processing Details");
            os << "#\n# WHOI:            "
               << mail.id_instrument()
               << "\n# IMEI:            "
               << mail.id_imei()
               << "\n# Dive:            "
               << mail.id_dive()
               << "\n# Message UID:     "
               << mail.id_msg_uid()
               << "\n# Message ID:      "
               << mail.id_msg()
               << "\n# Processed:       "
               << DateUtils::GenerateCurrentUtcTimeIso8601String()
               << "\n# Valid:           "
               << (mail.msg_is_valid() ? "yes" : "no")
               << "\n#\n";
            XMSG_HEADER2("Start of Message");
            os << "\n" << mail.msg_raw_text();

            break;

        case XMAIL_RAW_FORMAT_EXTENDED:

            XMSG_HEADER2("Processing Details");
            os << "\nWHOI:            "
               << mail.id_instrument()
               << "\nIMEI:            "
               << mail.id_imei()
               << "\nDive:            "
               << mail.id_dive()
               << "\nMessage UID:     "
               << mail.id_msg_uid()
               << "\nMessage ID:      "
               << mail.id_msg()
               << "\nProcessed:       "
               << DateUtils::GenerateCurrentUtcTimeIso8601String()
               << "\nValid:           "
               << (mail.msg_is_valid() ? "yes" : "no")
               << "\n\n";
            XMSG_HEADER2("Message Header");
            os << "\nFrom:            "
               << mail.msg_header_from()
               << "\nSubject:         "
               << mail.msg_header_subject()
               << "\nDate:            "
               << DateUtils::GenerateIso8601String(mail.msg_header_datetime())
               << "\n\n";

            XMSG_HEADER2("Iridium Session Details");
            os << "\nMOMSN:           "
               << mail.id_sbd_momsn()
               << "\nMTMSN:           "
               << mail.id_sbd_mtmsn()
               << "\nTime of Session: "
               << mail.iridium_position_datetime_as_iso8601_string()
               << "\nSession Status:  "
               << mail.sbd_session_status_code()
               << " - "
               << mail.sbd_session_status_string()
               << "\nMessage Size:    "
               << mail.sbd_file_size_bytes()
               << "\nUnit Location:   "
               << mail.iridium_position_as_iso6709_string()
               << "\nCEPradius:       "
               << mail.iridium_position_cep_radius()
               << "\nPosition Valid:  "
               << (mail.iridium_position_is_valid() ? "yes" : "no")
               << "\n\n";

            if(!mail.sbd_data().empty()) {
                XMSG_HEADER2("Iridium Short-Burst Data (SBD) Message");
                os << "\nFilename:        "
                   << mail.sbd_file_name()
                   << "\nLocal Path:      "
                   << mail.sbd_file_local_path()
                   << "\nHex:             "
                   << "\n"
                   << mail.sbd_data()
                   << "\n";
            }

            break;

        default:

            break;
        }

    }CATCH_BLOCK("Failed to generate RAW mail message string!")

    return os;
}


/** The stream extraction operator overload function for @ref XFloatMailMessage.
 *
 *  Parses mail message from istream.
 *
 *  @exception      util::log::MessageException of an error occurs.
 *
 *  @param[in]      is          The istream object.
 *  @param[in]      mail        The mail message object.
 *
 *  @return         The istream object, @p is.
 */
istream& operator>>(istream& is, XFloatMailMessage& mail) {

  XFloatMailRawFileFormat_t mode = XMAIL_RAW_FORMAT_STANDARD;

  string line;
  string msg_text;
  stringstream ss;

  const vmime::shared_ptr<vmime::message> msg = vmime::make_shared<vmime::message>();


  try {

    // Ignore comment lines and blank lines at start of file.
    while(getline(is, line) && regex_match(line, regex("^#.*|^$"))) ;


    // TODO Determine if mail message is extended format.

    // Read file line by line.
    do {
      ss << line << std::endl;
    } while(getline(is, line));


    msg_text = ss.str();
#   if defined(XMTPS_DEBUG_FETCH_MAIL) && XMTPS_DEBUG_FETCH_MAIL
    ldbg() << "Message text:" << std::endl << msg_text << std::endl;
#   endif // defined(XMTPS_DEBUG_FETCH_MAIL) && XMTPS_DEBUG_FETCH_MAIL
    msg->parse(msg_text);

    mail.ParseMailMsg(msg);

  } CATCH_BLOCK("Failed to parse mail message from istream.");

  return is;
}



/** Write the RAW mail message file.
 *
 *  This function is used to write the raw mail message file to the local filesystem. If the
 *  global RAW-file-generation-is-enabled flag, @ref raw_file_generation_is_enabled_, is FALSE,
 *  then this function will have no effect. Otherwise, the function will generate the path to the
 *  RAW mail file on the local filesystem and write the file to that path. If the RAW mail message
 *  file already exists, then it is left unchanged and is not rewritten.
 *
 *  The path to the RAW mail message file will be constructed in the format
 *  [RAW-DIR]/[WWWW]/raw[WWWW]-[MMMMM]-[DDDD]-[PPP].txt, where
 *      - [RAW-DIR] is the RAW mail directory path specified, by @ref raw_directory_path_,
 *      - [WWWW] is the four-digit unique instrument serial number,
 *      - [MMMMM] is the five-digit MOMSN number,
 *      - [DDDD] is the four-digit dive or cycle number, and
 *      - [PPP] is the three-digit SBD packet number.
 *
 *  If the mail message does not contain valid SBD data, then the path is
 *  [RAW-DIR]/[WWWW]/raw[WWWW]-[MMMMM]-incomplete-[DATETIME].txt, where [DATETIME] is the mail
 *  header date/time in ISO 8601 basic format.
 *
 *  @throw          log::util::MessageException if an error occurs while writing the file.
 */
void XFloatMailMessage::WriteRawFile() {
    try {
        if(!raw_file_generation_is_enabled_)
            return;

        {   // Get mail directory path.
            ostringstream os;
            static const format fmt("%s/%04d");
            os << format(fmt) % raw_directory_path_.string() % id_instrument();
            GetDirectoryPathFromString(os.str(), true);
        }

        {   // Set RAW local path.
            ostringstream os;
            if(sbd_data_is_valid()) { // Local path for good messages.
                static const format fmt("%04d/raw%04d-%05d-%04d-%03d.txt");
                os << format(fmt) % id_instrument() % id_instrument() % id_sbd_momsn()
                    % id_dive() % id_sbd_packet();
            } else { // Local path for incomplete messages.
                static const format fmt("%04d/raw%04d-%05d-incomplete-%s.txt");
                os << format(fmt) % id_instrument() % id_instrument() % id_sbd_momsn()
                    % to_iso_string(msg_header_datetime());
            }
            set_raw_file_local_path(os.str());
        }

        // Check if the file already exists.
        path raw_file_path(raw_directory_path_.string() + "/" + raw_file_local_path().string());
        if(!bfs::exists(raw_file_path)) {

            // Write raw mail message file.
            linf() << "Writing RAW file: " << raw_file_path << "...";
            ofstream raw_file_stream(raw_file_path);
            raw_file_stream << *this;
            raw_file_stream.flush();
            raw_file_stream.close();

        }

    }CATCH_BLOCK("Failed to write RAW mail message file!")
}

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */

namespace util {
namespace xmsg {

/** Compute the weighted average position from the Iridium positions for this dive.
 *
 *  @note           Each Iridium position is assigned a weight of CEP^-2 when computing the average
 *                  postion, where CEP is the circular error probable radius reported with each
 *                  Iridium postion.
 *
 *  @throw          log::MessageException if an error occurs while computing the average position.
 */
void XCompositeIridiumPosition::ComputeAverageIrdiumPosition() {
    using argo::iridium::xfloat::XFloatMailMessage;

    try {
        switch(num_elements()) {
        case 0:    // There are no Iridium positions.
            set_position_datetime(not_a_date_time);
            set_position_latitude(XMSG_POS_INVALID_LAT);
            set_position_longitude(XMSG_POS_INVALID_LON);
            set_position_standard_error(XMSG_POS_INVALID_STDERR);
            set_position_variance(XMSG_POS_INVALID_VAR);
            set_position_is_valid(false);
            set_total_satellite_comm_time_seconds(0);
            break;
        case 1:    // There is exactly one Iridium position.
            set_position_datetime(elements().at(0)->iridium_position_datetime());
            set_position_latitude(elements().at(0)->iridium_position_latitude());
            set_position_longitude(elements().at(0)->iridium_position_longitude());
            set_position_standard_error(elements().at(0)->iridium_position_cep_radius());
            set_position_variance(pow((double) elements().at(0)->iridium_position_cep_radius(), 2));
            set_position_is_valid(elements().at(0)->iridium_position_is_valid());
            set_total_satellite_comm_time_seconds(0);
            break;
        default: {    // There are two or more Iridium positions.
            ptime start(not_a_date_time), end(not_a_date_time);
            {    // Iterate over Iridium positions and compute weighted average position.
                double lat_sum(0), lon_sum(0), weight(0), weight_sum(0);

                /* Longitudes are averaged as offsets from the first position's longitude, wrapped
                 * to [-180, 180), so positions either side of the antimeridian average correctly. */
                double lon_ref(0);
                bool lon_ref_is_set = false;
                auto wrap_lon = [](double lon) {
                    lon = fmod(lon + 180., 360.);
                    return (lon < 0 ? lon + 360. : lon) - 180.;
                };

                for_each(
                        elements().begin(),
                        elements().end(),
                        [&](const shared_ptr<XFloatMailMessage>& msg) {
                            if(msg) {
                                if(!lon_ref_is_set) {
                                    lon_ref = msg->iridium_position_longitude();
                                    lon_ref_is_set = true;
                                }
                                weight = pow((double) msg->iridium_position_cep_radius(), -2.);
                                weight_sum += weight;
                                lat_sum += weight * msg->iridium_position_latitude();
                                lon_sum += weight * wrap_lon(msg->iridium_position_longitude() - lon_ref);
                                if(start.is_not_a_date_time()
                                    || msg->iridium_position_datetime() < start)
                                    start = msg->iridium_position_datetime();
                                if(end.is_not_a_date_time()
                                    || msg->iridium_position_datetime() > end)
                                    end = msg->iridium_position_datetime();
                            }
                        });

                // Set position and date/time.
                set_position_latitude(lat_sum / weight_sum);
                set_position_longitude(wrap_lon(lon_ref + lon_sum / weight_sum));
                set_total_satellite_comm_time_seconds(time_duration(end - start).total_seconds());
            }
            {    // Iterate over Iridium positions and compute weighted average position
                 // variance & standard error.
                double delta_time_sum(0), variance_sum(0), weight(0), weight_sum(0);

                for_each(
                        elements().begin(),
                        elements().end(),
                        [&](const shared_ptr<XFloatMailMessage>& msg) {
                            if(msg) {
                                weight = pow((double) msg->iridium_position_cep_radius(), -2.);
                                weight_sum += weight;
                                delta_time_sum += weight
                                * time_duration(msg->iridium_position_datetime() - start)
                                .total_seconds();
                                variance_sum += weight
                                * sqrt(
                                        pow(
                                                CalculateGeographicDistance(
                                                        msg->iridium_position_latitude(),
                                                        msg->iridium_position_longitude(),
                                                        position_latitude(),
                                                        position_longitude()),
                                                2)
                                        + pow((double) msg->iridium_position_cep_radius(), 2));
                            }
                        });

                // Set position variance.
                set_position_variance(variance_sum / weight_sum);
                set_position_standard_error(sqrt(position_variance()) / sqrt(num_elements() - 1));
                set_position_datetime(start + seconds((long)(delta_time_sum / weight_sum)));
            }
        }
            break;

        }
    }CATCH_BLOCK("Failed to compute average Iridium position!");
}

} /* namespace xmsg */
} /* namespace util */
