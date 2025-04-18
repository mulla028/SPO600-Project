.text
.globl    _start
min = 0
max = 6
_start:
    mov     $min,%r15     ## loop index

loop:
    mov     %r15,%rax     ## copy loop index
    add     $48,%rax      ## convert to ASCII (48 = '0')

    ## Load the address of message
    lea     message(%rip),%rsi

    ## Store the ASCII digit in the message at digit_pos
    mov     %al,digit_pos(%rsi)

    mov     $1,%rax       ## syscall number: 1 is write
    mov     $1,%rdi       ## file descriptor: 1 is stdout
    mov     $message_len,%rdx
    syscall

    inc     %r15          ## increment the loop index
    cmp     $max,%r15     ## see if we've hit the max
    jne     loop

    mov     $0,%rdi       ## set exit status to 0
    mov     $60,%rax      ## exit is syscall #60
    syscall

.data
message:
        .ascii "Loop: #\n"
message_len = . - message
digit_pos = 6

