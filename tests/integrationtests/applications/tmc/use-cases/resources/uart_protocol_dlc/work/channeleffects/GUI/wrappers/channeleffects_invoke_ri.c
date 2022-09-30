// Implementation of the glue code in C handling required interfaces

#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"
#include "dataview-uniq.h"

extern unsigned channeleffects_initialized;

void channeleffects_RI_CorruptChar
      (const asn1SccUINT8 *IN_p1);
void channeleffects_RI_CorruptChar
      (const asn1SccUINT8 *IN_p1)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to ErroredChannel (corresponding PI: CorruptChar)
      printf ("INNER_RI: channeleffects,erroredchannel,corruptchar,corruptchar,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_channeleffects_corruptchar
     (void *, size_t);

   vm_channeleffects_corruptchar
     ((void *)IN_p1, sizeof(asn1SccUINT8));


}



void channeleffects_RI_DropChar(void);
void channeleffects_RI_DropChar(void)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to ErroredChannel (corresponding PI: DropChar)
      printf ("INNER_RI: channeleffects,erroredchannel,dropchar,dropchar,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_channeleffects_dropchar(void);
   vm_channeleffects_dropchar();

}

