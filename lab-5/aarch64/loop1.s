.text
.globl _start
min = 0
max = 6
_start:
    mov     x19, min      // initialize loop index

loop:
    mov     x0, 1         // 1 is stdout
    ldr     x1, =message  // pointer to message
    mov     x2, message_len
    mov     x8, 64        // syscall number: 64 is write
    svc     0

    add     x19, x19, 1   // increment the loop counter
    cmp     x19, max      // see if we've hit the max
    b.ne    loop

    mov     x0, 0         // set exit status to 0
    mov     x8, 93        // exit is syscall #93
    svc     0
.data
message:
    .ascii "Loop\n"
message_len = . - message
~
~
