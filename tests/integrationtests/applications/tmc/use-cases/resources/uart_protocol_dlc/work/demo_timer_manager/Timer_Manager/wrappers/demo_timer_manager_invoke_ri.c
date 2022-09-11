// Implementation of the glue code in C handling required interfaces

#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "dataview-uniq.h"

extern unsigned demo_timer_manager_initialized;

void demo_timer_manager_RI_rtdlcr_FrameTimer(void);
void demo_timer_manager_RI_rtdlcr_FrameTimer(void)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to rtdlcr (corresponding PI: FrameTimer)
      printf ("INNER_RI: demo_timer_manager,rtdlcr,rtdlcr_frametimer,frametimer,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_demo_timer_manager_rtdlcr_frametimer(void);
   vm_demo_timer_manager_rtdlcr_frametimer();

}

