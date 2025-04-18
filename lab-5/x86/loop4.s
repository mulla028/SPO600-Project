.text
.globl    _start
min = 0
max = 33
_start:
    mov     $min,%r15

loop:
    mov     %r15,%rax     ## copy loop index to rax for division
    mov     $0,%rdx       ## clear rdx for division
    mov     $10,%rcx      ## divisor
    div     %rcx          ## divide rax by rcx, quotient in rax, remainder in rdx


    mov     %rax,%r12     ## store tens digit in r12

    add     $48,%rax      ## convert tens to ASCII
    add     $48,%rdx      ## convert ones to ASCII

    lea     message(%rip),%rsi

    cmp     $0,%r12       ## compare tens digit (original value) with 0
    jne     print_both

    mov     $space,%r12   ## space character
    mov     %r12b,tens_pos(%rsi)  ## replace tens position with space
    jmp     print_ones    ## skip storing tens digit again

print_both:
    mov     %al,tens_pos(%rsi)

print_ones:
    mov     %dl,ones_pos(%rsi)

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
	.ascii "Loop: ##\n"
message_len = . - message
tens_pos = 6
ones_pos = 7
space = 32
