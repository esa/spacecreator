// Implementation of the glue code in C handling required interfaces

#include <stdlib.h>
#include <stdio.h>
#include "PrintTypesAsASN1.h"
#include "timeInMS.h"
#include "C_ASN1_Types.h"
#include "dataview-uniq.h"

extern unsigned rtdlcr_initialized;

void rtdlcr_RI_DLCRxError
      (const asn1SccT_UART_DLC_RxErrorCode *IN_p1);
void rtdlcr_RI_DLCRxError
      (const asn1SccT_UART_DLC_RxErrorCode *IN_p1)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to RTApp (corresponding PI: DLCRxError)
      printf ("INNER_RI: rtdlcr,rtapp,dlcrxerror,dlcrxerror,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_rtdlcr_dlcrxerror
     (void *, size_t);

   vm_rtdlcr_dlcrxerror
     ((void *)IN_p1, sizeof(asn1SccT_UART_DLC_RxErrorCode));


}



void rtdlcr_RI_DLCRxSDU
      (const asn1SccT_UART_DLC_SDU *IN_p1);
void rtdlcr_RI_DLCRxSDU
      (const asn1SccT_UART_DLC_SDU *IN_p1)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to RTApp (corresponding PI: DLCRxSDU)
      printf ("INNER_RI: rtdlcr,rtapp,dlcrxsdu,dlcrxsdu,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_rtdlcr_dlcrxsdu
     (void *, size_t);

   vm_rtdlcr_dlcrxsdu
     ((void *)IN_p1, sizeof(asn1SccT_UART_DLC_SDU));


}



void rtdlcr_RI_RESET_FrameTimer(void);
void rtdlcr_RI_RESET_FrameTimer(void)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      // Log message to demo_Timer_Manager (corresponding PI: RESET_rtdlcr_FrameTimer)
      printf ("INNER_RI: rtdlcr,demo_timer_manager,reset_frametimer,reset_rtdlcr_frametimer,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_rtdlcr_reset_frametimer(void);
   vm_rtdlcr_reset_frametimer();

}



void rtdlcr_RI_SET_FrameTimer
      (const asn1SccT_UInt32 *IN_val);
void rtdlcr_RI_SET_FrameTimer
      (const asn1SccT_UInt32 *IN_val)
{
   // Log MSC data on Linux when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
      innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      PrintASN1T_UInt32 ("INNERDATA: set_frametimer::T_UInt32::val", IN_val);
      puts("");
      // Log message to demo_Timer_Manager (corresponding PI: SET_rtdlcr_FrameTimer)
      printf ("INNER_RI: rtdlcr,demo_timer_manager,set_frametimer,set_rtdlcr_frametimer,%lld\n", msc_time);
      fflush(stdout);
   }


   // Send the message via the middleware API
   extern void vm_rtdlcr_set_frametimer
     (void *, size_t);

   vm_rtdlcr_set_frametimer
     ((void *)IN_val, sizeof(asn1SccT_UInt32));


}

