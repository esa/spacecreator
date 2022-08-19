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

#ifndef IFACE_H
#define IFACE_H
#include "iface_states.h"
#include "qgen_base_workspace_myseqcomplex.h"
#include "qgen_base_workspace_myseqsimple.h"

extern void Iface_initStates
  (Iface_State* const State);
extern void Iface_comp
  (MySeqComplex const* const input,
   MySeqComplex* const output,
   Iface_State* const State);
extern void Iface_up
  (Iface_State* const State);

#endif
/*  @EOF  */
