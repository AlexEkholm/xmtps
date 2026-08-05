/*
 * XAlamoBit2.h
 *
 *  Created on: Apr 20, 2023
 *      Author: aekholm
 */

#ifndef DATA_XMSG_XALAMOBIT2_H_
#define DATA_XMSG_XALAMOBIT2_H_

/* Includes */

#include "XMessage.h"

namespace util {
namespace xmsg {

/* Type Definitions */


typedef enum XAlamoBIT2Type {
    UNKNOWN = -1,
    NO_SKY_VIEW = 0,
    SKY_VIEW= 1,
} XAlamoBIT2Type_t;

/* Class Definitions */

class XAlamoBit2:
    public util::date::DateUtils {

  private:

    /* BIT Header Information */

    /** The software version identifier.
     *
     */
    uint32_t float_software_version_;


    /** The Built-in-Test (BIT) version identifier.
     *
     */
    uint8_t bit_version_;


    /* BIT Time Fields */

    /** BIT Start time.
     *
     */
    ptime dt_start_;

    /** BIT start time, in seconds since 1970-01-01T00:00:00Z.
     *
     */
    uint32_t dt_start_epoch_;

    /** BIT End time.
     *
     */
    ptime dt_end_;

    /** BIT End time, in seconds since 1970-01-01T00:00:00Z.
     *
     */
    uint32_t dt_end_epoch_;

    /** BIT duration in seconds.
     *
     */
    int32_t duration_sec_;

    /* BIT Results */

    /** The BIT passed flag.
     *
     */
    bool bit_passed_;

    /** The Built-in-Test (BIT) status code.
     *
     */
    uint32_t bit_status_code_;

    /* BIT Results - Enumerations and Bitmasks */

    /** BIT test type enum.
     * 
     */
    uint8_t type_;
    
    /** Sensor type enum.
     *  
     */
    uint8_t sensor_type_;
    
    /** Air pump errors bitmask.
     * 
     */
    uint32_t air_pump_errors_;
    
    /** Oil pump errors bitmask.
     * 
     */
    uint32_t oil_pump_errors_;
    
    /** Air valve errors bitmask.
     * 
     */
    uint32_t air_valve_errors_;
        
    
    /** Oil valve errors bitmask.
     * 
     */
    uint32_t oil_valve_errors_;
        
    /** GPS errors bitmask.
     * 
     */
    uint32_t gps_errors_;
        
    uint32_t ctd_errors_;
        
    uint32_t iridum_errors_;
        
    uint32_t case_pres_errors_;
        
    uint32_t bat_errors_;

    uint32_t traveler_errors_;


    /* BIT Results - CTD Fields */

    float pres_before_cal_;

    float pres_offset_;

    float pres_after_cal_;

    float temp_;

    float saln_;

    uint32_t ctd_status_;
        

    /* BIT Results - Case Vac */
    
    /** Case vac at start and end of BIT, in inHg.
     *
     */
    float case_vac_start_, case_vac_end_;

    /* BIT Results - Battery */

    /** Battery voltages at start and end of BIT, in volts.
     *
     */
    float bat_volt_start_, bat_volt_end_;

    /* BIT Results - Air valve */

    /** Air valve open, closed, and middle calibration positions, in volts.
     *
     */
    float air_valve_cal_open_, air_valve_cal_closed_, air_valve_cal_mid_;

    /** Air valve open, closed, and middle positions, in volts.
     *
     */
    float air_valve_open_, air_valve_closed_, air_valve_mid_;

    /* BIT Results - Air pump */

    /** Air pump test duration, in seconds.
     *
     */
    uint16_t air_pump_test_duration_;
    
    /** Air pump cycle count.
     *
     */
    uint8_t air_pump_cycle_count_;
    
    /** Max. voltage during air pump test, in volts.
     *
     */
    float air_pump_volt_max_;

    /** Max. 12V rail current during air pump test, in amps.
     *
     */
    float air_pump_v12_curr_max_;

    /** Max. and mean current during air pump test, in amps.
     *
     */
    float air_pump_curr_max_, air_pump_curr_avg_;

    float air_pump_pres_max_;

   
    /* BIT Results - Oil valve */

    /** Oil valve open and clased calibration positions, in volts.
     *
     */
    float oil_valve_cal_open_, oil_valve_cal_closed_;
    
    /** Oil valve open and closed positions, in volts.
     *
     */
    float oil_valve_open_, oil_valve_closed_;

    /* BIT Results - Oil pump */
        
    /** Oil pump test duration, in seconds.
     *
     */
    uint16_t oil_pump_test_duration_;

    /** Max. and mean oil pump current during test, in amps.
     *
     */
    float oil_pump_curr_max_, oil_pump_curr_avg_;
    
    /** Max 12V rail current during oil pump test, in amps.
     *
     */
    float oil_pump_v12_curr_max_;
    
    /** Max battery voltage during oil pump test, in volts.
     *
     */
    float oil_pump_volt_max_;
    
  protected:

    /* Mutator Methods */

    /** Set the BIT passed flag.
     *
     *  @param[in]  bit_passed          BIT passed flag to set.
     */
    void set_bit_passed(bool bit_passed) {
      bit_passed_ = bit_passed;
    }

    /** Set the Built-in-Test (BIT) status code.
     *
     *  @param[in]  bit_status_code     The Built-in-Test (BIT) status code to set.
     */
    void set_bit_status_code(uint32_t bit_status_code) {
      bit_status_code_ = bit_status_code;
    }

    /** Set the Built-in-Test (BIT) version.
     *
     *  @param[in]  bit_version         The Built-in-Test (BIT) version to set.
     */
    void set_bit_version(uint8_t bit_version) {
      bit_version_ = bit_version;
    }

    /** Set the instrument software version.
     *
     *  @param[in]  float_software_version  The instrument software version to set.
     */
    void set_float_software_version(uint32_t float_software_version) {
      float_software_version_ = float_software_version;
    }

    void set_duration_sec(int32_t duration_sec) {
      duration_sec_ = duration_sec;
    }

    void set_dt_start(const ptime &dt_start) {
      dt_start_ = dt_start;
    }

    void set_dt_end(const ptime &dt_end) {
      dt_end_ = dt_end;
    }

    void set_air_pump_errors(uint32_t air_pump_errors) {
      air_pump_errors_ = air_pump_errors;
    }
    
    void set_air_valve_errors(uint32_t air_valve_errors) {
      air_valve_errors_ = air_valve_errors;
    }
    
    void set_bat_errors(uint32_t bat_errors) {
      bat_errors_ = bat_errors;
    }
    
    void set_case_pres_errors(uint32_t case_pres_errors) {
      case_pres_errors_ = case_pres_errors;
    }
    
    void set_ctd_errors(uint32_t ctd_errors) {
      ctd_errors_ = ctd_errors;
    }
    
    void set_sensor_type(uint8_t sensor_type) {
      sensor_type_ = sensor_type;
    }
    
    void set_gps_errors(uint32_t gps_errors) {
      gps_errors_ = gps_errors;
    }
    
    void set_iridum_errors(uint32_t iridum_errors) {
      iridum_errors_ = iridum_errors;
    }
    
    void set_oil_pump_errors(uint32_t oil_pump_errors) {
      oil_pump_errors_ = oil_pump_errors;
    }
    
    void set_oil_valve_errors(uint32_t oil_valve_errors) {
      oil_valve_errors_ = oil_valve_errors;
    }
    
    void set_type(uint8_t type) {
      type_ = type;
    }

    void set_dt_end_epoch(uint32_t dt_end_epoch) {
      dt_end_epoch_ = dt_end_epoch;
    }

    void set_dt_start_epoch(uint32_t dt_start_epoch) {
      dt_start_epoch_ = dt_start_epoch;
    }

    void set_ctd_status(uint32_t ctd_status) {
      ctd_status_ = ctd_status;
    }

    void set_pres_after_cal(float pres_after_cal) {
      pres_after_cal_ = pres_after_cal;
    }

    void set_pres_before_cal(float pres_before_cal) {
      pres_before_cal_ = pres_before_cal;
    }

    void set_pres_offset(float pres_offset) {
      pres_offset_ = pres_offset;
    }

    void set_saln(float saln) {
      saln_ = saln;
    }

    void set_temp(float temp) {
      temp_ = temp;
    }

    void set_bat_volt_end(float bat_volt_end) {
      bat_volt_end_ = bat_volt_end;
    }

    void set_bat_volt_start(float bat_volt_start) {
      bat_volt_start_ = bat_volt_start;
    }

    void set_case_vac_end(float case_vac_end) {
      case_vac_end_ = case_vac_end;
    }

    void set_case_vac_start(float case_vac_start) {
      case_vac_start_ = case_vac_start;
    }

    void set_air_pump_cycle_count(uint8_t air_pump_cycle_count) {
      air_pump_cycle_count_ = air_pump_cycle_count;
    }

    void set_air_pump_v12_curr_max(float air_pump_v12_curr_max) {
      air_pump_v12_curr_max_ = air_pump_v12_curr_max;
    }

    void set_air_pump_volt_max(float air_pump_volt_max) {
      air_pump_volt_max_ = air_pump_volt_max;
    }

    void set_air_pump_test_duration(uint16_t air_pump_test_duration) {
      air_pump_test_duration_ = air_pump_test_duration;
    }

    void set_air_valve_cal_closed(float air_valve_cal_closed) {
      air_valve_cal_closed_ = air_valve_cal_closed;
    }

    void set_air_valve_cal_mid(float air_valve_cal_mid) {
      air_valve_cal_mid_ = air_valve_cal_mid;
    }

    void set_air_valve_cal_open(float air_valve_cal_open) {
      air_valve_cal_open_ = air_valve_cal_open;
    }

    void set_air_valve_closed(float air_valve_closed) {
      air_valve_closed_ = air_valve_closed;
    }

    void set_air_valve_mid(float air_valve_mid) {
      air_valve_mid_ = air_valve_mid;
    }

    void set_air_valve_open(float air_valve_open) {
      air_valve_open_ = air_valve_open;
    }

    void set_oil_pump_curr_avg(float oil_pump_curr_avg) {
      oil_pump_curr_avg_ = oil_pump_curr_avg;
    }

    void set_oil_pump_curr_max(float oil_pump_curr_max) {
      oil_pump_curr_max_ = oil_pump_curr_max;
    }

    void set_oil_pump_volt_max(float oil_pump_volt_max) {
      oil_pump_volt_max_ = oil_pump_volt_max;
    }

    void set_oil_pump_test_duration(uint16_t oil_pump_test_duration) {
      oil_pump_test_duration_ = oil_pump_test_duration;
    }

    void set_oil_pump_v12_curr_max(float oil_pump_v12_curr_max) {
      oil_pump_v12_curr_max_ = oil_pump_v12_curr_max;
    }

    void set_oil_valve_cal_closed(float oil_valve_cal_closed) {
      oil_valve_cal_closed_ = oil_valve_cal_closed;
    }

    void set_oil_valve_cal_open(float oil_valve_cal_open) {
      oil_valve_cal_open_ = oil_valve_cal_open;
    }

    void set_oil_valve_closed(float oil_valve_closed) {
      oil_valve_closed_ = oil_valve_closed;
    }

    void set_oil_valve_open(float oil_valve_open) {
      oil_valve_open_ = oil_valve_open;
    }


    void set_air_pump_pres_max(float air_pump_pres_max) {
      air_pump_pres_max_ = air_pump_pres_max;
    }

    void set_air_pump_curr_max(float air_pump_curr_max) {
      air_pump_curr_max_ = air_pump_curr_max;
    }

    void set_air_pump_curr_avg(float air_pump_curr_avg) {
      air_pump_curr_avg_ = air_pump_curr_avg;
    }

    void set_traveler_errors(uint32_t travelerErrors) {
      traveler_errors_ = travelerErrors;
    }

  public:

    /* Accessor Methods */

    /** Get the Built-in-Test (BIT) version.
     *
     *  @return         The Built-in-Test (BIT) version.
     */
    uint8_t bit_version() const {
        return bit_version_;
    }

    /** Get the instrument software version.
     *
     *  @return         The instrument software version.
     */
    uint32_t float_software_version() const {
        return float_software_version_;
    }

    bool bit_passed() const {
      return bit_passed_;
    }

    uint32_t bit_status_code() const {
      return bit_status_code_;
    }

    const ptime& dt_end() const {
      return dt_end_;
    }

    const ptime& dt_start() const {
      return dt_start_;
    }

    int32_t duration_sec() const {
      return duration_sec_;
    }
    
    uint32_t air_pump_errors() const {
      return air_pump_errors_;
    }
    
    uint32_t air_valve_errors() const {
      return air_valve_errors_;
    }
    
    uint32_t bat_errors() const {
      return bat_errors_;
    }
    
    uint32_t case_pres_errors() const {
      return case_pres_errors_;
    }
    
    uint32_t ctd_errors() const {
      return ctd_errors_;
    }
    
    uint8_t sensor_type() const {
      return sensor_type_;
    }

    uint32_t gps_errors() const {
      return gps_errors_;
    }
    
    uint32_t iridum_errors() const {
      return iridum_errors_;
    }
    
    uint32_t oil_pump_errors() const {
      return oil_pump_errors_;
    }
    
    uint32_t oil_valve_errors() const {
      return oil_valve_errors_;
    }
    
    uint8_t type() const {
      return type_;
    }

    uint32_t dt_end_epoch() const {
      return dt_end_epoch_;
    }

    uint32_t dt_start_epoch() const {
      return dt_start_epoch_;
    }

    uint32_t ctd_status() const {
      return ctd_status_;
    }

    float pres_after_cal() const {
      return pres_after_cal_;
    }

    float pres_before_cal() const {
      return pres_before_cal_;
    }

    float pres_offset() const {
      return pres_offset_;
    }

    float saln() const {
      return saln_;
    }

    float temp() const {
      return temp_;
    }

    float bat_volt_end() const {
      return bat_volt_end_;
    }

    float bat_volt_start() const {
      return bat_volt_start_;
    }

    float case_vac_end() const {
      return case_vac_end_;
    }

    float case_vac_start() const {
      return case_vac_start_;
    }

    uint8_t air_pump_cycle_count() const {
      return air_pump_cycle_count_;
    }

    float air_pump_v12_curr_max() const {
      return air_pump_v12_curr_max_;
    }

    float air_pump_volt_max() const {
      return air_pump_volt_max_;
    }

    uint16_t air_pump_test_duration() const {
      return air_pump_test_duration_;
    }

    float air_valve_cal_closed() const {
      return air_valve_cal_closed_;
    }

    float air_valve_cal_mid() const {
      return air_valve_cal_mid_;
    }

    float air_valve_cal_open() const {
      return air_valve_cal_open_;
    }

    float air_valve_closed() const {
      return air_valve_closed_;
    }

    float air_valve_mid() const {
      return air_valve_mid_;
    }

    float air_valve_open() const {
      return air_valve_open_;
    }

    float oil_pump_curr_avg() const {
      return oil_pump_curr_avg_;
    }

    float oil_pump_curr_max() const {
      return oil_pump_curr_max_;
    }

    float oil_pump_volt_max() const {
      return oil_pump_volt_max_;
    }

    uint16_t oil_pump_test_duration() const {
      return oil_pump_test_duration_;
    }

    float oil_pump_v12_curr_max() const {
      return oil_pump_v12_curr_max_;
    }

    float oil_valve_cal_closed() const {
      return oil_valve_cal_closed_;
    }

    float oil_valve_cal_open() const {
      return oil_valve_cal_open_;
    }

    float oil_valve_closed() const {
      return oil_valve_closed_;
    }

    float oil_valve_open() const {
      return oil_valve_open_;
    }

    float air_pump_pres_max() const {
      return air_pump_pres_max_;
    }


    float air_pump_curr_max() const {
      return air_pump_curr_max_;
    }

    float air_pump_curr_avg() const {
      return air_pump_curr_avg_;
    }

    uint32_t traveler_errors() const {
      return traveler_errors_;
    }

  public:

    /* Member Functions */

    /** The parameterized constructor.
     *
     *  @param[in]      data        The binary data to decode.
     *
     *  @throw          log::MessageException if an error occurs while decoding.
     */
    XAlamoBit2(const vector<uint8_t> &data) {
      DecodeXMsgData(data); // Decode the message.
    }

    /** The destructor. */
    ~XAlamoBit2() { /* Do nothing. */ }

    /** Decode the ALAMO BIT test engineering message from the binary message data.
     *
     *  @param[in]      data        The binary data to decode.
     *
     *  @throw          log::MessageException if an error occurs while decoding.
     */
    void DecodeXMsgData(const vector<uint8_t> &data) {

      try {

        const uint8_t *d = data.data();

        if(data.size() < 13) { // Check message size.
          ostringstream os;
          os << "Invalid X-Message: Expected at least 13 bytes, but only received"
             << data.size()
             << " bytes!";
          EXC_THROW(os.str());
        }

        if(d[0] == 0xE5) { // Check if BIT passed or failed.
          set_bit_passed(true);
        } else if(d[0] == 0xE6) {
          set_bit_passed(false);
        } else {
          ostringstream os;
          os << "Invalid X-Message: Expected X-Message format identifiers 0xE5 or 0xE6, but "
             << "received 0x"
             << std::hex
             << d[0]
             << "!";
          EXC_THROW(os.str());
        }

        set_bit_version(u08(d, 3)); // Get BIT version.


        /* Parse the BIT message: */

        switch(bit_version_) {

          case 9: // ALAMO BIT version 9. Starting firmware version 10.5.0
          case 10: // ALAMO BIT version 10. Starting firmware version 10.6.0

            if(data.size() != 256) { // Check the message size.
              ostringstream os;
              os << "Invalid X-Message: Expected 260 bytes, but received "
                 << data.size()
                 << " bytes!";
#if defined(XMSG_DEBUG_BIST) && XMSG_DEBUG_BIST
              lwrn() << os.str();
#else
              EXC_THROW(os.str());
#endif // defined(XMSG_DEBUG_BIST) && XMSG_DEBUG_BIST
            }

            /* header */

            set_float_software_version(   u32(d,   4));
            set_type(                     u08(d,   8));
            set_sensor_type(              u08(d,   9));

            /* status codes */

            set_bit_status_code(          u32(d,  10));
            set_air_pump_errors(          u32(d,  14));
            set_oil_pump_errors(          u32(d,  18));
            set_air_valve_errors(         u32(d,  22));
            set_oil_valve_errors(         u32(d,  26));
            set_gps_errors(               u32(d,  30));
            set_ctd_errors(               u32(d,  34));
            set_iridum_errors(            u32(d,  38));
            set_case_pres_errors(         u32(d,  42));
            set_bat_errors(               u32(d,  46));

            /* time stamps */

            set_dt_start_epoch(           u32(d,  50));
            set_dt_start(               s1970(d,  50));
            set_dt_end_epoch(             u32(d,  54));
            set_dt_end(                 s1970(d,  54));
            set_duration_sec(dt_end_epoch() - dt_start_epoch());

            /* ctd */

            set_pres_before_cal(          f32(d,  58));
            set_pres_offset(              f32(d,  62));
            set_pres_after_cal(           f32(d,  66));
            set_temp(                     f32(d,  70));
            set_saln(                     f32(d,  74));
            set_ctd_status(               u32(d,  78));

            /* case vac */

            set_case_vac_start(           f32(d,  82));
            set_case_vac_end(             f32(d,  86));

            /* bat voltage */

            set_bat_volt_start(           f32(d,  90));
            set_bat_volt_end(             f32(d,  94));

            /* air valve */

            set_air_valve_open(           f32(d,  98));
            set_air_valve_cal_open(       f32(d, 102));
            set_air_valve_closed(         f32(d, 106));
            set_air_valve_cal_closed(     f32(d, 110));
            set_air_valve_mid(            f32(d, 114));
            set_air_valve_cal_mid(        f32(d, 118));

            /* air pump */

            set_air_pump_test_duration(   u16(d, 122));
            set_air_pump_cycle_count(     u08(d, 124));
            set_air_pump_pres_max(        f32(d, 125));
            set_air_pump_curr_max(        f32(d, 129));
            set_air_pump_curr_avg(        f32(d, 133));
            set_air_pump_v12_curr_max(    f32(d, 137));
            set_air_pump_volt_max(        f32(d, 141));

            /* oil valve */

            set_oil_valve_open(           f32(d, 145));
            set_oil_valve_cal_open(       f32(d, 149));
            set_oil_valve_closed(         f32(d, 153));
            set_oil_valve_cal_closed(     f32(d, 157));

            /* oil pump */

            set_oil_pump_test_duration(   u16(d, 161));
            set_oil_pump_curr_max(        f32(d, 163));
            set_oil_pump_curr_avg(        f32(d, 167));
            set_oil_pump_v12_curr_max(    f32(d, 171));
            set_oil_pump_volt_max(        f32(d, 175));

            break;

          case 11: // ALAMO BIT version 11.

            if(data.size() != 256 && data.size() != 264) { // Check the message size.
              ostringstream os;
              os << "Invalid X-Message: Expected 256 or 264 bytes, but received "
                 << data.size()
                 << " bytes!";
#if defined(XMSG_DEBUG_BIST) && XMSG_DEBUG_BIST
              lwrn() << os.str();
#else
              EXC_THROW(os.str());
#endif // defined(XMSG_DEBUG_BIST) && XMSG_DEBUG_BIST
            }


            /* header */

            set_float_software_version(   u32(d,   4));
            set_type(                     u08(d,   8));
            set_sensor_type(              u08(d,   9));


            /* status codes */

            set_bit_status_code(          u32(d,  10));
            set_air_pump_errors(          u32(d,  14));
            set_oil_pump_errors(          u32(d,  18));
            set_air_valve_errors(         u32(d,  22));
            set_oil_valve_errors(         u32(d,  26));
            set_gps_errors(               u32(d,  30));
            set_ctd_errors(               u32(d,  34));
            set_iridum_errors(            u32(d,  38));
            set_case_pres_errors(         u32(d,  42));
            set_bat_errors(               u32(d,  46));
            set_traveler_errors(          u32(d,  50)); // added 10.8

            /* time stamps */

            set_dt_start_epoch(           u32(d,  54));
            set_dt_start(               s1970(d,  54));
            set_dt_end_epoch(             u32(d,  58));
            set_dt_end(                 s1970(d,  58));
            set_duration_sec(dt_end_epoch() - dt_start_epoch());

            /* ctd */

            set_pres_before_cal(          f32(d,  62));
            set_pres_offset(              f32(d,  66));
            set_pres_after_cal(           f32(d,  70));
            set_temp(                     f32(d,  74));
            set_saln(                     f32(d,  78));
            set_ctd_status(               u32(d,  82));

            /* case vac */

            set_case_vac_start(           f32(d,  86));
            set_case_vac_end(             f32(d,  90));

            /* bat voltage */

            set_bat_volt_start(           f32(d,  94));
            set_bat_volt_end(             f32(d,  98));

            /* air valve */

            set_air_valve_open(           f32(d, 102));
            set_air_valve_cal_open(       f32(d, 106));
            set_air_valve_closed(         f32(d, 110));
            set_air_valve_cal_closed(     f32(d, 114));
            set_air_valve_mid(            f32(d, 118));
            set_air_valve_cal_mid(        f32(d, 122));

            /* air pump */

            set_air_pump_test_duration(   u16(d, 126));
            set_air_pump_cycle_count(     u08(d, 128));
            set_air_pump_pres_max(        f32(d, 129));
            set_air_pump_curr_max(        f32(d, 133));
            set_air_pump_curr_avg(        f32(d, 137));
            set_air_pump_v12_curr_max(    f32(d, 141));
            set_air_pump_volt_max(        f32(d, 145));

            /* oil valve */

            set_oil_valve_open(           f32(d, 149));
            set_oil_valve_cal_open(       f32(d, 153));
            set_oil_valve_closed(         f32(d, 157));
            set_oil_valve_cal_closed(     f32(d, 161));

            /* oil pump */

            set_oil_pump_test_duration(   u16(d, 165));
            set_oil_pump_curr_max(        f32(d, 167));
            set_oil_pump_curr_avg(        f32(d, 171));
            set_oil_pump_v12_curr_max(    f32(d, 175));
            set_oil_pump_volt_max(        f32(d, 179));

            break;

          default: { // Unsupported ALAMO BIT version.

            ostringstream os;
            os << "Unsupported ALAMO BIT version: " << (int) bit_version_ << "!";
            EXC_THROW(os.str());
          }

            break;

        } // switch(bit_version_)

#       if defined(XMSG_DEBUG_BIST) && XMSG_DEBUG_BIST

        ldbg() << "BIT message listing string: "
                << endl << GenerateListingString(0, 0);

#       endif // defined(XMSG_DEBUG_BIST) && XMSG_DEBUG_BIST

      } CATCH_BLOCK("Failed to decode ALAMO Engineering (BIT v2) data message!")

    }

    /** Generate a listing entry string representation of the the ALAMO BIT test engineering
     *  message.
     *
     *  @param[in]      whoi        The instrument serial number.
     *  @param[in]      dive        The dive number of this ALAMO BIT test engineering message.
     *
     *  @return         A listing entry string representation of the the ALAMO BIT test engineering
     *                  message.
     */
    const string GenerateListingString(
        int whoi,
        int dive) const {

      ostringstream os;
      switch(bit_version()) {

        case 9: // ALAMO BIT version 9. Starting firmware version 10.5.0
        case 10: // ALAMO BIT version 10. Starting firmware version 10.6.1
        case 11: // ALAMO BIT version 11.

        {

          int midx = 0;
          string sensor_type_str;

          switch (sensor_type()) {
            case 0: sensor_type_str = "RBR"; break;
            case 1: sensor_type_str = "SBE"; break;
            default: sensor_type_str = "unknown"; break;
          }

          /* BIT results  */

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s   %14s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "   0x%08X    0x%08X %13d # %s");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "FWVer" % "BITResults" %  "sensorType" % "sensorTypeName"<< endl;
            os << "#" << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % float_software_version() % bit_status_code() % (int)sensor_type()
                % sensor_type_str
               << endl;
          }

          /* Timestamps */

          os << "# --- Date/Time -----------------------------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%27s %27s %13s");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%27s %27s %13s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "%27s %27s %13d");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "startTime" % "endTime" % "duration"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "" % "" % "(sec)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % GenerateIso8601String(dt_start()) % GenerateIso8601String(dt_end())
                % duration_sec()
               << endl;
          }

          /* CTD */

          os << "# --- CTD -----------------------------------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s %13s %13s");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s %13s %13s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "   0x%08X    0x%08X %13.3f %13.3f %13.3f");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "errors" % "status" % "presPreCal" % "presOffset" % "presPostCal"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "" % "" % "(dBar)" % "(dBar)" % "(dBar)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % ctd_errors() %ctd_status()  % pres_before_cal() % pres_offset()
                % pres_after_cal()
               << endl;
          }

          os << "# -------------------------------------------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%13s %13s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "%13.3f %13.3f");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "temp" % "saln"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "(degC)" % "(PSU)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % temp() % saln()
                << endl;
          }

          /* Case vac */

          os << "# --- Case Vacuum ---------------------------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "   0x%08X %13.3f %13.3f");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "errors" % "caseVacStart" % "caseVacEnd"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "" % "(inHg)" % "(inHg)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % case_pres_errors()  % case_vac_start() % case_vac_end()
               << endl;
          }

          /* Battery Voltage */

          os << "# --- Battery Voltage -----------------------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "   0x%08X %13.3f %13.3f");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "errors" % "batVoltStart" % "batVoltEnd"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "" % "(V)" % "(V)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % bat_errors()  % bat_volt_start() % bat_volt_end()
               << endl;
          }

          /* Air Valve */

          os << "# --- Air Valve Positions -------------------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "%13.3f %13.3f %13.3f");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "open" % "mid" % "closed"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "(V)" % "(V)" % "(V)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % air_valve_open()  % air_valve_mid() % air_valve_closed()
               << endl;
          }

          os << "# --- Air Valve Calibration Positions -------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "%13.3f %13.3f %13.3f");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "open" % "mid" % "closed"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "(V)" % "(V)" % "(V)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % air_valve_cal_open()  % air_valve_cal_mid() % air_valve_cal_closed()
               << endl;
          }


          /* Air pump */

          os << "# --- Air Pump ------------------------------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s "
                "%13s");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s "
                "%13s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "   0x%08X %13d %13d "
                "%13.3f");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "errors" % "duration" % "cycles"
                % "presMax"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "" % "(sec)" % ""
                % "(PSI)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % air_pump_errors() % air_pump_test_duration() % (int)air_pump_cycle_count()
                % air_pump_pres_max()
               << endl;
          }

          os << "# -------------------------------------------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s "
                "%13s %13s");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s "
                "%13s %13s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "%13.3f %13.3f %13.3f "
                "%13.3f %13.3f");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "currAvg" % "currMax" % "12VCurrMax"
                % "batMax" % "powerMax"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "(A)" % "(A)" % "(A)"
                % "(V)" % "(W)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % air_pump_curr_avg() % air_pump_curr_max() % air_pump_v12_curr_max()
                % air_pump_volt_max() % (12*air_pump_v12_curr_max())
               << endl;
          }


          /* Oil Valve */

          os << "# --- Oil Valve Positions -------------------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%13s %13s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "%13.3f %13.3f");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "open" % "closed"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "(V)" % "(V)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % oil_valve_open() % oil_valve_closed()
               << endl;
          }

          os << "# --- Oil Valve Calibration Positions -------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%13s %13s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "%13.3f %13.3f");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "open" % "closed"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "(V)" % "(V)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % oil_valve_cal_open() % oil_valve_cal_closed()
               << endl;
          }

          /* Oil pump */

          os << "# --- Oil Pump ------------------------------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s ");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%13s %13s ");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "   0x%08X %13d ");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "errors" % "duration"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "" % "(sec)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % oil_pump_errors() % oil_pump_test_duration()
               << endl;
          }

          os << "# -------------------------------------------------------------------------------"
              "-------------------"
             << endl;

          {
            static const format fmt1(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s "
                "%13s %13s");
            static const format fmt2(
                "#   %5s %4s %4s %6s "
                "%13s %13s %13s "
                "%13s %13s");
            static const format fmt3(
                "E0%c %5d %4d %4d %6d "
                "%13.3f %13.3f %13.3f "
                "%13.3f %13.3f");
            os << format(fmt1)
                % "SN" % "dive" % "midx" % "BITVer"
                % "currAvg" % "currMax" % "12VCurrMax"
                % "batMax" % "powerMax"
               << endl;
            os << format(fmt2)
                % "" % "" % "" % ""
                % "(A)" % "(A)" % "(A)"
                % "(V)" % "(W)"
               << endl;
            os << format(fmt3)
                % (bit_passed() ? '5' : '6') % whoi % dive % midx++ % (int) bit_version()
                % oil_pump_curr_avg()  % oil_pump_curr_max() % oil_pump_v12_curr_max()
                % oil_pump_volt_max() % (12*oil_pump_v12_curr_max())
               << endl;
          }


      }


      break;


        default: { // Unsupported Engineering (BIT) version.
          ostringstream os;
          os << "Unsupported ALAMO Engineering (BIT v2) message version: 0x"
             << hex
             << (int) bit_version()
             << "!";
          EXC_THROW(os.str());
      }

          break;

      }


      return os.str();

    }

};

} /* namespace xmsg */
} /* namespace util */

#endif /* DATA_XMSG_XALAMOBIT2_H_ */
