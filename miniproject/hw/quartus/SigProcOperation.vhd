library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity SigProcOperation is
	port(
		InData	:	in std_logic_vector(31 downto 0);
		OutData	:	out std_logic_vector(31 downto 0)
	);
end SigProcOperation;

architecture design of SigProcOperation is
	begin
		-- Placeholder bitswap operation 
		bitswap: for i in 0 to 31 generate
			OutData(31 - i) <= InData(i);
		end generate bitswap;
end design;