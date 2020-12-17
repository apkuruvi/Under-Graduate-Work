`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    08:26:29 03/06/2017 
// Design Name: 
// Module Name:    WB 
// Project Name: 
// Target Devices: 
// Tool versions: 
// Description: 
//
// Dependencies: 
//
// Revision: 
// Revision 0.01 - File Created
// Additional Comments: 
//
//////////////////////////////////////////////////////////////////////////////////
module WB(clk,rst,RegWrite_in,RegWrite_out,
			  MemtoReg_in,MemtoReg_out,WB_dataMem_in,
			  WB_dataMem_out, WB_ALU_in,WB_ALU_out,
			  WB_mux_in,WB_mux_out
			  );
	parameter WORD_SIZE = 32 ;
   input wire clk;
	input wire  rst;
	input wire RegWrite_in;
	output reg RegWrite_out;
	input wire MemtoReg_in;
	output reg MemtoReg_out;
	input wire [WORD_SIZE - 1:0] WB_dataMem_in;
	output reg [WORD_SIZE - 1:0] WB_dataMem_out;
	input wire [WORD_SIZE - 1:0] WB_ALU_in;
	output reg [WORD_SIZE - 1:0] WB_ALU_out;
	input wire [4:0] WB_mux_in;
	output reg [4:0]WB_mux_out;

always @ (posedge rst or posedge clk ) begin
	if ( rst ) begin
		RegWrite_out<= { 1{1'b0} };
		MemtoReg_out<= { 1{1'b0} };
		WB_dataMem_out <= { WORD_SIZE {1'b0} };
		WB_ALU_out <= { WORD_SIZE {1'b0} };
		WB_mux_out<= { 5{1'b0} };
	end else if ( clk ) begin
		RegWrite_out<=RegWrite_in;
		MemtoReg_out<=MemtoReg_in;
		WB_dataMem_out <= WB_dataMem_in;
		WB_ALU_out <= WB_ALU_in;
		WB_mux_out<= WB_mux_in;
	end
	end
	
endmodule
