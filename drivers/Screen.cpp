#include "Screen.h"



void Screen::print_char(char character, int col, int row, char attribute_byte) {
	unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;

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
	} else {
		vidmem[offset] = character;
		vidmem[offset + 1] = attribute_byte;
	}

	offset += 2;
	offset = handle_scrolling(offset);
	set_cursor(offset);
}

int Screen::get_screen_offset(int col, int row) {
	if(col >= MAX_COLS) {
		col = 0;
		++row;
	}

	int offset = (row*MAX_COLS + col)*2;
	return offset;
}


int Screen::get_cursor() {
	IO::port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = IO::port_byte_in(REG_SCREEN_DATA) << 8;
	IO::port_byte_out(REG_SCREEN_CTRL, 15);
	offset += IO::port_byte_in(REG_SCREEN_DATA);

	return offset*2;
}

void Screen::set_cursor(int offset) {

	offset /= 2;//cell offset to char offset
	IO::port_byte_out(REG_SCREEN_CTRL, 14);
	IO::port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset >> 8));
	IO::port_byte_out(REG_SCREEN_CTRL, 15);
	IO::port_byte_out(REG_SCREEN_DATA, (unsigned char) (offset & 0xff));
}

void Screen::print_at(char* message, int col, int row) {
	if(col >= 0 && row >= 0) {
		set_cursor(get_screen_offset(col, row));
	}

	int i = 0;
	int offset;
	while(message[i] != 0) {
		offset = get_cursor();
		print_char(message[i++], get_col(offset), get_row(offset), WHITE_ON_BLACK);
	}
}

int Screen::get_col(int offset) {
	offset /= 2;
	return offset % MAX_COLS;
}

int Screen::get_row(int offset) {
	offset /= 2;
	return offset/MAX_COLS;
}

void Screen::print(char* message) {
	print_at(message, -1, -1);
}

void Screen::clear_screen() {
	for(int row = 0; row < MAX_ROWS; ++row) {
		for(int col = 0; col < MAX_COLS; ++col) {
			print_char(' ', col, row, WHITE_ON_BLACK);
		}
	}

	set_cursor(get_screen_offset(0,0));
}


int Screen::handle_scrolling(int cursor_offset) {
	if(cursor_offset < MAX_ROWS*MAX_COLS*2) {
		return cursor_offset;
	}

	for(int i = 1; i < MAX_ROWS; ++i) {
		Utils::memory_copy((char*) get_screen_offset(0, i) + VIDEO_ADDRESS, (char*) get_screen_offset(0, i-1) + VIDEO_ADDRESS, MAX_COLS*2);
	}

	char* last_line = (char *) get_screen_offset(0, MAX_ROWS-1) + VIDEO_ADDRESS;
	for(int i = 0; i < MAX_COLS*2; ++i) {
		last_line[i] = 0;
	}

	cursor_offset -= 2*MAX_COLS;
	return cursor_offset;
}

