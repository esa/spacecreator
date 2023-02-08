/* Body file for function controller1
 * Generated by TASTE (kazoo/templates/skeletons/c-body/function.tmplt)
 * You can edit this file, it will not be overwritten

    !! IMPORTANT                                                        !!
    !! When you modify your design (interface view), you must update    !!
    !! the procedures corresponding to the provided interfaces in this  !!
    !! file. The up-to-date signatures can be found in the .ads file.   !!
*/

#include "controller1.h"
#include <stdio.h>

void controller1_startup(void)
{

}

void controller1_PI_trigger1(void)
{
    const asn1SccMyInteger in1 = 2;
    const asn1SccMyReal    in2 = 3.0;
    const asn1SccMyBool    in3 = true;
    const asn1SccMySeqOf   in4 = {{1, 2}};
    const asn1SccMySeq     in5 = {2, 3.0, true, {{1, 1}}};

    asn1SccMyInteger out1;
    asn1SccMyReal    out2;
    asn1SccMyBool    out3;
    asn1SccMySeqOf   out4;
    asn1SccMySeq     out5;

    controller1_RI_iface( &in1, &in2, &in3, &in4, &in5,
                          &out1, &out2, &out3, &out4, &out5);

    printf("controller1 out1 : %lu\n", out1);
    printf("controller1 out2 : %f\n", out2);
    printf("controller1 out3 : %d\n", out3);
    printf("controller1 out4 : %lu, %lu\n", out4.arr[0], out4.arr[1]);
    printf("controller1 out5 : %lu, %f, %d, %lu, %lu\n\n", out5.myInteger, out5.myReal, out5.myBool, out5.mySeqOfVar.arr[0], out5.mySeqOfVar.arr[1]);

    if((out1 == in1) &&
       (out2 == in2) &&
       (out3 == in3) &&
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
