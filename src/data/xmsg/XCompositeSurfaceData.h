/** The @ref util::xmsg::XCompositeSurfaceData class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Jul 27, 2014 3:18:55 PM
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

#ifndef XCOMPOSITESURFACEPRESSURE_H_
#define XCOMPOSITESURFACEPRESSURE_H_

/* Includes: */

// local
#include "AbstractXTimeSeriesDataComposite.h"

/* Definitions */



namespace util {
namespace xmsg {

/* Class Definition: */

/** Class representing the aggregate surface pressure data for a single dive, composed from one or
 *  more surface pressure messages.
 *
 */
class XCompositeSurfaceData:
        public AbstractXTimeSeriesDataComposite<XSurfaceData> {

private:

    /* Private Data Members: */

    int pres_num_samples_;

    DerivedSample_t pres_var_;

protected:

    /* Protected Member Functions: */

    void set_pres_var(DerivedSample_t pres_var) {

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        pres_var_ = pres_var;
    }

    void set_pres_num_samples(int pres_num_samples) {

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        pres_num_samples_ = pres_num_samples;
    }

public:

    /* Member Functions: */

    /** Default constructor.
     *
     */
    XCompositeSurfaceData() :
                    AbstractXTimeSeriesDataComposite<XSurfaceData>(),
                    pres_num_samples_(-1),
                    pres_var_(XMSG_SENS_BAD_PRES) {

        /* Do nothing. */

    }

    /** Destructor.
     *
     */
    ~XCompositeSurfaceData() {

        /* Do nothing. */

    }

    /** Aggregate the surface time series and compute outlier-filtered pressure statistics.
     *
     *  First aggregates the messages as in
     *  @ref AbstractXTimeSeriesDataComposite::AggregateSensorData(), then refines the surface
     *  pressure statistics in three passes over the valid samples (after the static gain):
     *   -# compute the pressure variance about the mean;
     *   -# discard samples more than two standard deviations from the mean, and recompute the
     *      maximum, minimum and mean from the rest;
     *   -# recompute the variance from the remaining samples.
     *
     *  These statistics drive the surface pressure offset correction (see
     *  @c XFloatDiveData::UpdateDerivedSurfaceDataFields()).
     *
     *  @throw      util::log::MessageException if a pass finds no valid samples.
     */
    void AggregateSensorData() {

        Sample_t pres;
        Sample_t pres_std_dev = 0;
        int64_t pres_sum = 0;
        int64_t pres_var_sum = 0;
        size_t valid_sample_count = 0;

#       if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
        ostringstream os;
#       endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA

        try {

            // Update aggregated data status.
            set_aggregated_data_valid(false);

#           if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
            {
                os << "Aggregating sensor data..." << endl;
            }
#           endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA

            AbstractXTimeSeriesDataComposite<XSurfaceData>::AggregateSensorData();

            /* First Iteration: Compute surface pressure variance.
             *
             */

#           if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
            {
                os << "First iteration:" << endl;
            }
#           endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA

            for(auto it = aggregated_pres_data().begin(); it < aggregated_pres_data().end(); ++it) {

                pres = *it;

                // Discard null samples.
                switch(pres) {
                case XMSG_SENS_BAD:
                case UINT16_MAX:
#                   if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
                    {
                        lwrn() << "Questionable surface pressure value: " << pres;
                    }
#                   endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
                    continue;

                default:
                    break;
                }

                // Apply static gain.
                pres <<= derived_fields_static_gain_exp();

                valid_sample_count++;
                pres_var_sum += std::pow(pres - pres_mean(), 2);

#               if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
                {
                    static const format fmt("  count = %3d pres = %15d    pres_var_sum = %15d");
                    os << format(fmt) % valid_sample_count % pres % pres_var_sum << endl;
                }
#               endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
            }

#           if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
            {
                ldbg() << os.str();
                os.str("");
                os.clear();
            }
#           endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA

            if(!valid_sample_count) {

                EXC_THROW("Zero valid samples on first iteration!");
            }

            // Compute variance and std. dev.
            set_pres_var(pres_var_sum / valid_sample_count);
            pres_std_dev = sqrt(pres_var());

            /* Second Iteration: Recompute derived fields after discarding outliers.
             *
             */

            valid_sample_count = 0;
            set_pres_max(XMSG_SENS_BAD);
            set_pres_min(XMSG_SENS_BAD);

#           if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
            {
                os << "Second iteration:" << endl;
            }
#           endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA

            for(auto it = aggregated_pres_data().begin(); it < aggregated_pres_data().end(); ++it) {

                pres = *it;

                // Discard null samples.
                switch(pres) {
                case XMSG_SENS_BAD:
                case UINT16_MAX:
#                   if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
                    {
                        lwrn() << "Questionable surface pressure value: " << pres;
                    }
#                   endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
                    continue;

                default:
                    break;
                }

                // Apply static gain.
                pres <<= derived_fields_static_gain_exp();

                if(std::abs(pres - pres_mean()) > 2 * pres_std_dev)
                    continue;

                valid_sample_count++;

                // Max.
                if(pres_max() < pres || pres_max() == XMSG_SENS_BAD)
                    set_pres_max(pres);

                // Sum.
                pres_sum += pres;

                // Min.
                if(pres_min() > pres || pres_min() == XMSG_SENS_BAD)
                    set_pres_min(pres);

#               if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
                {
                    static const format fmt("  count = %3d pres = %15d    pres_sum = %15d");
                    os << format(fmt) % valid_sample_count % pres % pres_sum << endl;
                }
#               endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA

            }

            // Update mean pressure.
            set_pres_mean(pres_sum/valid_sample_count);

#           if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
            {
                ldbg() << os.str();
                os.str("");
                os.clear();
            }
#           endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA

            if(!valid_sample_count) {
                EXC_THROW("Zero valid samples on second iteration!");
            }

            /* Third Iteration: Recompute variance after discarding outliers.
             *
             */

            pres_var_sum = 0;
            valid_sample_count = 0;

#           if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
            {
                os << "Third iteration:" << endl;
            }
#           endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA


            for(auto it = aggregated_pres_data().begin(); it < aggregated_pres_data().end(); ++it) {

                pres = *it;

                // Discard null samples.
                switch(pres) {
                case XMSG_SENS_BAD:
                case UINT16_MAX:
#                   if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
                    {
                        lwrn() << "Questionable surface pressure value: " << pres;
                    }
#                   endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
                    continue;

                default:
                    break;
                }

                // Apply static gain.
                pres <<= derived_fields_static_gain_exp();

                if(std::abs(pres - pres_mean()) > 2 * pres_std_dev)
                    continue;

                valid_sample_count++;
                pres_var_sum += std::pow(pres - pres_mean(), 2);


#               if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
                {
                    static const format fmt("  count = %3d pres = %15d    pres_var_sum = %15d");
                    os << format(fmt) % valid_sample_count % pres % pres_var_sum << endl;
                }
#               endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA

            }

#           if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
            {
                ldbg() << os.str();
            }
#           endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA

            if(!valid_sample_count) {

                EXC_THROW("Zero valid samples on third iteration!");
            }

            // Recompute mean and variance.
            set_pres_num_samples(valid_sample_count);
            set_pres_var(pres_var_sum / valid_sample_count);

            // Update aggregated data status.
            set_aggregated_data_valid(true);

#           if defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA
            {
                ldbg() << os.str();
                os.str("");
                os.clear();
            }
#           endif // defined(XMSG_DEBUG_SURF_PRES_AGGR_DATA) && XMSG_DEBUG_SURF_PRES_AGGR_DATA

        }CATCH_BLOCK("Failed to aggregate surface data!")

    }

    /** Generate a listing entry string from this composite surface pressure time-series data
     *  object.
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
     *  @throw      log::MessageException if an error occurs while generating the listing string.
     *
     *  @return     A listing entry string representation of this composite surface pressure
     *              time-series data object.
     */
    const string GenerateListingString(
            uint16_t pg,
            uint16_t po,
            uint16_t sg,
            uint16_t so,
            uint16_t tg,
            uint16_t to,
            float pc) const {

        MsgVectorIterConst_t it;
        ostringstream os;

        try {

            // Get an exclusive lock.
//            //lock_guard<recursive_mutex> guard(mutex_);

            XMSG_SECTION("Surface Time-Series Data");
            os << AbstractXFallRise::GenerateListingComment() << endl;

            for_each(messages().begin(), messages().end(), [&](const MsgPtr_t& msg) {

                if(msg) os << msg->GenerateListingString(pg, po, sg, so, tg, to, pc) << endl;

            });

            os << GenerateDerivedFieldsListingString(pg, po, pc);

        }CATCH_BLOCK("Failed to generate listing string for composite surface data object!")

        return os.str();
    }

    const string GenerateDerivedFieldsListingString(uint16_t pg, uint16_t po,
            float pres_correction) const {

        ostringstream os;

        int midx = 0;

        try {

            // Get an exclusive lock.
//            //lock_guard<recursive_mutex> guard(mutex_);

            XMSG_SECTION("Surface Time-Series Data - Derived Fields");

            {
                const format fmt("#   %4s %4s %4s "
                        "%4s %12s "
                        "%15s %15s %15s "
                        "%15s");
                os << format(fmt) % "SN" % "dive" % "midx" //
                           % "nSam" % "presValid" //
                           % "presMin" % "presMean" % "presMax" //
                           % "presVar"
                   << endl;
            }

            {
                const format fmt("#   %4s %4s %4s "
                        "%4s %12s "
                        "%15s %15s %15s "
                        "%15s");
                os << format(fmt) % "" % "" % "" //
                           % "" % "" //
                           % "(dBar)" % "(dBar)" % "(dBar)" //
                           % "(dBar^2)"
                   << endl;
            }

            {
                const format fmt("SFD %4d %4d %4d "
                        "%4d %12d "
                        "%15s %15s %15s "
                        "%15.3f");
                os << format(fmt) //
                % (int) messages().at(0)->id_instrument() % (int) messages().at(0)->id_dive()
                        % midx++ //
                        % pres_num_samples() % (int) is_aggregated_data_valid() //
                        % XMsgFormatPressure(PresMinAsFloat(pg, po, pres_correction))
                        % XMsgFormatPressure(PresMeanAsFloat(pg, po, pres_correction))
                        % XMsgFormatPressure(PresMaxAsFloat(pg, po, pres_correction)) //
                        % PresVarAsFloat(pg);
            }

        }CATCH_BLOCK("Failed to generate listing string for derived surface data!")

        return os.str();
    }

    DerivedSample_t pres_var() const {

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        return pres_var_;
    }

     int pres_num_samples() const {

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        return pres_num_samples_;
    }

    template<typename T = float> T PresVarAsFloat(uint16_t gain) const {

        T val = XMSG_SENS_BAD_PRES;

        // Get an exclusive lock.
        //lock_guard<recursive_mutex> guard(mutex_);

        if(gain) {

            val = ((T) pres_var() / ((T) gain * (T) gain)
                    / (T) ((int64_t) 1 << (derived_fields_static_gain_exp() * 2)));

//            if(val < XMSG_SENS_MIN_PRES || val > XMSG_SENS_MAX_PRES)
//                val = XMSG_SENS_BAD_PRES;

        }

        return val;

    }

};

} /* namespace xmsg */
} /* namespace util */

#endif /* XCOMPOSITESURFACEPRESSURE_H_ */
