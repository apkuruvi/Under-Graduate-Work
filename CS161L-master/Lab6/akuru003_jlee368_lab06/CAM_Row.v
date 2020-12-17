`timescale 1ns / 1ps

module CAM_Row(
      input wire clk , 
      input wire rst , 
      input wire  we, 
      input wire [CAM_WIDTH-1 : 0]  search_word , 
      input wire [CAM_WIDTH-1 : 0]  dont_care_mask , 
      output wire row_match 
);
	parameter CAM_WIDTH = 8;
	parameter CAM_DEPTH = 8;
	
	
	// Three tests for bcam, tcam, and stcam
	// BCAM is uncommented, comment the tests you're not using
	
	// BCAM Test
	
	
	
	genvar i;
	wire [CAM_WIDTH:0] track;
	assign track[0] = 1'b1;
	generate
		for (i = 0; i < CAM_WIDTH; i = i + 1)
		begin: 
		test1	BCAM_Cell inst1
			(
				.clk(clk), 
				.rst(rst), 
				.we(we), 
				.cell_search_bit(search_word[i]), 
				.cell_dont_care_bit(dont_care_mask[i]), 
				.cell_match_bit_in(track[i]), 
				.cell_match_bit_out(track[i + 1])
			);
		end
	endgenerate
	assign row_match  = track[CAM_WIDTH];

	
	
	
	//TCAM Test
	
	/*
	
	genvar i;
	wire [CAM_WIDTH:0] track;
	assign track[0] = 1'b1;
	generate
		for (i = 0; i < CAM_WIDTH; i = i + 1)
		begin: 
		test1	TCAM_Cell inst1
			(
				.clk(clk), 
				.rst(rst), 
				.we(we), 
				.cell_search_bit(search_word[i]), 
				.cell_dont_care_bit(dont_care_mask[i]), 
				.cell_match_bit_in(track[i]), 
				.cell_match_bit_out(track[i + 1])
			);
		end
	endgenerate
	assign row_match  = track[CAM_WIDTH];

	*/
	
	
	 // STCAM Test

	/*
	
	genvar i;
	wire [CAM_WIDTH:0] track;
	assign track[0] = 1'b1;
	generate
		for (i = 0; i < CAM_WIDTH; i = i + 1)
		begin: 
		test1	STCAM_Cell inst1
			(
				.clk(clk), 
				.rst(rst), 
				.we(we), 
				.cell_search_bit(search_word[i]), 
				.cell_dont_care_bit(dont_care_mask[i]), 
				.cell_match_bit_in(track[i]), 
				.cell_match_bit_out(track[i + 1])
			);
		end
	endgenerate
	assign row_match  = track[CAM_WIDTH];

	*/
	
endmodule



