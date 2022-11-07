/* Header file for function Actuator
 * Generated by TASTE (kazoo/templates/glue/language_wrappers/vm_if-header/function.tmplt)
 * DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
 */

/* The purpose of this file is to provide a wrapper around the user code,
 * with a single interface that is independent of the implementation language
 * of the function. The parameters of each PI are blackboxes encoded
 * in the form specified in the interface view (Native, uPER, or ACN)
*/

#pragma once
#if defined(__unix__) || defined (__MSP430__)
   #include <stdlib.h>
   #include <stdio.h>
#else
   typedef unsigned size_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif


// Function initialization - calls user startup code of all dependent functions
void init_actuator(void);


/* Provided interfaces */
void actuator_controlsignal
      (const char *IN_x, size_t IN_x_len);


#ifdef __cplusplus
}
#endif

