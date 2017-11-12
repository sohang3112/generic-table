#include <iostream>
#include <cstring>
#include <sys/stat.h>					// struct stat, stat()

#include "DataTable.hpp"

#define CHECK_TABLE_VALID(ret) \
if (!header) { cerr << ERR_MSG << "Header is NULL\n"; } \
if (!isValid()) { cerr << ERR_MSG << "Invalid Table\n"; return ret; } \
file.clear();          // clear eof flag
#define THROW(err) { printStatus(status = err); reset(); } \

bool DataTable::allocHeader() {
    String* header = new String[table_info.n_cols];
    if (!header) {
        cerr << ERR_MSG << "Out of Memory!!!\n";
        status = ERR_TABLE_INVALID;
    }
    return header != nullptr;
}

void input(String& str) {
    cin.getline(str, MAX_STR_LEN);
}

void printStatus(Status status) {
	if (status == SUCCESS) {
		cout << "Success\n";
		return;
	}
	cerr << ERR_MSG;
	if (status == ERR_FILE_NOT_CREATED) {
		cerr << "File could not be created";
	} else if (status == ERR_FILE_NOT_OPENED) {
		cerr << "File could not be opened";
	} else if (status == ERR_FILE_EXISTS) {
		cerr << "File already exists";
	} else if (status == ERR_FILE_NOT_EXISTS) {
		cerr << "File does not exist";
	} else if (status == ERR_TABLE_INVALID) {
		cerr << "Table is invalid";
	}
}

bool fileExists(String fname) {
	struct stat buf;
	return stat(fname, &buf) != -1;
}

int createFile(String fname) {
	if (fileExists(fname)) {
		return ERR_FILE_EXISTS;
	} else if (!ofstream(fname, ios::app)) {
		return ERR_FILE_NOT_CREATED;
	}
	return SUCCESS;
}

DataTable::DataTable(): header(nullptr) {
    status = ERR_FILE_NOT_CREATED;
	reset();
}

DataTable::~DataTable() {
    delete[] header;
}

void DataTable::reset() {
	table_info.n_rows = table_info.n_cols = 0;
	table_info.pad_char = ' ';
	table_info.boundary_fill = '-';
	table_info.cell_seperator = '|';
	table_info.boundary_junction = '+';

	delete[] header;
	header = nullptr;
}

void DataTable::inputFileName(String message, String& fname) {
	cout << message << PROMPT;
	input(fname);
	strcat(fname, TABLE_EXT);
}

void DataTable::openFile(String fname, int err_code) {
	file.open(fname, ios::in | ios::out  | ios::ate);
	if (!file) {
		status = err_code;
	} else {
		status = SUCCESS;
	}
}

int DataTable::getCell(String cell) {
    file.getline(cell, MAX_STR_LEN, CELL_DELIM);
    if (file.eof())
        return EOF;
    return SUCCESS;
}

bool DataTable::isValid() const {
	return status == SUCCESS;
}

int DataTable::getStatus() const {
	return status;
}

int DataTable::getNumCols() const {
	return table_info.n_cols;
}

void DataTable::setNumCols(int cols) {
	table_info.n_cols = cols;
	file << table_info.n_cols; 			// assuming file pointer at start of file
}

void DataTable::create(String fname) {
	if ((status = createFile(fname)) != SUCCESS) {
		 return;
	}
	openFile(fname, ERR_FILE_NOT_CREATED);
}

void DataTable::open(String fname) {
    /*
	openFile(fname, ERR_FILE_NOT_OPENED);
	if (!isValid())
        return;
    file.seekg(0, ios::beg);                    // cursor is at end due to ios::ate mode
    table_info.n_cols = file.get() - '0';             // extrqct (single-digit) no. of columns at start of file
    if (table_info.n_cols <= 0) {
        file.close();
        status = ERR_TABLE_INVALID;
        return;
    }

    // count rows
    int i = 0;
    table_info.n_rows = 0;
    while (true) {
        String cell;
        if (getCell(cell) == EOF)
            break;
        if (i++ % table_info.n_cols == 0)
            ++table_info.n_rows;
    }
    if (i % table_info.n_cols != 0) {              // incomplete row
        file.close();
        status = ERR_TABLE_INVALID;
    }
    */
    openFile(fname, ERR_FILE_NOT_OPENED);
	if (!isValid())
        return;

	// load n_cols
    file.seekg(0, ios::beg);                    // cursor is at end due to ios::ate mode
    table_info.n_cols = file.get() - '0';             // extract (single-digit) no. of columns at start of file
    if (table_info.n_cols <= 0) {
        close();
        status = ERR_TABLE_INVALID;
        return;
    }

    // load header
	if (!allocHeader())
		return;
	for (int i = 0; i < table_info.n_cols; ++i) {
		if (getCell(header[i]) == EOF) {
			close();
			status = ERR_TABLE_INVALID;
			return;
		}
	}

	// check for incomplete row
	int i = 0;
	table_info.n_rows = 1; 		// first row is header
    while (true) {
        String cell;
        if (getCell(cell) == EOF)
            break;
        if (i++ % table_info.n_cols == 0)
            ++table_info.n_rows;
    }
    if (i % table_info.n_cols != 0) {              // incomplete row
        file.close();
        status = ERR_TABLE_INVALID;
    }
}

void DataTable::close() {
	file.close();
	status = ERR_FILE_NOT_CREATED;
	reset();
}

void DataTable::inputRecord() {
    CHECK_TABLE_VALID( )

	if (!isValid()) {
		cerr << ERR_MSG << "Invalid Table";
		return;
	}
	file.seekg(0, ios::end);
	for (int i = 0; i < getNumCols(); ++i) {
		String cell;
		cout << "Enter value of field '" << header[i] << "'" << PROMPT;
		input(cell);
        file << cell << CELL_DELIM;
	}
	++table_info.n_rows;
}

void DataTable::inputHeader() {
    CHECK_TABLE_VALID( )

    if (!isValid()) {
		cerr << ERR_MSG << "Invalid Table";
		return;
	}
	file.seekg(TABLE_START, ios::beg);
	header = new String[table_info.n_cols];
	for (int i = 0; i < getNumCols(); ++i) {
		cout << "Enter header of column " << i+1 << PROMPT;
		input(header[i]);
        file << header[i] << CELL_DELIM;
	}
	++table_info.n_rows;
}

void DataTable::addHeader(String head[]) {      // assumes empty table
    CHECK_TABLE_VALID( )

    file.seekg(TABLE_START, ios::beg);
    for (int i = 0; i < table_info.n_cols; ++i) {
        file << head[i] << CELL_DELIM;
    }
}

bool DataTable::searchRecord(String what, int field) {
    CHECK_TABLE_VALID(-1)

    int i = 0, recordStartPos;
    file.seekg(TABLE_START, ios::beg);
    while (true) {
        String cell;
        if (getCell(cell) == EOF)
            break;
        if (i % table_info.n_cols == 0)
            recordStartPos = file.tellg();
        if (i % table_info.n_cols == field && strcmp(cell, what) == 0) {   // found
            file.seekg(recordStartPos, ios::beg);
            return true;
        }
        ++i;
    }
    return false;       // not found
}

void DataTable::printCurrentRecord() {
    CHECK_TABLE_VALID( )

    int i = 0;
    while (true) {
        String cell;
        if (getCell(cell) == EOF) {
            THROW(ERR_TABLE_INVALID)
            return;
        }
        cout << "\nValue of field '" << header[i] << "': " << cell;
        if (++i == table_info.n_cols)
            break;
    }
}

void DataTable::display() {
    CHECK_TABLE_VALID( )

    int n_cells = table_info.n_rows * table_info.n_cols;
    cout << "Rows: " << table_info.n_rows << " Cols: " << table_info.n_cols << " Cells: " << n_cells << endl;
    String* table = new String[n_cells];
    if (!table) {
        cerr << ERR_MSG << "Memory Problem (Buffer Overflow)";
        return;
    }

    int i = 0;
    file.seekg(TABLE_START, ios::beg);
    cout << "\nPosition: " << file.tellg();
    while (getCell(table[i]) != EOF) {
        cout << "\nPostition: " << file.tellg() << " table[" << i << "] = " << table[i];
        if (++i > n_cells) {
            cerr << ERR_MSG << "Too many cells!!!\n";
            status = ERR_TABLE_INVALID;
            break;
        }
    }

    if (i == n_cells)
        pretty_table(table, table_info);
    else
        THROW(ERR_TABLE_INVALID)

    cout << table << ' ' << table[0];
    delete[] table;
}

