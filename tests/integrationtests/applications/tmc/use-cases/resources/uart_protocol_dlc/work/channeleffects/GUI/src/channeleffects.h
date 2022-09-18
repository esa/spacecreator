/* Header file for function ChannelEffects
 * Generated by TASTE on 2022-09-19 00:08:14
 * DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
 */

#pragma once

#include "dataview-uniq.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>

void channeleffects_startup(void);

/* Provided interfaces */
void channeleffects_PI_Poll( void );

/* Required interfaces */
extern void channeleffects_RI_CorruptChar( const asn1SccUINT8 * );


extern void channeleffects_RI_DropChar( void );


#ifdef __cplusplus
}
#endif

