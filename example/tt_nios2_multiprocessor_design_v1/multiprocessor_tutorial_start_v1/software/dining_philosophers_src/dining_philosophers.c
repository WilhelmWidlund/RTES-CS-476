/*
 * Dining Philosophers for Multiprocessor Tutorial
 */

#include <stdio.h>

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

   alt_mutex_dev *left_chopstick, *right_chopstick;
   int philosopher_id;

   printf("Hello from CPU %d.\n", ALT_CPU_CPU_ID_VALUE);
   philosopher_id = ALT_CPU_CPU_ID_VALUE;

#ifdef PHILOSOPHER_ZERO_CHOPSTICK_MUTEX_BASE 
#ifdef PHILOSOPHER_ONE_CHOPSTICK_MUTEX_BASE 
   left_chopstick = altera_avalon_mutex_open( "/dev/philosopher_zero_chopstick_mutex");
   right_chopstick = altera_avalon_mutex_open( "/dev/philosopher_one_chopstick_mutex");
#endif
#endif

#ifdef PHILOSOPHER_ONE_CHOPSTICK_MUTEX_BASE 
#ifdef PHILOSOPHER_TWO_CHOPSTICK_MUTEX_BASE 
   left_chopstick = altera_avalon_mutex_open( "/dev/philosopher_one_chopstick_mutex");
   right_chopstick = altera_avalon_mutex_open( "/dev/philosopher_two_chopstick_mutex");
#endif
#endif

#ifdef PHILOSOPHER_TWO_CHOPSTICK_MUTEX_BASE 
#ifdef PHILOSOPHER_THREE_CHOPSTICK_MUTEX_BASE 
   left_chopstick = altera_avalon_mutex_open( "/dev/philosopher_two_chopstick_mutex");
   right_chopstick = altera_avalon_mutex_open( "/dev/philosopher_three_chopstick_mutex");
#endif
#endif

#ifdef PHILOSOPHER_THREE_CHOPSTICK_MUTEX_BASE 
#ifdef PHILOSOPHER_FOUR_CHOPSTICK_MUTEX_BASE 
   left_chopstick = altera_avalon_mutex_open( "/dev/philosopher_three_chopstick_mutex");
   right_chopstick = altera_avalon_mutex_open( "/dev/philosopher_four_chopstick_mutex");
#endif
#endif

#ifdef PHILOSOPHER_FOUR_CHOPSTICK_MUTEX_BASE 
#ifdef PHILOSOPHER_ZERO_CHOPSTICK_MUTEX_BASE 
   left_chopstick = altera_avalon_mutex_open( "/dev/philosopher_four_chopstick_mutex");
   right_chopstick = altera_avalon_mutex_open( "/dev/philosopher_zero_chopstick_mutex");
#endif
#endif

   while(1)
   {
      if (!altera_avalon_mutex_trylock (left_chopstick, philosopher_id))
      {
         if (!altera_avalon_mutex_trylock (right_chopstick, philosopher_id))
         {
            printf ("Philosopher %d is eating.\n", philosopher_id);
            altera_avalon_mutex_unlock (right_chopstick);
         }
         else
         {
            printf ("Philosopher %d could not pick up the right_chopstick.\n", philosopher_id);
         }
         altera_avalon_mutex_unlock (left_chopstick);
      }
      else
      { 
         printf ("Philosopher %d could not pick up the left_chopstick.\n", philosopher_id);
      }
            printf ("Philosopher %d is thinking.\n", philosopher_id);

   }
   return(0);
}

