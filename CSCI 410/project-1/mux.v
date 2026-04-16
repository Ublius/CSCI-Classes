module student_mux (input a, b, input sel, output out);
  wire nsel;
  wire b_and_sel, a_and_nsel;
  wire t1;

  student_not not1 (sel, nsel);
  
  student_and and1 (b, sel, b_and_sel);
  student_and and2 (a, nsel, a_and_nsel);

  student_or or1 (b_and_sel, a_and_nsel, out);
endmodule

/*
 * Using your single-bit solution above, create a 16-bit version here.
 */
module student_mux16 (input [15:0] a, b, input sel, output [15:0] out);
    genvar i;
    generate
        for (i=0; i<16; i=i+1) begin
            student_mux mux0 (a[i], b[i], sel, out[i]);
        end
    endgenerate
endmodule

/*
 * Using your 16-bit solution above, create a version that accepts four inputs.
 */
module student_mux4way16 (input [15:0] a, b, c, d, input [1:0] sel, output [15:0] out);
    wire [15:0] ab;
    wire [15:0] cd;

    student_mux16 mux0 (a, b, sel[0], ab);
    student_mux16 mux1 (c, d, sel[0], cd);
    student_mux16 mux2 (ab, cd, sel[1], out);
endmodule

/*
 * Using your four-way solution above, create a version that accepts eight inputs.
 */
module student_mux8way16 (input [15:0] a, b, c, d, e, f, g, h, input [2:0] sel, output [15:0] out);
  wire [15:0] abcd;
  wire [15:0] efgh;

  student_mux4way16 mux0 (a, b, c, d, sel[1:0], abcd);
  student_mux4way16 mux1 (e, f, g, h, sel[1:0], efgh);
  student_mux16  mux2 (abcd, efgh, sel[2], out);
endmodule
