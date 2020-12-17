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
wire MemRead;

wire [31:0] PC_Output;
wire [31:0] Increment_4_result;
wire [31:0] Shift2_result;
wire [31:0] AddALU_result;
wire [31:0] SignExt_result;
wire [31:0] PC_input;

//======Registers for lab 5 =============== 
/*
wire [31:0] ALU_result;
wire [31:0] ALU_input2;
wire [31:0] DataMem_result;

wire [31:0] mux_MemtoReg_result;
wire [4:0] mux_RegDst_result;
wire [3:0] alu_control_result;

wire [31:0] PC_Output;

wire [31:0] Shift2_result;
wire [31:0] AddALU_result;
wire [31:0] SignExt_result;
wire [31:0] PC_input;
*/
wire [31:0] IFID_instruction;
wire [31:0] IFID_Increment_4_result;
//=========================================
//==============IFID===========================
IFID_REG IFID_inst(.clk(clk),
						 .rst(rst),
						 .add4_in(Increment_4_result), .add4_out(IFID_Increment_4_result),
						 .inst_in(instruction), .inst_out(IFID_instruction) );
//PC 
gen_register PC (.clk(clk), .rst(rst), .in(PC_input), .out(PC_Output));

//==============END OF IFID====================
//==============IDEX===========================
wire [6:0] control_input;
wire [6:0] IDEX_control_input;
wire[1:0] IDEX_ALUOp;
//wire IDEX_ALU_zero;

wire IDEX_Branch;
wire IDEX_MemtoReg;
wire IDEX_MemRead;
wire IDEX_MemWrite;
wire IDEX_ALUSrc;
wire IDEX_RegWrite;
wire IDEX_RegDst;
//wire [6:0]IDEX_control_input;
wire IDEX_control_mux_Add4_AddALU;
wire [31:0] IDEX_Increment_4_result;
wire [31:0] IDEX_register_ReadData1;
wire [31:0] IDEX_register_ReadData2;
wire [31:0] IDEX_SignExt_result;
wire [4:0] IDEX_inst15;
wire [4:0] IDEX_inst20;
wire EX_RegWrite;
wire EX_MemtoReg;
wire EX_Branch;
wire EX_MemRead;
wire EX_alu_zero;
wire [4:0] EX_mux_RegDst_result;
wire [31:0] EX_ALU_result;
wire [31:0] EX_register_ReadData2;
wire [31:0] EX_AddALU_result;

wire WB_RegWrite;
wire WB_MemtoReg;
wire [31:0] WB_DataMem_result;
wire [31:0] WB_ALU_result;
wire [4:0] WB_mux_RegDst_result;
//extends the 16bits to 32 bits 
//assign SignExt_result = {32{1'b0}} + instruction[15:0];
IDEX IDEX_inst(.clk(clk), .rst(rst), /*.control_in(control_input),.control_out(IDEX_control_input) ,*/
				.add4_in(IFID_Increment_4_result),.add4_out(IDEX_Increment_4_result),
				.read_data1_int(register_ReadData1),.read_data1_out(IDEX_register_ReadData1),
				.read_data2_in(register_ReadData2),.read_data2_out(IDEX_register_ReadData2),
				.sext_in(SignExt_result), .sext_out(IDEX_SignExt_result),
				.inst15_in(IFID_instruction[15:11]), 	.inst15_out(IDEX_inst15), 
				.inst20_in(IFID_instruction[20:16]), .inst20_out(IDEX_inst20),
				.alu_in(ALUOp),.alu_out(IDEX_ALUOp),
				.RegDst_in(RegDst),.RegDst_out(IDEX_RegDst),
				.Branch_in(Branch),.Branch_out(IDEX_Branch),
				.MemRead_in(MemRead),.MemRead_out(IDEX_MemRead),
				.MemtoReg_in(MemtoReg),.MemtoReg_out(IDEX_MemtoReg),
				.ALUSrc_in(ALUSrc),.ALUSrc_out(IDEX_ALUSrc),
				.RegWrite_in(RegWrite),.RegWrite_out(IDEX_RegWrite)
				);
assign SignExt_result = {{16{IFID_instruction[15]}},IFID_instruction[15:0]};

cpu_registers CPURegisters (.clk(clk),
										.rst(rst), 
										.reg_write(WB_RegWrite), 
										.read_register_1(IFID_instruction[25:21]),
										.read_register_2(IFID_instruction[20:16]), 
										.write_register(WB_mux_RegDst_result), 
										.write_data(mux_MemtoReg_result), 
										.read_data_1(register_ReadData1), .read_data_2(register_ReadData2)); 


//Combination of instruction memory and data memory
cpumemory memories(
						.clk(clk), .rst(rst), 
						.instr_read_address(PC_Output[9:2]), 
						.instr_instruction(instruction),
						//.data_mem_write(MemWrite), 
						.data_address(EX_ALU_result[7:0]), 
						.data_write_data(EX_register_ReadData2), 
						.data_read_data(DataMem_result));
//uses different code 
control_unit control (.opcode(IFID_instruction[31:26]), .reg_dst(RegDst), 
							.branch(Branch), .mem_read(MemRead), 
							.mem_to_reg(MemtoReg), .alu_op(ALUOp), 
							//.mem_write(MemWrite), 
							.alu_src(ALUSrc), 
							.reg_write(RegWrite));

//==============END OF IDEX=====================
//==============EX/MEM===========================
/*
wire EX_RegWrite;
wire EX_MemtoReg;
wire EX_Branch;
wire EX_MemRead;
wire EX_alu_zero;
wire [4:0] EX_mux_RegDst_result;
wire [31:0] EX_ALU_result;
wire [31:0] EX_register_ReadData2;
wire [31:0] EX_AddALU_result;
*/
EXMEM EXMEM_inst(.clk(clk), .rst(rst),.RegWrite_in(IDEX_RegWrite),.RegWrite_out(EX_RegWrite),
				 .MemtoReg_in(IDEX_MemtoReg),.MemtoReg_out(EX_MemtoReg),
				 .MemRead_in(IDEX_MemRead),.MemRead_out(EX_MemRead),
				 .Branch_in(IDEX_Branch),.Branch_out(EX_Branch),
				 .ALUresult_in(ALU_result),.ALUresult_out(EX_ALU_result),
				 .regdata_in(IDEX_register_ReadData2),.regdata_out(EX_register_ReadData2),
				 .zero_in(ALU_zero),.zero_out(EX_alu_zero),
				 .add_in(AddALU_result),.add_out(EX_AddALU_result),
				 .mux_in(mux_RegDst_result),.mux_out(EX_mux_RegDst_result)
				 );
alu_control ALU_Control(.alu_op(IDEX_ALUOp),
								.instruction_5_0(IDEX_SignExt_result[5:0]),
								.alu_out(alu_control_result));

alu alu_withZero (.alu_control_in(alu_control_result),
						.channel_a_in(IDEX_register_ReadData1),
						.channel_b_in(ALU_input2),
						.zero_out(ALU_zero),
						.alu_result_out(ALU_result));

//mux that exist between register and alu_withZero
mux_2_1 registers_mux_alu (.select_in(IDEX_ALUSrc),
								   .datain1(IDEX_register_ReadData2),
									.datain2(IDEX_SignExt_result),
									.data_out(ALU_input2));

//mux between adder to pc
mux_2_1 Add_MUX_PC (.select_in(control_mux_Add4_AddALU),
						  .datain1(Increment_4_result),
						  .datain2(EX_AddALU_result),
						  .data_out(PC_input));

//.word_size controls data member within the file
//mux that exist from instruction to registers
mux_2_1 #(.WORD_SIZE(5)) mux_RegDst_inst (.select_in(IDEX_RegDst),
														.datain1(IDEX_inst20),
														.datain2(IDEX_inst15), 
														.data_out(mux_RegDst_result));
//exist between datamemory and registers
mux_2_1 mux_MemtoReg_inst (
    .select_in(WB_MemtoReg), 
	  .datain1(WB_ALU_result), 
	  .datain2(WB_DataMem_result), 
	  .data_out(mux_MemtoReg_result));

WB WB_inst(.clk(clk),.rst(rst), .RegWrite_in(EX_RegWrite),.RegWrite_out(WB_RegWrite),
			  .MemtoReg_in(EX_MemtoReg),.MemtoReg_out(WB_MemtoReg),
			  .WB_dataMem_in(DataMem_result),.WB_dataMem_out(WB_DataMem_result),
			  .WB_ALU_in(EX_ALU_result),.WB_ALU_out(WB_ALU_result),
			  .WB_mux_in(EX_mux_RegDst_result),.WB_mux_out(WB_mux_RegDst_result)
			  );
//shifts instruction
assign Shift2_result = IDEX_SignExt_result << 2;

//Increments position of pc

assign Increment_4_result = PC_Output + 4;

//adds result of incremented pc and shifted instruction
assign AddALU_result = Shift2_result + IDEX_Increment_4_result;

//Or 
assign control_mux_Add4_AddALU = EX_Branch & EX_alu_zero;


always @(posedge clk) begin
if(rst == 1'b1)
begin
prog_count <= 0;
	instr_opcode <= 0;
	reg1_addr <= 0;
	reg1_data <= 0;
	reg2_addr <= 0;
	reg2_data <= 0;
	write_reg_addr <= 0;
	write_reg_data <= 0;

end
else
begin
	prog_count <= PC_Output;
	instr_opcode <= IFID_instruction[31:26];
	reg1_addr <= IFID_instruction[25:21];
	reg1_data <= register_ReadData1;
	reg2_addr <= IFID_instruction[20:16];
	reg2_data <= register_ReadData2;
	write_reg_addr <= WB_mux_RegDst_result;
	write_reg_data <= mux_MemtoReg_result;
	end
end

endmodule
