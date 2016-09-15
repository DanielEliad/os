#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "isr_regs.h"
#include "irq.h"
#include "IO.h"
#include "../drivers/screen.h"
#include "utils.h"
#define KEYBOARD_DATA_REGISTER 0x60
#define KEYBOARD_STATUS_REGISTER 0x64

void keyboard_handler(struct regs *r);
void keyboard_install();

#endif
