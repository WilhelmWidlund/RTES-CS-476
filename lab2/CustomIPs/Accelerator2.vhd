library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity HwAccelerator2 is
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
		DEBUGCOUNT				:			OUT	STD_LOGIC_VECTOR(15 downto 0)
	);
end HwAccelerator2;

architecture comp of HwAccelerator2 is
	-- FSM stuff
	TYPE State_type is (Idle, DMALoadRequest, DMALoadOngoing, DMALoadReceive, Operation, DMAStoreRequest, DMAStoreOngoing, CheckDone, Done);
	SIGNAL	State, State_Next			:		State_type;
	-- Internal registers
	SIGNAL	iStartAddr, iStoreAddr, iStartAddr_Next, iStoreAddr_Next	:	STD_LOGIC_VECTOR(31 downto 0);
	SIGNAL	iNum, iNum_Next				:	unsigned(15 downto 0); -- Range 0 to 1023
	SIGNAL	iCount, iCount_Next			:	unsigned(15 downto 0); -- Range 0 to 1023
	SIGNAL	iInData, iInData_Next			:	STD_LOGIC_VECTOR(31 downto 0);
	SIGNAL	iOutData, iOutData_Next			:	STD_LOGIC_VECTOR(31 downto 0);
	SIGNAL	iStart, iStart_Next			:	STD_LOGIC_VECTOR(2 downto 0);
	SIGNAL	iStop, iStop_Next				:	STD_LOGIC;
	SIGNAL 	iWorking, iWorking_Next			:	STD_LOGIC;
	
begin
		Next_Process: process(Clk, Reset_n)
		begin
		if Reset_n = '0' then
			DEBUGCOUNT <= "1111111111111111";
			State <= Idle;
			iStartAddr <= (others => '0');
			iNum <= (others => '0');
			iInData <= (others => '0');
			iOutData <= (others => '0');
			iStart <= (others => '0');
			iCount <= (others => '0');
			iStop <= '0';
			iWorking <= '0';
		elsif rising_edge(Clk) then
			DEBUGCOUNT <= std_logic_vector(iCount);
			
			State <= State_Next;
			iStartAddr <= iStartAddr_Next;
			iStoreAddr <= iStoreAddr_Next;
			iNum <= iNum_Next;
			iCount <= iCount_Next;
			iInData <= iInData_Next;
			iOutData <= iOutData_Next;
			iStart <= iStart_Next;
			iStop <= iStop_Next;
			iWorking <= iWorking_Next;
		end if;
		end process Next_Process;


		-- internal signal process
		Signal_Proc: process(State, Reset_n)
			begin
				iCount_Next <= iCount;
				iStop_Next <= iStop;
				iWorking_Next <= iWorking;
				iInData_Next <= iInData;
				iOutData_Next <= iOutData;
				if Reset_n = '0' then
					iCount_Next <= (others => '0');
					iStop_Next <= '0';
				else
					case State is
						when Idle =>
							iWorking_Next <= '0';
						when DMALoadRequest =>
							iWorking_Next <= '1';
							
						when DMALoadOngoing =>
							iWorking_Next <= '1';
						when DMALoadReceive =>
							iWorking_Next <= '1';
							if AM_RDataValid = '1' then
								iInData_Next <= AM_RData;
							end if;
						when Operation =>
							iWorking_Next <= '1';
							-- Perform the operation, then move on
							iOutData_Next(31 downto 24) <= iInData(7 downto 0);
							iOutData_Next(7 downto 0) <= iInData(31 downto 24);
							for i in 8 to 23 loop
								iOutData_Next(31 - i) <= iInData(i);
							end loop;
						when DMAStoreRequest =>
							iWorking_Next <= '1';
						when DMAStoreOngoing =>
							iWorking_Next <= '1';
						when CheckDone =>
							iWorking_Next <= '1';
							iCount_Next <= iCount + 1;
							if iCount = (iNum - 1) then
								-- Move to Done
								iCount_Next <= (others => '0');
								iStop_Next <= '1';
							end if;
						when Done =>
							iWorking_Next <= '0';
							iStop_Next <= '1';
						when others => null;
					end case;
				end if;
			end process Signal_Proc;
		
		-- Drives Avalon master interface, State_Next
		-- Drives DEBUG
		FSMMAIN: process(State, Reset_n)
			begin
				-- Keep current values unless a change is called for
				State_Next <= State;
				-- Don't use master interface by default
				AM_Wr <= '0';
				AM_Addr <= (others => '0');
				AM_BE <= (others => '1');
				AM_WData <= (others => '0');
				AM_Rd <= '0';				
				if Reset_n = '0' then
					DEBUGSTATE <= (others => '1');
					State_Next <= Idle;
					AM_Wr <= '0';
					AM_Addr <= (others => '0');
					AM_BE <= (others => '1');
					AM_WData <= (others => '0');
					AM_Rd <= '0';
				else
					case State is
						when Idle =>
							DEBUGSTATE <= "00000000";
							-- Start the process when all three arguments have been received
							if iStart = "111" then
								State_Next <= DMALoadRequest;
							end if;
						when DMALoadRequest =>
							DEBUGSTATE <= "00000001";
							-- Initialize request to read the next indata word from memory
							AM_Rd <= '1';
							AM_BE <= (others => '1');
							AM_Addr <= std_logic_vector(unsigned(iStartAddr) + iCount*4);
							-- Move on if the read request was granted
							State_Next <= DMALoadOngoing;
						when DMALoadOngoing =>
							DEBUGSTATE <= "00000010";
							-- Continue requesting to read the next indata word from memory
							AM_Rd <= '1';
							AM_BE <= (others => '1');
							AM_Addr <= std_logic_vector(unsigned(iStartAddr) + iCount*4);
							-- Move on if the read request was granted
							if AM_WaitRq = '0' then
								State_Next <= DMALoadReceive;
							end if;
						when DMALoadReceive =>
							DEBUGSTATE <= "00000100";
							-- Wait for the read data to be received, then move on
							if AM_RDataValid = '1' then
								State_Next <= Operation;
							end if;
						when Operation =>
							DEBUGSTATE <= "00001000";
							State_Next <= DMAStoreRequest;
						when DMAStoreRequest =>
							DEBUGSTATE <= "00010000";
							-- Request to store the outdata in memory
							AM_Wr <= '1';
							AM_Addr <= std_logic_vector(unsigned(iStoreAddr) + iCount*4);
							AM_BE <= (others => '1');
							AM_WData <= iOutData;
							State_Next <= DMAStoreOngoing;
						when DMAStoreOngoing =>
							DEBUGSTATE <= "00100000";
							-- Continue requesting to store the outdata in memory
							AM_Wr <= '1';
							AM_Addr <= std_logic_vector(unsigned(iStoreAddr) + iCount*4);
							AM_BE <= (others => '1');
							AM_WData <= iOutData;
							-- Move on when the request is granted
							if AM_WaitRq = '0' then
								State_Next <= CheckDone;
							end if;
						when CheckDone =>
							DEBUGSTATE <= "01000000";
							-- Check if the process is done
							if iCount = (iNum - 1) then
								-- Move to Done
								State_Next <= Done;
							else
								-- Proceed with next word
								State_Next <= DMALoadRequest;
							end if;
						when Done =>
							DEBUGSTATE <= "10000000";
							-- This state delays 1 clock cycle for the flags to be reset, before returning to Idle
							-- Also keeps iStop high for one extra clock cycle
							State_Next <= Idle;
						when others => null;
					end case;
				end if;
		end process FSMMAIN;


		-- Avalon slave write
		-- Drives Avalon slave write interface, iStart_Next, iStartAddr_Next, iStoreAddr_Next, iNum_Next
		AS_WriteProc: process(Clk, Reset_n, iStop, iStartAddr, iStoreAddr, iNum, iStart) is
		begin
			if Reset_n = '0' then
				iStart_Next <= (others => '0');
			elsif rising_edge(Clk) then
				--iStartAddr_Next <= iStartAddr;
				--iStoreAddr_Next <= iStoreAddr;
				--iNum_Next <= iNum;
				--iStart_Next <= iStart;
				-- Check for stopping condition
				if iWorking = '1' then
					iStart_Next <= (others => '0');
				end if;
				if AS_Wr = '1' then
					case AS_Addr is
						when "00" => 
							iStartAddr_Next <= AS_WData;
							iStart_Next(0) <= '1';
						when "01" =>
							iStoreAddr_Next <= AS_WData;
							iStart_Next(1) <= '1';
						when "10" =>
							iNum_Next <= unsigned(AS_WData(15 downto 0));
							iStart_Next(2) <= '1';
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