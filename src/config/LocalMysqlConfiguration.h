/** The @ref util::mysql::LocalMysqlConfiguration class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Aug 17, 2014 4:09:43 PM
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

#ifndef LOCALMYSQLCONFIGURATION_H_
#define LOCALMYSQLCONFIGURATION_H_

/* Includes */

#include "LocalConfiguration.h"
#include "../utility/MysqlUtils.h"

/* Definitions */

/** The default MySQL server address. */
#define CFG_MYSQL_DFLT_ADDRESS      "127.0.0.1"

/** The default MySQL server port number. */
#define CFG_MYSQL_DFLT_PORT         3306

/** The default protocol for connecting to the MySQL server. */
#define CFG_MYSQL_DFLT_PROTOCOL     "tcp"

/** The default MySQL database schema. */
#define CFG_MYSQL_DFLT_SCHEMA       "float_db"

namespace util {
namespace mysql {

/* Class Definitions */

/** Class providing useful functions for accessing and manipulating the MySQL database, given the
 *  local run-time configuration settings.
 *
 */
class LocalMysqlConfiguration:
        public util::mysql::MysqlUtils {

    /* Friends */

    friend class config::LocalConfiguration;

private:

    /* Data Members */

    /** The SQL server qualified name or IP address string. */
    static string address_;

    /** The SQL server account username string. */
    static string user_;

    /** The SQL server account password string. */
    static string password_;

    /** The SQL server port number. */
    static unsigned int port_;

    /** The protocol to use in the SQL server connection e.g. "tcp". */
    static string protocol_;

    /** The SQL server schema name. */
    static string schema_;

protected:

    /* Member Functions */

    static sql::Connection* getSqlConnection();

public:

    /* Member Functions */

    LocalMysqlConfiguration();

    virtual ~LocalMysqlConfiguration();

    static shared_ptr<sql::ResultSet> executeQuery(const string& query);

    static int executeUpdate(const string& update);

    static shared_ptr<sql::ResultSet> executeSelectQuery(const string& what,
                                                                const string& from,
                                                                const string& where = "",
                                                                const string& groupBy = "",
                                                                const string& orderBy = "",
                                                                unsigned int limit = 0,
                                                                unsigned int offset = 0);

    static shared_ptr<sql::ResultSet> executeDeleteQuery(
                                                                const string& from,
                                                                const string& where = "");

    static shared_ptr<sql::PreparedStatement> generatePreparedStatement(
            const string& statement);
};

} /* namespace mysql */
} /* namespace util */
#endif /* LOCALMYSQLCONFIGURATION_H_ */
