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

#ifndef CALCULATE_H
#define CALCULATE_H
#include "calculate_states.h"
#include "qgen_base_workspace_myinteger.h"
#include "qgen_base_workspace_myreal.h"
#include "qgen_base_workspace_mybool.h"
#include "qgen_types.h"
#include "if_action_subsystem.h"
#include "if_action_subsystem1.h"

extern void Calculate_initStates
  (Calculate_State* const State);
extern void Calculate_comp
  (MyInteger const intparam,
   MyReal const realparam,
   MyBool const boolparam,
   MyReal* const outparam,
   Calculate_State* const State);
extern void Calculate_up
  (Calculate_State* const State);

#endif
/*  @EOF  */
