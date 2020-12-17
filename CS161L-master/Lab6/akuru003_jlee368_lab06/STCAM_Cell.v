`timescale 1ns / 1ps

module STCAM_Cell(
   input wire clk,
	input wire rst,
	input wire we,
	input wire cell_search_bit,
	input wire cell_dont_care_bit,
	input wire cell_match_bit_in,
	output wire cell_match_bit_out
	 );
reg temp;
reg temp1;

always @*begin
	if (clk ==1)
	begin
		if(rst==1)                  //if rst high, set to zero for both
		begin
			temp = 0;
			temp1 = 0;
		end
		else 
		begin
			if(we == 1)                  // if write enable high, set bits
			begin
				temp = cell_search_bit;
				temp1 = cell_dont_care_bit;
			end
		end
	end
end

// do xnor and and so you only set bit high when they're the same  plus OR so with second temp1
assign cell_match_bit_out = (((cell_search_bit ^~ temp  ) & cell_match_bit_in) | temp1);
endmodule

