.text
.globl _start
min = 0
max = 6
_start:
    mov     x19, min      // initialize loop index

loop:
    mov     x20, x19         // copy loop index
    add     x20, x20, 48     // convert to ASCII (48 = '0')

    ldr     x1, =message

    strb    w20, [x1, digit_pos]

    mov     x0, 1            // 1 is stdout
    mov     x2, message_len
    mov     x8, 64           // syscall number: 64 is write
    svc     0

    add     x19, x19, 1      // increment the loop counter
    cmp     x19, max         // see if we've hit the max
    b.ne    loop

    mov     x0, 0            // set exit status to 0
    mov     x8, 93           // exit is syscall #93
    svc     0                // invoke syscall

.data
message:
    .ascii "Loop: #\n"    // # will be replaced with the actual digit
message_len = . - message
digit_pos = 6
