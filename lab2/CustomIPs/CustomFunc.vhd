library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity CustomSwitcher is
	port(
		InData	:	in std_logic_vector(31 downto 0);
		OutData	:	out std_logic_vector(31 downto 0)
	);
end CustomSwitcher;

architecture design of CustomSwitcher is
	begin
		OutData(31 downto 24) <= InData(7 downto 0);
		OutData(7 downto 0) <= InData(31 downto 24);
		bitswap: for i in 8 to 23 generate
			OutData(31 - i) <= InData(i);
		end generate bitswap;
end design;