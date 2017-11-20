#include<iostream>
#include<stdio.h>
#include<conio.h>
#include<process.h>
#include<windows.h>
#include<dos.h>

#define SELECTED_CELL_HORIZONTAL_BOUNDARY "+=======+"
#define NORMAL_CELL_HORIZONTAL_BOUNDARY "+-------+"
#define bckspace  "\b\b\b\b\b\b\b\b\b"

#define N_ROWS 10
#define N_COLS 5
#define CELL_WIDTH 8
#define CELL_HEIGHT 2

const int TABLE_WIDTH = N_COLS * CELL_WIDTH;
const int TABLE_HEIGHT = N_ROWS * CELL_HEIGHT;

using namespace std;

void gotoxy(int x, int y){
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void drawxy(int x, int y /* top left corner */) {
	gotoxy(x, y);
	cout << NORMAL_CELL_HORIZONTAL_BOUNDARY;
	gotoxy(x, y + 1);
	cout << "|       |";
	gotoxy(x, y + 2);
	cout << NORMAL_CELL_HORIZONTAL_BOUNDARY;
}
void selectedbox(int x, int y /* top right corner */) {
	gotoxy(x, y);
	cout << bckspace << SELECTED_CELL_HORIZONTAL_BOUNDARY;
	gotoxy(x, y + 2);
	cout <<bckspace<< SELECTED_CELL_HORIZONTAL_BOUNDARY;
}
int main() {
	char ch;
	const unsigned short MSB = 0x8000;
	int selected_cell_x = 0, selected_cell_y = 0;
	do {
		for (int j = 0; j < TABLE_HEIGHT; j += CELL_HEIGHT) {
			for (int i = 0; i < TABLE_WIDTH; i += CELL_WIDTH) {
				drawxy(i, j);
			}
		}
		selectedbox(selected_cell_x + CELL_WIDTH + 1, selected_cell_y);
		gotoxy(selected_cell_x + 1, selected_cell_y + 1); // move cursor for user input
		ch=_getch();
		if (GetAsyncKeyState(VK_UP) & MSB) {
			selected_cell_y -= CELL_HEIGHT;
			if (selected_cell_y < 0)
				selected_cell_y = TABLE_HEIGHT - CELL_HEIGHT;
		}
		if (GetAsyncKeyState(VK_DOWN) & MSB) {
			selected_cell_y += CELL_HEIGHT;
			if (selected_cell_y > TABLE_HEIGHT - CELL_HEIGHT)
				selected_cell_y = 0;
		}
		if (GetAsyncKeyState(VK_LEFT) & MSB) {
			selected_cell_x -= CELL_WIDTH;
			if (selected_cell_x < 0)
				selected_cell_x = TABLE_WIDTH - CELL_WIDTH;
		}
		if (GetAsyncKeyState(VK_RIGHT) & MSB) {
			selected_cell_x += CELL_WIDTH;
			if (selected_cell_x >  TABLE_WIDTH - CELL_WIDTH)
				selected_cell_x = 0;
		}
		Sleep(200);
		system("cls");
	} while (1);
}
/*#define menu1 "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n"
#define menu2 ":::::::::::::::::''''':::::''''::::':::''''''::'''::::::::::::::::::\n"
#define menu3 ":::::::::::::::: oYoYo.   .oPYo.   o,   8  8    8 ::::::::::::::::::\n"
#define menu4 "::::::::::::::: B' B  B   8oooo8   8 4. 8  8    8 ::::::::::::::::::\n"
#define menu5 "::::::::::::::: B  B  B   8.   ,   8  4.8  8,  ,8 ::::::::::::::::::\n"
#define menu6 "::::::::::::::: B  B  B   'Yooo    8   'P  'Y88Y'.::::::::::::::::::\n"
#define menu7 "::::::::::::::::..:.. 8 :::....::::.:::..:::....::::::::::::::::::::\n"
#define menu8 "::::::::::::::::::::::.:::::::::::::::::::::::::::::::::::::::::::::\n"
#define menu9 "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n\n\n"

void logo_M() {
	cout << menu1 << menu2 << menu3 << menu4 << menu5 << menu6 << menu7 << menu8 << menu9;
}

int main() {
	char ch;
	int i = 0,k;
	char msg[4][20] = { "Create","Delete","open","Exit" };
	do {
		logo_M();
		for (int j = 0; j < 4; j++) {

			if (i == j) {
				cout << line;
				cout << "| [O] >>> " << msg[i];
				int k = (sizeof(line) - sizeof(msg[j]))+1;
				while (k!=0) {
					cout << " ";
					k--;
				}
				cout << "|\n" << line;
			}
			else {
				cout << "  [ ] --- " << msg[j] << endl;
			}

		}
			ch = _getch();
			if (ch == 'w') {
				i--;
				if (i == -1)
					i = 3;
			}
			if (ch == 's') {
				i++;
				if (i == 4)
					i = 0;
			}
			system("cls");
		}while ((int)ch != 27);
	cout << "Thank you!!!";
	ch = _getch();
	return 0;
}
*/
