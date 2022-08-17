// Implementation of the glue code in C handling required interfaces

#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"
#include "dataview-uniq.h"

extern unsigned controller_initialized;

void controller_RI_Iface
      (const asn1SccMyIA5String *IN_input,
       asn1SccMyIA5String       *OUT_output);
void controller_RI_Iface
      (const asn1SccMyIA5String *IN_input,
       asn1SccMyIA5String       *OUT_output)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to QGenC (corresponding PI: Iface)
      printf ("INNER_RI: controller,qgenc,iface,iface,%lld\n", msc_time);
      fflush(stdout);
   }

   size_t      size_OUT_buf_output = 0;

   // Send the message via the middleware API
   extern void vm_controller_iface
     (void *, size_t,
      void *, size_t *);

   vm_controller_iface
     ((void *)IN_input, sizeof(asn1SccMyIA5String),
      (void *)OUT_output, &size_OUT_buf_output);


}

