/*
 *--------------------------------- THIS IS CPU 0 ---------------------------------------------
 */

#include <stdio.h>
#include "system.h"
#include "io.h"
#include <stdint.h>
#include <unistd.h>
#include <altera_avalon_mutex.h>
#include "altera_avalon_mailbox_simple.h"
#include "altera_avalon_mailbox_simple_regs.h"

/*
 *--------------------------------- THIS IS CPU 0 ---------------------------------------------
 */

#define ArbVal 0xffffffff

// Custom counter address offsets
#define CustomCounterValue 0
#define CustomCounterReset 4
#define CustomCounterStart	8
#define CustomCounterStop	12
#define CustomCounterCommand	16
#define CustomCounterStatus	20
#define CustomCounterTarget 24
#define CustomCounterIncrVal 28

void part1()
{
	printf("Hello from the multicore system, this is CPU %d talking! \n", ALT_CPU_CPU_ID_VALUE);
}

/*
 *--------------------------------- THIS IS CPU 0 ---------------------------------------------
 */

// Access parallel port 0 connected to LEDs 3, 2 & 1
// and increment counter 0 every mscound milliseconds
// for iterations amount of times.
void pptest(int msdelay, uint32_t iterations)
{
	uint32_t itercount = 0x00;
	uint8_t curr_lamp = 0x00;
	// Reset custom counter
	IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterReset, ArbVal);
	// Set target
	IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterTarget, iterations);

	while(itercount < iterations)
	{
		// Access parallel port to toggle LED
		IOWR_8DIRECT(PARALLEL_PORT_0_BASE, 0, curr_lamp);
		// Increment counter
		IOWR_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterIncrVal, 0x01);
		// Make sure to toggle LEDs correctly for 3 available
		if(curr_lamp < 0x07)
		{
			curr_lamp = curr_lamp + 0x01;
		}
		else
		{
			curr_lamp = 0x00;
		}
		// Wait msdelay ms
		usleep(1000 * msdelay);
		// Get count value
		itercount = IORD_32DIRECT(CUSTOM_COUNTER_0_BASE, CustomCounterValue);
	}
}

void increment(uint32_t address, int incval)
{

}


int main()
{
	// Manipulation 1
	part1();
	// 3.1 Parallel port test, 100 iterations, 50 ms each
	uint32_t iters = 0x64;
	int delaytime = 50;
	pptest(delaytime, iters);

  return 0;
}

/*
 *--------------------------------- THIS IS CPU 0 ---------------------------------------------
 */
