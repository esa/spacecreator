// Implementation of the glue code in C handling required interfaces

#include "dataview-uniq.h" // Always required for the definition of the PID type
#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"

extern unsigned actuator_initialized;

void actuator_RI_reporting_To_PID(asn1SccPID dest_pid);
void actuator_RI_reporting(void);
void actuator_RI_reporting(void)
{
   // When no destination is specified, send to everyone (multicast)
   actuator_RI_reporting_To_PID(PID_env);
}

void actuator_RI_reporting_To_PID(asn1SccPID dest_pid)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to SystemControl (corresponding PI: reporting)
      printf ("INNER_RI: actuator,systemcontrol,reporting,reporting,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_actuator_reporting(asn1SccPID);
   vm_actuator_reporting(dest_pid);

}

// Get the PID of the sender function. The actual function is defined in _vm_if.c
// as the sender PID is received together with incoming PI calls
void actuator_RI_get_sender(asn1SccPID *sender_pid)
{
  extern void actuator_get_sender(asn1SccPID *sender_pid);
  actuator_get_sender(sender_pid);
}
