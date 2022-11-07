// Implementation of the glue code in C handling required interfaces

#include "dataview-uniq.h" // Always required for the definition of the PID type
#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"

extern unsigned systemcontrol_initialized;

void systemcontrol_RI_controlsignal_To_PID(asn1SccPID dest_pid, 
      const asn1SccT_Float *IN_x
);
void systemcontrol_RI_controlsignal(
      const asn1SccT_Float *IN_x
);
void systemcontrol_RI_controlsignal(
      const asn1SccT_Float *IN_x
)
{
   // When no destination is specified, send to everyone (multicast)
   systemcontrol_RI_controlsignal_To_PID(PID_env, IN_x
);
}

void systemcontrol_RI_controlsignal_To_PID(asn1SccPID dest_pid, 
      const asn1SccT_Float *IN_x
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to Actuator (corresponding PI: controlsignal)
      printf ("INNER_RI: systemcontrol,actuator,controlsignal,controlsignal,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_systemcontrol_controlsignal
     (asn1SccPID,
      void *, size_t);

   vm_systemcontrol_controlsignal
     (dest_pid,
      (void *)IN_x, sizeof(asn1SccT_Float));


}



void systemcontrol_RI_tm_To_PID(asn1SccPID dest_pid, 
      const asn1SccT_Int *IN_y
);
void systemcontrol_RI_tm(
      const asn1SccT_Int *IN_y
);
void systemcontrol_RI_tm(
      const asn1SccT_Int *IN_y
)
{
   // When no destination is specified, send to everyone (multicast)
   systemcontrol_RI_tm_To_PID(PID_env, IN_y
);
}

void systemcontrol_RI_tm_To_PID(asn1SccPID dest_pid, 
      const asn1SccT_Int *IN_y
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to EGSE (corresponding PI: tm)
      printf ("INNER_RI: systemcontrol,egse,tm,tm,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_systemcontrol_tm
     (asn1SccPID,
      void *, size_t);

   vm_systemcontrol_tm
     (dest_pid,
      (void *)IN_y, sizeof(asn1SccT_Int));


}

// Get the PID of the sender function. The actual function is defined in _vm_if.c
// as the sender PID is received together with incoming PI calls
void systemcontrol_RI_get_sender(asn1SccPID *sender_pid)
{
  extern void systemcontrol_get_sender(asn1SccPID *sender_pid);
  systemcontrol_get_sender(sender_pid);
}
