`timescale 1ns / 1ps

module CAM_Array( input wire clk , 
      input wire rst , 
      input wire [CAM_DEPTH-1 : 0]  we_decoded_row_address , 
      input wire [CAM_WIDTH-1 : 0]  search_word , 
      input wire [CAM_WIDTH-1 : 0]  dont_care_mask , 
      output wire [CAM_WIDTH-1 : 0] decoded_match_address 
		);
	parameter CAM_WIDTH = 8;
	parameter CAM_DEPTH = 8;
	genvar i;                                 // can't use integer because using in a for loop generate
	wire [CAM_WIDTH - 1:0] track;
	generate
		for (i = 0; i < CAM_WIDTH; i = i + 1)
		begin: 
		Row CAM_Row inst
			(
				.clk(clk), 
				.rst(rst), 
				.we(we_decoded_row_address[i]), 
				.search_word(search_word), 
				.dont_care_mask(dont_care_mask), 
				.row_match(track[i])
			);
		end
	endgenerate
	
	assign decoded_match_address = track;
endmodule

