`timescale 1ns / 1ps

module BCD_alu #( parameter NUMBITS = 32 ) ( 
input wire clk, 
input wire reset , 
input wire [NUMBITS-1:0] A, 
input wire [NUMBITS-1:0] B, 
input wire [3:0] opcode, 
output reg [35:0] result, 
output reg carryout,
output reg overflow,
output reg zero);

// Combinatorial Block 
reg [35:0] c_result;
reg [3:0] Acheck; 
reg [3:0] Acheck1;
reg [3:0] Acheck2; 
reg [3:0] Acheck3;
reg [3:0] Acheck4;
reg [3:0] Acheck5; 
reg [3:0] Acheck6; 
reg [3:0] Acheck7;
reg [3:0] Bcheck; 
reg [3:0] Bcheck1;
reg [3:0] Bcheck2; 
reg [3:0] Bcheck3;
reg [3:0] Bcheck4;
reg [3:0] Bcheck5; 
reg [3:0] Bcheck6; 
reg [3:0] Bcheck7;

reg[31:0] fullA;
reg[31:0] fullB;
reg [3:0] temp;
reg [3:0] temp1;
reg [3:0] temp2;
reg [3:0] temp3;
reg [3:0] temp4;
reg [3:0] temp5;
reg [3:0] temp6;
reg [3:0] temp7;
reg [31:0] ans;
reg [40:0] tempans;
reg [63:0] ans2;  
integer i;

always @(*) begin 
 c_result = 'd0; 
 case (opcode)    // o - unsigned add, 1 is signed add, 2 is unsigned sub, 3 is signed sub
 //unsigned add works fine 
 4'd8 : begin 
			  overflow = 0;
           Acheck  = A[0+:4];   // 0 -3
			  Acheck1 = A[4+:4];   // 4 -7
			  Acheck2 = A[8+:4];   // 8-11
			  Acheck3 = A[12+:4];  // 12 -15
			  Acheck4 = A[16+:4];  //16-19
			  Acheck5 = A[20+:4];  // 20-23
			  Acheck6 = A[24+:4];  //24 - 27
			  Acheck7 = A[28+:4];  //28-31
			  Bcheck  = B[0+:4];   // 0 -3
			  Bcheck1 = B[4+:4];   // 4 -7
			  Bcheck2 = B[8+:4];   // 8-11
			  Bcheck3 = B[12+:4];  // 12 -15
			  Bcheck4 = B[16+:4];  //16-19
			  Bcheck5 = B[20+:4];  // 20-23
			  Bcheck6 = B[24+:4];  //24 - 27
			  Bcheck7 = B[28+:4];  //28-31
 
           fullA = Acheck + (Acheck1 * 10) + (Acheck2 * 100) + (Acheck3 * 1000) + (Acheck4 * 10000) + (Acheck5 * 100000) + (Acheck6 * 1000000) + (Acheck7 * 10000000);
           fullB = Bcheck + (Bcheck1 * 10) + (Bcheck2 * 100) + (Bcheck3 * 1000) + (Bcheck4 * 10000) + (Bcheck5 * 100000) + (Bcheck6 * 1000000) + (Bcheck7 * 10000000);
 
			  tempans = fullA + fullB;
			  if(tempans > 2576980377)
			  carryout = 1;
			  else
			  carryout = 0;
			  ans = fullA + fullB;
			  ans2 = ans;
			   for(i = 0; i < 32; i = i + 1)          
				begin
					  temp = ans2[32+:4];
					  temp1 = ans2[36+:4];
					  temp2 = ans2[40+:4];
					  temp3 = ans2[44+:4];
					  temp4 = ans2[48+:4];
					  temp5 = ans2[52+:4];
					  temp6 = ans2[56+:4];
					  temp7 = ans2[60+:4];
					  if(temp >=5 )
					  temp = temp + 3;
					  if(temp1 >=5)
					  temp1 = temp1 + 3;
					  if(temp2 >=5 )
					  temp2 = temp2 + 3;
					  if(temp3 >=5 )
					  temp3 = temp3 + 3;
					  if(temp4 >=5 )
					  temp4 = temp4 + 3;
					  if(temp5 >=5 )
					  temp5 = temp5 + 3;
					  if(temp6 >=5 )
					  temp6 = temp6 + 3;
					  if(temp7 >=5 )
					  temp7 = temp7 + 3;
					  
					  ans2[35:32] = temp[0+:4];
					  ans2[39:36] = temp1[0+:4];
					  ans2[43:40] = temp2[0+:4];
					  ans2[47:44] = temp3[0+:4];
					  ans2[51:48] = temp4[0+:4];
					  ans2[55:52] = temp5[0+:4];
					  ans2[59:56] = temp6[0+:4];
					  ans2[63:60] = temp7[0+:4];
					  ans2 = ans2 << 1;
			 end
			        c_result[3:0] =  ans2[35:32];
					  c_result[7:4] =  ans2[39:36];
					  c_result[11:8] = ans2[43:40];
					  c_result[15:12] = ans2[47:44];
					  c_result[19:16] = ans2[51:48];
					  c_result[23:20] = ans2[55:52];
					  c_result[27:24] = ans2[59:56];
					  c_result[31:28] = ans2[63:60];										  
			end  
 //signed add
	4'd12 : begin
	  carryout = 0;
           Acheck  = A[0+:4];   // 0 -3
			  Acheck1 = A[4+:4];   // 4 -7
			  Acheck2 = A[8+:4];   // 8-11
			  Acheck3 = A[12+:4];  // 12 -15
			  Acheck4 = A[16+:4];  //16-19
			  Acheck5 = A[20+:4];  // 20-23
			  Acheck6 = A[24+:4];  //24 - 27
			  Bcheck  = B[0+:4];   // 0 -3
			  Bcheck1 = B[4+:4];   // 4 -7
			  Bcheck2 = B[8+:4];   // 8-11
			  Bcheck3 = B[12+:4];  // 12 -15
			  Bcheck4 = B[16+:4];  //16-19
			  Bcheck5 = B[20+:4];  // 20-23
			  Bcheck6 = B[24+:4];  //24 - 27
 
           fullA = Acheck + (Acheck1 * 10) + (Acheck2 * 100) + (Acheck3 * 1000) + (Acheck4 * 10000) + (Acheck5 * 100000) + (Acheck6 * 1000000);
           fullB = Bcheck + (Bcheck1 * 10) + (Bcheck2 * 100) + (Bcheck3 * 1000) + (Bcheck4 * 10000) + (Bcheck5 * 100000) + (Bcheck6 * 1000000);
 
			  tempans = $signed(fullA) + $signed(fullB);
			  overflow = 0;
			  ans = $signed(fullA) + $signed(fullB);
			  ans2 = ans;
			   for(i = 0; i < 32; i = i + 1)          
				begin
					  temp = ans2[32+:4];
					  temp1 = ans2[36+:4];
					  temp2 = ans2[40+:4];
					  temp3 = ans2[44+:4];
					  temp4 = ans2[48+:4];
					  temp5 = ans2[52+:4];
					  temp6 = ans2[56+:4];
					  if(temp >=5 )
					  temp = temp + 3;
					  if(temp1 >=5)
					  temp1 = temp1 + 3;
					  if(temp2 >=5 )
					  temp2 = temp2 + 3;
					  if(temp3 >=5 )
					  temp3 = temp3 + 3;
					  if(temp4 >=5 )
					  temp4 = temp4 + 3;
					  if(temp5 >=5 )
					  temp5 = temp5 + 3;
					  if(temp6 >=5 )
					  temp6 = temp6 + 3;
		 
					  ans2[35:32] = temp[0+:4];
					  ans2[39:36] = temp1[0+:4];
					  ans2[43:40] = temp2[0+:4];
					  ans2[47:44] = temp3[0+:4];
					  ans2[51:48] = temp4[0+:4];
					  ans2[55:52] = temp5[0+:4];
					  ans2[59:56] = temp6[0+:4];
					  ans2 = ans2 << 1;
			 end
			        c_result[3:0] =  ans2[35:32];
					  c_result[7:4] =  ans2[39:36];
					  c_result[11:8] = ans2[43:40];
					  c_result[15:12] = ans2[47:44];
					  c_result[19:16] = ans2[51:48];
					  c_result[23:20] = ans2[55:52];
					  c_result[27:24] = ans2[59:56];
					  //c_result[31:28] = ans2[63:60];	
					  if(tempans >= 0)
					  c_result[35:32] = 4'd0;
					  else
					  c_result[35:32] = 4'd1;
			end
			
	//signed sub	
	4'd13 : begin  
	carryout = 0;
           Acheck  = A[0+:4];   // 0 -3
			  Acheck1 = A[4+:4];   // 4 -7
			  Acheck2 = A[8+:4];   // 8-11
			  Acheck3 = A[12+:4];  // 12 -15
			  Acheck4 = A[16+:4];  //16-19
			  Acheck5 = A[20+:4];  // 20-23
			  Acheck6 = A[24+:4];  //24 - 27
			  Bcheck  = B[0+:4];   // 0 -3
			  Bcheck1 = B[4+:4];   // 4 -7
			  Bcheck2 = B[8+:4];   // 8-11
			  Bcheck3 = B[12+:4];  // 12 -15
			  Bcheck4 = B[16+:4];  //16-19
			  Bcheck5 = B[20+:4];  // 20-23
			  Bcheck6 = B[24+:4];  //24 - 27
 
           fullA = Acheck + (Acheck1 * 10) + (Acheck2 * 100) + (Acheck3 * 1000) + (Acheck4 * 10000) + (Acheck5 * 100000) + (Acheck6 * 1000000);
           fullB = Bcheck + (Bcheck1 * 10) + (Bcheck2 * 100) + (Bcheck3 * 1000) + (Bcheck4 * 10000) + (Bcheck5 * 100000) + (Bcheck6 * 1000000);
 
			  tempans = $signed(fullA) - $signed(fullB);
			  overflow = 0;
			  ans = $signed(fullA) - $signed(fullB);
			  ans2 = ans;
			   for(i = 0; i < 32; i = i + 1)          
				begin
					  temp = ans2[32+:4];
					  temp1 = ans2[36+:4];
					  temp2 = ans2[40+:4];
					  temp3 = ans2[44+:4];
					  temp4 = ans2[48+:4];
					  temp5 = ans2[52+:4];
					  temp6 = ans2[56+:4];
					  temp7 = ans2[60+:4];
					  if(temp >=5 )
					  temp = temp + 3;
					  if(temp1 >=5)
					  temp1 = temp1 + 3;
					  if(temp2 >=5 )
					  temp2 = temp2 + 3;
					  if(temp3 >=5 )
					  temp3 = temp3 + 3;
					  if(temp4 >=5 )
					  temp4 = temp4 + 3;
					  if(temp5 >=5 )
					  temp5 = temp5 + 3;
					  if(temp6 >=5 )
					  temp6 = temp6 + 3;
					  
					  ans2[35:32] = temp[0+:4];
					  ans2[39:36] = temp1[0+:4];
					  ans2[43:40] = temp2[0+:4];
					  ans2[47:44] = temp3[0+:4];
					  ans2[51:48] = temp4[0+:4];
					  ans2[55:52] = temp5[0+:4];
					  ans2[59:56] = temp6[0+:4];
					  ans2 = ans2 << 1;
			 end
			        c_result[3:0] =  ans2[35:32];
					  c_result[7:4] =  ans2[39:36];
					  c_result[11:8] = ans2[43:40];
					  c_result[15:12] = ans2[47:44];
					  c_result[19:16] = ans2[51:48];
					  c_result[23:20] = ans2[55:52];
					  c_result[27:24] = ans2[59:56];
					  c_result[31:28] = ans2[63:60];	
					  if(tempans >= 0)
					  c_result[35:32] = 4'd0;
					  else
					  c_result[35:32] = 4'd1;
			end
			
	//unsigned sub works 100%
	4'd9 : begin 		
			overflow = 0;
           Acheck  = A[0+:4];   // 0 -3
			  Acheck1 = A[4+:4];   // 4 -7
			  Acheck2 = A[8+:4];   // 8-11
			  Acheck3 = A[12+:4];  // 12 -15
			  Acheck4 = A[16+:4];  //16-19
			  Acheck5 = A[20+:4];  // 20-23
			  Acheck6 = A[24+:4];  //24 - 27
			  Acheck7 = A[28+:4];  //28-31
			  Bcheck  = B[0+:4];   // 0 -3
			  Bcheck1 = B[4+:4];   // 4 -7
			  Bcheck2 = B[8+:4];   // 8-11
			  Bcheck3 = B[12+:4];  // 12 -15
			  Bcheck4 = B[16+:4];  //16-19
			  Bcheck5 = B[20+:4];  // 20-23
			  Bcheck6 = B[24+:4];  //24 - 27
			  Bcheck7 = B[28+:4];  //28-31
 
           fullA = Acheck + (Acheck1 * 10) + (Acheck2 * 100) + (Acheck3 * 1000) + (Acheck4 * 10000) + (Acheck5 * 100000) + (Acheck6 * 1000000) + (Acheck7 * 10000000);
           fullB = Bcheck + (Bcheck1 * 10) + (Bcheck2 * 100) + (Bcheck3 * 1000) + (Bcheck4 * 10000) + (Bcheck5 * 100000) + (Bcheck6 * 1000000) + (Bcheck7 * 10000000);
 
			  tempans = fullA - fullB;
			  if(tempans > 2576980377)
			  carryout = 1;
			  else
			  carryout = 0;
			  if(carryout == 1)
			  ans = 0;
			  else
			  ans = fullA - fullB;
			  ans2 = ans;
			   for(i = 0; i < 32; i = i + 1)          
				begin
					  temp = ans2[32+:4];
					  temp1 = ans2[36+:4];
					  temp2 = ans2[40+:4];
					  temp3 = ans2[44+:4];
					  temp4 = ans2[48+:4];
					  temp5 = ans2[52+:4];
					  temp6 = ans2[56+:4];
					  temp7 = ans2[60+:4];
					  if(temp >=5 )
					  temp = temp + 3;
					  if(temp1 >=5)
					  temp1 = temp1 + 3;
					  if(temp2 >=5 )
					  temp2 = temp2 + 3;
					  if(temp3 >=5 )
					  temp3 = temp3 + 3;
					  if(temp4 >=5 )
					  temp4 = temp4 + 3;
					  if(temp5 >=5 )
					  temp5 = temp5 + 3;
					  if(temp6 >=5 )
					  temp6 = temp6 + 3;
					  if(temp7 >=5 )
					  temp7 = temp7 + 3;
					  
					  ans2[35:32] = temp[0+:4];
					  ans2[39:36] = temp1[0+:4];
					  ans2[43:40] = temp2[0+:4];
					  ans2[47:44] = temp3[0+:4];
					  ans2[51:48] = temp4[0+:4];
					  ans2[55:52] = temp5[0+:4];
					  ans2[59:56] = temp6[0+:4];
					  ans2[63:60] = temp7[0+:4];
					  ans2 = ans2 << 1;
			 end
			        c_result[3:0] =  ans2[35:32];
					  c_result[7:4] =  ans2[39:36];
					  c_result[11:8] = ans2[43:40];
					  c_result[15:12] = ans2[47:44];
					  c_result[19:16] = ans2[51:48];
					  c_result[23:20] = ans2[55:52];
					  c_result[27:24] = ans2[59:56];
					  c_result[31:28] = ans2[63:60];			
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