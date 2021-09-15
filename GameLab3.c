#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
char cursor(int x, int y) {
	HANDLE hStd = GetStdHandle(STD_OUTPUT_HANDLE);
	char buf[2]; 
	COORD c = { x,y }; 
	DWORD num_read;
	if (!ReadConsoleOutputCharacter(hStd, (LPTSTR)buf, 1, c, (LPDWORD)&num_read))
		return '\0';
	else
		return buf[0];
}
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
int checkstarX[21], checkstarY[21];
void rand_star(int star) {
	
	int i, k = 0,ch1=0;
	for (i = 0; i < star; i++) {
		
		checkstarX[i] = 9 + rand() % (69 - 9);
		checkstarY[i] = 1 + rand() % (4 - 1);
		if (cursor(checkstarX[i], checkstarY[i]) != '*') {
			gotoxy(checkstarX[i], checkstarY[i]);
			printf("*");
			gotoxy(0, i);
			//printf("%d %d %d",i, checkstarX[i], checkstarY[i]);	
		}
		else {
			i--;
		}
	}
}
int main() {

	srand(time(NULL));
	setcolor(3, 1);
	system("cls");
	
	char ch = ' ', ch_auto = ' '; 
	int i=0;
	int x = 38, y = 20;
	int shootMode[7] = { 0,0,0,0,0,0,0 }, bullet[7] = { 0,0,0,0,0,0,0 };
	int cBullet = 0, nBullet = 0;
	int xBullet[7] = { 0,0,0,0,0,0,0 }, yBullet[7] = { 0,0,0,0,0,0,0 };
	int cols, rows;
	int nStar = 20;
	int score = 0;
	d_ship(x, y);

	rand_star(nStar);

	do {
		setcursor(0);
		cols = check_windowSizeX() - 2;
		rows = check_windowSizeY() - 1;

		if (_kbhit()) {                                                                 // input W A S D
			ch = _getch();
			if (ch_auto == ' ' || (ch == 'a' || ch == 'd' || ch == 's')) {
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

		if (ch_auto != 's' && ch_auto != ' ') {					    // Auto drive
			if (x >= 1)
				if (ch_auto == 'a') { e_ship(x, y); d_ship(--x, y); }
			if (x <= cols)
				if (ch_auto == 'd') { e_ship(x, y); d_ship(++x, y); }
		}
		else {
			ch_auto = ' ';
		}
		//if (cBullet > 0)
			//Beep(700, 50);

		for (int i = 1; i <= 5; i++) {
			

			if (yBullet[i] == 0 && shootMode[i] == 1) {				/// When bullet move to y=0.

				gotoxy(xBullet[i] + 2, yBullet[i]);
				printf(" ");											/// Bullet will disapeared.
				cBullet--;												/// Number of Bullet is decreased.
				shootMode[i] = 0;
				
			}
			if (shootMode[i] == 1) {					/// Bullet start to move
				if (cursor(xBullet[i] + 2, yBullet[i] - 1) == '*') {
					shootMode[i] = 0;
					gotoxy(xBullet[i] + 2, yBullet[i] );
					printf(" ");
					gotoxy(xBullet[i] + 2, yBullet[i]-1);
					printf(" ");
					cBullet--;
					Beep(500, 20);
					score++;
						rand_star(1);
					break;
					
				}
				gotoxy(xBullet[i] + 2, --yBullet[i]);
				printf("o");

				gotoxy(xBullet[i] + 2, yBullet[i] + 1);
				printf(" ");
				
			}
			
			d_ship(x, y);
		}
		
		gotoxy(cols - 5, 0);
		printf("SCORE : %d", score);
		Sleep(50);
			
	} while (ch != 'x');
	
	return 0;
}
