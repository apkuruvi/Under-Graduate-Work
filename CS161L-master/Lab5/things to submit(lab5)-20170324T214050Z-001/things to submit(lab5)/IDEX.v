`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    00:11:05 03/06/2017 
// Design Name: 
// Module Name:    IDEX 
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
module IDEX(clk, rst ,add4_in,add4_out,// control_in,control_out
				read_data1_int,read_data1_out,
				read_data2_in,read_data2_out,
				sext_in,sext_out,inst15_in,inst15_out,inst20_in,inst20_out,
				alu_in,alu_out,RegDst_in,RegDst_out,Branch_in,Branch_out,
				MemRead_in,MemRead_out,
				MemtoReg_in,MemtoReg_out,ALUSrc_in,ALUSrc_out,
				RegWrite_in,RegWrite_out);
	parameter WORD_SIZE = 32 ; 
	
		input wire clk, rst;

		input wire [WORD_SIZE - 1:0] add4_in;
		output reg [WORD_SIZE - 1:0] add4_out;
		input wire [WORD_SIZE - 1:0] read_data1_int;
		output reg [WORD_SIZE - 1:0] read_data1_out;	
		input wire [WORD_SIZE - 1:0] read_data2_in;
		output reg [WORD_SIZE - 1:0] read_data2_out;		
		input wire [WORD_SIZE - 1:0] sext_in;
		output reg [WORD_SIZE - 1:0] sext_out;		
		input wire [4:0]inst15_in;
		output reg [4:0]inst15_out ;
		input wire [4:0]inst20_in;
		output reg [4:0]inst20_out ;
		input wire [1:0]alu_in;
		output reg [1:0]alu_out ;	
		input wire RegDst_in;
		output reg RegDst_out ;
		input wire Branch_in;
		output reg Branch_out ;
		input wire MemRead_in;
		output reg MemRead_out ;
		input wire ALUSrc_in;
		output reg ALUSrc_out ;
		input wire RegWrite_in;
		output reg RegWrite_out ;
		input wire MemtoReg_in;
		output reg MemtoReg_out ;
	
always @ (posedge rst or posedge clk ) begin
	if ( rst ) begin
		add4_out <= { WORD_SIZE {1'b0} };
		read_data1_out <= { WORD_SIZE {1'b0} };
		read_data2_out<= { WORD_SIZE {1'b0} };
		sext_out<= { WORD_SIZE {1'b0} };
		inst15_out<= { 5{1'b0} };
		inst20_out<= { 5{1'b0} };
		//control_out<= { 7 {1'b0} };
		alu_out<= { 2 {1'b0} };
		RegDst_out <= {1 {1'b0} };
		Branch_out <= {1 {1'b0} };
		MemRead_out <= {1 {1'b0} };
		ALUSrc_out <= {1 {1'b0} };
		RegWrite_out <= {1 {1'b0} };
		MemtoReg_out <= {1 {1'b0} };
	end else if ( clk ) begin
		add4_out <= add4_in;
		read_data1_out <= read_data1_int;
		read_data2_out <= read_data2_in;
		sext_out <= sext_in;
		alu_out<= alu_in;
		//fcontrol_out<= control_in;
		inst15_out<= inst15_in;
		inst20_out<= inst20_in;
		RegDst_out <= RegDst_in;
		Branch_out <= Branch_in;
		MemRead_out <= MemRead_in;
		ALUSrc_out <= ALUSrc_in;
		RegWrite_out <= RegWrite_in;
		MemtoReg_out <= MemtoReg_in;
	end
	end
	
endmodule 
