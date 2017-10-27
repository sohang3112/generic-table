#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>              //strtod
#include <cctype>
#define MAX_ROWS 10
#define MAX_COLS 5
#define WINDOW_WIDTH 120         //in Windows Shell Prompt
#define WINDOW_HEIGHT 50         //in Windows Shell Prompt
#define STD_CELL_WIDTH 5

using namespace std;
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

class Table {				// contains functions for displaying table in different ways
    std::string table[MAX_ROWS][MAX_COLS], header[MAX_COLS];
    int n_rows, n_cols;
    bool hasHeader;
    typedef Align (*AlignFunc)(const std::string&);

    void setRow(std::string row[], const std::vector<std::string>& vec) {
        for (size_t i = 0; i < n_cols; ++i)
            row[i] = vec[i];
    }
    void displayRowSingleLine(const std::string row[], const size_t colWidths[], AlignFunc getAlign) const {
        cout << cellSeperator << padCharacter;
        for (size_t i = 0; i < n_cols; ++i) {
            cout << alignedText(row[i], colWidths[i], getAlign(row[i]), padCharacter);
            cout << padCharacter << cellSeperator << padCharacter;
        }
        cout << '\n';
    }
    void displayRowMultiLine(const std::string row[], const size_t colWidths[], AlignFunc getAlign) const {
        for (size_t i = 0; ; ++i) {
            bool all_done = true;      //all_done stores whether cell contents of each cell in row has been printed
            string line = "";
            for (size_t c = 0; c < n_cols; ++c) {
                size_t cell_curr_pos = i * colWidths[c];
                string cell_line;
                if (cell_curr_pos < row[c].size()) {       //cell contents have not been completely printed
                    all_done = false;
                    cell_line = row[c].substr(cell_curr_pos, colWidths[c]);
                } else
                    cell_line = "";              //empty line, since cell contents have already been printed
                line += alignedText(cell_line, colWidths[c], getAlign(cell_line)) + padCharacter + cellSeperator + padCharacter;
            }
            if (all_done)
                break;
            cout << cellSeperator << padCharacter << line << '\n';
        }
    }
    void displayBoundary(size_t tableSize, char endChar, char boundaryChar) const {
        cout << endChar << std::string(tableSize, boundaryChar) << endChar << '\n';
    }
    public:
        static const Align headerAlign = Align::Center, normalAlign = Align::Left, numberAlign = Align::Right;
        static const size_t maxWidth = 72;              //default console width

        char padCharacter, cellSeperator, boundaryFill;

        Table(): n_rows(0), n_cols(MAX_COLS), hasHeader(false), padCharacter(' '), cellSeperator('|'), boundaryFill('-') {
            for (size_t i = 0; i < n_cols; ++i)
                header[i] = "";
        }
        void addHeader(const std::vector<std::string>& vec) {
            hasHeader = true;
            setRow(header, vec);
        }
        void addRow(const std::vector<std::string>& vec) {
            setRow(table[n_rows++], vec);
        }
        std::string getCell(size_t i, size_t j) const {
            return table[i][j];
        }
        void setCell(size_t i, size_t j, std::string data) {
            table[i][j] = data;
        }
        void display_cell_width_adjust() const {
            // cell widths of columns are adjustable,
            // set to maximum required by any cell of column
            size_t colWidths[MAX_COLS], tableWidth = -1;  //(tableSize = -1) to handle shortage in size at start and excess at end
            for (size_t col = 0; col < n_cols; ++col) {
                colWidths[col] = header[col].size();
                for (size_t row = 0; row < n_rows; ++row)
                    colWidths[col] = std::max(colWidths[col], table[row][col].size());
                tableWidth += colWidths[col] + 3;         // 3 is added to account for the cell padding and cell boundary
            }

            displayBoundary(tableWidth, boundaryFill, boundaryFill);
            if (hasHeader) {
                displayRowSingleLine(header, colWidths, getHeadAlign);
                displayBoundary(tableWidth, cellSeperator, boundaryFill);
            }
            for (size_t r = 0; r < n_rows; ++r)
                displayRowSingleLine(table[r], colWidths, getCellAlign);
            displayBoundary(tableWidth, boundaryFill, boundaryFill);
        }
        void display_word_wrap_adjust() const {
            //ignoring header, boundaries
            //column width can be at most STD_CELL_WIDTH, shrink if possible

            size_t colWidths[MAX_COLS], tableWidth = -1;

            //calculating column widths (maximum STD_CELL_SIZE, may be less)
            for (size_t col = 0; col < n_cols; ++col) {
				if (header[col].size() > STD_CELL_WIDTH) {
					colWidths[col] = STD_CELL_WIDTH;
				} else {
					for (size_t row = 0; row < n_rows; ++row) {
						if (table[row][col].size() > colWidths[col]) {
							if (table[row][col].size() > STD_CELL_WIDTH) {
								colWidths[col] = STD_CELL_WIDTH;
								break;
							}
							colWidths[col] = table[row][col].size();
						}
					}
				}
                tableWidth += colWidths[col] + 3;
            }

            displayBoundary(tableWidth, boundaryFill, boundaryFill);
            if (hasHeader) {
                displayRowMultiLine(header, colWidths, getHeadAlign);
                displayBoundary(tableWidth, cellSeperator, boundaryFill);
            }
            for (size_t r = 0; r < n_rows; ++r) {
                displayRowMultiLine(table[r], colWidths, getCellAlign);
            }
            displayBoundary(tableWidth, cellSeperator, boundaryFill);
        }
        void display_word_wrap_noadjust() {
            //ignoring header, boundaries
            //all columns have width equal to STD_CELL_WIDTH

            //(r, c) -> (row number, column number)
            for (size_t r = 0; r < n_rows; ++r) {
                for (size_t i = 0; ; ++i) {
                    bool all_done = true;      //all_done stores whether cell contents of each cell in row has been printed
                    string line = "";
                    for (size_t c = 0; c < n_cols; ++c) {
                        size_t cell_curr_pos = i * STD_CELL_WIDTH;
                        string cell_line;
                        if (cell_curr_pos < table[r][c].size()) {       //cell contents have not been completely printed
                            all_done = false;
                            cell_line = table[r][c].substr(cell_curr_pos, STD_CELL_WIDTH);
                        } else
                            cell_line = "";              //empty line, since cell contents have already been printed
                        line += alignedText(cell_line, STD_CELL_WIDTH, normalAlign) + ' ';
                    }
                    if (all_done)
                        break;
                    cout << line << '\n';
                }
            }
        }
    protected:
        static Align getCellAlign(const std::string& cell) {
            return isNumber(cell) ? numberAlign : normalAlign;
        }
        static Align getHeadAlign(const std::string& cell) {
            return headerAlign;
        }
};

int main() {
    Table table;
    table.addHeader({"S. No.", "Product", "Cost Price", "Sale Price", "Discount"});
    table.addRow({"1", "Science kit", "220", "250", "10"});
    table.addRow({"2", "Test tubes (5)", "540", "600", "200"});

    table.padCharacter = ' ';
    table.cellSeperator = '|';
    table.boundaryFill = '-';

	cout << "WORD WRAP ADJUST:\n\n";
    table.display_word_wrap_adjust();
    cout << "\n\nWORD WRAP NO ADJUST, NO BOUNDARY:\n\n";
    table.display_word_wrap_noadjust();
    cout << "\n\nCELL WIDTH ADJUST:\n\n";
    table.display_cell_width_adjust();
}
