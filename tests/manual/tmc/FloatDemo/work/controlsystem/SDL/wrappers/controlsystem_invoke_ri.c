// Implementation of the glue code in C handling required interfaces

#include "dataview-uniq.h" // Always required for the definition of the PID type
#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"

extern unsigned controlsystem_initialized;

void controlsystem_RI_controlSignal_To_PID(asn1SccPID dest_pid, 
      const asn1SccNumber *IN_controlvalue
);
void controlsystem_RI_controlSignal(
      const asn1SccNumber *IN_controlvalue
);
void controlsystem_RI_controlSignal(
      const asn1SccNumber *IN_controlvalue
)
{
   // When no destination is specified, send to everyone (multicast)
   controlsystem_RI_controlSignal_To_PID(PID_env, IN_controlvalue
);
}

void controlsystem_RI_controlSignal_To_PID(asn1SccPID dest_pid, 
      const asn1SccNumber *IN_controlvalue
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to Actuator (corresponding PI: controlSignal)
      printf ("INNER_RI: controlsystem,actuator,controlsignal,controlsignal,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_controlsystem_controlsignal
     (asn1SccPID,
      void *, size_t);

   vm_controlsystem_controlsignal
     (dest_pid,
      (void *)IN_controlvalue, sizeof(asn1SccNumber));


}



void controlsystem_RI_tm_To_PID(asn1SccPID dest_pid);
void controlsystem_RI_tm(void);
void controlsystem_RI_tm(void)
{
   // When no destination is specified, send to everyone (multicast)
   controlsystem_RI_tm_To_PID(PID_env);
}

void controlsystem_RI_tm_To_PID(asn1SccPID dest_pid)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to Host (corresponding PI: tm)
      printf ("INNER_RI: controlsystem,host,tm,tm,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_controlsystem_tm(asn1SccPID);
   vm_controlsystem_tm(dest_pid);

}

// Get the PID of the sender function. The actual function is defined in _vm_if.c
// as the sender PID is received together with incoming PI calls
void controlsystem_RI_get_sender(asn1SccPID *sender_pid)
{
  extern void controlsystem_get_sender(asn1SccPID *sender_pid);
  controlsystem_get_sender(sender_pid);
}
