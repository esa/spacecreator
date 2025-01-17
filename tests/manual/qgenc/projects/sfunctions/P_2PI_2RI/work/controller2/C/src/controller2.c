/* Body file for function controller2
 * Generated by TASTE (kazoo/templates/skeletons/c-body/function.tmplt)
 * You can edit this file, it will not be overwritten

    !! IMPORTANT                                                        !!
    !! When you modify your design (interface view), you must update    !!
    !! the procedures corresponding to the provided interfaces in this  !!
    !! file. The up-to-date signatures can be found in the .ads file.   !!
*/

#include "controller2.h"
#include <stdio.h>

void controller2_startup(void)
{

}

void controller2_PI_trigger2(void)
{
    const asn1SccMyInteger2 in1 = 4;
    const asn1SccMyReal2    in2 = 6.0;
    const asn1SccMyBool2    in3 = false;
    const asn1SccMySeqOf2   in4 = {{2, 4}};
    const asn1SccMySeq2     in5 = {4, 6.0, false, {{2, 2}}};

    asn1SccMyInteger2 out1;
    asn1SccMyReal2    out2;
    asn1SccMyBool2    out3;
    asn1SccMySeqOf2   out4;
    asn1SccMySeq2     out5;

    controller2_RI_iface2( &in1, &in2, &in3, &in4, &in5,
                           &out1, &out2, &out3, &out4, &out5);

    printf("controller2 out1 : %lu\n", out1);
    printf("controller2 out2 : %f\n", out2);
    printf("controller2 out3 : %d\n", out3);
    printf("controller2 out4 : %lu, %lu\n", out4.arr[0], out4.arr[1]);
    printf("controller2 out5 : %lu, %f, %d, %lu, %lu\n\n", out5.myInteger, out5.myReal, out5.myBool, out5.mySeqOfVar.arr[0], out5.mySeqOfVar.arr[1]);

    if((out1 == in1 + 1) &&
       (out2 == in2 + 1) &&
       (out3 == !in3   ) &&
       ((out4.arr[0] == in4.arr[0]) && (out4.arr[1] == in4.arr[1])) &&
       (
           (out5.myInteger         == in5.myInteger) &&
           (out5.myReal            == in5.myReal) &&
           (out5.myBool            == in5.myBool) &&
           (out5.mySeqOfVar.arr[0] == in5.mySeqOfVar.arr[0]) &&
           (out5.mySeqOfVar.arr[1] == in5.mySeqOfVar.arr[1])
       )) {
        printf("Pass\n\n");
    } else {
        printf("FAILED !");
    }
}
