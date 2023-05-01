/*
 * alt_sys_init.c - HAL initialization source
 *
 * Machine generated for CPU 'cpu_top' in SOPC Builder design 'multiprocessor_tutorial_main_system'
 * SOPC Builder design path: ../../multiprocessor_tutorial_main_system.sopcinfo
 *
 * Generated: Mon May 23 11:32:18 PDT 2011
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

#include "system.h"
#include "sys/alt_irq.h"
#include "sys/alt_sys_init.h"

#include <stddef.h>

/*
 * Device headers
 */

#include "altera_nios2_qsys_irq.h"
#include "altera_avalon_jtag_uart.h"
#include "altera_avalon_mutex.h"
#include "altera_avalon_sysid_qsys.h"
#include "altera_avalon_timer.h"

/*
 * Allocate the device storage
 */

ALTERA_NIOS2_QSYS_IRQ_INSTANCE ( CPU_TOP, cpu_top);
ALTERA_AVALON_JTAG_UART_INSTANCE ( JTAG_UART_TOP, jtag_uart_top);
ALTERA_AVALON_MUTEX_INSTANCE ( PHILOSOPHER_FOUR_CHOPSTICK_MUTEX, philosopher_four_chopstick_mutex);
ALTERA_AVALON_MUTEX_INSTANCE ( PHILOSOPHER_ONE_CHOPSTICK_MUTEX, philosopher_one_chopstick_mutex);
ALTERA_AVALON_MUTEX_INSTANCE ( PHILOSOPHER_THREE_CHOPSTICK_MUTEX, philosopher_three_chopstick_mutex);
ALTERA_AVALON_MUTEX_INSTANCE ( PHILOSOPHER_TWO_CHOPSTICK_MUTEX, philosopher_two_chopstick_mutex);
ALTERA_AVALON_MUTEX_INSTANCE ( PHILOSOPHER_ZERO_CHOPSTICK_MUTEX, philosopher_zero_chopstick_mutex);
ALTERA_AVALON_SYSID_QSYS_INSTANCE ( SYSID_QSYS, sysid_qsys);
ALTERA_AVALON_TIMER_INSTANCE ( TIMER_TOP, timer_top);

/*
 * Initialize the interrupt controller devices
 * and then enable interrupts in the CPU.
 * Called before alt_sys_init().
 * The "base" parameter is ignored and only
 * present for backwards-compatibility.
 */

void alt_irq_init ( const void* base )
{
    ALTERA_NIOS2_QSYS_IRQ_INIT ( CPU_TOP, cpu_top);
    alt_irq_cpu_enable_interrupts();
}

/*
 * Initialize the non-interrupt controller devices.
 * Called after alt_irq_init().
 */

void alt_sys_init( void )
{
    ALTERA_AVALON_TIMER_INIT ( TIMER_TOP, timer_top);
    ALTERA_AVALON_JTAG_UART_INIT ( JTAG_UART_TOP, jtag_uart_top);
    ALTERA_AVALON_MUTEX_INIT ( PHILOSOPHER_FOUR_CHOPSTICK_MUTEX, philosopher_four_chopstick_mutex);
    ALTERA_AVALON_MUTEX_INIT ( PHILOSOPHER_ONE_CHOPSTICK_MUTEX, philosopher_one_chopstick_mutex);
    ALTERA_AVALON_MUTEX_INIT ( PHILOSOPHER_THREE_CHOPSTICK_MUTEX, philosopher_three_chopstick_mutex);
    ALTERA_AVALON_MUTEX_INIT ( PHILOSOPHER_TWO_CHOPSTICK_MUTEX, philosopher_two_chopstick_mutex);
    ALTERA_AVALON_MUTEX_INIT ( PHILOSOPHER_ZERO_CHOPSTICK_MUTEX, philosopher_zero_chopstick_mutex);
    ALTERA_AVALON_SYSID_QSYS_INIT ( SYSID_QSYS, sysid_qsys);
}
