/* Manage table through file handling
 * File Format: <n_cols> <cell> <CELL_DELIM> <cell> <CELL_DELIM> <cell> <CELL_DELIM> ... <cell> <CELL_DELIM>
 * First <n_cols> (integer) is read, then first <n_cols> cells are read to form header (title bar).
 * The remaining records are read by reading <n_cols> cells at a time.
 * (For reading a cell, getline() may be used)
 */
 
#ifndef TABLE_INC
#define TABLE_INC

#include "settings.hpp"            //CELL_END

//Example interface (parameters may be added later)
class Table {
    public:
        void add_record();
        void modify_record();
        void delete_record();
        void find_record();
        void display();
};

#endif        //TABLE_INC
