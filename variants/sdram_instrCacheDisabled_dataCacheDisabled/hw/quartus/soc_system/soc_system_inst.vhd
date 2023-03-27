	component soc_system is
		port (
			clk_clk                           : in    std_logic                     := 'X';             -- clk
			new_sdram_controller_0_wire_addr  : out   std_logic_vector(12 downto 0);                    -- addr
			new_sdram_controller_0_wire_ba    : out   std_logic_vector(1 downto 0);                     -- ba
			new_sdram_controller_0_wire_cas_n : out   std_logic;                                        -- cas_n
			new_sdram_controller_0_wire_cke   : out   std_logic;                                        -- cke
			new_sdram_controller_0_wire_cs_n  : out   std_logic;                                        -- cs_n
			new_sdram_controller_0_wire_dq    : inout std_logic_vector(15 downto 0) := (others => 'X'); -- dq
			new_sdram_controller_0_wire_dqm   : out   std_logic_vector(1 downto 0);                     -- dqm
			new_sdram_controller_0_wire_ras_n : out   std_logic;                                        -- ras_n
			new_sdram_controller_0_wire_we_n  : out   std_logic;                                        -- we_n
			pin_buttons_export                : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			pll_0_sdram_clk                   : out   std_logic;                                        -- clk
			pout_custom_readdata              : out   std_logic_vector(31 downto 0);                    -- readdata
			reset_reset_n                     : in    std_logic                     := 'X'              -- reset_n
		);
	end component soc_system;

	u0 : component soc_system
		port map (
			clk_clk                           => CONNECTED_TO_clk_clk,                           --                         clk.clk
			new_sdram_controller_0_wire_addr  => CONNECTED_TO_new_sdram_controller_0_wire_addr,  -- new_sdram_controller_0_wire.addr
			new_sdram_controller_0_wire_ba    => CONNECTED_TO_new_sdram_controller_0_wire_ba,    --                            .ba
			new_sdram_controller_0_wire_cas_n => CONNECTED_TO_new_sdram_controller_0_wire_cas_n, --                            .cas_n
			new_sdram_controller_0_wire_cke   => CONNECTED_TO_new_sdram_controller_0_wire_cke,   --                            .cke
			new_sdram_controller_0_wire_cs_n  => CONNECTED_TO_new_sdram_controller_0_wire_cs_n,  --                            .cs_n
			new_sdram_controller_0_wire_dq    => CONNECTED_TO_new_sdram_controller_0_wire_dq,    --                            .dq
			new_sdram_controller_0_wire_dqm   => CONNECTED_TO_new_sdram_controller_0_wire_dqm,   --                            .dqm
			new_sdram_controller_0_wire_ras_n => CONNECTED_TO_new_sdram_controller_0_wire_ras_n, --                            .ras_n
			new_sdram_controller_0_wire_we_n  => CONNECTED_TO_new_sdram_controller_0_wire_we_n,  --                            .we_n
			pin_buttons_export                => CONNECTED_TO_pin_buttons_export,                --                 pin_buttons.export
			pll_0_sdram_clk                   => CONNECTED_TO_pll_0_sdram_clk,                   --                 pll_0_sdram.clk
			pout_custom_readdata              => CONNECTED_TO_pout_custom_readdata,              --                 pout_custom.readdata
			reset_reset_n                     => CONNECTED_TO_reset_reset_n                      --                       reset.reset_n
		);

