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

		void create(String fname);
		void open(String fname);
		void close();

		void inputRecord();
		void inputHeader();

        void addHeader(String head[]);          // n_cols must have been previously set
        bool searchRecord(String what, int field);           // (return value) 1: found, 0: not found
        void printCurrentRecord();                          // print record starting from current file position
		void display();
    private:
        //typedef void (*RecordInputMessageFunc)(int index);

        fstream file;
        Status status;
        TableInfo table_info;
        String* header;

        bool allocHeader();
        void reset();
        void openFile(String fname, int err_code);
        int getCell(String cell);                          // 0 on success, -1 on failure
};

void printStatus(Status status);

#endif // DATA_TABLE_INC
