# TCL File Generated by Component Editor 18.1
# Fri Apr 21 15:45:59 CEST 2023
# DO NOT MODIFY


# 
# DumbAccelerator "DumbAccelerator" v1.0
#  2023.04.21.15:45:59
# 
# 

# 
# request TCL package from ACDS 16.1
# 
package require -exact qsys 16.1


# 
# module DumbAccelerator
# 
set_module_property DESCRIPTION ""
set_module_property NAME DumbAccelerator
set_module_property VERSION 1.0
set_module_property INTERNAL false
set_module_property OPAQUE_ADDRESS_MAP true
set_module_property AUTHOR ""
set_module_property DISPLAY_NAME DumbAccelerator
set_module_property INSTANTIATE_IN_SYSTEM_MODULE true
set_module_property EDITABLE true
set_module_property REPORT_TO_TALKBACK false
set_module_property ALLOW_GREYBOX_GENERATION false
set_module_property REPORT_HIERARCHY false


# 
# file sets
# 
add_fileset QUARTUS_SYNTH QUARTUS_SYNTH "" ""
set_fileset_property QUARTUS_SYNTH TOP_LEVEL HwAccelerator
set_fileset_property QUARTUS_SYNTH ENABLE_RELATIVE_INCLUDE_PATHS false
set_fileset_property QUARTUS_SYNTH ENABLE_FILE_OVERWRITE_MODE false
add_fileset_file DumbAccelerator.vhd VHDL PATH ../../CustomIPs/DumbAccelerator.vhd TOP_LEVEL_FILE


# 
# parameters
# 


# 
# display items
# 


# 
# connection point clock
# 
add_interface clock clock end
set_interface_property clock clockRate 0
set_interface_property clock ENABLED true
set_interface_property clock EXPORT_OF ""
set_interface_property clock PORT_NAME_MAP ""
set_interface_property clock CMSIS_SVD_VARIABLES ""
set_interface_property clock SVD_ADDRESS_GROUP ""

add_interface_port clock Clk clk Input 1


# 
# connection point reset
# 
add_interface reset reset end
set_interface_property reset associatedClock clock
set_interface_property reset synchronousEdges DEASSERT
set_interface_property reset ENABLED true
set_interface_property reset EXPORT_OF ""
set_interface_property reset PORT_NAME_MAP ""
set_interface_property reset CMSIS_SVD_VARIABLES ""
set_interface_property reset SVD_ADDRESS_GROUP ""

add_interface_port reset Reset_n reset_n Input 1


# 
# connection point avalon_slave_0
# 
add_interface avalon_slave_0 avalon end
set_interface_property avalon_slave_0 addressUnits WORDS
set_interface_property avalon_slave_0 associatedClock clock
set_interface_property avalon_slave_0 associatedReset reset
set_interface_property avalon_slave_0 bitsPerSymbol 8
set_interface_property avalon_slave_0 burstOnBurstBoundariesOnly false
set_interface_property avalon_slave_0 burstcountUnits WORDS
set_interface_property avalon_slave_0 explicitAddressSpan 0
set_interface_property avalon_slave_0 holdTime 0
set_interface_property avalon_slave_0 linewrapBursts false
set_interface_property avalon_slave_0 maximumPendingReadTransactions 0
set_interface_property avalon_slave_0 maximumPendingWriteTransactions 0
set_interface_property avalon_slave_0 readLatency 0
set_interface_property avalon_slave_0 readWaitTime 1
set_interface_property avalon_slave_0 setupTime 0
set_interface_property avalon_slave_0 timingUnits Cycles
set_interface_property avalon_slave_0 writeWaitTime 0
set_interface_property avalon_slave_0 ENABLED true
set_interface_property avalon_slave_0 EXPORT_OF ""
set_interface_property avalon_slave_0 PORT_NAME_MAP ""
set_interface_property avalon_slave_0 CMSIS_SVD_VARIABLES ""
set_interface_property avalon_slave_0 SVD_ADDRESS_GROUP ""

add_interface_port avalon_slave_0 AS_Addr address Input 2
add_interface_port avalon_slave_0 AS_Wr write Input 1
add_interface_port avalon_slave_0 AS_WData writedata Input 32
add_interface_port avalon_slave_0 As_Rd read Input 1
add_interface_port avalon_slave_0 AS_RData readdata Output 32
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isFlash 0
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isMemoryDevice 0
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isNonVolatileStorage 0
set_interface_assignment avalon_slave_0 embeddedsw.configuration.isPrintableDevice 0


# 
# connection point Debug_state
# 
add_interface Debug_state conduit end
set_interface_property Debug_state associatedClock clock
set_interface_property Debug_state associatedReset ""
set_interface_property Debug_state ENABLED true
set_interface_property Debug_state EXPORT_OF ""
set_interface_property Debug_state PORT_NAME_MAP ""
set_interface_property Debug_state CMSIS_SVD_VARIABLES ""
set_interface_property Debug_state SVD_ADDRESS_GROUP ""

add_interface_port Debug_state DEBUGSTATE readdata Output 8


# 
# connection point Debug_counter
# 
add_interface Debug_counter conduit end
set_interface_property Debug_counter associatedClock clock
set_interface_property Debug_counter associatedReset ""
set_interface_property Debug_counter ENABLED true
set_interface_property Debug_counter EXPORT_OF ""
set_interface_property Debug_counter PORT_NAME_MAP ""
set_interface_property Debug_counter CMSIS_SVD_VARIABLES ""
set_interface_property Debug_counter SVD_ADDRESS_GROUP ""

add_interface_port Debug_counter DEBUGCOUNT readdata Output 10


# 
# connection point avalon_master
# 
add_interface avalon_master avalon start
set_interface_property avalon_master addressUnits SYMBOLS
set_interface_property avalon_master associatedClock clock
set_interface_property avalon_master associatedReset reset
set_interface_property avalon_master bitsPerSymbol 8
set_interface_property avalon_master burstOnBurstBoundariesOnly false
set_interface_property avalon_master burstcountUnits WORDS
set_interface_property avalon_master doStreamReads false
set_interface_property avalon_master doStreamWrites false
set_interface_property avalon_master holdTime 0
set_interface_property avalon_master linewrapBursts false
set_interface_property avalon_master maximumPendingReadTransactions 0
set_interface_property avalon_master maximumPendingWriteTransactions 0
set_interface_property avalon_master readLatency 0
set_interface_property avalon_master readWaitTime 1
set_interface_property avalon_master setupTime 0
set_interface_property avalon_master timingUnits Cycles
set_interface_property avalon_master writeWaitTime 0
set_interface_property avalon_master ENABLED true
set_interface_property avalon_master EXPORT_OF ""
set_interface_property avalon_master PORT_NAME_MAP ""
set_interface_property avalon_master CMSIS_SVD_VARIABLES ""
set_interface_property avalon_master SVD_ADDRESS_GROUP ""

add_interface_port avalon_master AM_Addr address Output 32
add_interface_port avalon_master AM_BE byteenable Output 4
add_interface_port avalon_master AM_RData readdata Input 32
add_interface_port avalon_master AM_RDataValid readdatavalid Input 1
add_interface_port avalon_master AM_Rd read Output 1
add_interface_port avalon_master AM_WData writedata Output 32
add_interface_port avalon_master AM_WaitRq waitrequest Input 1
add_interface_port avalon_master AM_Wr write Output 1

