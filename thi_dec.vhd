library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity thi_dec is 
	port(
		n : in std_logic_vector(3 downto 0);
		dig : out std_logic_vector(7 downto 0));
end entity;

architecture behavioral of thi_dec is
begin

	process(n)
	begin
	
		case n is
		when "0000" => 
			dig <= "11111111";
			
		when "0001" => 
			dig <= "10111111";
			
		when others =>
			dig <= "00000000";
			
	end case;
	
	end process;
end behavioral;