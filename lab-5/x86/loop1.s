.text
.globl    _start
min = 0
max = 6
_start:
    mov     $min,%r15     # loop index

loop:
    mov     $1,%rax       # syscall number: 1 is write
    mov     $1,%rdi       # file descriptor: 1 is stdout
    lea     message(%rip),%rsi
    mov     $message_len,%rdx
    syscall

    inc     %r15          # increment the loop index
    cmp     $max,%r15     # see if we've hit the max
    jne     loop

    mov     $0,%rdi
    mov     $60,%rax
    syscall

.data
message:
	.ascii "Loop\n"
message_len = . - message
