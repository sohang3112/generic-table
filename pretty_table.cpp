// TODO: Improve pretty_table() to decrease amount of word wrapping
// Conventions followed for Function Parameters:
// Output parameter, if any, is the first parameter
// The 'table' parameter is immediately followed by 'info' parameter
#include <iostream>
#include <cstdlib>                  // strtod()
#include <cstring>                  // strcpy(), strncpy(), strcat()
#include <cctype>                   // isspace()
#include "pretty_table.hpp"
using namespace std;

bool is_number(String str) {
    char* p;
    strtod(str, &p);            // point p to the last character after number
    // check that number is followed only by whitespace
    for (; *p != '\0'; ++p)
        if (!isspace(*p))
            return false;
    return true;
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
void calc_column_widths(int col_widths[], String table[], const TableInfo& info) {
    static const int MAX_CELL_WIDTH = WINDOW_WIDTH / info.n_cols - 10;
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
    }
}

void display_row(String table[], const TableInfo& info, int row, int col_widths[], AlignFunc get_align) {
    char between_cells[] = {info.pad_char, info.cell_seperator, info.pad_char, '\0'};
    for (int i = 0; ; ++i) {
        bool all_done = true;      //all_done stores whether cell contents of each cell in row has been printed
        String line = "";
        for (int c = 0; c < info.n_cols; ++c) {
            int cell_curr_pos = i * col_widths[c];
            char* line_curr = line + strlen(line);
            if (cell_curr_pos < strlen(get_cell(table,info, row, c))) {
                all_done = false;
                String cell_curr_line;
                strncpy(cell_curr_line, get_cell(table, info, row, c) + cell_curr_pos, col_widths[c]);
                cell_curr_line[col_widths[c]] = '\0'; // since strncpy is not guaranteed to terminate with null character
                copy_aligned_text(line_curr, cell_curr_line, col_widths[c], get_align(cell_curr_line), info.pad_char);
            } else {
                copy_repeat(line_curr, info.pad_char, col_widths[c]);
            }
            strcat(line, between_cells);
        }
        if (all_done)
            break;
        cout << info.cell_seperator << info.pad_char << line << '\n';
    }
}

void display_boundary(const TableInfo& info, int col_widths[]) {
    cout << info.boundary_junction;
    for (int i = 0; i < info.n_cols; ++i) {
        String out;
        copy_repeat(out, info.boundary_fill, col_widths[i]+2 /* +2 because of cell padding */);
        cout << out << info.boundary_junction;
    }
    cout << '\n';
}

// implements word wrapping, limits cell widths of large columns and prints boundaries
void pretty_table(String table[], const TableInfo& info) {
    if (info.n_rows <= 1) {
        cout << "(Empty Table)";
        return;
    }

    int col_widths[MAX_COLS];
    calc_column_widths(col_widths, table, info);

    display_boundary(info, col_widths);             // top boundary
    // first row assumed to be header
    display_row(table, info, 0, col_widths, get_head_align);                  // header
    display_boundary(info, col_widths);             // boundary below header
    // rest of the table
    for (int row = 1; row < info.n_rows; ++row) {
        display_row(table, info, row, col_widths, get_cell_align);
    }
    display_boundary(info, col_widths);             // bottom boundary
}
