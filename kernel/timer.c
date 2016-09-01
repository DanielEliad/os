#include "timer.h"

void timer_phase(int hz) {
	int divisor = 1193180 / hz;       /* Calculate our divisor */
    port_byte_out(0x43, 0x36);             /* Set our command byte 0x36 */
    port_byte_out(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    port_byte_out(0x41, divisor >> 8);//maybe 0x41??     /* Set high byte of divisor */
}


/* Handles the timer. In this case, it's very simple: We
*  increment the 'timer_ticks' variable every time the
*  timer fires. By default, the timer fires 18.222 times
*  per second. 18.222Hz tick rate was used in order for the
*  tick count to cycle at 0.055 seconds. Using a 16-bit timer
*  tick counter, the counter will overflow and wrap around to 0
*  once every hour. */
void timer_handler(struct regs *r) {
    /* Increment our 'tick count' */
    timer_ticks++;
    print(itoa(timer_ticks));
    print("\n");
    /* Every 100 clocks (approximately 1 second), we will
    *  display a message on the screen */
    if (timer_ticks % 100 == 0)
    {
        print("One second has passed\n");
    }
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install() {
	timer_ticks = 0;
	print(itoa(timer_ticks));
	print("\n");

	timer_phase(100); //set the tick time to 100 HZ
    
    /* Installs 'timer_handler' to IRQ0 */
    irq_install_handler(0, timer_handler);
}


/* This will continuously loop until the given time has
*  been reached */
void timer_wait(int ticks) {
    unsigned long eticks;

    eticks = timer_ticks + ticks;
    while(timer_ticks < eticks);
}

