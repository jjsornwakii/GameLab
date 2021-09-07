#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>

void gotoxy(int x, int y) {
	COORD c = { x,y };                                                          // goto cursor
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void setcursor(bool visible) {                                
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;                                                // disable cursor
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}
int check_windowSizeX() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);          // check window size  
	int x = csbi.srWindow.Right - 4;
	return x;
}
int check_windowSizeY() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);           // check window size  
	int y = csbi.srWindow.Bottom;
	return y;
}
void setcolor(int fg, int bg) {                                                   // set color  
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, bg * 16 + fg);
}
void e_ship(int x, int y) {
	//system("cls");
	gotoxy(x - 1, y);                                                             // erase trace
	printf("      ");
}
void d_ship(int x, int y) {
	gotoxy(x, y);                                                                 // draw ship
	printf("<-0->");
}
int main() {

	
	setcolor(3, 1);
	system("cls");
	char ch = ' ', ch_auto = ' '; int i;
	int x = 38, y = 20;
	int shootMode[7] = {0,0,0,0,0,0,0}, bullet[7] = { 0,0,0,0,0,0,0 };
	int cBullet = 0, nBullet = 0;
	int xBullet[7] = { 0,0,0,0,0,0,0 }, yBullet[7] = { 0,0,0,0,0,0,0 };
	int cols, rows;
	d_ship(x, y);

	do {
		setcursor(0);
		cols = check_windowSizeX()-2;
		rows = check_windowSizeY();
		
		if (_kbhit()) {                                                                 // input W A S D
			ch = _getch();
			if (ch_auto == ' ' || (ch == 'a'|| ch == 'd' || ch == 's') ) {
				ch_auto = ch;                                                           // input A D and S for Auto pilot
			}
			if (x >= 1)
				if (ch == 'a') { e_ship(x, y); d_ship(--x, y); }                        // move left
			if (x <= cols)
				if (ch == 'd') { e_ship(x, y); d_ship(++x, y); }                        // move right
			if (y >= 1)
				if (ch == 'w') { e_ship(x, y); d_ship(x, --y); }                        // move forward
			if (y <= rows)
				if (ch == 's') { e_ship(x, y); d_ship(x, ++y); }                        // move backward
			
			if (ch == 32 && cBullet < 5) {	//////// Press Space Bar to shoot and check number of bullet
				++cBullet;
				for (i = 1; i <= 5; i++) {
					if (shootMode[i] != 1) {
						shootMode[i] = 1;			// Turn on bullet number [i] 
						xBullet[i] = x;
						yBullet[i] = y;
						break;
					}
				}
			}
			fflush(stdin);
		}

		if (ch_auto != 's' && ch_auto != ' ' ) {					    // Auto drive
			if (x >= 1)
				if (ch_auto == 'a') { e_ship(x, y); d_ship(--x, y); }
			if (x <= cols)
				if (ch_auto == 'd') { e_ship(x, y); d_ship(++x, y); }
		}
		else {
			ch_auto = ' ';
		}

		if (nBullet == 5) {   /// select bullet number for moving
			nBullet = 0;			
		}
		nBullet++;
		if (yBullet[nBullet] == 0 && shootMode[nBullet] == 1) {				/// When bullet move to y=0.

			gotoxy(xBullet[nBullet] + 2, yBullet[nBullet]);
			printf(" ");											/// Bullet will disapeared.
			cBullet--;												/// Number of Bullet is decreased.
			shootMode[nBullet] = 0;
		}
		if (shootMode[nBullet] == 1) {					/// Bullet start to move
			
			gotoxy(xBullet[nBullet] + 2, --yBullet[nBullet]);
			printf("o");
			
			gotoxy(xBullet[nBullet] + 2, yBullet[nBullet] + 1);
			printf(" ");
		}
		d_ship(x, y);
		Sleep(50);
	} while (ch != 'x');
	return 0;
}
