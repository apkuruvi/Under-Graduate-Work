`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

`define NULL 0
`define MAX_REG 256
`define WORD_SIZE 32 

module cpumemory(
	clk	,
   rst  ,
   instr_read_address ,
   instr_instruction  ,
   data_mem_write  ,   
   data_address   ,    
   data_write_data ,    
   data_read_data  );

input	clk	; 
input rst  ; 
input instr_read_address  ; 
output instr_instruction  ; 

input  data_mem_write  ;   
input  data_address  ;     
input  data_write_data   ;  
output data_read_data ; 

wire clk , rst  ; 
wire [7:0]  instr_read_address  ; 
wire  [`WORD_SIZE-1:0] instr_instruction  ; 

wire data_mem_write  ;   
wire [7:0]  data_address  ;     
wire [`WORD_SIZE-1:0] data_write_data   ;  
wire [`WORD_SIZE-1:0] data_read_data ; 

// ------------------------------------------
// Init memory 
// ------------------------------------------
	
reg [`WORD_SIZE-1:0] buff [`MAX_REG-1:0];

initial begin 
	$readmemb("init.coe", buff,0,255);
end 

// ------------------------------------------
// Read and Write block 
// ------------------------------------------ 

assign instr_instruction = buff[instr_read_address];
assign  data_read_data = buff[data_address];
	
always @(posedge clk )
begin 
	
	if (data_mem_write) begin 
		buff[data_address] = data_write_data;
	end 
	
end 


endmodule
