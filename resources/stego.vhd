LIBRARY ieee; 
USE ieee.Std_Logic_1164.all;
use ieee.Std_Logic_arith.all; 
use ieee.Std_Logic_signed.all;  

entity StegoEmbed is
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
end StegoEmbed;

architecture bhvr of StegoEmbed is
    
    --SIGNALS FOR STORING ACTUAL DATA
    signal saved_value, 
           image_data, 
           embed_data, 
           datalength, 
           embed_byte, 
       --    embed_bit,
           offset : std_logic_vector(31 downto 0);
    

    --SIGNALS FOR DATA ADDRESSES
    signal image_data_addr, 
           embed_data_addr, 
           datalength_addr,
           offset_addr : std_logic_vector(31 downto 0);

    
    --Done signals to indicate that we are done in the loop
    signal datalength_done, 
           RGB_done, 
           image_done : std_logic;
    

    --Counters for Different For Loop Operations
    signal image_counter : std_logic_vector(31 downto 0);
    signal RGB_counter : std_logic_vector(2 downto 0);
    signal datalength_counter : std_logic_vector(4 downto 0);


    --Signals for currenta and next states
    signal  CurrentState, 
            NextState           : Std_Logic_Vector(7 downto 0);
     
    
   
    --Contstants to describe the different states
    constant S_READ_LENGTH_ADDR                            : Std_Logic_Vector(7 downto 0) := X"00";        -- state for modifying image data
    constant S_INCR_IMAGE_ADDR                             : Std_Logic_Vector(7 downto 0) := X"01";        -- State is to Increment the address space
    constant S_INCR_EMBED_ADDR                             : Std_Logic_Vector(7 downto 0) := X"0C";        -- State is to Increment the address space
    constant S_READ_IMAGE                                  : Std_Logic_Vector(7 downto 0) := X"02";        -- State for reading image data
    constant S_READ_IMAGE_ADDR                             : Std_Logic_Vector(7 downto 0) := X"03";        -- State for reading embedded data
    constant S_READ_EMBED                                  : Std_Logic_Vector(7 downto 0) := X"04";        -- State for reading datalength
    constant S_READ_EMBED_ADDR                             : Std_Logic_Vector(7 downto 0) := X"05";        -- State for reading datalength
    constant S_READ_LENGTH                                 : Std_Logic_Vector(7 downto 0) := X"06";        -- State for reading image offset
    constant S_IDLE                                        : Std_Logic_Vector(7 downto 0) := X"07";        -- State for waiting
    
-- These are placeholder states for modifying image data 
    constant S_MODIFY_IMAGE                                : Std_Logic_Vector(7 downto 0) := X"08";        -- State for modifying the image data.
    constant S_MODIFY2                                     : Std_Logic_Vector(7 downto 0) := X"09";        -- State for modifying the image data.
    constant S_MODIFY3                                     : Std_Logic_Vector(7 downto 0) := X"0A";        -- State for modifying the image data.
   
--not needed
    constant S_READ_OFFSET                                 : Std_Logic_Vector(7 downto 0) := X"0B";        -- State for modifying the image data.


begin


--========== State Machine ============

process(clk, reset_n)

    variable  embed_bit : std_logic_vector(31 downto 0);
begin
  
  if(reset_n = '0') then
    
    NextState <= S_IDLE;
  
  elsif rising_edge(clk) then
           

           CurrentState <= NextState;

           case CurrentState is

            when S_READ_LENGTH_ADDR =>

              if(master_waitrequest = '0') then
                datalength_addr <= master_readdata;

                NextState <= S_READ_LENGTH;
              end if;


            when S_INCR_IMAGE_ADDR =>

              image_data_addr <= image_data_addr + 1;


              NextState <= S_READ_IMAGE;  

            when S_INCR_EMBED_ADDR =>

              embed_data_addr <= embed_data_addr + 1;

              NextState <= S_READ_EMBED;


            --State used for reading the image data at the current address space
            --Next states should be S_INCR_ADDR if counter not done or S_READ_EMBED
            when S_READ_IMAGE =>

              if(master_waitrequest = '0') then
                image_data <= master_readdata;

              --Change Next State Logic

                if(image_done = '1') then 
                  NextState <= S_READ_EMBED_ADDR;
                else
                  NextState <= S_INCR_IMAGE_ADDR;
                end if;
              end if;

            --State for reading the starting address where the image data is stored
            --Increment address until offset value is 0
            when S_READ_IMAGE_ADDR =>
              
              if(master_waitrequest = '0') then   
                
                image_data_addr <= master_readdata;

                NextState <= S_READ_IMAGE;
              end if;

            --State for reading the data we want to embed
            when S_READ_EMBED =>

              if(master_waitrequest = '0') then
                
                embed_data <= master_readdata;

                if(RGB_done = '1') then 
                  NextState <= S_READ_LENGTH;
                else
                  NextState <= S_INCR_EMBED_ADDR;
                end if;
              end if;

            --State for reading the starting address for data we want to embed
            when S_READ_EMBED_ADDR =>

              if(master_waitrequest = '0') then
                embed_data_addr <= master_readdata;

                NextState <= S_READ_EMBED;
              end if;

            --State for reading the data length
            when S_READ_LENGTH =>

              if(master_waitrequest = '0') then
                datalength <= master_readdata;

               -- NextState <= ; rework order of reading these.

              end if;

            --State for reading the offset value to decide when to start reading image data.
            --not needed
            when S_READ_OFFSET =>

            --State for just staying still and reseting everything
            when S_IDLE =>

              RGB_done <= '0';
              RGB_counter <= RGB_counter xor RGB_counter;
              image_done <= '0';
              image_counter <= image_counter xor image_counter;
              
              datalength_done <= '0';
              datalength_counter <= datalength_counter xor datalength_counter;


            --States used for actually modifying the data (All may or may not be needed)
            when S_MODIFY_IMAGE =>

                embed_byte <= datalength;

                NextState <= S_MODIFY2;




            when S_MODIFY2 =>

                embed_bit  := x"0001" and embed_byte; 

                if(embed_bit = 1 ) then
                  image_data <= image_data or x"0001";
                else
                  image_data <= image_data and x"00FE";
                end if;

                embed_byte <= '0' & embed_byte(31 downto 1);

                NextState <= S_READ_IMAGE;

            when S_MODIFY3 =>


           when others =>
        end case;       
    end if; 
end process;


--========== Combinational Master Logic ===============

process(slave_addr, master_readdata, slave_rd_en, slave_wr_en, slave_writedata, master_waitrequest, CurrentState, image_data, embed_data, datalength, image_data_addr, embed_data_addr, datalength_addr)

begin
        master_wr_en <= '0';
        master_rd_en <= '0';

    case CurrentState is
        
      
        when S_READ_LENGTH_ADDR =>

        --State used for moving to the next address in the sRAM
        when S_INCR_IMAGE_ADDR =>

        --State used for reading the image data at the current address space
        --Next states should be S_INCR_ADDR if counter not done or S_READ_EMBED
        when S_READ_IMAGE =>

        --State for reading the starting address where the image data is stored
        --Increment address until offset value is 0
        when S_READ_IMAGE_ADDR =>
          
        --State for reading the data we want to embed
        when S_READ_EMBED =>

        --State for reading the starting address for data we want to embed
        when S_READ_EMBED_ADDR =>

        --State for reading the data length
        when S_READ_LENGTH =>

        --State for reading the offset value to decide when to start reading image data.
        when S_READ_OFFSET =>

        --State for just staying still and reseting everything
        when S_IDLE =>


        --States used for actually modifying the data (All may or may not be needed)
        when S_MODIFY_IMAGE =>

        when S_MODIFY2 =>

        when S_MODIFY3 =>


       when others =>

    end case;
end process;


end bhvr;

