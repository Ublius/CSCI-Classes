// Read the section of the Verilog Primer (linked to on the Canvas page) about
// Muxlib. It will be helpful, though you can also use your multi-way
// multiplexers from project 1.

// If you use your student ram implementations for each of these your solution
// will probably time out. Instead, use the `fast_ram` parameterized module
// defined in the tests directory in place of your previous ram gate.
//
// For instance, in student_ram64, instead of creating 8 instances of
// student_ram8, create 8 instances of fast_ram #(8).

module student_ram8 (input [15:0] in, input load, input [2:0] address, output [15:0] out);
    wire [7:0] load_select;
    wire [15:0] ram_outputs [7:0];
    
    dmux #(8, 1) load_dmux (load, address, load_select);
    
    student_register ram0 (in, load_select[0], ram_outputs[0]);
    student_register ram1 (in, load_select[1], ram_outputs[1]);
    student_register ram2 (in, load_select[2], ram_outputs[2]);
    student_register ram3 (in, load_select[3], ram_outputs[3]);
    student_register ram4 (in, load_select[4], ram_outputs[4]);
    student_register ram5 (in, load_select[5], ram_outputs[5]);
    student_register ram6 (in, load_select[6], ram_outputs[6]);
    student_register ram7 (in, load_select[7], ram_outputs[7]);
    
    mux #(8, 16) read_mux (
        {ram_outputs[7], ram_outputs[6], ram_outputs[5], ram_outputs[4], ram_outputs[3], ram_outputs[2], ram_outputs[1], ram_outputs[0]},
        address,
        out
    );
endmodule

module student_ram64 (input [15:0] in, input load, input [5:0] address, output [15:0] out);
    wire [7:0] load_select;
    wire [15:0] ram_outputs [7:0];
    
    dmux #(8, 1) load_dmux (load, address[5:3], load_select);
    
    student_ram8 ram0 (in, load_select[0], address[2:0], ram_outputs[0]);
    student_ram8 ram1 (in, load_select[1], address[2:0], ram_outputs[1]);
    student_ram8 ram2 (in, load_select[2], address[2:0], ram_outputs[2]);
    student_ram8 ram3 (in, load_select[3], address[2:0], ram_outputs[3]);
    student_ram8 ram4 (in, load_select[4], address[2:0], ram_outputs[4]);
    student_ram8 ram5 (in, load_select[5], address[2:0], ram_outputs[5]);
    student_ram8 ram6 (in, load_select[6], address[2:0], ram_outputs[6]);
    student_ram8 ram7 (in, load_select[7], address[2:0], ram_outputs[7]);
    
    mux #(8, 16) read_mux (
        {ram_outputs[7], ram_outputs[6], ram_outputs[5], ram_outputs[4], ram_outputs[3], ram_outputs[2], ram_outputs[1], ram_outputs[0]},
        address[5:3],
        out
    );
endmodule

module student_ram512 (input [15:0] in, input load, input [8:0] address, output [15:0] out);
    wire [7:0] load_select;
    wire [15:0] ram_outputs [7:0];
    
    dmux #(8, 1) load_dmux (load, address[8:6], load_select);
    
    student_ram64 ram0 (in, load_select[0], address[5:0], ram_outputs[0]);
    student_ram64 ram1 (in, load_select[1], address[5:0], ram_outputs[1]);
    student_ram64 ram2 (in, load_select[2], address[5:0], ram_outputs[2]);
    student_ram64 ram3 (in, load_select[3], address[5:0], ram_outputs[3]);
    student_ram64 ram4 (in, load_select[4], address[5:0], ram_outputs[4]);
    student_ram64 ram5 (in, load_select[5], address[5:0], ram_outputs[5]);
    student_ram64 ram6 (in, load_select[6], address[5:0], ram_outputs[6]);
    student_ram64 ram7 (in, load_select[7], address[5:0], ram_outputs[7]);
    
    mux #(8, 16) read_mux (
        {ram_outputs[7], ram_outputs[6], ram_outputs[5], ram_outputs[4], ram_outputs[3], ram_outputs[2], ram_outputs[1], ram_outputs[0]},
        address[8:6],
        out
    );
endmodule

module student_ram4k (input [15:0] in, input load, input [11:0] address, output [15:0] out);
    wire [7:0] load_select;
    wire [15:0] ram_outputs [7:0];
    
    dmux #(8, 1) load_dmux (load, address[11:9], load_select);
    
    student_ram512 ram0 (in, load_select[0], address[8:0], ram_outputs[0]);
    student_ram512 ram1 (in, load_select[1], address[8:0], ram_outputs[1]);
    student_ram512 ram2 (in, load_select[2], address[8:0], ram_outputs[2]);
    student_ram512 ram3 (in, load_select[3], address[8:0], ram_outputs[3]);
    student_ram512 ram4 (in, load_select[4], address[8:0], ram_outputs[4]);
    student_ram512 ram5 (in, load_select[5], address[8:0], ram_outputs[5]);
    student_ram512 ram6 (in, load_select[6], address[8:0], ram_outputs[6]);
    student_ram512 ram7 (in, load_select[7], address[8:0], ram_outputs[7]);
    
    mux #(8, 16) read_mux (
        {ram_outputs[7], ram_outputs[6], ram_outputs[5], ram_outputs[4], ram_outputs[3], ram_outputs[2], ram_outputs[1], ram_outputs[0]},
        address[11:9],
        out
    );
endmodule

module student_ram16k (input [15:0] in, input load, input [13:0] address, output [15:0] out);
    wire [7:0] load_select;
    wire [15:0] ram_outputs [7:0];
    
    dmux #(8, 1) load_dmux (load, address[13:11], load_select);
    
    student_ram4k ram0 (in, load_select[0], address[10:0], ram_outputs[0]);
    student_ram4k ram1 (in, load_select[1], address[10:0], ram_outputs[1]);
    student_ram4k ram2 (in, load_select[2], address[10:0], ram_outputs[2]);
    student_ram4k ram3 (in, load_select[3], address[10:0], ram_outputs[3]);
    student_ram4k ram4 (in, load_select[4], address[10:0], ram_outputs[4]);
    student_ram4k ram5 (in, load_select[5], address[10:0], ram_outputs[5]);
    student_ram4k ram6 (in, load_select[6], address[10:0], ram_outputs[6]);
    student_ram4k ram7 (in, load_select[7], address[10:0], ram_outputs[7]);
    
    mux #(8, 16) read_mux (
        {ram_outputs[7], ram_outputs[6], ram_outputs[5], ram_outputs[4], ram_outputs[3], ram_outputs[2], ram_outputs[1], ram_outputs[0]},
        address[13:11],
        out
    );
endmodule
