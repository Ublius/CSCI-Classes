module student_xor (input a, b, output out);
  wire na, nb;
  wire t1, t2;

  student_not not1 (a, na);
  student_not not2 (b, nb);

  nand (t1, a, nb);
  nand (t2, na, b);
  nand (out, t1, t2);
endmodule
<<<<<<< HEAD

/*
 * Using your single-bit solution above, create a 16-bit version here.
 */
module student_xor16 (input [15:0] a, b, output [15:0] out);
    genvar i;
    generate
        for (i=0; i<16; i=i+1) begin
          student_xor(a[i], b[i], out);
        end
    endgenerate
endmodule
=======
>>>>>>> 1cbacaff7cc900281b655ca7ebeb56fc448408db
