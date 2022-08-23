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

#ifndef QGEN_TYPE_WRAP_IO_QGEN_ENTRY_CALCULATE_H
#define QGEN_TYPE_WRAP_IO_QGEN_ENTRY_CALCULATE_H
#include "qgen_base_workspace_myinteger.h"
#include "qgen_base_workspace_myreal.h"
#include "qgen_base_workspace_mybool.h"

typedef struct {
   MyInteger intparam;
   MyReal realparam;
   MyBool boolparam;
} qgen_entry_Calculate_comp_Input;
typedef struct {
   MyReal outparam;
} qgen_entry_Calculate_comp_Output;

#endif
/*  @EOF  */
