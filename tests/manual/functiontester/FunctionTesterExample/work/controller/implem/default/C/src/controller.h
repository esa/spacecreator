/* Header file for function Controller
 * Generated by TASTE on 2022-08-08 12:50:27
 * DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
 */

#pragma once

#include "dataview-uniq.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>

void controller_startup(void);

/* Provided interfaces */
void controller_PI_Trigger( void );

/* Required interfaces */
extern void controller_RI_Calculate( const asn1SccMyInteger *, const asn1SccMyReal *, const asn1SccMyBool *, asn1SccMyReal * );


#ifdef __cplusplus
}
#endif
