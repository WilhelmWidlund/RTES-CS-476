	component soc_system is
		port (
			accelerator_0_conduit_end_readdata : out std_logic_vector(7 downto 0);        -- readdata
			clk_clk                            : in  std_logic                    := 'X'; -- clk
			reset_reset_n                      : in  std_logic                    := 'X'  -- reset_n
		);
	end component soc_system;

	u0 : component soc_system
		port map (
			accelerator_0_conduit_end_readdata => CONNECTED_TO_accelerator_0_conduit_end_readdata, -- accelerator_0_conduit_end.readdata
			clk_clk                            => CONNECTED_TO_clk_clk,                            --                       clk.clk
			reset_reset_n                      => CONNECTED_TO_reset_reset_n                       --                     reset.reset_n
		);

