`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date:    23:23:12 03/05/2017 
// Design Name: 
// Module Name:    IFID_REG 
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
module IFID_REG(clk, rst, add4_in, add4_out,inst_in,inst_out  );
	parameter WORD_SIZE = 32 ; 
	
	input wire clk, rst;
	input wire [WORD_SIZE - 1:0] add4_in;
	output reg [WORD_SIZE - 1:0] add4_out;
input wire [WORD_SIZE - 1:0] inst_in;
	output reg [WORD_SIZE - 1:0] inst_out;		
	
always @ (posedge rst or posedge clk ) begin
	if ( rst ) begin
		add4_out <= { WORD_SIZE {1'b0} };
		inst_out <= { WORD_SIZE {1'b0} };
	end else if ( clk ) begin
		add4_out <= add4_in;
		inst_out <= inst_in;
	end
	end
	
endmodule 
