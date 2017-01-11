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


unsigned char kbdusysh[128] =
{
0,  0, '!', '@', '#', '$', '%', '^', '&', '*',   /* 9 */
  '(', ')', '_', '+', '\b', '\t',
  'Q', 'W', 'E', 'R',
  'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',      /* Enter key */
    0,         /* 29   - Control */
  'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';',   /* 39 */
'|', '¬',   42,      /* Left shift */
'\\', 'Z', 'X', 'C', 'V', 'B', 'N',         /* 49 */
  'M', '<', '>', '?',   0,               /* Right shift */
  0,
    0,   /* Alt */
  ' ',   /* Space bar */
    58,   /* Caps lock */
    0,   /* 59 - F1 key ... > */
    0,   0,   0,   0,   0,   0,   0,   0,
    0,   /* < ... F10 */
    0,   /* 69 - Num lock*/
    0,   /* Scroll Lock */
    0,   /* Home key */
    0,   /* Up Arrow */
    0,   /* Page Up */
  '-',
    0,   /* Left Arrow */
    0,
    0,   /* Right Arrow */
  '+',
    0,   /* 79 - End key*/
    0,   /* Down Arrow */
    0,   /* Page Down */
    0,   /* Insert Key */
    0,   /* Delete Key */
    0,   0,   0,
    0,   /* F11 Key */
    0,   /* F12 Key */
    0,   /* All other keys are undefined */
};


    //keyboard_wait_outport();
    while ( !(port_byte_in(0x64)&1) );
    unsigned char scancode = port_byte_in(KEYBOARD_DATA_REGISTER);
    struct KeyboardBuffer* keys = (struct KeyboardBuffer* )keyboard_base;
    switch(scancode) {
        case KRLEFT_SHIFT: case KRRIGHT_SHIFT:
            keys->SHIFT = 1;
            return;
        case KRLEFT_ALT:
            keys->ALT = 1;
            return;
        case KRLEFT_CTRL:
            keys->CTRL = 1;
            return;
        case KRCAPS_LOCK:
            keys->SHIFT = ~keys->SHIFT;
            return;
    }
    unsigned char key;
    if(keys->SHIFT) {
        key = kbdusysh[scancode & 0x7f];
    } else {
        key = keyboard_map[scancode & 0x7f];
    }
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
        // print("KEY UP\n");
        switch(scancode&0x7f) {
            case KRLEFT_SHIFT: case KRRIGHT_SHIFT:
                keys->SHIFT = 0;
                break;
            case KRLEFT_ALT:
                keys->ALT = 0;
                break;
            case KRLEFT_CTRL:
                keys->CTRL = 0;
                break;
        }
        return;
    }
    

    switch(key) {
        case BACKSPACE:
            if(keys->i == 0) {
                break;
            }
            keys->buffer[keys->i--] = 0;
            backspace();
            break;
        case ENTER:
            keys->buffer[keys->i++] = 0;
            printch(ENTER);
            runCommand(keys->buffer);
            memory_set(keys->buffer, 0x00, keys->i);
            keys->i = 0;
            break;
        case '\t':
            keys->buffer[keys->i++] = ' ';
            keys->buffer[keys->i++] = ' ';
            keys->buffer[keys->i++] = ' ';
            keys->buffer[keys->i++] = ' ';
            printch('\t');
            break;
        case KUP:
            if(keys->CTRL) {
                // Scroll up
                shiftAndUpdateAll(-MAX_COLS*2);
            } else {
                // Repeat last command up
            }
            break;
        case KDOWN:
            if(keys->CTRL) {
                // Scroll down
                if(!isBottomOfScreen()) {
                    shiftAndUpdateAll(MAX_COLS*2);
                }
            } else {
                // Repeat last command down
            }
            break;
        default:
            keys->buffer[keys->i++] = key;
            printch(key);
            break;
    }
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

