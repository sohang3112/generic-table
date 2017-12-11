#ifndef SETTINGS_INC
#define SETTINGS_INC

#define MAX_STR_LEN 81
#define MAX_COLS 10

// status codes
#define SUCCESS 0
#define ERR_FILE_NOT_OPENED 1
#define ERR_FILE_EXISTS 2
#define ERR_FILE_NOT_EXISTS 3
#define ERR_FILE_NOT_CREATED 4
#define ERR_TABLE_INVALID 5

// user messages
#define ERR_MSG "ERROR: "
#define PROMPT "\n>>> "

// file specifics
#define CELL_DELIM '\x01'               // any non-printable character would do
#define TABLE_EXT ".table"
#define TABLE_HEADING_START 1                   // first character reserved for (single-digit) no. of columns

#define WINDOW_WIDTH 120                // in Windows Shell Prompt

// alignment codes
#define ALIGN_LEFT 0
#define ALIGN_RIGHT 1
#define ALIGN_CENTER 2

typedef char String[MAX_STR_LEN];
typedef int Status;
typedef int Align;
typedef Align (*AlignFunc)(String);

#endif // SETTINGS_INC
