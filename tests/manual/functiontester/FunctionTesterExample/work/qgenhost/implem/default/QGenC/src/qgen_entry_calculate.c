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

#include "qgen_entry_calculate.h"

Calculate_State Calculate_memory;
void qgen_entry_Calculate_init (void)
{
   Calculate_initStates (&Calculate_memory);
}
void qgen_entry_Calculate_comp
  (qgen_entry_Calculate_comp_Input const* const Input,
   qgen_entry_Calculate_comp_Output* const Output)
{
   Calculate_comp (Input->intparam, Input->realparam, Input->boolparam, &Output->outparam, &Calculate_memory);
   Calculate_up (&Calculate_memory);
}
/*  @EOF  */
