#ifndef DATA_TABLE_INC
#define DATA_TABLE_INC

#include <fstream>
#include "pretty_table.hpp"
#include "settings.hpp"
using namespace std;

class DataTable {
    public:
        static void inputFileName(String message, String& fname);

        DataTable();
        ~DataTable();

		bool isValid() const;
		Status getStatus() const;
		int getNumCols() const;
		void setNumCols(int);
		String* getHeader() const;

		void create(String fname);
		void open(String fname);
		void close();

		void inputRecord();
		void inputHeader();

        void addHeader(String head[]);          // n_cols must have been previously set
        bool searchRecord(String what, int field);          // (return value) 1: found, 0: not found
        void modifyRecord();    // modify the record found by searchRecord()
        void printCurrentRecord();                          // print record starting from current file position
		void display();
    private:
        //typedef void (*RecordInputMessageFunc)(int index);

        String file_name;
        fstream file;
        Status status;
        TableInfo table_info;
        String* header;

        // for search, modify, delete
        streampos recordStartPos, recordEndPos;

        bool allocHeader();
        void reset();
        void openFile(String fname, int err_code);
        void _inputRecord();            // file pointer must be set at correct position
    public:
        int getCell(String cell);                          // 0 on success, -1 on failure
};

void input(String input);
bool fileExists(String fname);
void printStatus(Status status);

#endif // DATA_TABLE_INC
