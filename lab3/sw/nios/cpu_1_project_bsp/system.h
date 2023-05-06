/*
 * system.h - SOPC Builder system and BSP software package information
 *
 * Machine generated for CPU 'cpu_1' in SOPC Builder design 'soc_system'
 * SOPC Builder design path: C:/RTES/lab3/hw/quartus/soc_system.sopcinfo
 *
 * Generated: Sat May 06 16:46:16 CEST 2023
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
#define ALT_CPU_BREAK_ADDR 0x04040820
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
#define ALT_CPU_EXCEPTION_ADDR 0x04020020
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
#define ALT_CPU_NAME "cpu_1"
#define ALT_CPU_NUM_OF_SHADOW_REG_SETS 0
#define ALT_CPU_OCI_VERSION 1
#define ALT_CPU_RESET_ADDR 0x04020000


/*
 * CPU configuration (with legacy prefix - don't use these anymore)
 *
 */

#define NIOS2_BIG_ENDIAN 0
#define NIOS2_BREAK_ADDR 0x04040820
#define NIOS2_CPU_ARCH_NIOS2_R1
#define NIOS2_CPU_FREQ 50000000u
#define NIOS2_CPU_ID_SIZE 1
#define NIOS2_CPU_ID_VALUE 0x00000001
#define NIOS2_CPU_IMPLEMENTATION "fast"
#define NIOS2_DATA_ADDR_WIDTH 0x1b
#define NIOS2_DCACHE_BYPASS_MASK 0x80000000
#define NIOS2_DCACHE_LINE_SIZE 32
#define NIOS2_DCACHE_LINE_SIZE_LOG2 5
#define NIOS2_DCACHE_SIZE 2048
#define NIOS2_EXCEPTION_ADDR 0x04020020
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
#define NIOS2_RESET_ADDR 0x04020000


/*
 * Define for each module class mastered by the CPU
 *
 */

#define __ALTERA_AVALON_JTAG_UART
#define __ALTERA_AVALON_MAILBOX_SIMPLE
#define __ALTERA_AVALON_MUTEX
#define __ALTERA_AVALON_NEW_SDRAM_CONTROLLER
#define __ALTERA_AVALON_ONCHIP_MEMORY2
#define __ALTERA_AVALON_PERFORMANCE_COUNTER
#define __ALTERA_AVALON_PIO
#define __ALTERA_AVALON_TIMER
#define __ALTERA_NIOS2_GEN2
#define __ALTERA_UP_AVALON_PARALLEL_PORT
#define __CUSTOM_COUNTER_0


/*
 * System configuration
 *
 */

#define ALT_DEVICE_FAMILY "Cyclone V"
#define ALT_ENHANCED_INTERRUPT_API_PRESENT
#define ALT_IRQ_BASE NULL
#define ALT_LOG_PORT "/dev/null"
#define ALT_LOG_PORT_BASE 0x0
#define ALT_LOG_PORT_DEV null
#define ALT_LOG_PORT_TYPE ""
#define ALT_NUM_EXTERNAL_INTERRUPT_CONTROLLERS 0
#define ALT_NUM_INTERNAL_INTERRUPT_CONTROLLERS 1
#define ALT_NUM_INTERRUPT_CONTROLLERS 1
#define ALT_STDERR "/dev/jtag_uart_1"
#define ALT_STDERR_BASE 0x40410e8
#define ALT_STDERR_DEV jtag_uart_1
#define ALT_STDERR_IS_JTAG_UART
#define ALT_STDERR_PRESENT
#define ALT_STDERR_TYPE "altera_avalon_jtag_uart"
#define ALT_STDIN "/dev/jtag_uart_1"
#define ALT_STDIN_BASE 0x40410e8
#define ALT_STDIN_DEV jtag_uart_1
#define ALT_STDIN_IS_JTAG_UART
#define ALT_STDIN_PRESENT
#define ALT_STDIN_TYPE "altera_avalon_jtag_uart"
#define ALT_STDOUT "/dev/jtag_uart_1"
#define ALT_STDOUT_BASE 0x40410e8
#define ALT_STDOUT_DEV jtag_uart_1
#define ALT_STDOUT_IS_JTAG_UART
#define ALT_STDOUT_PRESENT
#define ALT_STDOUT_TYPE "altera_avalon_jtag_uart"
#define ALT_SYSTEM_NAME "soc_system"


/*
 * custom_counter_1 configuration
 *
 */

#define ALT_MODULE_CLASS_custom_counter_1 custom_counter_0
#define CUSTOM_COUNTER_1_BASE 0x4041080
#define CUSTOM_COUNTER_1_IRQ -1
#define CUSTOM_COUNTER_1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define CUSTOM_COUNTER_1_NAME "/dev/custom_counter_1"
#define CUSTOM_COUNTER_1_SPAN 32
#define CUSTOM_COUNTER_1_TYPE "custom_counter_0"


/*
 * custom_counter_2 configuration
 *
 */

#define ALT_MODULE_CLASS_custom_counter_2 custom_counter_0
#define CUSTOM_COUNTER_2_BASE 0x4041040
#define CUSTOM_COUNTER_2_IRQ -1
#define CUSTOM_COUNTER_2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define CUSTOM_COUNTER_2_NAME "/dev/custom_counter_2"
#define CUSTOM_COUNTER_2_SPAN 32
#define CUSTOM_COUNTER_2_TYPE "custom_counter_0"


/*
 * hal configuration
 *
 */

#define ALT_INCLUDE_INSTRUCTION_RELATED_EXCEPTION_API
#define ALT_MAX_FD 32
#define ALT_SYS_CLK TIMER_1
#define ALT_TIMESTAMP_CLK none


/*
 * jtag_uart_1 configuration
 *
 */

#define ALT_MODULE_CLASS_jtag_uart_1 altera_avalon_jtag_uart
#define JTAG_UART_1_BASE 0x40410e8
#define JTAG_UART_1_IRQ 2
#define JTAG_UART_1_IRQ_INTERRUPT_CONTROLLER_ID 0
#define JTAG_UART_1_NAME "/dev/jtag_uart_1"
#define JTAG_UART_1_READ_DEPTH 64
#define JTAG_UART_1_READ_THRESHOLD 8
#define JTAG_UART_1_SPAN 8
#define JTAG_UART_1_TYPE "altera_avalon_jtag_uart"
#define JTAG_UART_1_WRITE_DEPTH 64
#define JTAG_UART_1_WRITE_THRESHOLD 8


/*
 * mailbox_simple_0 configuration
 *
 */

#define ALT_MODULE_CLASS_mailbox_simple_0 altera_avalon_mailbox_simple
#define MAILBOX_SIMPLE_0_BASE 0x40410b0
#define MAILBOX_SIMPLE_0_IRQ 0
#define MAILBOX_SIMPLE_0_IRQ_INTERRUPT_CONTROLLER_ID 0
#define MAILBOX_SIMPLE_0_NAME "/dev/mailbox_simple_0"
#define MAILBOX_SIMPLE_0_SPAN 16
#define MAILBOX_SIMPLE_0_TYPE "altera_avalon_mailbox_simple"


/*
 * mutex_0 configuration
 *
 */

#define ALT_MODULE_CLASS_mutex_0 altera_avalon_mutex
#define MUTEX_0_BASE 0x40410e0
#define MUTEX_0_IRQ -1
#define MUTEX_0_IRQ_INTERRUPT_CONTROLLER_ID -1
#define MUTEX_0_NAME "/dev/mutex_0"
#define MUTEX_0_OWNER_INIT 0
#define MUTEX_0_OWNER_WIDTH 16
#define MUTEX_0_SPAN 8
#define MUTEX_0_TYPE "altera_avalon_mutex"
#define MUTEX_0_VALUE_INIT 0
#define MUTEX_0_VALUE_WIDTH 16


/*
 * onchip_memory_1 configuration
 *
 */

#define ALT_MODULE_CLASS_onchip_memory_1 altera_avalon_onchip_memory2
#define ONCHIP_MEMORY_1_ALLOW_IN_SYSTEM_MEMORY_CONTENT_EDITOR 0
#define ONCHIP_MEMORY_1_ALLOW_MRAM_SIM_CONTENTS_ONLY_FILE 0
#define ONCHIP_MEMORY_1_BASE 0x4020000
#define ONCHIP_MEMORY_1_CONTENTS_INFO ""
#define ONCHIP_MEMORY_1_DUAL_PORT 0
#define ONCHIP_MEMORY_1_GUI_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY_1_INIT_CONTENTS_FILE "soc_system_onchip_memory_1"
#define ONCHIP_MEMORY_1_INIT_MEM_CONTENT 1
#define ONCHIP_MEMORY_1_INSTANCE_ID "NONE"
#define ONCHIP_MEMORY_1_IRQ -1
#define ONCHIP_MEMORY_1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define ONCHIP_MEMORY_1_NAME "/dev/onchip_memory_1"
#define ONCHIP_MEMORY_1_NON_DEFAULT_INIT_FILE_ENABLED 0
#define ONCHIP_MEMORY_1_RAM_BLOCK_TYPE "AUTO"
#define ONCHIP_MEMORY_1_READ_DURING_WRITE_MODE "DONT_CARE"
#define ONCHIP_MEMORY_1_SINGLE_CLOCK_OP 0
#define ONCHIP_MEMORY_1_SIZE_MULTIPLE 1
#define ONCHIP_MEMORY_1_SIZE_VALUE 131072
#define ONCHIP_MEMORY_1_SPAN 131072
#define ONCHIP_MEMORY_1_TYPE "altera_avalon_onchip_memory2"
#define ONCHIP_MEMORY_1_WRITABLE 1


/*
 * parallel_port_1 configuration
 *
 */

#define ALT_MODULE_CLASS_parallel_port_1 altera_up_avalon_parallel_port
#define PARALLEL_PORT_1_BASE 0x40410d0
#define PARALLEL_PORT_1_IRQ -1
#define PARALLEL_PORT_1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PARALLEL_PORT_1_NAME "/dev/parallel_port_1"
#define PARALLEL_PORT_1_SPAN 16
#define PARALLEL_PORT_1_TYPE "altera_up_avalon_parallel_port"


/*
 * parallel_port_2 configuration
 *
 */

#define ALT_MODULE_CLASS_parallel_port_2 altera_up_avalon_parallel_port
#define PARALLEL_PORT_2_BASE 0x40410c0
#define PARALLEL_PORT_2_IRQ -1
#define PARALLEL_PORT_2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PARALLEL_PORT_2_NAME "/dev/parallel_port_2"
#define PARALLEL_PORT_2_SPAN 16
#define PARALLEL_PORT_2_TYPE "altera_up_avalon_parallel_port"


/*
 * performance_counter_1 configuration
 *
 */

#define ALT_MODULE_CLASS_performance_counter_1 altera_avalon_performance_counter
#define PERFORMANCE_COUNTER_1_BASE 0x4041000
#define PERFORMANCE_COUNTER_1_HOW_MANY_SECTIONS 3
#define PERFORMANCE_COUNTER_1_IRQ -1
#define PERFORMANCE_COUNTER_1_IRQ_INTERRUPT_CONTROLLER_ID -1
#define PERFORMANCE_COUNTER_1_NAME "/dev/performance_counter_1"
#define PERFORMANCE_COUNTER_1_SPAN 64
#define PERFORMANCE_COUNTER_1_TYPE "altera_avalon_performance_counter"


/*
 * pio_2 configuration
 *
 */

#define ALT_MODULE_CLASS_pio_2 altera_avalon_pio
#define PIO_2_BASE 0x40410a0
#define PIO_2_BIT_CLEARING_EDGE_REGISTER 1
#define PIO_2_BIT_MODIFYING_OUTPUT_REGISTER 0
#define PIO_2_CAPTURE 1
#define PIO_2_DATA_WIDTH 8
#define PIO_2_DO_TEST_BENCH_WIRING 0
#define PIO_2_DRIVEN_SIM_VALUE 0
#define PIO_2_EDGE_TYPE "RISING"
#define PIO_2_FREQ 50000000
#define PIO_2_HAS_IN 1
#define PIO_2_HAS_OUT 0
#define PIO_2_HAS_TRI 0
#define PIO_2_IRQ 4
#define PIO_2_IRQ_INTERRUPT_CONTROLLER_ID 0
#define PIO_2_IRQ_TYPE "EDGE"
#define PIO_2_NAME "/dev/pio_2"
#define PIO_2_RESET_VALUE 0
#define PIO_2_SPAN 16
#define PIO_2_TYPE "altera_avalon_pio"


/*
 * sdram_controller_2 configuration
 *
 */

#define ALT_MODULE_CLASS_sdram_controller_2 altera_avalon_new_sdram_controller
#define SDRAM_CONTROLLER_2_BASE 0x0
#define SDRAM_CONTROLLER_2_CAS_LATENCY 3
#define SDRAM_CONTROLLER_2_CONTENTS_INFO
#define SDRAM_CONTROLLER_2_INIT_NOP_DELAY 0.0
#define SDRAM_CONTROLLER_2_INIT_REFRESH_COMMANDS 2
#define SDRAM_CONTROLLER_2_IRQ -1
#define SDRAM_CONTROLLER_2_IRQ_INTERRUPT_CONTROLLER_ID -1
#define SDRAM_CONTROLLER_2_IS_INITIALIZED 1
#define SDRAM_CONTROLLER_2_NAME "/dev/sdram_controller_2"
#define SDRAM_CONTROLLER_2_POWERUP_DELAY 100.0
#define SDRAM_CONTROLLER_2_REFRESH_PERIOD 7.8125
#define SDRAM_CONTROLLER_2_REGISTER_DATA_IN 1
#define SDRAM_CONTROLLER_2_SDRAM_ADDR_WIDTH 0x19
#define SDRAM_CONTROLLER_2_SDRAM_BANK_WIDTH 2
#define SDRAM_CONTROLLER_2_SDRAM_COL_WIDTH 10
#define SDRAM_CONTROLLER_2_SDRAM_DATA_WIDTH 16
#define SDRAM_CONTROLLER_2_SDRAM_NUM_BANKS 4
#define SDRAM_CONTROLLER_2_SDRAM_NUM_CHIPSELECTS 1
#define SDRAM_CONTROLLER_2_SDRAM_ROW_WIDTH 13
#define SDRAM_CONTROLLER_2_SHARED_DATA 0
#define SDRAM_CONTROLLER_2_SIM_MODEL_BASE 0
#define SDRAM_CONTROLLER_2_SPAN 67108864
#define SDRAM_CONTROLLER_2_STARVATION_INDICATOR 0
#define SDRAM_CONTROLLER_2_TRISTATE_BRIDGE_SLAVE ""
#define SDRAM_CONTROLLER_2_TYPE "altera_avalon_new_sdram_controller"
#define SDRAM_CONTROLLER_2_T_AC 5.4
#define SDRAM_CONTROLLER_2_T_MRD 3
#define SDRAM_CONTROLLER_2_T_RCD 15.0
#define SDRAM_CONTROLLER_2_T_RFC 70.0
#define SDRAM_CONTROLLER_2_T_RP 15.0
#define SDRAM_CONTROLLER_2_T_WR 14.0


/*
 * timer_1 configuration
 *
 */

#define ALT_MODULE_CLASS_timer_1 altera_avalon_timer
#define TIMER_1_ALWAYS_RUN 0
#define TIMER_1_BASE 0x4041060
#define TIMER_1_COUNTER_SIZE 32
#define TIMER_1_FIXED_PERIOD 0
#define TIMER_1_FREQ 50000000
#define TIMER_1_IRQ 3
#define TIMER_1_IRQ_INTERRUPT_CONTROLLER_ID 0
#define TIMER_1_LOAD_VALUE 49999
#define TIMER_1_MULT 0.001
#define TIMER_1_NAME "/dev/timer_1"
#define TIMER_1_PERIOD 1
#define TIMER_1_PERIOD_UNITS "ms"
#define TIMER_1_RESET_OUTPUT 0
#define TIMER_1_SNAPSHOT 1
#define TIMER_1_SPAN 32
#define TIMER_1_TICKS_PER_SEC 1000
#define TIMER_1_TIMEOUT_PULSE_OUTPUT 0
#define TIMER_1_TYPE "altera_avalon_timer"

#endif /* __SYSTEM_H_ */