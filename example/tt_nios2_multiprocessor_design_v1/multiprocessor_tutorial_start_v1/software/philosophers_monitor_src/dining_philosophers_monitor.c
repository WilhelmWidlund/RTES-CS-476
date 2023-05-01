/*
 * Dining Philosophers monitor for Multiprocessor Tutorial
 */

#include "alt_types.h"
#include <stdio.h>
#include <sys/alt_stdio.h>

/* 
 * Qsys hardware definitions
 */
#include "system.h"

/*
 * Mutex API
 */
#include "altera_avalon_mutex.h"

int main()
{

   alt_mutex_dev *chopstick_mutex[5];
   long chopsticks_held[5];

   int philosopher_id;
   int selected_chopstick_mutex_index;


   printf("Hello from top CPU %d.\n", ALT_CPU_CPU_ID_VALUE);
   philosopher_id = ALT_CPU_CPU_ID_VALUE;

#ifdef PHILOSOPHER_ZERO_CHOPSTICK_MUTEX_BASE 
   chopstick_mutex[0] = altera_avalon_mutex_open("/dev/philosopher_zero_chopstick_mutex");
   chopsticks_held[0] = 0;
#endif

#ifdef PHILOSOPHER_ONE_CHOPSTICK_MUTEX_BASE 
   chopstick_mutex[1]  = altera_avalon_mutex_open("/dev/philosopher_one_chopstick_mutex");
   chopsticks_held[1] = 0;
#endif

#ifdef PHILOSOPHER_TWO_CHOPSTICK_MUTEX_BASE 
   chopstick_mutex[2]  = altera_avalon_mutex_open("/dev/philosopher_two_chopstick_mutex");
   chopsticks_held[2] = 0;
#endif

#ifdef PHILOSOPHER_THREE_CHOPSTICK_MUTEX_BASE 
   chopstick_mutex[3] = altera_avalon_mutex_open("/dev/philosopher_three_chopstick_mutex");
   chopsticks_held[3] = 0;
#endif

#ifdef PHILOSOPHER_FOUR_CHOPSTICK_MUTEX_BASE 
   chopstick_mutex[4]  = altera_avalon_mutex_open("/dev/philosopher_four_chopstick_mutex");
   chopsticks_held[4] = 0;
#endif

   printf ("To alternately grab or drop a chopstick, enter a chopstick id (0 - 4): \n");

   while(1)
   {
	  /* Wait for chopstick selection input on jtag_uart_top. */
      selected_chopstick_mutex_index = alt_getchar(stdin);

      /* Echo input character. */
      alt_putchar(selected_chopstick_mutex_index);

      /* Convert ASCII character to equivalent digit. */
      selected_chopstick_mutex_index -= '0';

      /* Check that a valid chopstick number selected. */
      if ((selected_chopstick_mutex_index < 0) || (selected_chopstick_mutex_index > 4))
      {
          printf ("\nPlease select a valid chopstick id (0 - 4).\n");
      }
      else
      {
         printf("\nChopstick %d selected.\n", selected_chopstick_mutex_index);

         /* Try to grab, or alternately release, the selected chopstick mutex */
         if (chopsticks_held[selected_chopstick_mutex_index])
         {
            altera_avalon_mutex_unlock (chopstick_mutex[selected_chopstick_mutex_index]);
            printf("Chopstick %d dropped.\n", selected_chopstick_mutex_index);
            chopsticks_held[selected_chopstick_mutex_index] = 0;
         }

         else
         {
            printf("Attempting to grab chopstick %d.\n", selected_chopstick_mutex_index);
            while (altera_avalon_mutex_trylock(chopstick_mutex[selected_chopstick_mutex_index], philosopher_id))
            {
            	/* Loop until selected chopstick mutex is successfully locked */;
            }
            printf("Chopstick %d grabbed.\n", selected_chopstick_mutex_index);
            chopsticks_held[selected_chopstick_mutex_index] = 1;
         }
      }
   }

   return(0);
}

