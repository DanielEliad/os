#include "isrs.h"

/* These are function prototypes for all of the exception
*  handlers: The first 32 entries in the IDT are reserved
*  by Intel, and are designed to service exceptions! */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

const char *exception_messages[] =
{
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void isrs_install() {
	void (*isrs[32])() =   {
							isr0,
							isr1,
							isr2,
							isr3,
							isr4,
							isr5,
							isr6,
							isr7,
							isr8,
							isr9,
							isr10,
							isr11,
							isr12,
							isr13,
							isr14,
							isr15,
							isr16,
							isr17,
							isr18,
							isr19,
							isr20,
							isr21,
							isr22,
							isr23,
							isr24,
							isr25,
							isr26,
							isr27,
							isr28,
							isr29,
							isr30,
							isr31,
						};
	for(int i = 0;i<sizeof(isrs)/sizeof(isrs[0]);i++) {
		idt_set_gate(i, (unsigned)isrs[i], 0x08, 0x8E);
	}
}


/* This is a simple string array. It contains the message that
*  corresponds to each and every exception. We get the correct
*  message by accessing like:
*  exception_message[interrupt_number] */


/* All of our Exception handling Interrupt Service Routines will
*  point to this function. This will tell us what exception has
*  happened! Right now, we simply halt the system by hitting an
*  endless loop. All ISRs disable interrupts while they are being
*  serviced as a 'locking' mechanism to prevent an IRQ from
*  happening and messing up kernel data structures */
void fault_handler(struct regs *r)
{
    print("HANDLING ISR\n");
    print("ISR#: ");
    print(itoa(r->int_no));
    print("\n");
    if (r->int_no < 32)
    {
        print(itoa(exception_messages[r->int_no]));
    	print(" Exception. System Halted!\n");
        for (;;);
    }
}




