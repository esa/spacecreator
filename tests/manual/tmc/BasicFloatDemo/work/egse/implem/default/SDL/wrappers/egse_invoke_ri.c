// Implementation of the glue code in C handling required interfaces

#include "dataview-uniq.h" // Always required for the definition of the PID type
#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"

extern unsigned egse_initialized;

void egse_RI_tc_To_PID(asn1SccPID dest_pid, 
      const asn1SccT_Int *IN_x
);
void egse_RI_tc(
      const asn1SccT_Int *IN_x
);
void egse_RI_tc(
      const asn1SccT_Int *IN_x
)
{
   // When no destination is specified, send to everyone (multicast)
   egse_RI_tc_To_PID(PID_env, IN_x
);
}

void egse_RI_tc_To_PID(asn1SccPID dest_pid, 
      const asn1SccT_Int *IN_x
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to SystemControl (corresponding PI: tc)
      printf ("INNER_RI: egse,systemcontrol,tc,tc,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_egse_tc
     (asn1SccPID,
      void *, size_t);

   vm_egse_tc
     (dest_pid,
      (void *)IN_x, sizeof(asn1SccT_Int));


}

// Get the PID of the sender function. The actual function is defined in _vm_if.c
// as the sender PID is received together with incoming PI calls
void egse_RI_get_sender(asn1SccPID *sender_pid)
{
  extern void egse_get_sender(asn1SccPID *sender_pid);
  egse_get_sender(sender_pid);
}
