	soc_system u0 (
		.audio_0_external_interface_ADCDAT                (<connected-to-audio_0_external_interface_ADCDAT>),                //                  audio_0_external_interface.ADCDAT
		.audio_0_external_interface_ADCLRCK               (<connected-to-audio_0_external_interface_ADCLRCK>),               //                                            .ADCLRCK
		.audio_0_external_interface_BCLK                  (<connected-to-audio_0_external_interface_BCLK>),                  //                                            .BCLK
		.audio_0_external_interface_DACDAT                (<connected-to-audio_0_external_interface_DACDAT>),                //                                            .DACDAT
		.audio_0_external_interface_DACLRCK               (<connected-to-audio_0_external_interface_DACLRCK>),               //                                            .DACLRCK
		.audio_and_video_config_0_external_interface_SDAT (<connected-to-audio_and_video_config_0_external_interface_SDAT>), // audio_and_video_config_0_external_interface.SDAT
		.audio_and_video_config_0_external_interface_SCLK (<connected-to-audio_and_video_config_0_external_interface_SCLK>), //                                            .SCLK
		.audio_pll_0_audio_clk_clk                        (<connected-to-audio_pll_0_audio_clk_clk>),                        //                       audio_pll_0_audio_clk.clk
		.clk_clk                                          (<connected-to-clk_clk>),                                          //                                         clk.clk
		.hw_debug_readdata                                (<connected-to-hw_debug_readdata>),                                //                                    hw_debug.readdata
		.pio_2_export                                     (<connected-to-pio_2_export>),                                     //                                       pio_2.export
		.pll_2_outclk2_clk                                (<connected-to-pll_2_outclk2_clk>),                                //                               pll_2_outclk2.clk
		.pp0_out_export                                   (<connected-to-pp0_out_export>),                                   //                                     pp0_out.export
		.pp1_out_export                                   (<connected-to-pp1_out_export>),                                   //                                     pp1_out.export
		.pp2_out_export                                   (<connected-to-pp2_out_export>),                                   //                                     pp2_out.export
		.reset_reset_n                                    (<connected-to-reset_reset_n>),                                    //                                       reset.reset_n
		.sdram_controller_2_wire_addr                     (<connected-to-sdram_controller_2_wire_addr>),                     //                     sdram_controller_2_wire.addr
		.sdram_controller_2_wire_ba                       (<connected-to-sdram_controller_2_wire_ba>),                       //                                            .ba
		.sdram_controller_2_wire_cas_n                    (<connected-to-sdram_controller_2_wire_cas_n>),                    //                                            .cas_n
		.sdram_controller_2_wire_cke                      (<connected-to-sdram_controller_2_wire_cke>),                      //                                            .cke
		.sdram_controller_2_wire_cs_n                     (<connected-to-sdram_controller_2_wire_cs_n>),                     //                                            .cs_n
		.sdram_controller_2_wire_dq                       (<connected-to-sdram_controller_2_wire_dq>),                       //                                            .dq
		.sdram_controller_2_wire_dqm                      (<connected-to-sdram_controller_2_wire_dqm>),                      //                                            .dqm
		.sdram_controller_2_wire_ras_n                    (<connected-to-sdram_controller_2_wire_ras_n>),                    //                                            .ras_n
		.sdram_controller_2_wire_we_n                     (<connected-to-sdram_controller_2_wire_we_n>)                      //                                            .we_n
	);

