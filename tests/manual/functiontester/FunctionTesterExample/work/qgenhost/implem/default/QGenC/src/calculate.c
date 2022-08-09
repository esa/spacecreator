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

#include "calculate.h"

void Calculate_initStates
  (Calculate_State* const State)
{
   /* Block 'Calculate/If' */
   State->If_last = 0;
   /* End Block 'Calculate/If' */

   /* Block 'Calculate/Merge' */
   State->Merge_out1 = 0.0E+00;
   /* End Block 'Calculate/Merge' */

   /* Block 'Calculate/Unit Delay' */
   State->Unit_Delay_memory = 0;
   /* End Block 'Calculate/Unit Delay' */

}
void Calculate_comp
  (MyInteger const intparam,
   MyReal const realparam,
   MyBool const boolparam,
   MyReal* const outparam,
   Calculate_State* const State)
{
   GAUINT8 If_current = 2;

   /* Block 'Calculate/intparam' */
   State->intparam_out1 = intparam;
   /* End Block 'Calculate/intparam' */

   /* Block 'Calculate/realparam' */
   State->realparam_out1 = realparam;
   /* End Block 'Calculate/realparam' */

   /* Block 'Calculate/boolparam' */
   /* Block 'Calculate/If' */
   /* Determine action to execute */
   if (boolparam == GATRUE) {
      If_current = 1;
   }
   /* Execute target action */
   switch (If_current) {
      case 1:
         If_Action_Subsystem_comp (State->intparam_out1, State->realparam_out1, &State->Merge_out1);
         break;
      default:
         If_Action_Subsystem1_comp (State->intparam_out1, State->realparam_out1, &State->Merge_out1);
         break;
   }
   /* Memorize action for the next step */
   State->If_last = If_current;
   /* End Block 'Calculate/If' */
   /* End Block 'Calculate/boolparam' */

   /* Block 'Calculate/outparam' */
   *outparam = State->Merge_out1;
   /* End Block 'Calculate/outparam' */

}
void Calculate_up
  (Calculate_State* const State)
{
   /* update 'Calculate/Unit Delay' */
   State->Unit_Delay_memory = State->intparam_out1;
   /* End update 'Calculate/Unit Delay' */

}
/*  @EOF  */
