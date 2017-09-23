# Generic Table (School Project)
## Description
A command line interface that allows you to create, open or modify 'generic' tables,
i.e., you choose which and how many columns you want in a table. In the table, you have
the options to:
- Display whole table.
   (in tabular form, nice features could be adjustable column width, word wrapping within columns)
- Add a row (must fill all columns).
- Edit a cell in a row (to edit whole row, type column-name '.').
- Find a cell using a field (column) name.
- Delete table.

## Ideas
- Default Field Values: When user is adding/modifying record, and leaves a field blank, then a default
  value for the field could be used (and setting the default field value could be done by showing an 
  option for this in menu).

## Style Guide
### (mostly borrowed from online style guides)
- Try to avoid macros, pointers, `goto`. <br />
  (If you must use pointers, don't use raw pointers, use 
  [smart pointers](https://www.google.co.in/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&cad=rja&uact=8&ved=0ahUKEwjPv7C2_bjWAhWJOY8KHeL3DZgQFggnMAE&url=https%3A%2F%2Fen.wikipedia.org%2Fwiki%2FSmart_pointer&usg=AFQjCNH4ZytUJdqQBsDb1CY4MfxUS3hROg) instead.)
- Use `int main() {... return 0; }` pattern.
- Use tab or four spaces for indentation.
- Always use [header guards](https://www.google.co.in/url?sa=t&rct=j&q=&esrc=s&source=web&cd=13&cad=rja&uact=8&ved=0ahUKEwiy-az-_bjWAhWMLI8KHQwVAMsQFghfMAw&url=https%3A%2F%2Fen.wikipedia.org%2Fwiki%2FInclude_guard&usg=AFQjCNHepjZ5tQKjHbavij1ZAaG1oRQ-fQ) in header files. <br />
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
- When a function pointer needs to be stored or passed to a function, then always use `typedef`. <br />
   Example:
   ```c++
   //avoid this
   void do_something(void (*func)(int));

   //do this instead
   typedef void (*Callback)(int);
   void do_something(Callback func);
   ```
 - Function/Class/Variable names must be descriptive, yet not too long. <br />
   eg. `getName()`, `setName()`                          //accessor functions <br />
       `isRunning()`                                     //boolean state functions <br />
       `toString()`                                      //conversion functions <br />
   Note: If a short form is used in a name, then do NOT use all caps. <br />
   eg. `validateHtml()` not `validateHTML()` <br />

## Naming Conventions
- MACRO_WORDS - Completely upper-case, use underscore between words.
- ClassName - Upper camel case (First letter in upper-case, first word of each word capital).
- classMemberFunctions - Lower camel case (First letter in lower case, first word of each word capital).
- variable_name - Underscore case (Completely lower-case, underscore between words).

## Documentation
- Describe a module (header file) in its first line using multi-line comment form `/* ... */`
- Describe a class or function before its definition using single-line comment form `// ... `. <br />
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
- Add single-line comments wherever appropriate.
