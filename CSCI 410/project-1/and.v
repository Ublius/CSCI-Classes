module student_and (input a, b, output out);
  wire n1;

  nand n (n1, a, b);
  nand n2 (out, n1, n1);
endmodule

/*
 * Using your single-bit solution above, create a 16-bit version here.
 */
module student_and16 (input [15:0] a, b, output [15:0] out);
    genvar i;
    generate
        for (i=0; i<16; i=i+1) begin
          student_and and1 (a[i], b[i], out[i]);
        end
    endgenerate
endmodule
