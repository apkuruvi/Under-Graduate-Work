`timescale 1ns / 1ps

module control_unit_tb;

	// Inputs
	reg [5:0] instr_op;

	// Outputs
	wire reg_dst;
	wire branch;
	wire mem_read;
	wire mem_to_reg;
	wire [1:0] alu_op;
	wire mem_write;
	wire alu_src;
	wire reg_write;
	
	reg[0:0] reg_dst1;
	reg[0:0] branch1;
	reg[0:0] mem_read1;
	reg[0:0] mem_to_reg1;
	reg[1:0] alu_op1;
	reg[0:0] mem_write1;
	reg[0:0] alu_src1;
	reg[0:0] reg_write1;

	// Instantiate the Unit Under Test (UUT)
	control_unit uut (
		.instr_op(instr_op), 
		.reg_dst(reg_dst), 
		.branch(branch), 
		.mem_read(mem_read), 
		.mem_to_reg(mem_to_reg), 
		.alu_op(alu_op), 
		.mem_write(mem_write), 
		.alu_src(alu_src), 
		.reg_write(reg_write)
	);

	initial begin
		// Initialize Inputs
		instr_op = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here

	end
	
	initial begin
	#100
		instr_op = 6'd0;
		
		reg_dst1 = 1'd1;
		branch1 = 1'd0;;
		mem_read1 = 1'd0;
		mem_to_reg1 = 1'd0;
		alu_op1 = 2'd2;;
		mem_write1 = 1'd0;
		alu_src1 = 1'd0;
		reg_write1 = 1'd1;
	   #100 ; // Wait 
     
		$display("R-format opcode test ");
		if (reg_dst1 != reg_dst) $display  ("RegDst is wrong");
		if (branch1 != branch) $display  ("Branch is wrong");
		if (mem_read1 != mem_read) $display  ("Mem_read is wrong");
		if (mem_to_reg1 != mem_to_reg) $display  ("Mem_to_reg is wrong");
		if (alu_op1 != alu_op) $display  ("Alu_op is wrong");
		if (mem_write1 != mem_write1) $display  ("mem_write is wrong");
		if (alu_src1 != alu_src) $display  ("alu_src is wrong");
		if (reg_write1 != reg_write) $display  ("Reg_write is wrong");
	
	#100
		instr_op = 6'd35;
		
		reg_dst1 = 1'd0;
		branch1 = 1'd0;;
		mem_read1 = 1'd1;
		mem_to_reg1 = 1'd1;
		alu_op1 = 2'd0;;
		mem_write1 = 1'd0;
		alu_src1 = 1'd1;
		reg_write1 = 1'd1;
	   #100 ; // Wait 
     
		$display("1w opcode test ");
		if (reg_dst1 != reg_dst) $display  ("RegDst is wrong");
		if (branch1 != branch) $display  ("Branch is wrong");
		if (mem_read1 != mem_read) $display  ("Mem_read is wrong");
		if (mem_to_reg1 != mem_to_reg) $display  ("Mem_to_reg is wrong");
		if (alu_op1 != alu_op) $display  ("Alu_op is wrong");
		if (mem_write1 != mem_write1) $display  ("mem_write is wrong");
		if (alu_src1 != alu_src) $display  ("alu_src is wrong");
		if (reg_write1 != reg_write) $display  ("Reg_write is wrong");
	
	
	#100
		instr_op = 6'd43;
		
		reg_dst1 = 1'd0;
		branch1 = 1'd0;
		mem_read1 = 1'd0;
		mem_to_reg1 = 1'd0;
		alu_op1 = 2'd0;
		mem_write1 = 1'd1;
		alu_src1 = 1'd1;
		reg_write1 = 1'd0;
	   #100 ; // Wait 
     
		$display("sw opcode test ");
		if (reg_dst1 != reg_dst) $display  ("RegDst is wrong");
		if (branch1 != branch) $display  ("Branch is wrong");
		if (mem_read1 != mem_read) $display  ("Mem_read is wrong");
		if (mem_to_reg1 != mem_to_reg) $display  ("Mem_to_reg is wrong");
		if (alu_op1 != alu_op) $display  ("Alu_op is wrong");
		if (mem_write1 != mem_write1) $display  ("mem_write is wrong");
		if (alu_src1 != alu_src) $display  ("alu_src is wrong");
		if (reg_write1 != reg_write) $display  ("Reg_write is wrong");
	
	
	#100
		instr_op = 6'd4;
		
		reg_dst1 = 1'd0;
		branch1 = 1'd1;
		mem_read1 = 1'd0;
		mem_to_reg1 = 1'd0;
		alu_op1 = 2'd1;
		mem_write1 = 1'd0;
		alu_src1 = 1'd0;
		reg_write1 = 1'd0;
	   #100 ; // Wait 
     
		$display("Beq opcode test ");
		if (reg_dst1 != reg_dst) $display  ("RegDst is wrong");
		if (branch1 != branch) $display  ("Branch is wrong");
		if (mem_read1 != mem_read) $display  ("Mem_read is wrong");
		if (mem_to_reg1 != mem_to_reg) $display  ("Mem_to_reg is wrong");
		if (alu_op1 != alu_op) $display  ("Alu_op is wrong");
		if (mem_write1 != mem_write1) $display  ("mem_write is wrong");
		if (alu_src1 != alu_src) $display  ("alu_src is wrong");
		if (reg_write1 != reg_write) $display  ("Reg_write is wrong");
		
		
		#100
		instr_op = 6'd8;
		
		reg_dst1 = 1'd0;
		branch1 = 1'd0;
		mem_read1 = 1'd0;
		mem_to_reg1 = 1'd0;
		alu_op1 = 2'd3;
		mem_write1 = 1'd0;
		alu_src1 = 1'd1;
		reg_write1 = 1'd1;
	   #100 ; // Wait 
     
		$display("I type opcode test ");
		if (reg_dst1 != reg_dst) $display  ("RegDst is wrong");
		if (branch1 != branch) $display  ("Branch is wrong");
		if (mem_read1 != mem_read) $display  ("Mem_read is wrong");
		if (mem_to_reg1 != mem_to_reg) $display  ("Mem_to_reg is wrong");
		if (alu_op1 != alu_op) $display  ("Alu_op is wrong");
		if (mem_write1 != mem_write1) $display  ("mem_write is wrong");
		if (alu_src1 != alu_src) $display  ("alu_src is wrong");
		if (reg_write1 != reg_write) $display  ("Reg_write is wrong");
	
	end
	
      
endmodule











