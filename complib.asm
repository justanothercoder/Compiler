	global _putchar_char:function
	global _getchar:function
	
	global ___fopen_const~string~ref_int_int:function
	global ___fclose_int:function
	global ___fwrite_int_void~ptr_int:function
	global ___fread_int_void~ptr_int:function

	global _string_string_string~ref_const~string~ref:function
	global _string_operatorelem_string~ref_int:function
	global _string_length_string~ref:function
	global _string_operatorplus_string~ref_const~string~ref:function
	global _string_operatorassign_string~ref_const~string~ref:function
		
	global _print_const~string~ref:function
	
	global ___brk_void~ptr:function

    global ___mmap_int_int_int_int_int_int:function
    global ___fork

_putchar_char:
	push rbp
	mov rbp, rsp

	push rsi
	push rax
	push rdi
	push rdx

	lea rsi, [rbp + 16]
	mov rax, 1
	mov rdi, 1
	mov rdx, 1
	syscall

	pop rdx
	pop rdi
	pop rax
	pop rsi
	
	mov rsp, rbp
	pop rbp
	ret

_getchar:
	push rbp
	mov rbp, rsp

	lea rsi, [rbp - 40]
	mov rax, 0
	mov rdi, 0
	mov rdx, 1
	syscall

	mov rbx, [rbp - 40]
	mov rax, [rbp + 16]
	mov [rax], rbx
	
	mov rsp, rbp
	pop rbp
	ret
	
___fopen_const~string~ref_int_int:
	push rbp
	mov rbp, rsp

	mov rax, 2
	mov rdi, [rbp + 24]
	mov rsi, [rbp + 32]
	mov rdx, [rbp + 40]
	syscall

	mov rbx, rax
	mov rax, [rbp + 16]
	mov [rax], rbx

	mov rsp, rbp
	pop rbp
	ret

___fclose_int:
	push rbp
	mov rbp, rsp

	mov rax, qword 3
	mov rdi, [rbp + 16]
	syscall

	mov rsp, rbp
	pop rbp
	ret

___fwrite_int_void~ptr_int:
	push rbp 
	mov rbp, rsp	

    mov rax, 1
    mov rdi, [rbp + 24]
    mov rsi, [rbp + 32]
    mov rdx, [rbp + 40]
    syscall

	mov rsp, rbp
	pop rbp
	ret

___fread_int_void~ptr_int:
	push rbp
	mov rbp, rsp
	
	mov rax, qword 0
	mov rdi, [rbp + 24]
	mov rsi, [rbp + 32]
	mov rdx, [rbp + 40]
	syscall

	mov rbx, rax
	mov rax, [rbp + 16]
	mov [rax], rbx

    mov byte [rsi + rbx], 0

	mov rsp, rbp
	pop rbp
	ret

_string_string_string~ref_const~string~ref:
	push rbp
	mov rbp, rsp

	mov rsi, [rbp + 16]
	mov rdi, [rbp + 24]

.loop:

	cmp byte [rdi], 0
	jz .end
	
	mov bl, byte [rdi]
	mov byte [rsi], bl
	
	inc rsi 
	inc rdi

	jmp .loop

.end:

	mov byte [rsi], 0

	mov rsp, rbp
	pop rbp
	ret

_string_operatorelem_string~ref_int:
	push rbp
	mov rbp, rsp

	mov rsi, [rbp + 24]
	mov rbx, [rbp + 32]

	sub rsi, rbx

	mov rax, [rbp + 16]
	mov [rax], rsi

	mov rsp, rbp
	pop rbp
	ret
	
_string_length_string~ref:
	push rbp
	mov rbp, rsp

	mov rdi, [rbp + 24]
	
	mov rax, 0

.loop:

	cmp byte [rdi], 0
	jz .end

	inc rax
	inc rdi

	jmp .loop

.end:

	mov rbx, rax
	mov rax, [rbp + 16]
	mov [rax], rbx

	mov rsp, rbp
	pop rbp
	ret
	
_print_const~string~ref:
	push rbp
	mov rbp, rsp

    mov rdx, 0
    mov rdi, [rbp + 16]

.loop:

    cmp byte [rdi], 0
    jz .end

    inc rdx
    inc rdi
    jmp .loop

.end:

    mov rax, 1
    mov rdi, 1
    mov rsi, [rbp + 16]
    syscall

	mov rsp, rbp
	pop rbp
	ret
	
_string_operatorplus_string~ref_const~string~ref:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rdi, rax

	mov rsi, [rbp + 24]

.floop:
	
	cmp byte [rsi], 0
	jz .fend

	mov bl, byte [rsi]
	mov byte [rdi], bl

	inc rsi
	inc rdi

	jmp .floop

.fend:

	mov rsi, [rbp + 32]

.sloop:

	cmp byte [rsi], 0
	jz .send

	mov bl, byte [rsi]
	mov byte [rdi], bl

	inc rsi
	inc rdi

	jmp .sloop

.send:

	mov byte [rdi], 0

	mov rsp, rbp
	pop rbp
	ret
	
_string_operatorassign_string~ref_const~string~ref:
	push rbp
	mov rbp, rsp

	mov rdi, [rbp + 24]
	mov rsi, [rbp + 32]

.loop:

	cmp byte [rsi], 0
	jz .end

	mov bl, byte [rsi]
	mov byte [rdi], bl

	inc rsi
	inc rdi

	jmp .loop

.end:

	mov byte [rdi], 0

	mov rsp, rbp
	pop rbp
	ret

___brk_void~ptr:
	push rbp
	mov rbp, rsp

	mov rax, 12
	mov rdi, [rbp + 24]
	syscall

	mov rbx, rax
	mov rax, [rbp + 16]
	mov [rax], rbx

	mov rsp, rbp
	pop rbp
	ret
    
___mmap_int_int_int_int_int_int:
    push rbp
    mov rbp, rsp

    mov rax, 9
    mov rdi, [rbp + 24]
    mov rsi, [rbp + 32]
    mov rdx, [rbp + 40]
    mov r10, [rbp + 48]
    mov r8,  [rbp + 56]
    mov r9,  [rbp + 64]
    syscall

    mov rbx, rax
    mov rax, [rbp + 16]
    mov [rax], rbx

    mov rsp, rbp
    pop rbp
    ret

___fork:
    push rbp
    mov rbp, rsp

    mov rax, 57
    syscall

    mov rbx, rax    
    mov rax, [rbp + 16]
    mov [rax], rbx

    mov rsp, rbp
    pop rbp
    ret
