`timescale 1ns / 1ps

module myalutb;

	// Parameters 
	parameter NUMBITS = 32; 

	// Inputs
	reg clk ;
	reg reset ; 
	reg [NUMBITS-1:0] A;
	reg [NUMBITS-1:0] B;
	reg [3:0] opcode;
	
	wire [NUMBITS+3:0] result;
	wire carryout;
	wire overflow;
	wire zero;

	// Result 
	reg [NUMBITS+3:0] R ;

	// Instantiate the Unit Under Test (UUT)
	BCD_alu #(32) uut (
	.clk(clk),
		.reset(reset) ,  
		.A(A), 
		.B(B), 
		.opcode(opcode), 
		.result(result), 
		.carryout(carryout), 
		.overflow(overflow), 
		.zero(zero)
	);

initial begin 
	
	 clk = 0 ; reset = 1 ; #50 ; 
	 clk = 1 ; reset = 1 ; #50 ; 
	 clk = 0 ; reset = 0 ; #50 ; 
	 clk = 1 ; reset = 0 ; #50 ; 
		 
	 forever begin 
		clk = ~clk; #50 ; 
	 end 
	 
	end 


	initial begin
	
		// ---------------------------------------------- 
		// Working with unsigned operations additions 
		// ----------------------------------------------
		
		opcode = 4'b1000; 
		
		A = 32'h0010_0000;
		B = 32'h0000_0001; 
		R = 36'h00010_0001; 
			
		#300;
		
		$display("TC10 Unsigned adds %b , %b ", R, result);
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		A = 32'h0000_9999;
		B = 32'h0000_0002; 
		R = 36'h00001_0001; 
			
		#300;

		$display("TC11 Unsigned adds ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		A = 32'h0000_0099;
		B = 32'h0000_0099; 
		R = 36'h00000_0198; 
			
		#300;
		
		$display("TC12 Unsigned adds ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		A = 32'h9999_9999;
		B = 32'h0000_0001; 
		R = 36'h10000_0000 ; 
			
		#300;
		
		$display("TC13 Unsigned adds ");
		if (R != result) $display  ("Result is wrong %b %b", R, result);
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b1) $display ("Carryout is wrong");
		
		A = 32'h9999_9998;
		B = 32'h0000_0001; 
		R = 36'h09999_9999 ; 
			
		#300;
		
		$display("TC14 Unsigned adds ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		A = 32'h0000_2000;
		B = 32'h0002_0000; 
	   R = 36'h00002_2000; 
			
		#300;
		
		$display("TC15 Unsigned adds ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		
		// ---------------------------------------------- 
		// Working with unsigned operations 
		// Substraction 
		// ----------------------------------------------
		
	   opcode = 4'b1001; 
		
		A = 32'h0000_0998;
		B = 32'h0000_0997; 
		R = 36'h00000_0001; 
		
		#300; 
		
		$display("TC21 Unsigned subs ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		
		A = 32'h0000_0999;
		B = 32'h0000_0008; 
		R = 36'h00000_0991; 
		
		#300; 
		
		$display("TC22 Unsigned subs ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		A = 32'h9000_0999;
		B = 32'h9000_0008; 
		R = 36'h00000_0991; 
		
		#300; 
		
		$display("TC23 Unsigned subs ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		A = 32'h9999_9999;
		B = 32'h0000_0008; 
		R = 36'h09999_9991; 
		
		#300;
		
		$display("TC24 Unsigned subs ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		// -----------------------------------------
		// -----------------------------------------
		// Working with signed additions 
		// ----------------------------------------- 
		// ----------------------------------------- 
		
		opcode = 4'b1100 ; 
		
		A = 32'h0000_0098; // +998
		B = 32'h0000_0001; // +1
		R = 36'h00000_0099; // +999 
		
		#300; 
				
		$display("TC30 Signed adds ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
				
	
		A = 32'h0000_0998; // +998
		B = 32'h0100_0001; // +100_0001
		R = 36'h00100_0999; // +100_0999 
	
		#300; 
	
		$display("TC31 Signed adds ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		
		A = 32'h1000_0998; // -998
		B = 32'h0000_0998; // +998
		R = 36'h00000_0000; // 0 

		#300; 

		
		$display("TC32 Signed adds %b ", result );
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b1) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		
		A = 32'h0000_0996; // +996 
		B = 32'h1000_0997; // -997
		R = 36'h10000_0001; // -1 

		#300; 
		
		$display("TC33 Signed adds ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong"); 
		
		A = 32'h1999_1111; //  
		B = 32'h1999_1111; // 
		R = 36'h11998_2222; //  

		#300; 
		
		$display("TC34 Signed adds ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong"); 
		
		A = 32'h1999_1111; //  
		B = 32'h0999_1111; // 
		R = 36'h00000_0000; //  

		#300; 
		
		$display("TC35 Signed adds ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b1) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		
		// -------------------------------------------
		// -------------------------------------------
		// Working with signed numbers substractions 
		// ------------------------------------------- 
		// -------------------------------------------
		
		opcode = 4'b1101 ; 
		
		A = 32'h0000_1998; // +1998
		B = 32'h0000_1998; // +1998
		R = 36'h00000_0000; // 0  
		
		#300; 
		
		$display("TC40 Signed Subs ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b1) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");

		A = 32'h0000_0998; // +998
		B = 32'h0000_0997; // +997
		R = 36'h00000_0001; // 1 
		
		#300; 
		
		$display("TC41 Signed Subs ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		A = 32'h1000_0098; //  -98
		B = 32'h0000_0098; //  +98
		R = 36'h10000_0196; // -196 
		
		#300; 
		
		$display("TC42 Signed Subs ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		A = 32'h1000_0098; //  -98
		B = 32'h1000_0098; //  -98
		R = 36'h00000_0000; // 0 
		
		#300; 
		
		$display("TC43 Signed Subs ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b1) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		A = 32'h0000_0098; //  +98
		B = 32'h0000_0098; //  +98
		R = 36'h00000_0000; // 0 
		
		#300; 
		
		$display("TC44 Signed Subs ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b1) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		
		A = 32'h1999_9999; //  -999 9999
		B = 32'h0999_9999; // +999 9999
		R = 36'h11999_9998;//(1)1999 9998
		
		#300; 
		
		$display("TC45 Signed Subs ");
		if (R != result) $display  ("Result is wrong");
		if (zero != 1'b0) $display ("Zero  is wrong");
		if (carryout != 1'b0) $display ("Carryout is wrong");
		
		
	end
      
endmodule

