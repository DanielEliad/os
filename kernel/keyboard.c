#include "keyboard.h"


/* KBDUS means US Keyboard Layout. This is a scancode table
*  used to layout a standard US keyboard. I have left some
*  comments in to give you an idea of what key is what, even
*  though I set it's array index to 0. You can change that to
*  whatever you want using a macro, if you wish! */
unsigned char keyboard_map[128] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', /* 9 */
  '9', '0', '-', '=', '\b', /* Backspace */
  '\t',         /* Tab */
  'q', 'w', 'e', 'r',   /* 19 */
  't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', /* Enter key */
    0,          /* 29   - Control */
  'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', /* 39 */
 '\'', '`',   0,        /* Left shift */
 '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* 49 */
  'm', ',', '.', '/',   0,              /* Right shift */
  '*',
    0,  /* Alt */
  ' ',  /* Space bar */
    0,  /* Caps lock */
    0,  /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,  /* < ... F10 */
    0,  /* 69 - Num lock*/
    0,  /* Scroll Lock */
    0,  /* Home key */
    0,  /* Up Arrow */
    0,  /* Page Up */
  '-',
    0,  /* Left Arrow */
    0,
    0,  /* Right Arrow */
  '+',
    0,  /* 79 - End key*/
    0,  /* Down Arrow */
    0,  /* Page Down */
    0,  /* Insert Key */
    0,  /* Delete Key */
    0,   0,   0,
    0,  /* F11 Key */
    0,  /* F12 Key */
    0,  /* All other keys are undefined */
};



void keyboard_handler(struct regs *r) {
    keyboard_wait_outport();
    char scancode = port_byte_in(KEYBOARD_DATA_REGISTER);
    //if(!(port_byte_in(KEYBOARD_STATUS_REGISTER) & 0x01)) {
    //    return;
    //}

    /*
        if the top bit is set, the key
        has just been released.
    */
    if(scancode & 0x80) {
        

    } 
    
    //Here, a key was just pressed. 
    //If the key was held down there will be repeated key press interrupts
    else {
        print("Button clicked:");
        printch(keyboard_map[(unsigned char)scancode]);
        //print("\nButton clicked:");
        // char s[2];
        // s[0] = keyboard_map[(unsigned char) scancode];
        // s[1] = 0;
        // print(s);
        //print("\n");
        //print(itoa(keyboard_map[(unsigned char)scancode]));
        //print("\n-----------\n");
        print(itoa(scancode));
    }

}

void keyboard_install() {
    
    //Install the keyboard handler to IRQ1
    irq_install_handler(1, keyboard_handler);

    keyboard_wait_outport();
    port_byte_out(KEYBOARD_DATA_REGISTER, KEYBOARD_RESET_COMMAND);      // Reset kb


    keyboard_set_scancode_set(2);

    //disables keyboard scan code translation
    disable_translation();
    

}


void disable_translation() {
    keyboard_wait_outport();
    port_byte_out(KEYBOARD_STATUS_REGISTER, KEYBOARD_READ_CONFIGURATION);                // Send "read configuration byte" command

    keyboard_wait_outport();
    char config = port_byte_in(KEYBOARD_DATA_REGISTER);                  // foo = old value of configuration byte

    config = config & ~(1 << 6);                // foo = old value with translation disabled

    keyboard_wait_outport();
    port_byte_out(KEYBOARD_STATUS_REGISTER, KEYBOARD_DATA_REGISTER);                // Send "write configuration byte" command
    keyboard_wait_outport();
    port_byte_out(KEYBOARD_DATA_REGISTER, config);                 // Set configuration byte to disable translation
}

void keyboard_wait_outport() {
    while(!(port_byte_in(KEYBOARD_STATUS_REGISTER) &0x02));
}



void keyboard_set_scancode_set(unsigned char set) {
    if(set >3 || set <= 0) return;
    keyboard_wait_outport();
    port_byte_out(KEYBOARD_DATA_REGISTER, 0xf0);

    keyboard_wait_outport();
    port_byte_out(KEYBOARD_DATA_REGISTER, set);
}

