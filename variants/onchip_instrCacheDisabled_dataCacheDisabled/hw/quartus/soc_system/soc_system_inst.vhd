	component soc_system is
		port (
			clk_clk              : in  std_logic                     := 'X';             -- clk
			pin_buttons_export   : in  std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			pout_custom_readdata : out std_logic_vector(31 downto 0);                    -- readdata
			reset_reset_n        : in  std_logic                     := 'X'              -- reset_n
		);
	end component soc_system;

	u0 : component soc_system
		port map (
			clk_clk              => CONNECTED_TO_clk_clk,              --         clk.clk
			pin_buttons_export   => CONNECTED_TO_pin_buttons_export,   -- pin_buttons.export
			pout_custom_readdata => CONNECTED_TO_pout_custom_readdata, -- pout_custom.readdata
			reset_reset_n        => CONNECTED_TO_reset_reset_n         --       reset.reset_n
		);

