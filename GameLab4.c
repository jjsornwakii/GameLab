#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>
#include <thread>
#include <iostream>

#define screen_x 80
#define screen_y 25
#define scount 80

HANDLE wHnd;
HANDLE rHnd;

DWORD numEvents = 0;
DWORD numEventsRead = 0;
DWORD fdwMode;

CHAR_INFO consoleBuffer[screen_x * screen_y];

SMALL_RECT windowSize = { 0,0,screen_x - 1,screen_y - 1 };

COORD bufferSize = { screen_x,screen_y };
COORD characterPos = { 0,0 };
COORD star[scount];
COORD ship;

int count = 0;

int randomInt(int min, int max) {					// Random MIN MAX 
	return min + rand() % (max - min);
}

int setConsole(int x, int y)
{
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleWindowInfo(wHnd, TRUE, &windowSize);
	SetConsoleScreenBufferSize(wHnd, bufferSize);
	return 0;
}

void clear_buffer() {													// CLEAR DISPLAY
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			consoleBuffer[x + screen_x * y].Char.AsciiChar = ' ';		/// Char
			consoleBuffer[x + screen_x * y].Attributes = 7;				/// color
		}
	}
	
}

void fill_buffer_to_console() {
	WriteConsoleOutputA(wHnd, consoleBuffer, bufferSize, characterPos, &windowSize);		// Draw
}


void init_star() {
	for(int i=0;i<scount;i++){
		star[i] = { (short)randomInt(0,screen_x),(short)randomInt(0,screen_y) };		// Random Star Pos
	}	
}

void star_fall(){
	int i;
	for (i = 0; i < scount; i++) {
		if (star[i].Y >= screen_y - 1) {
			star[i] = { short(rand() % screen_x),1 };
		}																// Fall down Y
		else {
			star[i] = { star[i].X,short(star[i].Y + 1 )};			
		}
	}
}



void fill_star_to_buffer() {
	for (int y = 0; y < screen_y; ++y) {
		for (int x = 0; x < screen_x; ++x) {
			for (int i = 0; i < scount; i++) {
				if (x == star[i].X && y == star[i].Y) {
					consoleBuffer[x + screen_x * y].Char.AsciiChar = '*';		/// Char
					consoleBuffer[x + screen_x * y].Attributes = 7;				/// color
				}
			}
			
		}
	}
	
}
int colorShip = randomInt(0, 255);
void draw_ship_buffer() {
	int POS = (ship.X + screen_x * ship.Y) - 2;
					consoleBuffer[(ship.X + screen_x * ship.Y) - 2].Char.AsciiChar = '<';
					consoleBuffer[(ship.X + screen_x * ship.Y) - 1].Char.AsciiChar = '-';
					consoleBuffer[ship.X + screen_x * ship.Y].Char.AsciiChar = 'O';				/// SHIP
					consoleBuffer[(ship.X + screen_x * ship.Y)+1].Char.AsciiChar = '-';
					consoleBuffer[(ship.X + screen_x * ship.Y)+2].Char.AsciiChar = '>';
				for (int i = 0; i < 5; i++) {
					consoleBuffer[POS+i].Attributes = colorShip;				/// color
				}
}


int setMode()
{
	rHnd = GetStdHandle(STD_INPUT_HANDLE);
	fdwMode = ENABLE_EXTENDED_FLAGS | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT;
	SetConsoleMode(rHnd, fdwMode);
	return 0;
}
////////////////////////////////////////////////////


void setcursor(bool visible) {
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;                                                // disable cursor
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}


int main(){
	
	srand(time(NULL));
	setcursor(0);
	setConsole(screen_x, screen_y);
	setMode();
	init_star();
	
	do{
		star_fall();
		clear_buffer();
		fill_star_to_buffer();

		GetNumberOfConsoleInputEvents(rHnd, &numEvents);

		if (numEvents != 0) {
			INPUT_RECORD* eventBuffer = new INPUT_RECORD[numEvents];
			ReadConsoleInput(rHnd, eventBuffer, numEvents, &numEventsRead);
			for (DWORD i = 0; i < numEventsRead; ++i) {
				
				if (eventBuffer[i].Event.MouseEvent.dwEventFlags == MOUSE_MOVED) {
					ship.X = eventBuffer[i].Event.MouseEvent.dwMousePosition.X;
					ship.Y = eventBuffer[i].Event.MouseEvent.dwMousePosition.Y;
				}
				if (eventBuffer[i].Event.KeyEvent.wVirtualKeyCode == VK_ESCAPE) {
					return 0;
				}
				if (eventBuffer[i].Event.KeyEvent.uChar.AsciiChar == 'c') {
					colorShip = randomInt(0, 255);
				}	
			}
			delete[] eventBuffer;
		}

		draw_ship_buffer();
		fill_buffer_to_console();
		
		for (int k = 0; k < scount; k++) {
			if ((star[k].X >= ship.X - 2 && star[k].X <= ship.X + 2) && (star[k].Y == ship.Y)) {
				colorShip = randomInt(0, 255);

				std::thread q(Beep, 500, 200);
				q.detach();

				star[k].Y = screen_y + 1;
				count++;
			}
		}
		Sleep(100);
	} while (count<=9);
	return 0;
}
