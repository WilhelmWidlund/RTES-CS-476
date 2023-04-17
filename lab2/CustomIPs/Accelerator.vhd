library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity HwAccelerator is
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
		
		-- Debug PIO output signal
		DEBUG						:			OUT	STD_LOGIC_VECTOR(7 downto 0)
	);
end HwAccelerator;

architecture comp of HwAccelerator is
	-- FSM stuff
	TYPE State_type is (Idle, DMALoadRequest, DMALoadReceive, Operation, DMAStore, CheckDone);
	SIGNAL	State, Next_State			:		State_type;
	-- Internal registers
	SIGNAL	iStartAddr, iStoreAddr	:	STD_LOGIC_VECTOR(31 downto 0);
	SIGNAL	iNum				:	unsigned(9 downto 0); -- Range 0 to 1023
	SIGNAL	iCount, iCountNext			:	unsigned(9 downto 0); -- Range 0 to 1023
	SIGNAL	iInData			:	STD_LOGIC_VECTOR(31 downto 0);
	SIGNAL	iOutData			:	STD_LOGIC_VECTOR(31 downto 0);
	SIGNAL	iStart			:	STD_LOGIC_VECTOR(2 downto 0);
	SIGNAL	iStop				:	STD_LOGIC;
	
begin
		-- Drives Avalon master interface, Next_State, iCountNext, iStop
		-- Drives DEBUG
		FSMMAIN: process(State, Reset_n, iStart, AM_WaitRq, AM_RDataValid, iCountNext)
			begin
				-- Stay in the current state unless the continue condition is met
				Next_State <= State;
				-- Keep the current count as default
				iCountNext <= iCount;
				-- Don't stop by default
				iStop <= '0';
				-- Don't use master interface by default
				AM_Wr <= '0';
				AM_Addr <= (others => '0');
				AM_BE <= (others => '1');
				AM_WData <= (others => '0');
				AM_Rd <= '0';
				
				if Reset_n = '0' then
					Next_State <= Idle;
					iCountNext <= (others => '0');
					iStop <= '0';
					AM_Wr <= '0';
					AM_Addr <= (others => '0');
					AM_BE <= (others => '1');
					AM_WData <= (others => '0');
					AM_Rd <= '0';
				else
					case State is
						when Idle =>
							DEBUG <= "00000001";
							-- Start the process when all three arguments have been received
							if iStart = "111" then
								Next_State <= DMALoadRequest;
							end if;
						when DMALoadRequest =>
							DEBUG <= "00000010";
							-- Request to read the next indata word from memory
							AM_Rd <= '1';
							AM_BE <= (others => '1');
							AM_Addr <= std_logic_vector(unsigned(iStartAddr) + iCount*4);
							-- Move on if the read request was granted
							if AM_WaitRq = '0' then
								Next_State <= DMALoadReceive;
							end if;
						when DMALoadReceive =>
							DEBUG <= "00000011";
							-- Wait for the read data to be received, then move on
							if AM_RDataValid = '1' then
								iInData <= AM_RData;
								Next_State <= Operation;
							end if;
						when Operation =>
							DEBUG <= "00000100";
							-- Perform the operation, then move on
							iOutData(31 downto 24) <= iInData(7 downto 0);
							iOutData(7 downto 0) <= iInData(31 downto 24);
							for i in 8 to 23 loop
								iOutData(31 - i) <= iInData(i);
							end loop;
							Next_State <= DMAStore;
						when DMAStore =>
							DEBUG <= "00000101";
							-- Request to store the outdata in memory
							AM_Wr <= '1';
							AM_Addr <= std_logic_vector(unsigned(iStoreAddr) + iCount*4);
							AM_BE <= (others => '1');
							AM_WData <= iOutData;
							-- Move on when the request is granted
							if AM_WaitRq = '0' then
								iCountNext <= iCount + 1;
								Next_State <= CheckDone;
							end if;
						when CheckDone =>
							DEBUG <= "00000110";
							-- Check if the process is done
							if iCountNext = iNum then
								-- Move to idle
								Next_State <= Idle;
								iStop <= '1';
							else
								-- Proceed with next word
								Next_State <= DMALoadRequest;
							end if;
					end case;
				end if;
		end process FSMMAIN;


		-- State machine update process
		-- Drives State, iCount
		FSMUpdate: process(Clk, Reset_n) is
			Begin
				if Reset_n = '0' then
					State <= Idle;
				elsif rising_edge(Clk) then
					State <= Next_State;
					iCount <= iCountNext;		
				end if;
		end process FSMUpdate;


		-- Avalon slave write
		-- Drives Avalon slave interface, iStart
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
		
end comp;