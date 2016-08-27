[bits 32]
section .text


; Loads the IDT defined in '_idtp' into the processor.
; This is declared in C as 'extern void idt_load();'
global idt_load
extern idtp
idt_load:
;	mov bx, MSG_IDT_LOAD
;	call print_string_pm
    lidt [idtp]
    ret

;MSG_IDT_LOAD db "--------------------REACHED IDT LOAD--------------------", 0
; Interrupt Service Routines (ISRs)
;%include "16bit/pm/print_string_pm.asm"