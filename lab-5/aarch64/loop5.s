.text
.globl _start
min = 0
max = 33
_start:
    mov     x19, min      // initialize loop index

loop:
    mov     x20, x19         // copy loop index
    mov     x21, 16          // divisor
    udiv    x22, x20, x21    // x22 = x20 / x21 (high nibble)

    msub    x23, x22, x21, x20  // x23 = x20 - (x22 * x21) = remainder

    cmp     x22, 10          // compare with 10
    b.ge    high_alpha       // if >= 10, convert to A-F

    add     x22, x22, 48     // convert 0-9 to ASCII '0'-'9'
    b       high_done

high_alpha:
    add     x22, x22, 55     // convert 10-15 to ASCII 'A'-'F'

high_done:
    cmp     x23, 10          // compare with 10
    b.ge    low_alpha        // if >= 10, convert to A-F

    add     x23, x23, 48     // convert 0-9 to ASCII '0'-'9'
    b       low_done

low_alpha:
    add     x23, x23, 55     // convert 10-15 to ASCII 'A'-'F'

low_done:
    ldr     x1, =message

    cmp     x22, 48          // compare with ASCII '0'
    b.ne    print_both       // if not '0', print both digits

    mov     x24, space       // space character
    strb    w24, [x1, hex1_pos]  // replace high nibble position with space
    b       print_low        // skip storing high nibble again

print_both:
    strb    w22, [x1, hex1_pos]

print_low:
    strb    w23, [x1, hex2_pos]

    mov     x0, 1
    mov     x2, message_len
    mov     x8, 64
    svc     0

    add     x19, x19, 1      // increment the loop counter
    cmp     x19, max         // see if we've hit the max
    b.ne    loop

    mov     x0, 0            // set exit status to 0
    mov     x8, 93           // exit is syscall #93
    svc     0

.data
message:
	.ascii "Loop: ##\n"
message_len = . - message
hex1_pos = 6
hex2_pos = 7
space = 32
