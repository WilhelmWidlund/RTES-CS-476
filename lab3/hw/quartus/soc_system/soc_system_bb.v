
module soc_system (
	clk_clk,
	debug_export,
	pll_2_sdram_clk,
	pp0_out_export,
	pp1_out_export,
	pp2_out_export,
	reset_reset_n,
	sdram_controller_2_wire_addr,
	sdram_controller_2_wire_ba,
	sdram_controller_2_wire_cas_n,
	sdram_controller_2_wire_cke,
	sdram_controller_2_wire_cs_n,
	sdram_controller_2_wire_dq,
	sdram_controller_2_wire_dqm,
	sdram_controller_2_wire_ras_n,
	sdram_controller_2_wire_we_n);	

	input		clk_clk;
	output	[31:0]	debug_export;
	output		pll_2_sdram_clk;
	output	[7:0]	pp0_out_export;
	output	[7:0]	pp1_out_export;
	output	[7:0]	pp2_out_export;
	input		reset_reset_n;
	output	[12:0]	sdram_controller_2_wire_addr;
	output	[1:0]	sdram_controller_2_wire_ba;
	output		sdram_controller_2_wire_cas_n;
	output		sdram_controller_2_wire_cke;
	output		sdram_controller_2_wire_cs_n;
	inout	[15:0]	sdram_controller_2_wire_dq;
	output	[1:0]	sdram_controller_2_wire_dqm;
	output		sdram_controller_2_wire_ras_n;
	output		sdram_controller_2_wire_we_n;
endmodule
