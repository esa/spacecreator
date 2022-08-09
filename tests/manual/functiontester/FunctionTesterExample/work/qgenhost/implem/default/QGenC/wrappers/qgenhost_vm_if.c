/* Body file for function QGenHost
 * Generated by TASTE on 2022-08-08 12:50:27
 */
#include "qgenhost_vm_if.h"
#include "C_ASN1_Types.h"
#include <stdio.h>
extern long long getTimeInMilliseconds(void);




unsigned qgenhost_initialized = 0;
void init_qgenhost(void)
{
   if (0 == qgenhost_initialized) {
      // Initialize epoch for the MSC tracing in debug mode
      long long unused = getTimeInMilliseconds();
      qgenhost_initialized = 1;
      // Call user code startup function
      qgenhost_PI_Calculate_startup();
      puts ("[TASTE] Initialization completed for function QGenHost");
      qgenhost_initialized = 2;
   }
}
void qgenhost_Calculate
      (const char *IN_intparam, size_t IN_intparam_len,
       const char *IN_realparam, size_t IN_realparam_len,
       const char *IN_boolparam, size_t IN_boolparam_len,
       char *OUT_outparam, size_t *OUT_outparam_len)

{
   // In Debug mode and x86 builts only: log MSC data when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
     innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      printf ("INNER_PI: qgenhost,calculate,%lld\n", msc_time);
      fflush(stdout);
   }

   //  Declare user code function as external (it may not have a .h if it is in Ada)
   extern void qgenhost_PI_Calculate
      (const asn1SccMyInteger *,
       const asn1SccMyReal *,
       const asn1SccMyBool *,
       asn1SccMyReal *);





   // Call user code
   qgenhost_PI_Calculate ((asn1SccMyInteger *)IN_intparam, (asn1SccMyReal *)IN_realparam, (asn1SccMyBool *)IN_boolparam, (asn1SccMyReal *)OUT_outparam);

}

