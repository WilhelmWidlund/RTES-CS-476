/*
 *--------------------------------- THIS IS CPU 0 ---------------------------------------------
 * Main code for the cpu 0 subsystem 
 */

#include <stdio.h>
#include "system.h"
#include "io.h"
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <altera_avalon_mutex.h>
#include "altera_avalon_mailbox_simple.h"
#include <altera_avalon_performance_counter.h>

/*
 *--------------------------------- THIS IS CPU 0 ---------------------------------------------
 */

// To monitor terminal in powershell:
// nios2-terminal --device 2 --instance 0

// To download code to board
// nios2-download -g cpu_0_project.elf --device 2 --instance 0

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

// Message max length
#define MSG_MAX		50

void part1()
{
	printf("Hello from the multicore system, this is CPU %d talking! \n", NIOS2_CPU_ID_VALUE);
}

// Access parallel port 0 connected to LEDs 3, 2 & 1
// and increment counter 0 every mscound milliseconds
// for iterations amount of times.
void pptest(int msdelay, uint32_t iterations)
{
	// Start overall performance counter
	PERF_RESET(PERFORMANCE_COUNTER_0_BASE);
	PERF_START_MEASURING(PERFORMANCE_COUNTER_0_BASE);
	// Start performance counter, recording the setup portion (1)
	PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 1);
	printf("Starting parallel port test on CPU %d\n", NIOS2_CPU_ID_VALUE);
	uint32_t itercount = 0x03;
	uint8_t curr_lamp = 0x03;
	// Reset custom counter
	IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterReset, ArbVal);
	// Set target value to be larger than iterations to be sure we never reach it (avoid triggering interrupt)
	IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterTarget, iterations + 1);
	// Stop performance counter for the setup portion (1)
	PERF_END(PERFORMANCE_COUNTER_0_BASE, 1);
	while(itercount < iterations)
	{
		// Start performance counter, recording the iterative work (2)
		PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 2);
		// Access parallel port to toggle LED
		IOWR_8DIRECT(PARALLEL_PORT_0_BASE, 0, curr_lamp);
		// Increment counter
		IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterIncr, 0x01);
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
		PERF_END(PERFORMANCE_COUNTER_0_BASE, 2);
		// Start performance counter, recording the wait (3)
		PERF_BEGIN(PERFORMANCE_COUNTER_0_BASE, 3);
		// Wait msdelay ms
		usleep(1000 * msdelay);
		// Stop performance counter for the wait (3)
		PERF_END(PERFORMANCE_COUNTER_0_BASE, 3);
		// Get count value
		itercount = IORD_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterValue);
	}
	printf("Parallel port test on CPU %d finished\n\n", NIOS2_CPU_ID_VALUE);
	// Stop overall performance counter and print results
	PERF_STOP_MEASURING(PERFORMANCE_COUNTER_0_BASE);
	perf_print_formatted_report(PERFORMANCE_COUNTER_0_BASE, alt_get_cpu_freq(), 3, "Startup", "Iterations", "Sleep");
	printf("\n");
}

void hwmutex(uint8_t startvalue)
{
	// Initialize counter and timer variables
	uint8_t counter = 0x1;
	uint32_t timer_start = 0x0;
	uint32_t timer_stop = 0x0;
	// Fetch mutex address
	alt_mutex_dev* common_mutex = altera_avalon_mutex_open(MUTEX_0_NAME);
	// Setup start value
	altera_avalon_mutex_lock(common_mutex, 1);
	IOWR_8DIRECT(PARALLEL_PORT_2_BASE, 0, startvalue);
	altera_avalon_mutex_unlock(common_mutex);
	// Setup and start custom counter
	IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterReset, ArbVal);
	IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterCommand, 0x0);
	IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterTarget, 0xffffffff);
	IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterStart, ArbVal);
	printf("Starting incrementation synchronized by Mutex.\n");
	while(counter > 0x0)
	{
		// Increment every 20 ms
		usleep(20000);
		// Read start value
		timer_start = IORD_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterValue);
		altera_avalon_mutex_lock(common_mutex, 1);
		counter = IORD_8DIRECT(PARALLEL_PORT_2_BASE, 0);
		IOWR_8DIRECT(PARALLEL_PORT_2_BASE, 0, counter + 1);
		altera_avalon_mutex_unlock(common_mutex);
		timer_stop = IORD_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterValue);
		printf("Increment time: %ld cycles.\n", (timer_stop - timer_start));

	}
	// Make sure the LED is turned off when it's all done
	altera_avalon_mutex_lock(common_mutex, 1);
	IOWR_8DIRECT(PARALLEL_PORT_2_BASE, 0, 0);
	altera_avalon_mutex_unlock(common_mutex);
	printf("Finished with incrementation synchronized by Mutex.\n\n");
}

void hwcounter()
{
	// Initialize counter and timer variables
	uint8_t LED_Counter = 0x0f;
	uint32_t timer_start = 0x0;
	uint32_t timer_stop = 0x0;
	// Setup start value to LEDs
	IOWR_8DIRECT(PARALLEL_PORT_2_BASE, 0, LED_Counter);
	// Setup custom counter 2, which can be accessed by both CPUs
	IOWR_32DIRECT(CUSTOM_COUNTER_2_BASE, CustomCounterReset, ArbVal);
	IOWR_32DIRECT(CUSTOM_COUNTER_2_BASE, CustomCounterCommand, 0x0);
	IOWR_32DIRECT(CUSTOM_COUNTER_2_BASE, CustomCounterTarget, 0xffffffff);
	IOWR_32DIRECT(CUSTOM_COUNTER_2_BASE, CustomCounterIncr, LED_Counter);
	// Setup and start custom counter 0, used for measuring how long access takes
	IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterReset, ArbVal);
	IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterCommand, 0x0);
	IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterTarget, 0xffffffff);
	IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterStart, ArbVal);
	printf("Starting incrementation of shared hardware counter.\n");
	while(LED_Counter > 0x0)
	{
		// Increment every 20 ms
		usleep(20000);
		// Read start value
		timer_start = IORD_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterValue);

		// Increment counter 2
		IOWR_32DIRECT(CUSTOM_COUNTER_2_BASE, CustomCounterIncr, 0x01);
		// Read counter 2 value
		LED_Counter = (uint8_t)(IORD_32DIRECT(CUSTOM_COUNTER_2_BASE, 0) & 0xFF);


		// Display new value on LED
		IOWR_8DIRECT(PARALLEL_PORT_2_BASE, 0, LED_Counter);

		timer_stop = IORD_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterValue);
		printf("Increment time: %ld cycles.\n", (timer_stop - timer_start));
		if (LED_Counter == 0x01)
		{
			break;
		}

	}
	// Make sure the LED is turned off when it's all done
	IOWR_8DIRECT(PARALLEL_PORT_2_BASE, 0, 0);
	printf("Finished with incrementation of shared hardware counter.\n\n");
}

void send_callback(void* report, int status)
{
	if(!status)
	{
		printf("Sending completed.\n");
	}
	else
	{
		printf("Sending error.\n");
	}
}

void send_mail()
{
	printf("Use the mailbox, sender side.\n");
	// Load mailbox
	altera_avalon_mailbox_dev* mailbox = altera_avalon_mailbox_open("/dev/mailbox_simple_0", send_callback, NULL);
	// Create mail "envelope"
	alt_u32 mail[2] = {0x0, 0x0};
	// Create pointer and message
	char *msg_ptr = SDRAM_CONTROLLER_2_BASE;
	char message[MSG_MAX];
	// Ask user for message
	printf("What message would you like to send? Enter a %i characters long message.\n", MSG_MAX);
	fgets(message, sizeof(message), stdin);


	printf("The message being sent is: ");
	for(int i = 0; i < MSG_MAX; i++)
	{
		if(message[i] == '/')
		{
			break;
		}
		*(msg_ptr + i) = message[i];
		mail[0] = i + 1;
		printf("%c", message[i]);
	}
	printf("\n");
	mail[1] = (alt_u32) msg_ptr;
	alt_dcache_flush_all();
	// Send message
	altera_avalon_mailbox_send(mailbox, mail, 0, POLL);
	// Close mailbox
	altera_avalon_mailbox_close(mailbox);
	printf("Done using the mailbox, sender side.\n\n");
}

void choose_task(int task)
{
	if(task == 1)
	{
		// 3.1 Parallel port test, 100 iterations, 50 ms each
		uint32_t iters = 0x64;
		int delaytime = 50;
		pptest(delaytime, iters);
	}
	else if(task == 2)
	{
		// Manipulation 2: hardware mutex
		hwmutex(0x0f);
	}
	else if(task == 4)
	{
		// Manipulation 3: hardware mailbox
		send_mail();
	}
	else if(task == 8)
	{
		// Manipulation 4: hardware counter
		hwcounter();
	}
}

void isr_buttons(void* context)
{
	uint8_t pinvals = IORD_8DIRECT(PIO_2_BASE, PIO_IRQFLAG);
	choice = (int)pinvals;
	// Clear the interrupt flag
	IOWR_8DIRECT(PIO_2_BASE, PIO_IRQFLAG, pinvals);
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
 *--------------------------------- THIS IS CPU 0 ---------------------------------------------
 */