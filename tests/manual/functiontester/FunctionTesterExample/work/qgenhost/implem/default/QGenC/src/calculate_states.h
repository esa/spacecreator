/* Copyright (C) Your Company Name */
/*
 * @generated with QGen Code Generator 23.0w (20211221)
 * Command line arguments: ./xmi/Calculate.xmi
 *   --gen-entrypoint
 *   --wrap-io
 *   --pre-process-xmi
 *   --incremental
 *   --no-misra
 *   --language c
 *   --output src
*/

#ifndef CALCULATE_STATES_H
#define CALCULATE_STATES_H
#include "qgen_base_workspace_myinteger.h"
#include "qgen_base_workspace_myreal.h"
#include "qgen_types.h"

typedef struct {
   MyInteger intparam_out1;
   /* Output from Calculate/intparam/OutDataPort1 */

   MyReal realparam_out1;
   /* Output from Calculate/realparam/OutDataPort1 */

   GAREAL Merge_out1;
   /* Output from Calculate/Merge/OutDataPort1 */

   GAUINT8 If_last;
   MyInteger Unit_Delay_memory;
   /* Memory from Calculate/Unit Delay */

} Calculate_State;

#endif
/*  @EOF  */
