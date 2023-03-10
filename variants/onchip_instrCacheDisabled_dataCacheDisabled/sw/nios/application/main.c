/* Main file for lab 1: interrupt time measurements
 */

#include <stdio.h>
#include "system.h"
#include "io.h"
#include "altera_avalon_pio_regs.h"

#define COUNT_RZ		1
#define COUNT_Start		2
#define COUNT_Stop		3
#define COUNT_IntEn		4
#define COUNT_IntClr	5
#define COUNT_Target	6

#define	Count_1sec		0x2FAF080
#define arbval			0x00000000



void test_parallel_port()
{
	IOWR_32DIRECT(PARALLELPORT_0_BASE, 0, 0x1234abcd);
	alt_printf("ParallelPort0=%x\n", IORD_32DIRECT(PARALLELPORT_0_BASE, 0));
}

void test_counter()
{
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_Target, Count_1sec);
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_RZ, arbval);
	alt_printf("iCounter after reset=%x\n", IORD_32DIRECT(COUNTER_0_BASE,0));
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_Start, arbval);
	alt_printf("iCounter while running=%x\n", IORD_32DIRECT(COUNTER_0_BASE,0));
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_Stop, arbval);
	alt_printf("iCounter, first read after stop=%x\n", IORD_32DIRECT(COUNTER_0_BASE,0));
	alt_printf("iCounter, second read after stop=%x\n", IORD_32DIRECT(COUNTER_0_BASE,0));
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_Start, arbval);
	alt_printf("iCounter, first read after restarting=%x\n", IORD_32DIRECT(COUNTER_0_BASE,0));
	alt_printf("iCounter, second read after restarting=%x\n", IORD_32DIRECT(COUNTER_0_BASE,0));
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_Stop, arbval);
}

int main()
{
	test_parallel_port();
	test_counter();
	return 0;
}
