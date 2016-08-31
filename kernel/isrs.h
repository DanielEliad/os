#ifndef ISRS_H
#define ISRS_H

#include "idt.h"
#include "../drivers/screen.h"
#include "isr_regs.h"



void isrs_install();

void fault_handler(struct regs *r);

#endif
