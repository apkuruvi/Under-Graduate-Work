`timescale 1ns / 1ps

module cs161_processor_testbench( );

  // Inputs
  
  reg clk ;
  reg rst ;

  // Outputs
  
  wire [31:0] prog_count  ;
  wire [5:0] instr_opcode ;
  wire [4:0] reg1_addr    ;
  wire [31:0] reg1_data   ;
  wire [4:0] reg2_addr  ;
  wire [31:0] reg2_data ;
  wire [4:0] write_reg_addr ;
  wire [31:0] write_reg_data ;

 cs161_processor uut (
    .clk                 ( clk ),
    .rst                 ( rst ),
    .prog_count          ( prog_count ),
    .instr_opcode        ( instr_opcode ),
    .reg1_addr           ( reg1_addr ),
    .reg1_data           ( reg1_data ),
    .reg2_addr           ( reg2_addr ),
    .reg2_data           ( reg2_data ) ,
    .write_reg_addr      ( write_reg_addr ),
    .write_reg_data      ( write_reg_data )
    );
  
  	initial begin 
	 
	 clk = 0 ; 
	 rst = 1 ; 
	 # 20 ; 
	 
	 clk = 1 ; 
	 rst = 1 ; 
	 # 20 ; 

	 clk = 0 ; 
	 rst = 0 ; 
	 # 20 ; 
		 
	 forever begin 
		#20 clk = ~clk;
	 end 
	 
	end 
	
endmodule
