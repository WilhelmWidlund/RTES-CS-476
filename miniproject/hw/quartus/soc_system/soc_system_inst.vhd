	component soc_system is
		port (
			clk_clk                                        : in    std_logic                     := 'X';             -- clk
			in_switches_export                             : in    std_logic_vector(7 downto 0)  := (others => 'X'); -- export
			out_led_export                                 : out   std_logic_vector(9 downto 0);                     -- export
			pio_1st_7seg_external_connection_export        : out   std_logic_vector(13 downto 0);                    -- export
			pio_2nd_7seg_external_connection_export        : out   std_logic_vector(13 downto 0);                    -- export
			pio_3rd_7seg_external_connection_export        : out   std_logic_vector(13 downto 0);                    -- export
			pll_shared_outclk2_clk                         : out   std_logic;                                        -- clk
			reset_reset_n                                  : in    std_logic                     := 'X';             -- reset_n
			sdram_controller_shared_wire_addr              : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_controller_shared_wire_ba                : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_controller_shared_wire_cas_n             : out   std_logic;                                        -- cas_n
			sdram_controller_shared_wire_cke               : out   std_logic;                                        -- cke
			sdram_controller_shared_wire_cs_n              : out   std_logic;                                        -- cs_n
			sdram_controller_shared_wire_dq                : inout std_logic_vector(31 downto 0) := (others => 'X'); -- dq
			sdram_controller_shared_wire_dqm               : out   std_logic_vector(3 downto 0);                     -- dqm
			sdram_controller_shared_wire_ras_n             : out   std_logic;                                        -- ras_n
			sdram_controller_shared_wire_we_n              : out   std_logic;                                        -- we_n
			sysaudio_audio_core_external_interface_ADCDAT  : in    std_logic                     := 'X';             -- ADCDAT
			sysaudio_audio_core_external_interface_ADCLRCK : in    std_logic                     := 'X';             -- ADCLRCK
			sysaudio_audio_core_external_interface_BCLK    : in    std_logic                     := 'X';             -- BCLK
			sysaudio_audio_core_external_interface_DACDAT  : out   std_logic;                                        -- DACDAT
			sysaudio_audio_core_external_interface_DACLRCK : in    std_logic                     := 'X';             -- DACLRCK
			sysaudio_av_config_external_interface_SDAT     : inout std_logic                     := 'X';             -- SDAT
			sysaudio_av_config_external_interface_SCLK     : out   std_logic                                         -- SCLK
		);
	end component soc_system;

	u0 : component soc_system
		port map (
			clk_clk                                        => CONNECTED_TO_clk_clk,                                        --                                    clk.clk
			in_switches_export                             => CONNECTED_TO_in_switches_export,                             --                            in_switches.export
			out_led_export                                 => CONNECTED_TO_out_led_export,                                 --                                out_led.export
			pio_1st_7seg_external_connection_export        => CONNECTED_TO_pio_1st_7seg_external_connection_export,        --       pio_1st_7seg_external_connection.export
			pio_2nd_7seg_external_connection_export        => CONNECTED_TO_pio_2nd_7seg_external_connection_export,        --       pio_2nd_7seg_external_connection.export
			pio_3rd_7seg_external_connection_export        => CONNECTED_TO_pio_3rd_7seg_external_connection_export,        --       pio_3rd_7seg_external_connection.export
			pll_shared_outclk2_clk                         => CONNECTED_TO_pll_shared_outclk2_clk,                         --                     pll_shared_outclk2.clk
			reset_reset_n                                  => CONNECTED_TO_reset_reset_n,                                  --                                  reset.reset_n
			sdram_controller_shared_wire_addr              => CONNECTED_TO_sdram_controller_shared_wire_addr,              --           sdram_controller_shared_wire.addr
			sdram_controller_shared_wire_ba                => CONNECTED_TO_sdram_controller_shared_wire_ba,                --                                       .ba
			sdram_controller_shared_wire_cas_n             => CONNECTED_TO_sdram_controller_shared_wire_cas_n,             --                                       .cas_n
			sdram_controller_shared_wire_cke               => CONNECTED_TO_sdram_controller_shared_wire_cke,               --                                       .cke
			sdram_controller_shared_wire_cs_n              => CONNECTED_TO_sdram_controller_shared_wire_cs_n,              --                                       .cs_n
			sdram_controller_shared_wire_dq                => CONNECTED_TO_sdram_controller_shared_wire_dq,                --                                       .dq
			sdram_controller_shared_wire_dqm               => CONNECTED_TO_sdram_controller_shared_wire_dqm,               --                                       .dqm
			sdram_controller_shared_wire_ras_n             => CONNECTED_TO_sdram_controller_shared_wire_ras_n,             --                                       .ras_n
			sdram_controller_shared_wire_we_n              => CONNECTED_TO_sdram_controller_shared_wire_we_n,              --                                       .we_n
			sysaudio_audio_core_external_interface_ADCDAT  => CONNECTED_TO_sysaudio_audio_core_external_interface_ADCDAT,  -- sysaudio_audio_core_external_interface.ADCDAT
			sysaudio_audio_core_external_interface_ADCLRCK => CONNECTED_TO_sysaudio_audio_core_external_interface_ADCLRCK, --                                       .ADCLRCK
			sysaudio_audio_core_external_interface_BCLK    => CONNECTED_TO_sysaudio_audio_core_external_interface_BCLK,    --                                       .BCLK
			sysaudio_audio_core_external_interface_DACDAT  => CONNECTED_TO_sysaudio_audio_core_external_interface_DACDAT,  --                                       .DACDAT
			sysaudio_audio_core_external_interface_DACLRCK => CONNECTED_TO_sysaudio_audio_core_external_interface_DACLRCK, --                                       .DACLRCK
			sysaudio_av_config_external_interface_SDAT     => CONNECTED_TO_sysaudio_av_config_external_interface_SDAT,     --  sysaudio_av_config_external_interface.SDAT
			sysaudio_av_config_external_interface_SCLK     => CONNECTED_TO_sysaudio_av_config_external_interface_SCLK      --                                       .SCLK
		);

