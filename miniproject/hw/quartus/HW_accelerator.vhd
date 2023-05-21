library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

entity HW_accelerator is
port (
	-- Clock & reset interface
	Clk 		: in std_logic;
	nReset 	: in std_logic;
	
	-- Avalon master interface
	AM_address 		: out std_logic_vector (31 downto 0);
	AM_byteenable 	: out std_logic_vector (3 downto 0);
	AM_write 		: out std_logic;
	AM_writedata 	: out std_logic_vector (31 downto 0);
	AM_read 			: out std_logic;
	AM_readdata 	: in std_logic_vector (31 downto 0);
	AM_waitrequest : in std_logic;
	
	-- Avalon slave interface
	AS_address		: in std_logic_vector (2 downto 0);
	AS_read 			: in std_logic;
	AS_readdata 	: out std_logic_vector (31 downto 0);
	AS_write 		: in std_logic;
	AS_writedata 	: in std_logic_vector (31 downto 0)
);
end HW_accelerator;

architecture design of HW_accelerator is

-- Define possible states
type all_states is (Idle, Setup, Init_Read, Finish_Read, Operation, Init_Write, Finish_Write);
signal State : all_states := Idle;

-- Define AS registers
signal StartAddrRead_Reg	: std_logic_vector(31 downto 0);
signal StartAddrWrite_Reg	: std_logic_vector(31 downto 0);
signal WordCount_Reg			: std_logic_vector(31 downto 0);
signal VolumeMode_Reg		: std_logic;
signal Working_Reg			: std_logic;
signal Start_Reg				: std_logic;

-- Address | Register
--     00  | Start Address for Reading
--     01  | Start Address for Writing
--     10  | Word count to process
--     11  | Status (bit 0 = VolumeMode_Reg. high -> volume up, bit 0 low -> volume down
--							bit 1 = Working_Reg. high -> working, bit 1 low -> Idle/Ready for new request)

-- Define internal registers
signal iCurrentAddrRead		: unsigned(31 downto 0);
signal iCurrentAddrWrite	: unsigned(31 downto 0);
signal iWordsLeftCount		: unsigned(31 downto 0);
signal iInData					: std_logic_vector(31 downto 0);
signal iOutData				: std_logic_vector(31 downto 0);

begin

-- Avalon slave write process
-- Drives the AS registers
AS_WriteProc: process(Clk, nReset)
begin
	if nReset = '0' then
		StartAddrRead_Reg <= (others => '0');
		StartAddrWrite_Reg <= (others => '0');
		WordCount_Reg <= (others => '0');
		VolumeMode_Reg <= '0';
		Start_Reg <= '0';
	elsif rising_edge(Clk) then
		Start_Reg <= '0';
		if AS_write = '1' then
			case AS_address is
				when "000" =>
					StartAddrRead_Reg <= AS_writedata;
				when "001" =>
					StartAddrWrite_Reg <= AS_writedata;
				when "010" =>
					WordCount_Reg <= AS_writedata;
				when "011" =>
					VolumeMode_Reg <= AS_writedata(0);
				when "100" =>
					Start_Reg <= '1';
				when others => null;
			end case;
		end if;
	end if;
end process AS_WriteProc;

-- Avalon slave read process
-- Drives AS_readdata
AS_ReadProc: process(Clk)
begin
	if rising_edge(Clk) then
		if AS_read = '1' then
			case AS_address is
				when "000" =>
					AS_readdata <= StartAddrRead_Reg;
				when "001" =>
					AS_readdata <= StartAddrWrite_Reg;
				when "010" =>
					AS_readdata <= WordCount_Reg;
				when "011" =>
					AS_readdata(1 downto 0) <= Working_Reg & VolumeMode_Reg;
					AS_readdata(31 downto 2) <= (others => '0');
				when others => null;
			end case;
		end if;
	end if;
end process AS_ReadProc;

-- Main finite state machine and Avalon master interface process
-- Drives State, Avalon master interface, Working_Reg, and internal register
StateProc: process(Clk, nReset)
begin
	if nReset = '0' then
		State <= Idle;
		iCurrentAddrRead <= (others => '0');
		iCurrentAddrWrite <= (others => '0');
		iWordsLeftCount <= (others => '0');
		iInData <= (others => '0');
		iOutData <= (others => '0');
		Working_Reg <= '0';
	elsif rising_edge(Clk) then
		case State is
			when Idle =>
				AM_address <= (others => '0');
				AM_byteenable <= (others => '0');
				AM_write <= '0';
				AM_writedata <= (others => '0');
				AM_read <= '0';
				Working_Reg <= '0';
				if Start_Reg = '1' then
					State <= Setup;
					Working_Reg <= '1';
				end if;
			when Setup =>
				iCurrentAddrRead <= unsigned(StartAddrRead_Reg);
				iCurrentAddrWrite <= unsigned(StartAddrWrite_Reg);
				iWordsLeftCount <= unsigned(WordCount_Reg);
				State <= Init_Read;
			when Init_Read =>
				AM_write <= '0';
				AM_read <= '1';
				AM_address <= std_logic_vector(iCurrentAddrRead);
				AM_byteenable <= (others => '1');
				State <= Finish_Read;
			when Finish_Read =>
				if AM_waitrequest = '0' then
					iInData <= AM_readdata;
					AM_read <= '0';
					State <= Operation;
				end if;
			when Operation =>
				case VolumeMode_Reg is
					when '0' =>
						iOutData <= std_logic_vector(shift_right(signed(iInData), 1));
					when '1' =>
						iOutData <= std_logic_vector(shift_left(signed(iInData), 1));
					when others =>
						iOutData <= iInData;
				end case;
				State <= Init_Write;
			when Init_Write =>
				AM_write <= '1';
				AM_read <= '0';
				AM_address <= std_logic_vector(iCurrentAddrWrite);
				AM_byteenable <= (others => '1');
				AM_writedata <= iOutData;
				State <= Finish_Write;
				iWordsLeftCount <= iWordsLeftCount - 1;
			when Finish_Write =>
				if AM_waitrequest = '0' then
					AM_write <= '0';
					if iWordsLeftCount > 0 then
						iCurrentAddrRead <= iCurrentAddrRead + 4;
						iCurrentAddrWrite <= iCurrentAddrWrite + 4;
						State <= Init_Read;
					else
						Working_Reg <= '0';
						State <= Idle;
					end if;
				end if;
		end case;
	end if;
end process StateProc;
		

end design;