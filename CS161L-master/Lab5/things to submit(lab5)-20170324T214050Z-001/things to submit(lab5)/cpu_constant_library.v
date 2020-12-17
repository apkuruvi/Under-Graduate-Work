`timescale 1ns / 1ps

// --------------------------------------
// Opcodes 
// --------------------------------------

`define OPCODE_R_TYPE       6'b000000
`define OPCODE_LOAD_WORD    6'b100011
`define OPCODE_STORE_WORD   6'b101011
`define OPCODE_BRANCH_EQ    6'b000100
`define OPCODE_ADDI         6'b001000


// --------------------------------------
// Parameters
// --------------------------------------

`define ALU_AND  		 4'b0000
`define ALU_OR   		 4'b0001
`define ALU_ADD  		 4'b0010
`define ALU_SUBTRACT   4'b0110
`define ALU_LESS_THAN  4'b0111
`define ALU_NOR        4'b1100
  
`define FUNCT_AND   		6'b100100
`define FUNCT_OR    		6'b100101
`define FUNCT_ADD   		6'b100000
`define FUNCT_SUBTRACT 	6'b100010
`define FUNCT_LESS_THAN 6'b101010
`define FUNCT_NOR 		6'b100111
