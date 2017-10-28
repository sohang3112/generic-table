/* Reimplements table.cpp using only C Strings
 * Contains function display_table() for pretty-printing a table,
 * i.e, with boundaries and word-wrapping.
 * Helper functions copy_repeat(), copy_padded_text(), copy_aligned_text()
 * each takes an 'out' parameter as the first parameter.
 * This is a string to which the result is copied.
 */
// Conventions followed for Function Parameters:
// Output parameter, if any, is the first parameter
// The 'table' parameter is immediately followed by 'info' parameter
#include <iostream>
#include <cstdlib>                  // strtod()
#include <cstring>                  // strcpy(), strncpy(), strcat()
#include <cctype>                   // isspace()
#define MAX_COLS 5

#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1
#define ALIGN_CENTER 2
using namespace std;
typedef char String[81];
typedef int Align;
typedef Align (*AlignFunc)(String);

struct TableInfo {
    int n_rows, n_cols;
    char pad_char;              // for cell padding (and also cells which do not occupy full column width)
    char cell_seperator;        
    char boundary_fill;         // in horizontal boundary
};

bool is_number(String str) {
    char* p;
    strtod(str, &p);            // point p to the last character after number
    return *p != '\0';          // str must contain only a number, nothing else
}

Align get_cell_align(String cell) {
    return is_number(cell) ? ALIGN_RIGHT : ALIGN_LEFT;
}

Align get_head_align(String cell) {
    return ALIGN_CENTER;
}

char* get_cell(String table[], const TableInfo& info, int i, int j) {
    return table[i * info.n_cols + j];                   // row-major
}

void copy_repeat(String out, char ch, int n_repeat) {
    for (int i = 0; i < n_repeat; ++i)
        out[i] = ch;
    out[n_repeat] = '\0';
}

void copy_padded_text(String out, String str, int left, int right, char pad_char=' ') {
    copy_repeat(out, pad_char, left);
    strcat(out, str);
    copy_repeat(out + left + strlen(str), pad_char, right);
}

// Aligns str in given width according to provided alignment
// Note: If str is larger than width, then it is written to out as it is.
void copy_aligned_text(String out, String str, int width, Align align, char pad_char=' ') {
    int remaining_width = width - strlen(str);;
    if (remaining_width <= 0)
        strcpy(out, str);
    else if (align == ALIGN_LEFT)
        copy_padded_text(out, str, 0, remaining_width, pad_char);
    else if (align == ALIGN_RIGHT)
        copy_padded_text(out, str, remaining_width, 0, pad_char);
    else if (align == ALIGN_CENTER)
        copy_padded_text(out, str, remaining_width / 2, (remaining_width + 1) / 2, pad_char);
}

// calculates the widths of each column in table
// @return: Total table width (including cell padding and cell boundary)
int calc_column_widths(int col_widths[], String table[], const TableInfo& info) {
    static const int MAX_CELL_WIDTH = 7;
    int table_width = 0;
    for (size_t col = 0; col < info.n_cols; ++col) {
        col_widths[col] = 0;
        for (size_t row = 0; row < info.n_rows; ++row) {
            int len = strlen(get_cell(table, info, row, col));
            if (len > col_widths[col]) {
                if (len > MAX_CELL_WIDTH) {
                    col_widths[col] = MAX_CELL_WIDTH;
                    break;
                }
                col_widths[col] = len;
            }
        }
        table_width += col_widths[col] + 3;             // +3 for cell padding & cell boundary
    }
    return table_width - 1;                 // -1 to remove extra width
}

void display_row(String table[], const TableInfo& info, int row, int col_widths[], AlignFunc get_align) {
    char between_cells[] = {info.pad_char, info.cell_seperator, info.pad_char, '\0'};
    for (int i = 0; ; ++i) {
        bool all_done = true;      //all_done stores whether cell contents of each cell in row has been printed
        String line = "";
        //int line_length = 0;
        for (int c = 0; c < info.n_cols; ++c) {
            int cell_curr_pos = i * col_widths[c];
            char* line_curr = line + strlen(line);
            if (cell_curr_pos < strlen(get_cell(table,info, row, c))) {
                all_done = false;
                String cell_curr_line;
                // extract number of characters (= column width) from cell
                strncpy(cell_curr_line, get_cell(table, info, row, c) + cell_curr_pos, col_widths[c]);
                cell_curr_line[col_widths[c]] = '\0'; // since strncpy is not guaranteed to terminate with null character
                copy_aligned_text(line_curr, cell_curr_line, col_widths[c], get_align(cell_curr_line), info.pad_char);
            } else {
                copy_repeat(line_curr, info.pad_char, col_widths[c]);       // empty cell, pad with pad_char
            }
            strcat(line, between_cells);
        }
        if (all_done)
            break;
        cout << info.cell_seperator << info.pad_char << line << '\n';
    }
}

void display_boundary(int width, char end_char, char fill_char) {
    String out;
    copy_repeat(out, fill_char, width);
    cout << end_char << out << end_char << '\n';
}

// implements word wrapping, limits cell widths of large columns
void display_table(String table[], const TableInfo& info) {
    int col_widths[MAX_COLS];
    if (info.n_rows <= 0) {
        cout << "(Empty Table)";
        return;
    }
    int table_width = calc_column_widths(col_widths, table, info);
    display_boundary(table_width, info.boundary_fill, info.boundary_fill);    // top boundary
    // first row assumed to be header
    display_row(table, info, 0, col_widths, get_head_align);                  // header
    display_boundary(table_width, info.cell_seperator, info.boundary_fill);   // boundary below header
    // rest of the table
    for (int row = 1; row < info.n_rows; ++row) {
        display_row(table, info, row, col_widths, get_cell_align);
    }
    display_boundary(table_width, info.boundary_fill, info.boundary_fill);    // bottom boundary
}

int main() {
    String table[] = {
        "S. No.", "Product", "Cost Price", "Sale Price", "Discount",
        "1", "Science Kit", "220", "250", "10",
        "2", "Test Tubes (5)", "540", "600", "200"
    };
    TableInfo info;
    info.n_rows = 3;
    info.n_cols = 5;
    info.pad_char = ' ';
    info.cell_seperator = '|';
    info.boundary_fill = '-';
    display_table(table, info);
}
