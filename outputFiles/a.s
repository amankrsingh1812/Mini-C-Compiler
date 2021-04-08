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
mov $1, %rax
push %rax 
mov $2, %rax
mov %rax, -8(%rbp)
mov -8(%rbp), %rax
mov %rbp, %rsp
pop %rbp
ret
mov $0, %rax
mov %rbp, %rsp
pop %rbp
ret
