	soc_system u0 (
		.clk_clk                           (<connected-to-clk_clk>),                           //                         clk.clk
		.new_sdram_controller_0_wire_addr  (<connected-to-new_sdram_controller_0_wire_addr>),  // new_sdram_controller_0_wire.addr
		.new_sdram_controller_0_wire_ba    (<connected-to-new_sdram_controller_0_wire_ba>),    //                            .ba
		.new_sdram_controller_0_wire_cas_n (<connected-to-new_sdram_controller_0_wire_cas_n>), //                            .cas_n
		.new_sdram_controller_0_wire_cke   (<connected-to-new_sdram_controller_0_wire_cke>),   //                            .cke
		.new_sdram_controller_0_wire_cs_n  (<connected-to-new_sdram_controller_0_wire_cs_n>),  //                            .cs_n
		.new_sdram_controller_0_wire_dq    (<connected-to-new_sdram_controller_0_wire_dq>),    //                            .dq
		.new_sdram_controller_0_wire_dqm   (<connected-to-new_sdram_controller_0_wire_dqm>),   //                            .dqm
		.new_sdram_controller_0_wire_ras_n (<connected-to-new_sdram_controller_0_wire_ras_n>), //                            .ras_n
		.new_sdram_controller_0_wire_we_n  (<connected-to-new_sdram_controller_0_wire_we_n>),  //                            .we_n
		.pin_buttons_export                (<connected-to-pin_buttons_export>),                //                 pin_buttons.export
		.pll_0_sdram_clk                   (<connected-to-pll_0_sdram_clk>),                   //                 pll_0_sdram.clk
		.pout_custom_readdata              (<connected-to-pout_custom_readdata>),              //                 pout_custom.readdata
		.reset_reset_n                     (<connected-to-reset_reset_n>)                      //                       reset.reset_n
	);

