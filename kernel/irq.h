#ifndef IRQ_H
#define IRQ_H

#include "isr_regs.h"
#include "IO.h"
#include "idt.h"

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, void (*handler)(struct regs *r));

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq);

/* handles every IRQ */
void irq_handler(struct regs *r);

/* install all the irq handlers */
void irq_install();


#endif