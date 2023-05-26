	component soc_system is
		port (
			clk_clk                                          : in    std_logic                     := 'X';             -- clk
			pio_2_export                                     : in    std_logic_vector(7 downto 0)  := (others => 'X'); -- export
			pll_2_outclk2_clk                                : out   std_logic;                                        -- clk
			pp0_out_export                                   : out   std_logic_vector(7 downto 0);                     -- export
			pp1_out_export                                   : out   std_logic_vector(7 downto 0);                     -- export
			pp2_out_export                                   : out   std_logic_vector(7 downto 0);                     -- export
			reset_reset_n                                    : in    std_logic                     := 'X';             -- reset_n
			sdram_controller_2_wire_addr                     : out   std_logic_vector(12 downto 0);                    -- addr
			sdram_controller_2_wire_ba                       : out   std_logic_vector(1 downto 0);                     -- ba
			sdram_controller_2_wire_cas_n                    : out   std_logic;                                        -- cas_n
			sdram_controller_2_wire_cke                      : out   std_logic;                                        -- cke
			sdram_controller_2_wire_cs_n                     : out   std_logic;                                        -- cs_n
			sdram_controller_2_wire_dq                       : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			sdram_controller_2_wire_dqm                      : out   std_logic_vector(1 downto 0);                     -- dqm
			sdram_controller_2_wire_ras_n                    : out   std_logic;                                        -- ras_n
			sdram_controller_2_wire_we_n                     : out   std_logic;                                        -- we_n
			audio_0_external_interface_ADCDAT                : in    std_logic                     := 'X';             -- ADCDAT
			audio_0_external_interface_ADCLRCK               : in    std_logic                     := 'X';             -- ADCLRCK
			audio_0_external_interface_BCLK                  : in    std_logic                     := 'X';             -- BCLK
			audio_0_external_interface_DACDAT                : out   std_logic;                                        -- DACDAT
			audio_0_external_interface_DACLRCK               : in    std_logic                     := 'X';             -- DACLRCK
			audio_and_video_config_0_external_interface_SDAT : inout std_logic                     := 'X';             -- SDAT
			audio_and_video_config_0_external_interface_SCLK : out   std_logic;                                        -- SCLK
			audio_pll_0_audio_clk_clk                        : out   std_logic                                         -- clk
		);
	end component soc_system;

	u0 : component soc_system
		port map (
			clk_clk                                          => CONNECTED_TO_clk_clk,                                          --                                         clk.clk
			pio_2_export                                     => CONNECTED_TO_pio_2_export,                                     --                                       pio_2.export
			pll_2_outclk2_clk                                => CONNECTED_TO_pll_2_outclk2_clk,                                --                               pll_2_outclk2.clk
			pp0_out_export                                   => CONNECTED_TO_pp0_out_export,                                   --                                     pp0_out.export
			pp1_out_export                                   => CONNECTED_TO_pp1_out_export,                                   --                                     pp1_out.export
			pp2_out_export                                   => CONNECTED_TO_pp2_out_export,                                   --                                     pp2_out.export
			reset_reset_n                                    => CONNECTED_TO_reset_reset_n,                                    --                                       reset.reset_n
			sdram_controller_2_wire_addr                     => CONNECTED_TO_sdram_controller_2_wire_addr,                     --                     sdram_controller_2_wire.addr
			sdram_controller_2_wire_ba                       => CONNECTED_TO_sdram_controller_2_wire_ba,                       --                                            .ba
			sdram_controller_2_wire_cas_n                    => CONNECTED_TO_sdram_controller_2_wire_cas_n,                    --                                            .cas_n
			sdram_controller_2_wire_cke                      => CONNECTED_TO_sdram_controller_2_wire_cke,                      --                                            .cke
			sdram_controller_2_wire_cs_n                     => CONNECTED_TO_sdram_controller_2_wire_cs_n,                     --                                            .cs_n
			sdram_controller_2_wire_dq                       => CONNECTED_TO_sdram_controller_2_wire_dq,                       --                                            .dq
			sdram_controller_2_wire_dqm                      => CONNECTED_TO_sdram_controller_2_wire_dqm,                      --                                            .dqm
			sdram_controller_2_wire_ras_n                    => CONNECTED_TO_sdram_controller_2_wire_ras_n,                    --                                            .ras_n
			sdram_controller_2_wire_we_n                     => CONNECTED_TO_sdram_controller_2_wire_we_n,                     --                                            .we_n
			audio_0_external_interface_ADCDAT                => CONNECTED_TO_audio_0_external_interface_ADCDAT,                --                  audio_0_external_interface.ADCDAT
			audio_0_external_interface_ADCLRCK               => CONNECTED_TO_audio_0_external_interface_ADCLRCK,               --                                            .ADCLRCK
			audio_0_external_interface_BCLK                  => CONNECTED_TO_audio_0_external_interface_BCLK,                  --                                            .BCLK
			audio_0_external_interface_DACDAT                => CONNECTED_TO_audio_0_external_interface_DACDAT,                --                                            .DACDAT
			audio_0_external_interface_DACLRCK               => CONNECTED_TO_audio_0_external_interface_DACLRCK,               --                                            .DACLRCK
			audio_and_video_config_0_external_interface_SDAT => CONNECTED_TO_audio_and_video_config_0_external_interface_SDAT, -- audio_and_video_config_0_external_interface.SDAT
			audio_and_video_config_0_external_interface_SCLK => CONNECTED_TO_audio_and_video_config_0_external_interface_SCLK, --                                            .SCLK
			audio_pll_0_audio_clk_clk                        => CONNECTED_TO_audio_pll_0_audio_clk_clk                         --                       audio_pll_0_audio_clk.clk
		);

