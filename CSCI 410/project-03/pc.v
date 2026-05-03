module student_pc (input [15:0] in, input reset, load, inc, output [15:0] out);
    wire [15:0] current;
    wire [15:0] inc_out;
    wire [15:0] after_inc;
    wire [15:0] after_load;
    wire [15:0] after_reset;

    student_register reg0 (after_reset, 1'b1, current);
    assign inc_out = current + 16'd1;

    mux #(2,16) mux_inc ({current, inc_out}, inc, after_inc);
    mux #(2,16) mux_load ({after_inc, in}, load, after_load);
    mux #(2,16) mux_reset ({after_load, 16'd0}, reset, after_reset);

    assign out = current;
endmodule