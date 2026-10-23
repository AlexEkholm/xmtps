/** The @ref argo::iridium::xfloat::XFloatMailMessage class header file.
 *
 *  This is the header file for the @ref argo::iridium::xfloat::XFloatMailMessage class. This
 *  class represents a single mail message received from the Iridium system, including fields
 *  that are parsed from the various parts of the mail message and the attached Iridium Short
 *  Burst Data (SBD) file. It is used to parse, validate, and write the mail message files to the
 *  local filesystem.
 *
 *  This file contains the class definition and the majority of the functionality of the class.
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

#ifndef XFLOATMAILMESSAGE_H_
#define XFLOATMAILMESSAGE_H_

/* Includes: */

// local
#include "XFloatDiveData.h"

/* Definitions: */

/** The default mail message header valid sender regular expression value.
 *
 *  The mail message header valid sender regular expression value, @ref
 *  argo::iridium::xfloat::XFloatMailMessage::msg_header_valid_sender_regex_, is set as a result
 *  of parsing the command-line arguments and/or configuration file options during application
 *  initialization. If the option is not explicitly specified, then it will default to this value.
 */
#define XMAIL_DFLT_MSG_HEADER_VALID_SNDR_REGEX \
    "(sbdservice@sbd.iridium.com|service@sbd.pac.disa.mil)"

/** The default mail message header valid subject regular expression value.
 *
 *  The mail message header valid subject regular expression value, @ref
 *  argo::iridium::xfloat::XFloatMailMessage::msg_header_valid_subject_regex_, is set as a result
 *  of parsing the command-line arguments and/or configuration file options during application
 *  initialization. If the option is not explicitly specified, then it will default to this value.
 */
#define XMAIL_DFLT_MSG_HEADER_VALID_SUBJ_REGEX  "SBD Msg From Unit: (\\d+)"

/** The default mail message binary SBD file attachment file name valid regular expression.
 *
 *  The mail message binary SBD file attachment file name valid regular expression, @ref
 *  argo::iridium::xfloat::XFloatMailMessage::sbd_file_valid_name_regex_, is set as a result
 *  of parsing the command-line arguments and/or configuration file options during application
 *  initialization. If the option is not explicitly specified, then it will default to this value.
 */
#define XMAIL_DFLT_SBD_FILE_VALID_NAME_REGEX    "(\\d+)_(\\d{6})\\.sbd"

/** The default mail message raw file format.
 *
 *  The mail message raw file format, @ref
 *  argo::iridium::xfloat::XFloatMailMessage::raw_file_format_, is set as a result of parsing the
 *  command-line arguments and/or configuration file options during application initialization. If
 *  the option is not explicitly specified, then it will default to this value.
 */
#define XMAIL_DFLT_RAW_FILE_FORMAT              XMAIL_RAW_FORMAT_EXTENDED

/** The default mail message binary-SBD-file-generation-is-enabled flag value.
 *
 *  The mail message binary-SBD-file-generation-is-enabled flag value, @ref
 *  argo::iridium::xfloat::XFloatMailMessage::sbd_file_generation_is_enabled_, is set as a result
 *  of parsing the command-line arguments and/or configuration file options during application
 *  initialization. If the option is not explicitly specified, then it will default to this value.
 *
 */
#define XMAIL_DFLT_SBD_FILE_GENERATRION_ENABLE  true

/** The default mail message RAW-mail-file-generation-is-enabled flag value.
 *
 *  The mail message RAW-mail-file-generation-is-enabled flag value, @ref
 *  argo::iridium::xfloat::XFloatMailMessage::raw_file_generation_is_enabled_, is set as a result
 *  of parsing the command-line arguments and/or configuration file options during application
 *  initialization. If the option is not explicitly specified, then it will default to this value.
 */
#define XMAIL_DFLT_RAW_FILE_GENERATRION_ENABLE  true

namespace argo {
namespace iridium {
namespace xfloat {

/* Type Definitions: */

/** The SBD byte type.
 *
 *  This type definition determines the type that is used to represent a single byte the Iridium
 *  Short Burst Data (SBD) message. Generally, this field should be a single signed or unsigned
 *  byte type.
 */
typedef uint8_t XMailSbdByte_t;

/** The SBD data type.
 *
 *  This type definition determines the type that is used to represent the Iridium Short Burst
 *  Data (SBD) message. It is defined to be a vector of bytes of type @ref XMailSbdByte_t.
 */
typedef vector<XMailSbdByte_t> XMailSbdData_t;

/** The mail message raw file format type identifier enumeration.
 *
 *  This enumeration lists the supported formats for the raw mail message file. The format of RAW
 *  mail message files that are written to the local files system will be specified by one of the
 *  values of this enumeration.
 */
typedef enum XFloatMailRawFileFormat {

  /** The standard or legacy format for raw mail message files.
   *
   *  This format represents the standard raw mail message file format in which the attachment
   *  data is encoded in base-64. This format is currently used by the Solo-II decoding software.
   */
  XMAIL_RAW_FORMAT_STANDARD = 0,

  /** The extended format for raw mail message files.
   *
   *  This format is an extended version of the raw file format in which the attachment data
   *  is encoded in hex, rather than base-64.
   */
  XMAIL_RAW_FORMAT_EXTENDED = 1

} XFloatMailRawFileFormat_t;

/* Class Definition: */

/** The mail message class.
 *
 *  This class represents a single mail message received from the Iridium system, including
 *  fields that are parsed from the various parts of the mail message and the attached Iridium
 *  Short Burst Data (SBD) file. It is used to parse, validate, and write the mail message files
 *  to the local filesystem.
 *
 *
 *  @note   This class is mapped to an SQL database table by the @ref sql_table_description_
 *          object. This allows some or all of the field of this class to be persisted to the SQL
 *          database. Likewise, an object of this type may be constructed from the results of an
 *          SQL query to the corresponding table.
 */
class XFloatMailMessage:
    protected util::file::FileSystemUtils,
    public Iridium,
    public IridiumPosition,
    public util::mysql::LocalPersistentEntity {

    /* Friends Classes: */

    friend class XFloat;

    friend class XFloatDiveData;

    friend class XFloatMailProcessingService;

    friend class XFloatMailRetrievalService;

    /* Friend Functions: */

    friend ostream& operator<<(ostream& os, const XFloatMailMessage& mail);

    friend istream& operator>>(istream& is, XFloatMailMessage& mail) ;

    /** Friend function for populating some of the static members of this class from the
     *  configuration file options.
     */
    friend options_description util::config::LocalConfiguration::GenerateConfigurationFileOptionsDescription();

  private:

    /* Private Static Data Members: */

    /** The mail message header valid sender regular expression.
     *
     *  This regular expression is used the validate the FROM field of the email message header.
     *  If the mail message header FROM field does not match this regular expression, the the mail
     *  message will be considered invalid and will be discarded.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization. If the value is unspecified,
     *  then it defaults to a value of @ref XMAIL_DFLT_MSG_HEADER_VALID_SNDR_REGEX.
     */
    static string msg_header_valid_sender_regex_;

    /** The mail message header valid subject regular expression.
     *
     *  This regular expression is used to validate the SUBJECT field of the email message header.
     *  If the mail message header SUBJECT field does not match this regular expression, the the
     *  mail message will be considered invalid and will be discarded.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization. If the value is unspecified,
     *  then it defaults to a value of @ref XMAIL_DFLT_MSG_HEADER_VALID_SUBJ_REGEX.
     */
    static string msg_header_valid_subject_regex_;

    /** The RAW mail message file directory.
     *
     *  This field determines the directory to which RAW mail message files are written. If
     *  enabled, RAW mail message files will be written to subdirectories of this directory that
     *  are organized by the unique instrument identifiers.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static path raw_directory_path_;

    /** The raw mail message file format.
     *
     *  The value of this field determines the format of the raw mail message that is written to
     *  the local filesystem. The support formats are enumerated by @ref XFloatMailRawFileFormat_t.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization. If the value is unspecified,
     *  then it defaults to a value of @ref XMAIL_DFLT_RAW_FILE_FORMAT.
     */
    static int raw_file_format_;

    /** The global RAW-mail-message-file-generation-is-enabled flag.
     *
     *  This flag determines whether the ASCII RAW mail message files are written to the local
     *  filesystem. If TRUE, then the RAW mail message files will be written to subdirectories of
     *  the raw mail message directory which is specified by @ref raw_directory_path_. Otherwise,
     *  if FALSE, raw mail message will not be written to the local filesystem.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization. If the value is unspecified,
     *  then it defaults to a value of @ref XMAIL_DFLT_SBD_FILE_VALID_NAME_REGEX.
     */
    static bool raw_file_generation_is_enabled_;

    /** The global binary-SBD-file-generation-is-enabled flag.
     *
     *  This flag determines whether binary Iridium Short Burst Data (SBD) files are written to
     *  the local file system. If TRUE, then SBD files will be written to subdirectories of the
     *  SBD directory which is specified by @ref sbd_directory_path_. Otherwise, if FALSE, SBD
     *  files will not be written to the local filesystem.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization. If the value is unspecified,
     *  then it defaults to a value of FALSE.
     */
    static bool sbd_file_generation_is_enabled_;

    /** The valid SBD file email attachment filename regular expression.
     *
     *  This regular expression is used to validate the filename of the email message attachment.
     *  If the filename of the file attachment to the mail message matches this regular expression,
     *  then the mail message will be considered valid. Otherwise, if the filename does not match
     *  this regular expression, then the mail message will be considered invalid and will be
     *  discarded.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization. If the value is unspecified,
     *  then it defaults to a value of @ref XMAIL_DFLT_SBD_FILE_VALID_NAME_REGEX.
     */
    static string sbd_file_valid_name_regex_;

    /** The SBD file directory.
     *
     *  This field determines the directory to which the binary Iridium Short Burst Data (SBD)
     *  files are written. If enabled, SBD files will be written to subdirectories of this
     *  directory that are organized by the unique instrument identifiers.
     *
     *  The value of this field is set as a result of parsing the command-line arguments and/or
     *  configuration file options during application initialization.
     */
    static path sbd_directory_path_;

    /** The SQL table description object.
     *
     *  This SQL table description provides the details needed to map this object to a relational
     *  database table. It utilizes the accessor methods of the class and the details of the SQL
     *  table column to (1) create an SQL INSERT statement to save the object into the database,
     *  (2) populate one or more objects' fields as a result of an SQL SELECT query, or (3) query
     *  the SQL database to access one or more of the values that are stored the the SQL table
     *  corresponding to this object.
     */
    static const util::mysql::SqlTableDescription<XFloatMailMessage> sql_table_description_;

  private:

    /* Private Data Members: */

    /** The dive or cycle number of this mail message object.
     *
     *  This field is provides a non-unique index of the dive or cycle number of the mail message
     *  object for a given instrument. It is used to determine to which dive this mail message
     *  belongs. Together, the dive or cycle number and the Iridium Short Burst Data (SBD) packet
     *  index, @ref id_sbd_packet_, provide a unique index of the mail message. However, these
     *  fields are not used as the primary index of the mail message objects.
     *
     *  The value of this field is set as a result of parsing the mail message attachment. The
     *  value is read from the header of the attached SBD file. If the mail message has no SBD
     *  attachment, or the attachment is invalid, then the value of this field will be set to
     *  @ref XFLT_DIVE_UNKNOWN, which specifies an unknown dive or cycle number.
     */
    int id_dive_;

    /** The mail message unique identifier string.
     *
     *  This character sequence is a unique identifier for the mail message that is assigned by
     *  the mail server. However, this value is not used a primary identifier because the mail
     *  server all provides a unique integer identifier.
     *
     *  The value of this field is set as a result of parsing the mail message header. All mail
     *  messages will be assigned a unique identifier string, regardless of their validity.
     */
    string id_msg_;

    /** The mail message unique identifier (UID).
     *
     *  This unique integer identifier is assigned by the remote mail server. It is used as the
     *  primary unique identifier for a mail message.
     *
     *  The mail messages' UIDs also specify the order in which the mail messages were received by
     *  the mail server. Each message that the mail server receives is assigned a monotonically
     *  increasing UID value. Thus this field is also used to determine which mail messages have
     *  already been fetch from the mail server. When the mail retrieval thread, @ref
     *  XFloatMailRetrievalService, queries the maximum UID of mail messages that have been
     *  persisted to the SQL database. The result of the query plus one determines the minimum UID
     *  of the mail messages to fetch from the mail server.
     *
     *
     *  The value of this field is set as a result of parsing the mail message header. All mail
     *  messages will be assigned a unique integer identifier, regardless of their validity.
     */
    long id_msg_uid_;

    /** The Iridium Short Burst Data (SBD) Service Mobile Originated Message Sequence Number
     *  (MOMSN) identifier.
     *
     *  This identifier, provided by the Iridium system, uniquely identifies the sequence number
     *  of messages successfully sent from the instrument to the ground station. For each new
     *  message that is transmitted from the instrument, this number is incremented. It is
     *  provided in the body of the email from the Iridium system along with information about
     *  the position, etc.
     *
     *  The value of this field is set as a result of parsing the mail message body text. If the
     *  mail message body is invalid, then the value of this field will be set to @ref
     *  XFLT_ID_UNKNOWN.
     *
     *  @warning    This identifier should NOT be used to unique identify mail messages for a given
     *              instrument. If the instrument experiences a incomplete or failed Iridium
     *              transmission that results in an email from the Iridium system, then MOMSN will
     *              NOT be incremented for that message. Thus, there may exist multiple mail
     *              messages with the same MOMSN identifier.
     */
    int id_sbd_momsn_;

    /** The Iridium Short Burst Data (SBD) Service Mobile Terminated Message Sequence Number
     *  (MTMSN) identifier.
     *
     *  This identifier, provided by the Iridium system, uniquely identifies the sequence number
     *  of messages successfully sent from the ground station to the instrument. For each new
     *  message that is transmitted to the instrument from the ground station, this number is
     *  incremented. It is provided in the body of the email from the Iridium system along with
     *  information about the position, etc.
     *
     *  The value of this field is set as a result of parsing the mail message body text. If the
     *  mail message body is invalid, then the value of this field will be set to @ref
     *  XFLT_ID_UNKNOWN.
     */
    int id_sbd_mtmsn_;

    /** The Iridium Short Burst Data (SBD) message packet index.
     *
     *  This identifier uniquely indexes each SBD message transmitted for a single dive. It is
     *  used both to reconstruct the sequence in which the message were transmitted and to
     *  determine whether any of the message in the sequence are missing. A complete set of mail
     *  messages for a given dive will have a sequence of monotonically increasing SBD packet
     *  identifiers starting with a value of zero.
     *
     *  The value of this field is set as a result of parsing the mail message attachment. The
     *  value is read from the header of the attached SBD file. If the mail message has no SBD
     *  attachment, or the attachment is invalid, then the value of this field will be set to
     *  @ref XFLT_DIVE_UNKNOWN, which specifies an unknown SBD message packet index.
     */
    int id_sbd_packet_;

    /** The mail message body is valid flag.
     *
     *  This field specifies the validity of the mail message body. It is set as a result of
     *  parsing the mail message body text, which contains information about the Iridium session
     *  including the Iridium position, date/time, status, and message identifiers.
     */
    bool msg_body_is_valid_;

    /** The mail message body text.
     *
     *  This field contains the full text of the body of the mail message. The body of a valid
     *  mail message includes information about the Iridium session that is parsed and stored in
     *  the corresponding field of this object. These field include
     *      - the MOMSN identifier,
     *      - the MTMSN identifier,
     *      - the time of the Iridium session,
     *      - the Iridium session status code and status string, and
     *      - the Iridium position and Circular Error Probable (CEP) radius.
     */
    string msg_body_text_;

    /** The mail message header date/time.
     *
     *  This field represents the date/time-of-flight of the mail message from the Iridium system.
     *  The value of this field, is set as a result of parsing the mail message header.
     *
     *  @note       This date/time, along with the Iridium session time may be used to calculate
     *              the latency of the Iridium system.
     */
    ptime msg_header_datetime_;

    /** The mail message header FROM field.
     *
     *  This field represents the sender of the mail message. This field is used to validate the
     *  mail message by ensuring the only messages from desired senders are consumed by the
     *  application. The value of this field is set as a result of parsing the mail message header.
     */
    string msg_header_from_;

    /** The mail message header is valid flag.
     *
     *  This field indicates the validity of the mail message header. It is set as a result of
     *  parsing the mail message header. The criteria for a valid mail message header is that
     *      - the SUBJECT field of the mail message header matches the valid sender regular
     *        expression, @ref msg_header_valid_sender_regex_, and
     *      - the FROM field of the mail message header matcher the valid subject regular
     *        expression, @ref msg_header_valid_subject_regex_.
     */
    bool msg_header_is_valid_;

    /** The mail message header raw text.
     *
     *  This field contains the "raw" text of the mail message header. This text represents the
     *  header of the mail message in its native form, which includes additional information about
     *  routing, etc. that is not currently parsed from the mail message header.
     */
    string msg_header_raw_text_;

    /** The mail message header SUBJECT field.
     *
     *  This field contains the text of the subject line of the mail message. It is used to
     *  validate the mail message by ensuring that only messages with the proper subject line are
     *  consumed by the application. In addition, a valid mail message contains the IMEI number
     *  in the subject line. The subject line is parsed to determine the IMEI of the instrument
     *  that transmitted the message.
     */
    string msg_header_subject_;

    /** The mail message is valid flag.
     *
     *  This flag indicates the validity of the mail message. It is set as a result of parsing the
     *  mail message. The criteria for a valid mail message is that
     *      - the mail message header is valid (see @ref msg_header_is_valid_),
     *      - the mail message body is valid (see @ref msg_body_is_valid_), and
     *      - the mail message SBD file attachment is valid (see @ref sbd_data_is_valid_).
     */
    bool msg_is_valid_;

    /** The full mail message raw text.
     *
     *  This field contains the full mail message text in its raw form. This represents the native
     *  ASCII format of the mail message.
     */
    string msg_raw_text_;

    /** The path to the raw mail message file on the local filesystem.
     *
     *  This path is set as a result of writing the raw mail message file to the local filesystem.
     *  This path is relative to the raw file directory, which is specified by @ref
     *  raw_directory_path_.
     *
     */
    path raw_file_local_path_;

    /** The Iridium Short Burst Data (SBD) binary data.
     *
     *  This field contains the binary data that is read from the SBD file attachment. The content
     *  of this data is exactly that which was transmitted by the instrument.
     */
    XMailSbdData_t sbd_data_;

    /** The Iridium Short Burst Data (SBD) binary data is valid flag.
     *
     *  This field indicates whether the attached SBD message is valid. The criteria for a valid
     *  SBD message is that
     *      - the mail message has a file attachment,
     *      - the filename of the attached file matches the SBD file email attachment filename
     *        regular expression, @ref sbd_file_valid_name_regex_,
     *      - the Iridium session status code, @ref sbd_session_status_code_, has a value of
     *        zero, and
     *      - the SBD message size, in bytes, is greater than or equal to @ref XMSG_SBD_MIN_BYTES.
     */
    bool sbd_data_is_valid_;

    /** The path to the Iridium Short Burst Data (SBD) binary file on the local filesystem.
     *
     *  This path is set as a result of writing the binary SBD message file to the local
     *  filesystem. This path is relative to the SBD file directory, which is specified by @ref
     *  sbd_directory_path_.
     */
    path sbd_file_local_path_;

    /** The Iridium Short Burst Data (SBD) email attachment filename.
     *
     *  This field represents the name of the file attached to the mail message. This field is
     *  used, in part, to determined the validity of the SBD message attachment.
     */
    string sbd_file_name_;

    /** The size of the Iridium Short Burst Data (SBD) message, in bytes.
     *
     *  This field is parsed from the body of the mail message. It should match the size of the
     *  SBD data, @ref sbd_data_, which is read from the mail message attachment.
     */
    int sbd_file_size_bytes_;

    /** The Iridium Short Burst Data (SBD) session status code.
     *
     *  This field is parsed from the body of the mail message. It indicates that status of the
     *  Iridium session. A successful Iridium session reports a status code of 0. An unsuccessful
     *  Iridium session reports a non-zero status code, where the exact value of the status code
     *  indicates a specific error or type of error in the Iridium session.
     */
    int sbd_session_status_code_;

    /** The Iridium Short Burst Data (SBD) session status string.
     *
     *  This field is parsed from the body of the mail message. It contains a brief description
     *  of the status of the Iridium session that is indicated by the status code, @ref
     *  sbd_session_status_code_.
     */
    string sbd_session_status_string_;

    shared_ptr<vector<uint8_t> > xmsg_id_types_;


  private:

    /* Private Member Functions: */

    /** Parse a mail message.
     *
     *  This function is used to parse and validate a mail message. As a result of parsing the
     *  mail message, the fields of this class are populated.
     *
     *  @exception      log::util::MessageException if @c msg is NULL or an error occurs while
     *                  parsing the various parts of the mail message.
     *
     *  @param[in]      msg     The mail message to parse.
     */
    void ParseMailMsg(const vmime::shared_ptr<vmime::net::message>& msg) {

      try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        if(!msg)
          EXC_THROW("Message pointer is NULL!");

        // Get the message RAW text.
        vmime::utility::outputStreamStringAdapter msg_raw_text_ostream(msg_raw_text_);
        msg->extract(msg_raw_text_ostream);

        // Parse the mail message.
        istringstream(msg->getUID()) >> id_msg_uid_;
        vmime::shared_ptr<vmime::message> parsed_msg(msg->getParsedMessage());
        ParseMailMsgHeader(msg->getHeader());
        ParseMailMsgBody(parsed_msg);
        ParseMailMsgSbdAttachment(parsed_msg);

        // Set the validity of the mail message.
        set_msg_is_valid(msg_header_is_valid() && msg_body_is_valid() && sbd_data_is_valid());

      }CATCH_BLOCK("Failed to parse mail message!")
    }

    /** Parse a mail message.
     *
     *  This function is used to parse and validate a mail message. As a result of parsing the
     *  mail message, the fields of this class are populated.
     *
     *  @exception      log::util::MessageException if @c msg is NULL or an error occurs while
     *                  parsing the various parts of the mail message.
     *
     *  @param[in]      msg     The mail message to parse.
     */
    void ParseMailMsg(const vmime::shared_ptr<vmime::message>& msg) {

      try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        if(!msg)
          EXC_THROW("Message pointer is NULL!");

        // Get the message RAW text.
        vmime::utility::outputStreamStringAdapter msg_raw_text_ostream(msg_raw_text_);
        msg->generate(msg_raw_text_ostream);

        // Parse the mail message.
        //istringstream(msg->getUID()) >> id_msg_uid_;
        ParseMailMsgHeader(msg->getHeader());
        ParseMailMsgBody(msg);
        ParseMailMsgSbdAttachment(msg);

        // Set the validity of the mail message.
        set_msg_is_valid(msg_header_is_valid() && msg_body_is_valid() && sbd_data_is_valid());

      }CATCH_BLOCK("Failed to parse mail message!")
    }

    /** Parse the mail message header.
     *
     *  This function is used to parse and validate the mail message header. As a result of
     *  parsing the mail message, the field of this class relating to the mail message header
     *  are populated.
     *
     *  @exception      log::util::MessageException if @c head is NULL or an error occurs while
     *                  parsing the header of the mail message.
     *
     *  @param[in]      head        The mail message header to parse.
     */
    void ParseMailMsgHeader(const vmime::shared_ptr<const vmime::header>& head) {
      static const vmime::charset cs = vmime::charset(vmime::charsets::UTF_8);

      smatch sm;

      try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        if(!head)
          EXC_THROW("Message header pointer is NULL!");

        // Get the complete message header text.
        vmime::utility::outputStreamStringAdapter msg_header_text_stream(msg_header_raw_text_);
        head->generate(msg_header_text_stream);

        // Get "FROM" field.
        set_msg_header_from(head->From()->getValue<vmime::mailbox>()->getEmail().toString());

        // Get "DATE" field.
        vmime::shared_ptr<const vmime::datetime> vdt(head->Date()->getValue<vmime::datetime>());
        set_msg_header_datetime(
            ptime(
                date(vdt->getYear(), vdt->getMonth(), vdt->getDay()),
                hours(vdt->getHour()) + minutes(vdt->getMinute() + vdt->getZone())
                + seconds(vdt->getSecond())));

        try {
          // Get "SUBJECT" field.
          set_msg_header_subject(
              head->Subject()->getValue<vmime::text>()->getConvertedText(cs));
          if(regex_search(msg_header_subject(), sm, regex(msg_header_valid_subject_regex_))) {
            long imei;
            istringstream(sm[1]) >> imei;
            set_id_imei(imei);
          }
        }CATCH_BLOCK("Failed to parse SUBJECT field!")

        // Get "MESSAGE-ID" field.
        set_id_msg(head->MessageId()->getValue<vmime::messageId>()->getId());

        try {
          // Validate FROM and SUBJECT fields.
          if(!regex_match(
              msg_header_from(),
              regex(msg_header_valid_sender_regex_, regex::extended)))
            set_msg_header_is_valid(false);
          else if(!regex_match(
              msg_header_subject(),
              regex(msg_header_valid_subject_regex_, regex::extended)))
            set_msg_header_is_valid(false);
          else
            set_msg_header_is_valid(true);

        }CATCH_BLOCK("Failed to validate FROM and SUBJECT fields!")

      }CATCH_BLOCK("Failed to parse mail message header!")
    }

    /** Parse the mail message body.
     *
     *  This function is used to parse and validate the mail message body. The body of a valid
     *  mail message includes information about the Iridium session which are parsed in order to
     *  populate the corresponding fields of this class.
     *
     *  @exception      log::util::MessageException if @c msg is NULL or an error occurs while
     *                  parsing the body of the mail message.
     *
     *  @param[in]      msg         The message to parse
     */
    void ParseMailMsgBody(const vmime::shared_ptr<vmime::message>& msg) {
      smatch sm;

      try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        if(!msg)
          EXC_THROW("Message pointer is NULL!");

        // Get body text.
        vmime::messageParser mp(msg);
        vmime::utility::outputStreamStringAdapter body_text_stream(msg_body_text_);
        vmime::shared_ptr<const vmime::body> body(msg->getBody());
        for(size_t i = 0; i < mp.getTextPartCount(); ++i)
          vmime::shared_ptr<const vmime::textPart>(mp.getTextPartAt(i))->getText()->extract(
              body_text_stream);
        regex_replace(msg_body_text_, regex("\\n[^a-zA-Z0-9]*\\n"), string("\n"));
        trim(msg_body_text_);

        // Parse MOMSN.
        if(regex_search(msg_body_text_, sm, regex("MOMSN: (\\d+)")))
          istringstream(sm[1]) >> id_sbd_momsn_;

        // Parse MTMSN.
        if(regex_search(msg_body_text_, sm, regex("MTMSN: (\\d+)")))
          istringstream(sm[1]) >> id_sbd_mtmsn_;

        // Parse Time of Session.
        if(regex_search(
            msg_body_text_,
            sm,
            regex("Time of Session \\(UTC\\): ([a-zA-Z]{3} ([a-zA-Z]{3})[ ]+(\\d{1,2}) "
                "(\\d{2}:\\d{2}:\\d{2}) (\\d{4}))"))) {
          ptime time;
          stringstream ss;
          ss << sm[5] << "-" << sm[2] << "-" << setw(2) << setfill('0') << sm[3] << " " << sm[4];
          ss >> time;
          set_iridium_position_datetime(time);
        }

        // Parse session status.
        if(regex_search(msg_body_text_, sm, regex("Session Status: (\\d{2}) - ([^\n]+)"))) {
          istringstream(sm[1]) >> sbd_session_status_code_;
          set_sbd_session_status_string(sm[2]);
        }

        // Parse SBD message size (bytes);
        if(regex_search(msg_body_text_, sm, regex("Message Size \\(bytes\\): (\\d+)")))
          istringstream(sm[1]) >> sbd_file_size_bytes_;

        // Parse Iridium location.
        if(regex_search(
            msg_body_text_,
            sm,
            regex("Unit Location: Lat = (-?\\d+\\.\\d+) Long = (-?\\d+\\.\\d+)"))) {
          double lat, lon;
          istringstream(sm[1]) >> lat;
          istringstream(sm[2]) >> lon;
          set_iridium_position_latitude(lat);
          set_iridium_position_longitude(lon);
        }
        if(regex_search(msg_body_text_, sm, regex("CEPradius = (\\d+)"))) {
          int cep;
          istringstream(sm[1]) >> cep;
          set_iridium_position_cep_radius(cep);
        }

        // Validate message body.
        set_msg_body_is_valid(
            id_sbd_momsn_ != XMSG_ID_UNKNOWN && id_sbd_mtmsn_ != XMSG_ID_UNKNOWN
            && !iridium_position_datetime().is_not_a_date_time());
      }CATCH_BLOCK("Failed to parse mail message body!")
    }

    /** Parse the mail message SBD attachment.
     *
     *  This function is used to parse and validate the attachment of the mail message. A valid
     *  mail message contains a binary SBD file attachment that represents the data transmitted
     *  by the instrument.
     *
     *  @exception      log::util::MessageException if @c msg is NULL or an error occurs while
     *                  parsing the attachment of the mail message.
     *
     *  @param[in]      msg         The mail message to parse.
     */
    void ParseMailMsgSbdAttachment(const vmime::shared_ptr<vmime::message>& msg) {

      try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        if(!msg)
          EXC_THROW("Message pointer is NULL!");

        // Parse the attachment(s).
        vmime::messageParser mp(msg);
        for(size_t i = 0; i < mp.getAttachmentCount(); ++i) {
          vmime::shared_ptr<const vmime::attachment> attach = mp.getAttachmentAt(i);

          // Check attachment type.
          vmime::mediaType type = attach->getType();
          if(type.getType() != vmime::mediaTypes::APPLICATION)
            continue;

          // Check attachment name.
          if(!regex_search(
              sbd_file_name_ = attach->getName().getBuffer(),
              regex(sbd_file_valid_name_regex_)))
            continue;

          // Get bytes.
          vmime::utility::outputStreamByteArrayAdapter ostream_adapter(sbd_data_);
          attach->getData()->extract(ostream_adapter);

          // Validate.
          set_sbd_data_is_valid(sbd_data_.size() >= XMSG_SBD_MIN_BYTES
              && (sbd_session_status_code() == 0 || sbd_session_status_code() == 2));
          if(sbd_data_is_valid()) {

            // Parse dive and instrument identifier.
            set_id_instrument(u16(3, sbd_data_.data()));
            id_dive_ = i16(5, sbd_data_.data());
            id_sbd_packet_ = u08(7, sbd_data_.data());
          }
          return;
        }

      }CATCH_BLOCK("Failed to parse mail message attachment!")
    }

    static vector<shared_ptr<XFloatMailMessage> > SelectAllObjectsFromDatabase(
        const string& groupBy = "",
        const string& orderBy = "",
        const unsigned int& limit = 0,
        const unsigned int& offset = 0) {

      try {

        return sql_table_description_.SelectAllObjectsFromDatabase(
            groupBy,
            orderBy,
            limit,
            offset);

      }CATCH_BLOCK("Failed to query instrument objects from the database!")

      return vector<shared_ptr<XFloatMailMessage> >();
    }

    static shared_ptr<XFloatMailMessage> SelectObjectFromDatabase(
        const string& where = "",
        const string& groupBy = "",
        const string& orderBy = "",
        const unsigned int& offset = 0) {

      try {

        return sql_table_description_.SelectObjectFromDatabase(where, groupBy, orderBy, offset);

      }CATCH_BLOCK("Failed to query XFloat objects from the database!")

      return make_shared<XFloatMailMessage>();
    }

    /** Query mail message objects from the SQL database.
     *
     *  This function is used to query the SQL database table corresponding to the mail message
     *  class and construct a vector of mail message objects, populated from the results of the
     *  SQL query.
     *
     *  @exception  util::log::MessageException if an error occurs while querying the database
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
    static vector<shared_ptr<XFloatMailMessage> > SelectObjectsFromDatabase(
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

      }CATCH_BLOCK("Failed to query all X-Float objects from the database!")

      return vector<shared_ptr<XFloatMailMessage> >();
    }

    /** Select a value from the SQL database table that contains the persisted mail message
     *  objects.
     *
     *  This function may be used to select a singular value from the table that comprises the
     *  persisted mail message objects.
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

    /** Set the dive or cycle number of this mail message.
     *
     *  @param[in]  id_dive     The dive or cycle number of this mail message.
     */
    void set_id_dive(int id_dive) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      id_dive_ = id_dive;
    }

    /** Set the mail message unique identifier string.
     *
     *  @param[in]  id_msg      The mail message unique identifier string.
     */
    void set_id_msg(const string& id_msg) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      id_msg_ = id_msg;
    }

    /** Set the mail message unique identifier
     *
     *  @param[in]  id_msg_uid  The mail message unique identifier.
     */
    void set_id_msg_uid(long id_msg_uid) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      id_msg_uid_ = id_msg_uid;
    }

    /** Set the Iridium Short Burst Data (SBD) Service Mobile Originated Message Sequence Number
     *  (MOMSN).
     *
     *  @param[in]  momsn       The Iridium Short Burst Data (SBD) Service Mobile Originated
     *                          Message Sequence Number (MOMSN).
     */
    void set_id_sbd_momsn(int momsn) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      id_sbd_momsn_ = momsn;
    }

    /** Set the Iridium Short Burst Data (SBD) Service Mobile Terminated Message Sequence Number
     *  (MTMSN).
     *
     *  @param[in]  mtmsn       The Iridium Short Burst Data (SBD) Service Mobile Terminated
     *                          Message Sequence Number (MTMSN).
     */
    void set_id_sbd_mtmsn(int mtmsn) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      id_sbd_mtmsn_ = mtmsn;
    }

    /** Set the Iridium Short Burst Data (SBD) message packet index.
     *
     *  @param[in]  id_sbd_packet   Iridium Short Burst Data (SBD) message packet index.
     */
    void set_id_sbd_packet(int id_sbd_packet) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      id_sbd_packet_ = id_sbd_packet;
    }

    /** Set the message-body-is-valid flag.
     *
     *  @param[in]  is_valid    TRUE if the body of the Iridium message is valid, or FALSE
     *                          otherwise.
     */
    void set_msg_body_is_valid(bool is_valid) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      msg_body_is_valid_ = is_valid;
    }

    /** Set the mail message body text.
     *
     *  @param[in]  msg_body_text   The mail message body text.
     */
    void set_msg_body_text(const string& msg_body_text) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      msg_body_text_ = msg_body_text;
    }

    /** Set the mail message header date/time.
     *
     *  @param[in]  datetime        The mail message header date/time.
     */
    void set_msg_header_datetime(const ptime& datetime) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      msg_header_datetime_ = datetime;
    }

    /** Set the mail message header FROM field value.
     *
     *  @param[in]  from        The mail message header FROM field value.
     */
    void set_msg_header_from(const string& from) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      msg_header_from_ = from;
    }

    /** Set the mail-message-header-is-valid flag.
     *
     *  @param[in]  is_valid    TRUE if the mail message header is valid, or FALSE otherwise.
     */
    void set_msg_header_is_valid(bool is_valid) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      msg_header_is_valid_ = is_valid;
    }

    /** Set the mail message header SUBJECT field value.
     *
     *  @param[in]  subject     The mail message header SUBJECT field value.
     */
    void set_msg_header_subject(const string& subject) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      msg_header_subject_ = subject;
    }

    /** Set the mail message header raw text.
     *
     *  @param[in]  text        The mail message header raw text.
     */
    void set_msg_header_raw_text(const string& text) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      msg_header_raw_text_ = text;
    }

    /** Set the mail-message-is-valid flag.
     *
     *  @param[in]  is_valid    TRUE if the mail message is valid, or FALSE otherwise.
     */
    void set_msg_is_valid(bool is_valid) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      msg_is_valid_ = is_valid;
    }

    /** Set the full mail message raw text.
     *
     *  @param[in]  text        The full mail message raw text.
     */
    void set_msg_raw_text(const string& text) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      msg_raw_text_ = text;
    }

    /** Set the path to the raw mail message file on the local file system.
     *
     *  @param[in]  path        The path to the raw mail message file on the local file system.
     */
    void set_raw_file_local_path(const path& path) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      raw_file_local_path_ = path;
    }

    /** Set the Iridium Short Burst Data (SBD) for this mail message.
     *
     *  @param[in]  sbd_data    The Iridium Short Burst Data (SBD) for this mail message.
     */
    void set_sbd_data(const vector<uint8_t>& sbd_data) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      sbd_data_ = sbd_data;
    }

    /** Set the SBD-message-is-valid flag.
     *
     *  @param[in]  is_valid    TRUE if the Iridium Short Burst Data (SBD) message is valid, or
     *                          FALSE otherwise.
     */
    void set_sbd_data_is_valid(bool is_valid) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      sbd_data_is_valid_ = is_valid;
    }

    /** Set the path to the Iridium Short Burst Data (SBD) message file on the local file system.
     *
     *  @param[in]  path        The path to the Iridium Short Burst Data (SBD) message file on the
     *                          local file system.
     */
    void set_sbd_file_local_path(const path& path) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      sbd_file_local_path_ = path;
    }

    /** Set the name of the Iridium Short Burst Data (SBD) file.
     *
     *  @param[in]  name        The name of the Iridium Short Burst Data (SBD) file.
     */
    void set_sbd_file_name(const string& name) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      sbd_file_name_ = name;
    }

    /** Set the size of the Iridium Short Burst Data (SBD) message, in bytes.
     *
     *  @param[in]  size        The size of the Iridium Short Burst Data (SBD) message, in bytes.
     */
    void set_sbd_file_size_bytes(int size) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      sbd_file_size_bytes_ = size;
    }

    /** Set the Iridium Short Burst Data (SBD) session status code.
     *
     *  @param[in]  code       The Iridium Short Burst Data (SBD) session status code.
     */
    void set_sbd_session_status_code(int code) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      sbd_session_status_code_ = code;
    }

    /** Set the Iridium Short Burst Data (SBD) session status string.
     *
     *  @param[in]  str         The Iridium Short Burst Data (SBD) session status string.
     */
    void set_sbd_session_status_string(const string& str) {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      sbd_session_status_string_ = str;
    }

    void set_xmsg_id_types(const shared_ptr<vector<uint8_t> >& xmsg_id_types) {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      xmsg_id_types_ = xmsg_id_types;
    }

    /** Write the mail message files.
     *
     *  This function is used to write the RAW and SBD files to the local filesystem. First, the
     *  function checks if the unique instrument identifier is known. If the unique instrument
     *  identifier is unknown, the function will throw an exception. Otherwise, the function will
     *  continue on to write the RAW mail message file and the binary SBD message file to the
     *  local filesystem, if their respective criteria are met.
     *
     *  @exception  util::log::MessageException if the unique instrument identifier is unknown, or
     *              if an error occurs while writing the mail files to the local filesystem,
     */
    void WriteFiles() {

      try {


        // Check the WHOI ID.
        if(id_instrument() == XFLT_ID_UNKNOWN)
          EXC_THROW("The WHOI identifier is unknown!");

        // Write mail files.
        WriteRawFile();
        WriteSbdFile();

      }CATCH_BLOCK("Failed to write mail message files!")
    }

    void WriteRawFile();

    /** Write the binary Iridium Short Burst Data (SBD) file.
     *
     *  This function is used to write the binary SBD file to the local filesystem. If either the
     *  global SBD-file-generation-is-enabled flag, @ref sbd_file_generation_is_enabled_, or the
     *  SBD-data-is-valid field, @ref sbd_data_is_valid_, is FALSE, then this function will have
     *  no effect.
     *
     *  Otherwise, if the SBD data is valid and global SBD-file-generation-is-enabled flag is TRUE,
     *  then the SBD file path will be generated. If the SBD file already exists at the
     *  constructed path, then it will not be overwritten. If the file does not already exist, it
     *  will be written.
     *
     *  The path to the binary SBD message file will be constructed in the format
     *  [SBD-DIR]/[WWWW]/[WWWW]-[MMMMMM]-[DDDD].sbd, where
     *      - [SBD-DIR] is the SBD mail directory path specified, by @ref sbd_directory_path_,
     *      - [WWWW] is the four-digit unique instrument serial number,
     *      - [MMMMMM] is the six-digit MOMSN number, and
     *      - [DDDD] is the four-digit dive or cycle number.
     *
     *  @exception  util::log::MessageException if an error occurs while writing the binary SBD
     *              file to the local filesystem.
     */
    void WriteSbdFile() {

      try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        if(!sbd_file_generation_is_enabled_ || !sbd_data_is_valid())
          return;

        {   // Get mail directory path.
          ostringstream os;
          os << format("%s/%04d") % sbd_directory_path_.string() % id_instrument();
          GetDirectoryPathFromString(os.str(), true);
        }

        {   // Set SBD local path.
          ostringstream os;
          os << format("%04d/%04d-%06d-%04d.sbd")   //
                    % id_instrument() % id_instrument() % id_sbd_momsn() % id_dive();
          set_sbd_file_local_path(os.str());
        }

        // Write SBD file.
        path sbd_file_path(sbd_directory_path_.string() + "/" + sbd_file_local_path().string());
        if(!bfs::exists(sbd_file_path)) {
          linf() << "Writing SBD file: " << sbd_file_path << "...";
          ofstream sbd_file_stream(sbd_file_path);
          sbd_file_stream.write((char*) sbd_data().data(), sbd_data().size());
          sbd_file_stream.flush();
          sbd_file_stream.close();
        }
      }CATCH_BLOCK("Failed to write SBD file!")
    }

  public:

    /* Public Member Functions: */

    /** The default constructor.
     *
     */
    XFloatMailMessage() :
      Iridium(),
      IridiumPosition(),
      id_dive_(XFLT_DIVE_UNKNOWN),
      id_msg_(),
      id_msg_uid_(XFLT_ID_UNKNOWN),
      id_sbd_momsn_(XFLT_ID_UNKNOWN),
      id_sbd_mtmsn_(XFLT_ID_UNKNOWN),
      id_sbd_packet_(XFLT_ID_UNKNOWN),
      msg_body_is_valid_(false),
      msg_body_text_(),
      msg_header_datetime_(not_a_date_time),
      msg_header_from_(),
      msg_header_is_valid_(false),
      msg_header_subject_(),
      msg_is_valid_(false),
      raw_file_local_path_(),
      sbd_data_(),
      sbd_data_is_valid_(false),
      sbd_file_local_path_(),
      sbd_file_name_(),
      sbd_file_size_bytes_(XFLT_SBD_SIZE_BYTES_UNKNOWN),
      sbd_session_status_code_(XFLT_SBD_SESSION_STATUS_CODE_UNKNOWN),
      sbd_session_status_string_() {

      sbd_data_.reserve(340);

    }

    /** Construct from a VMime mail message.
     *
     *  @param[in]      msg         The mail message.
     */
    XFloatMailMessage(const vmime::shared_ptr<vmime::net::message>& msg) :
      Iridium(),
      IridiumPosition(),
      id_dive_(XFLT_DIVE_UNKNOWN),
      id_msg_(),
      id_msg_uid_(XFLT_ID_UNKNOWN),
      id_sbd_momsn_(XFLT_ID_UNKNOWN),
      id_sbd_mtmsn_(XFLT_ID_UNKNOWN),
      id_sbd_packet_(XFLT_ID_UNKNOWN),
      msg_body_is_valid_(false),
      msg_body_text_(),
      msg_header_datetime_(not_a_date_time),
      msg_header_from_(),
      msg_header_is_valid_(false),
      msg_header_subject_(),
      msg_is_valid_(false),
      raw_file_local_path_(),
      sbd_data_(),
      sbd_data_is_valid_(false),
      sbd_file_local_path_(),
      sbd_file_name_(),
      sbd_file_size_bytes_(XFLT_SBD_SIZE_BYTES_UNKNOWN),
      sbd_session_status_code_(XFLT_SBD_SESSION_STATUS_CODE_UNKNOWN),
      sbd_session_status_string_() {

      sbd_data_.reserve(340);

      try {

        ParseMailMsg(msg);

      }CATCH_BLOCK("Failed to construct mail message object!")
    }

    /** The destructor.
     *
     */
    ~XFloatMailMessage() {
      /* Do nothing. */
    }

    /** Get the mail message listing file comment string.
     *
     *  @return         The mail message listing file comment string.
     */
    static string GenerateListingComment() {

      using namespace std;
      using boost::format;

      ostringstream os;

      try {

        //            static const format fmt1("#   %5s %4s %4s %15s %5d %5d %3d %20s %19s %4d");
        //            static const format fmt2("#    %5s %4s %4s %15s %5d %5d %3d %20s %19s %4d");
        //
        //            os << format(fmt1) % "SN" % "dive" % "size" % "IMEI" % "MOMSN" % "MTMSN" % "PID"
        //                       % "date/time" % "position" % "CEP"
        //               << endl;
        //            os << format(fmt2) % "" % "" % "(b)" % "" % "" % "" % "" % "" % "" % "(km)";
        os << "#      SN dive size            IMEI MOMSN MTMSN PID            date/time            position  CEP"
            << endl
            << "#               (b)                                                                          (km)";

      }CATCH_BLOCK("Failed to generate mail message listing comment!")

      return os.str();
    }

    /** Generate the listing file entry string for this mail message.
     *
     *  @return         The listing file entry string for this mail message.
     */
    const string GenerateListingString() const {
      ostringstream os;

      try {

        const format fmt("SBD %5d %4d %4d %015d %5d %5d %3d %20s %19s %4d");

        os << format(fmt) % id_instrument() % id_dive() % sbd_file_size_bytes() % (id_imei() % 10000000)
                        % id_sbd_momsn() % id_sbd_mtmsn() % id_sbd_packet()
                        % iridium_position_datetime_as_iso8601_string()
                        % iridium_position_as_iso6709_string() % iridium_position_cep_radius();

      }CATCH_BLOCK("Failed to generate listing string from mail message object!")

      return os.str();
    }

    const string GenerateSbdHexListingString() const {

      static const format fmt_char(" %2c");
      static const format fmt_hex(" %02X");
      static const format fmt_int(" %02d");
      static const format fmt_line("shx %5d %4d %5d %3d %3d 0x%04X (%04u) | 0x");

      size_t byte_count = 0;
      ostringstream os;
      size_t idx = 0;

      try {

        /* Print header lines:*/

        {

          os << "#      SN dive   size MOMSN MTMSN PID"
              << endl
              << "#                 (b)"
              << endl;

          //          static const format fmt1("#   %4s %4s %6s %5d %5d %3d");
          //          static const format fmt2("#    %4s %4s%7s %5d %5d %3d");
          //          os << format(fmt1) % "SN" % "dive" % "size"    % "MOMSN" % "MTMSN" % "PID" << endl;
          //          os << format(fmt2) % ""   % ""     % "(bytes)" % ""      % ""      % ""    << endl;
        }

        { static const format fmt("SHX %5d %4d %6d %5d %5d %3d");
        os << format(fmt)
                  % id_instrument() % id_dive()      % sbd_file_size_bytes()
                  % id_sbd_momsn()  % id_sbd_mtmsn() % id_sbd_packet()        << endl;
        }

        os << "#---------------------------------------------------------------------------------------------------"
            << endl
            << "#      SN dive MOMSN idx PID               | 0x  0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F"
            << endl
            << "#                                          |    00 01 02 03  04 05 06 07  08 09 10 11  12 13 14 15"
            << endl
            << "# - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - "
            << endl;

        // Print the bytes in a readable format.
        vector<uint8_t> data(sbd_data());
        for(vector<uint8_t>::const_iterator data_iterator = data.begin();  data_iterator < data.end();) {

          ostringstream readable_line_char;
          ostringstream readable_line_hex;

          // Iterate over bytes in groups of 16.
          for(size_t i = 0; i < 16 && data_iterator < data.end(); i++) {
            if(i == 0) {
              readable_line_hex << format(fmt_line)
                      % id_instrument() % id_dive() % id_sbd_momsn() % idx++ % id_sbd_packet()
                      % byte_count % byte_count;
              readable_line_char << "#                                          |   ";
            } else if(byte_count % 4 == 0) {
              readable_line_hex << " ";
              readable_line_char << " ";
            }

            readable_line_hex << format(fmt_hex) % (int) *data_iterator;

            if(isprint(*data_iterator)) readable_line_char << format(fmt_char) % (char) *data_iterator;
            else                        readable_line_char << format(fmt_char) % ".";

            data_iterator++;
            byte_count++;
          }

          os << readable_line_hex.str() << endl << readable_line_char.str() << endl;
        }
        os << "#---------------------------------------------------------------------------------------------------" << endl;

      }CATCH_BLOCK("Failed to generate SBD hex listing string from mail message object!")

      return os.str();
    }

    /** Get the cycle or dive number of this mail message.
     *
     *  @return         The cycle or dive number of this mail message.
     */
    int id_dive() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return id_dive_;
    }

    /** Get the mail message unique identifier string.
     *
     *  @return         The mail message unique identifier string.
     */
    string id_msg() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return id_msg_;
    }

    /** Get the mail message unique identifier.
     *
     *  @return         The mail message unique identifier.
     */
    long id_msg_uid() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return id_msg_uid_;
    }

    /** Get the Iridium Short Burst Data (SBD) Service Mobile Originated Message Sequence Number
     *  (MOMSN) identifier.
     *
     *  @return         The  Iridium Short Burst Data (SBD) Service Mobile Originated Message
     *                  Sequence Number (MOMSN) identifier.
     */
    int id_sbd_momsn() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return id_sbd_momsn_;
    }

    /** Get the Iridium Short Burst Data (SBD) Service Mobile Terminated Message Sequence Number
     *  (MTMSN) identifier.
     *
     *  @return         The Iridium Short Burst Data (SBD) Service Mobile Terminated Message
     *                  Sequence Number (MTMSN) identifier.
     */
    int id_sbd_mtmsn() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return id_sbd_mtmsn_;
    }

    /** Get the Iridium Short Burst Data (SBD) message packet index.
     *
     *  @return         The Iridium Short Burst Data (SBD) message packet index.
     */
    int id_sbd_packet() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return id_sbd_packet_;
    }

    /** Is the body of the mail message valid?
     *
     *  @return         TRUE if the body of the mail message is valid, or FALSE otherwise.
     */
    bool msg_body_is_valid() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return msg_body_is_valid_;
    }

    /** Get the mail message body text.
     *
     *  @return         The mail message body text.
     */
    string msg_body_text() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return msg_body_text_;
    }

    /** Get the mail message header date/time.
     *
     *  @return         The mail message header date/time.
     */
    ptime msg_header_datetime() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return msg_header_datetime_;
    }

    /** Get the mail message header FROM field.
     *
     *  @return         The mail message header FROM field.
     */
    string msg_header_from() const {
      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return msg_header_from_;
    }

    /** Is the header of the mail message valid?
     *
     *  @return         TRUE if the header of the mail message is valid, or FALSE otherwise.
     */
    bool msg_header_is_valid() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return msg_header_is_valid_;
    }

    /** Get the message header raw text.
     *
     *  @return         The message header raw text.
     */
    string msg_header_raw_text() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return msg_header_raw_text_;
    }

    /** Get the mail message header SUBJECT field.
     *
     *  @return         The mail message header SUBJECT field.
     */
    string msg_header_subject() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return msg_header_subject_;
    }

    /** Is the mail message valid?
     *
     *  @return         TRUE if the mail message is valid, or FALSE otherwise.
     */
    bool msg_is_valid() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return msg_is_valid_;
    }

    /** Get the full mail message raw text.
     *
     *  @return         The full mail message raw text.
     */
    string msg_raw_text() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return msg_raw_text_;
    }

    /** Get the path to the raw mail message file on the local filesystem.
     *
     *  @return         The path to the raw mail message file on the local filesystem.
     */
    path raw_file_local_path() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return raw_file_local_path_;
    }

    /** Save this mail message object to the SQL database.
     *
     *  @throw          util::log::MessageException if an error occurs.
     *
     */
    void SaveObjectToDatabase() const {

      try {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        sql_table_description_.SaveObjectToDatabase(*this);

      }CATCH_BLOCK("Failed to save mail message to database!")
    }

    /** Get the Iridium Short Burst Data (SBD) binary data.
     *
     *  @return         The Iridium Short Burst Data (SBD) binary data.
     */
    vector<uint8_t> sbd_data() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return sbd_data_;
    }

    /** Is the Iridium Short Burst Data (SBD) message valid?
     *
     *  @return         TRUE if the Iridium Short Burst Data (SBD) message is valid, or FALSE
     *                  otherwise.
     */
    bool sbd_data_is_valid() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return sbd_data_is_valid_;
    }

    /** Get the path to the Iridium Short Burst Data (SBD) binary file on the local filesystem.
     *
     *  @return         The path to the Iridium Short Burst Data (SBD) binary file on the local
     *                  filesystem.
     */
    path sbd_file_local_path() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return sbd_file_local_path_;
    }

    /** Get the Iridium Short Burst Data (SBD) email attachment filename.
     *
     *  @return         The Iridium Short Burst Data (SBD) email attachment filename.
     */
    string sbd_file_name() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return sbd_file_name_;
    }

    /** Get the size of the Iridium Short Burst Data (SBD) message, in bytes.
     *
     *  @return         The size of the Iridium Short Burst Data (SBD) message, in bytes.
     */
    int sbd_file_size_bytes() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return sbd_file_size_bytes_;
    }

    /** Get the Iridium Short Burst Data (SBD) session status code.
     *
     *  @return         The Iridium Short Burst Data (SBD) session status code.
     */
    int sbd_session_status_code() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return sbd_session_status_code_;
    }

    /** Get the Iridium Short Burst Data (SBD) session status string.
     *
     *  @return         The Iridium Short Burst Data (SBD) session status string.
     */
    string sbd_session_status_string() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return sbd_session_status_string_;
    }

    const shared_ptr<vector<uint8_t> >& xmsg_id_types() const {

      // Get an exclusive lock.
      lock_guard<recursive_mutex> guard(mutex_);

      return xmsg_id_types_;
    }

};

/* Function Prototypes: */

ostream& operator<<(ostream& os, const XMailSbdData_t& data);

ostream& operator<<(ostream& os, const XFloatMailMessage& mail);

istream& operator>>(istream& is, XFloatMailMessage& mail);

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */

#endif /* XFLOATMAILMESSAGE_H_ */
