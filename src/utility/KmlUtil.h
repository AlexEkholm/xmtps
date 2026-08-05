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


    static const Color32 kml_default_path_colors_[KML_NUM_DFLT_PATH_COLORS];

    static const string kml_default_path_style_ids_[KML_NUM_DFLT_PATH_COLORS];

    static const string kml_default_placemark_style_ids_[KML_NUM_DFLT_PATH_COLORS];

    static const string kml_small_placemark_style_ids_[KML_NUM_DFLT_PATH_COLORS];

    static const string kml_triangle_placemark_style_ids_[KML_NUM_DFLT_PATH_COLORS];

    static thread_specific_ptr<KmlFactory> kml_factory_;

public:

    KmlUtil() {
      /* Do nothing. */
    }

    virtual ~KmlUtil() {
      /* Do nothing. */
    }


    /* Protected Member Functions: */

    static thread_specific_ptr<KmlFactory>& GetKmlFactory() {

        try {

            // Create a new KML factory object, if it does not exist.
            if(!kml_factory_.get())
                kml_factory_.reset(KmlFactory::GetFactory());

        }CATCH_BLOCK("Failed to get KmlFactory object!")

        return kml_factory_;
    }

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

    static void SetAllDefaultKmlSyles(const DocumentPtr& document) {

        try {

            for(int i = 0; i < KML_NUM_DFLT_PATH_COLORS; ++i)

                SetDefaultKmlStyle(document, i);

        }CATCH_BLOCK("Failed to create default KML path styles!")

    }

    static void CloseKmlFactory() {

        try {

            if(kml_factory_.get())
                kml_factory_.reset();

        }CATCH_BLOCK("Failed to close KmlFactory object!")

    }

    static const Color32& GetDefaltKmlPathColor(int id) {
        return kml_default_path_colors_[id % KML_NUM_DFLT_PATH_COLORS];
    }

    static const string& GetDefaltKmlPathStyleId(int id) {
        return kml_default_path_style_ids_[id % KML_NUM_DFLT_PATH_COLORS];
    }

    static const string& GetDefaltKmlPlacemarkStyleId(int id) {
        return kml_default_placemark_style_ids_[id % KML_NUM_DFLT_PATH_COLORS];
    }

    static const string& GetSmallKmlPlacemarkStyleId(int id) {
        return kml_small_placemark_style_ids_[id % KML_NUM_DFLT_PATH_COLORS];
    }

    static const string& GetTriangleKmlPlacemarkStyleId(int id) {
        return kml_triangle_placemark_style_ids_[id % KML_NUM_DFLT_PATH_COLORS];
    }

};

} /* namespace kml */
} /* namespace util */

#endif /* KMLUTIL_H_ */
