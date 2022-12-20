#include <stdlib.h>
#include <stdio.h>

#include "asn1crt.h"
#include "asn1crt_encoding.h"
#include "bidirectional_enc.h"
#include "bidirectional_dec.h"

int main()
{
    // === TC1 ===

    TC1TypeEnc input;
    TC1TypeEnc_Initialize(&input);

    input.a = 2;
    input.b = 1.11;

    BitStream bitStream;
    uint8_t encBuff[TC1TypeEnc_REQUIRED_BYTES_FOR_ACN_ENCODING + 1];
    BitStream_Init(&bitStream, encBuff, TC1TypeEnc_REQUIRED_BYTES_FOR_ACN_ENCODING);

    int errCode = 0;

    bool ret = TC1TypeEnc_ACN_Encode(&input, &bitStream, &errCode, TRUE);
    if(!ret) {
        printf("TC1 Encoding error: %d\n", ret);
        return EXIT_FAILURE;
    }

    printf("TC1 Encoding successful\n");

    BitStream_AttachBuffer(&bitStream, encBuff, TC1TypeEnc_REQUIRED_BYTES_FOR_ACN_ENCODING);

    TC1TypeDec output;
    ret = TC1TypeDec_ACN_Decode(&output, &bitStream, &errCode);
    if(!ret) {
        printf("TC1 Decoding error: %d\n", ret);
        return EXIT_FAILURE;
    }

    printf("TC1 Decoding successful\n");

    printf("output.a = %lu\n", (unsigned long)output.a);
    printf("output.b = %f\n", output.b);

    // === TC2 ===

    TC2TypeEnc input2;
    TC2TypeEnc_Initialize(&input2);

    input2.c = TRUE;
    input2.d = bEnc;

    BitStream bitStream2;
    uint8_t encBuff2[TC2TypeEnc_REQUIRED_BYTES_FOR_ACN_ENCODING + 1];
    BitStream_Init(&bitStream2, encBuff2, TC2TypeEnc_REQUIRED_BYTES_FOR_ACN_ENCODING);

    ret = TC2TypeEnc_ACN_Encode(&input2, &bitStream2, &errCode, TRUE);
    if(!ret) {
        printf("TC2 Encoding error: %d\n", ret);
        return EXIT_FAILURE;
    }

    printf("TC2 Encoding successful\n");

    BitStream_AttachBuffer(&bitStream2, encBuff2, TC2TypeEnc_REQUIRED_BYTES_FOR_ACN_ENCODING);

    TC2TypeDec output2;
    ret = TC2TypeDec_ACN_Decode(&output2, &bitStream2, &errCode);
    if(!ret) {
        printf("TC2 Decoding error: %d\n", ret);
        return EXIT_FAILURE;
    }

    printf("TC2 Decoding successful\n");

    printf("output.c = %d\n", output2.c);
    printf("output.d = %d\n", (int)output2.d);

    // === TC1s ===

    TC1sEnc input3;
    TC1sEnc_Initialize(&input3);

    input3.nCount = 2;
    input3.arr[0].a = 5;
    input3.arr[0].b = 0.12;
    input3.arr[1].a = 4;
    input3.arr[1].b = 0.34;

    BitStream bitStream3;
    uint8_t encBuff3[TC1sEnc_REQUIRED_BYTES_FOR_ACN_ENCODING + 1];
    BitStream_Init(&bitStream3, encBuff3, TC1sEnc_REQUIRED_BYTES_FOR_ACN_ENCODING);

    ret = TC1sEnc_ACN_Encode(&input3, &bitStream3, &errCode, TRUE);
    if(!ret) {
        printf("TC1s Encoding error: %d\n", ret);
        return EXIT_FAILURE;
    }

    printf("TC1s Encoding successful\n");

    BitStream_AttachBuffer(&bitStream3, encBuff3, TC1sEnc_REQUIRED_BYTES_FOR_ACN_ENCODING);

    TC1sDec output3;
    ret = TC1sDec_ACN_Decode(&output3, &bitStream3, &errCode);
    if(!ret) {
        printf("TC1s Decoding error: %d\n", ret);
        return EXIT_FAILURE;
    }

    printf("TC1s Decoding successful\n");

    printf("output.nCount = %d\n", output3.elements.nCount);
    printf("output.arr[0].a = %d\n", output3.elements.arr[0].a);
    printf("output.arr[0].b = %f\n", output3.elements.arr[0].b);
    printf("output.arr[1].a = %d\n", output3.elements.arr[1].a);
    printf("output.arr[1].b = %f\n", output3.elements.arr[1].b);
}
