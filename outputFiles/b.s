.global _start
_start:
mov $2, %rax
cmp $0, %rax
mov 0, %rax
sete %al
mov %rax, %rdi
mov $60, %rax
syscall
