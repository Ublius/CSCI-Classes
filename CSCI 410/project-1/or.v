module student_or (input a, b, output out);
  wire na, nb; 

  student_not not1 (a, na);
  student_not not2 (b, nb);
  nand n(out, na, nb);
endmodule

/*
 * Using your single-bit solution above, create a 16-bit version here.
 */
module student_or16 (input [15:0] a, b, output [15:0] out);
    genvar i;
    generate
        for (i=0; i<16; i=i+1) begin
          student_or or1 (a[i], b[i], out[i]);
        end
    endgenerate
endmodule

/*
 * Using your single-bit solution above, create gate that uses a vector of
 * booleans as an input.
 * If any of the bits of `in` are `1`, the output should be `1`.
 * 
 * There are several ways of approaching this, but I recommend using a for
 * loop in some way to reduce repeated code.
 */
module student_or8way (input [7:0] in, output out);
    genvar i;
    wire [7:0] temp;
    student_or or2 (in[0], in[1], temp[1]);
    generate
        for (i=2; i<8; i=i+1) begin
            // Use your student_or gate here for each bit in the input vector
            student_or or3 (temp[i-1], in[i], temp[i]);
        end
    endgenerate
    assign out = temp[7];
endmodule
