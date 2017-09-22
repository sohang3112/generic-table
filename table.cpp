#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>              //strtod
#include <cctype>
#include <cmath>
#define WINDOW_WIDTH 120         //in Windows Shell Prompt
#define WINDOW_HEIGHT 50         //in Windows Shell Prompt
#define STD_CELL_WIDTH 10

using namespace std;
typedef std::string String;
enum Align {Left, Right, Center};

string withPadding(const string& str, size_t left, size_t right, char padCharacter=' ') {
    return std::string(left, padCharacter) + str + std::string(right, padCharacter);
}

string alignedText(const string& str, int width, Align alignment=Align::Left, char padCharacter=' ') {
    size_t remainingWidth = width - str.size();
    if (remainingWidth <= 0)
        return str;
    else if (alignment == Align::Left)
        return withPadding(str, 0, remainingWidth, padCharacter);
    else if (alignment == Align::Right)
        return withPadding(str, remainingWidth, 0, padCharacter);
    else if (alignment == Align::Center)
        return withPadding(str, remainingWidth / 2, (remainingWidth + 1) / 2, padCharacter);
}

bool isNumber(const string& str) {
    char *p;
    strtod(str.c_str(), &p);
    for (; *p != '\0'; ++p)
        if (!isspace(*p))
            return false;
    return true;
}

class Table {
    String table[5][5], header[5];
    int n_rows, n_cols;
    bool hasHeader;

    void setRow(String row[], const std::vector<String>& vec) {
        for (size_t i = 0; i < 5; ++i)
            row[i] = vec[i];
    }
    void displayRow(const String row[], const size_t colSizes[], Align (*getAlign)(const String&)) const {
        cout << cellSeperator << padCharacter;
        for (size_t i = 0; i < 5; ++i) {
            cout << alignedText(row[i], colSizes[i], getAlign(row[i]), padCharacter);
            cout << padCharacter << cellSeperator << padCharacter;
        }
        cout << '\n';
    }
    void displayBoundary(size_t tableSize, char endChar, char boundaryChar) const {
        cout << endChar << std::string(tableSize, boundaryChar) << endChar << '\n';
    }
    public:
        static const Align headerAlign = Align::Center, normalAlign = Align::Left, numberAlign = Align::Right;
        static const size_t maxWidth = 72;              //default console width

        char padCharacter, cellSeperator, boundaryFill;

        Table(): n_rows(0), hasHeader(false), padCharacter(' '), cellSeperator('|'), boundaryFill('-') {
            for (size_t i = 0; i < 5; ++i)
                header[i] = "";
        }
        void addHeader(const std::vector<String>& vec) {
            hasHeader = true;
            setRow(header, vec);
        }
        void addRow(const std::vector<String>& vec) {
            setRow(table[n_rows++], vec);
        }
        String getCell(size_t i, size_t j) const {
            return table[i][j];
        }
        void setCell(size_t i, size_t j, String data) {
            table[i][j] = data;
        }
        void display_cell_width_adjust() const {
            size_t colWidths[5], tableWidth = -1;  //(tableSize = -1) to handle shortage in size at start and excess at end
            for (size_t i = 0; i < 5; ++i) {
                colWidths[i] = header[i].size();
                for (size_t row = 0; row < n_rows; ++row)
                    colWidths[i] = std::max(colWidths[i], table[row][i].size());
                tableWidth += colWidths[i] + 3;
            }
            displayBoundary(tableWidth, boundaryFill, boundaryFill);

            if (hasHeader) {
                displayRow(header, colWidths, getHeadAlign);
                displayBoundary(tableWidth, cellSeperator, boundaryFill);
            }
            for (size_t r = 0; r < n_rows; ++r)
                displayRow(table[r], colWidths, getCellAlign);
            displayBoundary(tableWidth, boundaryFill, boundaryFill);
        }
        void display_word_wrap() const {
            size_t colWidths[5], tableWidth = -1;

        }
    protected:
        static Align getCellAlign(const String& cell) {
            return isNumber(cell) ? numberAlign : normalAlign;
        }
        static Align getHeadAlign(const String& cell) {
            return headerAlign;
        }
};

int main() {
    Table table;
    table.addHeader({"S. No.", "Product", "Cost Price", "Sale Price", "Discount"});
    table.addRow({"1", "Science kit", "220", "250", "10"});
    table.addRow({"2", "Test tubes (5)", "540", "600", "20"});

    table.padCharacter = '_';
    table.cellSeperator = '^';
    table.boundaryFill = '*';
    table.display_cell_width_adjust();
}
