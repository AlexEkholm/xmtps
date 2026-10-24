/** The @ref util::xmsg::AbstractXTimeSeriesDataComposite class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jun 17, 2016 3:03:57 PM
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

#ifndef DATA_XMSG_ABSTRACTXTIMESERIESDATACOMPOSITE_H_
#define DATA_XMSG_ABSTRACTXTIMESERIESDATACOMPOSITE_H_

/* Includes: */

// local
#include "AbstractXComposite.h"
#include "XSurfaceData.h"

/* Definitions: */



namespace util {
namespace xmsg {

/* Types: */

/** The abstract time-series data composite class template.
 *
 *  This template aggregates the indexed time-series X-messages of type @p MSG_CLASS received for
 *  a single dive into one time series. It is used for the surface time-series data (see
 *  @ref XCompositeSurfaceData).
 */
template<class MSG_CLASS> class AbstractXTimeSeriesDataComposite/*:
        public util::thread::RecursiveLockable */{

public:

    /* Type Definitions: */

    typedef vector<uint32_t> ElapsedTimeVector_t;

    typedef shared_ptr<MSG_CLASS> MsgPtr_t;

    typedef vector<MsgPtr_t> MsgVector_t;

    typedef typename MsgVector_t::iterator MsgVectorIter_t;

    typedef typename MsgVector_t::const_iterator MsgVectorIterConst_t;

    typedef vector<uint8_t> PhaseCodeVector_t;

    typedef int32_t Sample_t;


    typedef int64_t DerivedSample_t;

    typedef vector<Sample_t> SampleVector_t;

public:

    /* Public Static Data Members: */

    static const int32_t DERIVED_FIELDS_STATIC_GAIN_EXP;

private:

    /* Private Data Members: */

    /** The aggregated data is valid flag.
     *
     *  This flag indicates the status of the aggregated sensor data. It is used to ensure the
     *  composite sensor data is aggregated only once following each assignment of sensor data to
     *  the composite data message.
     *
     *  This field is set to FALSE during initialization, and is set to FALSE each time a sensor
     *  data message is assigned to the composite data message. Each time @c AggregateSensorData()
     *  is called, the data will be aggregated, and this field will be set to TRUE.
     *
     */
    bool aggregated_data_valid_;


    DerivedSample_t pres_max_;

    DerivedSample_t pres_mean_;

    DerivedSample_t pres_min_;

//protected:
//
//    /* Protected Data Members: */


    ElapsedTimeVector_t aggregated_elapsed_time_;

    PhaseCodeVector_t aggregated_phase_codes_;

    SampleVector_t aggregated_pres_data_;

    SampleVector_t aggregated_saln_data_;

    SampleVector_t aggregated_temp_data_;

    MsgVector_t messages_;


public:

    /* Public Static Member Functions: */

    static int32_t derived_fields_static_gain_exp() {

        return DERIVED_FIELDS_STATIC_GAIN_EXP;

    }

public:

    /* Public Member Functions: */

    AbstractXTimeSeriesDataComposite() :
                    aggregated_data_valid_(false),
                    pres_max_(XMSG_SENS_BAD),
                    pres_mean_(XMSG_SENS_BAD),
                    pres_min_(XMSG_SENS_BAD) {

        // Pre-allocate memory for aggregated data.
        aggregated_pres_data_.reserve(XMSG_INIT_SAM_N_RESERVE);
        aggregated_saln_data_.reserve(XMSG_INIT_SAM_N_RESERVE);
        aggregated_temp_data_.reserve(XMSG_INIT_SAM_N_RESERVE);

    }

    virtual ~AbstractXTimeSeriesDataComposite() {

        /* Do nothing. */

    }

    virtual void AssignMessage(const MsgPtr_t& x) {

        MsgVectorIter_t it;

        try {

            ldbg() << "Assigning data message to composite time-series data object." << endl;

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            // Check the pointer to the sample data message.
            if(!x)
                EXC_THROW("Pointer to message object is NULL!");

            // Iterate to emplacement position.
            for(it = messages_.begin(); it < messages_.end(); it++)

                // Compare message indices.
                if(x->xmsg_index() == (*it)->xmsg_index()) {

                    EXC_THROW("Duplicate X-Message index encountered!");

                } else if(x->xmsg_index() < (*it)->xmsg_index())

                    break;

            set_aggregated_data_valid(false);

            // Emplace element at position.
            messages_.emplace(it, x);

            // Aggregate the sensor data.
            AggregateSensorData();

        }CATCH_BLOCK("Failed to assign time-series message to composite object!")

    }

    virtual const string GenerateListingString(
            uint16_t& pg,
            uint16_t& po,
            uint16_t& sg,
            uint16_t& so,
            uint16_t& tg,
            uint16_t& to,
            float pres_correction) const {

        MsgVectorIterConst_t it;
        ostringstream os;

        try {

            os << AbstractXFallRise::GenerateListingComment();

            for_each(messages_.begin(), messages_.end(), [&](const MsgPtr_t& msg) {

                if(msg) os << endl << msg->GenerateListingString(pg, po, sg, so, tg, to, pres_correction);

            });

        }CATCH_BLOCK("Failed to generate listing string for composite time-series data object!")

        return os.str();
    }

    virtual bool HasContiguousMessageIndices() const {

        size_t idx = 0;

        try {

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            ldbg() << "Checking if -series data has contiguous message indices." << endl;

            // Check the pressure messages' indices.
            if(messages_.size() > 1) {

                // Iterate over the messages.
                for(MsgVectorIterConst_t it = messages_.begin(); it < messages_.end(); ++it)

                    // Compare the actual vs. expected messages indices
                    if((*it)->xmsg_index() != idx++)

                        return false;

            }

        }CATCH_BLOCK("Failed to determine if time-series data has contiguous message indices!")

        return true;
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
        //lock_guard<recursive_mutex> guard(mutex_);

        return aggregated_data_valid_;
    }

    bool IsDataComplete() const {

        try {

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            return !IsEmpty();

        }CATCH_BLOCK("Failed to determine if time-series data is complete!")

        return true;

    }

    /** Is this composite data object empty?
     *
     *  @return     TRUE if empty, or FALSE otherwise.
     */
    bool IsEmpty() const {

        return SampleCount() == 0;

    }

    size_t NumPresSamples() const {

        size_t num_samples = 0;

        try {

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            // If the aggregated data is valid, then return the size of the aggregated data.
            if(is_aggregated_data_valid())
                return aggregated_pres_data_.size();

            // Check if there are any messages.
            if(!messages_.size())
                return 0;

            // Determine message encoding type.
            switch(messages_.at(0)->data_pack_format()) {
            case XRISEFALL_FMT_LEGACY:
            case XRISEFALL_FMT_STNDRD:
            case XRISEFALL_FMT_STNDRD_WITH_TEMP_AND_SALN:

                // Iterate over the messages.
                for_each(messages_.begin(), messages_.end(), [&](const MsgPtr_t msg) {
                    num_samples += msg->num_samples();
                });

                break;

            case XRISEFALL_FMT_UNKNOWN:
            default:

                EXC_THROW("Unsupported message encoding type!");

            }

        }CATCH_BLOCK("Failed to compute pressure sample count!")

        return num_samples;

    }

    size_t NumTempSamples() const {

        size_t num_samples = 0;

        try {

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            //  If the aggregated data is valid, then return the size of the aggregated data.
            if(is_aggregated_data_valid())
                return aggregated_temp_data_.size();

            // Check if there are any messages.
            if(!messages_.size())
                return 0;

            // Determine message encoding type.
            switch(messages_.at(0)->data_pack_format()) {

            case XRISEFALL_FMT_LEGACY:
            case XRISEFALL_FMT_STNDRD:

                /* Do nothing. */

                break;

            case XRISEFALL_FMT_STNDRD_WITH_TEMP_AND_SALN:

                // Iterate over the messages.
                for_each(messages_.begin(), messages_.end(), [&](const MsgPtr_t msg) {
                    num_samples += msg->num_samples();
                });

                break;

            case XRISEFALL_FMT_UNKNOWN:
            default:

                EXC_THROW("Unsupported message encoding type!");

            }

        }CATCH_BLOCK("Failed to compute temperature sample count!")

        return num_samples;

    }

    size_t NumSalnSamples() const {

        size_t num_samples = 0;

        try {

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            //  If the aggregated data is valid, then return the size of the aggregated data.
            if(is_aggregated_data_valid())
                return aggregated_saln_data_.size();

            // Check if there are any messages.
            if(!messages_.size())
                return 0;

            // Determine message encoding type.
            switch(messages_.at(0)->data_pack_format()) {

            case XRISEFALL_FMT_LEGACY:
            case XRISEFALL_FMT_STNDRD:

                /* Do nothing. */

                break;

            case XRISEFALL_FMT_STNDRD_WITH_TEMP_AND_SALN:

                // Iterate over the messages.
                for_each(messages_.begin(), messages_.end(), [&](const MsgPtr_t msg) {
                    num_samples += msg->num_samples();
                });

                break;

            case XRISEFALL_FMT_UNKNOWN:
            default:

                EXC_THROW("Unsupported message encoding type!");

            }

        }CATCH_BLOCK("Failed to compute salinity sample count!")

        return num_samples;

    }

    DerivedSample_t pres_max() const {

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        return pres_max_;
    }

    DerivedSample_t pres_mean() const {

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        return pres_mean_;
    }

    DerivedSample_t pres_min() const {

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        return pres_min_;
    }

    void ResetAggregatedData() {

        try {

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            // Clear aggregated data
            set_aggregated_data_valid(false);
            aggregated_elapsed_time_.clear();
            aggregated_phase_codes_.clear();
            aggregated_pres_data_.clear();
            aggregated_saln_data_.clear();
            aggregated_temp_data_.clear();
            set_pres_max(XMSG_SENS_BAD);
            set_pres_mean(XMSG_SENS_BAD);
            set_pres_min(XMSG_SENS_BAD);

        }CATCH_BLOCK("Failed to reset aggregated data!")

    }

    void Reset() {

        try {

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            // Reset data.
            ResetAggregatedData();
            messages_.clear();

        }CATCH_BLOCK("Failed to reset time-series data object!")

    }

    virtual size_t SampleCount() const {

        size_t num_samples = 0;

        try {

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            // Compute the number of samples.
            if(messages_.size())
                num_samples = std::max(
                        NumPresSamples(),
                        std::max(NumTempSamples(), NumSalnSamples()));

        }CATCH_BLOCK("Failed to get the number of samples in the composite time-series message!")

        return num_samples;
    }

    template<typename T = float> T PresMaxAsFloat(uint16_t gain, uint16_t offs,
            float pres_correction = 0 ) const {

        T val = XMSG_SENS_BAD_PRES;

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        if(gain) {

            val = ((T) pres_max() / (T) gain / (T)(1 << derived_fields_static_gain_exp()))
                    - (T) offs - pres_correction;

            if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
                val = XMSG_SENS_BAD_PRES;

        }

        return val;

    }

    template<typename T = float> T PresMeanAsFloat(uint16_t gain, uint16_t offs,
            float pres_correction = 0) const {

        T val = XMSG_SENS_BAD_PRES;

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        if(gain) {

            val = ((T) pres_mean() / (T) gain / (T)(1 << derived_fields_static_gain_exp()))
                    - (T) offs - pres_correction;

            if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
                val = XMSG_SENS_BAD_PRES;

        }

        return val;

    }

    template<typename T = float> T PresMinAsFloat(uint16_t gain, uint16_t offs,
            float pres_correction = 0) const {

        T val = XMSG_SENS_BAD_PRES;

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        if(gain) {

            val = ((T) pres_min() / (T) gain / (T)(1 << derived_fields_static_gain_exp()))
                    - (T) offs - pres_correction;

            if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
                val = XMSG_SENS_BAD_PRES;

        }

        return val;

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
        //lock_guard<recursive_mutex> guard(mutex_);

        return i < NumPresSamples() ? aggregated_pres_data_.at(i) : XMSG_SENS_BAD;

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
    template<typename T = float> T SamplePresAsFloat(
            size_t i,
            int16_t pg,
            int16_t po,
            float pc) const {

        T val = XMSG_SENS_BAD_PRES;

        try {

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            if(i < NumPresSamples() && pg) {

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
        //lock_guard<recursive_mutex> guard(mutex_);

        return i < NumSalnSamples() ? aggregated_saln_data_.at(i) : XMSG_SENS_BAD;

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
    template<typename T = float> T SampleSalnAsFloat(size_t i, int16_t gain, int16_t offs) const {

        T val = XMSG_SENS_BAD_SALN;

        try {

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            if(i < NumSalnSamples() && gain) {

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
        //lock_guard<recursive_mutex> guard(mutex_);

        return i < NumTempSamples() ? aggregated_temp_data_.at(i) : XMSG_SENS_BAD;

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
    template<typename T = float> T SampleTempAsFloat(size_t i, int16_t gain, int16_t offs) const {

        T val = XMSG_SENS_BAD_TEMP;

        try {

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            if(i < NumTempSamples() && gain) {

                val = (T) aggregated_temp_data_.at(i) / (T) gain - (T) offs;

                if(val < XMSG_SENS_MIN_TEMP || val > XMSG_SENS_MAX_TEMP)
                    val = XMSG_SENS_BAD_TEMP;

            }

        }CATCH_BLOCK("Failed to get sensor data value from raw sample counts!")

        return val;
    }

protected:

    /* Protected Member Functions: */

    ElapsedTimeVector_t& aggregated_elapsed_time() {
        return aggregated_elapsed_time_;
    }

    PhaseCodeVector_t& aggregated_phase_codes() {
        return aggregated_phase_codes_;
    }

    SampleVector_t& aggregated_pres_data() {
        return aggregated_pres_data_;
    }

    SampleVector_t& aggregated_saln_data() {
        return aggregated_saln_data_;
    }

    SampleVector_t& aggregated_temp_data() {
      return aggregated_temp_data_;
    }

    virtual void AggregateSensorData() {

        MsgPtr_t msg;
        MsgVectorIter_t msg_iter;
        ptime ref_time;
        int64_t pres_sum = 0;
        Sample_t pres = 0;
        size_t num_valid_pres_samples = 0;
        vector<XSensorTripletCodeCounts>::const_iterator sample_it;

        try {

            // Get an exclusive lock.
            //lock_guard<recursive_mutex> guard(mutex_);

            ldbg() << "Aggregating time-series sensor data...";

            // Check if sensor data is already aggregated.
            if(is_aggregated_data_valid())
                return;

            // Clear existing sensor data, if any.
            ResetAggregatedData();

            if(!messages_.size()) {
                EXC_THROW("No messages found!");
            }

            // Determine message encoding type.
            switch(messages_.at(0)->data_pack_format()) {

            case XRISEFALL_FMT_LEGACY:
            case XRISEFALL_FMT_STNDRD:

                // Reserve storage for sample data.
                aggregated_pres_data().reserve(NumPresSamples());

                // Iterate over messages.
                for(msg_iter = messages_.begin(); msg_iter < messages_.end(); ++msg_iter) {

                    if(!(msg = *msg_iter)){
                        EXC_THROW("Null pointer to message!");
                    }

                    ref_time = msg->start_time();

                    // Iterate over samples.
                    for(sample_it = msg->sensor_data().begin();
                            sample_it < msg->sensor_data().end(); ++sample_it) {

                                // Append pressure value to aggregated data.
                                aggregated_pres_data().push_back(pres = sample_it->pressure_counts());

                                // Append phase code value to aggregated data.
                                aggregated_phase_codes().push_back(sample_it->phase_code());

                                // TODO Append elapsed time value to aggregated data.

                                /* Compute derived fields: */

                                // Ignore invalid values.
                                switch(pres) {
                                case XMSG_SENS_BAD:
                                case UINT16_MAX:
                                    continue;

                                default:
                                    break;
                                }

                                // Apply static gain.
                                pres <<= derived_fields_static_gain_exp();

                                // Max.
                                if(pres_max() < pres || pres_max() == XMSG_SENS_BAD)
                                    set_pres_max(pres);

                                // Sum.
                                pres_sum += pres;

                                // Min.
                                if(pres_min() > pres || pres_min() == XMSG_SENS_BAD)
                                    set_pres_min(pres);

                                // Count the number of valid samples.
                                num_valid_pres_samples++;

                            }

                }

                break;

            case XRISEFALL_FMT_STNDRD_WITH_TEMP_AND_SALN:

                // Reserve storage for sample data.
#               if defined( XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA) &&  XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA
                ldbg() << "Reserving " << NumPresSamples() << " samples.";
#               endif // defined( XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA) &&  XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA
                aggregated_pres_data().reserve(NumPresSamples());
                aggregated_temp_data().reserve(NumTempSamples());
                aggregated_saln_data().reserve(NumSalnSamples());

                // Iterate over messages.
                for(msg_iter = messages_.begin(); msg_iter < messages_.end(); ++msg_iter) {

                    if(!(msg = *msg_iter)){
                        EXC_THROW("Null pointer to message!");
                    }

#                   if defined( XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA) &&  XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA
                    {
                        ldbg() << "Processing surface message " << (int) msg->xmsg_index() << "...";
                    }
#                   endif // defined( XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA) &&  XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA

                    ref_time = msg->start_time();

#                   if defined( XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA) &&  XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA
                    {
                        ldbg() << "ref time:  "
                                  << util::date::DateUtils::GenerateIso8601String(ref_time);
                    }
#                   endif // defined( XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA) &&  XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA

                    // Iterate over samples.
                    for(sample_it = msg->sensor_data().begin();
                            sample_it < msg->sensor_data().end(); ++sample_it) {

#                       if defined(XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA) &&  XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA
                        {
                            const format fmt(
                                    "Processing surface data sample p = %5d t = %5d s = %5d ...");
                            ldbg()
                                    << format(fmt) % sample_it->pressure_counts()
                                            % sample_it->temperature_counts()
                                            % sample_it->salinity_counts();
                        }
#                       endif // defined( XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA) &&  XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA

                        // Append values to aggregated data.
#                       if defined(XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA) &&  XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA
                        {
                            ldbg() << "Appending samples to aggregated data.";
                        }
#                       endif
                        aggregated_pres_data().push_back(pres = sample_it->pressure_counts());
                        aggregated_temp_data().push_back(sample_it->temperature_counts());
                        aggregated_saln_data().push_back(sample_it->salinity_counts());
                        aggregated_phase_codes().push_back(sample_it->phase_code());

                        // TODO Append elapsed time value to aggregated data.

                        /* Compute derived fields: */

                        // Ignore invalid values.
                        switch(pres) {
                        case XMSG_SENS_BAD:
                        case UINT16_MAX:
                            continue;

                        default:
                            break;
                        }

#                       if defined(XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA) &&  XMSG_DEBUG_AGGREGATE_TIMESERIES_DATA
                        {
                            ldbg() << "Applying static gain "
                                      << (int) (1 << derived_fields_static_gain_exp());
                        }
#                       endif

                        // Apply static gain.
                        pres <<= derived_fields_static_gain_exp();

                        // Max.
                        if(pres_max() < pres || pres_max() == XMSG_SENS_BAD)
                            set_pres_max(pres);

                        // Sum.
                        pres_sum += pres;

                        // Min.
                        if(pres_min() > pres || pres_min() == XMSG_SENS_BAD)
                            set_pres_min(pres);

                        // Count the number of valid samples.
                        num_valid_pres_samples++;

                    }

                }

                {   // TODO Remove null salinity data (temp. only floats).
                    bool contains_valid_saln_data = false;
                    for(SampleVector_t::const_iterator it = aggregated_saln_data().begin();
                            it < aggregated_saln_data_.end(); ++it) {

                        if(*it != XMSG_SENS_BAD) {

                            // Found valid salinity data.
                            contains_valid_saln_data = true;
                                break;

                        }

                    }

                    if(!contains_valid_saln_data)
                        aggregated_saln_data_.clear();

                }

                break;

            case XRISEFALL_FMT_UNKNOWN:
            default:

                EXC_THROW("Unsupported message encoding type!");
                break;
            }

            if(!num_valid_pres_samples) {

                lwrn() << "No valid pressure samples found in time-series data.";

                // Update aggregated data status.
                set_aggregated_data_valid(false);

                return;
            }

            // Compute mean pressure.
            set_pres_mean(pres_sum / num_valid_pres_samples);

            // Update aggregated data status.
            set_aggregated_data_valid(true);

        }CATCH_BLOCK("Failed to aggregate time-series data!")

    }



    const MsgVector_t& messages() const {
        return messages_;
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
        //lock_guard<recursive_mutex> guard(mutex_);

        aggregated_data_valid_ = valid;
    }

    void set_pres_max(DerivedSample_t pres_max) {

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        pres_max_ = pres_max;
    }

    void set_pres_mean(DerivedSample_t pres_mean) {

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        pres_mean_ = pres_mean;
    }

    void set_pres_min(DerivedSample_t pres_min) {

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        pres_min_ = pres_min;
    }

};

/* Functions: */

} /* namespace xmsg */
} /* namespace util */

#endif /* DATA_XMSG_ABSTRACTXTIMESERIESDATACOMPOSITE_H_ */
