/** The @ref util::kml::KmlUtil class source file.
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

/* Includes: */

// header
#include "KmlUtil.h"

namespace util {
namespace kml {

/* Static Data Members: */

const Color32 KmlUtil::kml_default_path_colors_[KML_NUM_DFLT_PATH_COLORS] = {
        Color32(0xB0, 0xFF, 0x00, 0x00), // 0
        Color32(0xB0, 0xFF, 0x80, 0x00), // 30
        Color32(0xB0, 0xFF, 0xFF, 0x00), // 60
        Color32(0xB0, 0x80, 0xFF, 0x00), // 90
        Color32(0xB0, 0x00, 0xFF, 0x00), // 120
        Color32(0xB0, 0x00, 0xFF, 0x80), // 150
        Color32(0xB0, 0x00, 0xFF, 0xFF), // 180
        Color32(0xB0, 0x00, 0x80, 0xFF), // 210
        Color32(0xB0, 0x00, 0x00, 0xFF), // 240
        Color32(0xB0, 0x80, 0x00, 0xFF), // 270
        Color32(0xB0, 0xFF, 0x00, 0xFF), // 300
        Color32(0xB0, 0xFF, 0x00, 0x80), // 330
        Color32(0xB0, 0x80, 0x80, 0x080),
 };

const string KmlUtil::kml_default_path_style_ids_[KML_NUM_DFLT_PATH_COLORS] = {
        "path1",
        "path2",
        "path3",
        "path4",
        "path5",
        "path6",
        "path7",
        "path8",
        "path9",
        "pathA",
        "pathB",
        "pathC",
        "pathD",};

const string KmlUtil::kml_default_placemark_style_ids_[KML_NUM_DFLT_PATH_COLORS] = {
        "dot1_1",
        "dot1_2",
        "dot1_3",
        "dot1_4",
        "dot1_5",
        "dot1_6",
        "dot1_7",
        "dot1_8",
        "dot1_9",
        "dot1_A",
        "dot1_B",
        "dot1_C",
        "dot1_D", };

const string KmlUtil::kml_small_placemark_style_ids_[KML_NUM_DFLT_PATH_COLORS] = {
        "dot1s_1",
        "dot1s_2",
        "dot1s_3",
        "dot1s_4",
        "dot1s_5",
        "dot1s_6",
        "dot1s_7",
        "dot1s_8",
        "dot1s_9",
        "dot1s_A",
        "dot1s_B",
        "dot1s_C",
        "dot1s_D", };

const string KmlUtil::kml_triangle_placemark_style_ids_[KML_NUM_DFLT_PATH_COLORS] = {
        "tri1_1",
        "tri1_2",
        "tri1_3",
        "tri1_4",
        "tri1_5",
        "tri1_6",
        "tri1_7",
        "tri1_8",
        "tri1_9",
        "tri1_A",
        "tri1_B",
        "tri1_C",
        "tri1_D", };

thread_specific_ptr<KmlFactory> KmlUtil::kml_factory_;

} /* namespace kml */
} /* namespace util */

