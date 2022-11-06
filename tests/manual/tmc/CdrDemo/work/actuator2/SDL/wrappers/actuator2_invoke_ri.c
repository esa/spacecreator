// Implementation of the glue code in C handling required interfaces

#include "dataview-uniq.h" // Always required for the definition of the PID type
#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"

extern unsigned actuator2_initialized;

void actuator2_RI_actuatorTm_To_PID(asn1SccPID dest_pid, 
      const asn1SccActuatorStatus *IN_tm
);
void actuator2_RI_actuatorTm(
      const asn1SccActuatorStatus *IN_tm
);
void actuator2_RI_actuatorTm(
      const asn1SccActuatorStatus *IN_tm
)
{
   // When no destination is specified, send to everyone (multicast)
   actuator2_RI_actuatorTm_To_PID(PID_env, IN_tm
);
}

void actuator2_RI_actuatorTm_To_PID(asn1SccPID dest_pid, 
      const asn1SccActuatorStatus *IN_tm
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to ModeManager (corresponding PI: actuatorTm2)
      printf ("INNER_RI: actuator2,modemanager,actuatortm,actuatortm2,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_actuator2_actuatortm
     (asn1SccPID,
      void *, size_t);

   vm_actuator2_actuatortm
     (dest_pid,
      (void *)IN_tm, sizeof(asn1SccActuatorStatus));


}

// Get the PID of the sender function. The actual function is defined in _vm_if.c
// as the sender PID is received together with incoming PI calls
void actuator2_RI_get_sender(asn1SccPID *sender_pid)
{
  extern void actuator2_get_sender(asn1SccPID *sender_pid);
  actuator2_get_sender(sender_pid);
}
