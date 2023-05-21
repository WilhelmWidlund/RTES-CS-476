library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity SigProcOperation is
	port(
		Param		:	in std_logic_vector(31 downto 0);
		InData	:	in std_logic_vector(31 downto 0);
		OutData	:	out std_logic_vector(31 downto 0)
	);
end SigProcOperation;

architecture design of SigProcOperation is
	begin
		-- Choose volume up or down
		with Param(0) select OutData <=
			-- Increase the volume
			 std_logic_vector(shift_left(signed(InData), 1)) when '1',
			-- Decrease the volume
			std_logic_vector(shift_right(signed(InData), 1)) when '0',
			InData when others;
end design;