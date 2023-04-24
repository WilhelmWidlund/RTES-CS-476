
module soc_system (
	accelerator_v2_0_debug_count_readdata,
	accelerator_v2_0_debug_state_readdata,
	clk_clk,
	reset_reset_n);	

	output	[9:0]	accelerator_v2_0_debug_count_readdata;
	output	[7:0]	accelerator_v2_0_debug_state_readdata;
	input		clk_clk;
	input		reset_reset_n;
endmodule
