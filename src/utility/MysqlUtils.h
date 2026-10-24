/** The @ref util::mysql::MysqlUtils class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       May 12, 2014 4:19:03 PM
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

#ifndef MYSQL_UTILS_H_
#define MYSQL_UTILS_H_

/* Includes */

// local
#include "ThreadUtils.h"

// mysql
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

// boost
#include <boost/smart_ptr.hpp>

namespace util {
namespace mysql {

using std::shared_ptr;
using boost::thread_specific_ptr;
using sql::Connection;
using sql::mysql::MySQL_Driver;
using sql::PreparedStatement;
using sql::ResultSet;
using std::endl;
using std::ostringstream;
using std::string;

/* Class Definition */

/** Class provides a library of function for accessing and manipulating a MySQL database.
 *
 */
class MysqlUtils {

private:

    /* Private Data Members: */

    /** The thread-specific pointer to MySQL driver object. */
    static thread_specific_ptr<MySQL_Driver> sql_driver_;

    /** Tthe thread-specific pointer to MySQL connection object. */
    static thread_specific_ptr<Connection> sql_connection_;

public:

    MysqlUtils() {
      /* Do nothing. */
    }

    virtual ~MysqlUtils() {
      /* Do nothing. */
    }


private:

    /* Private Member Functions: */

    /** Cleanup an SQL connection.
     *
     *  This function for memory cleanup of the SQL connection on destruction. The thread-specific SQL
     *  connection object will be deleted if it exists.
     *
     *  @param[in]      con         The SQL connection to cleanup.
     *
     *  @throw          log::MessageException if an error occurs while cleaning up the SQL connection
     *                  object.
     */
    static void CleanupSqlConnection(Connection* con) {

        try {

            if(con)
                // Delete the connection object.
                delete (con);

        }CATCH_BLOCK("Failed to cleanup SQL connection object!")
    }

    /** Function for memory cleanup of MySQL driver on destruct.
     *
     *  Signals the SQL driver to end operation, if it exists.
     *
     *  @param[in]      driver      Driver to cleanup.
     *
     *  @throw          log::MessageException if an error occurs while cleaning up the SQL driver
     *                  object.
     */
    static void CleanupSqlDriver(MySQL_Driver* driver) {

        (void) driver;

        try {

            //if(driver)
                // XXX End this MySQL thread instance.
                //driver->threadEnd();

        }CATCH_BLOCK("Failed to cleanup the SQL driver object!")
    }

public:

    /* Public Member Functions */

    /** Close the thread-specific MySQL connection if it is currently open.
     *
     *  If the thread local MySQL connection is not currently open, then this function will have no
     *  effect.
     *
     *  @note           The MySQL connection should be closed when idle to prevent the connection from
     *                  timing out and throwing exceptions.
     *
     *  @throw          log::MessageException if an error occurs while attempting to close the MySQL
     *                  connection.
     */
    static void CloseSqlConnection() {

        try {

            // Check the SQL connection pointer.
            if(!sql_connection_.get())
                return;

            // Close the SQL connection object.
            if(!sql_connection_->isClosed())
                sql_connection_->close();

            // Reset the pointer.
            sql_connection_.reset();

        }CATCH_BLOCK("Failed to close SQL connection!");
    }

    /** Execute an SQL query and return a shared pointer to the ResultSet object.
     *
     *  @param[in]      con         A pointer to the SQL connection object. This must not be NULL.
     *  @param[in]      query       The SQL query to execute. This must be a well-formatted SQL query
     *                              string.
     *
     *  @throw          log::MessageException if 'con' is null, 'query' is a malfored SQL string,
     *                  or any other error occurs.
     *
     *  @return         A shared pointer to the query ResultSet object.
     */
    static shared_ptr<ResultSet> ExecuteQuery(Connection * con, const string& query) {
        shared_ptr<ResultSet> ptr;

        try {

            ptr = shared_ptr<ResultSet>(GeneratePreparedStatement(con, query)->executeQuery());

        }CATCH_BLOCK("Failed to execute SQL query:\n " + query)

        return ptr;
    }

    /** Execute an SQL update and return the status code.
     *
     *  @param[in]      con         A pointer to the SQL connection object. This must not be NULL.
     *  @param[in]      update      The SQL update to execute. This must be a well-formatted SQL update
     *                              string.
     *
     *  @throw          log::MessageException if 'con' is null, 'update' is a malformed SQL string,
     *                  or any other error occurs.
     *
     *  @return         The update status code.
     */
    static int ExecuteUpdate(Connection * con, const string& update) {
        int stat = -1;

        try {

            stat = GeneratePreparedStatement(con, update)->executeUpdate();

        }CATCH_BLOCK("Failed to execute SQL update: " + update + "!")

        return stat;
    }

    /** Get a thread-specific instance of a MySQL connection object.
     *
     *  A new connection will be created, if necessary.
     *
     *  @param[in]      url         The MySQL host server qualified address.
     *  @param[in]      user        The MySQL account username.
     *  @param[in]      password    The MySQL account password.
     *  @param[in]      schema      The MySQL account schema name.
     *
     *  @throw          log::MessageException if an error occurs while creating a new thread-specific
     *                  instance of the SQL connection object
     *
     *  @return         A thread-specific instance of a MySQL connection object.
     */
    static thread_specific_ptr<Connection>& GetSqlConnection(
            string url,
            string user,
            string password,
            string schema) {

        try {

            // Check if the connection already exists.
            if(!sql_connection_.get() || sql_connection_->isClosed()) {

                // Connect to SQL server - get a new instance of the SQL object for this thread.
                sql_connection_.reset(GetSqlDriver()->connect(url, user, password));
                sql_connection_->setSchema(schema);
            }

        }CATCH_BLOCK("Failed to get MySQL connection object!")

        return sql_connection_;
    }

    /** Get a thread-specific instance of a MySQL driver object.
     *
     *  A new driver will created, if necessary.
     *
     *  @throw          log::MessageException if an error occurs while creating a new thread-specific
     *                  instance of the SQL driver object
     *
     *  @return         A thread-specific instance of a MySQL driver object.
     */
    static thread_specific_ptr<MySQL_Driver>& GetSqlDriver() {

        try {

            // Check if the driver already exists
            if(!(sql_driver_.get()))

                // Get a new instance of the MySQL driver for this thread.
                sql_driver_.reset(sql::mysql::get_mysql_driver_instance());

        }CATCH_BLOCK("Failed to get instance of MySQL driver object!")

        return sql_driver_;
    }

    /** Generate a prepared SQL statement object for the given SQL statement.
     *
     *  @param[in]      con         A pointer to the SQL connection object. This must not be NULL.
     *  @param[in]      statement   The SQL statement to prepare. This must be a well-formed SQL string.
     *
     *  @throw          log::MessageException if 'con' is NULL, 'statement' is not a valid SQL string,
     *                  or if an error occurs.
     *
     *  @return         A prepared SQL statement object generated from the given SQL statement.
     */
    static shared_ptr<PreparedStatement> GeneratePreparedStatement(
            Connection * con,
            const string& statement){

        shared_ptr<PreparedStatement> stmt;

        try {

            // Check the SQL connection pointer.
            if(!con)
                EXC_THROW("Pointer to SQL connection is NULL!");

//            dbg1() << "SQL: " << endl << statement;

            // Generate the prepared statement.
            stmt.reset( con->prepareStatement(statement));

        }CATCH_BLOCK("Failed to generate prepared SQL statement!")

        return stmt;
    }

    /** Generate an SQL select statement.
     *
     *  @param[in]      what        The SQL select statement select-expression clause.
     *  @param[in]      from        The SQL select statement table-references clause.
     *  @param[in]      where       The SQL select statement where-condition clause.
     *  @param[in]      groupBy     The SQL select statement group-by clause.
     *  @param[in]      orderBy     The SQL select statement order-by clause.
     *  @param[in]      limit       The SQL select statement result set row limit value.
     *  @param[in]      offset      The SQL select statement result set row offset.
     *
     *  @return         The SQL select statement.
     */
    static string GenerateSqlSelectStr(
            const string& what,
            const string& from,
            const string& where = "",
            const string& groupBy = "",
            const string& orderBy = "",
            unsigned int limit = 0,
            unsigned int offset = 0) {

        ostringstream os;

        os << "SELECT \n\t" << what << " \nFROM \n\t" << from;

        if(!where.empty())
            os << " \nWHERE \n\t" << where;

        if(!groupBy.empty())
            os << " \nGROUP BY \n\t" << groupBy;

        if(!orderBy.empty())
            os << " \nORDER BY \n\t" << orderBy;

        if(limit) {
            os << " \nLIMIT \n\t" << limit;
            if(offset)
                os << " \nOFFSET \n\t" << offset;
        }

        os << ";";

        string select = os.str();

        return select;
    }

    /** Generate an SQL DELETE statement.
     *
     *  @warning    Without a @p where clause, the statement deletes every row in the table.
     *
     *  @param[in]  from        The name of the table to delete from.
     *  @param[in]  where       The SQL "where-condition" clause (empty = all rows).
     *
     *  @return     The SQL statement.
     */
    static string GenerateSqlDeleteStr(
            const string& from,
            const string& where = "") {

        ostringstream os;

        os << "DELETE FROM \n\t" << from;

        if(!where.empty())
            os << " \nWHERE \n\t" << where;

        os << ";";

        return os.str();

    }

};

} /* namespace iridium */
} /* namespace argo */
#endif /* MYSQL_UTILS_H_ */
