module student_alu(input [15:0] x, y, input zx, nx, zy, ny, f, no, output [15:0] out, output zr, ng);
    wire [15:0] x_mod, y_mod;
    wire [15:0] x_after_zx, y_after_zy;
    wire [15:0] x_after_nx, y_after_ny;

    wire [15:0] add_out;
    wire [15:0] and_out;
    wire [15:0] f_out;
    
    assign x_after_zx = (zx) ? 16'b0000000000000000 : x;
    assign x_mod = (nx) ? ~x_after_zx : x_after_zx;
    
    assign y_after_zy = (zy) ? 16'b0000000000000000 : y;
    assign y_mod = (ny) ? ~y_after_zy : y_after_zy;
    
    student_add16 add_1 (x_mod, y_mod, add_out);
    assign and_out = x_mod & y_mod;
    assign f_out = (f) ? add_out : and_out;
    assign out = (no) ? ~f_out : f_out;
    
    assign zr = (out == 16'b0000000000000000) ? 1'b1 : 1'b0;
    assign ng = out[15];
endmodule
