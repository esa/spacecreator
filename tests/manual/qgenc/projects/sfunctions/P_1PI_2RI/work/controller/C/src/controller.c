/* Body file for function controller
 * Generated by TASTE (kazoo/templates/skeletons/c-body/function.tmplt)
 * You can edit this file, it will not be overwritten

    !! IMPORTANT                                                        !!
    !! When you modify your design (interface view), you must update    !!
    !! the procedures corresponding to the provided interfaces in this  !!
    !! file. The up-to-date signatures can be found in the .ads file.   !!
*/

#include "controller.h"
#include <stdio.h>

void controller_startup(void)
{

}

void controller_PI_trigger(void)
{
    const asn1SccMyInteger in1 = 2;
    const asn1SccMyReal    in2 = 3.0;
    const asn1SccMyBool    in3 = true;
    const asn1SccMySeqOf   in4 = {{1, 2}};
    const asn1SccMySeq     in5 = {2, 3.0, true, {{1, 1}}};

    const asn1SccMyInteger2 in21 = 3;
    const asn1SccMyReal2    in22 = 6.0;
    const asn1SccMyBool2    in23 = false;
    const asn1SccMySeqOf2   in24 = {{2, 4}};
    const asn1SccMySeq2     in25 = {4, 6.0, false, {{2, 2}}};

    asn1SccMyInteger out1;
    asn1SccMyReal    out2;
    asn1SccMyBool    out3;
    asn1SccMySeqOf   out4;
    asn1SccMySeq     out5;

    asn1SccMyInteger2 out21;
    asn1SccMyReal2    out22;
    asn1SccMyBool2    out23;
    asn1SccMySeqOf2   out24;
    asn1SccMySeq2     out25;

    controller_RI_iface( &in1, &in2, &in3, &in4, &in5,
                         &out1, &out2, &out3, &out4, &out5,
                         &in21, &in22, &in23, &in24, &in25,
                         &out21, &out22, &out23, &out24, &out25);

    printf("controller out1 : %lu\n", out1);
    printf("controller out2 : %f\n", out2);
    printf("controller out3 : %d\n", out3);
    printf("controller out4 : %lu, %lu\n", out4.arr[0], out4.arr[1]);
    printf("controller out5 : %lu, %f, %d, %lu, %lu\n", out5.myInteger, out5.myReal, out5.myBool, out5.mySeqOfVar.arr[0], out5.mySeqOfVar.arr[1]);

    printf("controller out21 : %lu\n", out21);
    printf("controller out22 : %f\n", out22);
    printf("controller out23 : %d\n", out23);
    printf("controller out24 : %lu, %lu\n", out24.arr[0], out24.arr[1]);
    printf("controller out25 : %lu, %f, %d, %lu, %lu\n\n", out25.myInteger2, out25.myReal2, out25.myBool2, out25.mySeqOfVar2.arr[0], out25.mySeqOfVar2.arr[1]);

    if((out1 == in1 * 2) &&
       (out2 == in2 * 2) &&
       (out3 == !in3   ) &&
       ((out4.arr[0] == in4.arr[0]) && (out4.arr[1] == in4.arr[1])) &&
       (
           (out5.myInteger         == in5.myInteger) &&
           (out5.myReal            == in5.myReal) &&
           (out5.myBool            == in5.myBool) &&
           (out5.mySeqOfVar.arr[0] == in5.mySeqOfVar.arr[0]) &&
           (out5.mySeqOfVar.arr[1] == in5.mySeqOfVar.arr[1])
       )
       &&
       (out21 == in21 + 1) &&
       (out22 == in22 + 1) &&
       (out23 == !in23   ) &&
       ((out24.arr[0] == in24.arr[0]) && (out24.arr[1] == in24.arr[1])) &&
       (
           (out25.myInteger2         == in25.myInteger2) &&
           (out25.myReal2            == in25.myReal2) &&
           (out25.myBool2            == in25.myBool2) &&
           (out25.mySeqOfVar2.arr[0] == in25.mySeqOfVar2.arr[0]) &&
           (out25.mySeqOfVar2.arr[1] == in25.mySeqOfVar2.arr[1])
       )
      ) {
        printf("Pass\n\n");
    } else {
        printf("FAILED !");
    }
}
