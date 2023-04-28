/*
 *--------------------------------- THIS IS CPU 0 ---------------------------------------------
 */

#include <stdio.h>
#include "system.h"
#include <altera_avalon_mutex.h>
#include "altera_avalon_mailbox_simple.h"
#include "altera_avalon_mailbox_simple_regs.h"

/*
 *--------------------------------- THIS IS CPU 0 ---------------------------------------------
 */

void part1()
{
	printf("Hello from the multicore system, this is CPU %d talking! \n", ALT_CPU_CPU_ID_VALUE);
}

/*
 *--------------------------------- THIS IS CPU 0 ---------------------------------------------
 */

int main()
{
  part1();

  return 0;
}

/*
 *--------------------------------- THIS IS CPU 0 ---------------------------------------------
 */
