/** The @ref util::xmsg::AbstractXSensorData class header file.
 *
 *  This is the header file for the @ref util::xmsg::AbstractXSensorData class. The abstract
 *  sensor data class provides the functionality to decode sensor data messages using either
 *  first-difference or curvature encoding. It is used to decode and represent the sensor data
 *  returned from the instrument that is contained within a single message.
 *
 *  The classes that inherit from this class represent profile or drift data taken from a single
 *  sensor e.g. pressure, temperature, or salinity. The sensor data is stored in a vector of raw
 *  integer counts, @ref util::xmsg::AbstractXSensorData::raw_sensor_data_. The data from the
 *  sensor is recovered in scientific units by applying the appropriate offset and gain
 *  corrections, as specified in the Argo data message.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 6, 2014 5:19:53 PM
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

#ifndef ABSTRACTXSENSORDATA_H_
#define ABSTRACTXSENSORDATA_H_

/* Includes: */

// local
#include "XMessageIndexable.h"

namespace argo {
  namespace iridium {
    namespace xfloat {

// forward declaration
      class XFloatDiveData;

    } /* namespace xfloat */
  } /* namespace iridium */
} /* namespace argo */

namespace util {
  namespace xmsg {

    /* Type Definitions: */

    /** The X-Message sensor data packing format enumeration.
     *
     *  This type enumerates the different packing format types that may be used by the class
     *  @ref AbstractXSensorData and the classes that inherit from it. The manner in which the sensor
     *  data is decoded is determined by the data packing formats enumerated here.
     */
    typedef enum XSensorDataFormat {

      /** The X-Message sensor data packing format 0: Legacy first-difference packing format.
       *
       */
      XSENS_FMT_DIFFERENCE = 0,

      /** The X-Message sensor data packing format 1: Standard curvature packing format.
       *
       *  @note           The packing format was added in X version 2.0.
       *
       */
      XSENS_FMT_CURVATURE = 1,

      XSENS_FMT_CURVATURE_AUX_TEMP_CHANNEL = 3,

      /** The X-Message sensor data packing format 255: Unknown packing format.
       *
       */
      XSENS_FMT_UNKNOWN = 0xFF

    } XSensorDataFormat_t;

    /* Class Definition: */

    /** The abstract sensor data class.
     *
     *  The abstract sensor data class provides the functionality to decode sensor data messages using
     *  either first-difference or curvature encoding. It is used to decode and represent the sensor
     *  data returned from the instrument that is contained within a single message.
     *
     *  The classes that inherit from this class represent profile or drift data taken from a single
     *  sensor e.g. pressure, temperature, or salinity. The sensor data is stored in a vector of raw
     *  integer counts, @ref raw_sensor_data_. The data from the sensor is recovered in scientific
     *  units by applying the appropriate offset and gain corrections, as specified in the Argo data
     *  message.
     *
     */
    class AbstractXSensorData:
        public XMessageIndexable {

        /* Friend Classes: */

#   if defined(XMSG_ENABLE_MULTI_PROF) && XMSG_ENABLE_MULTI_PROF
        friend class argo::iridium::xfloat::XFloatDiveData;
#   endif // defined(XMSG_ENABLE_MULTI_PROF) && XMSG_ENABLE_MULTI_PROF

      protected:

        /* Protected Data Members: */

        bool aux_channel;

        /** The enumerated sensor data packing format.
         *
         *  This field indicates the format of encoded sensor data. The manner in which the sensor
         *  data is decoded is determined by the format of this field.
         *
         */
        XSensorDataFormat_t sensor_data_format_;

        /** The raw integer counts of the measured sensor data.
         *
         *  This vector of integers represents the "raw" sample values of the message. That is, by
         *  applying the appropriate gain and offset corrections, the sensor values may be recovered
         *  in scientific units. This data may represent pressure, temperature, salinity, or any other
         *  data that is recorded during the drift or profile phases of the dive cycle.
         */
        vector<int32_t> raw_sensor_data_;

      public:

        /* Public Member Functions: */

        /** Parameterized constructor.
         *
         *  @param[in]      dive        The dive or cycle number of this message.
         *  @param[in]      whoi        The unique WHOI identifier.
         *  @param[in]      data        Binary X-message data to decode.
         *
         *  @throw          util::log::MessageException if
         *                      - an invalid X-Message type identifier is encountered.
         *                      - the message is of insufficient length,
         *                      - an invalid packing format is encountered, or
         *                      - the message terminator is in an invalid location.
         */
        AbstractXSensorData(int16_t dive, uint16_t whoi, const vector<uint8_t>& data) :
                XMessageIndexable(dive, whoi, data),
                aux_channel(false),
                sensor_data_format_(XSENS_FMT_UNKNOWN) {
          /* Do nothing. */
        }

        /** Destructor.
         *
         */
        virtual ~AbstractXSensorData() {
          /* Do nothing. */
        }

        /** Decode the X message.
         *
         *  @param[in]      data        The encoded X message to decode.
         */
        virtual void DecodeXMsgData(const vector<uint8_t>& data) = 0;

        /** Get the number of samples in this message.
         *
         *  @return         The number of samples in this message.
         */
        size_t num_samples() const {
          return raw_sensor_data_.size();
        }

        /** Get the raw sensor data vector.
         *
         *  @return         The raw sensor data vector.
         */
        const vector<int32_t>& raw_sensor_data() const {
          return raw_sensor_data_;
        }

        /** Get the raw integer value of the @f$i^{th}@f$ sample.
         *
         *  @param[in]      i       The index of the sensor value.
         *
         *  @return         The raw integer value of the @f$i^{th}@f$ ith sample.
         */
        int32_t Sample(size_t i) const {
          return raw_sensor_data_.at(i);
        }

        /** Get the @f$i^{th}@f$ sensor value in scientific units.
         *
         *  @param[in]      i       The index of the sensor value.
         *  @param[in]      gain    The sensor sensitivity scale factor.
         *  @param[in]      offs    The sensor offset.
         *
         *  @return         The @f$i^{th}@f$ sensor value in scientific units.
         */
        template<typename T = double> T SampleValue(size_t i, int16_t gain, int16_t offs) const {
          T val;

          try {
            val = gain ? (T) raw_sensor_data_.at(i) / (T) gain - (T) offs : -100;
          }CATCH_BLOCK("Failed to get sensor data value from raw counts sample!")

          return val;
        }

        /** Get the @f$i^{th}@f$ pressure sensor value, in dBar.
         *
         *  @param[in]      i       The index of the sensor value.
         *  @param[in]      gain    The pressure sensor sensitivity scale factor, in LSB/dBar.
         *  @param[in]      offs    The pressure sensor offset, in dBar.
         *
         * @return          The @f$i^{th}@f$ pressure sensor value, in dBar.
         */
        template<typename T = double> T SampleValueAsPressure(
            size_t i,
            int16_t gain,
            int16_t offs) const {
          T val = SampleValue<T>(i, gain, offs);

          if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
            return XMSG_SENS_BAD_PRES;

          return val;
        }

        /** Get the @f$i^{th}@f$ salinity sensor value, in PSU.
         *
         *  @param[in]      i       The index of the sensor value.
         *  @param[in]      gain    The salinity sensor sensitivity scale factor, in LSB/PSU.
         *  @param[in]      offs    The salinity sensor offset, in PSU.
         *
         * @return          The @f$i^{th}@f$ salinity sensor value, in PSU.
         */
        template<typename T = double> T SampleValueAsSalinity(
            size_t i,
            int16_t gain,
            int16_t offs) const {
          T val = SampleValue<T>(i, gain, offs);

          if(val < XMSG_SENS_MIN_SALN || val > XMSG_SENS_MAX_SALN)
            return XMSG_SENS_BAD_SALN;

          return val;
        }

        /** Get the @f$i^{th}@f$ temperature sensor value, in degC.
         *
         *  @param[in]      i       The index of the sensor value.
         *  @param[in]      gain    The temperature sensor sensitivity scale factor, in LSB/degC.
         *  @param[in]      offs    The temperature sensor offset, in degC.
         *
         * @return          The @f$i^{th}@f$ temperature sensor value, in degC.
         */
        template<typename T = double> T SampleValueAsTemperature(
            size_t i,
            int16_t gain,
            int16_t offs) const {
          T val = SampleValue<T>(i, gain, offs);

          if(val < XMSG_SENS_MIN_TEMP || val > XMSG_SENS_MAX_TEMP)
            return XMSG_SENS_BAD_TEMP;

          return val;
        }

        /** Get the enumerated sensor data format value.
         *
         *  @return         The enumerated sensor data format value.
         */
        XSensorDataFormat_t sensor_data_format() const {
          return sensor_data_format_;
        }

        bool is_aux_channel() const {
          return aux_channel;
        }

      protected:

        /* Protected Member Functions: */

        /** Parameterized constructor.
         *
         *  @param[in]      dive        The dive or cycle number of this message.
         *  @param[in]      whoi        The unique WHOI identifier.
         *  @param[in]      data        Binary X-message data to decode.
         *
         *  @throw          util::log::MessageException if
         *                      - an invalid X-Message type identifier is encountered.
         *                      - the message is of insufficient length,
         *                      - an invalid packing format is encountered, or
         *                      - the message terminator is in an invalid location.
         */
        AbstractXSensorData() :
                XMessageIndexable(),
                aux_channel(false),
                sensor_data_format_(XSENS_FMT_UNKNOWN) {
          /* Do nothing. */
        }

        /** Set raw integer counts of the measured sensor data.
         *
         * @param[in]   raw_sensor_data     The data to set.
         */
        void set_raw_sensor_data(const vector<int32_t>& raw_sensor_data) {
          raw_sensor_data_ = raw_sensor_data;
        }

        /** Set the sensor data format
         *
         *  @param[in]  sensor_data_format  The sensor data format to set.
         */
        void set_sensor_data_format(XSensorDataFormat_t sensor_data_format) {
          sensor_data_format_ = sensor_data_format;
        }

        /** Unpack the sensor data from the binary message.
         *
         *  This field unpacks the encoded sensor data from the message and stores the decoded sensor
         *  values in @ref raw_sensor_data_. First the function checks the size of the message, and
         *  the data packing format. The message data is decoded according to the packing format type.
         *  Both first-difference and curvature encoding are supported. The encoding method is
         *  determined from the value of the high nibble of byte @c data[1].
         *
         *  Once this function completes successfully, then the data may be recovered in scientific
         *  units from the raw sensor values by applying the appropriate sensor gain and offset values.
         *
         *  @param[in]      data        The binary message to unpack, in a vector of unsigned bytes.
         *
         *  @warning        The function assumes the message data is valid. Message validity should
         *                  be ensured before this function is called.
         *
         *  @throw          util::log::MessageException if
         *                      - an error occurs while decoding the message,
         *                      - the message is of insufficient length, or
         *                      - an invalid packing format is encountered.
         *
         *  @return         The sensor data raw counts, in a vector of signed integers.
         */
        virtual vector<int32_t> UnpackSensorData(const vector<uint8_t>& data) {
          try {
            // Check message length
            if(data.size() < 5) {
              ostringstream os;
              os << "Message too short! Expected at least 5 bytes, but only received "
                 << data.size()
                 << " bytes!";
            }

            // Set the message type & index.
            set_id_xmsg_type(static_cast<XMsgTypeId_t>(data.at(0)));
            set_xmsg_index(0x0F & data.at(0));

            // Clear previous, if any.
            raw_sensor_data_.clear();

            /* Do Version-Specific Decoding: */

            switch((data.at(1) & 0xF0) >> 4) {

              case XSENS_FMT_DIFFERENCE:

                /* Handle Fist-Difference Decoding: */

                try {

                  ldbg() << "Unpacking first-difference-encoded sensor data..." << endl;

                  uint32_t scale;
                  int32_t x0;

                  // This message uses the first-difference packing method.
                  set_sensor_data_format(XSENS_FMT_DIFFERENCE);

                  // Iterate over the sensor data.
                  for(vector<uint8_t>::const_iterator it = data.begin() + 3; it < data.end() - 5;) {

                    // Parse sub-block header.
                    scale = *it++;
                    raw_sensor_data_.push_back(x0 = (*it++ << 8) | *it++);
                    size_t isub = XMSG_SENS_MAX_SUB;

                    // Iterate over sub-block data.
                    while(it < data.end() - 1 && --isub) {
                      int32_t dx = *it++;
                      if(dx > 0x7F)
                        dx -= 0x0100;
                      raw_sensor_data_.push_back(x0 += dx * scale);
                    }
                  }

                }CATCH_BLOCK("Failed to unpack first-difference data!")

                break;

              case XSENS_FMT_CURVATURE_AUX_TEMP_CHANNEL:

                aux_channel = true;

                /* No break. */

              case XSENS_FMT_CURVATURE:

                /* Handle Curvature Decoding: */

                try {

                  ldbg() << "Unpacking curvature-encoded sensor data..." << endl;

                  /*  The data first differences.
                   *
                   *  The first difference are used to reconstruct the raw sensor data, and
                   *  are themselves constructed from the data second differences.
                   *
                   */
                  vector<int32_t> data_1st_differences;

                  /*  The data second differences.
                   *
                   *  The encoded data represents the second differences of the raw sensor data.
                   *  The second differences are used to reconstruct the first differences,
                   *  which, in turn, are used to reconstruct the raw sensor data.
                   *
                   */
                  vector<int32_t> data_2nd_differences;

                  /*  The message data iterator.
                   *
                   *  This field is used for iterating over the values of the sub-blocks of the
                   *  message. One nibble is read at a time, so this iterator is iterated for
                   *  every other nibble that is read.
                   */
                  vector<uint8_t>::const_iterator data_iterator;

                  /*  The 32 3-bit packing factors.
                   *
                   *  The packing factor represents the dynamic range for each sub-block. The
                   *  packing factor represents the number of nibbles that is needed to store
                   *  the value.
                   */
                  uint8_t data_packing_factors[32];
                  memset(data_packing_factors, 0, sizeof(data_packing_factors));

                  /*  The number of remaining values in the message.
                   *
                   *  This field is used as a control variable to determine when data unpacking
                   *  is complete.
                   *
                   */
                  size_t num_remaining_values = 0;

                  /*  The total number of values in this message.
                   *
                   *  This field is parsed from the message header. It is used as a control
                   *  variable do determine when message unpacking is complete.
                   *
                   */
                  size_t num_values = 0;

                  /*  The index of the sub block.
                   *
                   *  This field is used to track the index of the sub-block during iteration
                   *  of the message data. It is used as an index to the data_packing_factors[]
                   *  array.
                   */
                  size_t sub_block_index = 0;

                  /*  The sub-block-nibble-is-high flag.
                   *
                   *  This field is used to keep track of whether we are reading the high or low
                   *  nibble of the data.
                   */
                  bool sub_block_nibble_is_high = true;

                  /* The number of values in a sub-block.
                   *
                   * This field is used for iteration over the values in a sub-block.
                   *
                   */
                  size_t sub_block_num_values = 0;

                  // This message uses the second-difference (curvature) packing method.
                  set_sensor_data_format(XSENS_FMT_CURVATURE);

                  /* Parse the message header: */

                  // Get the first sensor value.
                  const int32_t first_sensor_value = i24(data, 6);

                  // Read the first difference value.
                  data_1st_differences.push_back(i24(data, 9));

                  // Determine if the first sensor value should be included in the data.
                  switch(id_xmsg_type()) {
                    case XMSG_TYPE_ID_PRES0:            // 0x10
                    case XMSG_TYPE_ID_TEMP0:            // 0x20
                    case XMSG_TYPE_ID_SALN0:            // 0x30
                    case XMSG_TYPE_ID_PRES_HI_RES0:     // 0x90
                    case XMSG_TYPE_ID_TEMP_HI_RES0:     // 0xA0
                    case XMSG_TYPE_ID_SALN_HI_RES0:     // 0xB0
                    case XMSG_TYPE_ID_PRES_DRIFT8:      // 0x98
                    case XMSG_TYPE_ID_TEMP_DRIFT8:      // 0xA8
                    case XMSG_TYPE_ID_SALN_DRIFT8:      // 0xB8
                      raw_sensor_data_.push_back(first_sensor_value);

                      break;

                    default:

                      /* Do nothing. */

                      break;
                  }

                  // Get the number of values.
                  switch(num_values = u16(data, 4)) {
                    case 0:

                      /* Handle messages containing zero values: */

                      // Throw exception.
                      EXC_THROW("Message contains 0 sensor values!");
                      break;

                    case 2: {

                      /* Handle messages containing two values: */

                      // Compute the second sensor value from the first difference.
                      raw_sensor_data_.push_back(first_sensor_value + data_1st_differences.at(0));
                    }
                      /* No break. */

                    case 1:

                      /* Handle messages containing one value: */

                      // We have all the data for this message, so return.
                      return raw_sensor_data_;

                    default:

                      /* Handle messages containing three or more values: */

                      raw_sensor_data_.reserve(num_values);
                      data_1st_differences.reserve(num_values);
                      data_2nd_differences.reserve(num_values);
                      break;
                  }

                  /* Get the 32 3-bit packing factors from the 12-byte field: */

                  // Iterate over the four 3-byte blocks.
                  for(size_t i = 0; i < 4; ++i)

                    // Iterate over the eight values in each 3-byte block.
                    for(size_t j = 0; j < 8; ++j)

                      // Parse the packing factor.
                      data_packing_factors[i * 8 + j] = (u24(data, 12 + i*3) >> (3 * (7 - j)))
                          & 0x07;

#                   if defined(XMSG_DEBUG_SENSOR_CURVATURE) && XMSG_DEBUG_SENSOR_CURVATURE
                  {
                    ostringstream os1, os2;

                    os1 << "Decoding curvature data packing factors..."
                    << endl
                    << " Curvature Data Packing Factors:"
                    << endl
                    << "   ";

                    static const format fmt1(" p[%2d]"), fmt2(" %5d");

                    for(size_t i = 0; i < 32; ++i) {
                      os1 << format(fmt1) % i;
                      os2 << format(fmt2) % (unsigned int) data_packing_factors[i];
                    }

                    ldbg() << os1.str() << endl << "   " << os2.str() << endl;
                  }
#                   endif

                  /* Unpack the message data: */

#                   if defined(XMSG_DEBUG_SENSOR_CURVATURE) && XMSG_DEBUG_SENSOR_CURVATURE
                  ostringstream xmsg_debug_os_;
                  xmsg_debug_os_ << "Decoding curvature data..." << endl;
#                   endif

                  // Iterate over the sub-blocks.
                  data_iterator = data.begin() + 24;
                  num_remaining_values = num_values - 2;
                  while(num_remaining_values > 0 && data_iterator < data.end() - 1) {

                    try {

                      // Get the data packing factor for this sub-block.
                      uint8_t data_packing_factor = data_packing_factors[sub_block_index];

                      // Skip sub-blocks with data packing factor = 0.
                      if(!data_packing_factor) {
                        sub_block_index++;
                        continue;
                      }

                      // Get the number of values in this sub-block.
                      sub_block_num_values = min(16, (int) num_remaining_values);

#                           if defined(XMSG_DEBUG_SENSOR_CURVATURE) && XMSG_DEBUG_SENSOR_CURVATURE
                      {
                        static const format fmt1(
                            " sub-block %2d (p: %u) %4d values remaining");
                        static const format fmt2(" %8d");
                        xmsg_debug_os_ << format(fmt1) % sub_block_index
                        % (unsigned int) data_packing_factors[sub_block_index]
                        % num_remaining_values
                        << endl
                        << "     ";
                        for(size_t i = 0; i < 16; ++i)
                        xmsg_debug_os_ << format(fmt2) % i;
                        xmsg_debug_os_ << endl << "   0x";
                      }
#                           endif

                      // Iterate over the values in this sub-block.
                      for(size_t sub_block_value_idx = 0;
                          sub_block_value_idx < sub_block_num_values
                              && data_iterator < data.end() - 1; ++sub_block_value_idx) {
                        try {

#                               if defined(XMSG_DEBUG_SENSOR_CURVATURE) && XMSG_DEBUG_SENSOR_CURVATURE
                          ostringstream xmsg_debug_nibbles_os_;
#                               endif

                          // Iterate over the nibbles of the value.
                          int32_t second_difference = 0;

                          for(uint8_t value_nibble_index = 0;
                              value_nibble_index < data_packing_factor && data_iterator < data.end();
                              ++value_nibble_index) {

                            // Read the nibble. If this is a low nibble, increment the iterator.
                            uint8_t byte = (*data_iterator);
                            uint8_t nibble = 0;
                            if(sub_block_nibble_is_high) {
                              nibble = byte >> 4;
                            } else {
                              nibble = byte & 0x0F;
                              data_iterator++;
                            }
                            second_difference = (second_difference << 4) | nibble;

                            // Update the nibble-is-high value.
                            sub_block_nibble_is_high = !sub_block_nibble_is_high;

#                                       if defined(XMSG_DEBUG_SENSOR_CURVATURE) && XMSG_DEBUG_SENSOR_CURVATURE
                            {
                              static const format fmt("%X");
                              xmsg_debug_nibbles_os_ << format(fmt)
                              % (unsigned int) nibble;
                            }
#                                       endif

                          }

                          // Correct for signed second differences.
                          if(second_difference >= (1 << (4 * data_packing_factor - 1)))
                            second_difference -= (1 << (4 * data_packing_factor));

#                                   if defined(XMSG_DEBUG_SENSOR_CURVATURE) && XMSG_DEBUG_SENSOR_CURVATURE
                          {
                            static const format fmt(" %8s");
                            xmsg_debug_os_ << format(fmt)
                            % xmsg_debug_nibbles_os_.str();
                          }
#                                   endif

                          // Append the computed second difference.
                          data_2nd_differences.push_back(second_difference);

                          // Update the number of remaining values.
                          num_remaining_values--;

                        }CATCH_BLOCK(
                            string("Failed to decoded the sensor value in sub-block: ")
                                + lexical_cast<string>(sub_block_value_idx) + "!")
                      }

#                           if defined(XMSG_DEBUG_SENSOR_CURVATURE) && XMSG_DEBUG_SENSOR_CURVATURE
                      {
                        xmsg_debug_os_ << endl;
                      }
#                           endif

                      // Update the sub-block index.
                      sub_block_index++;

                    }CATCH_BLOCK(
                        string("Failed to decode sub-block: ")
                            + lexical_cast<string>(sub_block_index) + "!")

                  }

                  /* Compute the sensor data values: */

                  // Compute the first differences.
                  try {
                    int32_t first_difference = data_1st_differences.at(0);
                    for_each(
                        data_2nd_differences.begin(),
                        data_2nd_differences.end(),
                        [&](const int32_t second_difference) {
                          data_1st_differences.push_back((first_difference = first_difference + second_difference));
                        });

                  }CATCH_BLOCK("Failed to compute the sensor data first differences!")

                  try {
                    // Get the first sensor value.
                    int32_t sensor_value = first_sensor_value;

                    // Compute the sensor values.
                    for_each(
                        data_1st_differences.begin(),
                        data_1st_differences.end(),
                        [&](const int32_t first_difference) {
                          raw_sensor_data_.push_back((sensor_value = sensor_value + first_difference));
                        });
                  }CATCH_BLOCK("Failed to compute the sensor data values!")

                  /* --- Start of XMSG_DEBUG_SENSOR_CURVATURE Block ---------------------------------------------- */
#                   if defined(XMSG_DEBUG_SENSOR_CURVATURE) && XMSG_DEBUG_SENSOR_CURVATURE
                  try {

                    static const format fmt1("%20s %20s %20s %20s");
                    static const format fmt2("%20d %20d %20d %20d");
                    xmsg_debug_os_ << format(fmt1) % "idx" % "2nd" % "1st" % "value" << endl;
                    for(size_t i = 0; i < data_2nd_differences.size(); i++) {
                      xmsg_debug_os_ << format(fmt2) % i % data_2nd_differences.at(i)
                      % data_1st_differences.at(i)
                      % raw_sensor_data_.at(i)
                      << endl;
                    }
                    ldbg() << xmsg_debug_os_.str();
                  }CATCH_NOTHROW("Throw in XMSG_DEBUG_SENSOR_CURVATURE block!")
#                   endif
                  /* --- End of XMSG_DEBUG_SENSOR_CURVATURE Block ------------------------------------------------ */

                  // Throw exception if we do not have all the variables.
                  if(num_remaining_values) {
                    ostringstream os;
                    os << "Reached end of message data before reading all values! "
                       << num_remaining_values
                       << " of "
                       << num_values
                       << " values remaining.";
                    lwrn() << os.str();
//                    EXC_THROW(os.str());
                  }

                }CATCH_BLOCK("Failed to unpack curvature data!")

                break;

              default:

                /* Handle Unsupported Packing Formats: */

                // Throw exception
                EXC_THROW("Unsupported packing format!");
                break;
            }

          }CATCH_BLOCK("Failed to unpack X-Message sensor data.")

          return raw_sensor_data_;
        }

      private:

        /* Private Member Functions: */

    };

  } /* namespace xmsg */
} /* namespace util */
#endif /* ABSTRACTXSENSORDATA_H_ */
