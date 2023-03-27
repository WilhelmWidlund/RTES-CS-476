library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_misc.all;

-- Register map
-- 000 = Port values (RW)
-- 001 = set bit access (W)
-- 010 = clear bit access (W)
-- 011 = IRQ enable (RW)
-- 100 = IRQ clear (W)

entity ParallelPort is
	port(
		Clk			:	IN		std_logic;
		nReset		:	IN		std_logic;
		-- Avalon Slave interface signals
		Address		:	IN		std_logic_vector(2 downto 0);
		Read			:	IN		std_logic;
		ReadData		:	OUT	std_logic_vector(31 downto 0);
		Write			:	IN		std_logic;
		WriteData	:	IN		std_logic_vector(31 downto 0);
		-- Port output
		Pout			:	OUT	std_logic_vector(31 downto 0);
		-- Interrupt Request signal
		IRQ			:	OUT	std_logic
	);
end ParallelPort;

architecture comp of ParallelPort is
	signal	iPortValues	:	std_logic_vector(31 downto 0);
	signal	iIRQEN		:	std_logic_vector(31 downto 0);
	signal	iIRQCLEAR	:	std_logic;
	
	begin
	
	-- Read process
	-- Any read request will return the port values, regardless of address
	-- Drives no signals
	Read_process:
	process(Clk)
	begin
		if rising_edge(Clk) then
			if Read = '1' then
				case Address(2 downto 0)  is
					when "000" =>
						ReadData <= iPortValues;
					when "011" =>
						ReadData <= iIRQEN;
					when others => null;
				end case;
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
			iIRQCLEAR <= '0';
			if Write = '1' then
				case Address(2 downto 0)  is
					when "000" =>
						iPortValues <= WriteData;
					when "001" =>
						-- Set bits: the corresponding bits are set to 1, 
						-- the others are left untouched
						iPortValues <= (iPortValues OR WriteData);
					when "010" =>
						-- Clear bits: the corresponding bits are cleared,
						-- the others are left untouched
						iPortValues <= (iPortValues AND NOT WriteData);
					when "011" =>
						-- Toggle interrupt enabled or disabled
						iIRQEN <= WriteData;
					when "100" =>
						iIRQCLEAR <= '1';
					when others => null;
				end case;
			end if;
		end if;
	end process Write_process;
	
	Interrupt_process:
	process(Clk, iPortValues, iIRQCLEAR)
	begin
		if rising_edge(Clk) then
			if( or_reduce(iPortValues AND iIRQEN) = '1') then
				IRQ <= '1';
			end if;
			if(iIRQCLEAR = '1') then
				IRQ <= '0';
			end if;
		end if;
	end process Interrupt_process;
	
	
	-- Set port values
	Pout <= iPortValues;
	
end comp;
			