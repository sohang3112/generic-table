/* BUGS
 *     Displaying the table (sometimes) crashes the program (eg. in 'family.table')
 *        - Appears even in isolated testing of function
 *     Modifying Record has some problem (check table file after using modify)
 *     Deleting Record achieves necessary changes in file, but if you then Search a Record,
 *     the program pauses (maybe infinite loop?)

 * TODO
 *     Improve User Interface
 *     Add facilities (display, find, edit, delete, etc.)
 *     Strip Terminal Whitespace from user input
 */
#define NDEBUG

#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>                   // toupper()
#include <cstdlib> 					// exit()
#include <unistd.h>                 // chdir()
#include <conio.h>
#include "DataTable.hpp"
#include "large_text.hpp"
using namespace std;

void clrscr() {
    system("cls");
}

void mainMenu(DataTable& table);

void submenuCreateTable(DataTable& table);
void inputCreateEmptyTable(DataTable& table);
void submenuCreateTableFromTemplate(DataTable& table);

void submenuTableOperations(DataTable& table);

char inputOption();
void pause();
bool confirm(String message);
bool inputFileSearch(DataTable& table, String action);         // @return whether record found or not

int main() {

	if (chdir("Tables") != SUCCESS) {
        cerr << ERR_MSG << "Working Directory could not be changed";
	} else {
        cout << "Directory changed";
	}

	DataTable table;
    /*
	table.create("new.table");
	printStatus(table.getStatus());
	String report_card_template_headings[] = {"Roll No.", "Name", "Marks", "Percentage"};
    int report_card_template_cols = 4;
    if (table.isValid()) {
        table.setNumCols(report_card_template_cols);
        table.addHeader(report_card_template_headings);
        table.inputRecord();
    }
    */
	while (true) {
		if (table.isValid()) {
			submenuTableOperations(table);
		} else {
			mainMenu(table);
		}
	}
    /*
    // table.open("family.table;");
    table.file.open("family.table", ios::in);
    table.table_info.n_rows = table.table_info.n_cols = 4;
    table.status = SUCCESS;
    table.display();
    */
	/*String cell;
	table.open("library.table");
	printStatus(table.getStatus());
	cout << "\nheader: " << table.getHeader()[0] << " | "
                         << table.getHeader()[1] << " | "
                         << table.getHeader()[2] << " | "
                         << table.getHeader()[3] << "\n";
    if (table.getCell(cell) == EOF)
        cout << "\nEOF\n";
    else
        cout << "Cell: " << cell;
	table.display();
	*/

	/*
    TableInfo info;
    info.n_rows = 2;
    info.n_cols = 2;
    info.pad_char = ' ';
    info.cell_seperator = '|';
    info.boundary_fill = '-';

    String* table = new String[info.n_rows * info.n_cols];
    strcpy(table[0], "Name"); strcpy(table[1], "Age");
    strcpy(table[2], "Sohang"); strcpy(table[3], "100");
    pretty_table(table, info);
    delete[] table
    */
}

void mainMenu(DataTable& table) {
    //clrscr();
    cout << "======================== DATA TABLE MANAGEMENT SYSTEM =================================";
    cout << "Project by Sohang Chopra and Aman Gill\n\n\n";
	cout << "\nMain Menu:"
         << "\nC - Create Table"
         << "\nO - Open Table"
         << "\nD - Delete Table"
         << "\nE - Exit"
         << PROMPT;
	char opt = inputOption();
    if (opt == 'E') {
        clrscr();
        cout << THANK_YOU_LARGE_TEXT;
        pause();
        exit(SUCCESS);
    }

    String fname;
    if (opt == 'C') {
        DataTable::inputFileName("Enter table name", fname);
        table.create(fname);
		printStatus(table);
        if (table.isValid()) {
            pause();
            submenuCreateTable(table);
        }
    } else if (opt == 'O') {
        DataTable::inputFileName("Enter table name", fname);
        table.open(fname);
		printStatus(table);
    } else if (opt == 'D') {
        DataTable::inputFileName("Enter table name", fname);
        if (!fileExists(fname)) {
            cerr << ERR_MSG << "File '" << fname << "' does not exist";
        } else if (confirm("Are you sure you want to permanently delete this table? ")) {
            if (remove(fname) == SUCCESS) {
                cout << "Table '" << fname << "' successfully deleted";
            } else {
                cerr << ERR_MSG << "Table '" << fname << "' could not be deleted";
            }
        }
    } else {
        cerr << ERR_MSG << "Invalid Option '" << opt << "'";
        return;
    }
    pause();
}

void submenuTableOperations(DataTable& table) {
    clrscr();
    cout << MENU_LARGE_TEXT;
	cout << "\nTable Submenu:"
		 << "\nA - Add Record"
		 << "\nD - Display Table"
		 << "\nF - Find Record"
		 << "\nM - Modify Record"
		 << "\nR - Remove Record"
		 << "\nC - Close Table"
		 << PROMPT;
	char opt = inputOption();
    if (opt == 'A') {
        table.inputRecord();
    } else if (opt == 'C') {
        table.close();
    } else if (opt == 'D') {
        table.display();
    } else if (opt == 'F') {
        if (inputFileSearch(table, "search"))
            table.printCurrentRecord();
    } else if (opt == 'M') {
        if (inputFileSearch(table, "modify"))
            table.modifyRecord();
    } else if (opt == 'R') {
        if (inputFileSearch(table, "remove"))
            table.deleteRecord();
    } else {
        cerr << ERR_MSG << "Invalid Option '" << opt << "'";
	}
	pause();
}

void submenuCreateTable(DataTable& table) {
    clrscr();
	bool correctOptionEntered;
	while (!correctOptionEntered) {
        correctOptionEntered = true;
		cout << "\nCreate Submenu:"
			 << "\nE - Empty File"
			 << "\nT - Template"
			 << PROMPT;
		char opt = inputOption();
        if (opt == 'E') {
            inputCreateEmptyTable(table);
        } else if (opt == 'T') {
            submenuCreateTableFromTemplate(table);
        } else {
            cerr << ERR_MSG << "Invalid Option '" << opt << "'";
            correctOptionEntered = false;
        }
	}
}

void inputCreateEmptyTable(DataTable& table) {
    int cols;
    while (true) {
        cout << "Enter number of columns (max " << MAX_COLS << ")" << PROMPT;
        cin >> cols;
        if (!cin) {                     /* TODO: Prevent multiple inputs (with whitespace) */
            String str;
            cin.clear();                // clear fail bit
            cin >> str;                 // extract last input
            cerr << ERR_MSG << "Not a number ('" << str << "')\n";
        } else if (cols <= 0) {
            cerr << ERR_MSG << "Number of columns cannot be 0 or negative ('" << cols << "')\n";
        } else if (cols > MAX_COLS) {
            cerr << ERR_MSG << "Number of columns exceeds maximum limit ('" << cols << "')\n";
        } else {
            break;
        }
    }
    cin.ignore(1000, '\n');
    table.setNumCols(cols);
    table.inputHeader();
}

void submenuCreateTableFromTemplate(DataTable& table) {
    clrscr();
    // Table Template Definitions
    static String report_card_template_headings[] = {"Roll No.", "Name", "Marks", "Percentage"};
    static int report_card_template_cols = 4;
    static String library_form_template_headings[] = {"Book No.", "Name", "Author", "Price", "Issued On"};
    static int library_form_template_cols = 5;

    bool valid_input;
    while (!valid_input) {        // terminates when correct input is provided
        valid_input = true;

        clrscr();
        cout << TEMPLATE_LARGE_TEXT;
        cout << "\nTemplate Submenu:"
             << "\nS - School Report Card"
             << "\nL - Library Form"
             << PROMPT;
        char opt = inputOption();
        if (opt == 'S') {
            table.setNumCols(report_card_template_cols);
            table.addHeader(report_card_template_headings);
        } else if (opt == 'L') {
            table.setNumCols(library_form_template_cols);
            table.addHeader(library_form_template_headings);
        } else {
            cerr << ERR_MSG << "Invalid Option '" << opt << "'";
            valid_input = false;
            pause();
        }
    }
}

char inputOption() {
    char opt;
    cin >> opt;
    cin.ignore(1000, '\n');
    return toupper(opt);
}

void pause() {
    cout << "\nPress any key to continue...";
    getch();
}

bool confirm(String message) {
    while (true) {
        char opt;
        cout << message << "(y|n)" << PROMPT;
        cin >> opt;
        cin.ignore(1000, '\n');
        if (opt == 'y' || opt == 'Y')
            return true;
        else if (opt == 'n' || opt == 'N')
            return false;
        cerr << ERR_MSG << "Invalid Option '" << opt << "'\n";
    }
}

bool inputFileSearch(DataTable& table, String action) {
    int field_no;
    String value;
    cout << "Enter field number " << PROMPT;
    cin >> field_no;
    --field_no;             // adjust from 0-based to 1-based
    cin.ignore(1000, '\n');
    cout << "Enter value to " << action << " in field '" << table.getHeader()[field_no] << "'" << PROMPT;
    input(value);
    if (table.searchRecord(value, field_no)) {
        cout << "Record Found...\n";
        return true;
    } else {
        cout << "Record Not Found\n";
        return false;
    }
}
