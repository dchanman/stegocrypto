library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity bit_flipper is
port (
    clk: in std_logic;
    reset_n: in std_logic;
    
    slave_addr: in std_logic_vector(1 downto 0);
    slave_rd_en: in std_logic;
    slave_wr_en: in std_logic;
    slave_readdata: out std_logic_vector(31 downto 0);
    slave_writedata: in std_logic_vector(31 downto 0);

    
    master_addr: out std_logic_vector(31 downto 0);
    master_rd_en: out std_logic;
    master_wr_en: out std_logic;
    master_waitrequest: in std_logic;
    master_readdata: in std_logic_vector(31 downto 0);
    master_writedata: out std_logic_vector(31 downto 0)

);
end bit_flipper;

architecture rtl of bit_flipper is
    signal saved_value: std_logic_vector(31 downto 0);
begin
    


--process()
--  if rising_edge(clk)
--S_WRITE:
--  if (!master_waitrequest)
--     state <= S_INCR_ADDR;
--
--S_INCR_ADDR:
--  myaddr <= myaddr + 1;
--
--S_READ:
--  if (!master_waitrequest)
--     mydata <= master_readdata;
--     state <=
--
--S_MODIFY:
--end
--end process;
--
--
--process()
--
--master_wr_en = '0';
--master_rd_en = '0';
--
--case(state):
--S_WRITE:
--  master_wr_en <= '1';
--  master_addr <= myaddr;
--  master_writedata <= mydata;
--
--
--S_INCR_ADDR:
--  
--
--S_READ:
--master_rd_en = '1'
--addr = ...
--
--S_MODIFY:
--end process




end rtl;

