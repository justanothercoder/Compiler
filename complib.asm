	global _int_operatorassign_int~ref_const~int~ref:function
	global _int_operatorplus_int~ref_int:function
	global _int_operatorminus_int~ref_int:function
	global _int_operatormul_int~ref_int:function
	global _int_operatoreq_int~ref_int:function
	global _int_operatorneq_int~ref_int:function
	global _int_operatordiv_int~ref_int:function
	global _int_operatormod_int~ref_int:function
	
	global _int_operatorand_int~ref_int:function
	global _int_operatoror_int~ref_int:function

	global _int_int_int~ref_const~int~ref:function
	global _int_int_int~ref:function

	global _char_operatorassign_char~ref_const~char~ref:function
	global _char_operatoreq_char~ref_char:function
	global _char_operatorneq_char~ref_char:function

	global _char_char_char~ref_const~char~ref:function
	global _char_char_char~ref_const~int~ref:function
	global _char_char_char~ref:function

	global _putchar_char:function
	global _getchar:function
	
	global ___fopen_const~string~ref_int_int:function
	global ___fclose_int:function
	global ___fwrite_int_const~string~ref_int:function
	global ___fread_int_string~ref_int:function

	global _string_string_string~ref_const~string~ref:function
	global _string_operatorelem_string~ref_int:function
	global _string_length_string~ref:function
	global _string_operatorplus_string~ref_const~string~ref:function
	global _string_operatorassign_string~ref_const~string~ref:function
		
	global _print_const~string~ref:function

_int_operatorassign_int~ref_const~int~ref:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rbx, [rbp + 24]
	mov rbx, [rbx]

	mov [rax], rbx

	mov rsp, rbp
	pop rbp
	ret

_int_operatorplus_int~ref_int:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rax, [rax]
	add rax, [rbp + 24]

	mov [rsp - 8], rax
	lea rax, [rsp - 8]

	mov rsp, rbp
	pop rbp
	ret

_int_operatorminus_int~ref_int:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rax, [rax]
	sub rax, [rbp + 24]

	mov [rsp - 8], rax
	lea rax, [rsp - 8]

	mov rsp, rbp
	pop rbp
	ret

_int_operatormul_int~ref_int:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rax, [rax]
	imul qword [rbp + 24]

	mov [rsp - 8], rax
	lea rax, [rsp - 8]

	mov rsp, rbp
	pop rbp
	ret

_int_int_int~ref:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov qword [rax], 0
	
	mov rsp, rbp
	pop rbp
	ret

_int_int_int~ref_const~int~ref:
	push rbp
	mov rbp, rsp

	mov rbx, [rbp + 24]
	mov rbx, [rbx]
	mov rax, [rbp + 16]	
	mov [rax], rbx

	mov r9, [rbp]
	mov [r9 - 8], rax
	lea rax, [r9 - 8]

	mov rsp, rbp
	pop rbp
	ret 

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

	lea rax, [rbp - 40]
	
	mov rsp, rbp
	pop rbp
	ret
	
_int_operatoreq_int~ref_int:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rax, [rax]

	cmp rax, qword [rbp + 24]
	jz .equal

	mov qword [rsp - 8], 0
	lea rax, [rsp - 8]
	mov rsp, rbp
	pop rbp
	ret

.equal:
	mov qword [rsp - 8], 1
	lea rax, [rsp - 8]		
	mov rsp, rbp
	pop rbp
	ret
	
_int_operatorneq_int~ref_int:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rax, [rax]

	cmp rax, qword [rbp + 24]
	je .equal

	mov qword [rsp - 8], 1
	lea rax, [rsp - 8]
	mov rsp, rbp
	pop rbp
	ret

.equal:
	mov qword [rsp - 8], 0
	lea rax, [rsp - 8]		
	mov rsp, rbp
	pop rbp
	ret
	
_int_operatordiv_int~ref_int:

	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rax, [rax]
	xor rdx, rdx
	idiv qword [rbp + 24]

	mov [rsp - 8], rax
	lea rax, [rsp - 8]

	mov rsp, rbp
	pop rbp
	ret

_int_operatormod_int~ref_int:
	
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rax, [rax]
	xor rdx, rdx
	idiv qword [rbp + 24]

	mov [rsp - 8], rdx
	lea rax, [rsp - 8]

	mov rsp, rbp
	pop rbp
	ret
	
_int_operatorand_int~ref_int:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rax, [rax]
	mov rbx, [rbp + 24]
	and rax, rbx

	mov r9, [rbp]
	mov [r9 - 8], rax
	lea rax, [r9 - 8]

	mov rsp, rbp
	pop rbp
	ret

_int_operatoror_int~ref_int:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rax, [rax]
	mov rbx, [rbp + 24]
	or rax, rbx

	mov r9, [rbp]
	mov [r9 - 8], rax
	lea rax, [r9 - 8]

	mov rsp, rbp
	pop rbp
	ret

___fopen_const~string~ref_int_int:
	push rbp
	mov rbp, rsp

	sub rsp, 256

	mov rdi, [rbp + 16]
	lea rsi, [rbp - 256]

.reverse.loop:

	cmp byte [rdi], 0
	jz .end

	mov bl, byte [rdi]	
	mov byte [rsi], bl

	dec rdi
	inc rsi

	jmp .reverse.loop
	
.end:
	
	mov byte [rsi], 0

	mov rax, 2
	lea rdi, [rbp - 256]
	mov rsi, [rbp + 24]
	mov rdx, [rbp + 32]
	syscall

	mov r9, [rbp]
	mov [r9 - 8], rax
	lea rax, [r9 - 8]

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

___fwrite_int_const~string~ref_int:
	push rbp 
	mov rbp, rsp	

	sub rsp, 256

	mov rsi, [rbp + 24]
	lea rdi, [rbp - 256]

.reverse.loop:

	cmp byte [rsi], 0
	jz .end

	mov bl, byte [rsi]
	mov byte [rdi], bl

	dec rsi
	inc rdi

	jmp .reverse.loop

.end:
	
	mov byte [rdi], 0

	mov rax, qword 1
	mov rdi, [rbp + 16]
	lea rsi, [rbp - 256]
	mov rdx, [rbp + 32]
	syscall

	mov rsp, rbp
	pop rbp
	ret

___fread_int_string~ref_int:
	push rbp
	mov rbp, rsp
	
	sub rsp, 256

	mov qword [rbp - 256], 0

	mov rax, qword 0
	mov rdi, [rbp + 16]
	lea rsi, [rbp - 256]
	mov rdx, [rbp + 32]
	syscall

	lea rsi, [rbp - 256]
	mov rdi, [rbp + 24]

.loop:
	
	cmp byte [rsi], 0
	jz .end

	mov bl, byte [rsi]
	mov byte [rdi], bl

	inc rsi
	dec rdi

	jmp .loop

.end:

	mov byte [rdi], 0

	mov rsp, rbp
	pop rbp
	ret


_char_operatoreq_char~ref_char:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov al, byte [rax]

	cmp al, byte [rbp + 24]
	jz .equal

	mov qword [rsp - 8], 0
	lea rax, [rsp - 8]
	mov rsp, rbp
	pop rbp
	ret

.equal:
	mov qword [rsp - 8], 1
	lea rax, [rsp - 8]		
	mov rsp, rbp
	pop rbp
	ret
	
_char_operatorneq_char~ref_char:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov al, byte [rax]

	cmp al, byte [rbp + 24]
	je .equal

	mov qword [rsp - 8], 1
	lea rax, [rsp - 8]
	mov rsp, rbp
	pop rbp
	ret

.equal:
	mov qword [rsp - 8], 0
	lea rax, [rsp - 8]		
	mov rsp, rbp
	pop rbp
	ret

_char_char_char~ref:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov byte [rax], 0
	
	mov rsp, rbp
	pop rbp
	ret

_char_char_char~ref_const~char~ref:
	push rbp
	mov rbp, rsp
	
	mov rbx, [rbp + 24]
	mov bl, byte [rbx]
	mov rax, [rbp + 16]
	mov byte [rax], bl

	mov r9, [rbp]
	mov rbx, [rbp + 16]
	mov [r9 - 8], rbx
	lea rax, [r9 - 8]

	mov rsp, rbp
	pop rbp
	ret 

_char_operatorassign_char~ref_const~char~ref:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rbx, [rbp + 24]
	mov bl, byte [rbx]

	mov byte [rax], bl

	mov rsp, rbp
	pop rbp
	ret
	
_char_char_char~ref_const~int~ref:
	push rbp
	mov rbp, rsp

	mov rbx, [rbp + 24]
	mov bl, byte [rbx]

	mov rax, [rbp + 16]
	mov byte [rax], bl
	
	mov r9, [rbp]
	mov [r9 - 8], rax
	lea rax, [r9 - 8]

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
	
	dec rsi 
	dec rdi

	jmp .loop

.end:

	mov byte [rsi], 0

	mov rsp, rbp
	pop rbp
	ret

_string_operatorelem_string~ref_int:
	push rbp
	mov rbp, rsp

	mov rsi, [rbp + 16]
	mov rbx, [rbp + 24]

	sub rsi, rbx

	mov r9, [rbp]
	mov [r9 - 8], rsi
	lea rax, [r9 - 8]

	mov rsp, rbp
	pop rbp
	ret
	
_string_length_string~ref:
	push rbp
	mov rbp, rsp

	mov rdi, [rbp + 16]
	
	mov rax, 0

.loop:

	cmp byte [rdi], 0
	jz .end

	inc eax
	dec rdi

	jmp .loop

.end:

	mov r9, [rbp]
	mov [r9 - 8], rax
	lea rax, [r9 - 8]	

	mov rsp, rbp
	pop rbp
	ret
	
_print_const~string~ref:
	push rbp
	mov rbp, rsp

	mov r10, [rbp + 16]

.loop:

	cmp byte [r10], 0
	jz .end

	mov rax, 1
	mov rdi, 1
	lea rsi, [r10]
	mov rdx, 1
	syscall

	dec r10

	jmp .loop

.end:

	mov rsp, rbp
	pop rbp
	ret
	
_string_operatorplus_string~ref_const~string~ref:
	push rbp
	mov rbp, rsp

	mov r9, [rbp]
	lea rax, [r9 - 8]

	lea rdi, [r9 - 8]
	
	mov rsi, [rbp + 16]

.floop:
	
	cmp byte [rsi], 0
	jz .fend

	mov bl, byte [rsi]
	mov byte [rdi], bl

	dec rsi
	dec rdi

	jmp .floop

.fend:

	mov rsi, [rbp + 24]

.sloop:

	cmp byte [rsi], 0
	jz .send

	mov bl, byte [rsi]
	mov byte [rdi], bl

	dec rsi
	dec rdi

	jmp .sloop

.send:

	mov byte [rdi], 0

	mov rsp, rbp
	pop rbp
	ret
	
_string_operatorassign_string~ref_const~string~ref:
	push rbp
	mov rbp, rsp

	mov rdi, [rbp + 16]
	mov rsi, [rbp + 24]

.loop:

	cmp byte [rsi], 0
	jz .end

	mov bl, byte [rsi]
	mov byte [rdi], bl

	dec rsi
	dec rdi

	jmp .loop

.end:

	mov byte [rdi], 0

	mov rsp, rbp
	pop rbp
	ret
