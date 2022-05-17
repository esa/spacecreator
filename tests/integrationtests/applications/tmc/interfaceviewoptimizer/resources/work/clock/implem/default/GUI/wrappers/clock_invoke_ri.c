// Implementation of the glue code in C handling required interfaces

#include <stdlib.h>
#ifdef __unix__
   #include <stdio.h>
   #include "PrintTypesAsASN1.h"
   #include "timeInMS.h"
#endif
#include "dataview-uniq.h"

extern unsigned clock_initialized;

void clock_RI_ClockInterface

(void);
void clock_RI_ClockInterface

(void)
{
   #ifdef __unix__
      // Log MSC data on Linux when environment variable is set
      static int innerMsc = -1;
      if (-1 == innerMsc)
         innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
      if (1 == innerMsc) {
         long long msc_time = getTimeInMilliseconds();
         // Log message to Pinger (corresponding PI: ClockInterface)
         printf ("INNER_RI: clock,pinger,clockinterface,clockinterface,%lld\n", msc_time);
         fflush(stdout);
      }
   #endif


   // Send the message via the middleware API
   extern void vm_clock_clockinterface(void);
   vm_clock_clockinterface();

}

