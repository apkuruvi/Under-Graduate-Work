`timescale 1ns / 1ps

module control_unit(
input [5:0] opcode,
output reg reg_dst,
output reg branch,
output reg mem_read,
output reg mem_to_reg,
output reg [1:0] alu_op,
output reg mem_write,
output reg alu_src,
output reg reg_write
    );

integer R =   6'b000000;
integer LW=   6'b100011;
integer SW=   6'b101011;
integer ADDI= 6'b001000;


integer BEQ = 6'b000100;



always @(*) begin
reg_dst     = 0;
    branch      = 0;
    mem_read    = 0;
    mem_to_reg  = 0;
    alu_op      = 0;
    mem_write   = 0;
    alu_src     = 0;
    reg_write   = 0;
	case(opcode)
		R:begin 	
						reg_dst    = 1;
                  reg_write  = 1;
                  alu_op     = 2;
					end 
		ADDI : 
			begin 
			 alu_src    = 1;
			 reg_write  = 1;
			end 
		LW:begin
			alu_src    = 1;
                     mem_to_reg = 1;
                     reg_write  = 1;
                     mem_read   = 1;
			end
		SW:begin
			alu_src    = 1;
							mem_write  = 1;
			end
		BEQ:begin
			reg_dst  = 1;
							branch   = 1;
                     alu_op   = 1;
			end
		default:begin	//for immediates
		   branch      = 0;
			end
		
endcase
end
endmodule
