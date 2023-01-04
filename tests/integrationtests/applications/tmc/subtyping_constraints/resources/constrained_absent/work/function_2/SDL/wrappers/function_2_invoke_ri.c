// Implementation of the glue code in C handling required interfaces

#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"
#include "dataview-uniq.h"

extern unsigned function_2_initialized;

void function_2_RI_PI_2
      (const asn1SccMyChoice *IN_p1);
void function_2_RI_PI_2
      (const asn1SccMyChoice *IN_p1)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to Function_1 (corresponding PI: PI_2)
      printf ("INNER_RI: function_2,function_1,pi_2,pi_2,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_function_2_pi_2
     (void *, size_t);

   vm_function_2_pi_2
     ((void *)IN_p1, sizeof(asn1SccMyChoice));


}

