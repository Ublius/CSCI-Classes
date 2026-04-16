module student_full_adder(input a, b, c, output sum, carry);
    wire sum1, carry1, carry2;

    student_half_adder h1 (a, b, sum1, carry1);
    student_half_adder h2 (sum1, c, sum, carry2);
    or u0 (carry, carry1, carry2);
endmodule
