/** The @ref util::xmsg::XCompositeDrift class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 19, 2014 12:41:37 PM
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

#ifndef XCOMPOSITEDRIFT_H_
#define XCOMPOSITEDRIFT_H_

/* Includes */

// local
#include "AbstractXSensorDataComposite.h"
#include "../../utility/DateUtils.h"

namespace util {
namespace xmsg {

/** Class representing the aggregate drift data for a single dive, composed from one or more drift
 *  messages.
 *
 */
class XCompositeDrift:
        public AbstractXSensorDataComposite<XDriftPressure, XDriftSalinity, XDriftTemperature,
        XMSG_MAX_DRFT> {
public:

    /** The default constructor.
     *
     */
    XCompositeDrift() :
                    AbstractXSensorDataComposite<XDriftPressure, XDriftSalinity, XDriftTemperature,
                    XMSG_MAX_DRFT>() {
        /* Do nothing. */
    }

    /** The destructor.
     *
     */
    ~XCompositeDrift() {
        /* Do nothing. */
    }

    /** Assign a drift sample time message (0x78-0x7F).
     *
     *  @param[in]  x       The drift time message.
     *
     *  @throw      util::log::MessageException if @p x is NULL or a drift time message with the
     *              same index has already been assigned.
     */
    void AssignTimeMessage(const shared_ptr<XDriftTime>& x) {

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            if(!x)
                EXC_THROW("Pointer to message object is NULL!");

            // Iterate to emplacement position.
            auto it = time_messages_.begin();
            for(; it < time_messages_.end(); it++) {
                if(x->xmsg_index() == (*it)->xmsg_index())
                    EXC_THROW("Duplicate XMessage index encountered!");
                else if(x->xmsg_index() < (*it)->xmsg_index())
                    break;
            }

            time_messages_.emplace(it, x);

        }CATCH_BLOCK("Failed to assign drift time message!")
    }

    /** Remove all assigned messages, including the drift time messages.
     *
     */
    void Reset() {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        AbstractXSensorDataComposite<XDriftPressure, XDriftSalinity, XDriftTemperature,
            XMSG_MAX_DRFT>::Reset();
        time_messages_.clear();
    }

    /** Does this composite hold drift time messages?
     *
     *  @return     TRUE if at least one drift time message is assigned.
     */
    bool HasTimeData() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        return !time_messages_.empty();
    }

    /** Get the drift sample times, in seconds since the start of the drift state.
     *
     *  @return     The sample times of all assigned drift time messages, in order of message index.
     */
    vector<int32_t> SampleTimes() const {
        vector<int32_t> t;

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        for(const auto& msg : time_messages_)
            if(msg)
                t.insert(t.end(), msg->raw_sensor_data().begin(), msg->raw_sensor_data().end());

        return t;
    }

    /** Do the drift sample times line up with the drift samples?
     *
     *  @return     TRUE if there is one time per drift sample and the time message indices are
     *              contiguous.
     */
    bool IsTimeDataComplete() const {

        // Get an exclusive lock.
        lock_guard<recursive_mutex> guard(mutex_);

        if(time_messages_.empty() || SampleTimes().size() != max_num_samples())
            return false;

        int idx = time_messages_.front()->xmsg_index();
        for(const auto& msg : time_messages_)
            if(msg->xmsg_index() != idx++)
                return false;

        return true;
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
     *  @param[in]  drift_start The drift state start time, printed on the D line when the float
     *                      sends drift sample times, or not_a_date_time if unknown.
     *
     *  @throw      log::MessageException if an error occurs while generating the listing string.
     *
     *  @note       This object will be locked for the scope of the execution of this function.
     *              This function places a lock guard on the mutual exclusion (mutex) lock,
     *              thereby locking the float object and preventing access or modification of the
     *              object by any other thread. Any thread that does attempt to access or modify
     *              this object will block until the lock is released.
     *
     *  @return     A listing string representation of this composite sensor data object. When the
     *              drift sample times are complete, the section uses the fall/rise layout: the D
     *              line gives the drift start time, and each d line has the time (+seconds since
     *              the drift start) before the sensor values.
     */
    const string GenerateListingString(
            int id_inst,
            int id_dive,
            uint16_t pg,
            uint16_t po,
            uint16_t sg,
            uint16_t so,
            uint16_t tg,
            uint16_t to,
            float pc,
            const ptime& drift_start) const {
        ostringstream os;

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            // Add the sample times when the float sends them and they line up with the samples.
            const bool times = IsTimeDataComplete();
            const vector<int32_t> t = times ? SampleTimes() : vector<int32_t>();

            if(times) {

                /* With sample times, use the fall/rise layout: one header, a D line with the
                 * drift start time, and d lines with the time (+seconds since the drift start)
                 * before the sensor values. */

                static const format fmt0("#   %5s %4s %4s %20s %9s %7s %7s");
                static const format fmt1("D   %5d %4d %4d %20s");
                static const format fmt2("d   %5d %4d %4d %+20d %9s %7.3f %7.3f");

                os << format(fmt0) % "SN" % "dive" % "nSam" % "time" % "pres" % "temp" % "saln"
                   << endl
                   << format(fmt0) % "" % "" % "" % "(UTC/+sec)" % "(dBar)" % "(degC)" % "(PSU)"
                   << endl
                   << format(fmt1) % id_inst % id_dive % max_num_samples()
                       % util::date::DateUtils::GenerateIso8601String(drift_start);

                if(!IsDataComplete()) {
                    os << endl
                       << "#   (Incomplete Drift Data)";
                    return os.str();
                }

                for(size_t i = 0; i < max_num_samples(); i++)
                    os << endl
                       << format(fmt2) % id_inst % id_dive % i % t.at(i)
                           % XMsgFormatPressure(SamplePresAsFloat(i, pg, po, pc))
                           % SampleTempAsFloat(i, tg, to) % SampleSalnAsFloat(i, sg, so);

                return os.str();
            }

            // Print the header line.
//            static const format fmt1("#   %5s %4s %11s");
            static const format fmt2("D   %5d %4d %11d");
            os << "#      SN dive  numSamples"
//               << format(fmt1) % "SN" % "dive" % "numSamples"
               << endl
               << format(fmt2) % id_inst % id_dive % max_num_samples();

            // Check if the data is complete.
            if(!IsDataComplete()) {
                os << endl
                   << "#   (Incomplete Drift Data)";
                return os.str();
            }

            if(HasTimeData())
                os << endl
                   << "#   (Incomplete Drift Time Data)";

            {
//              static const format fmt1("#   %5s %4s %11s %11s %11s %11s");
//              static const format fmt2("#    %5s %4s %11s %11s %12s %11s");
              static const format fmt3("d   %5d %4d %11d %11s %11.3f %11.3f");

              // Iterate over the sensor data samples.
              for(size_t i = 0; i < max_num_samples(); i++) {
                if(i % 50 == 0) {
                  os << endl
                     << "#---------------------------------------------------------------------------------------------------"
                     << endl
//                     << format(fmt1) % "SN" % "dive" % "sampleIdx" % "pres" % "temp" % "saln"
                     << "#      SN dive   sampleIdx        pres        temp        saln"
                     << endl
//                     << format(fmt2) % "" % "" % "" % "(dBar)" % "(degC)" % "(PSU)";
                     << "#                                (dBar)      (degC)      (PSU)";
                }

                os << endl
                   << format(fmt3) % id_inst % id_dive % i % XMsgFormatPressure(SamplePresAsFloat(i, pg, po, pc))
                       % SampleTempAsFloat(i, tg, to) % SampleSalnAsFloat(i, sg, so);
              }
            }

        }CATCH_BLOCK("Failed to generate listing string for composite profile data (binned)!")

        return os.str();
    }

    /** Generate a listing string representation of this composite drift data object, without
     *  sample date/times.
     *
     *  @see        GenerateListingString(int, int, uint16_t, uint16_t, uint16_t, uint16_t,
     *              uint16_t, uint16_t, float, const ptime&) const
     */
    const string GenerateListingString(
            int id_inst,
            int id_dive,
            uint16_t pg,
            uint16_t po,
            uint16_t sg,
            uint16_t so,
            uint16_t tg,
            uint16_t to,
            float pc) const {
        return GenerateListingString(id_inst, id_dive, pg, po, sg, so, tg, to, pc,
                                     ptime(not_a_date_time));
    }

    /** Generate a PHY entry string from this composite drift data object.
     *
     *  @param[in]  pg      The pressure sensor sensitivity scale factor, in LSB/dBar.
     *  @param[in]  po      The pressure sensor offset, in dBar.
     *  @param[in]  sg      The salinity sensor sensitivity scale factor, in LSB/PSU.
     *  @param[in]  so      The salinity sensor offset, in PSU.
     *  @param[in]  tg      The temperature sensor sensitivity scale factor, in LSB/degC.
     *  @param[in]  to      The temperature sensor offset, in degC.
     *  @param[in]  pc      The pressure correction, in dBar, to be subtracted computed pressure
     *                      value.
     *
     *  @throw      log::MessageException if an error occurs while generating the PHY string.
     *
     *  @return     A PHY entry string representation of this composite drift dataobject.
     */
    const string GeneratePhyString(
            uint16_t pg,
            uint16_t po,
            uint16_t sg,
            uint16_t so,
            uint16_t tg,
            uint16_t to,
            float pc) const {
        ostringstream os;

        try {

            // Get an exclusive lock.
            lock_guard<recursive_mutex> guard(mutex_);

            if(num_saln_samples()) {

                static const format fmt("%8.2f %7.3f %7.3f");

                // Iterate over the sensor data samples.
                for(size_t i = 0; i < max_num_samples(); i++)
                    os << endl
                       << format(fmt) % SamplePresAsFloat(i, pg, po, pc)
                               % SampleTempAsFloat(i, tg, to) % SampleSalnAsFloat(i, sg, so);

            } else {

                static const format fmt("%8.2f %7.3f");

                // Iterate over the sensor data samples.
                for(size_t i = 0; i < max_num_samples(); i++)
                    os << endl
                       << format(fmt) % SamplePresAsFloat(i, pg, po, pc)
                               % SampleTempAsFloat(i, tg, to);
            }

        }CATCH_BLOCK("Failed to generate PHY string for composite drift data!")

        return os.str();
    }

private:

    /** The drift sample time messages, in order of increasing message index. */
    static_vector<shared_ptr<XDriftTime>, XMSG_MAX_DRFT> time_messages_;

};

} /* namespace xmsg */
} /* namespace util */

#endif /* XCOMPOSITEDRIFT_H_ */
