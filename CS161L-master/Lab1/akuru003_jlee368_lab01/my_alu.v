`timescale 1ns / 1ps

module my_alu #( parameter NUMBITS = 32 ) ( 
input wire clk, 
input wire reset , 
input wire [NUMBITS-1:0] A, 
input wire [NUMBITS-1:0] B, 
input wire [2:0] opcode, 
output reg [NUMBITS-1:0] result, 
output reg zero,
output reg carryout,
output reg overflow);

// Combinatorial Block 
reg [NUMBITS-1:0] c_result;
reg [NUMBITS:0] check; 
reg [NUMBITS:0] check2; 
reg [NUMBITS:0] check3;
reg [NUMBITS:0] check4;
always @(*) begin 
 c_result = 'd0; 
 check2 = 'd0;
 check3 = 'd0;
 check4 = 'd0;
 case (opcode) 
 //unsigned add works fine 
 3'd0 : begin 
			  check = A + B;
			  if(check[NUMBITS] == 1)
					carryout = 1;
			  else
					carryout = 0;
			  c_result = A + B;
			  overflow = 1'd0;
			end  
 
   // signed add works fine 
	3'd1 : begin
			c_result = $signed(A) + $signed(B);			
			carryout = 1'd0;
			check2 = 2**NUMBITS;	 
			check2 = check2 / 2; 
			check2 = check2 * -1;	 
			check3 = (((2**NUMBITS) - 1) / 2); 
			check4 = $signed(A) + $signed(B); 
			if(($signed(check4)) < ($signed(check2)))
				overflow = 1;
			else if(($signed(check4)) > ($signed(check3)))
				overflow = 1;
			else
				overflow = 0;
			end
			
	//unsigned sub	works fine	
	3'd2 : begin  
			c_result = A - B; 
			if(A < B)
				carryout = 1;
			else	
				carryout = 0;
			overflow = 0;
			end
			
	//signed sub works fine
	3'd3 : begin 
			c_result = $signed(A) - $signed(B);
			carryout = 1'd0;
			check2 = 2**NUMBITS;	 
			check2 = check2 / 2; 
			check2 = check2 * -1;	 
			check3 = (((2**NUMBITS) - 1) / 2);
			check4 = $signed(A) - $signed(B); 
			if(($signed(check4)) < ($signed(check2)))
				overflow = 1;
			else if(($signed(check4)) > ($signed(check3)))
				overflow = 1;
			else
				overflow = 0;
			end
			
	3'd4 : begin 
			c_result = A & B;
			overflow = 1'd0;
			carryout = 1'd0;
			end
			
	3'd5 : begin 
			c_result = A | B;
			overflow = 1'd0;
			carryout = 1'd0;
			end
			
	3'd6 : begin 
			c_result = A^B;
			overflow = 1'd0;
			carryout = 1'd0;
			end
			
	3'd7 : begin 
			c_result = (A >> 1 );
			overflow = 1'd0;
		   carryout = 1'd0;
			end
	
endcase 
end 

// Sequential Block 
always @( posedge clk ) begin
 if ( reset == 1'b1 ) begin 
	result <= 'd0 ; 
	zero  <=  'd0 ;  
 end else begin 
 	result <= c_result ; 	
	zero   <= ( c_result == {NUMBITS{1'b0}}) ? 1'b1 : 1'b0 ; 
 end 

end 

endmodule
