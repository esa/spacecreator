// Implementation of the glue code in C handling required interfaces

#include "dataview-uniq.h" // Always required for the definition of the PID type
// There are no user-defined required interfaces for this function

// Get the PID of the sender function. The actual function is defined in _vm_if.c
// as the sender PID is received together with incoming PI calls
void actuator_RI_get_sender(asn1SccPID *sender_pid)
{
  extern void actuator_get_sender(asn1SccPID *sender_pid);
  actuator_get_sender(sender_pid);
}
