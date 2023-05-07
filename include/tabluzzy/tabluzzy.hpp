#ifndef TABLUZZY_HPP
#define TABLUZZY_HPP

#include <statsi/statsi.hpp>  // library of statistical functions to be used in program written by Mubarak
#include <string>
#include <variant>
#include <vector>
using namespace std;
// Namespace for use in this application

enum ValueType { str = 0, flt = 1 };

class Column {
 public:
  Column(string columnName, ValueType dttype);
  ~Column();
  string& operator[](size_t rowNo);
  string operator[](const size_t rowNo) const;

  string getValueAt(size_t rowNo);  // gets value at row number 'rowNo'
  void setValueAt(size_t rowNo,
                  string value);  // sets the value of row at 'rowNo' to float
                                  // value of 'value'

  void setHeader(string header);
  string getHeader();
  vector<int> getPrimes();
  void setIndex(size_t i);
  int getIndex();

  void setValueType(ValueType);
  ValueType getValueType();

  void displayColumn();

  float getMinimumValue();
  float getMaximumValue();
  float getMedian();
  float getMean();
  float getVariance();
  void pushValue(string value);
  float getStdDeviation();
  tuple<float, float> getRegression();
  vector<string> getAllValues();
  void deleteRow(size_t rowIndex);
  void insertAtRowIndex(size_t rowIndex, string value);

 private:
  int index;
  string header;
  vector<string> rows;
  ValueType type;
};

class Table {
 private:
  unsigned int columns, rows;
  vector<Column> data;

 public:
  Table(unsigned int columns, unsigned int rows);
  Table();
  ~Table();

  void addColumn(string header,
                 ValueType dttype);  // Creates new column with header 'header'
                                     // and appends it to the list of columns

  bool columnExists(string header);

  Column& operator[](size_t i);
  Column operator[](const size_t i) const;

  Column& getColumnByHeader(string i);

  string getValueAt(string header, size_t rowNo);
  string getValueAt(int colIndex, size_t rowNo);

  vector<string> getAllColumnHeaders();

  void displayTable() const;
  vector<string> to_csv();

  void from_csv(vector<vector<string>>&);

  float getMinimumValue();
  float getMaxiumValue();
  float getMedian();
  float getMean();
  float getVariance();
  float getStdDeviation();
  vector<string> getAllValues();

  void displayReport();
  int getNumberOfRows();
  int getNumberOfColumns();

  bool canBeInsertedIntoTable(vector<string> values);

  void sortColumnsByColumn(string& colHeader);

  void deleteRow(size_t rowNo);
  void deleteColumn(string& colHeader);
  void insertRowAtIndex(vector<string>& rawValues, size_t rowIndex);
  vector<string> to_html();
  void sortTableByColumn(string& colHeader);
  void swapTablRows(size_t rowIndex1, size_t rowIndex2);
  vector<string> getAllValuesInRow(size_t rowNo);
  int getRowIndexOfFirstOccurrence(string& colHeader, string value);
  int getRowIndexOfFirstOccurrence(string& colHeader, size_t value);
  void flushTable();
};

#endif
