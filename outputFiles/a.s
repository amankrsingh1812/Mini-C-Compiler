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
mov $0, %rax
push %rax 
mov -8(%rbp), %rax
cmp $0, %rax
je _notif1
mov $2, %rax
mov %rax, -8(%rbp)
jmp _postif1
_notif1:
mov $3, %rax
mov %rax, -8(%rbp)
_postif1:
mov -16(%rbp), %rax
cmp $0, %rax
je _notif2
mov $4, %rax
mov %rax, -16(%rbp)
jmp _postif2
_notif2:
mov $5, %rax
mov %rax, -16(%rbp)
_postif2:
mov -16(%rbp), %rax
push %rax
mov -8(%rbp), %rax
pop %rcx
add %rcx, %rax
mov %rbp, %rsp
pop %rbp
ret
