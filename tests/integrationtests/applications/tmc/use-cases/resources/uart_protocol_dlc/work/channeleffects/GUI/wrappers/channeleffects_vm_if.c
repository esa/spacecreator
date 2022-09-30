/* Body file for function ChannelEffects
 * Generated by TASTE on 2022-09-30 02:17:32
 */
#include "channeleffects_vm_if.h"
#include <stdio.h>
extern long long getTimeInMilliseconds(void);




unsigned channeleffects_initialized = 0;
void init_channeleffects(void)
{
   if (0 == channeleffects_initialized) {
      // Initialize epoch for the MSC tracing in debug mode
      long long unused = getTimeInMilliseconds();
      channeleffects_initialized = 1;
      // Call user code startup function
      extern void channeleffects_startup(void);
      channeleffects_startup();
      puts ("[TASTE] Initialization completed for function ChannelEffects");
      channeleffects_initialized = 2;
   }
}
void channeleffects_Poll()
{

   //  Directly call user code, no parameters to pass
   extern void channeleffects_PI_Poll(void);
   channeleffects_PI_Poll();
   // puts ("channeleffects_poll called");
}

