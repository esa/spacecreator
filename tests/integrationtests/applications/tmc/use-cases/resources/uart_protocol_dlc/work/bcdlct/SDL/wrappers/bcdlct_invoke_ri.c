// Implementation of the glue code in C handling required interfaces

#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"
#include "dataview-uniq.h"

extern unsigned bcdlct_initialized;

void bcdlct_RI_PHYTxChar
      (const asn1SccUINT8 *IN_p1);
void bcdlct_RI_PHYTxChar
      (const asn1SccUINT8 *IN_p1)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to ErroredChannel (corresponding PI: PHYTxChar)
      printf ("INNER_RI: bcdlct,erroredchannel,phytxchar,phytxchar,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_bcdlct_phytxchar
     (void *, size_t);

   vm_bcdlct_phytxchar
     ((void *)IN_p1, sizeof(asn1SccUINT8));


}

