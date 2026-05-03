module student_dmux (input in, input sel, output a, b);
    wire tmp;

    student_not not0 (sel, tmp);
    student_and and0 (in, tmp, a);
    student_and and1 (in, sel, b);
endmodule

/*
 * Using your solution above, create a version that has four outputs.
 */
module student_dmux4way (input in, input [1:0] sel, output a, b, c, d);
    wire ab;
    wire cd;

    student_dmux dmux0 (in, sel[1], ab, cd);

    student_dmux dmux1 (ab, sel[0], a, b);
    student_dmux dmux2 (cd, sel[0], c, d);
endmodule

/*
 * Using your four-way solution above, create a version that has eight outputs.
 */
module student_dmux8way (input in, input [2:0] sel, output a, b, c, d, e, f, g, h);
    wire abcd;
    wire efgh;

    student_dmux dmux0 (in, sel[2], abcd, efgh);

    student_dmux4way dmux1 (abcd,  sel[1:0], a, b, c, d);
    student_dmux4way dmux2 (efgh, sel[1:0], e, f, g, h);
endmodule
