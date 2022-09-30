/* Header file for function RTApp
 * Generated by TASTE on 2022-09-30 02:17:32
 * DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
 */

#pragma once

#include "dataview-uniq.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>

void rtapp_startup(void);

/* Provided interfaces */
void rtapp_PI_DLCRxError( const asn1SccT_UART_DLC_RxErrorCode * );


void rtapp_PI_DLCRxSDU( const asn1SccT_UART_DLC_SDU * );


void rtapp_PI_Poll( void );

/* Required interfaces */
extern void rtapp_RI_DLCRxReset( void );


#ifdef __cplusplus
}
#endif

