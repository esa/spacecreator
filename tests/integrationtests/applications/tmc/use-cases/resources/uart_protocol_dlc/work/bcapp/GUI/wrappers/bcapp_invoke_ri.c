// Implementation of the glue code in C handling required interfaces

#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"
#include "dataview-uniq.h"

extern unsigned bcapp_initialized;

void bcapp_RI_DLCTxReset(void);
void bcapp_RI_DLCTxReset(void)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to BCDLCT (corresponding PI: DLCTxReset)
      printf ("INNER_RI: bcapp,bcdlct,dlctxreset,dlctxreset,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_bcapp_dlctxreset(void);
   vm_bcapp_dlctxreset();

}



void bcapp_RI_DLCTxSDU
      (const asn1SccT_UART_DLC_SDU *IN_p1);
void bcapp_RI_DLCTxSDU
      (const asn1SccT_UART_DLC_SDU *IN_p1)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to BCDLCT (corresponding PI: DLCTxSDU)
      printf ("INNER_RI: bcapp,bcdlct,dlctxsdu,dlctxsdu,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_bcapp_dlctxsdu
     (void *, size_t);

   vm_bcapp_dlctxsdu
     ((void *)IN_p1, sizeof(asn1SccT_UART_DLC_SDU));


}

