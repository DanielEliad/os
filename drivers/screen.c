#include "screen.h"

void screen_install() {
	struct ScreenBuffer* screenBuffer = (struct ScreenBuffer *) screen_base;
	screenBuffer->screenMemory = malloc(screenMemoryBufferSize);
	for(int offset = 0; offset < screenMemoryBufferSize; offset += 2) {
		screenBuffer->screenMemory[offset] == ' ';
		screenBuffer->screenMemory[offset + 1] == WHITE_ON_BLACK;
	}
	screenBuffer->startOfWindow = 0;
	screenBuffer->lastRowWritten = 0;

}

void print_char(char character, int col, int row, char attribute_byte) {
	// unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
	struct ScreenBuffer* screenBuffer = (struct ScreenBuffer *) screen_base;
	unsigned char *vidmem = (unsigned char *) (screenBuffer->screenMemory + screenBuffer->startOfWindow);

	if(!attribute_byte) {
		attribute_byte = WHITE_ON_BLACK;
	}

	int offset;

	if(col >= 0 && row >= 0) {
		offset = get_screen_offset(col, row);
	} else {
		offset = get_cursor();
	}

	if(character == '\n') {
		int rows = offset/(2*MAX_COLS);
		offset = get_screen_offset(MAX_COLS -1, rows);
	} else if(character == '\t') {	// Replace tab with 4 spaces
		for(int i = 0; i < 3; i++) {
			vidmem[offset] == ' ';
			vidmem[offset + 1] == attribute_byte;
			offset += 2;
		}

		vidmem[offset] == ' ';
		vidmem[offset + 1] == attribute_byte;

	} else {
		vidmem[offset] = character;
		vidmem[offset + 1] = attribute_byte;
	}

	offset += 2;
	offset = handle_scrolling(offset);
	set_cursor(offset);
	screenBuffer->lastRowWritten = get_row(offset);
}

int get_screen_offset(int col, int row) {
	if(col >= MAX_COLS) {
		col = 0;
		++row;
	}

	int offset = (row*MAX_COLS + col)*2;
	return offset;
}


int get_cursor() {
	port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = port_byte_in(REG_SCREEN_DATA) << 8;
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);

	return offset*2;
}

void set_cursor(int offset) {
	offset /= 2;//cell offset to char offset
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
}

void print_at(const char* message, int col, int row, char color) {
	if(col >= 0 && row >= 0) {
		set_cursor(get_screen_offset(col, row));
	}

	int i = 0;
	int offset;
	while(message[i] != 0) {
		offset = get_cursor();
		print_char(message[i++], get_col(offset), get_row(offset), color);
	}

	struct ScreenBuffer* screenBuffer = (struct ScreenBuffer *) screen_base;
	update(offset - i*2, screenBuffer->startOfWindow + offset - i*2, i + 1);
}

int get_col(int offset) {
	offset /= 2;
	return offset % MAX_COLS;
}

int get_row(int offset) {
	offset /= 2;
	return offset/MAX_COLS;
}

void print(const char* message) {
	print_at(message, -1, -1, WHITE_ON_BLACK);
}

void printColor(const char* message, char color) {
	print_at(message, -1, -1, color);
}


void printch(char ch) {
	int offset = get_cursor();
	print_char(ch, get_col(offset), get_row(offset), WHITE_ON_BLACK);

	struct ScreenBuffer* screenBuffer = (struct ScreenBuffer *) screen_base;
	update(offset, screenBuffer->startOfWindow + offset, 1);
}

void clear_screen() {
	struct ScreenBuffer* screenBuffer = (struct ScreenBuffer *) screen_base;
	shiftWindow(SCREENSIZE);
	for(int row = 0; row < MAX_ROWS; ++row) {
		for(int col = 0; col < MAX_COLS; ++col) {
			print_char(' ', col, row, WHITE_ON_BLACK);
		}
	}
	set_cursor(get_screen_offset(0,0));
}


int handle_scrolling(int cursor_offset) {
	struct ScreenBuffer* screenBuffer = (struct ScreenBuffer *) screen_base;

	if(cursor_offset < SCREENSIZE) {
		return cursor_offset;
	}

	char* last_line = (char *) (get_screen_offset(0, MAX_ROWS) + screenBuffer->screenMemory + screenBuffer->startOfWindow);
	for(int i = 0; i < MAX_COLS*2; i += 2) {
		last_line[i] = ' ';
		last_line[i + 1] = WHITE_ON_BLACK;
	}

	shiftAndUpdateAll(2*MAX_COLS);
	cursor_offset -= 2*MAX_COLS;
	return cursor_offset;
}



/* Sets the forecolor and backcolor that we will use */
char gettextcolor(unsigned char forecolor, unsigned char backcolor) {
    /* Top 4 bytes are the background, bottom 4 bytes
    *  are the foreground color */
    return (backcolor << 4) | (forecolor & 0x0F);
}

void backspace() {
	// unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
	struct ScreenBuffer* screenBuffer = (struct ScreenBuffer *) screen_base;
	unsigned char *vidmem = (unsigned char *) (screenBuffer->screenMemory + screenBuffer->startOfWindow);
	int offset = get_cursor();
	if(get_col(offset) == 0) return;
	offset -= 2;	// To delete the previous character
	vidmem[offset] = ' ';
	vidmem[offset + 1] = WHITE_ON_BLACK;
	set_cursor(offset);
	update(offset, screenBuffer->startOfWindow + offset, 2);
}

void shiftAndUpdateAll(int newWindowDelta) {
	struct ScreenBuffer* screenBuffer = (struct ScreenBuffer *) screen_base;
	shiftWindow(newWindowDelta);
	update(0, screenBuffer->startOfWindow, SCREENSIZE/2);
}

void update(int screen_pos, int buffer_pos, int n_characters) {
	struct ScreenBuffer* screenBuffer = (struct ScreenBuffer *) screen_base;
	unsigned char *vidmem = (unsigned char *) screenBuffer->screenMemory;

	int n_bytes = n_characters*2;
	memory_copy(vidmem + buffer_pos, VIDEO_ADDRESS + screen_pos, n_bytes);
}


void shiftWindow(int delta) {
	struct ScreenBuffer* screenBuffer = (struct ScreenBuffer *) screen_base;
	if(screenBuffer->startOfWindow + delta - screenMemoryBufferSize < 0) {
		memory_copy
				(
					screenBuffer->screenMemory + delta,
					screenBuffer->screenMemory,
					screenBuffer->startOfWindow
				);	// Maybe +/- delta on n_bytes

	}

	screenBuffer->startOfWindow += delta;
	set_cursor(get_cursor() - delta);
}



char isBottomOfScreen() {
	struct ScreenBuffer* screenBuffer = (struct ScreenBuffer *) screen_base;

	int row = get_row(get_cursor());
	return get_row(get_cursor()) == screenBuffer->lastRowWritten;
}
