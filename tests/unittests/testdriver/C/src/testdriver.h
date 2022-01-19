
/* Header file for function TestDriver in C language
 * Generated by TASTE on 2022-01-19 15:56:11
 * Context Parameters present : NO
 * Provided interfaces : StartTest
 * Required interfaces : InterfaceUnderTest
 * User-defined properties for this function:
 *  |_ TASTE::Is_Component_Type = false
 * DO NOT EDIT THIS FILE, IT WILL BE OVERWRITTEN DURING THE BUILD
 */

#pragma once

#include "dataview-uniq.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __unix__
#include <stdio.h>
#include <stdlib.h>
#endif

void testdriver_startup(void);

/* Provided interfaces */
void testdriver_PI_StartTest(void);

/* Required interfaces */
extern void testdriver_RI_InterfaceUnderTest(const asn1SccMyBool *, const asn1SccMyReal *, const asn1SccMyInteger *,
        const asn1SccMyInteger *, asn1SccMyInteger *);

#ifdef __cplusplus
}
#endif
