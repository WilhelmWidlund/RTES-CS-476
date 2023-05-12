	soc_system u0 (
		.clk_clk                                        (<connected-to-clk_clk>),                                        //                                    clk.clk
		.in_switches_export                             (<connected-to-in_switches_export>),                             //                            in_switches.export
		.out_led_export                                 (<connected-to-out_led_export>),                                 //                                out_led.export
		.pio_1st_7seg_external_connection_export        (<connected-to-pio_1st_7seg_external_connection_export>),        //       pio_1st_7seg_external_connection.export
		.pio_2nd_7seg_external_connection_export        (<connected-to-pio_2nd_7seg_external_connection_export>),        //       pio_2nd_7seg_external_connection.export
		.pio_3rd_7seg_external_connection_export        (<connected-to-pio_3rd_7seg_external_connection_export>),        //       pio_3rd_7seg_external_connection.export
		.pll_shared_outclk2_clk                         (<connected-to-pll_shared_outclk2_clk>),                         //                     pll_shared_outclk2.clk
		.reset_reset_n                                  (<connected-to-reset_reset_n>),                                  //                                  reset.reset_n
		.sdram_controller_shared_wire_addr              (<connected-to-sdram_controller_shared_wire_addr>),              //           sdram_controller_shared_wire.addr
		.sdram_controller_shared_wire_ba                (<connected-to-sdram_controller_shared_wire_ba>),                //                                       .ba
		.sdram_controller_shared_wire_cas_n             (<connected-to-sdram_controller_shared_wire_cas_n>),             //                                       .cas_n
		.sdram_controller_shared_wire_cke               (<connected-to-sdram_controller_shared_wire_cke>),               //                                       .cke
		.sdram_controller_shared_wire_cs_n              (<connected-to-sdram_controller_shared_wire_cs_n>),              //                                       .cs_n
		.sdram_controller_shared_wire_dq                (<connected-to-sdram_controller_shared_wire_dq>),                //                                       .dq
		.sdram_controller_shared_wire_dqm               (<connected-to-sdram_controller_shared_wire_dqm>),               //                                       .dqm
		.sdram_controller_shared_wire_ras_n             (<connected-to-sdram_controller_shared_wire_ras_n>),             //                                       .ras_n
		.sdram_controller_shared_wire_we_n              (<connected-to-sdram_controller_shared_wire_we_n>),              //                                       .we_n
		.sysaudio_audio_core_external_interface_ADCDAT  (<connected-to-sysaudio_audio_core_external_interface_ADCDAT>),  // sysaudio_audio_core_external_interface.ADCDAT
		.sysaudio_audio_core_external_interface_ADCLRCK (<connected-to-sysaudio_audio_core_external_interface_ADCLRCK>), //                                       .ADCLRCK
		.sysaudio_audio_core_external_interface_BCLK    (<connected-to-sysaudio_audio_core_external_interface_BCLK>),    //                                       .BCLK
		.sysaudio_audio_core_external_interface_DACDAT  (<connected-to-sysaudio_audio_core_external_interface_DACDAT>),  //                                       .DACDAT
		.sysaudio_audio_core_external_interface_DACLRCK (<connected-to-sysaudio_audio_core_external_interface_DACLRCK>), //                                       .DACLRCK
		.sysaudio_av_config_external_interface_SDAT     (<connected-to-sysaudio_av_config_external_interface_SDAT>),     //  sysaudio_av_config_external_interface.SDAT
		.sysaudio_av_config_external_interface_SCLK     (<connected-to-sysaudio_av_config_external_interface_SCLK>)      //                                       .SCLK
	);

