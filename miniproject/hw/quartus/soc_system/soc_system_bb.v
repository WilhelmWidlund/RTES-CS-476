
module soc_system (
	clk_clk,
	in_switches_export,
	out_led_export,
	pio_1st_7seg_external_connection_export,
	pio_2nd_7seg_external_connection_export,
	pio_3rd_7seg_external_connection_export,
	pll_shared_outclk2_clk,
	reset_reset_n,
	sdram_controller_shared_wire_addr,
	sdram_controller_shared_wire_ba,
	sdram_controller_shared_wire_cas_n,
	sdram_controller_shared_wire_cke,
	sdram_controller_shared_wire_cs_n,
	sdram_controller_shared_wire_dq,
	sdram_controller_shared_wire_dqm,
	sdram_controller_shared_wire_ras_n,
	sdram_controller_shared_wire_we_n,
	sysaudio_audio_core_external_interface_ADCDAT,
	sysaudio_audio_core_external_interface_ADCLRCK,
	sysaudio_audio_core_external_interface_BCLK,
	sysaudio_audio_core_external_interface_DACDAT,
	sysaudio_audio_core_external_interface_DACLRCK,
	sysaudio_av_config_external_interface_SDAT,
	sysaudio_av_config_external_interface_SCLK);	

	input		clk_clk;
	input	[7:0]	in_switches_export;
	output	[9:0]	out_led_export;
	output	[13:0]	pio_1st_7seg_external_connection_export;
	output	[13:0]	pio_2nd_7seg_external_connection_export;
	output	[13:0]	pio_3rd_7seg_external_connection_export;
	output		pll_shared_outclk2_clk;
	input		reset_reset_n;
	output	[12:0]	sdram_controller_shared_wire_addr;
	output	[1:0]	sdram_controller_shared_wire_ba;
	output		sdram_controller_shared_wire_cas_n;
	output		sdram_controller_shared_wire_cke;
	output		sdram_controller_shared_wire_cs_n;
	inout	[31:0]	sdram_controller_shared_wire_dq;
	output	[3:0]	sdram_controller_shared_wire_dqm;
	output		sdram_controller_shared_wire_ras_n;
	output		sdram_controller_shared_wire_we_n;
	input		sysaudio_audio_core_external_interface_ADCDAT;
	input		sysaudio_audio_core_external_interface_ADCLRCK;
	input		sysaudio_audio_core_external_interface_BCLK;
	output		sysaudio_audio_core_external_interface_DACDAT;
	input		sysaudio_audio_core_external_interface_DACLRCK;
	inout		sysaudio_av_config_external_interface_SDAT;
	output		sysaudio_av_config_external_interface_SCLK;
endmodule
