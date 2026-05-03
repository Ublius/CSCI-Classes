// Read the textbook on this one. Especially chapter 4.
//
// One particularly sticky bit is the 'a' bit for C-instructions indicates the
// source is the inM not ARegister, despite the letter seeming to indicate the
// opposite.

module student_cpu (
    input signed [15:0] inM,
    input [15:0] instruction,
    input reset,
    output signed [15:0] outM,
    output writeM,
    output [14:0] address, pc,
    output signed [15:0] ARegister, DRegister // used so test scripts can see what's going on
);

    wire [15:0] outALU, inARegister, yALU, pcFull;
    wire zr, ng;
    wire isA = ~instruction[15];
    wire isC = instruction[15];
    
    assign inARegister = isA ? instruction : outALU;
    assign address = ARegister[14:0];
    
    student_register ARegister_inst (inARegister, isA | instruction[5], ARegister);
    student_register DRegister_inst (outALU, isC & instruction[4], DRegister);

    assign yALU = instruction[12] ? inM : ARegister;

    student_alu ALU (DRegister, yALU, instruction[11], instruction[10], instruction[9], instruction[8], instruction[7], instruction[6], outALU, zr, ng);
    
    assign outM = outALU;
    assign writeM = isC & instruction[3];

    wire jump = isC & ((instruction[0] & ~ng & ~zr) | (instruction[1] & zr) | (instruction[2] & ng));

    student_pc pc_inst (ARegister, reset, jump, ~jump, pcFull);
    assign pc = pcFull[14:0];

endmodule
