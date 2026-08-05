/** The @ref util::xmsg::XArgo class header file.
 *
 *  This is the header file for the Argo Data X-Message class, @ref util::xmsg::XArgo. This
 *  X-Message object contains details about the dive cycle characteristics of the instrument as
 *  well as the sensor gain and offset values.
 *
 *  This file contains the class definition and all of its functionality.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jun 12, 2014 2:29:34 PM
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

#ifndef XARGO_H_
#define XARGO_H_

/* Includes: */

// local
#include "XMessage.h"

/* Type Definitions: */

/** Enumeration of the Argo X-Message instrument type identifiers.
 *
 */
typedef enum XArgoInstrumentType {

    /** Argo X-Message instrument type identifier 0: The Solo-II Argo float identifier.
     *
     */
    XARGO_INSRUMENT_TYPE_SOLO2 = 0,

    /** Argo X-Message instrument type identifier 1: The Deep Argo float identifier.
     *
     */
    XARGO_INSRUMENT_TYPE_DEEP_ARGO = 1,

    /** Argo X-Message instrument type identifier 3: The Air-Launchable Autonomous Micro-Observer
     *  (ALAMO) hurricane float type identifier.
     */
    XARGO_INSRUMENT_TYPE_ALAMO = 3,

    /** Argo X-Message instrument type identifier 99: The unknown, unset, unspecified, or invalid
     *  instrument type identifier.
     */
    XARGO_INSRUMENT_TYPE_UNKNOWN = 99

} XArgoInstrumentType_t;

namespace util {
namespace xmsg {

/* Class Definition: */

/** Class representing Argo X message.
 *
 * 	This message contains information about the characteristics of the dive cycle.
 *
 *  @todo   Add support for the Solo-II-type Argo data message.
 *  @todo   Add support for the Deep-Argo-type Argo data message.
 */
class XArgo:
        public util::date::DateUtils,
        public XMessage {

private:

    /* Dynamic Data Members: */

    /** The Argo instrument type identifier.
     *
     */
    XArgoInstrumentType_t argo_instrument_type_;

    /** The X message format major version number.
     *
     */
    uint8_t x_version_major_;

    /** The X Message format minor version number.
     *
     */
    uint8_t x_version_minor_;

    /* Dynamic Data Members - State Start Times: */

    /** The ascend state start time.
     *
     *  @note           This field is used for ALAMO-type floats only.
     *  @note           This field was added in X-Message format version 2.1.
     */
    ptime ascent_state_start_time_;

    /** The descend state start time.
     *
     *  @note           This field is used for ALAMO-type floats only.
     *  @note           This field was added in X-Message format version 2.1.
     */
    ptime descent_state_start_time_;

    /** The park state start time.
     *
     *  @note           This field is used for ALAMO-type floats only.
     *  @note           This field was added in X-Message format version 2.1.
     */
    ptime drift_state_start_time_;

    /** The surface state start time.
     *
     *  @note           This field is used for ALAMO-type floats only.
     *  @note           This field was added in X-Message format version 2.1.
     */
    ptime surface_state_start_time_;

    /* Dynamic Data Members - Dive Cycle Characteristics: */

    /** The maximum time to fall from the surface to drift/park depth, in minutes.
     *
     */
    uint16_t max_fall_to_drift_duration_;

    /** The maximum time to fall from the drift/park depth to profile depth, in seconds.
     *
     */
    uint16_t max_fall_to_profle_duration_;

    /** The maximum rise/ascend/profile time, in minutes.
     *
     */
    uint16_t max_ascent_duration_;

    /** The maximum duration at the surface, in seconds.
     *
     */
    uint16_t max_surface_duration_;

    /** The mission phase number of this dive.
     *
     */
    uint8_t mission_phase_;

    /** The number of seeks to drift depth.
     *
     */
    uint16_t num_seeks_to_drift_pressure_;

    /** The interval duration between subsequent seeks to drift/park depth, in minutes.
     *
     */
    uint16_t seek_to_drift_pressure_interval_duration_;

    /** The target ascent rate, in cm/s.
     *
     */
    uint8_t target_ascent_rate_;

    /** The target duration of the drift/park phase of the dive cycle, in minutes.
     *
     */
    uint16_t target_drift_duration_;

    /** The target parking or drift depth (DTD), in dBar.
     *
     */
    uint16_t target_drift_pressure_;

    /** The target profile depth (PFD), in dBar.
     *
     */
    uint16_t target_profile_pressure_;

    /* Dynamic Data Members - Sensor Gains & Offsets */

    /** The pressure sensitivity gain factor, in LSB/dBar.
     *
     *  @note           This field was added in X-Message format version 1.2.
     */
    uint16_t gain_pressure_;

    /** The pressure sensitivity gain factor for surface data, in LSB/dBar.
     *
     *  @note           This field was added in X-Message format version 2.2.
     */
    uint16_t gain_pressure_surface_;

    /** The salinity sensitivity scale factor, in LSB/PSU.
     *
     *  @note           This field was added in X-Message format version 1.2.
     */
    uint16_t gain_salinity_;

    /** The salinity sensitivity scale factor for surface data, in LSB/PSU.
     *
     *  @note           This field was added in X-Message format version 2.2.
     */
    uint16_t gain_salinity_surface_;

    /** The temperature sensitivity scale factor, in LSB/degC.
     *
     *  @note           This field was added in X-Message format version 1.2.
     */
    uint16_t gain_temperature_;

    /** The temperature sensitivity scale factor for surface data, in LSB/degC.
     *
     *  @note           This field was added in X-Message format version 2.2.
     */
    uint16_t gain_temperature_surface_;

    /** The pressure offset, in dBar.
     *
     *  @note           This field was added in X-Message format version 1.2.
     */
    uint16_t offset_pressure_;

    /** The pressure offset for surface data, in dBar.
     *
     *  @note           This field was added in X-Message format version 2.2.
     */
    uint16_t offset_pressure_surface_;

    /** The salinity offset, in PSU.
     *
     *  @note           This field was added in X-Message format version 1.2.
     */
    uint16_t offset_salinity_;

    /** The salinity offset for surface data, in PSU.
     *
     *  @note           This field was added in X-Message format version 2.2.
     */
    uint16_t offset_salinity_surface_;

    /** The temperature offset, in degC.
     *
     *  @note           This field was added in X-Message format version 1.2.
     */
    uint16_t offset_temperature_;

    /** The temperature offset for surface data, in degC.
     *
     *  @note           This field was added in X-Message format version 2.2.
     */
    uint16_t offset_temperature_surface_;

protected:

    /* Protected Member Functions: */

    /** Set the Argo instrument type identifier.
     *
     *  @param[in]  type    The Argo instrument type identifier.
     */
    void set_argo_instrument_type(XArgoInstrumentType_t type) {
        argo_instrument_type_ = type;
    }

    /** Set the start time of the ascent state of the dive cycle.
     *
     *  @param[in]  time    The start time.
     */
    void set_ascent_state_start_time(const ptime& time) {
        ascent_state_start_time_ = time;
    }

    /** Set the start time of the descent state of the dive cycle.
     *
     *  @param[in]  time    The start time.
     */
    void set_descent_state_start_time(const ptime& time) {
        descent_state_start_time_ = time;
    }

    /** Set the start time of the drift state of the dive cycle.
     *
     *  @param[in]  time    The start time.
     */
    void set_drift_state_start_time(const ptime& time) {
        drift_state_start_time_ = time;
    }

    /** Set the pressure sensor gain, in LSB/dBar.
     *
     *  @param[in]  gain    The sensor gain value.
     */
    void set_gain_pressure(uint16_t gain) {
        gain_pressure_ = gain;
    }

    /** Set the pressure sensor gain for surface data, in LSB/dBar.
     *
     *  @param[in]  gain    The sensor gain value.
     */
    void set_gain_pressure_surface(uint16_t gain) {
        gain_pressure_surface_ = gain;
    }

    /** Set the salinity sensor gain, in LSB/PSU.
     *
     *  @param[in]  gain    The sensor gain value.
     */
    void set_gain_salinity(uint16_t gain) {
        gain_salinity_ = gain;
    }

    /** Set the salinity sensor gain for surface data, in PSU/PSU.
     *
     *  @param[in]  gain    The sensor gain value.
     */
    void set_gain_salinity_surface(uint16_t gain) {
        gain_salinity_surface_ = gain;
    }

    /** Set the temperature sensor gain, in PSU/degC.
     *
     *  @param[in]  gain    The sensor gain value.
     */
    void set_gain_temperature(uint16_t gain) {
        gain_temperature_ = gain;
    }

    /** Set the temperature sensor gain for surface data, in PSU/degC.
     *
     *  @param[in]  gain    The sensor gain value.
     */
    void set_gain_temperature_surface(uint16_t gain) {
        this->gain_temperature_surface_ = gain;
    }

    /** Set the maximum duration for the ascent state of the dive cycle, in minutes.
     *
     *  @param[in]  duration    The maximum duration.
     */
    void set_max_ascent_duration(uint16_t duration) {
        max_ascent_duration_ = duration;
    }

    /** Set the maximum duration for the descent-to-drift/park-pressure state of the dive cycle, in
     *  minutes.
     *
     *  @param[in]  duration    The maximum duration.
     */
    void set_max_fall_to_drift_duration(uint16_t duration) {
        max_fall_to_drift_duration_ = duration;
    }

    /** Set the maximum duration for the descent-to-profile-pressure state of the dive cycle, in
     *  minutes.
     *
     *  @param[in]  duration    The maximum duration.
     */
    void set_max_fall_to_profle_duration(uint16_t duration) {
        max_fall_to_profle_duration_ = duration;
    }

    /** Set the maximum duration for the surface state of the dive cycle, in seconds.
     *
     *  @param[in]  duration    The maximum duration.
     */
    void set_max_surface_duration(uint16_t duration) {
        max_surface_duration_ = duration;
    }

    /** Set the mission phase number.
     *
     *  @param[in]  phase   The mission phase number.
     */
    void set_mission_phase(uint8_t phase) {
        mission_phase_ = phase;
    }

    /** Set the maximum number of seeks to drift/park pressure.
     *
     *  @param[in]  seeks   The number of seeks.
     */
    void set_num_seeks_to_drift_pressure(uint16_t seeks) {
        num_seeks_to_drift_pressure_ = seeks;
    }

    /** Set the pressure sensor offset, in dBar.
     *
     *  @param[in]  offs    The sensor offset.
     */
    void set_offset_pressure(uint16_t offs) {
        offset_pressure_ = offs;
    }

    /** Set the pressure sensor offset for surface data, in dBar.
     *
     *  @param[in]  offs    The sensor offset.
     */
    void set_offset_pressure_surface(uint16_t offs) {
        offset_pressure_surface_ = offs;
    }

    /** Set the salinity sensor offset, in PSU.
     *
     *  @param[in]  offs    The sensor offset.
     */
    void set_offset_salinity(uint16_t offs) {
        offset_salinity_ = offs;
    }

    /** Set the salinity sensor offset for surface data, in PSU.
     *
     *  @param[in]  offs    The sensor offset.
     */
    void set_offset_salinity_surface(uint16_t offs) {
        offset_salinity_surface_ = offs;
    }

    /** Set the temperature sensor offset, in degC.
     *
     *  @param[in]  offs    The sensor offset.
     */
    void set_offset_temperature(uint16_t offs) {
        offset_temperature_ = offs;
    }

    /** Set the temperature sensor offset for surface data, in degC.
     *
     *  @param[in]  offs    The sensor offset.
     */
    void set_offset_temperature_surface(uint16_t offs) {
        offset_temperature_surface_ = offs;
    }

    /** Set the seek-to-drift/park-pressure interval duration, in minutes.
     *
     *  @param[in]  duration    The interval duration.
     */
    void set_seek_to_drift_pressure_interval_duration(uint16_t duration) {
        seek_to_drift_pressure_interval_duration_ = duration;
    }

    /** Set the start time of the surface state of the dive cycle.
     *
     *  @param[in]  time    The start time.
     */
    void set_surface_state_start_time(const ptime& time) {
        surface_state_start_time_ = time;
    }

    /** Set the target ascent rate, in cm/s.
     *
     *  @param[in]  rate    The target rate.
     */
    void set_target_ascent_rate(uint8_t rate) {
        target_ascent_rate_ = rate;
    }

    /** Set the target duration of the drift/park phase of the dive cycle, in minutes.
     *
     *  @param[in]  duration    The target duration.
     */
    void set_target_drift_duration(uint16_t duration) {
        target_drift_duration_ = duration;
    }

    /** Set the target drift/park pressure, in dBar.
     *
     *  @param[in]  pres    The target pressure.
     */
    void set_target_drift_pressure(uint16_t pres) {
        target_drift_pressure_ = pres;
    }

    /** Set the target profile pressure, in dBar.
     *
     *  @param[in]  pres    The target pressure.
     */
    void set_target_profile_pressure(uint16_t pres) {
        target_profile_pressure_ = pres;
    }

    /** Set the X-Message major version identifier.
     *
     *  @param[in]  vers    The version.
     */
    void set_x_version_major(uint8_t vers) {
        x_version_major_ = vers;
    }

    /** Set the X-Message minor version identifier.
     *
     *  @param[in]  vers    The version.
     */
    void set_x_version_minor(uint8_t vers) {
        x_version_minor_ = vers;
    }

public:

    /* Public Member Functions: */

    /** The constructor.
     *
     *  @param[in]  dive    The dive or cycle number of this message.
     *  @param[in]  whoi    The unique WHOI identifier.
     *  @param[in]  data    The binary Argo data X-message data to decode.
     *
     *  @exception  util::log::MessageException if an decode error occurs.
     */
    XArgo(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                    XMessage(dive, whoi, data),
                    argo_instrument_type_(XARGO_INSRUMENT_TYPE_UNKNOWN),
                    x_version_major_(XMSG_ENG_BAD_VER),
                    x_version_minor_(XMSG_ENG_BAD_VER),
                    // state start times
                    ascent_state_start_time_(bpt::not_a_date_time),
                    descent_state_start_time_(bpt::not_a_date_time),
                    drift_state_start_time_(bpt::not_a_date_time),
                    surface_state_start_time_(bpt::not_a_date_time),
                    // dive cycle characteristics
                    max_fall_to_drift_duration_(XMSG_BAD_U16),
                    max_fall_to_profle_duration_(XMSG_BAD_U16),
                    max_ascent_duration_(XMSG_BAD_U16),
                    max_surface_duration_(XMSG_BAD_U16),
                    mission_phase_(XMSG_BAD_U08),
                    num_seeks_to_drift_pressure_(XMSG_BAD_U08),
                    seek_to_drift_pressure_interval_duration_(XMSG_BAD_U16),
                    target_ascent_rate_(XMSG_BAD_U08),
                    target_drift_duration_(XMSG_BAD_U16),
                    target_drift_pressure_(XMSG_BAD_U16),
                    // sensor gains and offset
                    gain_pressure_(XMSG_GAIN_INVALID),
                    gain_pressure_surface_(XMSG_GAIN_INVALID),
                    gain_salinity_(XMSG_GAIN_INVALID),
                    gain_salinity_surface_(XMSG_GAIN_INVALID),
                    gain_temperature_(XMSG_GAIN_INVALID),
                    gain_temperature_surface_(XMSG_GAIN_INVALID),
                    offset_pressure_(XMSG_OFFS_INVALID),
                    offset_pressure_surface_(XMSG_OFFS_INVALID),
                    offset_salinity_(XMSG_OFFS_INVALID),
                    offset_salinity_surface_(XMSG_OFFS_INVALID),
                    offset_temperature_(XMSG_OFFS_INVALID),
                    offset_temperature_surface_(XMSG_OFFS_INVALID) {
        // Decode the Argo data message.
        DecodeXMsgData(data);
    }

    /** The destructor.
     *
     */
    ~XArgo() {
        /* Do nothing. */
    }

    /** Get the Argo instrument type identifier.
     *
     *  @return     The Argo instrument type identifier.
     */
    XArgoInstrumentType_t argo_instrument_type() const {
        return argo_instrument_type_;
    }

    /** Get the start time of the ascent state of the dive cycle.
     *
     *  @return     The start time of the ascent state of the dive cycle.
     */
    const ptime& ascent_state_start_time() const {
        return ascent_state_start_time_;
    }

    /** Get the start time of the descent state of the dive cycle.
     *
     *  @return     The start time of the descent state of the dive cycle.
     */
    const ptime& descent_state_start_time() const {
        return descent_state_start_time_;
    }

    /** Decode the Argo X message.
     *
     *  @param[in]  data        The data to decode.
     *
     *  @exception  util::log::MessageException if an error occurs while decoding the message.
     */
    void DecodeXMsgData(const vector<uint8_t>& data) {
        try {
            // Check for valid X message ID, minimum size, & message terminator.
            const uint8_t* d = CheckXMsgData(data, (int) 0xF0, 23);

            // Get X message version.
            set_x_version_major((d[3] & 0xF0) >> 4);
            set_x_version_minor((d[3] & 0x0F));

            // Get depths.
            set_target_profile_pressure(u16(d, 4));
            set_target_drift_pressure(u16(d, 6));

            /* Version-Specific Processing: */

            /** @todo   Implement the functionality to decode the argo data message for Solo-II
             *          floats.
             *
             */

            /** @todo   Implement the functionality to decode the argo data message for Deep Argo
             *          floats.
             *
             */

            switch(x_version_major()) {
            case 2:

                /* X-Message Version 2.x */

                switch(x_version_minor()) {
                case 1:

                    /* X-Message Version 2.1 */

                    if(xmsg_size_bytes() < 40) {
                        ostringstream os;
                        os << "Insufficient message length: expected 40 bytes, but received "
                           << (int) xmsg_size_bytes()
                           << " bytes!";
                        throw util::log::MessageException(os);
                    }

                    if(d[24] == XARGO_INSRUMENT_TYPE_ALAMO) {

                        /* ALAMO-Specific Processing: */

                        set_argo_instrument_type(XARGO_INSRUMENT_TYPE_ALAMO);

                        // Get times.
                        set_descent_state_start_time(s2000(d, 8));
                        set_drift_state_start_time(s2000(d, 12));
                        set_ascent_state_start_time(s2000(d, 16));
                        set_surface_state_start_time(s2000(d, 20));

                        // Target ascent rate.
                        set_target_ascent_rate(u08(d, 25));

                        // Gains & offsets.
                        set_gain_pressure(u16(d, 26));
                        set_offset_pressure(u16(d, 28));
                        set_gain_temperature(u16(d, 30));
                        set_offset_temperature(u16(d, 32));
                        set_gain_salinity(u16(d, 34));
                        set_offset_salinity(u16(d, 36));

                        set_gain_pressure_surface(gain_pressure());
                        set_offset_pressure_surface(offset_pressure());
                        set_gain_temperature_surface(gain_temperature());
                        set_offset_temperature_surface(offset_temperature());
                        set_gain_salinity_surface(gain_salinity());
                        set_offset_salinity_surface(offset_salinity_surface());
                    }

                    break;

                case 2:

                    /* X-Message Version 2.2 */

                    if(xmsg_size_bytes() < 66) {
                        ostringstream os;
                        os << "Insufficient message length: expected 66 bytes, but received "
                           << (int) xmsg_size_bytes()
                           << " bytes!";
                        throw util::log::MessageException(os);
                    }

                    if(d[32] == XARGO_INSRUMENT_TYPE_ALAMO) {

                        /* ALAMO-Specific Processing: */

                        set_argo_instrument_type(XARGO_INSRUMENT_TYPE_ALAMO);

                        // Get times.
                        set_descent_state_start_time(s2000(d, 16));
                        set_drift_state_start_time(s2000(d, 20));
                        set_ascent_state_start_time(s2000(d, 24));
                        set_surface_state_start_time(s2000(d, 28));

                        // Get dive cycle characteristics.
                        set_max_ascent_duration(u16(d, 8));
                        set_max_fall_to_drift_duration(u16(d, 10));
                        set_max_fall_to_profle_duration(u16(d, 12));
                        set_target_drift_duration(u16(d, 14));
                        set_target_ascent_rate(u08(d, 33));
                        set_num_seeks_to_drift_pressure(u08(d, 34));
                        set_max_surface_duration(u16(d, 36));
                        set_seek_to_drift_pressure_interval_duration(u16(d, 38));
                        set_mission_phase(u08(d, 64));

                        // Set sensor gains and offsets.
                        set_gain_pressure(u16(d, 40));
                        set_offset_pressure(u16(d, 42));
                        set_gain_temperature(u16(d, 44));
                        set_offset_temperature(u16(d, 46));
                        set_gain_salinity(u16(d, 48));
                        set_offset_salinity(u16(d, 50));
                        set_gain_pressure_surface(u16(d, 52));
                        set_offset_pressure_surface(u16(d, 54));
                        set_gain_temperature_surface(u16(d, 56));
                        set_offset_temperature_surface(u16(d, 58));
                        set_gain_salinity_surface(u16(d, 60));
                        set_offset_salinity_surface(u16(d, 62));

                    }

                    break;

                case 3:

                    /* X-Message Version 2.3 */

                    if(xmsg_size_bytes() < 66) {
                        ostringstream os;
                        os << "Insufficient message length: expected 66 bytes, but received "
                           << (int) xmsg_size_bytes()
                           << " bytes!";
                        throw util::log::MessageException(os);
                    }

                    if(d[32] == XARGO_INSRUMENT_TYPE_ALAMO) {

                        /* ALAMO-Specific Processing: */

                        set_argo_instrument_type(XARGO_INSRUMENT_TYPE_ALAMO);

                        // Get times.
                        set_descent_state_start_time(s2000(d, 16));
                        set_drift_state_start_time(s2000(d, 20));
                        set_ascent_state_start_time(s2000(d, 24));
                        set_surface_state_start_time(s2000(d, 28));

                        // Get dive cycle characteristics.
                        set_max_ascent_duration(u16(d, 8));
                        set_max_fall_to_drift_duration(u16(d, 10));
                        set_max_fall_to_profle_duration(u16(d, 12));
                        set_target_drift_duration(u16(d, 14));
                        set_target_ascent_rate(u08(d, 33));
                        set_num_seeks_to_drift_pressure(u08(d, 34));
                        set_max_surface_duration(u16(d, 36));
                        set_seek_to_drift_pressure_interval_duration(u16(d, 38));
                        set_mission_phase(u08(d, 64));

                        // Set sensor gains and offsets.
                        set_gain_pressure(u16(d, 40));
                        set_offset_pressure(u16(d, 42));
                        set_gain_temperature(u16(d, 44));
                        set_offset_temperature(u16(d, 46));
                        set_gain_salinity(u16(d, 48));
                        set_offset_salinity(u16(d, 50));
                        set_gain_pressure_surface(u16(d, 52));
                        set_offset_pressure_surface(u16(d, 54));
                        set_gain_temperature_surface(u16(d, 56));
                        set_offset_temperature_surface(u16(d, 58));
                        set_gain_salinity_surface(u16(d, 60));
                        set_offset_salinity_surface(u16(d, 62));

                    }

                    break;

                case 4:

                    /* X-Message Version 2.4 */

                    if(xmsg_size_bytes() != 66) {
                        ostringstream os;
                        os << "Insufficient message length: expected 66 bytes, but received "
                           << (int) xmsg_size_bytes()
                           << " bytes!";
                        throw util::log::MessageException(os);
                    }

                    if(d[32] == XARGO_INSRUMENT_TYPE_ALAMO) {

                        /* ALAMO-Specific Processing: */

                        set_argo_instrument_type(XARGO_INSRUMENT_TYPE_ALAMO);

                        // Get times.
                        set_descent_state_start_time(s2000(d, 16));
                        set_drift_state_start_time(s2000(d, 20));
                        set_ascent_state_start_time(s2000(d, 24));
                        set_surface_state_start_time(s2000(d, 28));

                        // Get dive cycle characteristics.
                        set_max_ascent_duration(u16(d, 8));
                        set_max_fall_to_drift_duration(u16(d, 10));
                        set_max_fall_to_profle_duration(u16(d, 12));
                        set_target_drift_duration(u16(d, 14));
                        set_target_ascent_rate(u08(d, 33));
                        set_num_seeks_to_drift_pressure(u08(d, 34));
                        set_max_surface_duration(u16(d, 36));
                        set_seek_to_drift_pressure_interval_duration(u16(d, 38));
                        set_mission_phase(u08(d, 64));

                        // Set sensor gains and offsets.
                        set_gain_pressure(u16(d, 40));
                        set_offset_pressure(u16(d, 42));
                        set_gain_temperature(u16(d, 44));
                        set_offset_temperature(u16(d, 46));
                        set_gain_salinity(u16(d, 48));
                        set_offset_salinity(u16(d, 50));
                        set_gain_pressure_surface(u16(d, 52));
                        set_offset_pressure_surface(u16(d, 54));
                        set_gain_temperature_surface(u16(d, 56));
                        set_offset_temperature_surface(u16(d, 58));
                        set_gain_salinity_surface(u16(d, 60));
                        set_offset_salinity_surface(u16(d, 62));

                    }

                    break;

                default:
                    throw util::log::MessageException("Encountered unsupported X-Message format!");
                    break;
                }

                break;

            default:
                throw util::log::MessageException("Encountered unsupported X-Message format!");
                break;
            }

        }CATCH_BLOCK("Failed to decode Argo Data X-message!")
    }

    /** Get the start time of the drift/park state of the dive cycle.
     *
     *  @return     The start time of the drift/park state of the dive cycle.
     */
    const ptime& drift_state_start_time() const {
        return drift_state_start_time_;
    }

    /** Get the pressure sensor gain value, in LSB/dBar.
     *
     *  @return     The pressure sensor gain value, in LSB/dBar.
     */
    uint16_t gain_pressure() const {
        return gain_pressure_;
    }

    /** Get the pressure sensor gain value for surface data, in LSB/dBar.
     *
     *  @return     The pressure sensor gain value, in LSB/dBar.
     */
    uint16_t gain_pressure_surface() const {
        return gain_pressure_surface_;
    }

    /** Get the salinity sensor gain value, in LSB/PSU.
     *
     *  @return     The salinity sensor gain value, in LSB/PSU.
     */
    uint16_t gain_salinity() const {
        return gain_salinity_;
    }

    /** Get the salinity sensor gain value for surface data, in LSB/PSU.
     *
     *  @return     The salinity sensor gain value, in LSB/PSU.
     */
    uint16_t gain_salinity_surface() const {
        return gain_salinity_surface_;
    }

    /** Get the temperature sensor gain value, in LSB/degC.
     *
     *  @return     The temperature sensor gain value, in LSB/degC.
     */
    uint16_t gain_temperature() const {
        return gain_temperature_;
    }

    /** Get the temperature sensor gain value for surface data, in LSB/degC.
     *
     *  @return     The temperature sensor gain value, in LSB/degC.
     */
    uint16_t gain_temperature_surface() const {
        return gain_temperature_surface_;
    }

    /** Generate a listing entry string representation of this Argo data message.
     *
     *  @return         A listing entry string representation of this Argo data message.
     */
    const string GenerateListingString() const {
        using namespace std;
        using boost::format;
        ostringstream os;

        try {
            switch(argo_instrument_type()) {
            case XARGO_INSRUMENT_TYPE_SOLO2:

                /** @todo   Implement the functionality for generating a listing string for a
                 *          Solo-II-type Argo data message.
                 */

                break;

            case XARGO_INSRUMENT_TYPE_DEEP_ARGO:

                /** @todo   Implement the functionality for generating a listing string for a
                 *          Deep-Argo-type Argo data message.
                 */

                break;

            case XARGO_INSRUMENT_TYPE_ALAMO: { // Generate listing string for ALAMO

                { /* Argo Data Message Listing - Minor Index 0: */
                    const format fmt(
//                            "#   %5s %4s %4s %3s %4s %10s %9s %9s %9s %14s %14s\n"
//                            "#   %5s %4s %4s %3s %4s %10s %9s %9s %9s %14s %14s\n"
                            "A   %5d %4d %4.1f %3d %4d %10d %9d %9d %9d %14d %14d");
                    os << "#      SN dive xVer typ midx  missPhase  profPres  parkPres   maxAscT  maxDesc2ParkT  maxDesc2ProfT" << endl
                       << "#                                          (dBar)    (dBar)     (min)          (min)          (sec)" << endl
                       << format(fmt)
//                            % "SN" % "dive" % "xVer" % "typ" % "midx" % "missPhase"
//                            % "profPres" % "parkPres" % "maxAscT" % "maxDesc2ParkT"
//                            % "maxDesc2ProfT" % "" % "" % "" % "" % "" % "" % "(dBar)" % "(dBar)"
//                            % "(min)" % "(min)" % "(sec)"
                            % id_instrument() % id_dive() % x_version_float()
                            % (int) argo_instrument_type() % 0 % (int) mission_phase()
                            % target_profile_pressure() % target_drift_pressure()
                            % max_ascent_duration() % max_fall_to_drift_duration()
                            % max_fall_to_profle_duration()
                       << endl << "#" << endl;
                }
                { /* Argo Data Message Listing - Minor Index 1: */
                    const format fmt(
//                            "#   %5s %4s %4s %3s %4s %10s %7s %14s %11s %9s\n"
//                            "#   %5s %4s %4s %3s %4s %10s %7s %14s %11s %9s\n"
                            "A   %5d %4d %4.1f %3d %4d %10d %7d %14d %11d %9d");
                    os << "#      SN dive xVer typ midx   tarParkT   nSeek  seekIntervalT  tarAscRate  maxSurfT" << endl
                       << "#                                                        (min)    (cm/sec)     (sec)" << endl
                       << format(fmt)
//                            % "SN" % "dive" % "xVer" % "typ" % "midx" % "tarParkT"
//                            % "nSeek" % "seekIntervalT" % "tarAscRate" % "maxSurfT" % "" % "" % ""
//                            % "" % "" % "" % "" % "(min)" % "(cm/sec)" % "(sec)"
                            % id_instrument() % id_dive() % x_version_float()
                            % (int) argo_instrument_type() % 1 % target_drift_duration()
                            % (int) num_seeks_to_drift_pressure()
                            % seek_to_drift_pressure_interval_duration()
                            % (int) target_ascent_rate() % max_surface_duration()
                       << endl << "#" << endl;
                }
                { /* Argo Data Message Listing - Minor Index 2: */
                    const format fmt(
//                            "#   %5s %4s %4s %3s %4s %24s %24s\n"
                            "A   %5d %4d %4.1f %3d %4d %24s %24s");
                    os << "#      SN dive xVer typ midx         start-of-descent           start-of-drift" << endl
                       << format(fmt)
//                            % "SN" % "dive" % "xVer" % "typ" % "midx" % "start-of-descent"
//                            % "start-of-drift"
                            % id_instrument() % id_dive() % x_version_float()
                            % (int) argo_instrument_type() % 2
                            % GenerateIso8601String(descent_state_start_time())
                            % GenerateIso8601String(drift_state_start_time())
                       << endl << "#" << endl;
                }
                { /* Argo Data Message Listing - Minor Index 3: */
                    const format fmt(
//                            "#   %5s %4s %4s %3s %4s %24s %24s\n"
                            "A   %5d %4d %4.1f %3d %4d %24s %24s");
                    os << "#      SN dive xVer typ midx          start-of-ascent         start-of-surface" << endl
                       << format(fmt)
//                            % "SN" % "dive" % "xVer" % "typ" % "midx" % "start-of-ascent"
//                            % "start-of-surface"
                            % id_instrument() % id_dive() % x_version_float()
                            % (int) argo_instrument_type() % 3
                            % GenerateIso8601String(ascent_state_start_time())
                            % GenerateIso8601String(surface_state_start_time())
                       << endl << "#" << endl;
                }
                { /* Argo Data Message Listing - Minor Index 4: */
                    const format fmt(
//                            "#   %5s %4s %4s %3s %4s %11s %11s %11s %11s %11s %11s\n"
//                            "#   %5s %4s %4s %3s %4s %11s %11s %12s %12s %11s %11s\n"
                            "A   %5d %4d %4.1f %3d %4d %11s %11s %11s %11s %11s %10s");
                    os << "#      SN dive xVer typ midx       pGain        pOff       tGain        tOff       sGain       sOff" << endl
                       << "#                               (1/dBar)      (dBar)    (1/degC)      (degC)     (1/PSU)      (PSU)" << endl
                       << format(fmt)
//                            % "SN" % "dive" % "xVer" % "typ" % "midx" % "pGain" % "pOff"
//                            % "tGain" % "tOff" % "sGain" % "sOff" % "" % "" % "" % "" % ""
//                            % "(1/dBar)" % "(dBar)" % "(1/degC)" % "(degC)" % "(1/PSU)" % "(PSU)"
                            % id_instrument() % id_dive() % x_version_float()
                            % (int) argo_instrument_type() % 4 % gain_pressure() % offset_pressure()
                            % gain_temperature() % offset_temperature() % gain_salinity()
                            % offset_salinity()
                       << endl << "#" << endl;
                }
                { /* Argo Data Message Listing - Minor Index 5: */
                    const format fmt(
//                            "#   %5s %4s %4s %3s %4s %11s %11s %11s %11s %11s %11s\n"
//                            "#   %5s %4s %4s %3s %4s %11s %11s %12s %12s %11s %11s\n"
                            "A   %5d %4d %4.1f %3d %4d %11d %11d %11d %11d %11d %10d");
                    os << "#      SN dive xVer typ midx   pGainSurf    pOffSurf   tGainSurf    tOffSurf   sGainSurf   sOffSurf" << endl
                       << "#                               (1/dBar)      (dBar)    (1/degC)      (degC)     (1/PSU)      (PSU)" << endl
                       << format(fmt)
//                            % "SN" % "dive" % "xVer" % "typ" % "midx" % "pGainSurf"
//                            % "pOffSurf" % "tGainSurf" % "tOffSurf" % "sGainSurf" % "sOffSurf" % ""
//                            % "" % "" % "" % "" % "(1/dBar)" % "(dBar)" % "(1/degC)" % "(degC)"
//                            % "(1/PSU)" % "(PSU)"
                            % id_instrument() % id_dive() % x_version_float()
                            % (int) argo_instrument_type() % 5 % gain_pressure_surface()
                            % offset_pressure_surface() % gain_temperature_surface()
                            % offset_temperature_surface() % gain_salinity_surface()
                            % offset_salinity_surface();
                }
            }

                break;
            case XARGO_INSRUMENT_TYPE_UNKNOWN:
            default:
                os << "A (unknown float type)";
                break;
            }
        }CATCH_BLOCK("Failed to generate Argo data listing string!")

        return os.str();
    }

    /** Get the maximum duration of the ascent state of the dive cycle, in minutes.
     *
     *  @return     The maximum duration of the ascent state of the dive cycle, in minutes.
     */
    uint16_t max_ascent_duration() const {
        return max_ascent_duration_;
    }

    /** Get the maximum duration of the descent-to-drift/park-pressure phase of the dive cycle.
     *
     *  @return     The maximum duration of the descent-to-drift/park-pressure phase of the dive
     *              cycle.
     */
    uint16_t max_fall_to_drift_duration() const {
        return max_fall_to_drift_duration_;
    }

    /** Get the maximum duration of the descent-to-profile-pressure phase of the dive cycle, in
     *  seconds.
     *
     *  @return     The maximum duration of the descent-to-profile-pressure phase of the dive
     *              cycle, in seconds.
     */
    uint16_t max_fall_to_profle_duration() const {
        return max_fall_to_profle_duration_;
    }

    /** Get the maximum duration of the surface state of the dive cycle, in seconds.
     *
     *  @return     The maximum duration of the surface state of the dive cycle, in seconds.
     */
    uint16_t max_surface_duration() const {
        return max_surface_duration_;
    }

    /** Get the mission phase.
     *
     *  @return     The mission phase.
     */
    uint8_t mission_phase() const {
        return mission_phase_;
    }

    /** Get the maximum number of seeks to drift/park pressure.
     *
     *  @return     The maximum number of seeks to drift/park pressure.
     */
    uint16_t num_seeks_to_drift_pressure() const {
        return num_seeks_to_drift_pressure_;
    }

    /** Get the pressure sensor offset, in dBar.
     *
     *  @return     The pressure sensor offset, in dBar.
     */
    uint16_t offset_pressure() const {
        return offset_pressure_;
    }

    /** Get the pressure sensor offset for surface data, in dBar.
     *
     *  @return     The pressure sensor offset, in dBar.
     */
    uint16_t offset_pressure_surface() const {
        return offset_pressure_surface_;
    }

    /** Get the salinity sensor offset, in PSU.
     *
     *  @return     The salinity sensor offset, in PSU.
     */
    uint16_t offset_salinity() const {
        return offset_salinity_;
    }

    /** Get the salinity sensor offset for surface data, in PSU.
     *
     *  @return     The salinity sensor offset, in PSU.
     */
    uint16_t offset_salinity_surface() const {
        return offset_salinity_surface_;
    }

    /** Get the temperature sensor offset, in degC.
     *
     *  @return     The temperature sensor offset, in degC.
     */
    uint16_t offset_temperature() const {
        return offset_temperature_;
    }

    /** Get the temperature sensor offset for surface data, in degC.
     *
     *  @return     The temperature sensor offset, in degC.
     */
    uint16_t offset_temperature_surface() const {
        return offset_temperature_surface_;
    }

    /** Get the seek-to-drift/park-pressure interval duration, in minutes.
     *
     *  @return     The seek-to-drift/park-pressure interval duration, in minutes.
     */
    uint16_t seek_to_drift_pressure_interval_duration() const {
        return seek_to_drift_pressure_interval_duration_;
    }

    /** Get the start time of the surface state of the dive cycle.
     *
     *  @return     The start time of the surface state of the dive cycle.
     */
    const ptime& surface_state_start_time() const {
        return surface_state_start_time_;
    }

    /** Get the target ascent/profile rate, in cm/sec.
     *
     *  @return     The target ascent/profile rate, in cm/sec.
     */
    uint8_t target_ascent_rate() const {
        return target_ascent_rate_;
    }

    /** Get the target druration of the drift/park phase of the dive cycle, in minutes.
     *
     *  @return     The target druration of the drift/park phase of the dive cycle, in minutes.
     */
    uint16_t target_drift_duration() const {
        return target_drift_duration_;
    }

    /** Get the target drift/park pressure, in dBar.
     *
     *  @return     The target drift/park pressure, in dBar.
     */
    uint16_t target_drift_pressure() const {
        return target_drift_pressure_;
    }

    /** Get the target profile pressure, in dBar.
     *
     *  @return     The target profile pressure, in dBar.
     */
    uint16_t target_profile_pressure() const {
        return target_profile_pressure_;
    }

    /** Get the X-Message major version indicator.
     *
     *  @return     The X-Message major version indicator.
     */
    uint8_t x_version_major() const {
        return x_version_major_;
    }

    /** Get the X-Message minor version indicator.
     *
     *  @return     The X-Message minor version indicator.
     */
    uint8_t x_version_minor() const {
        return x_version_minor_;
    }

    /** Get the X-Message version as a floating-point value.
     *
     *  @return     The X-Message version as a floating-point value.
     */
    float x_version_float() const {
        float ver = x_version_minor();

        do
            ver /= 10;
        while(ver > 1);

        return (float) x_version_major() + ver;
    }

};

} /* namespace xmsg */
} /* namespace util */
#endif /* XARGO_H_ */
