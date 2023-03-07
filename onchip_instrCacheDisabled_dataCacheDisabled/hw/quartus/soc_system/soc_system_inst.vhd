	component soc_system is
		port (
			clk_clk            : in  std_logic                     := 'X';             -- clk
			reset_reset_n      : in  std_logic                     := 'X';             -- reset_n
			custom_pp_readdata : out std_logic_vector(31 downto 0);                    -- readdata
			stock_pp_export    : in  std_logic_vector(3 downto 0)  := (others => 'X')  -- export
		);
	end component soc_system;

	u0 : component soc_system
		port map (
			clk_clk            => CONNECTED_TO_clk_clk,            --       clk.clk
			reset_reset_n      => CONNECTED_TO_reset_reset_n,      --     reset.reset_n
			custom_pp_readdata => CONNECTED_TO_custom_pp_readdata, -- custom_pp.readdata
			stock_pp_export    => CONNECTED_TO_stock_pp_export     --  stock_pp.export
		);

