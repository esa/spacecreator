/* Header file for function Controller
 * Generated by TASTE on 2022-08-10 13:16:54
 * DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
 */

#pragma once

#include "dataview-uniq.h"

#ifdef __cplusplus
extern "C" {
#endif


void controller_startup(void);

/* Provided interfaces */
void controller_PI_Trigger( void );

/* Required interfaces */
extern void controller_RI_IntIface( const asn1SccMyInteger *, asn1SccMyInteger * );


#ifdef __cplusplus
}
#endif
