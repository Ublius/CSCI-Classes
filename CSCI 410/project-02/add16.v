module student_add16(input [15:0] a, b, output [15:0] out);
    wire [15:0] carry;

    student_full_adder fa0 (a[0], b[0], 1'b0, out[0], carry[0]);

    genvar i;
    generate
        for (i=1; i<16; i=i+1) begin
            student_full_adder fa1 (a[i], b[i], carry[i-1], out[i], carry[i]);
        end
    endgenerate
endmodule
