
module soc_system (
	clk_clk,
	reset_reset_n,
	custom_pp_readdata,
	stock_pp_export);	

	input		clk_clk;
	input		reset_reset_n;
	output	[31:0]	custom_pp_readdata;
	input	[3:0]	stock_pp_export;
endmodule
