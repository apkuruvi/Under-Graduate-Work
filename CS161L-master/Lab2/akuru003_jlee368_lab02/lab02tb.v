`timescale 1ns / 1ps

module floattb;

	// Inputs
	reg clk ; 
	reg rst ; 
	reg [31:0] targetnumber;
	reg [4:0] fixpointpos;
	reg  opcode ; 
	
	wire [31:0] resultnumber;
	reg [31:0] R ;
	
	// ---------------------------------------------------
	// Instantiate the Unit Under Test (UUT)
	// --------------------------------------------------- 
	
	lab02 uut (
	   .clk( clk ) , 
      .rst(rst) , 		
		.targetnumber(targetnumber), 
		.fixpointpos(fixpointpos),
		.opcode(opcode) , 
		.result(resultnumber)
	);

  	initial begin 
	
	 clk = 0 ; rst = 1 ; #50 ; 
	 clk = 1 ; rst = 1 ; #50 ; 
	 clk = 0 ; rst = 0 ; #50 ; 
	 clk = 1 ; rst = 0 ; #50 ; 
	  
	 forever begin 
		clk = ~clk; #50 ; 
	 end 
	 
	end 
	
	initial begin

	opcode = 1'b0; 
	fixpointpos = 4'b0000;
	targetnumber = { 32'h0 } ; 
	R            = { 32'h0 } ; 
	
#100 ; 		
		
		// Working with 25.25 = 11001_01 	
		
		opcode = 1'b0; 
		fixpointpos = 4'b0010;
		targetnumber = { {24'h000000}, {8'b011001_01}  } ; 
		R = 32'b0_10000011_10010100000000000000000;

#100;
$display (" TC 01 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 1 ....... %h %h ", R,  resultnumber ); 
		
		opcode = 1'b1; 
		fixpointpos = 4'b0010;
		targetnumber = 32'b0_10000011_10010100000000000000000;
		R = { {24'h000000}, {8'b011001_01}  } ;  

#100; 
$display (" TC 02 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 2 ....... %h %h ", R,  resultnumber ); 
		
		// ---------------------------------------------------------
		// Working with 12.625 
		// ---------------------------------------------------------
		
		opcode = 1'b0; 
		fixpointpos = 4'b0011;
		targetnumber = { {24'h000000}, {8'b01100_101}  } ; 
		R = 32'b0_10000010_10010100000000000000000;

#100;
$display (" TC 03 ....... %h %h ", R,  resultnumber ); 
	   if ( R != resultnumber ) $display ("Failed TC 3 ....... %h %h ", R,  resultnumber ); 

		opcode = 1'b1; 
		fixpointpos = 4'b0011;
		targetnumber = 32'b0_10000010_10010100000000000000000; 
		R = { {24'h000000}, {8'b01100_101}  } ;  

#100;
$display (" TC 04 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 4 ....... %h %h ", R,  resultnumber ); 
		
		// Working with 6.3125
		
		opcode = 1'b0; 
		fixpointpos = 4'b0100;
		targetnumber = { {24'h000000}, {8'b0110_0101}  } ; 
		R = 32'b0_10000001_10010100000000000000000;

#100;
$display (" TC 05 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 5 ....... %h %h ", R,  resultnumber ); 

		opcode = 1'b1; 
		fixpointpos = 4'b0100;
		targetnumber = 32'b0_10000001_10010100000000000000000;
		R = { {24'h000000}, {8'b0110_0101}  } ;  
		

#100; 
$display (" TC 06 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 6 ....... %h %h ", R,  resultnumber ); 
		

		// Working with 1.5
		
		opcode = 1'b0; 
		fixpointpos = 4'b0111;
		targetnumber = { {24'h000000}, {8'b1100_0000}  } ; 
		R = 32'b0_01111111_10000000000000000000000;

#100;
$display (" TC 07 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 7 ....... %h %h ", R,  resultnumber ); 


		opcode = 1'b1; 
		fixpointpos = 4'b0111;
		targetnumber = 32'b0_01111111_10000000000000000000000;
		R = { {24'h000000}, {8'b1100_0000}  } ;  

#100; 
$display (" TC 08 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 8 ....... %h %h ", R,  resultnumber ); 
	
		// Working with 512.0 
		
		opcode = 1'b0; 
		fixpointpos = 5'b0001;
		targetnumber = { {20'h00000}, {12'h400}  } ; 
		R = 32'b0_10001000_00000000000000000000000;

#100;
$display (" TC 09 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 9 ....... %h %h ", R,  resultnumber ); 

		opcode = 1'b1; 
		fixpointpos = 5'b0001;
		targetnumber = 32'b0_10001000_00000000000000000000000 ;
		R = { {20'h00000}, {12'h400}  } ;  

#100;
$display (" TC 10 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 10 ....... %h %h ", R,  resultnumber ); 
		
		// ------------------------------------------------
		// Working with 0.75	
		// ------------------------------------------------ 

		opcode = 1'b0; 
		fixpointpos = 5'b0010;
		targetnumber = { {20'h00000}, {12'h003}  } ; 
		R = 32'b0_0111_1110_10000000000000000000000;
		
#100;
$display (" TC 11 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 11 ....... %h %h ", R,  resultnumber ); 
		
		opcode = 1'b1; 
		fixpointpos = 5'b0010;
		targetnumber = 32'b0_0111_1110_10000000000000000000000; 
		R = { {20'h00000}, {12'h003}  } ; 

#100;
$display (" TC 12 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 12 ....... %h %h ", R,  resultnumber ); 
		
		// --------------------------------------
		// Working with 1056 - 4  
		// --------------------------------------
		
		opcode = 1'b0;  
		targetnumber = { {16'h0000}, {16'b0100_0010_0000_0000}} ; 
		fixpointpos = 5'b0100;
		R = 32'b0_10001001_00001000000000000000000 ; 
		
#100;
$display (" TC 13 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 13 ....... %h %h ", R,  resultnumber ); 
		
		opcode = 1'b1;  
		targetnumber = 32'b0_10001001_00001000000000000000000 ; 
		fixpointpos = 5'b0100;
		R = { {16'h0000}, {16'b0100_0010_0000_0000}} ; 
	
#100;
$display (" TC 14 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 14 ....... %h %h ", R,  resultnumber ); 
		
		// --------------------------------------
		// Working with 1056 (Pos 7)   (8.25)
		// --------------------------------------
		
		opcode = 1'b0;  
		targetnumber = { {16'h0000}, {16'b0000_0100_0010_0000}} ; 
		fixpointpos = 5'b00111;
		R = 32'b0_10000010_00001000000000000000000; 
		
#100;
$display (" TC 15 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 15 ....... %h %h ", R,  resultnumber ); 
		
		opcode = 1'b1;  
		targetnumber = 32'b0_10000010_00001000000000000000000; 
		fixpointpos = 5'b00111;
		R = { {16'h0000}, {16'b0000_0100_0010_0000}} ; 

#100;
$display (" TC 16 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 16 ....... %h %h ", R,  resultnumber ); 
		
		// --------------------------------------
		// Working with f(32,20) of -1408576 is (-1) 
		// --------------------------------------
		
		opcode = 1'b0;  
		targetnumber = $unsigned( 32'b11111111_11110000_00000000_00000000 )  ; 
		fixpointpos = 5'h14;
		R = 32'b1_01111111_00000000000000000000000; 
		
#100;
$display (" TC 17 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 17 ....... %h %h ", R,  resultnumber ); 
		
		opcode = 1'b1;  
		R = $unsigned( 32'b11111111_11110000_00000000_00000000 )  ; 
		fixpointpos = 5'h14;
		targetnumber = 32'b1_01111111_00000000000000000000000; 
		
#100;
$display (" TC 18 ....... %h %h ", R,  resultnumber ); 
		if ( R != resultnumber ) $display ("Failed TC 18 ....... %h %h ", R,  resultnumber ); 
		
	end
      
endmodule
