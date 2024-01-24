library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity dec is 
	port(
		n : in std_logic_vector(3 downto 0);
		dig : out std_logic_vector(7 downto 0));
end entity;

architecture behavioral of dec is
begin

	process(n)
	begin
	
		case n is
			when "0000" =>
				dig <= "11000000";
				
			when "0001" =>
				dig <= "11111001"; 
				
			when "0010" =>
				dig <= "10100100"; 
				
			when "0011" =>
				dig <= "10110000"; 
				
			when "0100" =>
				dig <= "10011001";
				
			when "0101" =>
				dig <= "10010010";
				
			when "0110" =>
				dig <= "10000010";
				
			when "0111" =>
				dig <= "11111000";
					
			when "1000" =>
				dig <= "10000000";
			
			when "1001" =>
				dig <= "10010000";			
			when others =>
				dig <= "00000000"; -- null
				
		end case; 
	
	end process;
end behavioral;