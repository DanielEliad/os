#include "keyboard.h"






void keyboard_handler(struct regs *r) {
    /* KBDUS means US Keyboard Layout. This is a scancode table
    *  used to layout a standard US keyboard. I have left some
    *  comments in to give you an idea of what key is what, even
    *  though I set it's array index to 0. You can change that to
    *  whatever you want using a macro, if you wish! */
     unsigned char keyboard_map[] =
    {
NULL, ESC, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', BACKSPACE,
TAB, 'q', 'w',   'e', 'r', 't', 'y', 'u', 'i', 'o', 'p',   '[', ']', ENTER, 0,
'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, 0, 0, ' ', 0,
KF1, KF2, KF3, KF4, KF5, KF6, KF7, KF8, KF9, KF10, 0, 0,
KHOME, KUP, KPGUP,'-', KLEFT, '5', KRIGHT, '+', KEND, KDOWN, KPGDN, KINS, KDEL, 0, 0, 0, KF11, KF12 };;

    //keyboard_wait_outport();
    while ( !(port_byte_in(0x64)&1) );
    unsigned char scancode = port_byte_in(KEYBOARD_DATA_REGISTER);
    unsigned char key = keyboard_map[scancode & 0x7f];
    // char str_scancode[get_num_digits(scancode & 0x7f) + 1];
    // itoa(scancode & 0x7f, str_scancode);
    // print(str_scancode);
    // char str_key[get_num_digits(key) + 1];
    // itoa(key, str_key);
    // print(str_key);
    // printch('b');
    // printch(key);
    if(scancode & 0x80) {
        //KEY UP
        return;
    }
    struct KeyboardBuffer* keys = (struct KeyboardBuffer* )keyboard_base;
    if(key == BACKSPACE) {
        keys->buffer[keys->i--] = 0;
        backspace();
        return;
    }
    if(key == ENTER) {
        // print("ENTER");
        keys->buffer[keys->i++] = 0;
        printch(ENTER);
        runCommand(keys->buffer);
        memory_set(keys->buffer, 0x00, keys->i);
        keys->i = 0;
        return;
    }
    
    // char tmp[20]; itoa(keys->i, tmp); print(tmp); printch('\n');
    keys->buffer[keys->i++] = key;
    printch(key);
    return;
}

void keyboard_install() {
   
    //Allocate the buffer
    struct KeyboardBuffer* keys = (struct KeyboardBuffer* )keyboard_base;
    keys->buffer = malloc(BufferLen);
    keys->i = 0;
    // char tmp[200]; itoa(keys->i, tmp); print(tmp); printch('\n');

    //Install the keyboard handler to IRQ1
    irq_install_handler(1, keyboard_handler);

    // keyboard_wait_outport();
    // port_byte_out(KEYBOARD_DATA_REGISTER, KEYBOARD_RESET_COMMAND);      // Reset kb


    // keyboard_set_scancode_set(2);

    // //disables keyboard scan code translation
    // disable_translation();
    

}


// void disable_translation() {
//     keyboard_wait_outport();
//     port_byte_out(KEYBOARD_STATUS_REGISTER, KEYBOARD_READ_CONFIGURATION);                // Send "read configuration byte" command

//     keyboard_wait_outport();
//     char config = port_byte_in(KEYBOARD_DATA_REGISTER);                  // foo = old value of configuration byte

//     config = config & ~(1 << 6);                // foo = old value with translation disabled

//     keyboard_wait_outport();
//     port_byte_out(KEYBOARD_STATUS_REGISTER, KEYBOARD_WRITE_CONFIGURATION);                // Send "write configuration byte" command
//     keyboard_wait_outport();
//     port_byte_out(KEYBOARD_DATA_REGISTER, config);                 // Set configuration byte to disable translation
// }

// void keyboard_wait_outport() {
//     return;
//     while(!(port_byte_in(KEYBOARD_STATUS_REGISTER) &0x02));
// }



// void keyboard_set_scancode_set(unsigned char set) {
//     if(set >3 || set <= 0) return;
//     keyboard_wait_outport();
//     port_byte_out(KEYBOARD_DATA_REGISTER, 0xf0);

//     keyboard_wait_outport();
//     port_byte_out(KEYBOARD_DATA_REGISTER, set);
// }

