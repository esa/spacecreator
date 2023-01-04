/* Body file for function Function_2
 * Generated by TASTE on 2023-01-03 12:01:28
 */
#include "function_2_vm_if.h"
#include "C_ASN1_Types.h"
#include <stdio.h>
extern long long getTimeInMilliseconds(void);



extern void adainit(void);

unsigned function_2_initialized = 0;
void init_function_2(void)
{
   if (0 == function_2_initialized) {
      // Initialize epoch for the MSC tracing in debug mode
      long long unused = getTimeInMilliseconds();
      function_2_initialized = 1;
      // Initialize Ada runtime to call elaboration functions
      adainit();
      puts ("[TASTE] Initialization completed for function Function_2");
      function_2_initialized = 2;
   }
}
void function_2_PI_1
      (const char *IN_p1, size_t IN_p1_len)

{
   // In Debug mode and x86 builts only: log MSC data when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
     innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      printf ("INNER_PI: function_2,pi_1,%lld\n", msc_time);
      fflush(stdout);
   }

   //  Declare user code function as external (it may not have a .h if it is in Ada)
   extern void function_2_PI_PI_1
      (const asn1SccMySeqOf *);





   // Call user code
   function_2_PI_PI_1 ((asn1SccMySeqOf *)IN_p1);

   // trace the SDL state (appears on the MSC)
   if (1 == innerMsc) {
      extern char *function_2_state(void);
      printf ("INNER_SDL_STATE: function_2,%s\n", function_2_state());
      fflush(stdout);
   }
}

