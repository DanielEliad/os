#include "timer.h"

void timer_phase(int hz) {
    int divisor = INPUT_TIMER_HZ / hz;       /* Calculate our divisor */
    port_byte_out(0x43, 0x36);             /* Set our command byte 0x36 */
    port_byte_out(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    port_byte_out(0x40, divisor >> 8);     /* Set high byte of divisor */
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
    struct TimerBuffer * timerBuffer = (struct TimerBuffer *) timer_base;
    timerBuffer->timer_ticks++;

    /* Every 100 clocks (1 second), we will
    *  display a message on the screen */
    if (timerBuffer->timer_ticks % 100 == 0) {
        // print("One second has passed\n");
    }
}

/* Sets up the system clock by installing the timer handler
*  into IRQ0 */
void timer_install() {
	struct TimerBuffer * timerBuffer = (struct TimerBuffer *) timer_base;
	timerBuffer->timer_ticks = 0;
	

    timer_phase(100); //set the tick time to 100 HZ
    
    
    //print(itoa(timer_ticks));
	//print("\n\n\n\n\n\n");
	/* Installs 'timer_handler' to IRQ0 */
	//print(timer_handler);
    irq_install_handler(0, timer_handler);
}


/* This will continuously loop until the given time has
*  been reached */
void timer_wait(int ticks) {
    struct TimerBuffer * timerBuffer = (struct TimerBuffer *) timer_base;
    unsigned long eticks;

    eticks = timerBuffer->timer_ticks + ticks;
    while(timerBuffer->timer_ticks < eticks);
}

