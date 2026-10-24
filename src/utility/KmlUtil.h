/** The @ref util::kml::KmlUtil class header  file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Aug 8, 2015 5:44:39 PM
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

#ifndef KMLUTIL_H_
#define KMLUTIL_H_

/* Includes: */

// local
#include "ThreadUtils.h"

// KML
#include <kml/base/attributes.h>
#include <kml/base/file.h>
#include <kml/dom.h>
#include <kml/engine.h>

/* Definitions: */

#define KML_NUM_DFLT_PATH_COLORS    (13)

namespace util {
namespace kml {

using boost::thread_specific_ptr;
using boost::lexical_cast;
using kmlbase::Color32;
using kmldom::CoordinatesPtr;
using kmldom::DocumentPtr;
using kmldom::HotSpotPtr;
using kmldom::IconStyleIconPtr;
using kmldom::IconStylePtr;
using kmldom::KmlPtr;
using kmldom::KmlFactory;
using kmldom::LabelStylePtr;
using kmldom::LineStylePtr;
using kmldom::PlacemarkPtr;
using kmldom::PointPtr;
using kmldom::StylePtr;
using kmldom::UNITS_FRACTION;


/** The KML utility class.
 *
 *  This class provides some useful functions for creating Google Earth KML files.
 *
 */
class KmlUtil {

private:

    /* Private Data Members: */


    /** The colour palette for instrument paths and placemarks, indexed by style index. */
    static const Color32 kml_default_path_colors_[KML_NUM_DFLT_PATH_COLORS];

    /** The KML style IDs of the path (line) styles, one per palette colour. */
    static const string kml_default_path_style_ids_[KML_NUM_DFLT_PATH_COLORS];

    /** The KML style IDs of the large placemark styles, one per palette colour. */
    static const string kml_default_placemark_style_ids_[KML_NUM_DFLT_PATH_COLORS];

    /** The KML style IDs of the small placemark styles, one per palette colour. */
    static const string kml_small_placemark_style_ids_[KML_NUM_DFLT_PATH_COLORS];

    /** The KML style IDs of the triangle placemark styles, one per palette colour. */
    static const string kml_triangle_placemark_style_ids_[KML_NUM_DFLT_PATH_COLORS];

    /** The libkml factory used to create KML elements, held per thread.
     *
     *  @warning    This holds libkml's process-wide factory (@c KmlFactory::GetFactory()), and the
     *              default cleanup deletes it when the owning thread exits or
     *              @ref CloseKmlFactory() is called. Only the post-processing thread uses KML, so
     *              this currently happens only at shutdown.
     */
    static thread_specific_ptr<KmlFactory> kml_factory_;

public:

    KmlUtil() {
      /* Do nothing. */
    }

    virtual ~KmlUtil() {
      /* Do nothing. */
    }


    /* Protected Member Functions: */

    /** Get the libkml factory, initializing it for this thread if necessary.
     *
     *  @throw      util::log::MessageException if the factory cannot be obtained.
     *
     *  @return     The thread's KML factory pointer (see @ref kml_factory_).
     */
    static thread_specific_ptr<KmlFactory>& GetKmlFactory() {

        try {

            // Create a new KML factory object, if it does not exist.
            if(!kml_factory_.get())
                kml_factory_.reset(KmlFactory::GetFactory());

        }CATCH_BLOCK("Failed to get KmlFactory object!")

        return kml_factory_;
    }

    /** Add the four default styles for one palette colour to a KML document.
     *
     *  Adds a 5-pixel path (line) style, a large dot placemark style, a small dot placemark style
     *  and a triangle placemark style, all in the colour @p style_idx (modulo the palette size).
     *  The icons are loaded from @c http://argo.whoi.edu/kml/img/. Errors are logged, not thrown.
     *
     *  @param[in]  document    The KML document to add the styles to.
     *  @param[in]  style_idx   The palette index of the colour.
     */
    static void SetDefaultKmlStyle(const DocumentPtr& document, unsigned int style_idx) {

        try {

            { /* Create Path Style: */

                // Create line style.
                LineStylePtr line_style = GetKmlFactory()->CreateLineStyle();
                line_style->set_color(GetDefaltKmlPathColor(style_idx));
                line_style->set_width(5);

                // Create style
                StylePtr style = GetKmlFactory()->CreateStyle();
                style->set_id(GetDefaltKmlPathStyleId(style_idx));
                style->set_linestyle(line_style);

                // Add style to document.
                document->add_styleselector(style);
            }

            { /* Create Icon Style: */

                // Create icon.
                IconStyleIconPtr icon = GetKmlFactory()->CreateIconStyleIcon();
                icon->set_href("http://argo.whoi.edu/kml/img/dot2.png");

                // Create hotspot.
                HotSpotPtr hot_spot = GetKmlFactory()->CreateHotSpot();
                hot_spot->set_x(0.5);
                hot_spot->set_xunits(UNITS_FRACTION);
                hot_spot->set_y(0.5);
                hot_spot->set_yunits(UNITS_FRACTION);

                // Create icon style.
                IconStylePtr icon_style = GetKmlFactory()->CreateIconStyle();
                icon_style->set_color(GetDefaltKmlPathColor(style_idx));
                icon_style->set_icon(icon);
                icon_style->set_scale(1.5);
                icon_style->set_hotspot(hot_spot);

                // Create label style;
                LabelStylePtr label_style = GetKmlFactory()->CreateLabelStyle();
                label_style->set_scale(1.5);

                // Create style.
                StylePtr style = GetKmlFactory()->CreateStyle();
                style->set_id(GetDefaltKmlPlacemarkStyleId(style_idx));
                style->set_iconstyle(icon_style);
                style->set_labelstyle(label_style);

                // Add style to document.
                document->add_styleselector(style);
            }

            { /* Create Small Icon Style: */

                // Create icon.
                IconStyleIconPtr icon = GetKmlFactory()->CreateIconStyleIcon();
                icon->set_href("http://argo.whoi.edu/kml/img/dot2.png");

                // Create hotspot.
                HotSpotPtr hot_spot = GetKmlFactory()->CreateHotSpot();
                hot_spot->set_x(0.5);
                hot_spot->set_xunits(UNITS_FRACTION);
                hot_spot->set_y(0.5);
                hot_spot->set_yunits(UNITS_FRACTION);

                // Create icon style.
                IconStylePtr icon_style = GetKmlFactory()->CreateIconStyle();
                icon_style->set_color(GetDefaltKmlPathColor(style_idx));
                icon_style->set_icon(icon);
                icon_style->set_scale(.7);
                icon_style->set_hotspot(hot_spot);

                // Create label style;
                LabelStylePtr label_style = GetKmlFactory()->CreateLabelStyle();
                label_style->set_scale(0.6);

                // Create style.
                StylePtr style = GetKmlFactory()->CreateStyle();
                style->set_id(GetSmallKmlPlacemarkStyleId(style_idx));
                style->set_iconstyle(icon_style);
                style->set_labelstyle(label_style);

                // Add style to document.
                document->add_styleselector(style);
            }

            { /* Create Triangle Icon Style: */

                // Create icon.
                IconStyleIconPtr icon = GetKmlFactory()->CreateIconStyleIcon();
                icon->set_href("http://argo.whoi.edu/kml/img/tri2.png");

                // Create hotspot.
                HotSpotPtr hot_spot = GetKmlFactory()->CreateHotSpot();
                hot_spot->set_x(0.5);
                hot_spot->set_xunits(UNITS_FRACTION);
                hot_spot->set_y(0);
                hot_spot->set_yunits(UNITS_FRACTION);

                // Create icon style.
                IconStylePtr icon_style = GetKmlFactory()->CreateIconStyle();
                icon_style->set_color(GetDefaltKmlPathColor(style_idx));
                icon_style->set_icon(icon);
//                    icon_style->set_scale(1);
                icon_style->set_hotspot(hot_spot);

                // Create label style;
                LabelStylePtr label_style = GetKmlFactory()->CreateLabelStyle();
                label_style->set_scale(0.6);

                // Create style.
                StylePtr style = GetKmlFactory()->CreateStyle();
                style->set_id(GetTriangleKmlPlacemarkStyleId(style_idx));
                style->set_iconstyle(icon_style);
                style->set_labelstyle(label_style);

                // Add style to document.
                document->add_styleselector(style);
            }

        }CATCH_NOTHROW("Failed to set default style " + lexical_cast<string>(style_idx) + "!")

    }

    /** Add the default styles for every palette colour to a KML document.
     *
     *  @param[in]  document    The KML document to add the styles to.
     *
     *  @throw      util::log::MessageException if an error occurs.
     */
    static void SetAllDefaultKmlSyles(const DocumentPtr& document) {

        try {

            for(int i = 0; i < KML_NUM_DFLT_PATH_COLORS; ++i)

                SetDefaultKmlStyle(document, i);

        }CATCH_BLOCK("Failed to create default KML path styles!")

    }

    /** Release this thread's KML factory pointer.
     *
     *  @warning    This deletes libkml's shared factory (see @ref kml_factory_). It is not
     *              currently called.
     *
     *  @throw      util::log::MessageException if an error occurs.
     */
    static void CloseKmlFactory() {

        try {

            if(kml_factory_.get())
                kml_factory_.reset();

        }CATCH_BLOCK("Failed to close KmlFactory object!")

    }

    /** Get the palette colour for an index (an instrument ID, for example), modulo the palette size.
     *
     *  @param[in]  id      The index.
     *
     *  @return     The colour.
     */
    static const Color32& GetDefaltKmlPathColor(int id) {
        return kml_default_path_colors_[id % KML_NUM_DFLT_PATH_COLORS];
    }

    /** Get the path (line) style ID for an index, modulo the palette size.
     *
     *  @param[in]  id      The index.
     *
     *  @return     The KML style ID.
     */
    static const string& GetDefaltKmlPathStyleId(int id) {
        return kml_default_path_style_ids_[id % KML_NUM_DFLT_PATH_COLORS];
    }

    /** Get the large placemark style ID for an index, modulo the palette size.
     *
     *  @param[in]  id      The index.
     *
     *  @return     The KML style ID.
     */
    static const string& GetDefaltKmlPlacemarkStyleId(int id) {
        return kml_default_placemark_style_ids_[id % KML_NUM_DFLT_PATH_COLORS];
    }

    /** Get the small placemark style ID for an index, modulo the palette size.
     *
     *  @param[in]  id      The index.
     *
     *  @return     The KML style ID.
     */
    static const string& GetSmallKmlPlacemarkStyleId(int id) {
        return kml_small_placemark_style_ids_[id % KML_NUM_DFLT_PATH_COLORS];
    }

    /** Get the triangle placemark style ID for an index, modulo the palette size.
     *
     *  @param[in]  id      The index.
     *
     *  @return     The KML style ID.
     */
    static const string& GetTriangleKmlPlacemarkStyleId(int id) {
        return kml_triangle_placemark_style_ids_[id % KML_NUM_DFLT_PATH_COLORS];
    }

};

} /* namespace kml */
} /* namespace util */

#endif /* KMLUTIL_H_ */
