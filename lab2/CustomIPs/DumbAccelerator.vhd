library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity DumbHwAccelerator is
	port(
		-- Main signals
		Clk						:			IN		STD_LOGIC;
		Reset_n					:			IN		STD_LOGIC;
		-- Avalon Master Interface
		AM_BE						:			OUT	STD_LOGIC_VECTOR(3 downto 0);
		AM_Addr					:			OUT	STD_LOGIC_VECTOR(31 downto 0);
		AM_Rd						:			OUT	STD_LOGIC;
		AM_Wr						:			OUT	STD_LOGIC;
		AM_WData					:			OUT	STD_LOGIC_VECTOR(31 downto 0);
		AM_WaitRq				:			IN		STD_LOGIC;
		AM_RDataValid			:			IN		STD_LOGIC;
		AM_RData					:			IN		STD_LOGIC_VECTOR(31 downto 0);
		-- Avalon slave interface
		AS_Addr					:			IN		STD_LOGIC_VECTOR(1 downto 0);
		AS_Wr						:			IN		STD_LOGIC;
		AS_WData					:			IN		STD_LOGIC_VECTOR(31 downto 0);
		As_Rd						:			IN		STD_LOGIC;
		AS_RData					:			OUT	STD_LOGIC_VECTOR(31 downto 0);
		
		
		-- Debug PIO output signal
		DEBUGSTATE				:			OUT	STD_LOGIC_VECTOR(7 downto 0);
		DEBUGCOUNT				:			OUT	STD_LOGIC_VECTOR(9 downto 0)
	);
end DumbHwAccelerator;

architecture comp of DumbHwAccelerator is
	-- FSM stuff
	TYPE State_type is (Idle, DMALoadRequest, DMALoadOngoing, DMALoadReceive, Operation, DMAStoreRequest, DMAStoreOngoing, CheckDone, Done);
	SIGNAL	State			:		State_type;
	-- Internal registers
	SIGNAL	iStartAddr, iStoreAddr	:	STD_LOGIC_VECTOR(31 downto 0);
	SIGNAL	iNum				:	unsigned(9 downto 0); -- Range 0 to 1023
	SIGNAL	iCurrentCount			:	unsigned(9 downto 0); -- Range 0 to 1023
	SIGNAL	iInData			:	STD_LOGIC_VECTOR(31 downto 0);
	SIGNAL	iOutData			:	STD_LOGIC_VECTOR(31 downto 0);
	SIGNAL	iStart			:	STD_LOGIC_VECTOR(2 downto 0);
	SIGNAL	iStop				:	STD_LOGIC;
	SIGNAL 	iWorking			:	STD_LOGIC;
	
begin
		-- Drives Avalon master interface, Next_State, iStop
		-- Drives DEBUG
		FSMMAIN: process(Clk, Reset_n)
			begin
				-- Don't stop by default
				iStop <= '0';
				-- Don't work by default
				iWorking <= '0';
				-- Don't use master interface by default
				AM_Wr <= '0';
				AM_Addr <= (others => '0');
				AM_BE <= (others => '1');
				AM_WData <= (others => '0');
				AM_Rd <= '0';
				if Reset_n = '0' then
					DEBUGSTATE <= (others => '1');
					iStop <= '0';
					AM_Wr <= '0';
					AM_Addr <= (others => '0');
					AM_BE <= (others => '1');
					AM_WData <= (others => '0');
					AM_Rd <= '0';
				elsif rising_edge(Clk) then
					case State is
						when Idle =>
							DEBUGSTATE <= "00000000";
							DEBUGCOUNT <= std_logic_vector(iCurrentCount);
							iWorking <= '0';
							-- Start the process when all three arguments have been received
							if iStart = "111" then
								State <= DMALoadRequest;
							end if;
						when DMALoadRequest =>
							DEBUGSTATE <= "00000001";
							DEBUGCOUNT <= std_logic_vector(iCurrentCount);
							iWorking <= '1';
							-- Initialize request to read the next indata word from memory
							AM_Rd <= '1';
							AM_BE <= (others => '1');
							AM_Addr <= std_logic_vector(unsigned(iStartAddr) + iCurrentCount*4);
							-- Move on if the read request was granted
							State <= DMALoadOngoing;
						when DMALoadOngoing =>
							DEBUGSTATE <= "00000010";
							DEBUGCOUNT <= std_logic_vector(iCurrentCount);
							iWorking <= '1';
							-- Continue requesting to read the next indata word from memory
							AM_Rd <= '1';
							AM_BE <= (others => '1');
							AM_Addr <= std_logic_vector(unsigned(iStartAddr) + iCurrentCount*4);
							-- Move on if the read request was granted
							if AM_WaitRq = '0' then
								State <= DMALoadReceive;
							end if;
						when DMALoadReceive =>
							DEBUGSTATE <= "00000100";
							iWorking <= '1';
							-- Wait for the read data to be received, then move on
							if AM_RDataValid = '1' then
								iInData <= AM_RData;
								State <= Operation;
							end if;
						when Operation =>
							DEBUGSTATE <= "00001000";
							DEBUGCOUNT <= std_logic_vector(iCurrentCount);
							iWorking <= '1';
							-- Perform the operation, then move on
							iOutData(31 downto 24) <= iInData(7 downto 0);
							iOutData(7 downto 0) <= iInData(31 downto 24);
							for i in 8 to 23 loop
								iOutData(31 - i) <= iInData(i);
							end loop;
							State <= DMAStoreRequest;
						when DMAStoreRequest =>
							DEBUGSTATE <= "00010000";
							DEBUGCOUNT <= std_logic_vector(iCurrentCount);
							iWorking <= '1';
							-- Request to store the outdata in memory
							AM_Wr <= '1';
							AM_Addr <= std_logic_vector(unsigned(iStoreAddr) + iCurrentCount*4);
							AM_BE <= (others => '1');
							AM_WData <= iOutData;
							State <= DMAStoreOngoing;
						when DMAStoreOngoing =>
							DEBUGSTATE <= "00100000";
							DEBUGCOUNT <= std_logic_vector(iCurrentCount);
							iWorking <= '1';
							-- Continue requesting to store the outdata in memory
							AM_Wr <= '1';
							AM_Addr <= std_logic_vector(unsigned(iStoreAddr) + iCurrentCount*4);
							AM_BE <= (others => '1');
							AM_WData <= iOutData;
							-- Move on when the request is granted
							if AM_WaitRq = '0' then
								--iCurrentCount <= iCurrentCount + 1;
								State <= CheckDone;
							end if;
						when CheckDone =>
							DEBUGSTATE <= "01000000";
							DEBUGCOUNT <= std_logic_vector(iCurrentCount);
							iWorking <= '1';
							-- Check if the process is done
							if iCurrentCount = (iNum - 1) then
								-- Move to Done
								State <= Done;
								iStop <= '1';
							else
								-- Proceed with next word
								State <= DMALoadRequest;
							end if;
						when Done =>
							DEBUGSTATE <= "10000000";
							DEBUGCOUNT <= std_logic_vector(iCurrentCount);
							-- This state delays 1 clock cycle for the flags to be reset, before returning to Idle
							-- Also keeps iStop high for one extra clock cycle
							State <= Idle;
							iStop <= '1';
							iWorking <= '0';
						when others => null;
					end case;
				end if;
		end process FSMMAIN;
		
		-- Counter update process
		CountProc: process(State, Reset_n) is
		begin
			if Reset_n = '0' then
				iCurrentCount <= (others => '0');
			else
				case State is
					when Idle =>
						iCurrentCount <= (others => '0');
					when CheckDone =>
						iCurrentCount <= iCurrentCount + 1;
					when others =>
						iCurrentCount <= iCurrentCount;
				end case;
			end if;
		end process CountProc;

		-- Avalon slave write
		-- Drives Avalon slave write interface, iStart
		AS_WriteProc: process(Clk, Reset_n, iStop) is
		begin
			if Reset_n = '0' then
				iStart <= (others => '0');
			elsif rising_edge(Clk) then
				-- Check for stopping condition
				if iStop = '1' then
					iStart <= (others => '0');
				end if;
				if AS_Wr = '1' then
					case AS_Addr is
						when "00" => 
							iStartAddr <= AS_WData;
							iStart(0) <= '1';
						when "01" =>
							iStoreAddr <= AS_WData;
							iStart(1) <= '1';
						when "10" =>
							iNum <= unsigned(AS_WData(9 downto 0));
							iStart(2) <= '1';
						when others => null;
					end case;
				end if;
			end if;
		end process AS_WriteProc;
		
		-- Avalon slave read
		-- Drives Avalon slave read interface
		AS_ReadProc: process(Clk, Reset_n) is
		begin
			if Reset_n = '0' then
				AS_RData <= (others => '0');
			elsif rising_edge(Clk) then
				AS_RData <= (others => '0');
				if(AS_Rd = '1') then
					AS_RData(0) <= iStop;
					AS_RData(1) <= iWorking;
				end if;
			end if;
		end process AS_ReadProc;
		
end comp;