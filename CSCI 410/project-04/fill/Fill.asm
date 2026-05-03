// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.

// Runs an infinite loop that listens to the keyboard input. 
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel. When no key is pressed, 
// the screen should be cleared.

(KEYBOARD)
    @KBD
    D=M        
    @BLACK
    D;JNE      
    @WHITE
    0;JMP        
(BLACK)
    @color
    M=-1       
    @DRAW
    0;JMP
(WHITE)
    @color
    M=0
    @DRAW
    0;JMP
(DRAW)
    @SCREEN
    D=A
    @current
    M=D 
    @8192
    D=A
    @counter
    M=D
(LOOP)
    @color
    D=M
    @current
    A=M
    M=D
    @current
    M=M+1 
    @counter
    MD=M-1 
    @LOOP
    D;JGT 
    @KEYBOARD
    0;JMP