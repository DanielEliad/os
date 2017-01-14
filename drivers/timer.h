#ifndef TIMER_H
#define TIMER_H

#include "../kernel/irq.h"
#include "../kernel/IO.h"
#include "../drivers/screen.h"
#include "../kernel/utils.h"
#define INPUT_TIMER_HZ 1193180 //In HZ
#define timer_base (0x100000)
struct TimerBuffer {
  int timer_ticks;
};
void timer_phase(int hz);
void timer_handler(struct regs *r);
void timer_install();
void timer_wait(int ticks);

#endif
