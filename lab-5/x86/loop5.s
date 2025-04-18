.text
.globl    _start
min = 0
max = 33
_start:
    mov     $min,%r15     ## loop index

loop:
    mov     %r15,%rax     ## copy loop index to rax for division
    mov     $0,%rdx       ## clear rdx for division
    mov     $16,%rcx      ## divisor
    div     %rcx          ## divide rax by rcx, quotient in rax, remainder in rdx

    mov     %rax,%r12     ## store high nibble in r12

    cmp     $10,%rax      ## compare with 10
    jge     high_alpha    ## if >= 10, convert to A-F

    add     $48,%rax      ## convert 0-9 to ASCII '0'-'9'
    jmp     high_done

high_alpha:
    add     $55,%rax      ## convert 10-15 to ASCII 'A'-'F'

high_done:
    cmp     $10,%rdx      ## compare with 10
    jge     low_alpha     ## if >= 10, convert to A-F

    add     $48,%rdx      ## convert 0-9 to ASCII '0'-'9'
    jmp     low_done

low_alpha:
    add     $55,%rdx      ## convert 10-15 to ASCII 'A'-'F'

low_done:
    lea     message(%rip),%rsi

    cmp     $0,%r12       ## compare high nibble (original value) with 0
    jne     print_both    ## if not zero, print both digits

    mov     $space,%r12   ## space character
    mov     %r12b,hex1_pos(%rsi)  ## replace high nibble position with space
    jmp     print_low

print_both:
    mov     %al,hex1_pos(%rsi)

print_low:
    mov     %dl,hex2_pos(%rsi)

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
hex1_pos = 6
hex2_pos = 7
space = 32
