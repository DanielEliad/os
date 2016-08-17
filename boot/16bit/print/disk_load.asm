disk_load:	;Load DH sectors to ES:BX from drive DL
	push dx

	mov ah , 0 x02 ; BIOS read sector function
	mov al , dh ; Read DH sectors
	mov ch , 0 x00 ; Select cylinder 0
	mov dh , 0 x00 ; Select head 0
	mov cl , 0 x02 ; Start reading from second sector ( i.e. after the boot sector )
	int 0 x13 ; BIOS interrupt
	jc disk_error ; Jump if error ( i.e. carry flag set )

	pop dx
	cmp dh, al
	jne disk_error
	ret

	disk_error:
		mov bx, DISK_ERROR_MSG
		call print_string
		jmp $

DISK_ERROR_MSG db "Disk read error!",0
