library ieee;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;

entity Lab3 is 
        port (
            clk: in  std_logic;        
				reset: in  std_logic;
				bp: in std_logic;
				scl: inout std_logic;
				sda: inout std_logic;
				--gsensor_address: out std_logic:='1';
				dig0: out std_logic_vector(7 downto 0);
				dig1: out std_logic_vector(7 downto 0);
				dig2: out std_logic_vector(7 downto 0)
            );
end entity;

architecture arch of Lab3 is
   
    component lab3_qsys is
        port (
            clk_clk        : in    std_logic                    := 'X'; -- clk
            dig0_export    : out   std_logic_vector(3 downto 0);        -- export
            dig1_export    : out   std_logic_vector(3 downto 0);        -- export
            dig2_export    : out   std_logic_vector(3 downto 0);        -- export
            i2c_scl_pad_io : inout std_logic                    := 'X'; -- scl_pad_io
            i2c_sda_pad_io : inout std_logic                    := 'X'; -- sda_pad_io
            reset_reset_n  : in    std_logic                    := 'X'; -- reset_n
            bp_export      : in    std_logic                    := 'X'  -- export
        );
	end component lab3_qsys;

	 
	component dec is
		port (
			n : in std_logic_vector(3 downto 0);
			dig : out std_logic_vector(7 downto 0)
			);
	end component dec;
	
	component sec_dec is
		port (
			n : in std_logic_vector(3 downto 0);
			dig : out std_logic_vector(7 downto 0)
			);
	end component sec_dec;
	
	component thi_dec is
		port (
			n : in std_logic_vector(3 downto 0);
			dig : out std_logic_vector(7 downto 0)
			);
	end component thi_dec;
	
	signal pio_dec0 : std_logic_vector(3 downto 0);
	signal pio_dec1: std_logic_vector(3 downto 0);
	signal pio_dec2 : std_logic_vector(3 downto 0);
	 
begin

   u0 : component lab3_qsys
        port map (
            clk_clk        => clk,        --   clk.clk
            dig0_export    => pio_dec0,    --  dig0.export
            dig1_export    => pio_dec1,    --  dig1.export
            dig2_export    => pio_dec2,    --  dig2.export
            reset_reset_n  => reset,  -- reset.reset_n
            i2c_scl_pad_io => scl, --   i2c.scl_pad_io
            i2c_sda_pad_io => sda,  --      .sda_pad_io
				bp_export      => bp
        );
		
	u1 : component dec
        port map (
            n => pio_dec0,
				dig => dig0
        );
	
	u2 : component sec_dec
        port map (
            n => pio_dec1,
				dig => dig1
        );
		  
	u3 : component thi_dec
        port map (
            n => pio_dec2,
				dig => dig2
        );

end architecture; 