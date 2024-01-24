library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity sec_dec is 
	port(
		n : in std_logic_vector(3 downto 0);
		dig : out std_logic_vector(7 downto 0));
end entity;

architecture behavioral of sec_dec is
begin

	process(n)
	begin
		
		case n is
	
			when "0000" =>
				dig <= "01000000";
				
			when "0001" =>
				dig <= "01111001";
				
			when "0010" =>
				dig <= "00100100";
				
			when "0011" =>
				dig <= "00110000";
				
			when "0100" =>
				dig <= "00011001";
				
			when "0101" =>
				dig <= "00010010";
				
			when "0110" =>
				dig <= "00000010";
				
			when "0111" =>
				dig <= "01111000";
				
			when "1000" =>
				dig <= "00000000";
				
			when "1001" =>
				dig <= "00010000";
				
			when others =>
				dig <= "00000000";
				
		end case;
	
	end process;
end behavioral;