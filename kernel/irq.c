#include "irq.h"

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();


 /* This holds custom IRQ handlers for each IRQ */
void *irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

/* This installs a custom IRQ handler for the given IRQ */
void irq_install_handler(int irq, void (*handler)(struct regs *r)) {
    irq_routines[irq] = handler;
}

/* This clears the handler for a given IRQ */
void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}


void irq_remap(void)
{
	/* initialize both PICs */
    port_byte_out(0x20, 0x11);
    port_byte_out(0xA0, 0x11);

    /* 	
    	set the master PIC's address to 0x20 
    	and the slave PIC's to 0x28 so they are sequential 
    */
    port_byte_out(0x21, 0x20);
    port_byte_out(0xA1, 0x28);

    /*
    	set the call address interval 4 (8)
    	and the mdoe to cascade mode

    */
    port_byte_out(0x21, 0x04);
    port_byte_out(0xA1, 0x02);

    /*
    	set to 8086 mode
    */
    port_byte_out(0x21, 0x01);
    port_byte_out(0xA1, 0x01);

    /*
    	reset the data port of the PICs
    */
    port_byte_out(0x21, 0x0);
    port_byte_out(0xA1, 0x0);
}

/*
	We first remap the interrupt controllers, and then we install
	the appropriate irqs to the correct entries in the IDT. This
	is just like installing the exception handlers 
*/
void irq_install() {
    irq_remap();
    void (*irqs[32])() =
    					{
							irq0,
							irq1,
							irq2,
							irq3,
							irq4,
							irq5,
							irq6,
							irq7,
							irq8,
							irq9,
							irq10,
							irq11,
							irq12,
							irq13,
							irq14,
							irq15
						};
	for(int i = 0;i<sizeof(irqs)/sizeof(irqs[0]);i++) {
		idt_set_gate(i + 32, (unsigned)irqs[i], 0x08, 0x8E);
	}
}








/*
	the handler runs the function that handles the specific IRQ and then
	send out the END OF INTERRUPT (EOI) command 0x20
*/
void irq_handler(struct regs *r) {
    /* This is a blank function pointer */
    void (*handler)(struct regs *r);

    /* Find out if we have a custom handler to run for this
    *  IRQ, and then finally, run it */
    handler = irq_routines[r->int_no - 32];
    if (handler)
    {
        handler(r);
    }

    /* If the IDT entry that was invoked was greater than 40
    *  (meaning IRQ8 - 15), then we need to send an EOI to
    *  the slave controller */
    if (r->int_no >= 40)
    {
        port_byte_out(0xA0, 0x20); //END OF INTERRUPT command to the slave PIC
    }

    /* In either case, we need to send an EOI to the master
    *  interrupt controller too */
    port_byte_out(0x20, 0x20); //END OF INTERRUPT command to the master
}



