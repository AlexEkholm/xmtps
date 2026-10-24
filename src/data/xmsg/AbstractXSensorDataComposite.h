/** The @ref util::xmsg::AbstractXSensorDataComposite class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 10, 2015 3:10:15 PM
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

#ifndef ABSTRACTXSENSORDATACOMPOSITE_H_
#define ABSTRACTXSENSORDATACOMPOSITE_H_

/* Includes: */

// local
#include "XDriftPressure.h"
#include "XDriftSalinity.h"
#include "XDriftTemperature.h"
#include "XHiResPressure.h"
#include "XHiResSalinity.h"
#include "XHiResTemperature.h"
#include "XProfilePressure.h"
#include "XProfileSalinity.h"
#include "XProfileTemperature.h"

/* Definitions: */

namespace util {
namespace xmsg {

/* Types: */

/** The AbstractXSensorDataComposite class.
 *
 *  This abstract, template class represents the composite sensor data for a single dive cycle. It
 *  provided the functionality for aggregating sensor data messages into a single object.
 *
 */
template<class PRES_CLASS, class SALN_CLASS, class TEMP_CLASS, unsigned int MAX_MSGS>
class AbstractXSensorDataComposite:
        public thread::RecursiveLockable {

    /* Type Definitions: */

    /** Type definition for a shared pointer to the pressure data message.
     *
     */
    typedef shared_ptr<PRES_CLASS> PresMsgPtr_t;

    /** Type definition for a shared pointer to the salinity data message.
     *
     */
    typedef shared_ptr<SALN_CLASS> SalnMsgPtr_t;

    /** Type definition for a shared pointer to the temperature data message.
     *
     */
    typedef shared_ptr<TEMP_CLASS> TempMsgPtr_t;

    /** Type definition for a static vector (of size MAX_MSGS) of shared pointers to the pressure
     *  data message.
     *
     */
    typedef static_vector<PresMsgPtr_t, MAX_MSGS> PresMsgVector_t;

    /** Type definition for a static vector (of size MAX_MSGS) of shared pointers to the salinity
     *  data message.
     *
     */
    typedef static_vector<SalnMsgPtr_t, MAX_MSGS> SalnMsgVector_t;

    /** Type definition for a static vector (of size MAX_MSGS) of shared pointers to the
     *  temperature data message.
     *
     */
    typedef static_vector<TempMsgPtr_t, MAX_MSGS> TempMsgVector_t;

private:

    /* Data Members: */

    /** The aggregated data is valid flag.
     *
     *  This flag indicates the status of the aggregated sensor data. It is used to ensure the
     *  composite sensor data is aggregated only once following each assignment of sensor data to
     *  the composite data message.
     *
     *  This field is set to FALSE during initialization, and is set to FALSE each time a sensor
     *  data message is assigned to the composite data message. Each time @ref AggregateSensorData()
     *  is called, the data will be aggregated, and this field will be set to TRUE.
     *
     */
    bool aggregated_data_valid_;

    /** A vector of the aggregated pressure data samples, in raw counts.
     *
     *  Each time the data is aggregated, the sensor data samples from the sensor data messages
     *  assigned to this composite objects are stored in this vector.
     *
     */
    vector<int32_t> aggregated_pres_data_;

    /** A vector of the aggregated salinity data samples, in raw counts.
     *
     *  Each time the data is aggregated, the sensor data samples from the sensor data messages
     *  assigned to this composite objects are stored in this vector.
     *
     */
    vector<int32_t> aggregated_saln_data_;

    /** A vector of the aggregated temperature data samples, in raw counts.
     *
     *  Each time the data is aggregated, the sensor data samples from the sensor data messages
     *  assigned to this composite object are stored in this vector.
     *
     */
    vector<int32_t> aggregated_temp_data_;

    vector<int32_t> aggregated_aux_temp_data_;

    /** A static vector (of size MAX_MSGS) containing pointers the pressure sensor data messages
     *  assigned to this composite object.
     *
     *  Each time a pressure sensor data message is assigned to the this object, it will be
     *  emplaced in this vector in order of increasing message indices.
     *
     */
    PresMsgVector_t pres_messages_;

    /** A static vector (of size MAX_MSGS) containing pointers the salinity sensor data messages
     *  assigned to this composite object.
     *
     *  Each time a salinity sensor data message is assigned to the this object, it will be
     *  emplaced in this vector in order of increasing message indices.
     *
     */
    SalnMsgVector_t saln_messages_;

    /** A static vector (of size MAX_MSGS) containing pointers the temperature sensor data messages
     *  assigned to this composite object.
     *
     *  Each time a temperature sensor data message is assigned to the this object, it will be
     *  emplaced in this vector in order of increasing message indices.
     *
     */
    TempMsgVector_t temp_messages_;

    TempMsgVector_t aux_temp_messages_;

protected:

    /* Protected Member Functions: */

    /** Aggregate the sensor data for this composite sensor data object.
     *
     *  This function iterates over each of the sensor data messages assigned to this composite
     *  data object, and copies of the sensor data values into the corresponding sensor data
     *  vector.
     *
     *  If @ref aggregated_data_valid_ is TRUE, then this function will have no effect. Otherwise,
     *  each of the aggregegated sensor data vectors will be cleared and the appropriate number of
     *  samples will be reserved.
     *
     *  Then, the sensor data message will be iterated over in order of increasing message indices,
     *  and the sensor data values will be copied into the corresponding sensor data vector in the
     *  appropriate order.
     *
     *  Finally, @ref aggregated_data_valid_ will be set to FALSE to ensure that subsequent calls
     *  to this function will have no effect, until another sensor data message object is assigned
     *  to this composite object and @ref aggregated_data_valid_ is set to TRUE.
     *
     *  @exception  util::log::MessageException if an error occurs while aggregating the sensor
     *              data.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     */
    void AggregateSensorData() ATTR_NO_EXCEPTIONS {

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            // Check if sensor data is already aggregated.
            if(is_aggregated_data_valid())
                return;

            // Clear existing sensor data, if any.
            aggregated_pres_data_.clear();
            aggregated_saln_data_.clear();
            aggregated_temp_data_.clear();
            aggregated_aux_temp_data_.clear();

            // Reallocate storage if necessary.
            aggregated_pres_data_.reserve(num_pres_samples());
            aggregated_saln_data_.reserve(num_saln_samples());
            aggregated_temp_data_.reserve(num_temp_samples());
            aggregated_aux_temp_data_.reserve(num_aux_temp_samples());

            // Aggregate the data.
            for_each(
                    pres_messages_.begin(),
                    pres_messages_.end(),
                    [&](const shared_ptr<PRES_CLASS>& msg) {
                        if(msg)
                        aggregated_pres_data_.insert(
                                aggregated_pres_data_.end(),
                                msg->raw_sensor_data().begin(),
                                msg->raw_sensor_data().end());
                    });
            for_each(
                    saln_messages_.begin(),
                    saln_messages_.end(),
                    [&](const shared_ptr<SALN_CLASS>& msg) {
                        if(msg)
                        aggregated_saln_data_.insert(
                                aggregated_saln_data_.end(),
                                msg->raw_sensor_data().begin(),
                                msg->raw_sensor_data().end());
                    });
            for_each(
                    temp_messages_.begin(),
                    temp_messages_.end(),
                    [&](const shared_ptr<TEMP_CLASS>& msg) {
                        if(msg)
                        aggregated_temp_data_.insert(
                                aggregated_temp_data_.end(),
                                msg->raw_sensor_data().begin(),
                                msg->raw_sensor_data().end());
                    });
            for_each(
                    aux_temp_messages_.begin(),
                    aux_temp_messages_.end(),
                    [&](const shared_ptr<TEMP_CLASS>& msg) {
                        if(msg)
                        aggregated_aux_temp_data_.insert(
                            aggregated_aux_temp_data_.end(),
                                msg->raw_sensor_data().begin(),
                                msg->raw_sensor_data().end());
                    });
            set_aggregated_data_valid(true);

        }CATCH_BLOCK("Failed to aggregate profile data!")

    }

    /** Set the value of the @ref aggregated_data_valid_ field.
     *
     *  @param[in]  valid   TRUE if the aggregated data is valid, or FALSE otherwise.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     */
    void set_aggregated_data_valid(bool valid) {
        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        aggregated_data_valid_ = valid;
    }

public:

    /* Public Data Members: */

    /** The constructor.
     *
     *  The field @ref aggregated_data_valid_ initialized to FALSE, and the aggregated sensor
     *  data vectors reserve @ref XMSG_INIT_SAM_N_RESERVE samples.
     *
     */
    AbstractXSensorDataComposite() :
                    aggregated_data_valid_(false) {

        // Pre-allocate memory for aggregated data.
        aggregated_pres_data_.reserve(XMSG_INIT_SAM_N_RESERVE);
        aggregated_saln_data_.reserve(XMSG_INIT_SAM_N_RESERVE);
        aggregated_temp_data_.reserve(XMSG_INIT_SAM_N_RESERVE);

    }

    /** The destructor.
     *
     */
    virtual ~AbstractXSensorDataComposite() {
        /* Do nothing. */
    }

    /** Assign a pressure data message to this composite data object.
     *
     *  The pressure data message will be emplaced into the sensor data message vector in order of
     *  increasing message indices. If a message with the same index is as @p x is already contained
     *  in the vector, then an exception will be thrown.
     *
     *  @param[in]  x   A shared pointer to the pressure data message.
     *
     *  @exception  util::log::MessageException if @p x is a null pointer, a duplicate message index
     *              is encountered, or any other error occurs.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     */
    void AssignPressureMessage(const PresMsgPtr_t& x) {
        typename PresMsgVector_t::iterator it;
        try {

            ldbg() << "Assigning pressure data message to composite sensor data object." << endl;

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            // Check the pointer to the sample data message.
            if(!x)
                EXC_THROW("Pointer to message object is NULL!");

            // Iterate to emplacement position.
            for(it = pres_messages_.begin(); it < pres_messages_.end(); it++) {
                if(x->xmsg_index() == (*it)->xmsg_index()) {
                    EXC_THROW("Duplicate X-Message index encountered!");
                } else if(x->xmsg_index() < (*it)->xmsg_index())
                    break;
            }

            // Emplace element at position.
            set_aggregated_data_valid(false);
            pres_messages_.emplace(it, x);

            // Aggregate the sensor data.
            AggregateSensorData();

        }CATCH_BLOCK("Failed to assign pressure message!")

    }

    /** Assign a salinity data message to this composite data object.
     *
     *  The salinity data message will be emplaced into the sensor data message vector in order of
     *  increasing message indices. If a message with the same index is as @p x is already contained
     *  in the vector, then an exception will be thrown.
     *
     *  @param[in]  x   A shared pointer to the salinity data message.
     *
     *  @exception  util::log::MessageException if @p x is a null pointer, a duplicate message index
     *              is encountered, or any other error occurs.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     */
    void AssignSalinityMessage(const SalnMsgPtr_t& x) {
        typename SalnMsgVector_t::iterator it;

        try {
            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            ldbg() << "Assigning salinity data message to composite sensor data object." << endl;

            // Check the pointer to the sample data message.
            if(!x)
                EXC_THROW("Pointer to message object is NULL!");

            // Iterate to emplacement position.
            for(it = saln_messages_.begin(); it < saln_messages_.end(); it++) {
                if(x->xmsg_index() == (*it)->xmsg_index()) {
                    EXC_THROW("Duplicate XMessage index encountered!");
                } else if(x->xmsg_index() < (*it)->xmsg_index())
                    break;
            }

            // Emplace element at position.
            set_aggregated_data_valid(false);
            saln_messages_.emplace(it, x);

            // Aggregate the sensor data.
            AggregateSensorData();

        }CATCH_BLOCK("Failed to assign salinity message!")

    }

    /** Assign a temperature data message to this composite data object.
     *
     *  The temperature data message will be emplaced into the sensor data message vector in order
     *  of  increasing message indices. If a message with the same index is as @p x is already
     *  contained in the vector, then an exception will be thrown.
     *
     *  @param[in]  x   A shared pointer to the temperature data message.
     *
     *  @exception  util::log::MessageException if @p x is a null pointer, a duplicate message index
     *              is encountered, or any other error occurs.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     */
    void AssignTemperatureMessage(const TempMsgPtr_t& x) {
        typename TempMsgVector_t::iterator it;

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            ldbg() << "Assigning temperature data message to composite sensor data object."
                      << endl;

            // Check the pointer to the sample data message.
            if(!x)
                EXC_THROW("Pointer to message object is NULL!");

            // Iterate to emplacement position.
            for(it = temp_messages_.begin(); it < temp_messages_.end(); it++) {
                if(x->xmsg_index() == (*it)->xmsg_index()) {
                    EXC_THROW("Duplicate XMessage index encountered!");
                } else if(x->xmsg_index() < (*it)->xmsg_index())
                    break;
            }

            // Emplace element at position.
            set_aggregated_data_valid(false);
            temp_messages_.emplace(it, x);

            // Aggregate the sensor data.
            AggregateSensorData();

        }CATCH_BLOCK("Failed to assign temperature message!")

    }

    /** Assign an auxiliary-channel temperature message and re-aggregate the sensor data.
     *
     *  The message is inserted in order of message index, and the aggregated sensor data is
     *  rebuilt.
     *
     *  @param[in]  x       The auxiliary-channel temperature message to assign.
     *
     *  @throw      util::log::MessageException if @p x is NULL or an auxiliary temperature message
     *              with the same index has already been assigned.
     */
    void AssignAuxTemperatureMessage(const TempMsgPtr_t& x) {
        typename TempMsgVector_t::iterator it;

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            ldbg() << "Assigning aux. temperature data message to composite sensor data object."
                      << endl;

            // Check the pointer to the sample data message.
            if(!x)
                EXC_THROW("Pointer to message object is NULL!");

            // Iterate to emplacement position.
            for(it = aux_temp_messages_.begin(); it < aux_temp_messages_.end(); it++) {
                if(x->xmsg_index() == (*it)->xmsg_index()) {
                    EXC_THROW("Duplicate XMessage index encountered!");
                } else if(x->xmsg_index() < (*it)->xmsg_index())
                    break;
            }

            // Emplace element at position.
            set_aggregated_data_valid(false);
            aux_temp_messages_.emplace(it, x);

            // Aggregate the sensor data.
            AggregateSensorData();

        }CATCH_BLOCK("Failed to assign aux. temperature message!")

    }

    template<typename T = double> void CopyPressureDataToArray(
            T dest[],
            size_t max_len,
            int16_t gain,
            int16_t offs,
            float pres_correction,
            T fill_value = XMSG_SENS_BAD_PRES) const {

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            size_t nsam = num_pres_samples();
            double sam;

            for(size_t i = 0; i < nsam && i < max_len; ++i)

                if((sam = SamplePresAsFloat(i, gain, offs, pres_correction)) == XMSG_SENS_BAD_PRES)
                    dest[i] = fill_value;
                else
                    dest[i] = sam;

        }CATCH_BLOCK("Failed to copy pressure data to array!")

    }

    template<typename T = double> void CopySalinityDataToArray(
            T dest[],
            size_t max_len,
            int16_t gain,
            int16_t offs,
            T fill_value = XMSG_SENS_BAD_SALN) const {

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            size_t nsam = num_saln_samples();
            double sam;

            for(size_t i = 0; i < nsam && i < max_len; ++i)

                if((sam = SampleSalnAsFloat(i, gain, offs)) == XMSG_SENS_BAD_SALN)
                    dest[i] = fill_value;
                else
                    dest[i] = sam;

        }CATCH_BLOCK("Failed to copy salinity data to array!")

    }

    template<typename T = double> void CopyTemperatureDataToArray(
            T dest[],
            size_t max_len,
            int16_t gain,
            int16_t offs,
            T fill_value = XMSG_SENS_BAD_TEMP) const {

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            size_t nsam = num_temp_samples();
            double sam;

            for(size_t i = 0; i < nsam && i < max_len; ++i)

                if((sam = SampleTempAsFloat(i, gain, offs)) == XMSG_SENS_BAD_TEMP)
                    dest[i] = fill_value;
                else
                    dest[i] = sam;

        }CATCH_BLOCK("Failed to copy temperature data to array!")

    }

    /** Generate a listing string representation of the this composite sensor data object.
     *
     *  @param[in]  id_inst The unique instrument identifier for this composite sensor data object.
     *  @param[in]  id_dive The dive or cycle number for this composite data object.
     *  @param[in]  pg      The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po      The pressure sensor offset, in dBar.
     *  @param[in]  sg      The salinity sensor sensitivity scale factor, in LSB/PSU.
     *  @param[in]  so      The salinity sensor offset, in PSU.
     *  @param[in]  tg      The temperature sensor sensitivity scale factor, in LSB/degC.
     *  @param[in]  to      The temperature sensor offset, in degC.
     *  @param[in]  pc      The pressure correction, in dBar, to be subtracted computed pressure
     *                      value.
     *
     *  @throw      log::MessageException if an error occurs while generating the listing string.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     A listing string representation of this composite sensor data object.
     */
    virtual const string GenerateListingString(
            int id_inst,
            int id_dive,
            uint16_t pg,
            uint16_t po,
            uint16_t sg,
            uint16_t so,
            uint16_t tg,
            uint16_t to,
            float pc) const = 0;

    /** Get the total number of pressure sensor samples contained in the pressure data messages
     *  assigned to this composite data object.
     *
     *  @exception  util::log::MessageException if an error occurs.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The total number of pressure sensor samples contained in the pressure data
     *              messages assigned to this composite data object.
     */
    size_t num_pres_samples() const {
        size_t num_pres_samples = 0;

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            //  If the aggregated data is valid, then return the size of the aggregated data.
            if(is_aggregated_data_valid())
                return aggregated_pres_data_.size();

            // Iterate over pressure messages.
            for_each(
                    pres_messages_.begin(),
                    pres_messages_.end(),
                    [&](const shared_ptr<PRES_CLASS>& msg) {
                        if(msg)
                        num_pres_samples += msg->num_samples();
                    });

        }CATCH_BLOCK("Failed to get the number of pressure samples!")

        return num_pres_samples;
    }

    /** Get the total number of salinity sensor samples contained in the salinity data messages
     *  assigned to this composite data object.
     *
     *  @exception  util::log::MessageException if an error occurs.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The total number of pressure sensor samples contained in the pressure data
     *              messages assigned to this composite data object.
     */
    size_t num_saln_samples() const {
        size_t num_saln_samples = 0;

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            //  If the aggregated data is valid, then return the size of the aggregated data.
            if(is_aggregated_data_valid())
                return aggregated_saln_data_.size();

            // Iterate over salinity messages.
            for_each(
                    saln_messages_.begin(),
                    saln_messages_.end(),
                    [&](const shared_ptr<SALN_CLASS>& msg) {
                        if(msg)
                        num_saln_samples += msg->num_samples();
                    });

        }CATCH_BLOCK("Failed to get the number of salinity samples!")

        return num_saln_samples;
    }

    /** Get the total number of temperature sensor samples contained in the temperature data
     *  messages assigned to this composite data object.
     *
     *  @exception  util::log::MessageException if an error occurs.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The total number of pressure sensor samples contained in the pressure data
     *              messages assigned to this composite data object.
     */
    size_t num_temp_samples() const {
        size_t num_temp_samples = 0;

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            //  If the aggregated data is valid, then return the size of the aggregated data.
            if(is_aggregated_data_valid())
                return aggregated_temp_data_.size();

            // Iterate over temperature messages.
            for_each(
                    temp_messages_.begin(),
                    temp_messages_.end(),
                    [&](const shared_ptr<TEMP_CLASS>& msg) {
                        if(msg)
                        num_temp_samples += msg->num_samples();
                    });

        }CATCH_BLOCK("Failed to get the number of temperature samples!")

        return num_temp_samples;
    }

    size_t num_aux_temp_samples() const {
        size_t num_temp_samples = 0;

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            //  If the aggregated data is valid, then return the size of the aggregated data.
            if(is_aggregated_data_valid())
                return aggregated_aux_temp_data_.size();

            // Iterate over temperature messages.
            for_each(
                aux_temp_messages_.begin(),
                aux_temp_messages_.end(),
                    [&](const shared_ptr<TEMP_CLASS>& msg) {
                        if(msg)
                          num_temp_samples += msg->num_samples();
                    });

        }CATCH_BLOCK("Failed to get the number of aux temperature samples!")

        return num_temp_samples;
    }

    /** Return the maximum of the number of pressure, temperature, or salinity sensor samples in
     *  the sensor data messages assigned to this composite sensor data object.
     *
     *
     *
     *  @return     The maximum of the number of pressure, temperature, or salinity sensor samples.
     */
    size_t max_num_samples() const {
        return max(max(num_pres_samples(), num_saln_samples()), num_temp_samples());
    }

    /** Do the sensor data messages assigned to this composite data object have contiguous message
     *  indices?
     *
     *  This function is used, in part, to determine the sensor data is complete.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     TRUE if the sensor data messages have contiguous message indices or if there
     *              is less than two of each sensor data message, or FALSE otherwise.
     */
    bool HasContiguousMessageIndices() const {

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            ldbg() << "Checking if sensor data has contiguous message indices." << endl;

            // Check the pressure messages' indices.
            if(pres_messages_.size() > 1) {
                int idx = pres_messages_.at(0)->xmsg_index();

                typename PresMsgVector_t::const_iterator it;
                for(it = pres_messages_.begin() + 1; it < pres_messages_.end(); ++it)
                    if((*it)->xmsg_index() != ++idx)
                        return false;

            }

            // Check the salinity messages' indices.
            if(saln_messages_.size() > 1) {
                int idx = saln_messages_.at(0)->xmsg_index();

                typename SalnMsgVector_t::const_iterator it;
                for(it = saln_messages_.begin() + 1; it < saln_messages_.end(); ++it)
                    if((*it)->xmsg_index() != ++idx)
                        return false;

            }

            // Check the pressure messages' indices.
            if(temp_messages_.size() > 1) {
                int idx = temp_messages_.at(0)->xmsg_index();

                typename TempMsgVector_t::const_iterator it;
                for(it = temp_messages_.begin() + 1; it < temp_messages_.end(); ++it)
                    if((*it)->xmsg_index() != ++idx)
                        return false;

            }

            if(aux_temp_messages_.size() > 1) {
                int idx = aux_temp_messages_.at(0)->xmsg_index();

                typename TempMsgVector_t::const_iterator it;
                for(it = aux_temp_messages_.begin() + 1; it < aux_temp_messages_.end(); ++it)
                    if((*it)->xmsg_index() != ++idx)
                        return false;

            }

        }CATCH_BLOCK("Failed to determine if sensor data has contiguous message indices!")

        return true;
    }

    bool HasAuxTempData() const {
      return !aux_temp_messages_.empty();
    }

    /** Is the aggregated data valid?
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     TRUE if the aggregated sensor is valid, or FALSE otherwise.
     */
    bool is_aggregated_data_valid() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return aggregated_data_valid_;
    }

    /** Is the sensor data for this composite date message complete?
     *
     *  This function returns true if
     *  - this composite sensor data object is not empty,
     *  - the sensor data messages assigned to this composite data object have contiguous message
     *  indices,
     *  - this sensor data object contains the same number of temperature and pressure sensor
     *  values, and
     *  - the number of salinity data samples is either zero or equal to the number of pressure
     *  data samples.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     TRUE if the sensor data is complete, or FALSE otherwise.
     */
    bool IsDataComplete() const {

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            if(IsEmpty())
                return false;

            if(!HasContiguousMessageIndices())
                return false;

            if(num_pres_samples() != num_temp_samples())
                return false;

            if(num_aux_temp_samples() && (num_aux_temp_samples() != num_temp_samples()))
              return false;

            if(num_saln_samples() && (num_saln_samples() != num_pres_samples()))
                return false;

        }CATCH_BLOCK("Failed to determine if sensor data is complete!")

        return true;

    }

    /** Is this composite data object empty?
     *
     *  @return     TRUE if empty, or FALSE otherwise.
     */
    bool IsEmpty() const {

        return max_num_samples() == 0;

    }

    /** Remove the assigned pressure, temperature and salinity messages and their aggregated data.
     *
     *  @warning    The auxiliary temperature messages and aggregated auxiliary temperature data are
     *              not cleared, so re-assigning them after a reset fails with a duplicate index.
     *
     *  @throw      util::log::MessageException if an error occurs.
     */
    void Reset() {

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            set_aggregated_data_valid(false);
            aggregated_pres_data_.clear();
            aggregated_saln_data_.clear();
            aggregated_temp_data_.clear();
            pres_messages_.clear();
            saln_messages_.clear();
            temp_messages_.clear();

        }CATCH_BLOCK("Failed to reset sensor data object!")

    }

    /** Get the @f$i^{th}@f$ pressure sensor value, in raw counts
     *
     *  @param[in]  i       The index of the pressure sensor sample to return. If the index exceeds
     *                      the number of values in the aggregated data vector, then
     *                      @ref XMSG_SENS_BAD will be returned.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     * @return      The @f$i^{th}@f$ pressure sensor value, in raw counts, or @ref XMSG_SENS_BAD if
     *              the index @p i exceeds the number of pressure sensor samples in the aggregated
     *              data vector.
     */
    int32_t SamplePres(size_t i) const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return i < num_pres_samples() ? aggregated_pres_data_.at(i) : XMSG_SENS_BAD;
    }

    /** Get the @f$i^{th}@f$ pressure sensor value, in dBar.
     *
     *  @param[in]  i       The index of the sensor value. If the index exceeds the number of
     *                      values in the aggregated data vector, then @ref XMSG_SENS_BAD_PRES will
     *                      be returned.
     *  @param[in]  pg      The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po      The pressure sensor offset, in dBar.
     *  @param[in]  pc      The pressure correction, in dBar, to be subtracted computed pressure
     *                      value.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The @f$i^{th}@f$ pressure sensor value, in dBar. If the index exceeds the
     *              number of values in the aggregated data vector, then @ref XMSG_SENS_BAD_PRES
     *              will be returned.
     */
    template<typename T = double> T SamplePresAsFloat(
            size_t i,
            int16_t pg,
            int16_t po,
            float pc) const {

        T val = XMSG_SENS_BAD_PRES;

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            if(i < num_pres_samples() && pg) {

                val = (T) aggregated_pres_data_.at(i) / (T) pg - (T) po - pc;

                if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
                    val = XMSG_SENS_BAD_PRES;

            }

        }CATCH_BLOCK("Failed to get sensor data value from raw sample counts!")

        return val;
    }

    /** Get the @f$i^{th}@f$ salinity sensor value, in raw counts
     *
     *  @param[in]  i       The index of the salinity sensor sample to return. If the index exceeds
     *                      the number of values in the aggregated data vector, then
     *                      @ref XMSG_SENS_BAD will be returned.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     * @return      The @f$i^{th}@f$ salinity sensor value, in raw counts, or @ref XMSG_SENS_BAD if
     *              the index @p i exceeds the number of pressure sensor samples in the aggregated
     *              data vector.
     */
    int32_t SampleSaln(size_t i) const {
        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return i < num_saln_samples() ? aggregated_saln_data_.at(i) : XMSG_SENS_BAD;
    }

    /** Get the @f$i^{th}@f$ salinity sensor value, in PSU.
     *
     *  @param[in]  i       The index of the sensor value. If the index exceeds the number of
     *                      values in the aggregated data vector, then @ref XMSG_SENS_BAD_SALN will
     *                      be returned.
     *  @param[in]  gain    The salinity sensor sensitivity scale factor, in LSB/PSU.
     *  @param[in]  offs    The salinity sensor offset, in PSU.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The @f$i^{th}@f$ salinity sensor value, in PSU. If the index exceeds the
     *              number of values in the aggregated data vector, then @ref XMSG_SENS_BAD_SALN
     *              will be returned.
     */
    template<typename T = double> T SampleSalnAsFloat(size_t i, int16_t gain, int16_t offs) const {

        T val = XMSG_SENS_BAD_SALN;

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            if(i < num_saln_samples() && gain) {

                val = (T) aggregated_saln_data_.at(i) / (T) gain - (T) offs;

                if(val < XMSG_SENS_MIN_SALN || val > XMSG_SENS_MAX_SALN)
                    val = XMSG_SENS_BAD_SALN;
            }

        }CATCH_BLOCK("Failed to get sensor data value from raw sample counts!")

        return val;
    }

    /** Get the @f$i^{th}@f$ temperature sensor value, in raw counts
     *
     *  @param[in]  i       The index of the temperature sensor sample to return. If the index exceeds
     *                      the number of values in the aggregated data vector, then
     *                      @ref XMSG_SENS_BAD will be returned.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     * @return      The @f$i^{th}@f$ temperature sensor value, in raw counts, or @ref XMSG_SENS_BAD
     *              if the index @p i exceeds the number of pressure sensor samples in the
     *              aggregated data vector.
     */
    int32_t SampleTemp(size_t i) const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return i < num_temp_samples() ? aggregated_temp_data_.at(i) : XMSG_SENS_BAD;
    }

    /** Get the @f$i^{th}@f$ temperature sensor value, in degC.
     *
     *  @param[in]  i       The index of the sensor value. If the index exceeds the number of
     *                      values in the aggregated data vector, then @ref XMSG_SENS_BAD_SALN will
     *                      be returned.
     *  @param[in]  gain    The temperature sensor sensitivity scale factor, in LSB/degC.
     *  @param[in]  offs    The temperature sensor offset, in degC.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     The @f$i^{th}@f$ temperature sensor value, in degC. If the index exceeds the
     *              number of values in the aggregated data vector, then @ref XMSG_SENS_BAD_TEMP
     *              will be returned.
     */
    template<typename T = double> T SampleTempAsFloat(size_t i, int16_t gain, int16_t offs) const {

        T val = XMSG_SENS_BAD_TEMP;

        try {
            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            if(i < num_temp_samples() && gain) {
                val = (T) aggregated_temp_data_.at(i) / (T) gain - (T) offs;

                if(val < XMSG_SENS_MIN_TEMP || val > XMSG_SENS_MAX_TEMP)
                    val = XMSG_SENS_BAD_TEMP;

            }
        }CATCH_BLOCK("Failed to get sensor data value from raw sample counts!")

        return val;
    }

    template<typename T = double> T SampleAuxTempAsFloat(size_t i, int16_t gain, int16_t offs) const {

        T val = XMSG_SENS_BAD_TEMP;

        try {
            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            if(i < num_aux_temp_samples() && gain) {
                val = (T) aggregated_aux_temp_data_.at(i) / (T) gain - (T) offs;

                if(val < XMSG_SENS_MIN_TEMP || val > XMSG_SENS_MAX_TEMP)
                    val = XMSG_SENS_BAD_TEMP;

            }
        }CATCH_BLOCK("Failed to get sensor data value from raw sample counts!")

        return val;
    }

};

/* Functions: */

} /* namespace xmsg */
} /* namespace util */

#endif /* ABSTRACTXSENSORDATACOMPOSITE_H_ */
