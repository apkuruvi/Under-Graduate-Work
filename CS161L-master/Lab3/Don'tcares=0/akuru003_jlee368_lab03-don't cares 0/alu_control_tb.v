`timescale 1ns / 1ps

module alu_control_tb;

	// Inputs
	reg [1:0] alu_op;
	reg [5:0] instruction_5_0;

	// Outputs
	wire [3:0] alu_out;
   reg[3:0] alu_out1;
	
	// Instantiate the Unit Under Test (UUT)
	alu_control uut (
		.alu_op(alu_op), 
		.instruction_5_0(instruction_5_0), 
		.alu_out(alu_out)
	);

	initial begin
		// Initialize Inputs
		alu_op = 0;
		instruction_5_0 = 0;

		// Wait 100 ns for global reset to finish
		#100;
        
		// Add stimulus here

	end
	
	
	initial begin
	#100
	
		alu_op = 2'd0;
		instruction_5_0 = 6'd0;
      alu_out1 = 4'd2;
		#100 ; // Wait 
		$display("Alu_op 00 opcode test ");
		if (alu_out1 != alu_out) $display  ("Alu_out is wrong");
		
		
		
		#100
	
		alu_op = 2'd1;
		instruction_5_0 = 6'd0;
      alu_out1 = 4'd6;
		#100 ; // Wait 
		$display("Alu_op 01 opcode test ");
		if(alu_out1 != alu_out) $display  ("Alu_out is wrong");
		
		
		#100
	
		alu_op = 2'd2;
		instruction_5_0 = 6'd32;
      alu_out1 = 4'd2;
		#100 ; // Wait 
		$display("Alu_op 10 add opcode test ");
		if (alu_out1 != alu_out) $display  ("Alu_out is wrong");
		
		
		#100
	
		alu_op = 2'd2;
		instruction_5_0 = 6'd34;
      alu_out1 = 4'd6;
		#100 ; // Wait 
		$display("Alu_op 10 subtract opcode test ");
		if (alu_out1 != alu_out) $display  ("Alu_out is wrong");
		
		#100
	
		alu_op = 2'd2;
		instruction_5_0 = 6'd36;
      alu_out1 = 4'd0;
		#100 ; // Wait 
		$display("Alu_op 10 and opcode test ");
		if (alu_out1 != alu_out) $display  ("Alu_out is wrong");
		
		#100
	
		alu_op = 2'd2;
		instruction_5_0 = 6'd37;
      alu_out1 = 4'd1;
		#100 ; // Wait 
		$display("Alu_op 10 or opcode test ");
		if (alu_out1 != alu_out) $display  ("Alu_out is wrong");
		
		#100
	
		alu_op = 2'd2;
		instruction_5_0 = 6'd42;
      alu_out1 = 4'd7;
		#100 ; // Wait 
		$display("Alu_op 10 set on less than opcode test ");
		if (alu_out1 != alu_out) $display  ("Alu_out is wrong");
		
		#100
	
		alu_op = 2'd2;
		instruction_5_0 = 6'd39;
      alu_out1 = 4'd5;
		#100 ; // Wait 
		$display("Alu_op 10 NOR opcode test ");
		if (alu_out1 != alu_out) $display  ("Alu_out is wrong");
		
		#100
	
		alu_op = 2'd3;
		instruction_5_0 = 6'd8;
      alu_out1 = 4'd2;
		#100 ; // Wait 
		$display("Alu_op 11 addi opcode test ");
		if (alu_out1 != alu_out) $display  ("Alu_out is wrong");
		
		
		#100
	
		alu_op = 2'd2;
		instruction_5_0 = 6'd33;
      alu_out1 = 4'd2;
		#100 ; // Wait 
		$display("Alu_op 10 addu opcode test ");
		if (alu_out1 != alu_out) $display  ("Alu_out is wrong");
		
	end
      
endmodule









