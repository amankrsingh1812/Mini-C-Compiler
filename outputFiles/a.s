.global _start
_start:
mov $223, %rax
push %rax
mov $323, %rax
push %rax
mov $600000000, %rax
pop %rcx
cqto
idivq %rcx
pop %rcx
cqto
idivq %rcx
mov %rax, %rdi
mov $60, %rax
syscall
