#ifndef TABLUZZY_HPP
#define TABLUZZY_HPP

#include <statsi/statsi.hpp>  // library of statistical functions to be used in program written by Mubarak
#include <string>
#include <variant>
#include <vector>
using namespace std;

// Enum to represent the data types of columns
// str = strings
// flt = float / numerical values
enum ValueType { str = 0, flt = 1 };

/// @brief Class for column, used to store the values of the column of the table
/// and to interact with those values on a column by column basis
class Column {
  // publicly accessible properites and members
 public:
  /// @brief  constructor member, takes in column name and datatype
  /// @param columnName - the name of the column
  /// @param dttype - the datatype
  Column(string columnName, ValueType dttype);
  // destructor member
  ~Column();

  /// @brief subscript operator method used to access elements in the column
  /// using the [] operator
  /// @param rowNo the index of the row number to access
  /// @return the string representing the value at that row index
  string& operator[](size_t rowNo);

  /// @brief returns the value at row index rowNo
  /// @param rowNothe index of the row number to access
  /// @return the string representing the value at that row index
  string getValueAt(size_t rowNo);

  /// @brief sets the value at row index rowNo to the value
  /// @param rowNo the index of the row to set
  /// @param value the value to set the row to
  void setValueAt(size_t rowNo, string value);

  /// @brief sets the header of the column to header
  /// @param header the new header
  void setHeader(string header);

  /// @brief gets the header of the column
  /// @return the header of the column
  string getHeader();

  /// @brief returns the prime numbers in a column
  /// @return the prime numbers in the column
  vector<int> getPrimes();

  /// @brief sets the index of the column to the index provided
  /// @param i the new index of the column
  void setIndex(size_t i);

  /// @brief gets the index of the table
  /// @return the new index of the column
  int getIndex();

  /// @brief sets the datatype of the column to the new datatype
  /// @param datatype the new datatype to set it to
  void setValueType(ValueType datatype);

  /// @brief gets the value type of the column
  /// @return the datatype of the column
  ValueType getValueType();

  /// @brief displays the values in the column
  void displayColumn();

  /// @brief gets the minimum value in the column
  /// @return the minimum value in the column
  float getMinimumValue();

  /// @brief  gets the maximum value i nthe column
  /// @return the maximum value in the column
  float getMaximumValue();

  /// @brief  gets the median value in the column
  /// @return the median value in the column
  float getMedian();

  /// @brief gets the mean value in the column
  /// @return the mean value in the column
  float getMean();

  /// @brief gets the variance of all values in the column
  /// @return the variance of the values in the column
  float getVariance();

  /// @brief adds a value to the last row in column
  /// @param value the value to add
  void pushValue(string value);

  /// @brief gets the standard deviation of all the values in the column
  /// @return the standard deviation
  float getStdDeviation();

  /// @brief gets the regression values for the values in the colun
  /// @return gets the regression values for the values in the column
  tuple<float, float> getRegression();

  /// @brief gets all the values in the column
  /// @return a list of all the values in the column
  vector<string> getAllValues();

  /// @brief delets the row at index rowIndex
  /// @param rowIndex the index of the row to be deleted
  void deleteRow(size_t rowIndex);

  /// @brief insert a row at index rowIndex with value value
  /// @param rowIndex the rowIndex of the row to insert
  /// @param value the value of the new rowIndex
  void insertAtRowIndex(size_t rowIndex, string value);

  // private memebers of the class Column
 private:
  // the index of the column in the table
  int index;
  // the header of the column
  string header;
  // the values of the column, stored in string
  vector<string> rows;
  // the datatype of the columnƒ
  ValueType type;
};

/// @brief Class for the table that contains all the columns of the table
class Table {
  // private members
 private:
  // the number of the columns and rows
  unsigned int columns, rows;
  // the list of columns
  vector<Column> data;

  // public members
 public:
  /// @brief constructor method that takes the number of columns and rows to
  /// initalize
  /// @param columns the number of columns
  /// @param rows the number of rows
  Table(size_t columns, size_t rows);

  /// @brief empty overloaded constructor method that initializes the table to
  /// an empty table
  Table();

  // destructor method
  ~Table();

  /// @brief adds a new column with header header and datatype dttype
  /// @param header header of the new column
  /// @param dttype datatype of the new dttype
  void addColumn(string header, ValueType dttype);

  /// @brief checks if the column exists in the table
  /// @param header the header of the column to check
  /// @return true if it exists, false if it doesnt
  bool columnExists(string header);

  /// @brief subscript operator that returns a reference to the column at index
  /// i
  /// @param i the index of the column
  /// @return the reference of the column
  Column& operator[](size_t i);

  /// @brief overloaded subscript operator that returns the column at index i
  /// @param i the index of the column
  /// @return the column at index i
  Column operator[](const size_t i) const;

  /// @brief gets the reference to the column by header
  /// @param header the header of the column to get
  /// @return the reference to the column
  Column& getColumnByHeader(string header);

  /// @brief gets the value at column with header header and row index rowNo
  /// @param header the header of the column to query
  /// @param rowNo the row index to query
  /// @return the stirng to get
  string getValueAt(string header, size_t rowNo);

  /// @brief gets all column headers
  /// @return a list of all the column headers
  vector<string> getAllColumnHeaders();

  /// @brief displays the table in ASCII text format
  void displayTable() const;

  /// @brief converts the table to csv
  /// @return list of lines of csv
  vector<string> to_csv();

  /// @brief populates the table with values parsed from csv
  /// @param csv 2D array of the parsed comma seperated values
  void from_csv(vector<vector<string>>& csv);

  /// @brief gets the minimum value in the table
  /// @return minimum value in the table
  float getMinimumValue();

  /// @brief gets the maxium value in the table
  /// @return maximum value in the table
  float getMaxiumValue();

  /// @brief get the median value in the table
  /// @return the median value un the table
  float getMedian();

  /// @brief gets the mean value in the table
  /// @return the mean value in the table
  float getMean();

  /// @brief gets the variance of the table
  /// @return the variance of the table
  float getVariance();

  /// @brief gets the standard deviation of the table
  /// @return the standard deviation
  float getStdDeviation();

  /// @brief  gets all values in the table
  /// @return th elist of all values in the table
  vector<string> getAllValues();

  /// @brief displays a report of all statistical values of the column in the
  /// table
  void displayReport();

  /// @brief  gets the number of rows of the table
  /// @return the number of rows in the table
  int getNumberOfRows();

  /// @brief gets the number of columns in the table
  /// @return the number of columns in the table
  int getNumberOfColumns();

  /// @brief checks if the values can be inserted into the table
  /// @param values the values to check against the table
  /// @return true if they can be inserted, false if they can't
  bool canBeInsertedIntoTable(vector<string> values);

  /// @brief deletes the column by its column header
  /// @param colHeader the column header of the column to be deleted
  void deleteColumn(string& colHeader);

  /// @brief inserts a list of values to the row index at rowIndex
  /// @param rawValues the list of values to be inserted
  /// @param rowIndex the row index of the row to insert the values in
  void insertRowAtIndex(vector<string>& rawValues, size_t rowIndex);

  /// @brief converts the content of the table to html
  vector<string> to_html();

  /// @brief sorts the table by the column with header colHeader
  /// @param colHeader the header of the column to be sorted
  void sortTableByColumn(string& colHeader);

  /// @brief swaps the row at rowIndex1 with the row at index rowIndex2
  /// @param rowIndex1 the index of the first row to be swapped
  /// @param rowIndex2 the index of the second row to be swapped
  void swapTablRows(size_t rowIndex1, size_t rowIndex2);

  /// @brief gets all the values in row rowNo
  /// @param rowNo the row index of the row to get all the values for
  /// @return the list of values at that row index
  vector<string> getAllValuesInRow(size_t rowNo);

  /// @brief gets the index of the first occurrence in column with header
  /// colHeader
  /// @param colHeader the header of the column
  /// @param value the value to find search for
  /// @return the index of the first occurrence
  int getRowIndexOfFirstOccurrence(string& colHeader, string value);

  /// @brief gets row index of the first occurrence in the column with header
  /// colHeader
  /// @param colHeader the header of the column to search in
  /// @param value the value to search for
  /// @return the index of the first element
  int getRowIndexOfFirstOccurrence(string& colHeader, size_t value);

  /// @brief delets the row at index rowIndex
  /// @param rowIndex the index of the row to be deleted
  void deleteRow(size_t rowIndex);

  /// @brief flushes all the previous values of the table
  void flushTable();
};

#endif
