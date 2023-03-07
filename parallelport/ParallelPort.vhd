library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- Register map
-- 00 = Port values (RW)
-- 01 = set bit access (W)
-- 10 = clear bit access (W)


entity ParallelPort is
	port(
		Clk			:	IN		std_logic;
		nReset		:	IN		std_logic;
		-- Avalon Slave interface signals
		Address		:	IN		std_logic_vector(1 downto 0);
		Read			:	IN		std_logic;
		ReadData		:	OUT	std_logic_vector(31 downto 0);
		Write			:	IN		std_logic;
		WriteData	:	IN	std_logic_vector(31 downto 0);
		-- Port output
		Pout			:	OUT	std_logic_vector(31 downto 0)
	);
end ParallelPort;

architecture comp of ParallelPort is
	signal	iPortValues	:	std_logic_vector(31 downto 0);
	
	begin
	
	-- Read process
	-- Any read request will return the port values, regardless of address
	-- Drives no signals
	Read_process:
	process(Clk)
	begin
		if rising_edge(Clk) then
			if Read = '1' then
				ReadData <= iPortValues;
			end if;
		end if;
	end process Read_process;
	
	-- Write process (also handles asynchronous reset)
	-- Writes, sets and clears bits in the Port values register
	-- Drives iPortValues
	Write_process:
	process(Clk, nReset)
	begin
		if nReset = '0' then
			iPortValues <= (others => '0');
		elsif rising_edge(Clk) then
			-- Write cycle
			if Write = '1' then
				case Address(1 downto 0)  is
					when "00" =>
						iPortValues <= WriteData;
					when "01" =>
						-- Set bits: the corresponding bits are set to 1, 
						-- the others are left untouched
						iPortValues <= (iPortValues OR WriteData);
					when "10" =>
						-- Clear bits: the corresponding bits are cleared,
						-- the others are left untouched
						iPortValues <= (iPortValues AND NOT WriteData);
					when others => null;
				end case;
			end if;
		end if;
	end process Write_process;
	
	-- Set port values
	Pout <= iPortValues;
	
end comp;
			