	global _int_operatorassign_int~ref_int:function
	global _int_operatorplus_int~ref_int:function
	global _int_operatorminus_int~ref_int:function
	global _int_operatormul_int~ref_int:function
	global _int_operatoreq_int~ref_int:function
	global _int_operatorneq_int~ref_int:function
	global _int_operatordiv_int~ref_int:function
	global _int_operatormod_int~ref_int:function
	global _int_int_int~ref:function
	global _getchar:function

	global _putchar_int:function
	global _int_int_int~ref_int:function


_int_operatorassign_int~ref_int:
	push rbp
	mov rbp, rsp

	mov rax, [rbp + 16]
	mov rbx, [rbp + 24]

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

_int_int_int~ref_int:
	push rbp
	mov rbp, rsp

	mov rbx, [rbp + 24]
	mov rax, [rbp + 16]
	mov [rax], rbx

	mov rsp, rbp
	pop rbp
	ret 

_putchar_int:
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
