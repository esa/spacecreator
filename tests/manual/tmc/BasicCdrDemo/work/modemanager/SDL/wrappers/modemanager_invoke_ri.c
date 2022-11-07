// Implementation of the glue code in C handling required interfaces

#include "dataview-uniq.h" // Always required for the definition of the PID type
#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"

extern unsigned modemanager_initialized;

void modemanager_RI_ctrl_To_PID(asn1SccPID dest_pid, 
      const asn1SccT_Integer *IN_x
);
void modemanager_RI_ctrl(
      const asn1SccT_Integer *IN_x
);
void modemanager_RI_ctrl(
      const asn1SccT_Integer *IN_x
)
{
   // When no destination is specified, send to everyone (multicast)
   modemanager_RI_ctrl_To_PID(PID_env, IN_x
);
}

void modemanager_RI_ctrl_To_PID(asn1SccPID dest_pid, 
      const asn1SccT_Integer *IN_x
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to Actuator (corresponding PI: ctrl)
      printf ("INNER_RI: modemanager,actuator,ctrl,ctrl,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_modemanager_ctrl
     (asn1SccPID,
      void *, size_t);

   vm_modemanager_ctrl
     (dest_pid,
      (void *)IN_x, sizeof(asn1SccT_Integer));


}



void modemanager_RI_tm_To_PID(asn1SccPID dest_pid, 
      const asn1SccT_Report *IN_reprt
);
void modemanager_RI_tm(
      const asn1SccT_Report *IN_reprt
);
void modemanager_RI_tm(
      const asn1SccT_Report *IN_reprt
)
{
   // When no destination is specified, send to everyone (multicast)
   modemanager_RI_tm_To_PID(PID_env, IN_reprt
);
}

void modemanager_RI_tm_To_PID(asn1SccPID dest_pid, 
      const asn1SccT_Report *IN_reprt
)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to EGSE (corresponding PI: tm)
      printf ("INNER_RI: modemanager,egse,tm,tm,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_modemanager_tm
     (asn1SccPID,
      void *, size_t);

   vm_modemanager_tm
     (dest_pid,
      (void *)IN_reprt, sizeof(asn1SccT_Report));


}

// Get the PID of the sender function. The actual function is defined in _vm_if.c
// as the sender PID is received together with incoming PI calls
void modemanager_RI_get_sender(asn1SccPID *sender_pid)
{
  extern void modemanager_get_sender(asn1SccPID *sender_pid);
  modemanager_get_sender(sender_pid);
}
