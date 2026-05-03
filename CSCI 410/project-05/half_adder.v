module student_half_adder(input a, b, output sum, carry);
  xor u0 (sum, a, b);
  and u1 (carry, a, b);
endmodule
