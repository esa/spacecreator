/* Body file for function utils2
 * Generated by TASTE (kazoo/templates/skeletons/c-body/function.tmplt)
 * You can edit this file, it will not be overwritten

    !! IMPORTANT                                                        !!
    !! When you modify your design (interface view), you must update    !!
    !! the procedures corresponding to the provided interfaces in this  !!
    !! file. The up-to-date signatures can be found in the .ads file.   !!
*/
#include "utils2.h"
//#include <stdio.h>


void utils2_startup(void)
{
   // Write your initialisation code
   // You may call sporadic required interfaces and start timers
   // puts ("[utils2] Startup");
}

void utils2_PI_calc2
      (const asn1SccMyInteger2 *IN_calc2in1,
       const asn1SccMyReal2 *IN_calc2in2,
       const asn1SccMyBool2 *IN_calc2in3,
       const asn1SccMySeqOf2 *IN_calc2in4,
       const asn1SccMySeq2 *IN_calc2in5,
       asn1SccMyInteger2 *OUT_cala2out1,
       asn1SccMyReal2 *OUT_cala2out2,
       asn1SccMyBool2 *OUT_cala2out3,
       asn1SccMySeqOf2 *OUT_cala2out4,
       asn1SccMySeq2 *OUT_cala2out5)

{
    *OUT_cala2out1 = (*IN_calc2in1) + 1;
    *OUT_cala2out2 = (*IN_calc2in2) + 1;
    *OUT_cala2out3 = !(*IN_calc2in3);
    *OUT_cala2out4 = (*IN_calc2in4);
    *OUT_cala2out5 = (*IN_calc2in5);
}


