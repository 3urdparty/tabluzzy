#include <iomanip>
#include <iostream>
#include <statsi/statsi.hpp>  // library of statistical functions to be used in program written by Mubarak
#include <strfmt/strfmt.hpp>  // library of simple generic functions Mustafa and Azi wrote to be used in the main program. Source code found at libs/strfmt
#include <string>
#include <terminalcancer/terminalcancer.hpp>  // library of simple terminal helper functions to be used in program written by Mustafa

#include "tabluzzy.hpp"
using namespace std;

// Column class constructor
// neads a column header and a value type to be constructed
Column::Column(string h, ValueType t) {
  // we initialized them to the values passed to the constructor
  header = h;
  type = t;
};

// Column class Destructor
Column::~Column(){

};

string& Column::operator[](size_t rowNo) {
  // returns the value at that row number
  return rows[rowNo];
};

void Column::setValueAt(size_t rowNo, string value) {
  // sets the value at row number to the value passed
  rows[rowNo] = value;
};

void Column::pushValue(string value) {
  // add a value to a new row in columns
  rows.push_back(value);
};
void Column::displayColumn() {
  // responsible for displaying the data in the column

  // gets the terminal dimensions for the current terminal
  auto [w, h] = getTerminalDimensions();

  // draws a line using '='
  cout << endl;
  cout << "+" << setfill('=') << setw(15) << "" << setw(1) << setfill(' ')
       << "+" << endl;

  cout << "|";

  // outputs the column header for this column
  cout << bold << colorfmt(fg::green) << getHeader() << left << setw(8) << "\t"
       << clearfmt << " |" << endl;

  // for every row in rows
  for (int y = 0; y < rows.size(); y++) {
    cout << "|";

    // if the value type is float
    if (type == ValueType::flt) {
      // we set the precision for the value to 0
      cout << setw(8) << setfill(' ') << setprecision(0) << left << fixed
           << stof(rows[y]) << "\t"
           << "|";

    } else if (type == ValueType::str) {
      // if the value is of type string
      cout << setw(8) << setfill(' ') << setprecision(0) << left << fixed
           << rows[y] << "\t"
           << "|";
    }

    cout << endl;
  };
  // draws a line
  cout << "+" << setfill('=') << setw(15) << "" << setw(1) << setfill(' ')
       << "+" << endl;
};

string Column::getHeader() {
  // returns the header for that column
  return header;
};

string Column::getValueAt(size_t rowNo) {
  // returns the value at row number
  return rows[rowNo];
}

void Column::setValueType(ValueType dttype) {
  // sets the return type of the current table
  type = dttype;
};

int Column::getIndex() {
  // returns the index of the column
  return index;
};

void Column::setHeader(string h) {
  // sets the header property to h
  header = h;
};
void Column::setIndex(size_t i) {
  // sets the index of column to index i
  index = i;
};

ValueType Column::getValueType() {
  // gets the value type of the column
  return type;
};

float Column::getMinimumValue() {
  // we get the values in the column
  // we convert the values to string
  vector<float> rawValues = convertToFloats(rows);
  // we return the minimum value
  return getMin(rawValues);
};

float Column::getMaximumValue() {
  // we convert all values to float
  vector<float> rawValues = convertToFloats(rows);
  // we get the maximum value of the raw values
  return getMax(rawValues);
};

float Column::getMedian() {
  // we get all the values in the column and convert the values to float
  vector<float> values = convertToFloats(rows);
  // we calculate the median from the set of values
  return calculateMedian(values);
};
float Column::getMean() {
  // we get all the values in the column and convert the values to float
  vector<float> values = convertToFloats(rows);
  // we calculate the mean using calculateMean
  return calculateMean(values);
};
float Column::getVariance() {
  // we get all the values in the column and convert the values to float
  vector<float> values = convertToFloats(rows);
  // we calculate the variance using calculateVariance
  return calculateVariance(values);
};
float Column::getStdDeviation() {
  // we get all the values in the column and convert the values to float
  vector<float> values = convertToFloats(rows);
  // we calculate the standard standard deviation from the set of values
  return calculateStandardDeviation(values);
};

vector<string> Column::getAllValues() {
  // declare a vector of values
  vector<string> values;
  // and then populate it with the values in rows
  for (int x = 0; x < rows.size(); x++) {
    values.push_back(rows[x]);
  };
  // we return the vector
  return values;
};

tuple<float, float> Column::getRegression() {
  // we get all the values in the column and convert the values to float
  // we convert al the values to flaots
  vector<float> values = convertToFloats(rows);
  // we calculate the regression from the values and return
  return calculateRegression(values);
};

vector<int> Column::getPrimes() {
  // we declare vectors to store the primes
  vector<int> primes;

  // we convert all values in rows to ints
  vector<int> numbers = convertToInts(rows);

  // we populate the vector primes if and only if those numbers are prime
  for (int num : numbers) {
    if (isPrime(num)) primes.push_back(num);
  }
  // return that list of primes
  return primes;
};

void Column::insertAtRowIndex(size_t rowIndex, string value) {
  // inserts a new value at row Index
  rows.insert(rows.begin() + rowIndex, value);
};

void Column::deleteRow(size_t rowIndex) {
  // deletes a row at row index
  rows.erase(rows.begin() + rowIndex);
};