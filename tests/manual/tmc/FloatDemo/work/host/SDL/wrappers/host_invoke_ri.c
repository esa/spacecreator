// Implementation of the glue code in C handling required interfaces

#include "dataview-uniq.h" // Always required for the definition of the PID type
#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"

extern unsigned host_initialized;

void host_RI_configuration_To_PID(asn1SccPID dest_pid, 
      const asn1SccPidConfig *IN_cfg
);
void host_RI_configuration(
      const asn1SccPidConfig *IN_cfg
);
void host_RI_configuration(
      const asn1SccPidConfig *IN_cfg
)
{
   // When no destination is specified, send to everyone (multicast)
   host_RI_configuration_To_PID(PID_env, IN_cfg
);
}

void host_RI_configuration_To_PID(asn1SccPID dest_pid, 
      const asn1SccPidConfig *IN_cfg
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to ControlSystem (corresponding PI: configuration)
      printf ("INNER_RI: host,controlsystem,configuration,configuration,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_host_configuration
     (asn1SccPID,
      void *, size_t);

   vm_host_configuration
     (dest_pid,
      (void *)IN_cfg, sizeof(asn1SccPidConfig));


}



void host_RI_tc_To_PID(asn1SccPID dest_pid);
void host_RI_tc(void);
void host_RI_tc(void)
{
   // When no destination is specified, send to everyone (multicast)
   host_RI_tc_To_PID(PID_env);
}

void host_RI_tc_To_PID(asn1SccPID dest_pid)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to Sensor (corresponding PI: tc)
      printf ("INNER_RI: host,sensor,tc,tc,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_host_tc(asn1SccPID);
   vm_host_tc(dest_pid);

}

// Get the PID of the sender function. The actual function is defined in _vm_if.c
// as the sender PID is received together with incoming PI calls
void host_RI_get_sender(asn1SccPID *sender_pid)
{
  extern void host_get_sender(asn1SccPID *sender_pid);
  host_get_sender(sender_pid);
}
