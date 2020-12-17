`timescale 1ns / 1ps

`define ALU_AND  4'b0000
`define ALU_OR   4'b0001
`define ALU_ADD  4'b0010
`define ALU_SUBTRACT 4'b0110
`define ALU_LESS_THAN 4'b0111
`define ALU_NOR 4'b1100
`define WORD_SIZE 32 

module alu( alu_control_in,  channel_a_in , channel_b_in , zero_out , alu_result_out  );

// ---------------------------------------------------------
// Input output declarations 
// --------------------------------------------------------- 

input wire [3:0] alu_control_in ;  
input wire [`WORD_SIZE-1:0] channel_a_in ; 
input wire [`WORD_SIZE-1:0] channel_b_in ; 

output reg zero_out ; 
output reg [`WORD_SIZE-1:0] alu_result_out ; 

reg [`WORD_SIZE-1:0] temp ; 

// ---------------------------------------------------------
// Parameters 
// --------------------------------------------------------- 

always @(*) 

begin

	case (alu_control_in)   // R Type Instruction 
	
		`ALU_AND :      temp = channel_a_in & channel_b_in ; 
		`ALU_OR :       temp = channel_a_in | channel_b_in ; 
		`ALU_ADD :      temp = channel_a_in + channel_b_in ; 
		`ALU_SUBTRACT : temp = channel_a_in - channel_b_in ; 
		`ALU_NOR :  temp = ~ (channel_a_in | channel_b_in) ; 
		
		`ALU_LESS_THAN :  
		
						if (channel_a_in < channel_b_in) begin 
							temp = { `WORD_SIZE {1'b1} } ; 
						end 
						
						else begin 
							 temp = { `WORD_SIZE {1'b0} } ; 
						end  
		
		default : temp = { `WORD_SIZE {1'b0} } ; 
	
	endcase 

	// Final results 
	
	alu_result_out = temp ; 
	zero_out = (temp == 0) ? 1 : 0 ; 

end 

endmodule
