#include <iostream>
#include <cstring>
#include <sys/stat.h>					// struct stat, stat()

#include "DataTable.hpp"

//#define NDEBUG

#ifndef NDEBUG

#define CHECK_TABLE_VALID(ret) \
if (!header) { cerr << ERR_MSG << "Header is NULL\n"; } \
if (!isValid()) { cerr << ERR_MSG << "Invalid Table\n"; return ret; } \
file.clear();          // clear eof flag
#define THROW(err) { printStatus(status = err); close(); } \

#else

#define CHECK_TABLE_VALID(ret) ;    // do nothing
#define THROW(err) ;                // do nothing

#endif // NDEBUG

bool DataTable::allocHeader() {
	header = new String[table_info.n_cols];
	if (!header) {
		cerr << ERR_MSG << "Out of Memory";
		close();
		status = ERR_TABLE_INVALID;
	}
	return header != nullptr;
}

void input(String input) {
    cin.getline(input, MAX_STR_LEN);
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
	table_info.pad_char = ' ';
	table_info.boundary_fill = '-';
	table_info.cell_seperator = '|';
	table_info.boundary_junction = '+';
	reset();
}

DataTable::~DataTable() {
    delete[] header;
}

void DataTable::reset() {
	table_info.n_rows = table_info.n_cols = 0;
	recordStartPos = recordEndPos = 0;
	strcpy(file_name, "");

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
		strcpy(file_name, fname);
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

String* DataTable::getHeader() const {
    return header;
}

void DataTable::create(String fname) {
	if ((status = createFile(fname)) != SUCCESS) {
		 return;
	}
	openFile(fname, ERR_FILE_NOT_CREATED);
}

void DataTable::open(String fname) {
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

void DataTable::_inputRecord() {
    CHECK_TABLE_VALID( )

	if (!isValid()) {
		cerr << ERR_MSG << "Invalid Table";
		return;
	}
    for (int i = 0; i < getNumCols(); ++i) {
		String cell;
		cout << "Enter value of field '" << header[i] << "'" << PROMPT;
		input(cell);
        file << cell << CELL_DELIM;
	}
}

void DataTable::inputRecord() {
    file.seekg(0, ios::end);
    _inputRecord();
    ++table_info.n_rows;
}

void DataTable::inputHeader() {
    CHECK_TABLE_VALID( )
	if (!allocHeader())
		return;

	file.seekg(TABLE_START, ios::beg);
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

bool DataTable::searchRecord(String what, int field) {                  // found record can be printed through
    CHECK_TABLE_VALID(false)

    int i = 0;
    file.seekg(TABLE_START, ios::beg);
    while (true) {
        String cell;
        if (i % table_info.n_cols == 0)                 // record end
            recordStartPos = file.tellg();
        if (getCell(cell) == EOF)
            break;
        if (i % table_info.n_cols == field && strcmp(cell, what) == 0) {   // found
            recordEndPos = file.tellg();
            return true;
        }
        ++i;
    }
    file.clear();
    return false;       // not found
}

void DataTable::modifyRecord() { // assumes file pointer positioned correctly (by calling searchRecord() first)
    CHECK_TABLE_VALID( )

    ofstream temp(".temp.txt");
    if (!temp) {
        cerr << ERR_MSG << "Could not create temporary file\n";
        return;
    }
    file.seekg(0, ios::beg);
    while (file.tellg() != recordStartPos)
        temp.put(file.get());
    file.seekg(recordEndPos, ios::beg);

    _inputRecord();

    while (file)
        temp.put(file.get());
    file.close();
    temp.close();
    //remove(file_name);
    //rename(".temp.txt", file_name);
}

void DataTable::printCurrentRecord() {
    CHECK_TABLE_VALID( )

    file.seekg(recordStartPos, ios::beg);
    for (int i = 0; i < table_info.n_cols; ++i) {
        String cell;
        if (getCell(cell) == EOF) {
            cerr << ERR_MSG << "Unexpected EOF (while displaying record)\n";
            THROW(ERR_TABLE_INVALID)
            return;
        }
        cout << "\nValue of field '" << header[i] << "': " << cell;
    }
}

void DataTable::display() {
    CHECK_TABLE_VALID( )

    cout << "Table Size: " << table_info.n_rows << " x " << table_info.n_cols << endl;
    int n_cells = table_info.n_rows * table_info.n_cols;
    cout << "No. of cells: " << n_cells << endl;

    String* table = new String[n_cells];
    if (!table) {
		cerr << "Out of Memory!!!";
		return;
	}
    cout << "\nTable Array Allocated\n";

    file.seekg(TABLE_START, ios::beg);
    cout << "\nMoved Cursor to Table Start\n";

    int i = 0;
    while (true) {
        if (getCell(table[i]) == EOF)
            break;
        cout << "i = " << i << " table[i] = " << table[i] << endl;
        if (++i > n_cells) {
            cerr << ERR_MSG << "Too many cells!!!";
            break;
        }
    }

    cout << "\n\nLoaded table array\n\n";
    if (i == n_cells) {
        cout << "Pretty Table Start\n";
        pretty_table(table, table_info);
        cout << "Pretty Table End\n";
    } else
        THROW(ERR_TABLE_INVALID)

    cout << "\nFreeing Memory of table: " << table << "\n";

    if (table) {
        cout << "\ntable[0]: " << table[0] << "\nDeleting table...";
        delete[] table;
    }
    cout << "\nFunction End\n";
}

