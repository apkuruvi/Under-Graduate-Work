`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    05:15:33 03/06/2017 
// Design Name: 
// Module Name:    EXMEM 
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
module EXMEM(clk, rst,RegWrite_in,RegWrite_out,
				 MemtoReg_in,MemtoReg_out,
				 MemRead_in,MemRead_out,
				 Branch_in,Branch_out,
				 ALUresult_in,ALUresult_out,regdata_in,regdata_out,
				 zero_in,zero_out,add_in,add_out,mux_in,mux_out
				 );
	parameter WORD_SIZE = 32 ; 
	/*
	wire EX_RegWrite;
	wire EX_MemtoReg;
	wire EX_Branch;
	wire EX_MemRead;
	wire EX_alu_zero;
	wire [4:0] EX_mux_RegDst_result;
	wire [31:0] EX_ALU_result;
	wire [31:0] EX_register_ReadData2;
	wire [31:0] EX_AddALU_result;
*/
	input wire clk;
	input wire  rst;
	input wire RegWrite_in;
	output reg RegWrite_out;
	input wire MemtoReg_in;
	output reg MemtoReg_out;
	input wire MemRead_in;
	output reg MemRead_out;
	input wire Branch_in;
	output reg Branch_out;
	input wire [WORD_SIZE - 1:0] ALUresult_in;
	output reg [WORD_SIZE - 1:0] ALUresult_out;
	input wire [WORD_SIZE - 1:0]regdata_in;
	output reg[WORD_SIZE - 1:0]regdata_out;
	input wire zero_in;
	output reg zero_out;
	input wire [WORD_SIZE - 1:0] add_in;
	output reg [WORD_SIZE - 1:0]add_out;
	input wire [4:0] mux_in;
	output reg [4:0]mux_out;

always @ (posedge rst or posedge clk ) begin
	if ( rst ) begin

		RegWrite_out<= {1 {1'b0} };
		MemtoReg_out<= {1 {1'b0} };
		MemRead_out<= {1 {1'b0} };
		Branch_out<= {1 {1'b0} };
		ALUresult_out<= { WORD_SIZE {1'b0} };
		regdata_out<= { WORD_SIZE {1'b0} };
		zero_out <= {1 {1'b0} };
		add_out <= { WORD_SIZE {1'b0} };
		mux_out<= { 5{1'b0} };
	end else if ( clk ) begin
	RegWrite_out <= RegWrite_in ;
	MemtoReg_out <= MemtoReg_in;
	MemRead_out <= MemRead_in;
	Branch_out <= Branch_in;
	ALUresult_out <= ALUresult_in;
	regdata_out <= regdata_in;
	zero_out <= zero_in;
	add_out <= add_in;
	mux_out <= mux_in;

	end
end
endmodule
