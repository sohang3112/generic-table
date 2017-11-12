/* Contains function pretty_table() for pretty printing a table
 * FEATURES:
 * 1. Boundaries, Cell Padding
 * 2. Word Wrapping
 * 3. Resizes columns so that table always fits on the console screen
 * 4. Cell Alignment (Heading cells are centre-aligned, numbers right-aligned
                      and the rest are left-aligned)
 */
#ifndef PRETTY_TABLE_INC
#define PRETTY_TABLE_INC

#include "settings.hpp"

struct TableInfo {
    int n_rows, n_cols;
    char pad_char;                  // for cell padding
    char cell_seperator;            // vertical boundary
    char boundary_fill;             // horizontal boundary
    char boundary_junction;         // junction of horizontal, vertical boundaries
};

void pretty_table(String table[], const TableInfo& info);

#endif // PRETTY_TABLE_INC
