// Implementation of the glue code in C handling required interfaces

#include "dataview-uniq.h" // Always required for the definition of the PID type
#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"

extern unsigned sensor_initialized;

void sensor_RI_RESET_trigger_To_PID(asn1SccPID dest_pid);
void sensor_RI_RESET_trigger(void);
void sensor_RI_RESET_trigger(void)
{
   // When no destination is specified, send to everyone (multicast)
   sensor_RI_RESET_trigger_To_PID(PID_env);
}

void sensor_RI_RESET_trigger_To_PID(asn1SccPID dest_pid)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to demo_Timer_Manager (corresponding PI: RESET_sensor_trigger)
      printf ("INNER_RI: sensor,demo_timer_manager,reset_trigger,reset_sensor_trigger,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_sensor_reset_trigger(asn1SccPID);
   vm_sensor_reset_trigger(dest_pid);

}



void sensor_RI_SET_trigger_To_PID(asn1SccPID dest_pid, 
      const asn1SccT_UInt32 *IN_val
);
void sensor_RI_SET_trigger(
      const asn1SccT_UInt32 *IN_val
);
void sensor_RI_SET_trigger(
      const asn1SccT_UInt32 *IN_val
)
{
   // When no destination is specified, send to everyone (multicast)
   sensor_RI_SET_trigger_To_PID(PID_env, IN_val
);
}

void sensor_RI_SET_trigger_To_PID(asn1SccPID dest_pid, 
      const asn1SccT_UInt32 *IN_val
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      PrintASN1T_UInt32 ("INNERDATA: set_trigger::T_UInt32::val", IN_val);
      puts("");
      // Log message to demo_Timer_Manager (corresponding PI: SET_sensor_trigger)
      printf ("INNER_RI: sensor,demo_timer_manager,set_trigger,set_sensor_trigger,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_sensor_set_trigger
     (asn1SccPID,
      void *, size_t);

   vm_sensor_set_trigger
     (dest_pid,
      (void *)IN_val, sizeof(asn1SccT_UInt32));


}



void sensor_RI_reading_To_PID(asn1SccPID dest_pid, 
      const asn1SccNumber *IN_signalvalue
);
void sensor_RI_reading(
      const asn1SccNumber *IN_signalvalue
);
void sensor_RI_reading(
      const asn1SccNumber *IN_signalvalue
)
{
   // When no destination is specified, send to everyone (multicast)
   sensor_RI_reading_To_PID(PID_env, IN_signalvalue
);
}

void sensor_RI_reading_To_PID(asn1SccPID dest_pid, 
      const asn1SccNumber *IN_signalvalue
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to ControlSystem (corresponding PI: reading)
      printf ("INNER_RI: sensor,controlsystem,reading,reading,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_sensor_reading
     (asn1SccPID,
      void *, size_t);

   vm_sensor_reading
     (dest_pid,
      (void *)IN_signalvalue, sizeof(asn1SccNumber));


}

// Get the PID of the sender function. The actual function is defined in _vm_if.c
// as the sender PID is received together with incoming PI calls
void sensor_RI_get_sender(asn1SccPID *sender_pid)
{
  extern void sensor_get_sender(asn1SccPID *sender_pid);
  sensor_get_sender(sender_pid);
}
