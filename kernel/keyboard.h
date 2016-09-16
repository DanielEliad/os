#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "isr_regs.h"
#include "irq.h"
#include "IO.h"
#include "../drivers/screen.h"
#include "utils.h"
#define KEYBOARD_DATA_REGISTER 0x60
#define KEYBOARD_STATUS_REGISTER 0x64
#define KEYBOARD_READ_CONFIGURATION 0x20
#define KEYBOARD_RESET_COMMAND 0xff

void keyboard_handler(struct regs *r);
void keyboard_install();
void keyboard_wait_outport();
void disable_translation();
void keyboard_set_scancode_set(unsigned char set);

#endif
