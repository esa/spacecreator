// Implementation of the glue code in C handling required interfaces

#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"
#include "dataview-uniq.h"

extern unsigned erroredchannel_initialized;

void erroredchannel_RI_PHYRxChar
      (const asn1SccUINT8 *IN_p1);
void erroredchannel_RI_PHYRxChar
      (const asn1SccUINT8 *IN_p1)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to RTDLCR (corresponding PI: PHYRxChar)
      printf ("INNER_RI: erroredchannel,rtdlcr,phyrxchar,phyrxchar,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_erroredchannel_phyrxchar
     (void *, size_t);

   vm_erroredchannel_phyrxchar
     ((void *)IN_p1, sizeof(asn1SccUINT8));


}



void erroredchannel_RI_PHYRxError(void);
void erroredchannel_RI_PHYRxError(void)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to RTDLCR (corresponding PI: PHYRxError)
      printf ("INNER_RI: erroredchannel,rtdlcr,phyrxerror,phyrxerror,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_erroredchannel_phyrxerror(void);
   vm_erroredchannel_phyrxerror();

}



void erroredchannel_RI_PHYTxError(void);
void erroredchannel_RI_PHYTxError(void)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to BCDLCT (corresponding PI: PHYTxError)
      printf ("INNER_RI: erroredchannel,bcdlct,phytxerror,phytxerror,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_erroredchannel_phytxerror(void);
   vm_erroredchannel_phytxerror();

}

