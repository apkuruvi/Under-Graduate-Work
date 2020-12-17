`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
module alu_control (
 input wire [1:0] alu_op ,
 input wire [5:0] instruction_5_0 ,
 output wire [3:0] alu_out
 );



reg[3:0] alu_out1;

// Combinatorial Block 

always @(alu_op,instruction_5_0) begin
if(alu_op == 2'd0)                   // lw and sw
	 alu_out1 = 4'd2;
else if(alu_op == 2'd1)                // beq
	 alu_out1 = 4'd6;
else if((alu_op == 2'd2) && (instruction_5_0 == 6'd32))   // add
	 alu_out1 = 4'd2;
else if((alu_op == 2'd2) && (instruction_5_0 == 6'd33))  // add u
	alu_out1 = 4'd2;
else if((alu_op == 2'd2) && (instruction_5_0 == 6'd34)) // sub
	alu_out1 = 4'd6;
else if((alu_op == 2'd2) && (instruction_5_0 == 6'd36)) // and
	alu_out1 = 4'd0;
else if((alu_op == 2'd2) && (instruction_5_0 == 6'd37)) // or
	alu_out1 = 4'd1;
else if((alu_op == 2'd2) && (instruction_5_0 == 6'd42)) // slt
	 alu_out1 = 4'd7;	
else if((alu_op == 2'd3) && (instruction_5_0 == 6'd8)) // addi
	alu_out1 = 4'd2;		 
else if((alu_op == 2'd2) && (instruction_5_0 == 6'd39))  // nor
	 alu_out1 = 4'd5;	// logic from circuit only on r type
end

assign alu_out = alu_out1;
endmodule
