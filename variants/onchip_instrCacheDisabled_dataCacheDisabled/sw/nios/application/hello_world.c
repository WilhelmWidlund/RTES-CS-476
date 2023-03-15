/* Main file for lab 1: interrupt time measurements
 */

#include <stdio.h>
#include "system.h"
#include "io.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer_regs.h"

#define COUNT_RZ		4
#define COUNT_Start		8
#define COUNT_Stop		12
#define COUNT_IntEn		16
#define COUNT_IntClr	20
#define COUNT_Target	24

#define PARPORT_Data	0
#define PARPORT_SetBits	4
#define PARPORT_ClrBits	8
#define PARPORT_IRQEN	12
#define PARPORT_IRQCLR	16

// Configure the timer IP to be in continuous mode, with interrupts enabled, and start it
#define TIMER_settings_start	7

#define	Count_1sec		0x02FAF080
#define	Count_1secL		0xF080
#define	Count_1secH		0x02FA
#define arbval			0x0000ffff

int snapl;
int snaph;
int flag;





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
	alt_printf("iEn and iEOT=%x\n", IORD_32DIRECT(COUNTER_0_BASE, 20));
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_Start, arbval);
	alt_printf("iCounter while running=%x\n", IORD_32DIRECT(COUNTER_0_BASE,0));
	alt_printf("iEn and iEOT=%x\n", IORD_32DIRECT(COUNTER_0_BASE, 20));
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_Stop, arbval);
	alt_printf("iCounter, first read after stop=%x\n", IORD_32DIRECT(COUNTER_0_BASE,0));
	alt_printf("iCounter, second read after stop=%x\n", IORD_32DIRECT(COUNTER_0_BASE,0));
	alt_printf("iEn and iEOT=%x\n", IORD_32DIRECT(COUNTER_0_BASE, 20));
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_Start, arbval);
	alt_printf("iCounter, first read after restarting=%x\n", IORD_32DIRECT(COUNTER_0_BASE,0));
	alt_printf("iCounter, second read after restarting=%x\n", IORD_32DIRECT(COUNTER_0_BASE,0));
	alt_printf("iEn and iEOT=%x\n", IORD_32DIRECT(COUNTER_0_BASE, 20));
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_Stop, arbval);
}

void resp_isr_print(void* context)
{
	IOWR_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE, arbval);
	IOWR_ALTERA_AVALON_TIMER_SNAPH(TIMER_0_BASE, arbval);
	snapl = IORD_ALTERA_AVALON_TIMER_SNAPL(TIMER_0_BASE);
	snaph = IORD_ALTERA_AVALON_TIMER_SNAPH(TIMER_0_BASE);
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0);
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);
	flag = 1;
}

void resp_isr_parport(void* context)
{
	IOWR_32DIRECT(PARALLELPORT_0_BASE, PARPORT_Data, 0x0);
	IOWR_32DIRECT(PARALLELPORT_0_BASE, PARPORT_IRQCLR, arbval);
}

void test_resptime_print()
{
	// Register isr
	alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, resp_isr_print, 0, 0);
	// Initialize flag value
	flag = 0;
	// Initialize timer
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0);
	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_0_BASE, 0xF080);
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_0_BASE, 0x02FA);
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, TIMER_settings_start);
	// Setup loop structure
	int counter = 0;
	int max_iter = 10;
	int accum_val = 0;
	int avg_val = 0;
	int snapval = 0;
	printf("Testing response time using stock timer and\n"
			" counting the average over %d iterations...\n", max_iter);
	while(counter < max_iter)
	{
		if(flag)
		{
			snapval = Count_1secH-snaph<<16|Count_1secL-snapl;
			alt_printf("Value at iteration %x is 0x%x\n", counter+1, snapval);
			flag = 0;
			IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, TIMER_settings_start);
			accum_val += snapval;
			counter += 1;
		}
	}
	avg_val = accum_val/max_iter;
	printf("Average value for response time is %d", avg_val);
}
void test_resptime_parport()
{
	// Register isr
	alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, resp_isr_parport, 0, 0);
	IOWR_32DIRECT(PARALLELPORT_0_BASE, PARPORT_Data, 0x0);
	IOWR_32DIRECT(PARALLELPORT_0_BASE, PARPORT_IRQEN, 0x2);
	int iterations = 1e2;
	int counter = 0;
	while(counter < iterations)
	{
		IOWR_32DIRECT(PARALLELPORT_0_BASE, PARPORT_Data, 0x2);
		counter += 1;
	}
}

void recov_isr_print(void* context)
{
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0);
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_Start, arbval);
}

void recov_isr_parport(void* context)
{

}

void test_recovtime_print()
{
	// Register interrupt function
	alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID, TIMER_0_IRQ, recov_isr_print, 0, 0);
	// Initialize counter, with maximum target value
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_Target, 0xFFFFFFFF);
	IOWR_32DIRECT(COUNTER_0_BASE, COUNT_RZ, arbval);
	// Initialize timer
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, 0);
	IOWR_ALTERA_AVALON_TIMER_PERIODL(TIMER_0_BASE, 0xF080);
	IOWR_ALTERA_AVALON_TIMER_PERIODH(TIMER_0_BASE, 0x02FA);
	IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, TIMER_settings_start);
	// Setup loop structure
	int counter = 0;
	int max_iter = 10;
	int accum_val = 0;
	int avg_val = 0;
	int read_val;
	printf("\n\nTesting recovery time using custom counter and\n"
			" counting the average over %d iterations...\n", max_iter);
	while(counter < max_iter)
	{
		read_val = IORD_32DIRECT(COUNTER_0_BASE,0);
		while(read_val == 0)
		{
			read_val = IORD_32DIRECT(COUNTER_0_BASE,0);
		}
		alt_printf("Value at iteration %x is 0x%x\n", counter+1, read_val);
		accum_val += read_val;
		counter += 1;
		IOWR_32DIRECT(COUNTER_0_BASE, COUNT_Stop, arbval);
		IOWR_32DIRECT(COUNTER_0_BASE, COUNT_RZ, arbval);
		IOWR_ALTERA_AVALON_TIMER_CONTROL(TIMER_0_BASE, TIMER_settings_start);
	}
	avg_val = accum_val/max_iter;
	printf("Average value for recovery time is %d", avg_val);
}

void test_recovtime_parport()
{

}

int main()
{
	//test_parallel_port();
	//test_counter();
	//test_resptime_print();
	//test_recovtime_print();
	test_resptime_parport();
	return 0;
}
