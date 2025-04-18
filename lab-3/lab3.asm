; Lab-3 SPO600 Winter Term 
; Professor Chris Tyler
; Number Guessing Game by Amir Mullagaliev
; Simple game where you try to guess a random number

; ROM routines from the emulator docs
define CHROUT $ffd2
define CHRIN $ffcf
define SCINIT $ff81

; Memory locations to store our variables
define TARGET $30    ; The number to guess
define GUESS $31     ; Player's guess
define ATTEMPTS $32  ; Number of tries
define TEMP1 $33     ; Temp storage for calculations
define TEMP2 $34     ; Another temp variable

* = $0600    ; Program starts here

    JSR SCINIT                 
    JSR TEST_GRAPHICS   ; Makes sure graphics work before we start
    
    ; Game setup
    LDA #0              
    STA ATTEMPTS        
    
    ; Get a random number between 1-99
    LDA $FE             ; Random byte from memory
    AND #$7F            ; Make sure it's positive
    CMP #100            
    BCC SAVE_TARGET     ; If < 100, we're good
    LSR                 ; Otherwise divide by 2 (could've used modulo but this is faster)
SAVE_TARGET:
    STA TARGET          

    ; Show game title
    LDA #$47            ; G
    JSR CHROUT
    LDA #$55            ; U
    JSR CHROUT
    LDA #$45            ; E
    JSR CHROUT
    LDA #$53            ; S
    JSR CHROUT
    LDA #$53            ; S
    JSR CHROUT
    LDA #$0D            ; New line
    JSR CHROUT
    
; Main gameplay loop
GAME_LOOP:
    JSR INPUT_GUESS
    
    INC ATTEMPTS
    JSR DISPLAY_ATTEMPTS
    
    ; Check if guess matches target 
    LDA GUESS
    CMP TARGET
    BEQ CORRECT_GUESS
    BCC TOO_LOW
    
    ; Too high
    LDA #$48            ; H
    JSR CHROUT
    LDA #$49            ; I
    JSR CHROUT
    LDA #$0D            
    JSR CHROUT
    
    LDA #$02            ; RED - this is hot/high
    JSR FILL_SCREEN
    JMP GAME_LOOP
    
TOO_LOW:
    LDA #$4C            ; L
    JSR CHROUT
    LDA #$4F            ; O
    JSR CHROUT
    LDA #$0D            
    JSR CHROUT
    
    LDA #$06            ; BLUE - this is cold/low
    JSR FILL_SCREEN
    JMP GAME_LOOP
    
CORRECT_GUESS:
    LDA #$57            ; W
    JSR CHROUT
    LDA #$49            ; I
    JSR CHROUT
    LDA #$4E            ; N
    JSR CHROUT
    LDA #$21            ; !
    JSR CHROUT
    LDA #$0D            
    JSR CHROUT
    
    LDA #$05            ; GREEN for winning
    JSR FILL_SCREEN
    
    ; Play again prompt
    LDA #$0D           
    JSR CHROUT
    LDA #$59            ; Y
    JSR CHROUT
    LDA #$2F            ; /
    JSR CHROUT
    LDA #$4E            ; N
    JSR CHROUT
    LDA #$3F            ; ?
    JSR CHROUT
    LDA #$20            ; Space
    JSR CHROUT
    
; Wait for Y or N
WAIT_YN:
    JSR CHRIN
    CMP #$59            ; Y
    BEQ RESTART
    CMP #$79            ; y (lowercase also works)
    BEQ RESTART
    CMP #$4E            ; N
    BEQ END_GAME
    CMP #$6E            ; n
    BEQ END_GAME
    JMP WAIT_YN         ; Invalid key, keep waiting
    
RESTART:
    JSR SCINIT          
    JMP $0600           ; Go back to start
    
END_GAME:
    BRK                 

; Quick test to make sure graphics work
; Had issues with this before so added a test
TEST_GRAPHICS:
    LDA #$01            ; WHITE flash
    JSR FILL_SCREEN
    
    ; Short delay (could use a timer but this works)
    LDX #$FF
    LDY #$FF
DELAY_LOOP:
    DEX
    BNE DELAY_LOOP
    DEY
    BNE DELAY_LOOP
    
    LDA #$00            ; BLACK (clear screen)
    JSR FILL_SCREEN
    RTS

; This fills the screen with color in register A
; Took me a while to figure out the right addresses!
FILL_SCREEN:
    LDX #0
    LDY #0
FILL_LOOP:
    STA $0200,X         ; Graphics memory starts at $0200
    STA $0300,X         ; And continues through $05FF
    STA $0400,X
    STA $0500,X
    INX
    BNE FILL_LOOP
    RTS

; Shows how many attempts we've made
; Handles double digits (10+)
DISPLAY_ATTEMPTS:
    LDA #$0D            
    JSR CHROUT
    LDA #$41            ; A
    JSR CHROUT
    LDA #$3A            ; :
    JSR CHROUT
    LDA #$20            ; Space
    JSR CHROUT
    
    ; Need special handling for 10+
    LDA ATTEMPTS
    CMP #10
    BCC SINGLE_DIGIT    ; If < 10, just show the number
    
    ; For numbers 10+, split into two digits
    LDA ATTEMPTS
    LDX #0              ; X = tens digit
DIVIDE_LOOP:
    CMP #10
    BCC DIGIT_READY     ; Remainder is ones digit
    SBC #10             
    INX                 
    JMP DIVIDE_LOOP     
    
DIGIT_READY:
    STA TEMP1           ; Save ones digit
    
    TXA                 ; Show tens digit
    CLC
    ADC #$30            ; Convert to ASCII
    JSR CHROUT
    
    LDA TEMP1           ; Show ones digit
    CLC
    ADC #$30           
    JSR CHROUT
    JMP NEWLINE
    
SINGLE_DIGIT:
    CLC
    ADC #$30            ; Convert to ASCII
    JSR CHROUT
    
NEWLINE:
    LDA #$0D           
    JSR CHROUT
    RTS

; Gets player's guess from keyboard
INPUT_GUESS:
    LDA #$3F            ; ?
    JSR CHROUT
    LDA #$20            ; Space
    JSR CHROUT
    
GET_FIRST_DIGIT:
    JSR CHRIN
    CMP #$30            ; Must be 0-9
    BCC GET_FIRST_DIGIT 
    CMP #$3A            
    BCS GET_FIRST_DIGIT 
    
    JSR CHROUT          ; Echo back what they typed
    SEC
    SBC #$30            ; Convert ASCII to actual number
    STA GUESS           
    
GET_SECOND_DIGIT:
    JSR CHRIN
    CMP #$0D            ; Enter = done inputting
    BEQ END_INPUT       
    
    CMP #$30            ; Must be 0-9
    BCC GET_SECOND_DIGIT 
    CMP #$3A            
    BCS GET_SECOND_DIGIT 
    
    JSR CHROUT          
    PHA                 ; Save digit temporarily
    
    ; First digit becomes tens place (×10)
    ; Could multiply by 10, but shifts are faster
    LDA GUESS
    ASL                 ; ×2
    STA TEMP2           
    ASL                 ; ×4
    ASL                 ; ×8
    CLC
    ADC TEMP2           ; +2 = ×10
    STA GUESS           
    
    PLA                 ; Get second digit back
    SEC
    SBC #$30            ; ASCII to number
    CLC
    ADC GUESS           ; Add to tens place
    STA GUESS           
    
WAIT_FOR_ENTER:
    JSR CHRIN
    CMP #$0D            ; Wait for Enter key
    BNE WAIT_FOR_ENTER  
    
END_INPUT:
    LDA #$0D            
    JSR CHROUT
    RTS
