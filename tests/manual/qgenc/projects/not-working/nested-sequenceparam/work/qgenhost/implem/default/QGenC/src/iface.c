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

#include "iface.h"

void Iface_initStates
  (Iface_State* const State)
{
   /* Block 'Iface/Unit Delay' */
   State->Unit_Delay_memory = 0.0E+00;
   /* End Block 'Iface/Unit Delay' */

}
void Iface_comp
  (MySeqComplex const* const input,
   MySeqComplex* const output,
   Iface_State* const State)
{
   MySeqSimple Bus_Creator_out1;
   /* Output from Iface/Bus Creator/OutDataPort1 */

   MySeqSimple Bus_Creator1_out1;
   /* Output from Iface/Bus Creator1/OutDataPort1 */

   MySeqComplex output_MySeqComplex_BusCre_out1;
   /* Output from Iface/output_MySeqComplex_BusCre/OutDataPort1 */


   /* Block 'Iface/NOT' */
   /* Block 'Iface/Bus Selector' */
   /* Block 'Iface/Gain1' */
   /* Block 'Iface/Gain' */
   /* Block 'Iface/Bus Creator' */
   Bus_Creator_out1.intSeqElement = 2 * input->simpleSeq1.intSeqElement;
   Bus_Creator_out1.realSeqElement = 2.0E+00 * input->simpleSeq1.realSeqElement;
   Bus_Creator_out1.boolSeqElement = !input->simpleSeq1.boolSeqElement;
   /* End Block 'Iface/Bus Creator' */
   /* End Block 'Iface/Gain' */
   /* End Block 'Iface/Gain1' */
   /* End Block 'Iface/Bus Selector' */
   /* End Block 'Iface/NOT' */

   /* Block 'Iface/NOT1' */
   /* Block 'Iface/input_MySeqComplex_BusSel' */
   /* Block 'Iface/input' */
   /* Block 'Iface/Bus Selector1' */
   /* Block 'Iface/Gain3' */
   /* Block 'Iface/Gain2' */
   /* Block 'Iface/Bus Creator1' */
   Bus_Creator1_out1.intSeqElement = 2 * input->simpleSeq2.intSeqElement;
   Bus_Creator1_out1.realSeqElement = 2.0E+00 * input->simpleSeq2.realSeqElement;
   Bus_Creator1_out1.boolSeqElement = !input->simpleSeq2.boolSeqElement;
   /* End Block 'Iface/Bus Creator1' */
   /* End Block 'Iface/Gain2' */
   /* End Block 'Iface/Gain3' */
   /* End Block 'Iface/Bus Selector1' */
   /* End Block 'Iface/input' */
   /* End Block 'Iface/input_MySeqComplex_BusSel' */
   /* End Block 'Iface/NOT1' */

   /* Block 'Iface/output_MySeqComplex_BusCre' */
   output_MySeqComplex_BusCre_out1.simpleSeq1 = Bus_Creator_out1;
   output_MySeqComplex_BusCre_out1.simpleSeq2 = Bus_Creator1_out1;
   /* End Block 'Iface/output_MySeqComplex_BusCre' */

   /* Block 'Iface/output' */
   *output = output_MySeqComplex_BusCre_out1;
   /* End Block 'Iface/output' */

   /* Block 'Iface/Constant' */
   State->Constant_out1 = 1.0E+00;
   /* End Block 'Iface/Constant' */

}
void Iface_up
  (Iface_State* const State)
{
   /* update 'Iface/Unit Delay' */
   State->Unit_Delay_memory = State->Constant_out1;
   /* End update 'Iface/Unit Delay' */

}
/*  @EOF  */
