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
 * (c) 2021 Daniel Cooper
 */

#include "csv.hpp"
#include <algorithm>
#include "globals.hpp"

CSVParser::CSVParser(File* csvFile) {
    this->file = csvFile;

    //Performing a pass on the file to obtain header information, line offsets, and
    //number of lines

    int lineNumber = 0;
    int bytesRead = 0;
    bool quotes = false;
    int fieldLength = 0;
    int totalOffset = 0;
    this->lineOffsets[0] = 0;
    bool firstQuote = false;
    std::string building;
    do {
        bytesRead = this->file->read(this->readBuffer, 128);

        for (int i = 0; bytesRead > i; i++) {
            if (firstQuote && this->readBuffer[i] != '"') {
                //Quote not escaped, so exiting quote condition
                quotes = false;
                firstQuote = false;
            }
            if (!quotes && this->readBuffer[i] == '\n') {
                //Line delimiter

                if (lineNumber == 0) {
                    header.push_back(building);
                    building = "";
                }

                lineNumber++;
                this->lineOffsets[lineNumber] = totalOffset + i + 1;
                continue;
            } else if (!quotes && fieldLength == 0 && readBuffer[i] == '"') {
                quotes = true;
                continue;
            } else if (quotes && this->readBuffer[i] == '"') {
                //could either end quotes or be a part of the string

                //if there are two quotes in a row, then it is an escape sequence
                if (firstQuote) {
                    //there already was a previous, so this is an escape sequence
                    
                    if (lineNumber == 0) {
                        building += '"';
                    }

                    firstQuote = false;
                } else {
                    firstQuote = true;
                }

                continue;
            } else if (!quotes && this->readBuffer[i] == ',') {
                //field separation
                if (lineNumber == 0) {
                    header.push_back(building);
                    building = "";
                }

                continue;
            }

            //would normally build the field string, but this part of the code only does it if the
            //first line for header info

            if (lineNumber == 0) {
                building += this->readBuffer[i];
            }
        }

        totalOffset += bytesRead;
    } while (bytesRead >= 128);

    this->fileLength = totalOffset;
    this->numRows = lineNumber + 1;
}

CSVParser::~CSVParser() {
    //By default, will not automatically close the file.
    //this->file->close();
}

int CSVParser::getColIndex(const char* headerName) {
    int index = 0;
    std::string str = std::string(headerName);

    for (auto it = this->header.begin(); it != this->header.end(); ++it) {
        if (*it == str) {
            return index;
        }

        index++;
    }

    return -1;
}

int CSVParser::getFieldStartOffset(int row, int col) {
    int position = this->lineOffsets[row];

    if (col == 0) {
        return position;
    }

    this->file->seek(position, SEEK_SET);

    int bytesRead = 0;
    bool quotes = false;
    int fieldLength = 0;
    bool firstQuote = false;
    int fieldIndex = 0;
    do {
        bytesRead = this->file->read(this->readBuffer, 128);

        for (int i = 0; bytesRead > i; i++) {
            if (firstQuote && this->readBuffer[i] != '"') {
                //Quote not escaped, so exiting quote condition
                quotes = false;
                firstQuote = false;
            }
            if (!quotes && this->readBuffer[i] == '\n') {
                //Line delimiter
                //If this isn't the requested field, then it doesn't exist on this line
                return -1;
            } else if (!quotes && fieldLength == 0 && readBuffer[i] == '"') {
                quotes = true;
                continue;
            } else if (quotes && this->readBuffer[i] == '"') {
                //could either end quotes or be a part of the string

                //if there are two quotes in a row, then it is an escape sequence
                if (firstQuote) {
                    //there already was a previous, so this is an escape sequence

                    firstQuote = false;
                } else {
                    firstQuote = true;
                }

                continue;
            } else if (!quotes && this->readBuffer[i] == ',') {
                //field separation
                fieldIndex++;

                if (fieldIndex == col) {
                    return position + i + 1;
                }

                continue;
            }
        }
        position += bytesRead;
    } while (bytesRead >= 128);

    return -1;
}

std::string CSVParser::getString(int row, int col) {
    if (row == -1 || col == -1) {
        return "";
    }

    int position = this->getFieldStartOffset(row, col);

    if (position == -1) {
        return "";
    }

    this->file->seek(position, SEEK_SET);

    int bytesRead = 0;
    bool quotes = false;
    int fieldLength = 0;
    bool firstQuote = false;
    std::string building;
    do {
        bytesRead = this->file->read(this->readBuffer, 128);

        for (int i = 0; bytesRead > i; i++) {
            if (firstQuote && this->readBuffer[i] != '"') {
                //Quote not escaped, so exiting quote condition
                quotes = false;
                firstQuote = false;
            }
            if (!quotes && this->readBuffer[i] == '\n') {
                //Line delimiter
                //If this isn't the requested field, then it doesn't exist on this line
                return building;
            } else if (!quotes && fieldLength == 0 && readBuffer[i] == '"') {
                quotes = true;
                continue;
            } else if (quotes && this->readBuffer[i] == '"') {
                //could either end quotes or be a part of the string

                //if there are two quotes in a row, then it is an escape sequence
                if (firstQuote) {
                    //there already was a previous, so this is an escape sequence
                    
                    building += '"';

                    firstQuote = false;
                } else {
                    firstQuote = true;
                }

                continue;
            } else if (!quotes && this->readBuffer[i] == ',') {
                //field separation
                return building;

                continue;
            }

            building += this->readBuffer[i];
        }
    } while (bytesRead >= 128);

    return building;
}

int CSVParser::addRow(int numFields) {
    //Currently can only add a row to the end
    
    if (this->lineOffsets[this->numRows - 1] != this->fileLength - 1) {
        //There isn't a line delimiter already
        this->file->seek(0, SEEK_END);
        this->file->write("\n", 1);
    }

    //TODO: Add a row

    return this->numRows++;
}

void CSVParser::deleteRow(int row) {
    //Unimplemented for now

}

std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}

void CSVParser::writeString(int row, int col, std::string str) {
    //Finding existing entry's length so that the rest of the file can be moved
    int oldLength = this->getString(row, col).length();

    int endSeeker = -1;
    int frontSeeker = this->getFieldStartOffset(row, col);
    int writeStart = frontSeeker;

    if (frontSeeker == -1) {
        //Field does not yet exist
        return;
    }

    if (str.find("\"") != std::string::npos || str.find("\n") != std::string::npos || str.find(",") != std::string::npos) {
        //Must surround in quotes and replace all existing quotes with the escape sequence
        str = replaceAll(str, "\"", "\"\"");

        //Now wrapping in quotes
        str = "\"" + str + "\"";
    }

    if (oldLength > str.length()) {
        //Have to shift the contents towards the start
        //Must start from the end of the file
        int amtToShift = this->file->size() - frontSeeker - oldLength;
        int delta = oldLength - str.length();
        frontSeeker += str.length();

        for (int i = 0; amtToShift > i; i++) {
            //Getting the value from the end
            this->file->seek(frontSeeker + delta, SEEK_SET);

            char c;
            this->file->read(&c, 1);

            this->file->seek(frontSeeker, SEEK_SET);
            this->file->write(&c, 1);
            frontSeeker++;
        }

        this->file->truncate(this->file->size() - delta);
    } else if (oldLength < str.length()) {
        //Have to shift the contents away from the start
        //Must start from the front of the file

        int delta = str.length() - oldLength;
        int amtToShift = this->file->size() - frontSeeker - delta;
        this->file->seek(0, SEEK_END);

        this->file->truncate(this->file->size() + delta);
        for (int i = 0; amtToShift > i; i++) {
            //Getting the value from the end
            this->file->seek(endSeeker - delta, SEEK_END);

            char c;
            this->file->read(&c, 1);

            this->file->seek(endSeeker, SEEK_END);
            this->file->write(&c, 1);
            endSeeker--;
        }
    }

    //Writing the value
    this->file->seek(writeStart);
    this->file->write(str.c_str(), str.length());
    this->file->sync();

    int delta = str.length() - oldLength;
    for (auto it = this->lineOffsets.begin(); it != this->lineOffsets.end(); ++it) {
        if (it->second > writeStart) {
            //Must update the offset
            it->second += delta;
        }
    }

    this->fileLength = this->file->size();
}