/** The @ref util::mysql::SqlTableDescription class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 18, 2014 12:57:33 PM
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

#ifndef TABLEDESCRIPTION_H_
#define TABLEDESCRIPTION_H_

/* Includes */

// local
#include "SqlTableColumnDescription.h"

namespace util {
namespace mysql {

using boost::dynamic_pointer_cast;
using boost::lexical_cast;
using std::make_shared;
using boost::posix_time::ptime;
//using std::shared_ptr;
using std::ostringstream;
using std::string;
using std::vector;
using sql::PreparedStatement;
using sql::SQLString;
using sql::ResultSet;

/* Class Definition */

/** Template class representing the description of an SQL database table.
 *
 *  The template parameter T represents the class that the SQL database table represents.
 *
 */
template<class T> class SqlTableDescription:
        public LocalMysqlConfiguration,
        protected util::stream::StreamUtils {

    /* Template-Dependent Types */

    /** A column of type @p bool.
     *
     */
    typedef Column_bool<T> cbol;

    /** A column of type @p char.
     *
     */
    typedef Column_char<T> cchar;

    /** A column of type @p double.
     *
     */
    typedef Column_double<T> cdbl;

    /** A column of type @p boost::filesystem::path.
     *
     */
    typedef Column_filesystem_path<T> cpth;

    /** A column of type @p float.
     *
     */
    typedef Column_float<T> cflt;

    /** A column of type @p int.
     *
     */
    typedef Column_int<T> cint;

    /** A column of type @p int16_t.
     *
     */
    typedef Column_int16_t<T> ci16;

    /** A column of type @p long.
     *
     */
    typedef Column_long<T> clng;

    /** A column of type @p boost::posix_time::ptime.
     *
     */
    typedef Column_posix_time<T> cptm;

    /** A column of type @p std::string.
     *
     */
    typedef Column_string<T> cstr;

    /** A column of type @p uint16_t.
     *
     */
    typedef Column_uint16_t<T> cu16;

    /** A column of type @p uint8_t.
     *
     */
    typedef Column_uint8_t<T> cu8;

    /** A column of type @p std::vector<uint8_t>.
     *
     */
    typedef Column_vector_uint8_t<T> cvu8;

private:

    /* Data Members */

    /** The insert statement for this SQL table. */
    string insert_on_duplicate_key_update_statement_;

    /** The select all statement for this SQL table. */
    string select_statement_;

    /** A vector of shared pointers to @ref SqlTableColumnDescription objects describing the
     *  columns of this SQL table.
     */
    vector<shared_ptr<SqlTableColumnDescriptionBase> > table_columns_;

    /** The name of this SQL table. */
    string table_name_;

private:

    /* Member Functions */

    /** Generate a vector of object T from the given result set.
     *
     *  @param[in]      res         A shared pointer to the result set from which the vector of
     *                              objects is to be constructed.
     *
     *  @note           The columns of the result set must correspond in name, type, and order to
     *                  column descriptions contained in @ref
     *                  SqlTableDescription<T>::table_columns_.
     *
     *  @throw          log::MessageException if an error occurs while constructing the result
     *                  vector.
     *
     *  @return         A vector of object T constructed from the given result set.
     */
    vector<shared_ptr<T> > GenerateObjectList(shared_ptr<ResultSet> res) const {
        shared_ptr<T> obj;                      // An object constructed from the result set.
        vector<shared_ptr<T> > obj_list;        // The list of constructed object to return.

        try {
            if(!res)
                EXC_THROW("Pointer to result set is NULL!");
            obj_list.reserve(res->rowsCount());

            // Iterate over the rows in result set and construct one object for each row.
            while(res->next()) {
                obj = make_shared<T>();

                // Iterate over columns and set the corresponding values of the object members.
                for_each(
                        table_columns_.begin(),
                        table_columns_.end(),
                        [&](const shared_ptr<SqlTableColumnDescriptionBase>& col) {
                            switch(col->data_type()) {
                                case COLUMN_DATA_TYPE_BOOL: // Get bool value.
                                (dynamic_pointer_cast<cbol>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_CHAR:// Get double value.
                                (dynamic_pointer_cast<cchar>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_DOUBLE:// Get double value.
                                (dynamic_pointer_cast<cdbl>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_FLOAT:// Get float value.
                                (dynamic_pointer_cast<cflt>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_FILESYSTEM_PATH:// Get path value.
                                (dynamic_pointer_cast<cpth>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_INT:// Get int value.
                                (dynamic_pointer_cast<cint>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_INT16_T:// Get int16_t value.
                                (dynamic_pointer_cast<ci16>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_LONG:// Get long value.
                                (dynamic_pointer_cast<clng>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_POSIX_TIME:// Get boost::posix_time::ptime value.
                                (dynamic_pointer_cast<cptm>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_STRING:// Get string value.
                                (dynamic_pointer_cast<cstr>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_UINT16_T:// Get uint16_t value.
                                (dynamic_pointer_cast<cu16>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_UINT8_T:// Get uint8_t value.
                                (dynamic_pointer_cast<cu8>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_VECTOR_UINT8_T:// Get vector<uint8_t> value.
                                (dynamic_pointer_cast<cvu8>(col))->SetObjectValueFromResultSet(
                                        *obj,
                                        *res);
                                break;

                                case COLUMN_DATA_TYPE_UNKOWN:
                                default:// If an unknown value type is encountered, throw exception.
                                EXC_THROW("Failed to determine column type!");
                                break;
                            }

                        });
                obj_list.push_back(obj);
            }
        }CATCH_BLOCK("Failed to generate vector of objects from the result set!")

        return obj_list;
    }

public:

    /* Member Functions */

    /** The parameterized constructor.
     *
     *  @param[in]      name            The name of the SQL database table.
     *  @param[in]      columns         Vector of shared pointers to ColumnDescription objects
     *                                  describing the columns of this SQL table.
     */
    SqlTableDescription(
            const string& name,
            const vector<shared_ptr<SqlTableColumnDescriptionBase> >& columns) :
                    table_columns_(columns),
                    table_name_(name) {
        generateInsertStatment();
        generateSelectAllStatement();

        // Set column indices.
        size_t idx = 0;
        for_each(
                table_columns_.begin(),
                table_columns_.end(),
                [&](const shared_ptr<SqlTableColumnDescriptionBase>& col) {
                    col->set_column_index(++idx);
                });

    }

    /** The destructor.
     *
     */
    ~SqlTableDescription() {
        /* Do nothing. */
    }

    void DeleteObjectsFromDatabase(const string& where = "") const  {

        try {

            // Execute query.
            executeDeleteQuery(table_name_, where);

        }CATCH_BLOCK("Failed to query objects from database!")

    }

    /** Save a persistent entity to the SQL table.
     *
     *  @throws         util::log::MessageException if an error occurs.
     *
     *  @param[in]      obj             The instance of the persistent entity to save to the
     *                                  SQL database table.
     */
    void SaveObjectToDatabase(const T& obj) const {
        shared_ptr<SqlTableColumnDescriptionBase> col;
        shared_ptr<PreparedStatement> stmt;

        try {
            // Generate the SQL prepared statement.
            stmt = generatePreparedStatement(insert_on_duplicate_key_update_statement_);

            // Iterate over the columns in the table.
            for(size_t i = 0; i < table_columns_.size(); ++i) {
                col = table_columns_.at(i);

                // Set the appropriate prepared statement value, based on the column type!
                switch(col->data_type()) {
                case COLUMN_DATA_TYPE_BOOL:             // Set bool value.
                    (dynamic_pointer_cast<cbol>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_CHAR:           // Set char value.
                    (dynamic_pointer_cast<cchar>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_DOUBLE:           // Set double value.
                    (dynamic_pointer_cast<cdbl>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_FILESYSTEM_PATH:  // Set path value.
                    (dynamic_pointer_cast<cpth>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_FLOAT:            // Set float value.
                    (dynamic_pointer_cast<cflt>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_INT:              // Set int value.
                    (dynamic_pointer_cast<cint>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_INT16_T:          // Set int16_t value
                    (dynamic_pointer_cast<ci16>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_LONG:             // Set long value.
                    (dynamic_pointer_cast<clng>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_POSIX_TIME:       // Set ptime value.
                    (dynamic_pointer_cast<cptm>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_STRING:           // Set string value.
                    (dynamic_pointer_cast<cstr>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_UINT16_T:         // Set uint16_t value.
                    (dynamic_pointer_cast<cu16>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_UINT8_T:          // Set uint8_t value.
                    (dynamic_pointer_cast<cu8>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_VECTOR_UINT8_T:   // Set vector<uint8_t> value.
                    (dynamic_pointer_cast<cvu8>(col))->SetPreparedStatementValueFromObject(
                            obj,
                            *stmt);
                    break;

                case COLUMN_DATA_TYPE_UNKOWN:
                default:  // If an unknown value type is encountered, throw exception.
                    EXC_THROW("Failed to set column value for unknown type!");
                    break;
                }
            }

            // Execu1e the insert statement.
            try {
                stmt->executeUpdate();
                return;
            }CATCH_NOTHROW("Failed to save object to database! Trying again...")

            try {
                stmt->executeUpdate();
                return;
            }CATCH_NOTHROW("Failed to save object to database! Trying again...")

            stmt->executeUpdate();
            return;

        }CATCH_BLOCK("Failed to save object to database!")

    }

    /** Select all objects from this SQL database table.
     *
     *  @param[in]      groupBy     The SQL select statement group-by clause.
     *  @param[in]      orderBy     The SQL select statement order-by clause.
     *  @param[in]      limit       The SQL select statement result set row limit value.
     *  @param[in]      offset      The SQL select statement result set row offset.
     *
     *  @throw          util::log::MessageException if an error occurs while querying the database.
     *
     *  @return         A vector of shared pointers to the object selected from the database.
     */
    vector<shared_ptr<T> > SelectAllObjectsFromDatabase(
            const string& groupBy = "",
            const string& orderBy = "",
            const unsigned int& limit = 0,
            const unsigned int& offset = 0) const {

        return SelectObjectsFromDatabase("", groupBy, orderBy, limit, offset);

    }

    /** Get a single object from the database.
     *
     *  @param[in]      where       The SQL select statement where-condition clause.
     *  @param[in]      groupBy     The SQL select statement group-by clause.
     *  @param[in]      orderBy     The SQL select statement order-by clause.
     *  @param[in]      offset      The SQL select statement result set row offset.
     *
     *  @throw          util::log::MessageException if an error occurs while querying the database.
     *
     *  @return         A shared pointer to the object selected from the database.
     */
    shared_ptr<T> SelectObjectFromDatabase(
            const string& where = "",
            const string& groupBy = "",
            const string& orderBy = "",
            const unsigned int& offset = 0) const {

        shared_ptr<T> x;

        try {

            vector<shared_ptr<T> > res(
                    SelectObjectsFromDatabase(where, groupBy, orderBy, 1, offset));

            if(!res.empty())
                x = res.at(0);

        }CATCH_BLOCK("Failed to get object from database!")

        return x;
    }

    /** Select objects from this SQL database table.
     *
     *  @param[in]      where       The SQL select statement where-condition clause.
     *  @param[in]      groupBy     The SQL select statement group-by clause.
     *  @param[in]      orderBy     The SQL select statement order-by clause.
     *  @param[in]      limit       The SQL select statement result set row limit value.
     *  @param[in]      offset      The SQL select statement result set row offset.
     *
     *  @throw          util::log::MessageException if an error occurs while querying the database.
     *
     *  @return         A vector of shared pointers to the object selected from the database.
     */
    vector<shared_ptr<T> > SelectObjectsFromDatabase(
            const string& where = "",
            const string& groupBy = "",
            const string& orderBy = "",
            const unsigned int& limit = 0,
            const unsigned int& offset = 0) const {

        vector<shared_ptr<T> > resultVector;

        try {
            // Execute query.
            resultVector = GenerateObjectList(
                    executeSelectQuery(
                            getColumnSelectNames(),
                            table_name_,
                            where,
                            groupBy,
                            orderBy,
                            limit,
                            offset));
        }CATCH_BLOCK("Failed to query objects from database!")

        return resultVector;
    }


    /** Select a value from the SQL database table represented by this object.
     *
     *  @warning        The "select expression" clause must name a singular element when querying a
     *                  singular value from the database.
     *
     *  @note           The result set of this query is always limited to a single result.
     *
     *  @param[in]      what        The SQL select statement select-expression clause.
     *  @param[in]      where       The SQL select statement where-condition clause.
     *  @param[in]      groupBy     The SQL select statement group-by clause.
     *  @param[in]      orderBy     The SQL select statement order-by clause.
     *  @param[in]      offset      The SQL select statement result set row offset.
     *
     *  @throw          log::MessageException if an error occurs while querying the database or if
     *                  con == NULL.
     *
     *  @return         The selected value from the SQL database table represented by this object.
     */
    template<typename TT> TT SelectValueFromDatabase(
        const string& what,
        const string& where = "",
        const string& groupBy = "",
        const string& orderBy = "",
        const unsigned int& offset = 0) const {

      TT val = TT();

      try {
        // Get table column.
        vector<shared_ptr<SqlTableColumnDescriptionBase> >::const_iterator it;
        for(it = table_columns_.begin(); it < table_columns_.end(); it++)
          if((*it)->column_name() == what)
            break;

        if(it >= table_columns_.end())
          EXC_THROW("'" + what + "' is not a column of " + table_name_ + "!");
        shared_ptr < SqlTableColumnDescriptionBase > column(*it);

        // Query the database.
        string query = GenerateSqlSelectStr(
            column->column_select_name(),
            table_name_,
            where,
            groupBy,
            orderBy,
            1,
            offset);
        shared_ptr < ResultSet > results(executeQuery(query));

        if(results->next())
          switch(column->data_type()) {

            case COLUMN_DATA_TYPE_BOOL:             // Get boolean value.

              val = static_cast<TT>(results->getBoolean(column->column_name()));

              break;

            case COLUMN_DATA_TYPE_VECTOR_UINT8_T:  // Get byte vector value.

              try {

                /** @todo   Implement the functionality to parse values of type
                 *          vector<uint8_t> from the result set of an SQL query.
                 *
                 */

                EXC_THROW("The SelectValueFromDatabase function does not support values of"
                    " type vector<uint8_t>!");

              }CATCH_BLOCK("Failed to parse by vector value from SQL result set!")

              break;

            case COLUMN_DATA_TYPE_DOUBLE:           // Get double value.

              val = static_cast<TT>(results->getDouble(column->column_name()));

              break;

            case COLUMN_DATA_TYPE_FLOAT:            // Get float value.

              val = static_cast<TT>(results->getDouble(column->column_name()));

              break;

            case COLUMN_DATA_TYPE_INT:              // Get integer value.

              val = static_cast<TT>(results->getInt(column->column_name()));

              break;

            case COLUMN_DATA_TYPE_INT16_T:              // Get integer value.

              val = static_cast<TT>(results->getInt(column->column_name()));

              break;

            case COLUMN_DATA_TYPE_LONG:             // Get long integer value.

              val = static_cast<TT>(results->getInt64(column->column_name()));

              break;

            case COLUMN_DATA_TYPE_POSIX_TIME:       // Get POSIX time value.

              try {

                bpt::ptime time = bpt::time_from_string(
                    results->getString(column->column_name()).asStdString());
                val = *reinterpret_cast<TT*>(&time);

              }CATCH_BLOCK("Failed to set POSIX time value from query result set!")

              break;

            case COLUMN_DATA_TYPE_STRING:           // Get string value.

              try {

                val = lexical_cast < TT > (results->getString(column->column_name()).c_str());

              }CATCH_BLOCK("Lexical cast to return type failed!")

              break;

            case COLUMN_DATA_TYPE_UINT16_T:         // Get unsigned short value.

              val = static_cast<TT>(results->getUInt(column->column_name()));

              break;

            case COLUMN_DATA_TYPE_UINT8_T:          // Get unsigned short value.

              val = static_cast<TT>(results->getUInt(column->column_name()));

              break;

            case COLUMN_DATA_TYPE_UNKOWN:

              // If an unknown value type is encountered, throw exception.

              /* No break. */

            default:

              EXC_THROW("Failed to get column value for unknown type!");

              break;

          }

      }CATCH_BLOCK("Failed to query '" + what + "' from database!")

      return val;
    }



    /** Generate a string containing comma-separated, ordered list of column names for this SQL
     *  table from the column descriptions.
     *
     *  @note           This function is used to generate both INSERT and SELECT query
     *                  statements for the SQL table.
     *
     *  @return         A string containing comma-separated, ordered list of column names for
     *                  this SQL table.
     */
    string getColumnNames() const {
        ostringstream os;

        for(size_t i = 0; i < table_columns_.size(); ++i) {
            os << (i ? ", " : "") << table_columns_.at(i)->column_name();
        }

        return os.str();
    }

    /** Generate a string containing comma-separated, ordered list of column names used for select
     *  statements for this SQL table from the column descriptions.
     *
     *  @note           This function is used to generate both INSERT and SELECT query
     *                  statements for the SQL table.
     *
     *  @return         A string containing comma-separated, ordered list of column names for
     *                  this SQL table.
     */
    string getColumnSelectNames() const {
        ostringstream os;

        for(size_t i = 0; i < table_columns_.size(); ++i) {
            os << (i ? ", " : "") << table_columns_.at(i)->column_select_name();
        }

        return os.str();
    }

    /** Get the vector of shared pointers to ColumnDescription objects describing the columns
     *  of this SQL table.
     *
     *  @return         The vector of shared pointers to ColumnDescription objects describing
     *                  the columns of this SQL table.
     */
    vector<shared_ptr<SqlTableColumnDescriptionBase> > getColumns() const {
        return table_columns_;
    }

    /** Generate an SQL INSERT statement for this SQL table.
     *
     *  @return         An SQL INSERT statement for this SQL table.
     */
    string generateInsertStatment() {
        ostringstream os;
        size_t nUpdate = 0;

        os << "INSERT INTO \n\t" << table_name_ << "(" << getColumnNames() << ") \n";
        os << "VALUES\n\t(" << values(table_columns_.size()) << ") \n";
        os << "ON DUPLICATE KEY UPDATE \n\t";

        // Iterate over columns.
        for(size_t i = 0; i < table_columns_.size(); ++i) {
            shared_ptr<SqlTableColumnDescriptionBase> column = table_columns_.at(i);
            if(!column->is_primary_key())
                os << (nUpdate++ ? ", " : "")
                   << column->column_name()
                   << "=VALUES("
                   << column->column_name()
                   << ")";
        }

        os << ";";

        return insert_on_duplicate_key_update_statement_ = os.str();
    }

    /** Generate the "select all" SQL statement for this table.
     *
     *  @return         The "select all" SQL statement for this table.
     */
    string generateSelectAllStatement() {
        return select_statement_ = GenerateSqlSelectStr(getColumnSelectNames(), table_name_);
    }

    /** Generate a values string component of an SQL insert statement with n values.
     *
     *  @param[in]      n               The number of values in the SQL insert statement.
     *
     *  @return         A values string component of an SQL insert statement with n values.
     */
    static string values(size_t n) {
        ostringstream os;

        for(size_t i = 0; i < n; ++i)
            os << (i ? ",?" : "?");

        return os.str();
    }

};

} /* namespace mysql */
} /* namespace util */
#endif /* TABLEDESCRIPTION_H_ */
