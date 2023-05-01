//
//  Nios2  Multiprocessor Tutorial Reference Design top-level verilog module for 3C120 design variant.
//  
//

module top_level
(
  input           clkin_50,
  input           clkin_125,

  input           cpu_reset_n,

  output  [24:0]  fsa,
  inout   [15:0]  fsd
     
);

//
// Declare a localparam for the number of reset sources that exist in this design.
// This parameter will be used by the global_reset_generator module.
//
localparam RESET_SOURCES_COUNT = 1;

//
// define the wires required for the top level stitching
//

reg [(RESET_SOURCES_COUNT - 1):0]   resetn_sources;

wire            global_resetn;


altddio_out     altddio_out_component (
                    .outclock ( tx_clk_to_the_tse_mac ),
                    .dataout ( enet_gtx_clk ),
                    .aclr (!global_resetn),
                    .datain_h (1'b1),
                    .datain_l (1'b0),
                    .outclocken (1'b1),
                    .aset (1'b0),
                    .sclr (1'b0),
                    .sset (1'b0),
                    .oe_out (),
                    .oe (1'b1)
                    );
defparam
                altddio_out_component.extend_oe_disable = "UNUSED",
                altddio_out_component.intended_device_family = "Cyclone III",
                altddio_out_component.invert_output = "OFF",
                altddio_out_component.lpm_type = "altddio_out",
                altddio_out_component.oe_reg = "UNUSED",
                altddio_out_component.width = 1;



//
// Tie the reset sources from the system into the global_reset_generator module.
// The reset counter width of 16 should provide a 2^16 clock assertion of global reset
// which at 50MHz should be 1.31ms long.
//
  always @ (*) begin
        resetn_sources[(RESET_SOURCES_COUNT - 1)]   <=  cpu_reset_n;
  end

global_reset_generator 
#(
  .RESET_SOURCES_WIDTH  (RESET_SOURCES_COUNT),
  .RESET_COUNTER_WIDTH  (16)
) global_reset_generator_inst
(
  .clk            (clkin_50),
  .resetn_sources (resetn_sources),
  .global_resetn  (global_resetn),
);

//
// The SOPC system instantiation.
//
multiprocessor_tutorial_main_system  multiprocessor_tutorial_main_system_inst
(
  // 1) global signals:
  .clk_in_clk                                                 (clkin_50),

  .clk_clk_in_reset_reset_n                                   (global_resetn)

);

endmodule
