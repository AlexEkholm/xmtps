/** The @ref util::mysql::LocalMysqlConfiguration class source file.
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

/* Includes */

// header
#include "LocalMysqlConfiguration.h"

namespace util {
namespace mysql {

/* Static Variables */

std::string LocalMysqlConfiguration::address_(CFG_MYSQL_DFLT_ADDRESS);

std::string LocalMysqlConfiguration::user_;

std::string LocalMysqlConfiguration::password_;

std::string LocalMysqlConfiguration::protocol_(CFG_MYSQL_DFLT_SCHEMA);

unsigned int LocalMysqlConfiguration::port_(CFG_MYSQL_DFLT_PORT);

std::string LocalMysqlConfiguration::schema_(CFG_MYSQL_DFLT_SCHEMA);

/* Function Definitions */

/** Default constructor.
 *
 */
LocalMysqlConfiguration::LocalMysqlConfiguration() {
    /* Do nothing. */

}

/** Destructor.
 *
 */
LocalMysqlConfiguration::~LocalMysqlConfiguration() {
    /* Do nothing. */
}

/** Generate a thread-specific instance of a MySQL connection object.
 *
 *  A new connection will be created, if necessary.
 *
 *  @throw          log::MessageException if an error occurs while generating the SQL connection
 *                  object.
 *
 *  @return         A thread-specific instance of a MySQL connection object.
 */
sql::Connection* LocalMysqlConfiguration::getSqlConnection() {
    using std::string;
    using std::to_string;
    using sql::Connection;
    using util::mysql::MysqlUtils;

    try {
        // Get configuration.

        // Get SQL host URL.
        std::ostringstream url;

        // Connect to the SQL server.
        Connection* con = MysqlUtils::GetSqlConnection(
                protocol_ + string("://") + address_ + ":" + to_string(port_),
                user_,
                password_,
                schema_).get();

        con->setAutoCommit(1);
        //con->setTransactionIsolation(sql::TRANSACTION_NONE);

        return con;

    }CATCH_BLOCK("Failed to connect to SQL server!")

    return NULL;
}

/** Generate a prepared SQL statement object for the given SQL statement.
 *
 *  @param[in]      statement   The SQL statement to prepare. This must be a well-formed SQL string.
 *
 *  @throw          log::MessageException if 'statement' is not a valid SQL string or if an error
 *                  occurs while generating the prepared statement object..
 *
 *  @return         A prepared SQL statement object generated from the given SQL statement.
 */
std::shared_ptr<sql::PreparedStatement> LocalMysqlConfiguration::generatePreparedStatement(
        const std::string& statement) {
    return MysqlUtils::GeneratePreparedStatement(getSqlConnection(), statement);
}

/** Execute an SQL select query and return a shared pointer to the ResultSet object.
 *
 *  @param[in]      what        The SQL select statement select-expression clause.
 *  @param[in]      from        The SQL select statement table--references clause.
 *  @param[in]      where       The SQL select statement where-condition clause.
 *  @param[in]      groupBy     The SQL select statement group-by clause.
 *  @param[in]      orderBy     The SQL select statement order-by clause.
 *  @param[in]      limit       The SQL select statement result set row limit value.
 *  @param[in]      offset      The SQL select statement result set row offset.
 *
 *  @throw          log::MessageException if an error occurs while executing the query.
 *
 *  @return         A shared pointer to the ResultSet object.
 */
shared_ptr<sql::ResultSet> LocalMysqlConfiguration::executeSelectQuery(
        const std::string& what,
        const std::string& from,
        const std::string& where,
        const std::string& groupBy,
        const std::string& orderBy,
        unsigned int limit,
        unsigned int offset) {

    std::string query;
    std::shared_ptr<sql::ResultSet> res;

    try {

        // Generate the query.
        query = GenerateSqlSelectStr(what, from, where, groupBy, orderBy, limit, offset);

        // Execute the query.
        res = executeQuery(query);

    }CATCH_BLOCK("Failed to execute SQL select query: " + query + "!")

    return res;
}

shared_ptr<sql::ResultSet> LocalMysqlConfiguration::executeDeleteQuery(
        const string& from,
        const string& where) {

    std::string query;
    std::shared_ptr<sql::ResultSet> res;

    try {

        // Execute the query.
        res = executeQuery(query = GenerateSqlDeleteStr(from, where));

    }CATCH_BLOCK("Failed to execute DELETE select query: " + query + "!")

    return res;

}

/** Execute an SQL query and return a shared pointer to the ResultSet object.
 *
 *  @param[in]      query       The SQL query to execute. This must be a well-formatted SQL query
 *                              string.
 *
 *  @throw          log::MessageException 'query' is a malfored SQL string or any other error
 *                  occurs while executing the query.
 *
 *  @return         A shared pointer to the query ResultSet object.
 */
shared_ptr<sql::ResultSet> LocalMysqlConfiguration::executeQuery(const std::string& query) {
    return MysqlUtils::ExecuteQuery(getSqlConnection(), query);
}

/** Execute an SQL update and return the status code.
 *
 *  @param[in]      update      The SQL update to execute. This must be a well-formatted SQL update
 *                              string.
 *
 *  @throw          log::MessageException if 'update' is a malformed SQL string, or any other error
 *                  occurs while executing the update.
 *
 *  @return         The update status code.
 */
int LocalMysqlConfiguration::executeUpdate(const std::string& update) {
    return MysqlUtils::ExecuteUpdate(getSqlConnection(), update);
}

} /* namespace mysql */
} /* namespace util */
