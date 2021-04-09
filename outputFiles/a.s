.global _start
_start:
call _main
mov %rax, %rdi
mov $60, %rax
syscall
.global _main
_main:
push %rbp
mov %rsp, %rbp
mov $0, %rax
push %rax 
_for0:
mov $3, %rax
push %rax
mov -8(%rbp), %rax
pop %rcx
cmp %rcx, %rax
mov $0, %rax
setle %al
cmp $1, %rax
jne _postfor0
mov $1, %rax
push %rax
mov -8(%rbp), %rax
pop %rcx
add %rcx, %rax
mov %rax, -8(%rbp)
jmp _for0
_postfor0:
mov -8(%rbp), %rax
mov %rbp, %rsp
pop %rbp
ret
