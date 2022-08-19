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

#ifndef QGEN_ENTRY_IFACE_H
#define QGEN_ENTRY_IFACE_H
#include "iface.h"
#include "qgen_type_wrap_io_qgen_entry_iface.h"
#include "iface_states.h"

extern Iface_State Iface_memory;
extern void qgen_entry_Iface_init (void);
extern void qgen_entry_Iface_comp
  (qgen_entry_Iface_comp_Input const* const Input,
   qgen_entry_Iface_comp_Output* const Output);

#endif
/*  @EOF  */
