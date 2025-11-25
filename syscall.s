bits 64

section .text
    global _start
    global syscall3
    global syscall6
    global syscall
    extern main

; Program entry point: call main(), then exit(0)
_start:
    call main
    mov     rax, 60       ; syscall: exit
    xor     rdi, rdi      ; status = 0
    syscall               ; does not return

; long syscall3(long number, long arg1, long arg2, long arg3);
syscall3:
    mov     rax, rdi      ; syscall number
    mov     rdi, rsi      ; arg1
    mov     rsi, rdx      ; arg2
    mov     rdx, rcx      ; arg3
    syscall
    ret

; long syscall6(long number, long arg1, long arg2, long arg3,
;               long arg4, long arg5, long arg6);
syscall6:
    mov     rax, rdi      ; syscall number
    mov     rdi, rsi      ; arg1
    mov     rsi, rdx      ; arg2
    mov     rdx, rcx      ; arg3
    mov     r10, r8       ; arg4
    mov     r8,  r9       ; arg5
    mov     r9,  [rsp+8]  ; arg6 (from stack)
    syscall
    ret

syscall:
    jmp     syscall6

section .note.GNU-stack noalloc noexec nowrite progbits
