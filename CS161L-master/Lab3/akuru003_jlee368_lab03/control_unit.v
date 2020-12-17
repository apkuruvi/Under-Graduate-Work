`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
module control_unit (
 input wire [5:0] instr_op ,
 output wire reg_dst ,
 output wire branch ,
 output wire mem_read ,
 output wire mem_to_reg ,
 output wire [1:0] alu_op ,
 output wire mem_write ,
 output wire alu_src ,
 output wire reg_write
 );

reg[0:0] reg_dst1;
reg[0:0] branch1;
reg[0:0] mem_read1;
reg[0:0] mem_to_reg1;
reg[1:0] alu_op1;
reg[0:0] mem_write1; 
reg[0:0] alu_src1;
reg[0:0] reg_write1;
// Combinatorial Block 


always @(instr_op) begin
if(instr_op == 6'd0) // addu is same as add, nor is same too because they're r type
begin
	 reg_dst1 = 1;
	 alu_src1 = 0;
	 mem_to_reg1 = 0;
	 reg_write1 = 1;
	 mem_read1 = 0;
	 mem_write1 = 0;
	 branch1 = 0;
	 alu_op1[1] = 1;
	 alu_op1[0] = 0;
end
else if(instr_op == 6'd8) // addi - i type
begin
	 reg_dst1 = 0;
	 alu_src1 = 1;
	 mem_to_reg1 = 0;
	 reg_write1 = 1;
	 mem_read1 = 0;
	 mem_write1 = 0;
	 branch1 = 0;
	 alu_op1[1] = 1;
	 alu_op1[0] = 1;
end
else if(instr_op == 6'd35)
begin
	 reg_dst1 = 0;
	 alu_src1 = 1;
	 mem_to_reg1 = 1;
	 reg_write1 = 1;
	 mem_read1 = 1;
	 mem_write1 = 0;
	 branch1 = 0;
	 alu_op1[1] = 0;
	 alu_op1[0] = 0;
end
else if(instr_op == 6'd43)
begin
	 reg_dst1 = 1'bx;
	 alu_src1 = 1;
	 mem_to_reg1 = 1'bx;
	 reg_write1 = 0;
	 mem_read1 = 0;
	 mem_write1 = 1;
	 branch1 = 0;
	 alu_op1[1] = 0;
	 alu_op1[0] = 0;
end
else if(instr_op == 6'd4)
begin
	 reg_dst1 = 1'bx;
	 alu_src1 = 0;
	 mem_to_reg1 = 1'bx;
	 reg_write1 = 0;
	 mem_read1 = 0;
	 mem_write1 = 0;
	 branch1 = 1;
	 alu_op1[1] = 0;
	 alu_op1[0] = 1;
end
end

assign reg_dst = reg_dst1;
assign alu_src = alu_src1;
assign mem_to_reg = mem_to_reg1;
assign reg_write = reg_write1;
assign mem_read = mem_read1;
assign mem_write = mem_write1;
assign branch = branch1;
assign alu_op = alu_op1;


endmodule




