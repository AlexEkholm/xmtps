/** The @ref util::xmsg::XGPS class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jun 9, 2014 3:57:31 PM
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

#ifndef XGPS_H_
#define XGPS_H_

/* Includes */

// local
#include "XMessage.h"

namespace argo {
namespace iridium {
namespace xfloat {

// forward declarations
class XFloatDiveData;

} /* namespace xfloat */
} /* namespace iridium */
} /* namespace argo */

namespace util {
namespace xmsg {

using kmldom::CoordinatesPtr;

/** Class representing a GPS type X-Message.
 *
 */
class XGPS:
        public XMessage,
        public position::Position,
        public util::kml::KmlUtil,
        public util::mysql::LocalPersistentEntity {

    friend class argo::iridium::xfloat::XFloatDiveData;

    /* Friend Functions: */

    /** Friend function for creating an X-Float object using the private constructor and a shared
     *  pointer to that object.
     *
     */
    friend shared_ptr<XGPS> std::make_shared<XGPS>();

private:

    /* Data Members: */

    /** The horizontal Dilution of Precision (HDOP) of the GPS fix. */
    float horizontal_dilution_of_precision_;

    /** The number of satellites used in GPS fix. */
    uint8_t num_satellites_;

    /** The maximum Signal-to-Noise Ratio (SNR) for this GPS fix. */
    uint8_t signal_to_noise_max_;

    /** The mean Signal-to-Noise Ratio (SNR) for this GPS fix. */
    uint8_t signal_to_noise_mean_;

    /** The minimum Signal-to-Noise Ratio (SNR) for this GPS fix. */
    uint8_t signal_to_noise_min_;

    static const util::mysql::SqlTableDescription<XGPS> sql_table_description_;

    /** The time to acquire this GPS fix, in seconds. */
    uint16_t time_to_acquire_fix_;

private:

    /* Private Member Functions: */

    void set_horizontal_dilution_of_precision(float horizontal_dilution_of_precision) {
        horizontal_dilution_of_precision_ = horizontal_dilution_of_precision;
    }

    void set_id_type(uint8_t type) {
        set_id_xmsg_type(static_cast<XMsgTypeId_t>(type));
    }

    void set_num_satellites(uint8_t num_satellites) {
        num_satellites_ = num_satellites;
    }

    void set_signal_to_noise_max(uint8_t signal_to_noise_max) {
        signal_to_noise_max_ = signal_to_noise_max;
    }

    void set_signal_to_noise_mean(uint8_t signal_to_noise_mean) {
        signal_to_noise_mean_ = signal_to_noise_mean;
    }

    void set_signal_to_noise_min(uint8_t signal_to_noise_min) {
        signal_to_noise_min_ = signal_to_noise_min;
    }

    void set_time_to_acquire_fix(uint16_t time_to_acquire_fix) {
        time_to_acquire_fix_ = time_to_acquire_fix;
    }

    static void DeleteFromDatabase(int id_inst) {

        ostringstream os;

        try {



            os << "id_instrument = " << id_inst;

            sql_table_description_.DeleteObjectsFromDatabase(
                                os.str());

        } CATCH_BLOCK("Failed to delete GPS objects from database!")
    }

    static void DeleteFromDatabase(int id_inst, int id_dive) {

        ostringstream os;

        try {



            os << "id_instrument = " << id_inst << " AND id_dive == " << id_dive;

            sql_table_description_.DeleteObjectsFromDatabase(
                                os.str());

        } CATCH_BLOCK("Failed to delete GPS objects from database!")
    }

public:

    /* Public Member Functions: */

    XGPS() :
                    XMessage(),
                    Position(),
                    horizontal_dilution_of_precision_(0),
                    num_satellites_(0),
                    signal_to_noise_max_(0),
                    signal_to_noise_mean_(0),
                    signal_to_noise_min_(0),
                    time_to_acquire_fix_(0) {
        /* Do nothing. */
    }

    /** The parameterized, blank constructor.
     *
     *  @param[in]  dive        The dive or cycle number of this message.
     *  @param[in]  whoi        The unique WHOI identifier.
     *  @param[in]  type        The X-Message type identifier.
     *
     *
     *  @exception  None. Never throws.
     */
    XGPS(int16_t dive, uint16_t whoi, XMsgTypeId_t type) ATTR_NO_EXCEPTIONS :
                    XMessage(dive, whoi, type),
                    Position(),
                    horizontal_dilution_of_precision_(0),
                    num_satellites_(0),
                    signal_to_noise_max_(0),
                    signal_to_noise_mean_(0),
                    signal_to_noise_min_(0),
                    time_to_acquire_fix_(0) {
        /* Do nothing. */
    }

    /** The parameterized, binary data constructor.
     *
     *  @param[in]      dive        The dive or cycle number of this message.
     *  @param[in]      whoi        The unique WHOI identifier.
     *  @param[in]      data        The binary X-message data to decode.
     *
     *  @throw          util::log::MessageException if an decode error occurs.
     */
    XGPS(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XMessage(dive, whoi, data),
                    Position(),
                    horizontal_dilution_of_precision_(0),
                    num_satellites_(0),
                    signal_to_noise_max_(0),
                    signal_to_noise_mean_(0),
                    signal_to_noise_min_(0),
                    time_to_acquire_fix_(0) {
        DecodeXMsgData(data);    // Decode message.
    }

    /** The destructor.
     *
     */
    ~XGPS() {
        /* Do nothing. */
    }

    /** Decode the GPS X-message from binary data.
     *
     *  @param[in]      data        The binary GPS X-message data to decode.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {
        //    int ifval;
        try {
            set_position_is_valid(false);

            // Check for valid X message ID, size, & terminator.
            const uint8_t* d = CheckXMsgData(data, (int) -1, (int) 24);
            if(id_xmsg_type() > 5) {
                ostringstream msg;
                msg << "Invalid GPS XMessage ID: expected 0x00 through 0x05, received 0x"
                    << hex
                    << setw(2)
                    << setfill('0')
                    << id_xmsg_type()
                    << "!";
                EXC_THROW(msg.str());
            }

            // Check if the fix is valid.
            if(i08(d,3) == 0) {

                // GPS fix is not valid.
                set_position_latitude(UTIL_POS_INVALID_LAT);
                set_position_longitude(UTIL_POS_INVALID_LON);
                set_position_is_valid(false);

            } else {

                // GPS fix is valid -- parse GPS position.
                int32_t llat = i32(d, 4), llon = i32(d, 8);
                set_position_latitude(((double) llat) * 1e-7);
                set_position_longitude(((double) llon) * 1e-7);

                if(!IsPositionValid(position_latitude(), position_longitude())) {

                    set_position_latitude(UTIL_POS_INVALID_LAT);
                    set_position_longitude(UTIL_POS_INVALID_LON);

                } else {

                    set_position_is_valid(true);

                }
            }

            // Set position time from GPS date.
            set_gps_position_datetime(u16(d, 12), u08(d, 14), u08(d, 15), u08(d, 16));
            if(position_datetime() < bpt::from_iso_string(string("20000101T000000")) || position_datetime() > GetCurrentUtcTime()) {
                ldbg() << "GPS pos date/time: " << position_datetime_as_iso8601_string();

                set_position_datetime(bpt::not_a_date_time);
                set_position_is_valid(false);
                set_position_latitude(UTIL_POS_INVALID_LAT);
                set_position_longitude(UTIL_POS_INVALID_LON);
            }

            time_to_acquire_fix_ = u08(d,17) * 10;                               // Time to fix
            num_satellites_ = u08(d,18) & 0x0F;                                  // No. satellites
            signal_to_noise_min_ = u08(d, 19);                                   // SNR
            signal_to_noise_mean_ = u08(d, 20);
            signal_to_noise_max_ = u08(d, 21);

            horizontal_dilution_of_precision_ = (((float) u08(d, 22)) * 0.1);    // HDOP

        }CATCH_BLOCK("Failed to decode GPS X message!")
    }

    void static DeleteObjectsFromDatabase(const string& where)   {

        try {

            sql_table_description_.DeleteObjectsFromDatabase(where);

        } CATCH_BLOCK("Failed to delete GPS objects from database!")

    }

    /** Get the listing entry comment string.
     *
     *  @return         The listing entry comment string.
     */
    static string GenerateListingComment() {
        ostringstream os;

        try {
//            const format fmt("#   %5s %4s %3s %20s %19s %5s %4s %5s %5s %5s %4s %4s\n"
//                    "#   %5s %4s %3s %20s %21s %6s%4s %6s%6s%6s %4s %4s");
//            os << format(fmt) % "SN" % "dive" % "idx" % "date/time" % "position" % "TTF" % "nSat"
//                    % "SNR" % "SNR" % "SNR" % "HDOP" % "stat" % "" % "" % "" % "" % "" % "(sec)"
//                    % "" % "(min)" % "(avg)" % "(max)" % "" % "";
          os << "#      SN dive idx            date/time            position   TTF nSat   SNR   SNR   SNR HDOP stat"
             << endl
             << "#                                                            (sec)      (min) (avg) (max)";

        }CATCH_BLOCK("Failed to generate GPS message listing comment!")

        return os.str();
    }

    /** Return a string representation of this GPS message.
     *
     *  @return         A string representation of this GPS message.
     */
    const string GenerateListingString() const {
        ostringstream os;

        try {
            const format fmt("GPS %5d %4d %3s %20s %19s %5d %4d %5d %5d %5d %4.1f %4d");
            os << format(fmt) % id_instrument() % id_dive() % (int) id_xmsg_type()
                    % position_datetime_as_iso8601_string() % postition_as_iso6709_string()
                    % time_to_acquire_fix() % (int) num_satellites() % (int) signal_to_noise_min()
                    % (int) signal_to_noise_mean() % (int) signal_to_noise_max()
                    % horizontal_dilution_of_precision() % 0;
        }CATCH_BLOCK("Failed to generate listing string from GPS position object!")

        return os.str();
    }

    /** Generate a phy file entry string representation of this GPS position message object.
     *
     *  @param[in]      xmits       The number of transmissions for this dive data.
     *
     *  @throw          log::MessageException if an error occurs
     *
     *  @return         A phy file entry string representation of this GPS position message object.
     */
    const string GeneratePhyString(size_t xmits) const {
        ostringstream os;

        try {
            bpt::ptime dt(position_datetime());
            os << "================================================================" << endl;
            os << "   LATITUDE  LONGITUDE YEAR/MO/DY HR:MN:SC XMITS SAT POSITION-QC" << endl;
            os << format(" %+10.5f %+10.5f ") % position_latitude() % position_longitude();
            if(dt.is_not_a_date_time()) {
                os << "9999/99/99 99:99:99 ";
            } else {
                os << format("%04d/%02d/%02d %02d:%02d:%02d ") //
                % dt.date().year() % dt.date().month().as_number() % dt.date().day()
                        % dt.time_of_day().hours() % dt.time_of_day().minutes()
                        % dt.time_of_day().seconds();
            }
            os << format("%5d %3d %11d") //
            % xmits % (int) num_satellites_ % (position_is_valid() ? 0 : 9)
               << endl;
            os << "================================================================";
        }CATCH_BLOCK("Failed to generate PHY string representation of GPS object!")

        return os.str();
    }

    float horizontal_dilution_of_precision() const {
        return horizontal_dilution_of_precision_;
    }

    uint8_t id_type() const {
        return id_xmsg_type();
    }

    uint8_t num_satellites() const {
        return num_satellites_;
    }

    /** Save this GPS message object to the SQL database.
     *
     *  @throw          util::log::MessageException if an error occurs.
     *
     */
    void SaveObjectToDatabase() const {

        try {

            sql_table_description_.SaveObjectToDatabase(*this);

        }CATCH_BLOCK("Failed to save GPS object to database!")
    }

    static vector<shared_ptr<XGPS> > SelectAllObjectsFromDatabase(
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

        }CATCH_BLOCK("Failed to query GPS message objects from the database!")

        return vector<shared_ptr<XGPS> >();
    }

    static shared_ptr<XGPS> SelectObjectFromDatabase(
            const string& where = "",
            const string& groupBy = "",
            const string& orderBy = "",
            const unsigned int& offset = 0) {

        try {

            return sql_table_description_.SelectObjectFromDatabase(where, groupBy, orderBy, offset);

        }CATCH_BLOCK("Failed to query GPS message objects from the database!")

        return make_shared<XGPS>();
    }

    /** XXX
     *
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
    static vector<shared_ptr<XGPS> > SelectObjectsFromDatabase(
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

        }CATCH_BLOCK("Failed to query GPS message objects from the database!")

        return vector<shared_ptr<XGPS> >();
    }

    /** XXX
     *
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

    uint8_t signal_to_noise_max() const {
        return signal_to_noise_max_;
    }

    uint8_t signal_to_noise_mean() const {
        return signal_to_noise_mean_;
    }

    uint8_t signal_to_noise_min() const {
        return signal_to_noise_min_;
    }

    uint16_t time_to_acquire_fix() const {
        return time_to_acquire_fix_;
    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XGPS_H_ */
