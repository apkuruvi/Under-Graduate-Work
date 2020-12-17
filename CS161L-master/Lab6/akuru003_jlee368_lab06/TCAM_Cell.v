`timescale 1ns / 1ps
                                      // template from online vhdl tcam file
module TCAM_Cell(
	input wire clk,
	input wire rst,
	input wire we,
	input wire cell_search_bit,
	input wire cell_dont_care_bit,
	input wire cell_match_bit_in,
	output wire cell_match_bit_out
    );

reg temp;
always @*begin
	if (clk ==1)
	begin
		if(rst==1)             // if rst high, set to zero
		begin
			temp = 0;
		end
		else 
		begin 
			if(we ==1)              // if write enable high, set bit
			begin
				temp = cell_search_bit;
			end
		end
	end
end

// do xnor and and so you only set bit high when they're the same  plus OR so with  dont care bit
assign cell_match_bit_out = (((cell_search_bit ^~ temp  ) & cell_match_bit_in) | cell_dont_care_bit);
endmodule

