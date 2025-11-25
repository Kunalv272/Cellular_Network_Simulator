# syscall.s
# This implements a raw system call to write to STDOUT (file descriptor 1)
# Architecture: x86_64 Linux

.intel_syntax noprefix  # Use Intel syntax (easier to read than AT&T)
.global low_level_write

# Function signature: void low_level_write(const char* buffer, unsigned long length)
# According to System V ABI:
# rdi = argument 1 (buffer)
# rsi = argument 2 (length)

low_level_write:
    mov rdx, rsi        # 3rd arg for syscall (count) goes in RDX. Move length there.
    mov rsi, rdi        # 2nd arg for syscall (buf) goes in RSI. Move buffer ptr there.
    mov rdi, 1          # 1st arg for syscall (fd). 1 = STDOUT.
    mov rax, 1          # System call number for 'sys_write' is 1 on x86_64.
    syscall             # Invoke the kernel
    ret                 # Return to C++