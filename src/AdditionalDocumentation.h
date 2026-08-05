/** Additional Doxygen documentation.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Apr 4, 2014 10:46:35 AM
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
 *
 * @note        This document is used for generating Doxygen documentation of namespaces and should
 *              not be included in the compiled project.
 *
 * This file defines Doxygen annotations for documenting elements that are otherwise undocumented.
 *
 *
 */

#ifndef NAMESPACEDEFS_H_
#define NAMESPACEDEFS_H_

#error  "This file is used for documentation purposes only. It should not be included from the project."

/** The Argo instrument namespace.
 *
 */
namespace argo {

/** The Argo Iridium instrument namespace.
 *
 */
namespace iridium {

/** The X-Float instrument namespace.
 *
 *  @note           This namespace contains all object that represents individual X-Floats
 *                  (including ALAMOs and Solo-IIs), their dives and mail messages, and all
 *                  of the constituents that make up these objects.
 */
namespace xfloat {

}
}
}

/** The Boost C++ library namespace.
 *
 */
namespace boost {

/** Dummy template class used for configuring Doxygen to interpret boost::shared_ptr objects as
 *  plain pointers.
 */
template<class A> class shared_ptr {

    /** The pointer to the element.
     *
     */
    A * element;
};

/** Dummy template class used for configuring Doxygen to interpret boost::thread_specific_ptr
 *  objects as plain pointers.
 */
template<class B> class thread_specific_ptr {

    /** The pointer to the element.
     *
     */
    B * element;
};

/** Boost container library namespace.
 *
 */
namespace container {

/** Dummy template class used for configuring Doxygen to interpret
 *  boost::container::flat_map correctly.

 *
 */
template<class C, class D> class flat_map {

    /** The index.*/
    C index[];

    /** The elements.
     *
     */
    D elements[];
};

/** Dummy template class used for configuring Doxygen to interpret
 *  boost::container::static_vector correctly.

 *
 */
template<class E> class static_vector {

    /** The elements.
     *
     */
    E elements[];
};
}
}

/** The utility namespace.
 *
 */
namespace util {

/** The configuration utility namespace.
 *

 *
 */
namespace config {

}
/** Date utility namespace.
 *
 */

namespace date {

}

/** The filesystem utility namespace.
 *
 */

namespace file {

}

/** The ftp utility namespace.
 *
 *  @deprecated         FTP client integration features have been scrapped. This namespace is
 *                      no longer utilized.
 */
namespace ftp {

}

/** The logging utility namespace.
 *
 */

namespace log {

}

/** The mail utility namespace.
 *
 */
namespace mail {

}

/** The math utility namespace.
 *
 */
namespace math {

}

/** The MySQL  utility namespace.
 *
 */
namespace mysql {

}

/** The position utility namespace.
 *
 */
namespace position {

}

/** The system signals utility namespace.
 *
 */
namespace sig {

}

/** The string utility namespace.
 *
 */
namespace string {

}

/** The stream utility namespace
 *
 */
namespace stream {

}

/** The system utility namespace
 *
 */
namespace sys {

}

/** The thread utility namespace.
 *
 */
namespace thread {

}

/** The types utility namespace.
 *
 */
namespace types {

}

/** The X-Message utility namespace.
 *
 */
namespace xmsg {

}
}

/** VMime library namespace.
 *
 */
namespace vmime {

/** Dummy template class used for configuring Doxygen to interpret vmime::shared_ptr objects as
 *  plain pointers.

 *
 */
template<class F> class shared_ptr {

    /** The pointer to the element.
     *
     */
    F * element;
};
}

#endif /* NAMESPACEDEFS_H_
     *
     */
