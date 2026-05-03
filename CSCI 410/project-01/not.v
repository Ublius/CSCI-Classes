module student_not (input in, output out);
  nand n (out, in, in);
endmodule

/*
 * Using your single-bit solution above, create a 16-bit version here.
 */
module student_not16 (input [15:0] in, output [15:0] out);
    genvar i;
    generate
        for (i=0; i<16; i=i+1) begin
          student_not n1 (in[i], out[i]);
        end
    endgenerate
endmodule
