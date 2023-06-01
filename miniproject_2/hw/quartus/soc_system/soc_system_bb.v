
module soc_system (
	audio_0_external_interface_ADCDAT,
	audio_0_external_interface_ADCLRCK,
	audio_0_external_interface_BCLK,
	audio_0_external_interface_DACDAT,
	audio_0_external_interface_DACLRCK,
	audio_and_video_config_0_external_interface_SDAT,
	audio_and_video_config_0_external_interface_SCLK,
	audio_pll_0_audio_clk_clk,
	clk_clk,
	pio_2_export,
	pll_2_outclk2_clk,
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

	input		audio_0_external_interface_ADCDAT;
	input		audio_0_external_interface_ADCLRCK;
	input		audio_0_external_interface_BCLK;
	output		audio_0_external_interface_DACDAT;
	input		audio_0_external_interface_DACLRCK;
	inout		audio_and_video_config_0_external_interface_SDAT;
	output		audio_and_video_config_0_external_interface_SCLK;
	output		audio_pll_0_audio_clk_clk;
	input		clk_clk;
	input	[7:0]	pio_2_export;
	output		pll_2_outclk2_clk;
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
