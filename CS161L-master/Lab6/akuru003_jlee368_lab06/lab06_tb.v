`timescale 1ns / 1ps

module lab06_tb;

	// Inputs
	reg clk;
	reg rst;
	reg [7:0] we_decoded_row_address;
	reg [7:0] search_word;
	reg [7:0] dont_care_mask;

	// Outputs
	wire [7:0] decoded_match_address;

	// Instantiate the Unit Under Test (UUT)
	CAM_Wrapper #(8,8) uut (
		.clk(clk), 
		.rst(rst), 
		.we_decoded_row_address(we_decoded_row_address), 
		.search_word(search_word), 
		.dont_care_mask(dont_care_mask), 
		.decoded_match_address(decoded_match_address)
	);

	// Clock block 

	initial begin 
	 clk = 0 ; 	
 
	 forever begin 
		#25 clk = ~clk;
	 end 
	 
	end
	
	initial begin
	    
		// **************************************************
		// We have to write values 
		// **************************************************
	 rst = 0 ; 
	 
	 rst = #25 1 ; 
	 rst = #25 0 ;	
		
		we_decoded_row_address = 8'h01  ; 
		search_word = 8'h01  ; 
		dont_care_mask = 8'h00  ; 
		#50 ;	
		
		we_decoded_row_address = 8'h02  ; 
		search_word = 8'h02  ; 
		dont_care_mask = 8'h00  ; 
		#50 ;	
		
		we_decoded_row_address = 8'h04  ; 
		search_word = 8'h04  ; 
		dont_care_mask = 8'h00  ; 
		#50 ;	

		we_decoded_row_address = 8'h08  ; 
		search_word = 8'h08  ; 
		dont_care_mask = 8'h00  ; 
		#50 ;	

		we_decoded_row_address = 8'h10  ; 
		search_word = 8'h11  ; 
		dont_care_mask = 8'h01  ; 
		#50 ;	

		we_decoded_row_address = 8'h20  ; 
		search_word = 8'h21  ; 
		dont_care_mask = 8'h01  ; 
		#50 ;	

		we_decoded_row_address = 8'h40  ; 
		search_word = 8'h41  ; 
		dont_care_mask = 8'h01  ; 
		#50 ;	

		we_decoded_row_address = 8'h80  ; 
		search_word = 8'h82  ; 
		dont_care_mask = 8'h02  ; 
		#50 ;	 
		
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// We have to search for values - Binary CAM 
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		dont_care_mask = 8'h00  ; 
		we_decoded_row_address  = 8'h00 ; 
		search_word  = 8'h01 ; 
		#50;

		search_word  = 8'h01 ; 
		#50;
		
		search_word  = 8'h02 ; 
		#50;

		search_word  = 8'h04 ; 
		#50;

		search_word  = 8'h08 ; 
		#50;

		search_word  = 8'h11 ; 
		#50;

		search_word  = 8'h21 ; 
		#50;
		
		search_word  = 8'h41 ; 
		#50;

		search_word  = 8'h81 ; 
		#50;
		
		search_word  = 8'h33 ; 
		#50;
		
		search_word  = 8'h81 ; 
		#50;
		
		search_word  = 8'h40 ; 
		#50;
		
		search_word  = 8'hFF ; 
		#50;
		
		search_word  = 8'h01 ; 
		#50;
		
		search_word  = 8'h82 ; 
		#50;
		
		search_word  = 8'h00 ; 
		#50;

	end
   


   
endmodule

