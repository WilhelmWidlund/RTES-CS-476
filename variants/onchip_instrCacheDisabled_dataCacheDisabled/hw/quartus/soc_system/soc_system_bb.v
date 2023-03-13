
module soc_system (
	clk_clk,
	pin_buttons_export,
	pout_custom_readdata,
	reset_reset_n);	

	input		clk_clk;
	input	[3:0]	pin_buttons_export;
	output	[31:0]	pout_custom_readdata;
	input		reset_reset_n;
endmodule
