`timescale 1ns / 1ps

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

module lab02(
input wire clk,
input wire rst,
input wire[31:0] targetnumber,
input wire[4:0] fixpointpos ,
input wire opcode ,
output reg[31:0] result );

reg [31:0] floatresult ;
reg [31:0] fixresult ;


reg [31:0] check;          // holding val             
reg [0:0] sign;             // check sign of number
reg [8:0] pos;            // holding position
integer i;                 // for loop
reg[7:0] check2;           // adding offset
reg[31:0] shift;           // holds final answer before transfering it to wire to output
reg[7:0] bits;             // holds exponent


reg[7:0] j;
reg [7:0] check3;          // holding val             
reg [0:0] sign1;             // check sign of number
reg [8:0] pos1;            // holding position
reg[31:0] check4;           // adding offset
reg[31:0] shift1;           // holds final answer before transfering it to wire to output
reg[7:0] bits1;             // holds exponent
reg[7:0] onetwoseven;
reg[7:0] border;
reg[7:0] temp;
reg[7:0] loop;
reg[31:0] shift3;
// -------------------------------------------

always @(*) begin 
sign = 0;
check = 0;
check2 = 0;
pos = 0;
shift = 0;
bits = 0;
onetwoseven = 0;
sign1 = 0;
check3 = 0;
check4 = 0;
pos1 = 0;
shift1 = 0;
bits1 = 0;
border = 0;
temp = 127;
loop = 0;
j = 0;
case (opcode) 
 
3'd0 : begin   // fixed  to floating - saved in shift - sent to floatresult - sent to result 
			 sign = targetnumber[31];
			 if(sign == 1)
					check = (~targetnumber) + 1;
			 else
					check = targetnumber;
			 for(i = 0; i < 32; i = i + 1) begin
					if(check[i] == 1)
						pos = i;
			 end
			 
			 if(pos == fixpointpos)
			 begin
			 bits = $signed(temp);         // hard code 127
			 shift = check << (32 - pos);  // clear all bits on left including that one
			 shift = shift >> 9;           // clear all bits for sign and exp
			 end
			 else if(pos > fixpointpos)
			 begin
			 check2 = pos - fixpointpos;
			 bits = $signed(temp) + $signed(check2);   // for exp
			 shift = check << (32 - pos);              // clear all bits in prep for sign and exp
			 shift = shift >> 9;                       // clear all bits for sign and exp
			 end
			 else
			 begin
			 check2 = fixpointpos - pos;
			 check2 = check2 * -1;
			 bits = $signed(temp) + $signed(check2);   // for exp
			 shift = check << (32 - pos);              // clear all bits on left including that one
			 shift = shift >> 9;                       // clear all bits for sign and exp
			 end
			 shift[31] = sign;                         // hard code rest of bits
			 shift[30] = bits[7];
			 shift[29] = bits[6];
			 shift[28] = bits[5];
			 shift[27] = bits[4];
			 shift[26] = bits[3];
			 shift[25] = bits[2];
			 shift[24] = bits[1];
			 shift[23] = bits[0];
			 floatresult = shift;
		 end  
		 
3'd1 : begin 
		  sign1 = targetnumber[31];       
		  bits1 = targetnumber[23+:8];  
		  onetwoseven = 127;
		  check3 = $signed(bits1) - $signed(onetwoseven);   // have sign and have how many bits shifted 
		  check4 = targetnumber;
		  shift3 = 0;
		  if(check3 > 0)                         // we shifted <<
		  begin
				shift3 = targetnumber[0+:23];      // get bits 0-22
				shift3[23] = 1;                    // append 1
				border = 24 - check3 - 1;
				if(border > fixpointpos)
				shift3 = shift3 >> (border - fixpointpos);
				else
				shift3 = shift3 << (fixpointpos - border);
	   	shift1 = shift3;
			if(sign1 == 1)                // negative - two's complement
		   begin
		      shift1 = ~shift1;
		      shift1 = shift1 + 1;
		  end
			fixresult = shift1;
		  end
		  if(check3 < 0)
		  begin
		   shift3 = targetnumber[0+:23];   // get bits 0-22
		   shift3[23] = 1;                 // append 1
		   border = 24 + check3;
				if(border > fixpointpos)
				shift3 = shift3 >> (border - fixpointpos);
				else
				shift3 = shift3 << (fixpointpos - border);
	   	shift1 = shift3;
			if(sign1 == 1)
		   begin
		     shift1 = ~shift1;
		     shift1 = shift1 + 1;
		  end
			fixresult = shift1;
		  end
		  if(check3 == 0)
		  begin
			shift3 = targetnumber[0+:23];   // get bits 0-22
			shift3[23] = 1;                 // append 1
			border = 23;
				if(border > fixpointpos)
				shift3 = shift3 >> (border - fixpointpos);
				else
				shift3 = shift3 << (fixpointpos - border);
	     shift1 = shift3;
		  if(sign1 == 1)
		  begin
		     shift1 = ~shift1;
		     shift1 = shift1 + 1;
		  end
		  fixresult = shift1;
		  end
		 end 
endcase
end 


// Sequential Block 
always @( posedge clk ) begin
 if ( rst == 1'b1 ) begin 
	result <= 'd0; 
 end else begin 
 	 result <= opcode == 1 ? fixresult : floatresult ;
 end 

end 


endmodule











