`timescale 1ns / 1ps

module cs161_processor (
	input wire clk,
	input wire rst,
	output reg [31:0] prog_count,
	output reg [31:0] reg1_data,
	output reg [31:0] reg2_data,
	output reg [31:0] write_reg_data,
	output reg [5:0]  instr_opcode,
	output reg [4:0]  reg1_addr,
	output reg [4:0]  reg2_addr,
	output reg [4:0]  write_reg_addr
);


wire [31:0] register_ReadData1;
wire [31:0] register_ReadData2;
wire [31:0] ALU_result;
wire [31:0] ALU_input2;
wire [31:0] DataMem_result;
wire [31:0] instruction;
wire [31:0] mux_MemtoReg_result;
wire [4:0] mux_RegDst_result;
wire [3:0] alu_control_result;
wire [1:0] ALUOp;

wire ALU_zero;
wire RegDst;
wire Branch;
wire MemtoReg;
wire MemWrite;
wire ALUSrc;
wire RegWrite;
wire control_mux_Add4_AddALU;

wire [31:0] PC_Output;
wire [31:0] Increment_4_result;
wire [31:0] Shift2_result;
wire [31:0] AddALU_result;
wire [31:0] SignExt_result;
wire [31:0] PC_input;

//Increments position of pc
assign Increment_4_result = PC_Output + 4;

//adds result of incremented pc and shifted instruction
assign AddALU_result = Shift2_result + Increment_4_result;

//shifts instruction
assign Shift2_result = SignExt_result << 2;

//extends the 16bits to 32 bits 
assign SignExt_result = {32{1'b0}} + instruction[15:0];

//Or 
assign control_mux_Add4_AddALU = Branch & ALU_zero;

//PC 
gen_register PC (.clk(clk), .rst(rst), .in(PC_input), .out(PC_Output));


cpu_registers CPURegisters (.clk(clk),
										.rst(rst), 
										.reg_write(RegWrite), 
										.read_register_1(instruction[25:21]),
										.read_register_2(instruction[20:16]), 
										.write_register(mux_RegDst_result), 
										.write_data(mux_MemtoReg_result), 
										.read_data_1(register_ReadData1), .read_data_2(register_ReadData2)); 

//Combination of instruction memory and data memory
cpumemory memories(
						.clk(clk), .rst(rst), 
						.instr_read_address(PC_Output[9:2]), 
						.instr_instruction(instruction),
						.data_mem_write(MemWrite), 
						.data_address(ALU_result[7:0]), 
						.data_write_data(register_ReadData2), 
						.data_read_data(DataMem_result));
//uses different code 
control_unit control (.opcode(instruction[31:26]), .reg_dst(RegDst), 
							.branch(Branch), .mem_read(control_mem_read), 
							.mem_to_reg(MemtoReg), .alu_op(ALUOp), 
							.mem_write(MemWrite), .alu_src(ALUSrc), 
							.reg_write(RegWrite));

alu_control ALU_Control(.alu_op(ALUOp),
								.instruction_5_0(instruction[5:0]),
								.alu_out(alu_control_result));

alu alu_withZero (.alu_control_in(alu_control_result),
						.channel_a_in(register_ReadData1),
						.channel_b_in(ALU_input2),
						.zero_out(ALU_zero),
						.alu_result_out(ALU_result));

//mux that exist between register and alu_withZero
mux_2_1 registers_mux_alu (.select_in(ALUSrc),
								   .datain1(register_ReadData2),
									.datain2(SignExt_result),
									.data_out(ALU_input2));

//mux between adder to pc
mux_2_1 Add_MUX_PC (.select_in(control_mux_Add4_AddALU),
						  .datain1(Increment_4_result),
						  .datain2(AddALU_result),
						  .data_out(PC_input));

//.word_size controls data member within the file
//mux that exist from instruction to registers
mux_2_1 #(.WORD_SIZE(5)) mux_RegDst_inst (.select_in(RegDst),
														.datain1(instruction[20:16]),
														.datain2(instruction[15:11]), 
														.data_out(mux_RegDst_result));
//exist between datamemory and registers
mux_2_1 mux_MemtoReg_inst (
    .select_in(MemtoReg), 
	  .datain1(ALU_result), 
	  .datain2(DataMem_result), 
	  .data_out(mux_MemtoReg_result));


always @(posedge clk) begin
	prog_count <= PC_Output;
	instr_opcode <= instruction[31:26];
	reg1_addr <= instruction[25:21];
	reg1_data <= register_ReadData1;
	reg2_addr <= instruction[20:16];
	reg2_data <= register_ReadData2;
	write_reg_addr <= mux_RegDst_result;
	write_reg_data <= mux_MemtoReg_result;
end

endmodule
