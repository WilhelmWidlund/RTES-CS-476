/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'SysAudio_NIOS_II' in SOPC Builder design 'soc_system'
 * SOPC Builder design path: C:/RTES/miniproject/hw/quartus/soc_system.sopcinfo
 *
 * Generated: Mon May 15 09:31:43 CEST 2023
 */

/*
 * DO NOT MODIFY THIS FILE
 *
 * Changing this file will have subtle consequences
 * which will almost certainly lead to a nonfunctioning
 * system. If you do modify this file, be aware that your
 * changes will be overwritten and lost when this file
 * is generated again.
 *
 * DO NOT MODIFY THIS FILE
 */

/*
 * License Agreement
 *
 * Copyright (c) 2008
 * Altera Corporation, San Jose, California, USA.
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * This agreement shall be governed in all respects by the laws of the State
 * of California and by the laws of the United States of America.
 */

#ifndef __SYSTEM_H_
#define __SYSTEM_H_

/* Include definitions from linker script generator */
#include "linker.h"


/*
 * CPU configuration
 *
 */

#define ALT_CPU_ARCHITECTURE "altera_nios2_gen2"
#define ALT_CPU_BIG_ENDIAN 0
#define ALT_CPU_BREAK_ADDR 0x04080820
#define ALT_CPU_CPU_ARCH_NIOS2_R1
#define ALT_CPU_CPU_FREQ 50000000u
#define ALT_CPU_CPU_ID_SIZE 1
#define ALT_CPU_CPU_ID_VALUE 0x00000000
#define ALT_CPU_CPU_IMPLEMENTATION "fast"
#define ALT_CPU_DATA_ADDR_WIDTH 0x1b
#define ALT_CPU_DCACHE_BYPASS_MASK 0x80000000
#define ALT_CPU_DCACHE_LINE_SIZE 32
#define ALT_CPU_DCACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_DCACHE_SIZE 2048
#define ALT_CPU_EXCEPTION_ADDR 0x04040020
#define ALT_CPU_FLASH_ACCELERATOR_LINES 0
#define ALT_CPU_FLASH_ACCELERATOR_LINE_SIZE 0
#define ALT_CPU_FLUSHDA_SUPPORTED
#define ALT_CPU_FREQ 50000000
#define ALT_CPU_HARDWARE_DIVIDE_PRESENT 0
#define ALT_CPU_HARDWARE_MULTIPLY_PRESENT 1
#define ALT_CPU_HARDWARE_MULX_PRESENT 0
#define ALT_CPU_HAS_DEBUG_CORE 1
#define ALT_CPU_HAS_DEBUG_STUB
#define ALT_CPU_HAS_EXTRA_EXCEPTION_INFO
#define ALT_CPU_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define ALT_CPU_HAS_JMPI_INSTRUCTION
#define ALT_CPU_ICACHE_LINE_SIZE 32
#define ALT_CPU_ICACHE_LINE_SIZE_LOG2 5
#define ALT_CPU_ICACHE_SIZE 4096
#define ALT_CPU_INITDA_SUPPORTED
#define ALT_CPU_INST_ADDR_WIDTH 0x1b
#define ALT_CPU_NAME "SysAudio_NIOS_II"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 0
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x04040000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x04080820
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 50000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000000
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x1b
#define NIOS2_DCACHE_BYPASS_MASK 0x80000000
#define NIOS2_DCACHE_LINE_SIZE 32
#define NIOS2_DCACHE_LINE_SIZE_LOG2 5
#define NIOS2_DCACHE_SIZE 2048
#define NIOS2_EXCEPTION_ADDR 0x04040020
#define NIOS2_FLASH_ACCELERATOR_LINES 0
#define NIOS2_FLASH_ACCELERATOR_LINE_SIZE 0
#define NIOS2_FLUSHDA_SUPPORTED
#define NIOS2_HARDWARE_DIVIDE_PRESENT 0
#define NIOS2_HARDWARE_MULTIPLY_PRESENT 1
#define NIOS2_HARDWARE_MULX_PRESENT 0
#define NIOS2_HAS_DEBUG_CORE 1
#define NIOS2_HAS_DEBUG_STUB
#define NIOS2_HAS_EXTRA_EXCEPTION_INFO
#define NIOS2_HAS_ILLEGAL_INSTRUCTION_EXCEPTION
#define NIOS2_HAS_JMPI_INSTRUCTION
#define NIOS2_ICACHE_LINE_SIZE 32
#define NIOS2_ICACHE_LINE_SIZE_LOG2 5
#define NIOS2_ICACHE_SIZE 4096
#define NIOS2_INITDA_SUPPORTED
#define NIOS2_INST_ADDR_WIDTH 0x1b
#define NIOS2_NUM_OF_SHADOW_REG_SETS 0
#define NIOS2_OCI_VERSION 1
#define NIOS2_RESET_ADDR 0x04040000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_MAILBOX_SIMPLE
#define __ALTERA_AVALON_NEW_SDRAM_CONTROLLER
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PERFORMANCE_COUNTER
#define __ALTERA_AVALON_PIO
#define __ALTERA_NIOS2_GEN2
#define __ALTERA_UP_AVALON_AUDIO
#define __ALTERA_UP_AVALON_AUDIO_AND_VIDEO_CONFIG


/*
 * Mailbox_AudioToSigProc configuration
 *
 */

#define ALT_MODULE_CLASS_Mailbox_AudioToSigProc altera_avalon_mailbox_simple
#define MAILBOX_AUDIOTOSIGPROC_BASE 0x40810f0
#define MAILBOX_AUDIOTOSIGPROC_IRQ 2
#define MAILBOX_AUDIOTOSIGPROC_IRQ_INTERRUPT_CONTROLLER_ID 0
#define MAILBOX_AUDIOTOSIGPROC_NAME "/dev/Mailbox_AudioToSigProc"
#define MAILBOX_AUDIOTOSIGPROC_SPAN 16
#define MAILBOX_AUDIOTOSIGPROC_TYPE "altera_avalon_mailbox_simple"


/*
 * Mailbox_SigProcToAudio configuration
 *
 */

#define ALT_MODULE_CLASS_Mailbox_SigProcToAudio altera_avalon_mailbox_simple
#define MAILBOX_SIGPROCTOAUDIO_BASE 0x4081100
#define MAILBOX_SIGPROCTOAUDIO_IRQ 3
#define MAILBOX_SIGPROCTOAUDIO_IRQ_INTERRUPT_CONTROLLER_ID 0
#define MAILBOX_SIGPROCTOAUDIO_NAME "/dev/Mailbox_SigProcToAudio"
#define MAILBOX_SIGPROCTOAUDIO_SPAN 16
#define MAILBOX_SIGPROCTOAUDIO_TYPE "altera_avalon_mailbox_simple"


/*
 * PIO_1st_7seg configuration
 *
 */

#define ALT_MODULE_CLASS_PIO_1st_7seg altera_avalon_pio
#define PIO_1ST_7SEG_BASE 0x40810c0
#define PIO_1ST_7SEG_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_1ST_7SEG_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_1ST_7SEG_CAPTURE 0
#define PIO_1ST_7SEG_DATA_WIDTH 14
#define PIO_1ST_7SEG_DO_TEST_BENCH_WIRING 0
#define PIO_1ST_7SEG_DRIVEN_SIM_VALUE 0
#define PIO_1ST_7SEG_EDGE_TYPE "NONE"
#define PIO_1ST_7SEG_FREQ 50000000
#define PIO_1ST_7SEG_HAS_IN 0
#define PIO_1ST_7SEG_HAS_OUT 1
#define PIO_1ST_7SEG_HAS_TRI 0
#define PIO_1ST_7SEG_IRQ -1
#define PIO_1ST_7SEG_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIO_1ST_7SEG_IRQ_TYPE "NONE"
#define PIO_1ST_7SEG_NAME "/dev/PIO_1st_7seg"
#define PIO_1ST_7SEG_RESET_VALUE 0
#define PIO_1ST_7SEG_SPAN 16
#define PIO_1ST_7SEG_TYPE "altera_avalon_pio"


/*
 * PIO_2nd_7seg configuration
 *
 */

#define ALT_MODULE_CLASS_PIO_2nd_7seg altera_avalon_pio
#define PIO_2ND_7SEG_BASE 0x40810b0
#define PIO_2ND_7SEG_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_2ND_7SEG_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_2ND_7SEG_CAPTURE 0
#define PIO_2ND_7SEG_DATA_WIDTH 14
#define PIO_2ND_7SEG_DO_TEST_BENCH_WIRING 0
#define PIO_2ND_7SEG_DRIVEN_SIM_VALUE 0
#define PIO_2ND_7SEG_EDGE_TYPE "NONE"
#define PIO_2ND_7SEG_FREQ 50000000
#define PIO_2ND_7SEG_HAS_IN 0
#define PIO_2ND_7SEG_HAS_OUT 1
#define PIO_2ND_7SEG_HAS_TRI 0
#define PIO_2ND_7SEG_IRQ -1
#define PIO_2ND_7SEG_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIO_2ND_7SEG_IRQ_TYPE "NONE"
#define PIO_2ND_7SEG_NAME "/dev/PIO_2nd_7seg"
#define PIO_2ND_7SEG_RESET_VALUE 0
#define PIO_2ND_7SEG_SPAN 16
#define PIO_2ND_7SEG_TYPE "altera_avalon_pio"


/*
 * PIO_3rd_7seg configuration
 *
 */

#define ALT_MODULE_CLASS_PIO_3rd_7seg altera_avalon_pio
#define PIO_3RD_7SEG_BASE 0x40810a0
#define PIO_3RD_7SEG_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_3RD_7SEG_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_3RD_7SEG_CAPTURE 0
#define PIO_3RD_7SEG_DATA_WIDTH 14
#define PIO_3RD_7SEG_DO_TEST_BENCH_WIRING 0
#define PIO_3RD_7SEG_DRIVEN_SIM_VALUE 0
#define PIO_3RD_7SEG_EDGE_TYPE "NONE"
#define PIO_3RD_7SEG_FREQ 50000000
#define PIO_3RD_7SEG_HAS_IN 0
#define PIO_3RD_7SEG_HAS_OUT 1
#define PIO_3RD_7SEG_HAS_TRI 0
#define PIO_3RD_7SEG_IRQ -1
#define PIO_3RD_7SEG_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIO_3RD_7SEG_IRQ_TYPE "NONE"
#define PIO_3RD_7SEG_NAME "/dev/PIO_3rd_7seg"
#define PIO_3RD_7SEG_RESET_VALUE 0
#define PIO_3RD_7SEG_SPAN 16
#define PIO_3RD_7SEG_TYPE "altera_avalon_pio"


/*
 * PIO_Debug configuration
 *
 */

#define ALT_MODULE_CLASS_PIO_Debug altera_avalon_pio
#define PIO_DEBUG_BASE 0x4081080
#define PIO_DEBUG_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_DEBUG_BIT_MODIFYING_OUTPUT_REGISTER 1
#define PIO_DEBUG_CAPTURE 0
#define PIO_DEBUG_DATA_WIDTH 32
#define PIO_DEBUG_DO_TEST_BENCH_WIRING 0
#define PIO_DEBUG_DRIVEN_SIM_VALUE 0
#define PIO_DEBUG_EDGE_TYPE "NONE"
#define PIO_DEBUG_FREQ 50000000
#define PIO_DEBUG_HAS_IN 0
#define PIO_DEBUG_HAS_OUT 1
#define PIO_DEBUG_HAS_TRI 0
#define PIO_DEBUG_IRQ -1
#define PIO_DEBUG_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIO_DEBUG_IRQ_TYPE "NONE"
#define PIO_DEBUG_NAME "/dev/PIO_Debug"
#define PIO_DEBUG_RESET_VALUE 0
#define PIO_DEBUG_SPAN 32
#define PIO_DEBUG_TYPE "altera_avalon_pio"


/*
 * PIO_LEDs_Shared configuration
 *
 */

#define ALT_MODULE_CLASS_PIO_LEDs_Shared altera_avalon_pio
#define PIO_LEDS_SHARED_BASE 0x40810e0
#define PIO_LEDS_SHARED_BIT_CLEARING_EDGE_REGISTER 0
#define PIO_LEDS_SHARED_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_LEDS_SHARED_CAPTURE 0
#define PIO_LEDS_SHARED_DATA_WIDTH 10
#define PIO_LEDS_SHARED_DO_TEST_BENCH_WIRING 0
#define PIO_LEDS_SHARED_DRIVEN_SIM_VALUE 0
#define PIO_LEDS_SHARED_EDGE_TYPE "NONE"
#define PIO_LEDS_SHARED_FREQ 50000000
#define PIO_LEDS_SHARED_HAS_IN 0
#define PIO_LEDS_SHARED_HAS_OUT 1
#define PIO_LEDS_SHARED_HAS_TRI 0
#define PIO_LEDS_SHARED_IRQ -1
#define PIO_LEDS_SHARED_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PIO_LEDS_SHARED_IRQ_TYPE "NONE"
#define PIO_LEDS_SHARED_NAME "/dev/PIO_LEDs_Shared"
#define PIO_LEDS_SHARED_RESET_VALUE 0
#define PIO_LEDS_SHARED_SPAN 16
#define PIO_LEDS_SHARED_TYPE "altera_avalon_pio"


/*
 * PIO_Switches configuration
 *
 */

#define ALT_MODULE_CLASS_PIO_Switches altera_avalon_pio
#define PIO_SWITCHES_BASE 0x40810d0
#define PIO_SWITCHES_BIT_CLEARING_EDGE_REGISTER 1
#define PIO_SWITCHES_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_SWITCHES_CAPTURE 1
#define PIO_SWITCHES_DATA_WIDTH 8
#define PIO_SWITCHES_DO_TEST_BENCH_WIRING 0
#define PIO_SWITCHES_DRIVEN_SIM_VALUE 0
#define PIO_SWITCHES_EDGE_TYPE "RISING"
#define PIO_SWITCHES_FREQ 50000000
#define PIO_SWITCHES_HAS_IN 1
#define PIO_SWITCHES_HAS_OUT 0
#define PIO_SWITCHES_HAS_TRI 0
#define PIO_SWITCHES_IRQ 5
#define PIO_SWITCHES_IRQ_INTERRUPT_CONTROLLER_ID 0
#define PIO_SWITCHES_IRQ_TYPE "EDGE"
#define PIO_SWITCHES_NAME "/dev/PIO_Switches"
#define PIO_SWITCHES_RESET_VALUE 0
#define PIO_SWITCHES_SPAN 16
#define PIO_SWITCHES_TYPE "altera_avalon_pio"


/*
 * SDRAM_Controller_Shared configuration
 *
 */

#define ALT_MODULE_CLASS_SDRAM_Controller_Shared altera_avalon_new_sdram_controller
#define SDRAM_CONTROLLER_SHARED_BASE 0x0
#define SDRAM_CONTROLLER_SHARED_CAS_LATENCY 3
#define SDRAM_CONTROLLER_SHARED_CONTENTS_INFO
#define SDRAM_CONTROLLER_SHARED_INIT_NOP_DELAY 0.0
#define SDRAM_CONTROLLER_SHARED_INIT_REFRESH_COMMANDS 2
#define SDRAM_CONTROLLER_SHARED_IRQ -1
#define SDRAM_CONTROLLER_SHARED_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SDRAM_CONTROLLER_SHARED_IS_INITIALIZED 1
#define SDRAM_CONTROLLER_SHARED_NAME "/dev/SDRAM_Controller_Shared"
#define SDRAM_CONTROLLER_SHARED_POWERUP_DELAY 100.0
#define SDRAM_CONTROLLER_SHARED_REFRESH_PERIOD 7.8125
#define SDRAM_CONTROLLER_SHARED_REGISTER_DATA_IN 1
#define SDRAM_CONTROLLER_SHARED_SDRAM_ADDR_WIDTH 0x19
#define SDRAM_CONTROLLER_SHARED_SDRAM_BANK_WIDTH 2
#define SDRAM_CONTROLLER_SHARED_SDRAM_COL_WIDTH 10
#define SDRAM_CONTROLLER_SHARED_SDRAM_DATA_WIDTH 16
#define SDRAM_CONTROLLER_SHARED_SDRAM_NUM_BANKS 4
#define SDRAM_CONTROLLER_SHARED_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_CONTROLLER_SHARED_SDRAM_ROW_WIDTH 13
#define SDRAM_CONTROLLER_SHARED_SHARED_DATA 0
#define SDRAM_CONTROLLER_SHARED_SIM_MODEL_BASE 0
#define SDRAM_CONTROLLER_SHARED_SPAN 67108864
#define SDRAM_CONTROLLER_SHARED_STARVATION_INDICATOR 0
#define SDRAM_CONTROLLER_SHARED_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_CONTROLLER_SHARED_TYPE "altera_avalon_new_sdram_controller"
#define SDRAM_CONTROLLER_SHARED_T_AC 5.4
#define SDRAM_CONTROLLER_SHARED_T_MRD 3
#define SDRAM_CONTROLLER_SHARED_T_RCD 15.0
#define SDRAM_CONTROLLER_SHARED_T_RFC 70.0
#define SDRAM_CONTROLLER_SHARED_T_RP 15.0
#define SDRAM_CONTROLLER_SHARED_T_WR 14.0


/*
 * SysAudio_AV_Config configuration
 *
 */

#define ALT_MODULE_CLASS_SysAudio_AV_Config altera_up_avalon_audio_and_video_config
#define SYSAUDIO_AV_CONFIG_BASE 0x4081110
#define SYSAUDIO_AV_CONFIG_IRQ -1
#define SYSAUDIO_AV_CONFIG_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYSAUDIO_AV_CONFIG_NAME "/dev/SysAudio_AV_Config"
#define SYSAUDIO_AV_CONFIG_SPAN 16
#define SYSAUDIO_AV_CONFIG_TYPE "altera_up_avalon_audio_and_video_config"


/*
 * SysAudio_Audio_Core configuration
 *
 */

#define ALT_MODULE_CLASS_SysAudio_Audio_Core altera_up_avalon_audio
#define SYSAUDIO_AUDIO_CORE_BASE 0x4081120
#define SYSAUDIO_AUDIO_CORE_IRQ 0
#define SYSAUDIO_AUDIO_CORE_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SYSAUDIO_AUDIO_CORE_NAME "/dev/SysAudio_Audio_Core"
#define SYSAUDIO_AUDIO_CORE_SPAN 16
#define SYSAUDIO_AUDIO_CORE_TYPE "altera_up_avalon_audio"


/*
 * SysAudio_JTAG_UART configuration
 *
 */

#define ALT_MODULE_CLASS_SysAudio_JTAG_UART altera_avalon_jtag_uart
#define SYSAUDIO_JTAG_UART_BASE 0x4081130
#define SYSAUDIO_JTAG_UART_IRQ 6
#define SYSAUDIO_JTAG_UART_IRQ_INTERRUPT_CONTROLLER_ID 0
#define SYSAUDIO_JTAG_UART_NAME "/dev/SysAudio_JTAG_UART"
#define SYSAUDIO_JTAG_UART_READ_DEPTH 64
#define SYSAUDIO_JTAG_UART_READ_THRESHOLD 8
#define SYSAUDIO_JTAG_UART_SPAN 8
#define SYSAUDIO_JTAG_UART_TYPE "altera_avalon_jtag_uart"
#define SYSAUDIO_JTAG_UART_WRITE_DEPTH 64
#define SYSAUDIO_JTAG_UART_WRITE_THRESHOLD 8


/*
 * SysAudio_Onchip_Memory configuration
 *
 */

#define ALT_MODULE_CLASS_SysAudio_Onchip_Memory altera_avalon_onchip_memory2
#define SYSAUDIO_ONCHIP_MEMORY_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define SYSAUDIO_ONCHIP_MEMORY_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define SYSAUDIO_ONCHIP_MEMORY_BASE 0x4040000
#define SYSAUDIO_ONCHIP_MEMORY_CONTENTS_INFO ""
#define SYSAUDIO_ONCHIP_MEMORY_DUAL_PORT 0
#define SYSAUDIO_ONCHIP_MEMORY_GUI_RAM_BLOCK_TYPE "AUTO"
#define SYSAUDIO_ONCHIP_MEMORY_INIT_CONTENTS_FILE "soc_system_SysAudio_Onchip_Memory"
#define SYSAUDIO_ONCHIP_MEMORY_INIT_MEM_CONTENT 1
#define SYSAUDIO_ONCHIP_MEMORY_INSTANCE_ID "NONE"
#define SYSAUDIO_ONCHIP_MEMORY_IRQ -1
#define SYSAUDIO_ONCHIP_MEMORY_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYSAUDIO_ONCHIP_MEMORY_NAME "/dev/SysAudio_Onchip_Memory"
#define SYSAUDIO_ONCHIP_MEMORY_NON_DEFAULT_INIT_FILE_ENABLED 0
#define SYSAUDIO_ONCHIP_MEMORY_RAM_BLOCK_TYPE "AUTO"
#define SYSAUDIO_ONCHIP_MEMORY_READ_DURING_WRITE_MODE "DONT_CARE"
#define SYSAUDIO_ONCHIP_MEMORY_SINGLE_CLOCK_OP 0
#define SYSAUDIO_ONCHIP_MEMORY_SIZE_MULTIPLE 1
#define SYSAUDIO_ONCHIP_MEMORY_SIZE_VALUE 131072
#define SYSAUDIO_ONCHIP_MEMORY_SPAN 131072
#define SYSAUDIO_ONCHIP_MEMORY_TYPE "altera_avalon_onchip_memory2"
#define SYSAUDIO_ONCHIP_MEMORY_WRITABLE 1


/*
 * SysAudio_Performance_Counter configuration
 *
 */

#define ALT_MODULE_CLASS_SysAudio_Performance_Counter altera_avalon_performance_counter
#define SYSAUDIO_PERFORMANCE_COUNTER_BASE 0x4081000
#define SYSAUDIO_PERFORMANCE_COUNTER_HOW_MANY_SECTIONS 7
#define SYSAUDIO_PERFORMANCE_COUNTER_IRQ -1
#define SYSAUDIO_PERFORMANCE_COUNTER_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SYSAUDIO_PERFORMANCE_COUNTER_NAME "/dev/SysAudio_Performance_Counter"
#define SYSAUDIO_PERFORMANCE_COUNTER_SPAN 128
#define SYSAUDIO_PERFORMANCE_COUNTER_TYPE "altera_avalon_performance_counter"


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone V"
#define ALT_IRQ_BASE NULL
#define ALT_LEGACY_INTERRUPT_API_PRESENT
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/SysAudio_JTAG_UART"
#define ALT_STDERR_BASE 0x4081130
#define ALT_STDERR_DEV SysAudio_JTAG_UART
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/SysAudio_JTAG_UART"
#define ALT_STDIN_BASE 0x4081130
#define ALT_STDIN_DEV SysAudio_JTAG_UART
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/SysAudio_JTAG_UART"
#define ALT_STDOUT_BASE 0x4081130
#define ALT_STDOUT_DEV SysAudio_JTAG_UART
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "soc_system"


/*
 * hal configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 32
#define ALT_SYS_CLK none
#define ALT_TIMESTAMP_CLK none

#endif /* __SYSTEM_H_ */