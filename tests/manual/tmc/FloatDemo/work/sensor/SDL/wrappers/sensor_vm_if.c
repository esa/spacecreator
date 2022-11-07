/* Body file for function Sensor
 * Generated by TASTE (kazoo/templates/glue/language_wrappers/vm-if-body/function.tmplt)
 */
#include "sensor_vm_if.h"
#include "dataview-uniq.h"  // contains definition of PID type
#include <stdio.h>
extern long long getTimeInMilliseconds(void);

// Define a global PID variable used to keep track of the sender of the last message
static asn1SccPID sender_pid = PID_env;

// API to provide to the user code the PID of the last PI caller
void sensor_get_sender(asn1SccPID *OUT_sender_pid, asn1SccPID unused)
{
  *OUT_sender_pid = sender_pid;
}



extern void adainit(void);

extern void init_demo_timer_manager(void);
extern unsigned demo_timer_manager_initialized;
#include "demo_timer_manager_vm_if.h"
extern void init_demo_timer_manager(void);
extern unsigned demo_timer_manager_initialized;
#include "demo_timer_manager_vm_if.h"
unsigned sensor_initialized = 0;
void init_sensor(void)
{
   if (0 == sensor_initialized) {
      // Initialize epoch for the MSC tracing in debug mode
      long long unused = getTimeInMilliseconds();
      sensor_initialized = 1;
      // Initialize Ada runtime to call elaboration functions
      adainit();
      puts ("[TASTE] Initialization completed for function Sensor");
      sensor_initialized = 2;
   }
}
void sensor_tc(void)
{
   sender_pid = sensor_tc_get_sender();
   // In Debug mode and x86 builts only: log MSC data when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
     innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      printf ("INNER_PI: sensor,tc,%lld\n", msc_time);
      fflush(stdout);
   }

   //  Directly call user code, no parameters to pass
   extern void sensor_PI_tc(void);
   sensor_PI_tc();
   // puts ("sensor_tc called");
   // trace the SDL state (appears on the MSC)
   if (1 == innerMsc) {
      extern char *sensor_state(void);
      printf ("INNER_SDL_STATE: sensor,%s\n", sensor_state());
      fflush(stdout);
   }
}




void sensor_trigger(void)
{
   sender_pid = sensor_trigger_get_sender();
   // In Debug mode and x86 builts only: log MSC data when environment variable is set
   static int innerMsc = -1;
   if (-1 == innerMsc)
     innerMsc = (NULL != getenv("TASTE_INNER_MSC"))?1:0;
   if (1 == innerMsc) {
      long long msc_time = getTimeInMilliseconds();
      printf ("INNER_PI: sensor,trigger,%lld\n", msc_time);
      fflush(stdout);
   }

   //  Directly call user code, no parameters to pass
   extern void sensor_PI_trigger(void);
   sensor_PI_trigger();
   // puts ("sensor_trigger called");
   // trace the SDL state (appears on the MSC)
   if (1 == innerMsc) {
      extern char *sensor_state(void);
      printf ("INNER_SDL_STATE: sensor,%s\n", sensor_state());
      fflush(stdout);
   }
}

