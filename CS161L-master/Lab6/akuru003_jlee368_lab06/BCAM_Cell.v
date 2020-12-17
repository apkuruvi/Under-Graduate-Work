`timescale 1ns / 1ps
                              // template from online vhdl bcam file
module BCAM_Cell(
	input wire clk,
	input wire rst,
	input wire we,               // write enable
	input wire cell_search_bit, 
	input wire cell_dont_care_bit,
	input wire cell_match_bit_in,
	output wire cell_match_bit_out
    );
	 
reg temp;
always @*begin
	if (clk == 1)
	begin
		if(rst == 1)                     // if rst is high,  set to 0
		begin
			temp = 0;
		end
		else
		begin
			if(we == 1)                 // if write enable high, assign bit
			begin
				temp = cell_search_bit;
			end
		end
	end
end

// do xnor and and so you only set bit high when they're the same                                              
assign cell_match_bit_out = ((cell_search_bit ^~temp ) & cell_match_bit_in);  
endmodule

