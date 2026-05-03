module student_inc16(input [15:0] in, output [15:0] out);
    student_add16 add_1 (in , 16'b0000000000000001, out);
endmodule
