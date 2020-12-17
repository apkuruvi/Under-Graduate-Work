`timescale 1ns / 1ps
module mux_2_1( select_in, datain1, datain2, data_out   );

// Parameters 
parameter WORD_SIZE = 32 ; 

// Input and outputs 
// Modelling with Continuous Assignments 

input  wire select_in ;  
input  wire [WORD_SIZE - 1:0] datain1 ; 
input  wire [WORD_SIZE - 1:0] datain2 ; 
output wire [WORD_SIZE - 1:0] data_out ;

assign data_out = (select_in) ? datain2 : datain1;

endmodule
