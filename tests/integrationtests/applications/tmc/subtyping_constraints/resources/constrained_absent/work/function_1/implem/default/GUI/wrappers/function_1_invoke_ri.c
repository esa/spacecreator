// Implementation of the glue code in C handling required interfaces

#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"
#include "dataview-uniq.h"

extern unsigned function_1_initialized;

void function_1_RI_PI_1
      (const asn1SccMySeqOf *IN_p1);
void function_1_RI_PI_1
      (const asn1SccMySeqOf *IN_p1)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to Function_2 (corresponding PI: PI_1)
      printf ("INNER_RI: function_1,function_2,pi_1,pi_1,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_function_1_pi_1
     (void *, size_t);

   vm_function_1_pi_1
     ((void *)IN_p1, sizeof(asn1SccMySeqOf));


}

