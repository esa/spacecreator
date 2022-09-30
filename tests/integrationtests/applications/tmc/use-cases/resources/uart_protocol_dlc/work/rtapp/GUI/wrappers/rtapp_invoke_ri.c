// Implementation of the glue code in C handling required interfaces

#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "dataview-uniq.h"

extern unsigned rtapp_initialized;

void rtapp_RI_DLCRxReset(void);
void rtapp_RI_DLCRxReset(void)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to RTDLCR (corresponding PI: DLCRxReset)
      printf ("INNER_RI: rtapp,rtdlcr,dlcrxreset,dlcrxreset,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_rtapp_dlcrxreset(void);
   vm_rtapp_dlcrxreset();

}

