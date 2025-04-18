.text
.globl    _start
min = 0
max = 33
_start:
    mov     $min,%r15     ## loop index

loop:
    mov     %r15,%rax     ## copy loop index to rax for division
    mov     $0,%rdx       ## clear rdx for division
    mov     $10,%rcx      ## divisor
    div     %rcx          ## divide rax by rcx, quotient in rax, remainder in rdx

    add     $48,%rax      ## convert tens to ASCII
    add     $48,%rdx      ## convert ones to ASCII

    lea     message(%rip),%rsi

    mov     %al,tens_pos(%rsi)
    mov     %dl,ones_pos(%rsi)

    mov     $1,%rax
    mov     $1,%rdi
    mov     $message_len,%rdx
    syscall

    inc     %r15          ## increment the loop index
    cmp     $max,%r15     ## see if we've hit the max
    jne     loop

    mov     $0,%rdi       ## set exit status to 0
    mov     $60,%rax      ## exit is syscall g60
    syscall

.data
message:
	.ascii "Loop: ##\n"
message_len = . - message
tens_pos = 6
ones_pos = 7
