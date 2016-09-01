#ifndef TIMER_H
#define TIMER_H

#include "irq.h"
#include "IO.h"
#include "../drivers/screen.h"
#include "utils.h"
#define INPUT_TIMER_HZ = 1193180; //In HZ

void timer_phase(int hz);
int timer_ticks;
void timer_handler(struct regs *r);
void timer_install();
void timer_wait(int ticks);

#endif
