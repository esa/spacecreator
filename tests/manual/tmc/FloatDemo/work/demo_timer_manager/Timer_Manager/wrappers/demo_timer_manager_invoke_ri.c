// Implementation of the glue code in C handling required interfaces

#include "dataview-uniq.h" // Always required for the definition of the PID type
#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"

extern unsigned demo_timer_manager_initialized;

void demo_timer_manager_RI_sensor_trigger_To_PID(asn1SccPID dest_pid);
void demo_timer_manager_RI_sensor_trigger(void);
void demo_timer_manager_RI_sensor_trigger(void)
{
   // When no destination is specified, send to everyone (multicast)
   demo_timer_manager_RI_sensor_trigger_To_PID(PID_env);
}

void demo_timer_manager_RI_sensor_trigger_To_PID(asn1SccPID dest_pid)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to sensor (corresponding PI: trigger)
      printf ("INNER_RI: demo_timer_manager,sensor,sensor_trigger,trigger,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_demo_timer_manager_sensor_trigger(asn1SccPID);
   vm_demo_timer_manager_sensor_trigger(dest_pid);

}

// Get the PID of the sender function. The actual function is defined in _vm_if.c
// as the sender PID is received together with incoming PI calls
void demo_timer_manager_RI_get_sender(asn1SccPID *sender_pid)
{
  extern void demo_timer_manager_get_sender(asn1SccPID *sender_pid);
  demo_timer_manager_get_sender(sender_pid);
}
