print_hex:
    pusha
    mov si, HEX_Map          ; Pointer to hex-character table
    mov di, HEX_Out
    mov bx, ax                      ; BX = argument AX
    and bx, 00FFh                   ; Clear BH (just to be on the safe side)
    shr bx, 4                       ; Isolate high nibble (i.e. 4 bits)
    mov dl, [si+bx]                 ; Read hex-character from the table
    mov [di+0], dl                  ; Store character at the first place in the output string

    mov bx, ax                      ; BX = argument AX (just to be on the safe side)
    and bx, 00FFh                   ; Clear BH (just to be on the safe side)
    and bl, 0Fh                     ; Isolate low nibble (i.e. 4 bits)
    mov dl, [si+bx]                 ; Read hex-character from the table
    mov [di+1], dl                  ; Store character at the second place in the output string

    mov bx, di
    call print_string
    
    popa
    ret
HEX_Map:   DB  '0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'
HEX_Out:   DB  "00", 13, 10   ; string with line feed and carriage return