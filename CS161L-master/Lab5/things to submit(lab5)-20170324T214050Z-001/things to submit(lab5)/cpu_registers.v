`timescale 1ns / 1ps

//////////////////////////////////////////////////////////////////////////////////

`define MAX_REG 32
`define WORD_SIZE 32

module cpu_registers(
	 clk,	
    rst , 
    reg_write ,
    read_register_1 ,
    read_register_2 , 
    write_register , 
    write_data    ,  
    read_data_1  ,   
    read_data_2  
    );

input wire clk, rst ,reg_write ; 
input wire [4:0] read_register_1; 
input wire [4:0] read_register_2 ; 

input wire [4:0]  write_register; 
input wire [`WORD_SIZE-1:0] write_data ; 

output wire [`WORD_SIZE-1:0] read_data_1 ; 
output wire [`WORD_SIZE-1:0] read_data_2 ; 

// -----------------------------------------------
// Memory Words and Locations  
// ----------------------------------------------- 	

reg [`WORD_SIZE-1:0] RFILE [`MAX_REG-1:0];
integer i;

// --------------------------------------
// Read statements 
// -------------------------------------- 

assign read_data_1 = RFILE[read_register_1] ; 
assign read_data_2 = RFILE[read_register_2] ; 
	
// ---------------------------------------------
// Write  
// --------------------------------------------- 

always @( posedge clk  )
begin 
	
       if ( rst )  begin 

	     for (i = 0; i < `MAX_REG; i = i +1) begin
			RFILE[i] <= { `WORD_SIZE {1'b0} } ; 
	     end 

       end else begin 

	     if (reg_write) begin 
			RFILE[write_register] <= write_data ;				
	     end 	

         end 
	
end 
	
endmodule