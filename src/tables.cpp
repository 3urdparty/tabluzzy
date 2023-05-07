#include <iomanip>
#include <iostream>
#include <strfmt/strfmt.hpp>  // library of simple generic functions Mustafa and Azi wrote to be used in the main program. Source code found at libs/strfmt
#include <utility>
#include <variant>

#include "tabluzzy.hpp"

using namespace std;

#include <terminalcancer/terminalcancer.hpp>  // library of simple terminal helper functions to be used in program written by Mustafa

// constructor for an empty table
Table::Table(){};

// overloaded constructor and set the dimensions of the table
Table::Table(size_t col, size_t row) {
  columns = col;
  rows = row;
};

// destructor class for table
Table::~Table(){};

string Table::getValueAt(string header, size_t rowNo) {
  // gets the column by column header
  Column col = getColumnByHeader(header);
  // column gets the value at row number in the column
  return col[rowNo];
};

void Table::addColumn(string header, ValueType dttype) {
  // creates a column header with header header and type dtttpye
  Column newCol(header, dttype);
  // we get the index of the new column
  int newIndex = data.size();
  // we set the column to that index
  newCol.setIndex(newIndex);
  // and we add the new column to the list of columns
  data.push_back(newCol);
};

void Table::deleteRow(size_t rowIndex) {
  // for every column in columns
  for (size_t i = 0; i < columns; i++) {
    // we get a reference to the column
    Column& col = operator[](i);
    // and we delete the row at index rowIndex
    col.deleteRow(rowIndex);
  };
  // we decrement the number of rows of the table by 1
  rows--;
};

Column& Table::operator[](size_t i) {
  // we return the column at index i
  return data[i];
};

Column Table::operator[](size_t i) const {
  // we get the column at index i
  const Column col = data[i];
  // we return the column
  return col;
};

bool Table::columnExists(string header) {
  // for every column in columns
  for (int i = 0; i <= columns; i++) {
    // if the header is the same as the header
    if (cmpstr(operator[](i).getHeader(), header)) {
      // we return true and exit the function
      return true;
    };
  };
  // otherwise if none of them matched we return false
  return false;
}

Column& Table::getColumnByHeader(string header) {
  // for every column in columns
  for (int i = 0; i <= data.size(); i++) {
    // if the column header matches the column header passed into the function
    if (cmpstr(operator[](i).getHeader(), header)) {
      // we return a reference to that column header
      return operator[](i);
    };
  };
  // otherwise we return the first column (optimally youd check for the
  // existence of the queried column using columnExists(), so youll never reach
  // this case )
  return operator[](0);
};

void Table::displayTable() const {
  if (columns == 0) {
    cout << "Table is empty" << endl;
  } else if (columns == 1) {
    operator[](0).displayColumn();
  } else {
    // gets the terminal dimension
    auto [w, h] = getTerminalDimensions();

    cout << endl;
    // draws a line
    cout << "+" << setfill('=') << setw(16 * columns - 1) << "" << setw(1)
         << setfill(' ') << "+" << endl;

    cout << "|";

    // for every column in the table
    for (int x = 0; x < columns; x++) {
      // we get the column
      Column col = operator[](x);
      // we output the header
      cout << bold << colorfmt(fg::green) << col.getHeader() << left << setw(8)
           << "\t"
           << "\t" << clearfmt;
    };
    cout << "|" << endl;
    // for every row in rows
    for (int y = 0; y < rows; y++) {
      cout << "|";
      // for every column in column
      for (int x = 0; x < columns; x++) {
        // we get column at index x
        Column col = operator[](x);
        // if the column is of type float
        if (col.getValueType() == ValueType::flt) {
          // we set the precision to 0 and output it
          cout << setw(8) << setfill(' ') << setprecision(0) << left << fixed
               << stof(col.getValueAt(y)) << "\t"
               << "|";

          // however, if the column is of type string
        } else if (col.getValueType() == ValueType::str) {
          // we output it
          cout << setw(8) << setfill(' ') << col.getValueAt(y) << "\t"
               << "|";
        }
      }
      cout << endl;
    };
    // we draw a line
    cout << "+" << setfill('=') << setw(16 * columns - 1) << "" << setw(1)
         << setfill(' ') << "+" << endl;
  }
};

void Table::from_csv(vector<vector<string>>& csv) {
  // gets the number of columns and number of rows from the first two lines in
  // the csv file and convert them to integer
  int colNo = stoi(csv[0][0]), rowNo = stoi(csv[1][0]);

  // we set the columns and rows of this table object to colNo and rowNo
  columns = colNo;
  rows = rowNo;

  // for every column in columns
  for (int x = 0; x < columns; x++) {
    // we get the header
    string header = csv[2][x];
    // we declare a variable to hold the datatype of the current column
    ValueType dttype;
    // if the type is string
    if (cmpstr(csv[3][x], "string")) {
      // we assign string to the datatype of the string
      dttype = ValueType::str;
      // however if it is numerical
    } else if (cmpstr(csv[3][x], "number")) {
      // we assign float to the data type of the column
      dttype = ValueType::flt;
    }
    // we add this new column
    addColumn(header, dttype);
  };

  // here we populate the columns in the table
  // for every row in rows of data
  for (int row = 4; row <= rows + 3; row++) {
    // for every column in columns
    for (int col = 0; col < columns; col++) {
      // we get the value from the 2D vector representing the strings
      string value = csv[row][col];

      // and we populate the columns with it
      operator[](col).pushValue(value);
    }
  }
};

vector<string> Table::to_csv() {
  // declare a variable that holds the lines of csv to be outputted
  vector<string> csv;

  // we convert the number of columns to strings
  string c = to_string(columns);
  // we convert the number of rows to strings
  string r = to_string(rows);

  // we add a line for the number of columns and a line for the number of rows
  csv.push_back(c);
  csv.push_back(r);

  // we get all the column headers from the table
  vector<string> headers = getAllColumnHeaders();
  // we join them using ',' as a delimtered
  string entry = join(headers, ",");
  // we add that line to the list of csv liens
  csv.push_back(entry);

  // we reset the entry variable
  entry = "";

  // we create a vector to store the string representation of the data types of
  // each column
  vector<string> datatypes;

  // for every column in columns
  for (int x = 0; x < columns; x++) {
    // we get the string representation of the data type of the column
    // using a ternary operator for brevity
    string dttype =
        (data[x].getValueType() == ValueType::flt) ? "number" : "string";
    // we add the representation to datatypes
    datatypes.push_back(dttype);
  }
  // we join the datatype representations using ',' as a delimiter
  entry = join(datatypes, ",");

  // we add that line to csv
  csv.push_back(entry);

  // then declare a vector for values
  vector<string> values;
  // for every row in rows
  for (int y = 0; y < rows; y++) {
    // for every column in columns
    for (int x = 0; x < columns; x++) {
      // we add the value at row y at column x
      values.push_back(data[x][y]);
    }
    // we join the values using ','
    entry = join(values, ",");
    // we add that line to entries
    csv.push_back(entry);
  };
  return csv;
}

vector<string> Table::to_html() {
  // declare a variable to store the html tags
  vector<string> tags;

  // we generate the first static part of the html
  tags.push_back(R"(
  <!DOCTYPE html>
    <html>
    <head>
      <meta charset="UTF-8">
      <title>Table</title>
      <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/meyer-reset/2.0/reset.min.css">
      <link rel="stylesheet" href="css/style.css">
    </head>
    <body>
      <section>
        <h1>Table</h1>
        <div class="tbl-header">
        <table cellpadding="0" cellspacing="0" border="0">
          <thead>
            <tr>)");

  // declare a variable that holds the tag
  string tag;

  // for every column in columns
  for (int x = 0; x < columns; x++) {
    // we get the header of that column
    string header = operator[](x).getHeader();
    // we enclose it in table header tags
    tag = "<th>" + header + "</th>";
    // we add it to tags
    tags.push_back(tag);
  };

  tags.push_back(R"(
          </tr>
        </thead>
      </table>
      </div>
      <div class="tbl-content">
      <table cellpadding="0" cellspacing="0" border="0">
    <tbody>)");

  // for every row in rows
  for (int y = 0; y < rows; y++) {
    // create a table row in the output html
    tags.push_back(R"(<tr>)");
    // for every column in columns
    for (int x = 0; x < columns; x++) {
      // gets the value from the table at column x and row y
      string value = operator[](x).getValueAt(y);
      // we enclose the value in html tags
      tag = "<td>" + value + "</td>";
      // we add that tag to the list of tags
      tags.push_back(tag);
    }
    // ending the table row
    tags.push_back(R"(</tr>)");
  };

  tags.push_back(R"(</tbody>)
   </table>)");
  tags.push_back(R"(
    </div>
    </section>
   <script src="js/index.js"></script>
   </body>
   </html>)");
  // return the list of tags to be outputted to a file
  return tags;
};

vector<string> Table::getAllColumnHeaders() {
  // declares a vector of strings to store the headers
  vector<string> headers;
  // for every column in columns
  for (int x = 0; x <= columns; x++) {
    // get the header of the colummn
    string header = operator[](x).getHeader();
    // add it to the list of headers
    headers.push_back(header);
  }

  // return the list of headers
  return headers;
};

float Table::getMinimumValue() {
  // declare a list of float values
  vector<float> values;

  // for every column in columns
  for (int x = 0; x < columns; x++) {
    // get the column
    Column col = operator[](x);
    // if the column is of type string then skip that column
    if (col.getValueType() == ValueType::str) continue;
    // however if it is of type integer then get the minimum value
    values.push_back(col.getMinimumValue());
  }

  // get the minumum of the values and return it
  return getMin(values);
};

float Table::getMaxiumValue() {
  // declare a list of float values
  vector<float> values;
  // for every column in columnss
  for (int x = 0; x < columns; x++) {
    // get the column
    Column col = operator[](x);
    // if the column is of type string then skip that column
    if (col.getValueType() == ValueType::str) continue;
    // however if it is of type integer then get the minimum value
    values.push_back(col.getMaximumValue());
  }

  // get the minumum of the values and return it
  return getMax(values);
};

vector<string> Table::getAllValues() {
  // delare a list of strings
  vector<string> rawValues;
  // for every column in columns
  for (int x = 0; x < columns; x++) {
    // get the column
    Column col = operator[](x);
    // if the column is of type string then skip that column
    if (col.getValueType() == ValueType::str) continue;
    // if the column is of type float get the values in the column
    vector<string> rawColValues = col.getAllValues();
    // for every value in rawValues
    for (string rawValue : rawColValues) {
      // add it to the list of values
      rawValues.push_back(rawValue);
    }
  };
  return rawValues;
}

float Table::getMedian() {
  // get all the values in the table
  vector<string> rawValues = getAllValues();
  // convert every value to float
  vector<float> values = convertToFloats(rawValues);
  // calculate the median and return it
  return calculateMedian(values);
};
float Table::getMean() {
  // get all the values in the table
  vector<string> rawValues = getAllValues();
  // convert every value to float
  vector<float> values = convertToFloats(rawValues);
  // calculate the mean and return it
  return calculateMean(values);
};
float Table::getVariance() {
  // get all the values if the table
  vector<string> rawValues = getAllValues();
  // convert the values to floats
  vector<float> values = convertToFloats(rawValues);
  // calculate the variance and return it
  return calculateVariance(values);
};
float Table::getStdDeviation() {
  // get all the values in the table
  vector<string> rawValues = getAllValues();
  // convert the values to floats
  vector<float> values = convertToFloats(rawValues);
  // calculate the standard deviation and return it
  return calculateStandardDeviation(values);
};

void Table::displayReport() {
  // for every column in columns
  for (int x = 0; x < columns; x++) {
    // get the column
    Column col = operator[](x);
    // if the column is of type string then skip that column
    if (col.getValueType() == ValueType::str) continue;
    // if it contains numerical values then calculate all the statistics of the
    // table
    float min = col.getMinimumValue();
    float max = col.getMaximumValue();
    float median = col.getMedian();
    float mean = col.getMean();
    float variance = col.getVariance();
    float stdv = col.getStdDeviation();
    auto [a, b] = col.getRegression();

    // and output them
    cout << "Column " << colorfmt(fg::cyan) << col.getHeader() << clearfmt
         << ":" << endl
         << setw(15) << setfill('-') << "" << endl
         << bold << "min" << clearfmt << " = " << min << endl
         << bold << "max" << clearfmt << " = " << max << endl
         << bold << "med" << clearfmt << " = " << median << endl
         << bold << "μ" << clearfmt << " = " << mean << endl
         << bold << "σ²" << clearfmt << " = " << variance << endl
         << bold << "σ" << clearfmt << " = " << stdv << endl
         << bold << "regr(x,y)" << clearfmt << " : " << bold << "Y" << clearfmt
         << " = " << colorfmt(fg::yellow) << a << clearfmt << " + "
         << colorfmt(fg::yellow) << b << clearfmt << "x" << endl
         << endl;
  };
  cout << endl;
};

int Table::getNumberOfRows() {
  // returns the number of rows in the table
  return rows;
};
int Table::getNumberOfColumns() {
  // returns the number of columns in the table
  return columns;
};

void Table::deleteColumn(string& colHeader) {
  // gets the column by its header
  Column col = getColumnByHeader(colHeader);
  // gets the index from the column
  int index = col.getIndex();
  // removes the column from the table
  data.erase(data.begin() + index, data.begin() + index + 1);

  int cols = data.size();
  columns = cols;

  for (int x = 0; x < columns; x++) {
    operator[](x).setIndex(x);
  }
  // decrements columns by 1
};

bool Table::canBeInsertedIntoTable(vector<string> values) {
  // for every column in columns
  for (int i = 0; i < columns; i++) {
    // if the column has type float
    if (operator[](i).getValueType() == ValueType::flt) {
      // if the value for that column cannot be converted to type float
      if (!stringIsFloat(values[i])) {
        // then return false
        return false;
      }
    }
  };
  // return false
  return true;
};

void Table::insertRowAtIndex(vector<string>& rawValues, size_t rowIndex) {
  // for every column in columns
  for (size_t i = 0; i < columns; i++) {
    // get the reference to the column
    Column& col = operator[](i);
    // insert a value into column at index rowIndex
    col.insertAtRowIndex(rowIndex, rawValues[i]);
  };
  // increment the number of rows by 1
  rows += 1;
};

void Table::sortTableByColumn(string& colHeader) {
  // get the column by its header
  Column col = getColumnByHeader(colHeader);
  // get all the values in that column
  vector<string> rawValues = col.getAllValues();
  // convert all those values to floats
  vector<float> values = convertToFloats(rawValues);

  // using bubble sort, sort the table
  // variable to check if a swap was made in the last pass
  bool swapped;
  // getting the number of values
  int n = values.size();
  // for every element in the list
  for (int i = 0; i < n - 1; i++) {
    // for every element that not peen pushed to the end
    for (int j = 0; j < n - i - 1; j++) {
      // if the first element is bigger than the second element
      if (values[j] > values[j + 1]) {
        // swap the values using std::swap
        swap(values[j], values[j + 1]);
        // swapTableRows() to apply that swap to every row in the table
        swapTablRows(j, j + 1);
        // setting swapped to true
        swapped = true;
      }
    }
    // the there was no swap in the last pass then break the loop prematurely
    // for the list is sorted
    if (!swapped) break;
  };
};

void Table::swapTablRows(size_t rowIndex1, size_t rowIndex2) {
  // for every element in columns
  for (int x = 0; x < columns; x++) {
    // we get the reference to the column
    Column& col = operator[](x);
    // we swap the values at rowIndex1 and rowIndex2
    string tmp1 = col.getValueAt(rowIndex1);
    string tmp2 = col.getValueAt(rowIndex2);
    col.setValueAt(rowIndex1, tmp2);
    col.setValueAt(rowIndex2, tmp1);
  }
};

vector<string> Table::getAllValuesInRow(size_t rowNo) {
  vector<string> rawValues;
  // for every column in columns
  for (int x = 0; x < columns; x++) {
    // we get the value at row index rowNo
    string val = operator[](x).getValueAt(rowNo);
    // we add that value to rawValues
    rawValues.push_back(val);
  };
  return rawValues;
};
int Table::getRowIndexOfFirstOccurrence(string& colHeader, string value) {
  // we get the column by its header
  Column col = getColumnByHeader(colHeader);
  // we get all the values in that column
  vector<string> rawValues = col.getAllValues();
  // for every value in rawValues
  for (int i = 0; i < rawValues.size(); i++) {
    // we compare the string to the string value
    if (cmpstr(rawValues[i], value)) {
      // if they are the same we return the index
      return i;
    }
  }
  // else we return -1, signifying there are no matches
  return -1;
};

int Table::getRowIndexOfFirstOccurrence(string& colHeader, size_t value) {
  // we get the column by its header
  Column col = getColumnByHeader(colHeader);
  // we get all the values in that column
  vector<string> rawValues = col.getAllValues();
  // for every value in rawValues
  for (int i = 0; i < rawValues.size(); i++) {
    // we check if the value at that row index is the same as the value passed
    // in
    float colVal = stoi(rawValues[i]);
    // if they are the same
    if (colVal == value) {
      // we return the index of that element
      return i;
    }
  }
  // else we return -1 to signify no matches were found
  return -1;
};

void Table::flushTable() {
  // used to clear the table from its previos values
  data.clear();
  // sets the table dimensions to 0
  columns = 0;
  rows = 0;
};