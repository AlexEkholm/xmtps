/** The @ref util::mysql::SqlTableColumnDescription class header file.
 *
 *  @file
 *
 *  @version    0.6
 *
 *  @date       Oct 18, 2014 12:37:26 PM
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

#ifndef COLUMNDESCRIPTION_H_
#define COLUMNDESCRIPTION_H_

/* Includes */

// local
#include "../config/LocalMysqlConfiguration.h"
#include "DateUtils.h"

// boost
#include <boost/function.hpp>

/** Macro definition of SQL table column of type bool. */
#define sql_colb(T, ...) std::make_shared<util::mysql::Column_bool<T> >(__VA_ARGS__)

/** Macro definition of SQL table column of type double. */
#define sql_cold(T, ...) std::make_shared<util::mysql::Column_double<T> >(__VA_ARGS__)

/** Macro definition of SQL table column of type boost::filesystem::path. */
#define sql_colp(T, ...) std::make_shared<util::mysql::Column_filesystem_path<T> >(__VA_ARGS__)

/** Macro definition of SQL table column of type float. */
#define sql_colf(T, ...) std::make_shared<util::mysql::Column_float<T> >(__VA_ARGS__)

/** Macro definition of SQL table column of type int. */
#define sql_coli(T, ...) std::make_shared<util::mysql::Column_int<T> >(__VA_ARGS__)

/** Macro definition of SQL table column of type int16_t. */
#define sql_ci16(T, ...) std::make_shared<util::mysql::Column_int16_t<T> >(__VA_ARGS__)

/** Macro definition of SQL table column of type long. */
#define sql_coll(T, ...) std::make_shared<util::mysql::Column_long<T> >(__VA_ARGS__)

/** Macro definition of SQL table column of type boost::posix_time::ptime. */
#define sql_colt(T, ...) std::make_shared<util::mysql::Column_posix_time<T> >(__VA_ARGS__)

/** Macro definition of SQL table column of type string. */
#define sql_cols(T, ...) std::make_shared<util::mysql::Column_string<T> >(__VA_ARGS__)

/** Macro definition of SQL table column of type uint16_t. */
#define sql_cu16(T, ...) std::make_shared<util::mysql::Column_uint16_t<T> >(__VA_ARGS__)

/** Macro definition of SQL table column of type uint8_t. */
#define sql_cou8(T, ...) std::make_shared<util::mysql::Column_uint8_t<T> >(__VA_ARGS__)

/** Macro definition of SQL table column of type vector<uint8_t>. */
#define sql_colv(T, ...) std::make_shared<util::mysql::Column_vector_uint8_t<T> >(__VA_ARGS__)

namespace util {
namespace mysql {

using boost::filesystem::path;
using boost::function;
using boost::posix_time::ptime;
using boost::posix_time::not_a_date_time;
using boost::posix_time::time_from_string;
using boost::posix_time::to_iso_string;
using std::ostringstream;
using std::vector;
using sql::PreparedStatement;
using sql::SQLString;
using sql::ResultSet;

/* Class Definition */

/** The ColumnDescription class is used to describe the attributes of a particular SQL table column
 * including its name, type, and whether or not the column is part of the primary key for the SQL
 * table. A collection of ColumnDescrition object may be used to fully describe the attributes of
 * one or more SQL database tables.
 */
typedef enum ColumnDataType {
    COLUMN_DATA_TYPE_UNKOWN,
    COLUMN_DATA_TYPE_BOOL,
    COLUMN_DATA_TYPE_CHAR,

    COLUMN_DATA_TYPE_DOUBLE,
    COLUMN_DATA_TYPE_FILESYSTEM_PATH,
    COLUMN_DATA_TYPE_FLOAT,
    COLUMN_DATA_TYPE_INT,
    COLUMN_DATA_TYPE_INT16_T,
    COLUMN_DATA_TYPE_LONG,
    COLUMN_DATA_TYPE_POSIX_TIME,
    COLUMN_DATA_TYPE_STRING,
    COLUMN_DATA_TYPE_TEST,
    COLUMN_DATA_TYPE_UINT16_T,
    COLUMN_DATA_TYPE_UINT8_T,
    COLUMN_DATA_TYPE_VECTOR_UINT8_T,
} ColumDataType_t;

/** An abstract base class for the description of an SQL database table column.
 *
 *  This base class is necessary for utilizing polymorphism such that a pointers to classes that
 *  inherit from this class may be stored in a single vector.
 *
 */
class SqlTableColumnDescriptionBase {

private:

    /* Data Members */

    /** The name of the SQL table column. */
    string column_name_;

    /** The index of the SQL table column.
     *
     */
    size_t column_index_;

    /** The enumerated SQL table column data type. */
    ColumDataType_t data_type_;

    /** Indicates whether the column is part of the primary key for the SQL table.
     *
     */
    bool is_primary_key_;

public:

    /* Member Functions */

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  type            They enumerated SQL table column type.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    SqlTableColumnDescriptionBase(string column_name, ColumDataType_t type, bool is_primary_key) :
                    column_name_(column_name),
                    column_index_(0),
                    data_type_(type),
                    is_primary_key_(is_primary_key) {
        /* Do nothing. */
    }

    /** The destructor.
     *
     */
    virtual ~SqlTableColumnDescriptionBase() {
        /* Do nothing. */
    }

    /** Get the index of the SQL table column.
     *
     *  @return     The index of the SQL table column.
     */
    size_t column_index() const {
        return column_index_;
    }

    /** Get the name of the SQL table column.
     *
     *  @return     The name of the SQL table column.
     */
    const string& column_name() const {
        return column_name_;
    }

    /** Get the name of the SQL table column to be used in a @c SELECT query.
     *
     *  @return     The name of the SQL table column to be used in a @c SELECT query.
     */
    string column_select_name() const {
        return data_type_ == COLUMN_DATA_TYPE_VECTOR_UINT8_T ?
                "HEX(" + column_name_ + ") AS " + column_name_ :
                column_name_;
    }

    /** Get the the enumerated SQL column data type.
     *
     *  @return     The the enumerated SQL column data type.
     */
    ColumDataType_t data_type() const {
        return data_type_;
    }

    /** Is this column a primary key for the corresponding SQL database table?
     *
     *  @return     TRUE if this column is a primary key for the corresponding SQL database table,
     *              or FALSE otherwise.
     */
    bool is_primary_key() const {
        return is_primary_key_;
    }

    /** Set the index of this SQL table column.
     *
     *  @param[in]  idx     The index to set.
     */
    void set_column_index(size_t idx) {
        column_index_ = idx;
    }
};

/** The @p SqlTableColumnDescription class is a template class used to describe an SQL table
 *  column for class @p C and data member of type @p T.
 *
 */
template<class C, typename T> class SqlTableColumnDescription:
        public SqlTableColumnDescriptionBase {

private:

    /** The accessor function object wrapper.
     *
     *  This object is used as a wrapper for the accessor method of the template class C. It is
     *  used to retrieve the value of the SQL table column from the object for generating an
     *  INSERT or UPDATE statement.
     */
    function<T(const C&)> accessor_;

    /** The mutator function object wrapper.
     *
     *  This object is used as a wrapper for the mutator method of the template class C. It is
     *  used to set the value of an object from the result set of an SQL SELECT query
     *  statement.
     */
    function<void(C&, T)> mutator_;

protected:

    /** Get the object member value from the object.
     *
     *  @param[in]      obj         The object of type @p C.
     *
     *  @return         The value of type @p T of the object member corresponding to this SQL
     *                  column.
     */
    T GetValueFromObject(const C& obj) const {
        return accessor_(obj);
    }

    /** Set the object member value.
     *
     *  @param[in]      obj         The object of type @p C.
     *  @param[in]      value       The value of type @p T, to set.
     */
    void SetObjectValue(C& obj, const T& value) const {
        mutator_(obj, value);
    }

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  data_type       They enumerated SQL table column type.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    SqlTableColumnDescription(
            string column_name,
            ColumDataType_t data_type,
            T (C::*accessor)() const = NULL,
            void (C::*mutator)(T) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescriptionBase(column_name, data_type, is_primary_key),
                    accessor_(accessor),
                    mutator_(mutator) {
        /* Do nothing. */
    }

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  data_type       They enumerated SQL table column type.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    SqlTableColumnDescription(
            string column_name,
            ColumDataType_t data_type,
            T (C::*accessor)() const = NULL,
            void (C::*mutator)(const T&) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescriptionBase(column_name, data_type, is_primary_key),
                    accessor_(accessor),
                    mutator_(mutator) {
        /* Do nothing. */
    }

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  data_type       They enumerated SQL table column type.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    SqlTableColumnDescription(
            string column_name,
            ColumDataType_t data_type,
            const T& (C::*accessor)() const = NULL,
            void (C::*mutator)(const T&) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescriptionBase(column_name, data_type, is_primary_key),
                    accessor_(accessor),
                    mutator_(mutator) {
        /* Do nothing. */
    }
};

/** Template column description class for value of type bool.
 *
 */
template<class C> class Column_bool:
        public SqlTableColumnDescription<C, bool> {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_bool(string column_name,
    bool (C::*accessor)() const = NULL, void (C::*mutator)(bool) = NULL,
    bool is_primary_key = false) :
                    SqlTableColumnDescription<C, bool>(
                            column_name,
                            COLUMN_DATA_TYPE_BOOL,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        this->SetObjectValue(obj, results.getBoolean(this->column_name()));
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        stmt.setBoolean(this->column_index(), this->GetValueFromObject(obj));
    }
};

/** Template column description class for value of type double.
 *
 */
template<class C> class Column_char:
        public SqlTableColumnDescription<C, char> {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_char(
            string column_name,
            double (C::*accessor)() const = NULL,
            void (C::*mutator)(double) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, double>(
                            column_name,
                            COLUMN_DATA_TYPE_DOUBLE,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        string str(results.getString(this->column_name()).asStdString());

        if(str.length() > 0)

            this->SetObjectValue(obj, str.at(0));

        else

            this->SetObjectValue(obj, '\0');
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        char c[2];

        memset(c, 0, sizeof(c));

        c[0] = this->GetValueFromObject(obj);

        stmt.setString(this->column_index(), string(c));
    }
};

/** Template column description class for value of type double.
 *
 */
template<class C> class Column_double:
        public SqlTableColumnDescription<C, double> {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_double(
            string column_name,
            double (C::*accessor)() const = NULL,
            void (C::*mutator)(double) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, double>(
                            column_name,
                            COLUMN_DATA_TYPE_DOUBLE,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        this->SetObjectValue(obj, results.getDouble(this->column_name()));
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        stmt.setDouble(this->column_index(), this->GetValueFromObject(obj));
    }
};

/** Template column description class for value of type path.
 *
 */
template<class C> class Column_filesystem_path:
        public SqlTableColumnDescription<C, path> {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_filesystem_path(
            string column_name,
            const path& (C::*accessor)() const = NULL,
            void (C::*mutator)(const path&) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, path>(
                            column_name,
                            COLUMN_DATA_TYPE_FILESYSTEM_PATH,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_filesystem_path(
            string column_name,
            path (C::*accessor)() const = NULL,
            void (C::*mutator)(const path&) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, path>(
                            column_name,
                            COLUMN_DATA_TYPE_FILESYSTEM_PATH,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        this->SetObjectValue(obj, path(results.getString(this->column_name()).asStdString()));
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        stmt.setString(this->column_index(), this->GetValueFromObject(obj).string());
    }
};

/** Template column description class for value of type float.
 *
 */
template<class C> class Column_float:
        public SqlTableColumnDescription<C, float> {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_float(
            string column_name,
            float (C::*accessor)() const = NULL,
            void (C::*mutator)(float) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, float>(
                            column_name,
                            COLUMN_DATA_TYPE_FLOAT,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        this->SetObjectValue(obj, results.getDouble(this->column_name()));
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        stmt.setDouble(this->column_index(), this->GetValueFromObject(obj));
    }
};

/** Template column description class for value of type int.
 *
 */
template<class C> class Column_int:
        public SqlTableColumnDescription<C, int> {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_int(
            string column_name,
            int (C::*accessor)() const = NULL,
            void (C::*mutator)(int) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, int>(
                            column_name,
                            COLUMN_DATA_TYPE_INT,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        this->SetObjectValue(obj, results.getInt(this->column_name()));
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        stmt.setInt(this->column_index(), this->GetValueFromObject(obj));
    }
};

/** Template column description class for value of type int16_t.
 *
 */
template<class C> class Column_int16_t:
        public SqlTableColumnDescription<C, int16_t> {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_int16_t(
            string column_name,
            int16_t (C::*accessor)() const = NULL,
            void (C::*mutator)(int16_t) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, int16_t>(
                            column_name,
                            COLUMN_DATA_TYPE_INT16_T,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        this->SetObjectValue(obj, results.getInt(this->column_name()));
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        stmt.setInt(this->column_index(), this->GetValueFromObject(obj));
    }
};

/** Template column description class for value of type long.
 *
 */
template<class C> class Column_long:
        public SqlTableColumnDescription<C, long> {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_long(
            string column_name,
            long (C::*accessor)() const = NULL,
            void (C::*mutator)(long) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, long>(
                            column_name,
                            COLUMN_DATA_TYPE_LONG,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        this->SetObjectValue(obj, results.getInt64(this->column_name()));
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        stmt.setInt64(this->column_index(), this->GetValueFromObject(obj));
    }
};

/** Template column description class for value of type boost::posix_time::ptime.
 *
 */
template<class C> class Column_posix_time:
        public SqlTableColumnDescription<C, ptime> {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_posix_time(
            string column_name,
            ptime (C::*accessor)() const = NULL,
            void (C::*mutator)(const ptime&) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, ptime>(
                            column_name,
                            COLUMN_DATA_TYPE_POSIX_TIME,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_posix_time(
            string column_name,
            const ptime& (C::*accessor)() const = NULL,
            void (C::*mutator)(const ptime&) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, ptime>(
                            column_name,
                            COLUMN_DATA_TYPE_POSIX_TIME,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        string time_str = results.getString(this->column_name()).asStdString();
        this->SetObjectValue(
                obj,
                time_str.empty() || time_str == "0000-00-00 00:00:00" ?
                        not_a_date_time :
                        time_from_string(time_str));
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        ptime time(this->GetValueFromObject(obj));
        stmt.setDateTime(
                this->column_index(),
                time.is_not_a_date_time() ? "0000-00-00 00:00:00" : to_iso_string(time));
    }
};

/** Template column description class for value of type string.
 *
 */
template<class C> class Column_string:
        public SqlTableColumnDescription<C, string> {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_string(
            string column_name,
            string (C::*accessor)() const = NULL,
            void (C::*mutator)(const string&) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, string>(
                            column_name,
                            COLUMN_DATA_TYPE_STRING,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_string(
            string column_name,
            const string& (C::*accessor)() const = NULL,
            void (C::*mutator)(const string&) = NULL,
            bool is_primary_key = false) ATTR_DEPRECATED :SqlTableColumnDescription<C, string>(
            column_name,
            COLUMN_DATA_TYPE_STRING,
            accessor,
            mutator,
            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        this->SetObjectValue(obj, results.getString(this->column_name()).asStdString());
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        stmt.setString(this->column_index(), this->GetValueFromObject(obj));
    }
};

/** Template column description class for value of type uint16_t.
 *
 */
template<class C> class Column_uint16_t:
        public SqlTableColumnDescription<C, uint16_t> {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_uint16_t(
            string column_name,
            uint16_t (C::*accessor)() const = NULL,
            void (C::*mutator)(uint16_t) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, uint16_t>(
                            column_name,
                            COLUMN_DATA_TYPE_UINT16_T,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        this->SetObjectValue(obj, results.getInt(this->column_name()));
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        stmt.setInt(this->column_index(), this->GetValueFromObject(obj));
    }
};

/** Template column description class for value of type uint8_t.
 *
 */
template<class C> class Column_uint8_t:
        public SqlTableColumnDescription<C, uint8_t> {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_uint8_t(
            string column_name,
            uint8_t (C::*accessor)() const = NULL,
            void (C::*mutator)(uint8_t) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, uint8_t>(
                            column_name,
                            COLUMN_DATA_TYPE_UINT8_T,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        this->SetObjectValue(obj, results.getInt(this->column_name()));
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        stmt.setInt(this->column_index(), this->GetValueFromObject(obj));
    }
};

/** Template column description class for value of type vector<uint8_t>.
 *
 */
template<class C> class Column_vector_uint8_t:
        public SqlTableColumnDescription<C, vector<uint8_t>>,
        protected util::stream::StreamUtils {

public:

    /** The parameterized constructor.
     *
     *  @param[in]  column_name     The name of the SQL table column.
     *  @param[in]  accessor        A pointer to the accessor method for acquiring the value of the
     *                              corresponding object field.
     *  @param[in]  mutator         A pointer to the mutator method for setting the value of the
     *                              corresponding object field.
     *  @param[in]  is_primary_key  TRUE if this column is a primary key for the SQL table, or
     *                              FALSE otherwise.
     */
    Column_vector_uint8_t(
            string column_name,
            vector<uint8_t> (C::*accessor)() const = NULL,
            void (C::*mutator)(const vector<uint8_t>&) = NULL,
            bool is_primary_key = false) :
                    SqlTableColumnDescription<C, vector<uint8_t>>(
                            column_name,
                            COLUMN_DATA_TYPE_VECTOR_UINT8_T,
                            accessor,
                            mutator,
                            is_primary_key) {
        /* Do nothing. */
    }

    /** Set the value in the field of the corresponding object from an SQL ResultSet Object.
     *
     *  @param[in,out]  obj         A reference to the object whose field value will be set.
     *  @param[in]      results     A reference to the ResultSet object.
     */
    void SetObjectValueFromResultSet(C& obj, const ResultSet& results) const {
        vector<uint8_t> data;
        string data_hex_str(results.getString(this->column_name()).asStdString());

        for(string::const_iterator it = data_hex_str.begin(); it < data_hex_str.end() - 1;
                it += 2) {

            data.push_back((htoi(*it) << 4) | htoi(*(it + 1)));

        }

        this->SetObjectValue(obj, data);
    }

    /** Set the value for the SQL PreparedStatement object from the value of the field of the
     *  corresponding object.
     *
     *  @param[in]      obj         A reference to the object whose field value will be read.
     *  @param[in,out]  stmt        A reference to the SQL PreparedStatement object whose column
     *                              value will be set.
     */
    void SetPreparedStatementValueFromObject(const C& obj, PreparedStatement& stmt) const {
        vector<uint8_t> data(this->GetValueFromObject(obj));
        ostringstream oss;

        for_each(data.begin(), data.end(), [&](uint8_t byte) {

            oss << byte;

        });

        stmt.setString(this->column_index(), oss.str());
    }
};

} /* namespace mysql */
} /* namespace util */
#endif /* COLUMNDESCRIPTION_H_ */
