/*
 * CSV Parser Class
 *
 * A general-purpose csv parsing utility. Supports row-column indexing or
 * row-header, where header is the name of the header. Header must be specified
 * on the first line of the file, or it will not be available to use.
 *
 * Supports reading and writing to CSV files. Does not cache CSV file contents
 * in memory to allow for reading large CSV files without running into memory
 * concerns and to deliver consistent performance across all sizes.
 *
 * NOTE: Row 0 is always the header if there is one, or the first line if there
 * is no header.
 *
 * (c) 2021 Daniel Cooper
 */

#ifndef CSV_PARSER_INCLUDED
#define CSV_PARSER_INCLUDED

#include "mbed.h"
#include <string>
#include <list>
#include <map>

class CSVParser {
    private:

    int getColIndex(const char* headerName);

    int getFieldStartOffset(int row, int col);

    File* file;
    std::list<std::string> header;
    std::map<int, int> lineOffsets; //For improving seeking performance
    char readBuffer[128];
    int fileLength;
    int numRows;

    public:

    CSVParser(File* csvFile);

    ~CSVParser();

    std::string getString(int row, int col);

    inline std::string getString(int row, const char* col) { return this->getString(row, this->getColIndex(col)); }

    inline int getInt(int row, int col) { return std::stoi(this->getString(row, col)); }

    inline int getInt(int row, const char* col) { return this->getInt(row, this->getColIndex(col)); }

    inline float getFloat(int row, int col) { return std::stof(this->getString(row, col)); }

    inline float getFloat(int row, const char* col) { return this->getFloat(row, this->getColIndex(col)); }

    //Returns the index of the new row
    int addRow(int numFields);

    void deleteRow(int row);

    void writeString(int row, int col, std::string str);

    inline void writeString(int row, const char* col, std::string str) { this->writeString(row, this->getColIndex(col), str); }

    inline void writeInt(int row, int col, int value) { this->writeString(row, col, std::to_string(value)); }

    inline void writeInt(int row, const char* col, int value) { this->writeInt(row, this->getColIndex(col), value); }

    inline void writeFloat(int row, int col, float value) { this->writeString(row, col, std::to_string(value)); }

    inline void writeFloat(int row, const char* col, float value) { this->writeFloat(row, this->getColIndex(col), value); }
};

#endif //CSV_PARSER_INCLUDED