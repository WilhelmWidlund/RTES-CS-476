/*
 *--------------------------------- THIS IS CPU 1 ---------------------------------------------
 */

#include <stdio.h>
#include "system.h"
#include "io.h"
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <altera_avalon_mutex.h>
#include "altera_avalon_mailbox_simple.h"
//#include "altera_avalon_mailbox_simple_regs.h"
#include <altera_avalon_performance_counter.h>

/*
 *--------------------------------- THIS IS CPU 1 ---------------------------------------------
 */
// REMEMBER TO CHANGE THE CPU ID VALUE IN SYSTEM.H AFTER GENERATING BSP

// To monitor terminal in powershell:
// nios2-terminal --device 2 --instance 1

// To download code to board
// nios2-download -g cpu_1_project.elf --device 2 --instance 1


#define ArbVal 0xffffffff

// Custom counter address offsets
#define CustomCounterValue 0	// For _reading_ access, address 0 is the current counter value
#define CustomCounterDecr 0		// For _writing_ access, address 0 is the command to Decrement by WriteData
#define CustomCounterReset 4
#define CustomCounterStart	8
#define CustomCounterStop	12
#define CustomCounterCommand	16
#define CustomCounterStatus	20
#define CustomCounterTarget 24
#define CustomCounterIncr 28

// PIO definitions
#define PIO_Data	0
#define PIO_IRQEN	4*2
#define PIO_IRQFLAG	4*3
// Variable for recording choices made by interrupt
volatile int choice;

void part1()
{
	printf("Hello from the multicore system, this is CPU %d talking! \n", NIOS2_CPU_ID_VALUE);
}

/*
 *--------------------------------- THIS IS CPU 1 ---------------------------------------------
 */

// Access parallel port 1 connected to LEDs 6, 5 & 4
// and increment counter 1 every mscound milliseconds
// for iterations amount of times.
void pptest(int msdelay, uint32_t iterations)
{
	// Start overall performance counter
	PERF_RESET(PERFORMANCE_COUNTER_1_BASE);
	PERF_START_MEASURING(PERFORMANCE_COUNTER_1_BASE);
	// Start performance counter, recording the setup portion (1)
	PERF_BEGIN(PERFORMANCE_COUNTER_1_BASE, 1);
	printf("Starting parallel port test on CPU %d\n", NIOS2_CPU_ID_VALUE);
	uint32_t itercount = 0x00;
	uint8_t curr_lamp = 0x00;
	// Reset custom counter
	IOWR_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterReset, ArbVal);
	// Set target value to be larger than iterations to be sure we never reach it (avoid triggering interrupt)
	IOWR_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterTarget, iterations + 1);
	// Stop performance counter for the setup portion (1)
	PERF_END(PERFORMANCE_COUNTER_1_BASE, 1);
	while(itercount < iterations)
	{
		// Start performance counter, recording the iterative work (2)
		PERF_BEGIN(PERFORMANCE_COUNTER_1_BASE, 2);
		// Access parallel port to toggle LED
		IOWR_8DIRECT(PARALLEL_PORT_1_BASE, 0, curr_lamp);
		// Increment counter
		IOWR_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterIncr, 0x01);
		// Make sure to toggle LEDs correctly for 3 available
		if(curr_lamp < 0x07)
		{
			curr_lamp = curr_lamp + 0x01;
		}
		else
		{
			curr_lamp = 0x00;
		}
		// Stop performance counter for the iterative work (2)
		PERF_END(PERFORMANCE_COUNTER_1_BASE, 2);
		// Start performance counter, recording the wait (3)
		PERF_BEGIN(PERFORMANCE_COUNTER_1_BASE, 3);
		// Wait msdelay ms
		usleep(1000 * msdelay);
		// Stop performance counter for the wait (3)
		PERF_END(PERFORMANCE_COUNTER_1_BASE, 3);
		// Get count value
		itercount = IORD_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterValue);
	}
	printf("Parallel port test on CPU %d finished\n\n", NIOS2_CPU_ID_VALUE);
	// Stop overall performance counter and print results
	PERF_STOP_MEASURING(PERFORMANCE_COUNTER_1_BASE);
	perf_print_formatted_report(PERFORMANCE_COUNTER_1_BASE, alt_get_cpu_freq(), 3, "Startup", "Iterations", "Sleep");
	printf("\n");
}

void hwmutex()
{
	// Initialize counter and timer variables
	uint8_t counter = 0x1;
	uint32_t timer_start = 0x0;
	uint32_t timer_stop = 0x0;
	// Fetch mutex address
	alt_mutex_dev* common_mutex = altera_avalon_mutex_open(MUTEX_0_NAME);
	// Wait for cpu_0 to finish setting up
	while(!altera_avalon_mutex_first_lock(common_mutex)){};
	// Setup and start custom counter
	IOWR_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterReset, ArbVal);
	IOWR_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterCommand, 0x0);
	IOWR_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterTarget, 0xffffffff);
	IOWR_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterStart, ArbVal);
	printf("Starting decrementation synchronized by Mutex.\n");
	while(counter > 0x0)
	{
		usleep(1000000);
		timer_start = IORD_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterValue);
		altera_avalon_mutex_lock(common_mutex, 2);
		counter = IORD_8DIRECT(PARALLEL_PORT_2_BASE, 0);
		counter = counter - 1;
		IOWR_8DIRECT(PARALLEL_PORT_2_BASE, 0, counter);
		altera_avalon_mutex_unlock(common_mutex);
		timer_stop = IORD_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterValue);
		printf("Decrement time: %ld cycles.\n", (timer_stop - timer_start));
	}
	printf("Finished with decrementation synchronized by Mutex.\n\n");
}

void hwcounter()
{
	// Initialize counter and timer variables
	uint8_t LED_Counter = 0x0f;
	uint32_t timer_start = 0x0;
	uint32_t timer_stop = 0x0;
	// Setup and start custom counter 1, used for measuring how long access takes
	IOWR_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterReset, ArbVal);
	IOWR_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterCommand, 0x0);
	IOWR_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterTarget, 0xffffffff);
	IOWR_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterStart, ArbVal);
	// Wait for the other processor to finish setting up custom counter 2
	uint8_t ready = (uint8_t)(IORD_32DIRECT(CUSTOM_COUNTER_2_BASE, CustomCounterValue) & 0xff);
	while(ready != LED_Counter)
	{
		ready = (uint8_t)(IORD_32DIRECT(CUSTOM_COUNTER_2_BASE, CustomCounterValue) & 0xff);
	}
	printf("Starting decrementation of shared hardware counter.\n");
	while(LED_Counter > 0x0)
	{
		// Increment every 10 ms
		usleep(1000000);
		// Read start value
		timer_start = IORD_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterValue);

		// Decrement counter 2
		IOWR_32DIRECT(CUSTOM_COUNTER_2_BASE, CustomCounterDecr, 0x01);
		// Read counter 2 value
		LED_Counter = (uint8_t)(IORD_32DIRECT(CUSTOM_COUNTER_2_BASE, 0) & 0xFF);

		// Display new value on LED
		IOWR_8DIRECT(PARALLEL_PORT_2_BASE, 0, LED_Counter);

		timer_stop = IORD_32DIRECT(CUSTOM_COUNTER_1_BASE, CustomCounterValue);
		printf("Decrement time: %ld cycles.\n", (timer_stop - timer_start));

	}
	printf("Finished with decrementation of shared hardware counter.\n\n");
}

void receive_callback(void* message)
{
	if(message != NULL)
	{
		printf("Receiving completed.\n");
	}
	else
	{
		printf("Receiving error.\n");
	}
}

void receive_mail()
{
	printf("Using the mailbox, receiver side.\n");
	// Load mailbox
	altera_avalon_mailbox_dev* mailbox = altera_avalon_mailbox_open("/dev/mailbox_simple_0", NULL, receive_callback);
	// Message storage array
	alt_u32 mail[2];
	altera_avalon_mailbox_retrieve_poll(mailbox, mail, 0);
	alt_dcache_flush_all();
	char *msg_ptr = (void*) mail[1];
	printf("Received message with contents: ");
	for(int i = 0; i < mail[0]; i++)
	{
		printf("%c", *(msg_ptr + i));
	}
	altera_avalon_mailbox_close(mailbox);
	printf("\nDone using the mailbox, receiver side.\n\n");
}

void choose_task(int task)
{
	if(task == 1)
	{
		// Manipulation 1: print tests
		part1();
	}
	else if(task == 2)
	{
		// 3.1 Parallel port test, 100 iterations, 50 ms each
		uint32_t iters = 0x64;
		int delaytime = 500;
		pptest(delaytime, iters);
	}
	else if(task == 3)
	{
		// Manipulation 2: hardware mutex
		hwmutex();
	}
	else if(task == 4)
	{
		// Manipulation 3: hardware mailbox
		receive_mail();
	}
	else if(task == 5)
	{
		// Manipulation 4: hardware counter
		hwcounter();
	}
}

void isr_buttons(void* context)
{
	uint32_t pinvals = IORD_8DIRECT(PIO_2_BASE, PIO_IRQFLAG);
	if(pinvals == 0x1)
	{
		choice = 0x2;
		// Clear the interrupt flag
		IOWR_8DIRECT(PIO_2_BASE, PIO_IRQFLAG, pinvals);
	}
	else if (pinvals == 0x2)
	{
		choice = 0x3;
		// Clear the interrupt flag
		IOWR_8DIRECT(PIO_2_BASE, PIO_IRQFLAG, pinvals);
	}
	else if (pinvals == 0x4)
	{
		choice = 0x4;
		// Clear the interrupt flag
		IOWR_8DIRECT(PIO_2_BASE, PIO_IRQFLAG, pinvals);
	}
	else if (pinvals == 0x8)
	{
		choice = 0x5;
		// Clear the interrupt flag
		IOWR_8DIRECT(PIO_2_BASE, PIO_IRQFLAG, pinvals);
	}
}

int main()
{
	// Setup interrupts on input pins
	IOWR_8DIRECT(PIO_2_BASE, PIO_IRQEN, 0xff);
	alt_ic_isr_register(PIO_2_IRQ_INTERRUPT_CONTROLLER_ID, PIO_2_IRQ, isr_buttons, NULL, NULL);
	// Print which CPU it is
	choose_task(1);
	printf("Use switches for performing tasks:\nSwitch No. |   Task\n   0       |   Parallel Port test\n   1       |   Hardware mutex\n   2       |   Hardware mailbox\n   3       |   Hardware counter\n\n");
	// Wait for buttons
	while(1)
	{
		// Poll choice once every millisecond
		usleep(1000);
		if(choice != 0x0)
		{
			choose_task(choice);
			choice = 0x0;
		}
	};
	return 0;
}

/*
 *--------------------------------- THIS IS CPU 1 ---------------------------------------------
 */
