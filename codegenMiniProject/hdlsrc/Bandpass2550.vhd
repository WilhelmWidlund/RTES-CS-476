-- -------------------------------------------------------------
--
-- Module: Bandpass2550
-- Generated by MATLAB(R) 9.14 and Filter Design HDL Coder 3.1.13.
-- Generated on: 2023-05-15 10:11:32
-- -------------------------------------------------------------

-- -------------------------------------------------------------
-- HDL Code Generation Options:
--
-- TargetLanguage: VHDL
-- Name: Bandpass2550
-- InputDataType: numerictype(1,32,0)
-- GenerateHDLTestBench: off

-- -------------------------------------------------------------
-- HDL Implementation    : Fully parallel
-- Folding Factor        : 1
-- -------------------------------------------------------------
-- Filter Settings:
--
-- Discrete-Time FIR Filter (real)
-- -------------------------------
-- Filter Structure  : Direct-Form Symmetric FIR
-- Filter Length     : 31
-- Stable            : Yes
-- Linear Phase      : Yes (Type 1)
-- -------------------------------------------------------------
-- Multipliers           : 16
--
-- HDL latency is 2 samples


LIBRARY IEEE;
USE IEEE.std_logic_1164.all;
USE IEEE.numeric_std.ALL;

ENTITY Bandpass2550 IS
   PORT( clk                             :   IN    std_logic; 
         clk_enable                      :   IN    std_logic; 
         reset                           :   IN    std_logic; 
         filter_in                       :   IN    real; -- double
         filter_out                      :   OUT   real  -- double
         );

END Bandpass2550;


----------------------------------------------------------------
--Module Architecture: Bandpass2550
----------------------------------------------------------------
ARCHITECTURE rtl OF Bandpass2550 IS
  -- Local Functions
  -- Type Definitions
  TYPE delay_pipeline_type IS ARRAY (NATURAL range <>) OF real; -- double
  -- Constants
  CONSTANT coeff1                         : real := -4.9573627107127782E-04; -- double
  CONSTANT coeff2                         : real := 2.0413077191851893E-03; -- double
  CONSTANT coeff3                         : real := 4.9911972583612362E-03; -- double
  CONSTANT coeff4                         : real := -4.8926230700263313E-18; -- double
  CONSTANT coeff5                         : real := -1.1435962459741161E-02; -- double
  CONSTANT coeff6                         : real := -9.8379439690512334E-03; -- double
  CONSTANT coeff7                         : real := 4.1089532798647731E-03; -- double
  CONSTANT coeff8                         : real := -4.7795307556093148E-18; -- double
  CONSTANT coeff9                         : real := -7.8089496336161968E-03; -- double
  CONSTANT coeff10                        : real := 3.6080279603841588E-02; -- double
  CONSTANT coeff11                        : real := 8.3430348800809781E-02; -- double
  CONSTANT coeff12                        : real := -2.4712018402001206E-17; -- double
  CONSTANT coeff13                        : real := -1.6472036918748967E-01; -- double
  CONSTANT coeff14                        : real := -1.5236609508070151E-01; -- double
  CONSTANT coeff15                        : real := 9.2016202253481505E-02; -- double
  CONSTANT coeff16                        : real := 2.4924848789999510E-01; -- double

  -- Signals
  SIGNAL delay_pipeline                   : delay_pipeline_type(0 TO 30) := (0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0, 0.0); -- double
  SIGNAL tapsum1                          : real := 0.0; -- double
  SIGNAL tapsum_mcand                     : real := 0.0; -- double
  SIGNAL tapsum2                          : real := 0.0; -- double
  SIGNAL tapsum_mcand_1                   : real := 0.0; -- double
  SIGNAL tapsum3                          : real := 0.0; -- double
  SIGNAL tapsum_mcand_2                   : real := 0.0; -- double
  SIGNAL tapsum4                          : real := 0.0; -- double
  SIGNAL tapsum_mcand_3                   : real := 0.0; -- double
  SIGNAL tapsum5                          : real := 0.0; -- double
  SIGNAL tapsum_mcand_4                   : real := 0.0; -- double
  SIGNAL tapsum6                          : real := 0.0; -- double
  SIGNAL tapsum_mcand_5                   : real := 0.0; -- double
  SIGNAL tapsum7                          : real := 0.0; -- double
  SIGNAL tapsum_mcand_6                   : real := 0.0; -- double
  SIGNAL tapsum8                          : real := 0.0; -- double
  SIGNAL tapsum_mcand_7                   : real := 0.0; -- double
  SIGNAL tapsum9                          : real := 0.0; -- double
  SIGNAL tapsum_mcand_8                   : real := 0.0; -- double
  SIGNAL tapsum10                         : real := 0.0; -- double
  SIGNAL tapsum_mcand_9                   : real := 0.0; -- double
  SIGNAL tapsum11                         : real := 0.0; -- double
  SIGNAL tapsum_mcand_10                  : real := 0.0; -- double
  SIGNAL tapsum12                         : real := 0.0; -- double
  SIGNAL tapsum_mcand_11                  : real := 0.0; -- double
  SIGNAL tapsum13                         : real := 0.0; -- double
  SIGNAL tapsum_mcand_12                  : real := 0.0; -- double
  SIGNAL tapsum14                         : real := 0.0; -- double
  SIGNAL tapsum_mcand_13                  : real := 0.0; -- double
  SIGNAL tapsum15                         : real := 0.0; -- double
  SIGNAL tapsum_mcand_14                  : real := 0.0; -- double
  SIGNAL product16                        : real := 0.0; -- double
  SIGNAL product15                        : real := 0.0; -- double
  SIGNAL product14                        : real := 0.0; -- double
  SIGNAL product13                        : real := 0.0; -- double
  SIGNAL product12                        : real := 0.0; -- double
  SIGNAL product11                        : real := 0.0; -- double
  SIGNAL product10                        : real := 0.0; -- double
  SIGNAL product9                         : real := 0.0; -- double
  SIGNAL product8                         : real := 0.0; -- double
  SIGNAL product7                         : real := 0.0; -- double
  SIGNAL product6                         : real := 0.0; -- double
  SIGNAL product5                         : real := 0.0; -- double
  SIGNAL product4                         : real := 0.0; -- double
  SIGNAL product3                         : real := 0.0; -- double
  SIGNAL product2                         : real := 0.0; -- double
  SIGNAL product1_cast                    : real := 0.0; -- double
  SIGNAL product1                         : real := 0.0; -- double
  SIGNAL sum1                             : real := 0.0; -- double
  SIGNAL sum2                             : real := 0.0; -- double
  SIGNAL sum3                             : real := 0.0; -- double
  SIGNAL sum4                             : real := 0.0; -- double
  SIGNAL sum5                             : real := 0.0; -- double
  SIGNAL sum6                             : real := 0.0; -- double
  SIGNAL sum7                             : real := 0.0; -- double
  SIGNAL sum8                             : real := 0.0; -- double
  SIGNAL sum9                             : real := 0.0; -- double
  SIGNAL sum10                            : real := 0.0; -- double
  SIGNAL sum11                            : real := 0.0; -- double
  SIGNAL sum12                            : real := 0.0; -- double
  SIGNAL sum13                            : real := 0.0; -- double
  SIGNAL sum14                            : real := 0.0; -- double
  SIGNAL sum15                            : real := 0.0; -- double
  SIGNAL output_register                  : real := 0.0; -- double


BEGIN

  -- Block Statements
  Delay_Pipeline_process : PROCESS (clk, reset)
  BEGIN
    IF reset = '1' THEN
      delay_pipeline(0 TO 30) <= (OTHERS => 0.0000000000000000E+00);
    ELSIF clk'event AND clk = '1' THEN
      IF clk_enable = '1' THEN
        delay_pipeline(0) <= filter_in;
        delay_pipeline(1 TO 30) <= delay_pipeline(0 TO 29);
      END IF;
    END IF; 
  END PROCESS Delay_Pipeline_process;

  tapsum1 <= delay_pipeline(0) + delay_pipeline(30);

  tapsum_mcand <= tapsum1;


  tapsum2 <= delay_pipeline(1) + delay_pipeline(29);

  tapsum_mcand_1 <= tapsum2;


  tapsum3 <= delay_pipeline(2) + delay_pipeline(28);

  tapsum_mcand_2 <= tapsum3;


  tapsum4 <= delay_pipeline(3) + delay_pipeline(27);

  tapsum_mcand_3 <= tapsum4;


  tapsum5 <= delay_pipeline(4) + delay_pipeline(26);

  tapsum_mcand_4 <= tapsum5;


  tapsum6 <= delay_pipeline(5) + delay_pipeline(25);

  tapsum_mcand_5 <= tapsum6;


  tapsum7 <= delay_pipeline(6) + delay_pipeline(24);

  tapsum_mcand_6 <= tapsum7;


  tapsum8 <= delay_pipeline(7) + delay_pipeline(23);

  tapsum_mcand_7 <= tapsum8;


  tapsum9 <= delay_pipeline(8) + delay_pipeline(22);

  tapsum_mcand_8 <= tapsum9;


  tapsum10 <= delay_pipeline(9) + delay_pipeline(21);

  tapsum_mcand_9 <= tapsum10;


  tapsum11 <= delay_pipeline(10) + delay_pipeline(20);

  tapsum_mcand_10 <= tapsum11;


  tapsum12 <= delay_pipeline(11) + delay_pipeline(19);

  tapsum_mcand_11 <= tapsum12;


  tapsum13 <= delay_pipeline(12) + delay_pipeline(18);

  tapsum_mcand_12 <= tapsum13;


  tapsum14 <= delay_pipeline(13) + delay_pipeline(17);

  tapsum_mcand_13 <= tapsum14;


  tapsum15 <= delay_pipeline(14) + delay_pipeline(16);

  tapsum_mcand_14 <= tapsum15;


  product16 <= delay_pipeline(15) * coeff16;

  product15 <= tapsum_mcand_14 * coeff15;

  product14 <= tapsum_mcand_13 * coeff14;

  product13 <= tapsum_mcand_12 * coeff13;

  product12 <= tapsum_mcand_11 * coeff12;

  product11 <= tapsum_mcand_10 * coeff11;

  product10 <= tapsum_mcand_9 * coeff10;

  product9 <= tapsum_mcand_8 * coeff9;

  product8 <= tapsum_mcand_7 * coeff8;

  product7 <= tapsum_mcand_6 * coeff7;

  product6 <= tapsum_mcand_5 * coeff6;

  product5 <= tapsum_mcand_4 * coeff5;

  product4 <= tapsum_mcand_3 * coeff4;

  product3 <= tapsum_mcand_2 * coeff3;

  product2 <= tapsum_mcand_1 * coeff2;

  product1_cast <= product1;


  product1 <= tapsum_mcand * coeff1;

  sum1 <= product1_cast + product2;

  sum2 <= sum1 + product3;

  sum3 <= sum2 + product4;

  sum4 <= sum3 + product5;

  sum5 <= sum4 + product6;

  sum6 <= sum5 + product7;

  sum7 <= sum6 + product8;

  sum8 <= sum7 + product9;

  sum9 <= sum8 + product10;

  sum10 <= sum9 + product11;

  sum11 <= sum10 + product12;

  sum12 <= sum11 + product13;

  sum13 <= sum12 + product14;

  sum14 <= sum13 + product15;

  sum15 <= sum14 + product16;

  Output_Register_process : PROCESS (clk, reset)
  BEGIN
    IF reset = '1' THEN
      output_register <= 0.0000000000000000E+00;
    ELSIF clk'event AND clk = '1' THEN
      IF clk_enable = '1' THEN
        output_register <= sum15;
      END IF;
    END IF; 
  END PROCESS Output_Register_process;

  -- Assignment Statements
  filter_out <= output_register;
END rtl;