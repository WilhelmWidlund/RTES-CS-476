library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- Register map:
-- 000 = Counter[31... 0]		Counter value (read access)
-- 001 = Rz				Reset the counter (write access)
-- 010 = Start				Start counting (write access)
-- 011 = Stop				Stop counting (write access)
-- 100 = Command[7... 0]		General command (RW access)
-- 101 = Status[7... 0]			General status (RW access)
-- 110 = Target[31... 0]		Count target value (RW access)
-- 111 = IncrVal[31... 0]		Increment by this amount once (write access)

entity Counter is
	PORT(
		Clk			:	IN		std_logic;
		nReset		:	IN		std_logic;
		-- Avalon Slave interface signals
		Address		:	IN		std_logic_vector(2 downto 0);
		Read			:	IN		std_logic;
		ReadData		:	OUT	std_logic_vector(31 downto 0);
		Write			:	IN		std_logic;
		WriteData	:	IN	std_logic_vector(31 downto 0);
		-- Interrupt Request signal
		IRQ			:	OUT	std_logic
	);
end Counter;

architecture comp of Counter is
	-- Counter
	signal 	iCounter	:	unsigned(31 downto 0);
	-- Enable count
	signal	iEn		:	std_logic;
	-- Reset count
	signal	iRz		:	std_logic;
	-- Has reached end
	signal	iEOT		:	std_logic;
	-- Clear has reached end
	signal	iClrEOT	:	std_logic;
	-- Interrupts enabled
	signal	iIRQEn	:	std_logic;
	-- Count target value
	signal	iTarget	:	std_logic_vector(31 downto 0);
	-- Increment/Decrement write flag
	signal	iIncr		: 	std_logic;
	signal	iDecr		:	std_logic;
	signal	iIncrVal	:	std_logic_vector(31 downto 0);
	
	begin
	-- Counter process
	-- Drives iCounter
	Counter_process:
	process(Clk)
	begin
		if rising_edge(Clk) then
			if iRz = '1' then
				-- Reset counter to zero
				iCounter <= (others => '0');
			elsif iEn = '1' then
				if iEOT = '0' then
					-- Increment counter by 1
					iCounter <= iCounter + 1;
				end if;
			elsif iIncr = '1' then
					-- Increment count
					iCounter <= iCounter + unsigned(iIncrVal);
			elsif iDecr = '1' then
					-- Decrement count
					iCounter <= iCounter - unsigned(iIncrVal);
			end if;
		end if;
	end process Counter_process;
	
	-- Read process
	-- Drives no signals
	Read_process:
	process(Clk)
	begin
		if rising_edge(Clk) then
			-- Default value
			
			-- TODO: should the default value be std_logic_vector(iCounter)
			-- to facilitate that 
			-- "The counter value must be readable at all times -> transfer
		   --	 the counter value at the start of the read cycle"
			
			ReadData <= (others => '0');
			-- Read cycle
			if Read = '1' then
				case Address(2 downto 0) is
					when "000" =>
						-- Read the counter register value
						ReadData <= std_logic_vector(iCounter);
					when "100" =>
						-- Read the command register value (which is only
						-- the iIRQEn value)
						ReadData(0) <= iIRQEn;
					when "101" =>
						-- Read the status register value (iEOT and iEn)
						ReadData(0) <= iEOT;
						ReadData(1) <= iEn;
					when "110" =>
						-- Read the count target value
						ReadData <= std_logic_vector(iTarget);
					when others => null;
				end case;
			end if;
		end if;
	end process Read_process;
	
	-- Write process (also handles asynchronous reset)
	-- Drives iEn, iRz, iIRQEn and iClrEOT
	Write_process:
	process(Clk, nReset)
	begin
		-- Asynchronous reset: counting disabled, counter is reset, interrupts disabled
		if nReset = '0' then
			iEn <= '0';
			iRz <= '1';
			iIncr <= '0';
			iDecr <= '0';
			iIRQEn <= '0';
			iClrEOT <= '1';
		elsif rising_edge(Clk) then
			-- Default values: don't reset or clear
			iRz <= '0';
			iClrEOT <= '0';
			iIncr <= '0';
			iDecr <= '0';
			-- Write cycle
			if Write = '1' then
				case Address(2 downto 0) is
					-- Commands: Writing to them just means to perform the
					-- command associated with the register, so WriteData 
					-- is not considered.
					when "001" =>
						-- Rz: reset counter command
						iRz <= '1';
						iClrEOT <= '1';
					when "010" =>
						-- Start: enable counting command
						iEn <= '1';
					when "011" =>
						-- Stop: disable counting command
						iEn <= '0';
					-- Registers: WriteData is considered
					when "100" =>
						-- Command register
						iIRQEn <= WriteData(0);
					when "101" =>
						-- Status register
						iClrEOT <= WriteData(0);
					when "110" =>
						-- Target value register
						iTarget <= WriteData;
						iClrEOT <= '1';
					when "111" =>
						iIncrVal <= WriteData;
						iIncr <= '1';
					when "000" =>
						iIncrVal <= WriteData;
						iDecr <= '1';
					when others => null;
				end case;
			end if;
		end if;
	end process Write_process;
	
	-- Interrupt process. End Of Time (iEOT) is activated when iCounter
	-- is at the value in iTarget
	-- Drives iEOT
	Interrupt_process:
	process(Clk)
	begin
		if rising_edge(Clk) then
			if std_logic_vector(iCounter) = iTarget then
				-- Flag End Of Time when counter = target
				iEOT <= '1';
			end if;
			if iClrEOT = '1' then
				-- Cleared by writing 1 to status(0)
				iEOT <= '0';
			end if;
		end if;
	end process Interrupt_process;
	
	-- IRQ activation. No process required since it's only one signal
	-- conditionally activated
	IRQ <= '1' when (iEOT = '1' and iIRQEn = '1' and iEN = '1') else '0';

end comp;