.text
.globl _start
min = 0                   // starting value for the loop index
max = 33                  // loop exits when the index hits this number (loop condition is i<max)
_start:
    mov     x19, min      // initialize loop index

loop:

    mov     x20, x19         // copy loop index
    mov     x21, 10          // divisor
    udiv    x22, x20, x21    // x22 = x20 / x21 (tens digit)

    msub    x23, x22, x21, x20  // x23 = x20 - (x22 * x21) = remainder

    add     x22, x22, 48     // convert tens to ASCII
    add     x23, x23, 48     // convert ones to ASCII

    ldr     x1, =message

    strb    w22, [x1, tens_pos]
    strb    w23, [x1, ones_pos]

    mov     x0, 1            // file descriptor: 1 is stdout
    mov     x2, message_len
    mov     x8, 64           // syscall number: 64 is write
    svc     0

    add     x19, x19, 1      // increment the loop counter
    cmp     x19, max         // see if we've hit the max
    b.ne    loop

    mov     x0, 0            // set exit status to 0
    mov     x8, 93
    svc     0

.data
message:
        .ascii "Loop: ##\n"
message_len = . - message
tens_pos = 6
ones_pos = 7
