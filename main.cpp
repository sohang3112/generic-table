/* TODO
 * Write number of columns in file (on creation), read (on opening)
 * Improve Menu
 * Add facilities (display, find, edit, etc.)
 */
#define NDEBUG

#include <iostream>
#include <fstream>
#include <cstdlib> 					// exit()
#include <unistd.h>                 // chdir()
#include "DataTable.hpp"
#include "settings.hpp"
using namespace std;

void mainMenu(DataTable& table);

void submenuCreateTable(DataTable& table);
void inputCreateEmptyTable(DataTable& table);
void submenuCreateTableFromTemplate(DataTable& table);

void submenuTableOperations(DataTable& table);

int main() {
	if (chdir("Tables") != SUCCESS) {
        cerr << ERR_MSG << "Working Directory could not be changed";
	}

	DataTable table;
	while (true) {
		if (table.isValid()) {
			submenuTableOperations(table);
		} else {
			mainMenu(table);
		}
	}
}

void mainMenu(DataTable& table) {
	cout << "\nMain Menu:"
         << "\nC - Create Table"
         << "\nO - Open Table"
         << "\nE - Exit"
         << PROMPT;
	char opt;
    cin >> opt;
    cin.ignore(1000, '\n');
    if (opt == 'E' || opt == 'e') {
        cout << "Exiting...\n";
        exit(SUCCESS);
    }

    String fname;
    if (opt == 'C' || opt == 'c') {
        DataTable::inputFileName("Enter table name", fname);
        table.create(fname);
		printStatus(table.getStatus());
        if (table.isValid()) {
            submenuCreateTable(table);
        }
    } else if (opt == 'O' || opt == 'o') {
        DataTable::inputFileName("Enter table name", fname);
        table.open(fname);
		printStatus(table.getStatus());
    } else {
        cerr << ERR_MSG << "Invalid Option '" << opt << "'";
        return;
    }
}

void submenuCreateTable(DataTable& table) {
    bool correct_input = true;
	do {
		cout << "\nCreate Submenu:"
			 << "\nE - Empty File"
			 << "\nT - Template"
			 << PROMPT;
		char opt;
		cin >> opt;
		if (opt == 'E' || opt == 'e') {
			inputCreateEmptyTable(table);
		} else if (opt == 'T' || opt == 't') {
			submenuCreateTableFromTemplate(table);
		} else {
			cerr << ERR_MSG << "Invalid Option '" << opt << "'";
			correct_input = false;
		}
	} while (!correct_input);
}

void inputCreateEmptyTable(DataTable& table) {
    int cols;
    while (true) {
        cout << "Enter number of columns (max " << MAX_COLS << ")" << PROMPT;
        if (!(cin >> cols)) {
            String str;
            cin.clear();                // clear fail bit
            cin.getline(str, MAX_STR_LEN);                // extract last input
            cerr << ERR_MSG << "Not a number ('" << str << "')\n";
        } else if (cols <= 0) {
            cerr << ERR_MSG << "Number of columns cannot be 0 or negative ('" << cols << "')\n";
        } else if (cols > MAX_COLS) {
            cerr << ERR_MSG << "Number of columns exceeds maximum limit ('" << cols << "')\n";
        } else
            break;
    }
    cin.ignore(1000, '\n');
    table.setNumCols(cols);
    table.inputHeader();
}

void submenuCreateTableFromTemplate(DataTable& table) {
    // Table Template Definitions
    static String report_card_template_headings[] = {"Roll No.", "Name", "Marks", "Percentage"};
    static int report_card_template_cols = 4;
    static String library_form_template_headings[] = {"Book No.", "Name", "Author", "Price", "Issued On"};
    static int library_form_template_cols = 5;

    do {        // terminates when correct input is provided
        char opt;
        cout << "\nTemplate Submenu:"
             << "\nS - School Report Card"
             << "\nL - Library Form"
             << PROMPT;
        cin >> opt;
        if (opt == 'S' || opt == 's') {
            table.setNumCols(report_card_template_cols);
            table.addHeader(report_card_template_headings);
        } else if (opt == 'L' || opt == 'l') {
            table.setNumCols(library_form_template_cols);
            table.addHeader(library_form_template_headings);
        } else {
            cerr << ERR_MSG << "Invalid Option '" << opt << "'";
            continue;
        }
    } while (false);
}

void submenuTableOperations(DataTable& table) {
	cout << "\nTable Submenu:"
		 << "\nA - Add Record"
		 << "\nD - Display Table"
		 << "\nC - Close Table"
		 << PROMPT;
	char opt;
	cin >> opt;
	cin.ignore(1000, '\n');
	if (opt == 'A' || opt == 'a') {
		table.inputRecord();
	} else if (opt == 'C' || opt == 'c') {
		table.close();
	} else if (opt == 'D' || opt == 'd') {
		table.display();
	} else {
		cerr << ERR_MSG << "Invalid Option '" << opt << "'";
	}
}


