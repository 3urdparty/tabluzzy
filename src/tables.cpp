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

string Table::getValueAt(size_t colIndex, size_t rowNo) {
  // gets the column at colIndex
  Column col = operator[](colIndex);
  // gets the value at row number in the column and returns it
  return col[rowNo];
};

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

void Table::deleteRow(int& rowIndex) {
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
  //
  vector<string> csv;
  string c = to_string(columns);
  string r = to_string(rows);

  csv.push_back(c);
  csv.push_back(r);

  vector<string> headers = getAllColumnHeaders();
  string entry = join(headers, ",");
  csv.push_back(entry);
  entry = "";

  vector<string> datatypes;
  for (int x = 0; x < columns; x++) {
    string dttype =
        (data[x].getValueType() == ValueType::flt) ? "number" : "string";
    datatypes.push_back(dttype);
  }
  entry = join(datatypes, ",");
  csv.push_back(entry);

  vector<string> values;
  for (int y = 0; y < rows; y++) {
    for (int x = 0; x < columns; x++) {
      values.push_back(data[x].getValueAt(y));
    }
    entry = join(values, ",");
    csv.push_back(entry);
  };
  return csv;
}

vector<string> Table::to_html() {
  vector<string> tags;

  tags.push_back(R"(<!DOCTYPE html>)");
  tags.push_back(R"(<html>)");

  tags.push_back(R"(<head>)");
  tags.push_back(R"(  <meta charset="UTF-8">)");

  tags.push_back(R"(  <title>Table</title>)");
  tags.push_back(
      R"(  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/meyer-reset/2.0/reset.min.css">)");
  tags.push_back(R"(  <link rel="stylesheet" href="css/style.css">)");
  tags.push_back(R"(</head>)");

  tags.push_back(R"(<body>)");
  tags.push_back(R"(  <section>)");
  tags.push_back(R"(    <h1>Table</h1>)");
  tags.push_back(R"(    <div class="tbl-header">)");
  tags.push_back(R"(      <table cellpadding="0" cellspacing="0" border="0">)");
  tags.push_back(R"(        <thead>)");
  tags.push_back(R"(          <tr>)");

  string tag;
  for (int x = 0; x < columns; x++) {
    string header = operator[](x).getHeader();
    tag = "<th>" + header + "</th>";
    tags.push_back(tag);
  };

  tags.push_back(R"(          </tr>)");
  tags.push_back(R"(        </thead>)");
  tags.push_back(R"(      </table>)");
  tags.push_back(R"(    </div>)");
  tags.push_back(R"(    <div class="tbl-content">)");
  tags.push_back(R"(      <table cellpadding="0" cellspacing="0" border="0">)");
  tags.push_back(R"(        <tbody>)");

  for (int y = 0; y < rows; y++) {
    tags.push_back(R"(        <tr>)");
    for (int x = 0; x < columns; x++) {
      string value = operator[](x).getValueAt(y);
      tag = "<td>" + value + "</td>";
      tags.push_back(tag);
    }
    tags.push_back(R"(        </tr>)");
  };

  tags.push_back(R"(        </tbody>)");
  tags.push_back(R"(      </table>)");
  tags.push_back(R"(    </div>)");
  tags.push_back(R"(  </section>)");
  tags.push_back(R"(  <script src="js/index.js"></script>)");
  tags.push_back(R"(</body>)");
  tags.push_back(R"(</html>)");
  return tags;
};

vector<string> Table::getAllColumnHeaders() {
  vector<string> headers;
  for (int x = 0; x <= columns; x++) {
    string header = operator[](x).getHeader();
    headers.push_back(header);
  }
  return headers;
};

float Table::getMinimumValue() {
  vector<float> values;
  for (int x = 0; x < columns; x++) {
    Column col = operator[](x);
    if (col.getValueType() == ValueType::str) continue;
    values.push_back(col.getMinimumValue());
  }
  return getMin(values);
};

float Table::getMaxiumValue() {
  vector<float> values;
  for (int x = 0; x < columns; x++) {
    Column col = operator[](x);
    if (col.getValueType() == ValueType::str) continue;
    values.push_back(col.getMaximumValue());
  }
  return getMax(values);
};

vector<string> Table::getAllValues() {
  vector<string> rawValues;
  for (int x = 0; x < columns; x++) {
    Column col = operator[](x);
    if (col.getValueType() == ValueType::str) continue;
    vector<string> rawColValues = col.getAllValues();
    for (string rawValue : rawColValues) {
      rawValues.push_back(rawValue);
    }
  };
  return rawValues;
}

float Table::getMedian() {
  vector<string> rawValues = getAllValues();
  vector<float> values = convertToFloats(rawValues);
  return calculateMedian(values);
};
float Table::getMean() {
  vector<string> rawValues = getAllValues();
  vector<float> values = convertToFloats(rawValues);
  return calculateMean(values);
};
float Table::getVariance() {
  vector<string> rawValues = getAllValues();
  vector<float> values = convertToFloats(rawValues);
  return calculateVariance(values);
};
float Table::getStdDeviation() {
  vector<string> rawValues = getAllValues();
  vector<float> values = convertToFloats(rawValues);
  return calculateStandardDeviation(values);
};
void Table::displayReport() {
  for (int x = 0; x < columns; x++) {
    Column col = operator[](x);
    if (col.getValueType() == ValueType::str) continue;
    float min = col.getMinimumValue();
    float max = col.getMaximumValue();
    float median = col.getMedian();
    float mean = col.getMean();
    float variance = col.getVariance();
    float stdv = col.getStdDeviation();
    auto [a, b] = col.getRegression();

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

int Table::getNumberOfRows() { return rows; };
int Table::getNumberOfColumns() { return columns; };
void Table::displayVerticalHistogram(){

};
void Table::displayHorizontalHistogram() {}

void Table::deleteColumn(string& colHeader){

};

bool Table::canBeInsertedIntoTable(vector<string> values) {
  for (int i = 0; i < columns; i++) {
    if (operator[](i).getValueType() == ValueType::flt) {
      if (!stringIsFloat(values[i])) {
        return false;
      }
    }
  };
  return true;
};

void Table::insertRowAtIndex(vector<string>& rawValues, size_t rowIndex) {
  for (size_t i = 0; i < columns; i++) {
    Column& col = operator[](i);
    col.insertAtRowIndex(rowIndex, rawValues[i]);
  };
  rows += 1;
};
void Table::replaceEveryInstance(string& valToBeReplaced, string& valToReplace){

};
void Table::replaceEveryInstanceInColumn(string& colHeader,
                                         string& valToBeReplaced,
                                         string& valToReplace){

};
void Table::sortTableByColumn(string& colHeader) {
  Column col = getColumnByHeader(colHeader);
  vector<string> rawValues = col.getAllValues();
  vector<float> values = convertToFloats(rawValues);

  bool swapped;
  int n = values.size();
  for (int i = 0; i < n - 1; i++) {
    for (int j = 0; j < n - i - 1; j++) {
      if (values[j] > values[j + 1]) {
        swap(values[j], values[j + 1]);
        swapTablRows(j, j + 1);
        swapped = true;
      }
    }
    if (!swapped) break;
  };
};

void Table::swapTablRows(size_t rowIndex1, size_t rowIndex2) {
  for (int x = 0; x < columns; x++) {
    Column& col = operator[](x);
    string tmp1 = col.getValueAt(rowIndex1);
    string tmp2 = col.getValueAt(rowIndex2);
    col.setValueAt(rowIndex1, tmp2);
    col.setValueAt(rowIndex2, tmp1);
  }
};

vector<string> Table::getAllValuesInRow(size_t rowNo) {
  vector<string> rawValues;
  for (int x = 0; x < columns; x++) {
    string val = operator[](x).getValueAt(rowNo);
    rawValues.push_back(val);
  };
  return rawValues;
};
int Table::getRowIndexOfFirstOccurrence(string& colHeader, string value) {
  Column col = getColumnByHeader(colHeader);
  vector<string> rawValues = col.getAllValues();
  for (int i = 0; i < rawValues.size(); i++) {
    if (cmpstr(rawValues[i], value)) {
      return i;
    }
  }
  return -1;
};

int Table::getRowIndexOfFirstOccurrence(string& colHeader, size_t value) {
  Column col = getColumnByHeader(colHeader);
  vector<string> rawValues = col.getAllValues();
  for (int i = 0; i < rawValues.size(); i++) {
    float colVal = stoi(rawValues[i]);
    if (colVal == value) {
      return i;
    }
  }
  return -1;
};

void Table::flushTable() {
  data.clear();
  columns = 0;
  rows = 0;
};