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
mov $2, %rax
push %rax 
mov $13, %rax
push %rax 
mov -8(%rbp), %rax
push %rax
mov -16(%rbp), %rax
pop %rcx
add %rcx, %rax
mov %rax, -16(%rbp)
mov -16(%rbp), %rax
mov %rbp, %rsp
pop %rbp
ret
