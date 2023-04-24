	component soc_system is
		port (
			accelerator_v2_0_debug_count_readdata : out std_logic_vector(9 downto 0);        -- readdata
			accelerator_v2_0_debug_state_readdata : out std_logic_vector(7 downto 0);        -- readdata
			clk_clk                               : in  std_logic                    := 'X'; -- clk
			reset_reset_n                         : in  std_logic                    := 'X'  -- reset_n
		);
	end component soc_system;

	u0 : component soc_system
		port map (
			accelerator_v2_0_debug_count_readdata => CONNECTED_TO_accelerator_v2_0_debug_count_readdata, -- accelerator_v2_0_debug_count.readdata
			accelerator_v2_0_debug_state_readdata => CONNECTED_TO_accelerator_v2_0_debug_state_readdata, -- accelerator_v2_0_debug_state.readdata
			clk_clk                               => CONNECTED_TO_clk_clk,                               --                          clk.clk
			reset_reset_n                         => CONNECTED_TO_reset_reset_n                          --                        reset.reset_n
		);

