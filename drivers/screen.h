#ifndef SCREEN_H
#define SCREEN_H

#include "../kernel/IO.h"
#include "../kernel/utils.h"

#define screen_base (timer_base + sizeof(struct TimerBuffer))
// beware of running over this address with code / data
#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define SCREENSIZE (MAX_COLS*MAX_ROWS*2)

// Value	Color		Value	Color
// 0		BLACK		8		DARK GREY
// 1		BLUE		9		LIGHT BLUE
// 2		GREEN		10		LIGHT GREEN
// 3		CYAN		11		LIGHT CYAN
// 4		RED			12		LIGHT RED
// 5		MAGENTA		13		LIGHT MAGENTA
// 6		BROWN		14		LIGHT BROWN
// 7		LIGHT GREY	15		WHITE


#define WHITE_ON_BLACK 0x0f
#define BLUE_ON_BLACK 0x0b
#define GREEN_ON_BLACK 0x02
#define RED_ON_BLACK 0x04
#define REG_SCREEN_CTRL 0x3D4
#define REG_SCREEN_DATA 0x3D5

// 1 KiloByte
#define screenMemoryBufferSize (1<<10)
struct ScreenBuffer {
	char* screenMemory;
	int startOfWindow;
	int lastRowWritten;
};


void screen_install();
void print_char(char character, int col, int row, char attribute_byte);
int get_screen_offset(int col, int row);
int get_cursor();
void set_cursor(int offset);
void print_at(const char* message, int col, int row, char color);
int get_col(int offset);
int get_row(int offset);
void print(const char* message);
void printColor(const char* message, char color);
void printch(char ch);
void clear_screen();
int handle_scrolling(int cursor_offset);
char gettextcolor(unsigned char forecolor, unsigned char backcolor);
void backspace();

void shiftAndUpdateAll(int newWindowDelta);
void update(int screen_pos, int buffer_pos, int n_characters);
void shiftWindow(int delta);

char isBottomOfScreen();
char isTopOfScreen();

#endif
