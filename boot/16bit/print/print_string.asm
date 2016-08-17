print_string:
	pusha
	mov ah, 0xe
	print_char:
		mov al, [bx]
		cmp al,0
		je end_print_string
		int 0x10
		inc bx
		jmp print_char
	end_print_string:
		popa
		ret
