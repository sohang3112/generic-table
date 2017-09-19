# generic-table
## Description
A command line interface that allows you to create, open or modify 'generic' tables,
i.e., you choose which and how many columns you want in a table. In the table, you have
the options to:
- Display whole table.
   (in tabular form, nice features could be adjustable column width, word wrapping within columns)
- Add a row (must fill all columns).
- Edit a cell in a row (to edit whole row, type column-name '.').
- Find a cell using a column name.
- Delete table.

## Implementation Details 
### (need suggestions)
- File extension for table could be '.table'.
- Suggest error messages.

## Style Guide
### (mostly borrowed from online style guides)
- Try to avoid macros, pointers, goto.
- Use 'int main() {... return 0; }' pattern.
- Use four spaces for indentation.
- Always use header guards in header files. <br />
   Example:
   ```c++
   /* Module documentation */
   
   #ifndef MODULENAME_INC
   #define MODULENAME_INC
   
   ...
   
   #endif       //MODULENAME_INC
   ```
- Use `cout` for normal output, `cerr` for error messages.
- Error messages should be of the form 'ERROR: Error message'

## Naming Conventions
- MACRO_WORDS - Completely upper-case, use underscore between words.
- ClassName - Upper camel case (First letter in upper-case, first word of each word capital).
- classMemberFunctions - Lower camel case (First letter in lower case, first word of each word capital).
- variable_name - Underscore case (Completely lower-case, underscore between words).

## Documentation
- Describe a module (header file) in its first line using multi-line comment form (/* ... */).
- Describe a class or function before its definition using single-line comment form (//...). <br />
   Example:
   ```c++
   /* Example module
    * Documentation
    * Provides some functionality
    */
    
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
- Add single-line comments for any line of code that may be difficult to understand.
