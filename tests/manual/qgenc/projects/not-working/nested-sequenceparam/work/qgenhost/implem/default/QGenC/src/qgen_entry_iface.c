/* Copyright (C) Your Company Name */
/*
 * @generated with QGen Code Generator 23.0w (20211221)
 * Command line arguments: ./xmi/Iface.xmi
 *   --gen-entrypoint
 *   --wrap-io
 *   --pre-process-xmi
 *   --incremental
 *   --no-misra
 *   --language c
 *   --output src
*/

#include "qgen_entry_iface.h"

Iface_State Iface_memory;
void qgen_entry_Iface_init (void)
{
   Iface_initStates (&Iface_memory);
}
void qgen_entry_Iface_comp
  (qgen_entry_Iface_comp_Input const* const Input,
   qgen_entry_Iface_comp_Output* const Output)
{
   Iface_comp (&Input->input, &Output->output, &Iface_memory);
   Iface_up (&Iface_memory);
}
/*  @EOF  */
