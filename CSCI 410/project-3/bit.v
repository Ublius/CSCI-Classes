module student_bit (input in, load, output out);
    wire signal;
    wire [1:0] mux_in;
    
    assign mux_in = {out, in};
    
    mux mx (mux_in, load, signal);
    dff ff (signal, out);
endmodule