// Implementation of the glue code in C handling required interfaces

#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"
#include "dataview-uniq.h"

extern unsigned controller_initialized;

void controller_RI_Calculate
      (const asn1SccMyInteger *IN_intparam,
       const asn1SccMyReal    *IN_realparam,
       const asn1SccMyBool    *IN_boolparam,
       asn1SccMyReal          *OUT_outparam);
void controller_RI_Calculate
      (const asn1SccMyInteger *IN_intparam,
       const asn1SccMyReal    *IN_realparam,
       const asn1SccMyBool    *IN_boolparam,
       asn1SccMyReal          *OUT_outparam)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to QGenHost (corresponding PI: Calculate)
      printf ("INNER_RI: controller,qgenhost,calculate,calculate,%lld\n", msc_time);
      fflush(stdout);
   }

   size_t      size_OUT_buf_outparam = 0;

   // Send the message via the middleware API
   extern void vm_controller_calculate
     (void *, size_t,
      void *, size_t,
      void *, size_t,
      void *, size_t *);

   vm_controller_calculate
     ((void *)IN_intparam, sizeof(asn1SccMyInteger),
      (void *)IN_realparam, sizeof(asn1SccMyReal),
      (void *)IN_boolparam, sizeof(asn1SccMyBool),
      (void *)OUT_outparam, &size_OUT_buf_outparam);


}

