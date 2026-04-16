module student_register (input [15:0] in, input load, output [15:0] out);
    wire [15:0] signal;
    assign signal = load ? in : out;
    
    dff #(16) ff (signal, out);
endmodule
