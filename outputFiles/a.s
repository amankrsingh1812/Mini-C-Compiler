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
mov $10, %rax
push %rax
mov $8, %rax
push %rax
mov $2, %rax
push %rax
mov $4, %rax
pop %rcx
cqto
idivq %rcx
pop %rcx
imul %rcx, %rax
push %rax
mov $23, %rax
push %rax
mov $121, %rax
pop %rcx
imul %rcx, %rax
pop %rcx
add %rcx, %rax
pop %rcx
add %rcx, %rax
mov %rbp, %rsp
pop %rbp
ret
