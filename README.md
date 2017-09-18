# generic-table
DESCRIPTION
A command line interface that allows you to create, open or modify 'generic' tables,
i.e., you choose which and how many columns you want in a table. In the table, you have
the options to:
1. Display whole table.
   (in tabular form, nice features could be adjustable column width, word wrapping within columns)
2. Add a row (must fill all columns).
3. Edit a cell in a row (to edit whole row, type column-name '.').
4. Find a cell using a column name.
5. Delete table.

IMPLEMENTATION DETAILS (need suggestions)
1. File extension for table could be '.table'.
2. Suggest error messages.

STYLE GUIDE (mostly borrowed from online style guides)
1. Try to avoid macros, pointers, goto.
2. Use 'int main() {... return 0; }' pattern.
3. Use four spaces for indentation.
4. Always use header guards in header files
   Example: <br />
   ```c++
   /* Module documentation */
   
   #ifndef MODULENAME_INC
   #define MODULENAME_INC
   
   ...
   
   #endif       //MODULENAME_INC
   ```
5. Use cout for normal output, cerr for error messages.
6. Error messages should be of the form 'ERROR: Error message'

NAMING CONVENTIONS
1. MACRO_WORDS - Completely upper-case, use underscore between words.
2. ClassName - Upper camel case (First letter in upper-case, first word of each word capital).
3. classMemberFunctions - Lower camel case (First letter in lower case, first word of each word capital).
4. variable_name - Underscore case (Completely lower-case, underscore between words).

DOCUMENTATION
1. Describe a module (header file) in its first line using multi-line comment form (/* ... */).
2. Describe a class or function before its definition using single-line comment form (//...) <br />
   Example: <br />
   ```c++
   //Example class
   //represents something
   class SomeClass {
       ...
   };
   
   //Example function
   //does something
   void some_function() {
       ...
   }
   ```
3. Add single-line comments for any line of code that may be difficult to understand.
