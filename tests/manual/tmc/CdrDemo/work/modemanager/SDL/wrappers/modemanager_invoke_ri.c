// Implementation of the glue code in C handling required interfaces

#include "dataview-uniq.h" // Always required for the definition of the PID type
#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"

extern unsigned modemanager_initialized;

void modemanager_RI_actuatorTc1_To_PID(asn1SccPID dest_pid, 
      const asn1SccActuatorCommand *IN_tc
);
void modemanager_RI_actuatorTc1(
      const asn1SccActuatorCommand *IN_tc
);
void modemanager_RI_actuatorTc1(
      const asn1SccActuatorCommand *IN_tc
)
{
   // When no destination is specified, send to everyone (multicast)
   modemanager_RI_actuatorTc1_To_PID(PID_env, IN_tc
);
}

void modemanager_RI_actuatorTc1_To_PID(asn1SccPID dest_pid, 
      const asn1SccActuatorCommand *IN_tc
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to Actuator1 (corresponding PI: actuatorTc)
      printf ("INNER_RI: modemanager,actuator1,actuatortc1,actuatortc,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_modemanager_actuatortc1
     (asn1SccPID,
      void *, size_t);

   vm_modemanager_actuatortc1
     (dest_pid,
      (void *)IN_tc, sizeof(asn1SccActuatorCommand));


}



void modemanager_RI_actuatorTc2_To_PID(asn1SccPID dest_pid, 
      const asn1SccActuatorCommand *IN_tc
);
void modemanager_RI_actuatorTc2(
      const asn1SccActuatorCommand *IN_tc
);
void modemanager_RI_actuatorTc2(
      const asn1SccActuatorCommand *IN_tc
)
{
   // When no destination is specified, send to everyone (multicast)
   modemanager_RI_actuatorTc2_To_PID(PID_env, IN_tc
);
}

void modemanager_RI_actuatorTc2_To_PID(asn1SccPID dest_pid, 
      const asn1SccActuatorCommand *IN_tc
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to Actuator2 (corresponding PI: actuatorTc)
      printf ("INNER_RI: modemanager,actuator2,actuatortc2,actuatortc,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_modemanager_actuatortc2
     (asn1SccPID,
      void *, size_t);

   vm_modemanager_actuatortc2
     (dest_pid,
      (void *)IN_tc, sizeof(asn1SccActuatorCommand));


}



void modemanager_RI_systemTm_To_PID(asn1SccPID dest_pid, 
      const asn1SccSystemTelemetry *IN_tm
);
void modemanager_RI_systemTm(
      const asn1SccSystemTelemetry *IN_tm
);
void modemanager_RI_systemTm(
      const asn1SccSystemTelemetry *IN_tm
)
{
   // When no destination is specified, send to everyone (multicast)
   modemanager_RI_systemTm_To_PID(PID_env, IN_tm
);
}

void modemanager_RI_systemTm_To_PID(asn1SccPID dest_pid, 
      const asn1SccSystemTelemetry *IN_tm
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to EGSE (corresponding PI: systemTm)
      printf ("INNER_RI: modemanager,egse,systemtm,systemtm,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_modemanager_systemtm
     (asn1SccPID,
      void *, size_t);

   vm_modemanager_systemtm
     (dest_pid,
      (void *)IN_tm, sizeof(asn1SccSystemTelemetry));


}

// Get the PID of the sender function. The actual function is defined in _vm_if.c
// as the sender PID is received together with incoming PI calls
void modemanager_RI_get_sender(asn1SccPID *sender_pid)
{
  extern void modemanager_get_sender(asn1SccPID *sender_pid);
  modemanager_get_sender(sender_pid);
}
