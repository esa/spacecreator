#ifdef __unix__
#include <stdio.h>
#include <assert.h>
#endif

#include <string.h>

#include "C_ASN1_Types.h"

#ifdef __NEED_UINT40_UPER
int Encode_UPER_UINT40(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT40 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT40_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT40 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT40_ACN
int Encode_ACN_UINT40(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT40 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT40_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT40 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT40_NATIVE
int Encode_NATIVE_UINT40(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT40 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT40) );
    return sizeof(asn1SccUINT40);
}
#endif

#ifdef __NEED_UINT40_UPER
int Decode_UPER_UINT40(asn1SccUINT40 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT40_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT40 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT40_ACN
int Decode_ACN_UINT40(asn1SccUINT40 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT40_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT40 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT40_NATIVE
int Decode_NATIVE_UINT40(asn1SccUINT40 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT40 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT32_UPER
int Encode_UPER_UINT32(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT32 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT32_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT32 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT32_ACN
int Encode_ACN_UINT32(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT32 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT32_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT32 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT32_NATIVE
int Encode_NATIVE_UINT32(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT32 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT32) );
    return sizeof(asn1SccUINT32);
}
#endif

#ifdef __NEED_UINT32_UPER
int Decode_UPER_UINT32(asn1SccUINT32 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT32_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT32 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT32_ACN
int Decode_ACN_UINT32(asn1SccUINT32 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT32_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT32 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT32_NATIVE
int Decode_NATIVE_UINT32(asn1SccUINT32 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT32 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT24_UPER
int Encode_UPER_UINT24(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT24 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT24_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT24 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT24_ACN
int Encode_ACN_UINT24(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT24 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT24_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT24 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT24_NATIVE
int Encode_NATIVE_UINT24(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT24 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT24) );
    return sizeof(asn1SccUINT24);
}
#endif

#ifdef __NEED_UINT24_UPER
int Decode_UPER_UINT24(asn1SccUINT24 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT24_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT24 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT24_ACN
int Decode_ACN_UINT24(asn1SccUINT24 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT24_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT24 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT24_NATIVE
int Decode_NATIVE_UINT24(asn1SccUINT24 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT24 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT16_UPER
int Encode_UPER_UINT16(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT16 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT16_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT16 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT16_ACN
int Encode_ACN_UINT16(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT16 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT16_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT16 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT16_NATIVE
int Encode_NATIVE_UINT16(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT16 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT16) );
    return sizeof(asn1SccUINT16);
}
#endif

#ifdef __NEED_UINT16_UPER
int Decode_UPER_UINT16(asn1SccUINT16 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT16_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT16 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT16_ACN
int Decode_ACN_UINT16(asn1SccUINT16 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT16_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT16 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT16_NATIVE
int Decode_NATIVE_UINT16(asn1SccUINT16 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT16 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT14_UPER
int Encode_UPER_UINT14(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT14 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT14_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT14 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT14_ACN
int Encode_ACN_UINT14(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT14 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT14_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT14 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT14_NATIVE
int Encode_NATIVE_UINT14(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT14 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT14) );
    return sizeof(asn1SccUINT14);
}
#endif

#ifdef __NEED_UINT14_UPER
int Decode_UPER_UINT14(asn1SccUINT14 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT14_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT14 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT14_ACN
int Decode_ACN_UINT14(asn1SccUINT14 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT14_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT14 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT14_NATIVE
int Decode_NATIVE_UINT14(asn1SccUINT14 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT14 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT12_UPER
int Encode_UPER_UINT12(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT12 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT12_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT12 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT12_ACN
int Encode_ACN_UINT12(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT12 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT12_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT12 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT12_NATIVE
int Encode_NATIVE_UINT12(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT12 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT12) );
    return sizeof(asn1SccUINT12);
}
#endif

#ifdef __NEED_UINT12_UPER
int Decode_UPER_UINT12(asn1SccUINT12 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT12_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT12 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT12_ACN
int Decode_ACN_UINT12(asn1SccUINT12 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT12_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT12 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT12_NATIVE
int Decode_NATIVE_UINT12(asn1SccUINT12 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT12 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT11_UPER
int Encode_UPER_UINT11(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT11 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT11_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT11 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT11_ACN
int Encode_ACN_UINT11(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT11 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT11_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT11 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT11_NATIVE
int Encode_NATIVE_UINT11(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT11 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT11) );
    return sizeof(asn1SccUINT11);
}
#endif

#ifdef __NEED_UINT11_UPER
int Decode_UPER_UINT11(asn1SccUINT11 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT11_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT11 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT11_ACN
int Decode_ACN_UINT11(asn1SccUINT11 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT11_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT11 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT11_NATIVE
int Decode_NATIVE_UINT11(asn1SccUINT11 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT11 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT10_UPER
int Encode_UPER_UINT10(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT10 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT10_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT10 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT10_ACN
int Encode_ACN_UINT10(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT10 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT10_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT10 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT10_NATIVE
int Encode_NATIVE_UINT10(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT10 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT10) );
    return sizeof(asn1SccUINT10);
}
#endif

#ifdef __NEED_UINT10_UPER
int Decode_UPER_UINT10(asn1SccUINT10 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT10_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT10 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT10_ACN
int Decode_ACN_UINT10(asn1SccUINT10 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT10_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT10 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT10_NATIVE
int Decode_NATIVE_UINT10(asn1SccUINT10 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT10 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT8_UPER
int Encode_UPER_UINT8(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT8 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT8_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT8 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT8_ACN
int Encode_ACN_UINT8(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT8 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT8_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT8 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT8_NATIVE
int Encode_NATIVE_UINT8(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT8 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT8) );
    return sizeof(asn1SccUINT8);
}
#endif

#ifdef __NEED_UINT8_UPER
int Decode_UPER_UINT8(asn1SccUINT8 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT8_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT8 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT8_ACN
int Decode_ACN_UINT8(asn1SccUINT8 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT8_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT8 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT8_NATIVE
int Decode_NATIVE_UINT8(asn1SccUINT8 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT8 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT7_UPER
int Encode_UPER_UINT7(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT7 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT7_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT7 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT7_ACN
int Encode_ACN_UINT7(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT7 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT7_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT7 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT7_NATIVE
int Encode_NATIVE_UINT7(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT7 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT7) );
    return sizeof(asn1SccUINT7);
}
#endif

#ifdef __NEED_UINT7_UPER
int Decode_UPER_UINT7(asn1SccUINT7 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT7_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT7 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT7_ACN
int Decode_ACN_UINT7(asn1SccUINT7 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT7_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT7 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT7_NATIVE
int Decode_NATIVE_UINT7(asn1SccUINT7 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT7 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT6_UPER
int Encode_UPER_UINT6(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT6 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT6_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT6 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT6_ACN
int Encode_ACN_UINT6(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT6 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT6_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT6 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT6_NATIVE
int Encode_NATIVE_UINT6(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT6 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT6) );
    return sizeof(asn1SccUINT6);
}
#endif

#ifdef __NEED_UINT6_UPER
int Decode_UPER_UINT6(asn1SccUINT6 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT6_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT6 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT6_ACN
int Decode_ACN_UINT6(asn1SccUINT6 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT6_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT6 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT6_NATIVE
int Decode_NATIVE_UINT6(asn1SccUINT6 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT6 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT5_UPER
int Encode_UPER_UINT5(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT5 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT5_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT5 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT5_ACN
int Encode_ACN_UINT5(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT5 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT5_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT5 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT5_NATIVE
int Encode_NATIVE_UINT5(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT5 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT5) );
    return sizeof(asn1SccUINT5);
}
#endif

#ifdef __NEED_UINT5_UPER
int Decode_UPER_UINT5(asn1SccUINT5 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT5_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT5 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT5_ACN
int Decode_ACN_UINT5(asn1SccUINT5 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT5_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT5 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT5_NATIVE
int Decode_NATIVE_UINT5(asn1SccUINT5 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT5 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT4_UPER
int Encode_UPER_UINT4(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT4 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT4_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT4 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT4_ACN
int Encode_ACN_UINT4(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT4 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT4_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT4 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT4_NATIVE
int Encode_NATIVE_UINT4(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT4 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT4) );
    return sizeof(asn1SccUINT4);
}
#endif

#ifdef __NEED_UINT4_UPER
int Decode_UPER_UINT4(asn1SccUINT4 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT4_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT4 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT4_ACN
int Decode_ACN_UINT4(asn1SccUINT4 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT4_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT4 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT4_NATIVE
int Decode_NATIVE_UINT4(asn1SccUINT4 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT4 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT3_UPER
int Encode_UPER_UINT3(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT3 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT3_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT3 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT3_ACN
int Encode_ACN_UINT3(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT3 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT3_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT3 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT3_NATIVE
int Encode_NATIVE_UINT3(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT3 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT3) );
    return sizeof(asn1SccUINT3);
}
#endif

#ifdef __NEED_UINT3_UPER
int Decode_UPER_UINT3(asn1SccUINT3 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT3_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT3 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT3_ACN
int Decode_ACN_UINT3(asn1SccUINT3 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT3_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT3 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT3_NATIVE
int Decode_NATIVE_UINT3(asn1SccUINT3 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT3 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT2_UPER
int Encode_UPER_UINT2(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT2 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT2_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT2 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT2_ACN
int Encode_ACN_UINT2(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT2 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT2_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT2 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT2_NATIVE
int Encode_NATIVE_UINT2(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT2 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT2) );
    return sizeof(asn1SccUINT2);
}
#endif

#ifdef __NEED_UINT2_UPER
int Decode_UPER_UINT2(asn1SccUINT2 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT2_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT2 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT2_ACN
int Decode_ACN_UINT2(asn1SccUINT2 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT2_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT2 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT2_NATIVE
int Decode_NATIVE_UINT2(asn1SccUINT2 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT2 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_UINT1_UPER
int Encode_UPER_UINT1(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT1 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT1_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT1 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT1_ACN
int Encode_ACN_UINT1(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT1 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccUINT1_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode UINT1 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_UINT1_NATIVE
int Encode_NATIVE_UINT1(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT1 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccUINT1) );
    return sizeof(asn1SccUINT1);
}
#endif

#ifdef __NEED_UINT1_UPER
int Decode_UPER_UINT1(asn1SccUINT1 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT1_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT1 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT1_ACN
int Decode_ACN_UINT1(asn1SccUINT1 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccUINT1_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode UINT1 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_UINT1_NATIVE
int Decode_NATIVE_UINT1(asn1SccUINT1 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccUINT1 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_Bit_UPER
int Encode_UPER_T_Bit(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Bit *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_Bit_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-Bit (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_Bit_ACN
int Encode_ACN_T_Bit(void *pBuffer, size_t iMaxBufferSize, asn1SccT_Bit *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_Bit_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-Bit (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_Bit_NATIVE
int Encode_NATIVE_T_Bit(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Bit *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_Bit) );
    return sizeof(asn1SccT_Bit);
}
#endif

#ifdef __NEED_T_Bit_UPER
int Decode_UPER_T_Bit(asn1SccT_Bit *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_Bit_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-Bit (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_Bit_ACN
int Decode_ACN_T_Bit(asn1SccT_Bit *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_Bit_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-Bit (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_Bit_NATIVE
int Decode_NATIVE_T_Bit(asn1SccT_Bit *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_Bit *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_INT16_UPER
int Encode_UPER_INT16(void *pBuffer, size_t iMaxBufferSize, const asn1SccINT16 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccINT16_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode INT16 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_INT16_ACN
int Encode_ACN_INT16(void *pBuffer, size_t iMaxBufferSize, asn1SccINT16 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccINT16_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode INT16 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_INT16_NATIVE
int Encode_NATIVE_INT16(void *pBuffer, size_t iMaxBufferSize, const asn1SccINT16 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccINT16) );
    return sizeof(asn1SccINT16);
}
#endif

#ifdef __NEED_INT16_UPER
int Decode_UPER_INT16(asn1SccINT16 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccINT16_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode INT16 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_INT16_ACN
int Decode_ACN_INT16(asn1SccINT16 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccINT16_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode INT16 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_INT16_NATIVE
int Decode_NATIVE_INT16(asn1SccINT16 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccINT16 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_INT32_UPER
int Encode_UPER_INT32(void *pBuffer, size_t iMaxBufferSize, const asn1SccINT32 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccINT32_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode INT32 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_INT32_ACN
int Encode_ACN_INT32(void *pBuffer, size_t iMaxBufferSize, asn1SccINT32 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccINT32_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode INT32 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_INT32_NATIVE
int Encode_NATIVE_INT32(void *pBuffer, size_t iMaxBufferSize, const asn1SccINT32 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccINT32) );
    return sizeof(asn1SccINT32);
}
#endif

#ifdef __NEED_INT32_UPER
int Decode_UPER_INT32(asn1SccINT32 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccINT32_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode INT32 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_INT32_ACN
int Decode_ACN_INT32(asn1SccINT32 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccINT32_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode INT32 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_INT32_NATIVE
int Decode_NATIVE_INT32(asn1SccINT32 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccINT32 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_INT24_UPER
int Encode_UPER_INT24(void *pBuffer, size_t iMaxBufferSize, const asn1SccINT24 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccINT24_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode INT24 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_INT24_ACN
int Encode_ACN_INT24(void *pBuffer, size_t iMaxBufferSize, asn1SccINT24 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccINT24_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode INT24 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_INT24_NATIVE
int Encode_NATIVE_INT24(void *pBuffer, size_t iMaxBufferSize, const asn1SccINT24 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccINT24) );
    return sizeof(asn1SccINT24);
}
#endif

#ifdef __NEED_INT24_UPER
int Decode_UPER_INT24(asn1SccINT24 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccINT24_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode INT24 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_INT24_ACN
int Decode_ACN_INT24(asn1SccINT24 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccINT24_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode INT24 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_INT24_NATIVE
int Decode_NATIVE_INT24(asn1SccINT24 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccINT24 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_OCTET_UPER
int Encode_UPER_T_OCTET(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_OCTET *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_OCTET_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-OCTET (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_OCTET_ACN
int Encode_ACN_T_OCTET(void *pBuffer, size_t iMaxBufferSize, asn1SccT_OCTET *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_OCTET_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-OCTET (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_OCTET_NATIVE
int Encode_NATIVE_T_OCTET(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_OCTET *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_OCTET) );
    return sizeof(asn1SccT_OCTET);
}
#endif

#ifdef __NEED_T_OCTET_UPER
int Decode_UPER_T_OCTET(asn1SccT_OCTET *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_OCTET_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-OCTET (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_OCTET_ACN
int Decode_ACN_T_OCTET(asn1SccT_OCTET *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_OCTET_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-OCTET (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_OCTET_NATIVE
int Decode_NATIVE_T_OCTET(asn1SccT_OCTET *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_OCTET *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_OnOffState_UPER
int Encode_UPER_T_OnOffState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_OnOffState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_OnOffState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-OnOffState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_OnOffState_ACN
int Encode_ACN_T_OnOffState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_OnOffState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_OnOffState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-OnOffState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_OnOffState_NATIVE
int Encode_NATIVE_T_OnOffState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_OnOffState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_OnOffState) );
    return sizeof(asn1SccT_OnOffState);
}
#endif

#ifdef __NEED_T_OnOffState_UPER
int Decode_UPER_T_OnOffState(asn1SccT_OnOffState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_OnOffState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-OnOffState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_OnOffState_ACN
int Decode_ACN_T_OnOffState(asn1SccT_OnOffState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_OnOffState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-OnOffState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_OnOffState_NATIVE
int Decode_NATIVE_T_OnOffState(asn1SccT_OnOffState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_OnOffState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_NominalRedundantState_UPER
int Encode_UPER_T_NominalRedundantState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_NominalRedundantState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_NominalRedundantState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-NominalRedundantState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_NominalRedundantState_ACN
int Encode_ACN_T_NominalRedundantState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_NominalRedundantState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_NominalRedundantState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-NominalRedundantState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_NominalRedundantState_NATIVE
int Encode_NATIVE_T_NominalRedundantState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_NominalRedundantState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_NominalRedundantState) );
    return sizeof(asn1SccT_NominalRedundantState);
}
#endif

#ifdef __NEED_T_NominalRedundantState_UPER
int Decode_UPER_T_NominalRedundantState(asn1SccT_NominalRedundantState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_NominalRedundantState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-NominalRedundantState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_NominalRedundantState_ACN
int Decode_ACN_T_NominalRedundantState(asn1SccT_NominalRedundantState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_NominalRedundantState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-NominalRedundantState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_NominalRedundantState_NATIVE
int Decode_NATIVE_T_NominalRedundantState(asn1SccT_NominalRedundantState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_NominalRedundantState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_ConnectionState_UPER
int Encode_UPER_T_ConnectionState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ConnectionState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_ConnectionState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-ConnectionState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_ConnectionState_ACN
int Encode_ACN_T_ConnectionState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_ConnectionState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_ConnectionState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-ConnectionState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_ConnectionState_NATIVE
int Encode_NATIVE_T_ConnectionState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ConnectionState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_ConnectionState) );
    return sizeof(asn1SccT_ConnectionState);
}
#endif

#ifdef __NEED_T_ConnectionState_UPER
int Decode_UPER_T_ConnectionState(asn1SccT_ConnectionState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_ConnectionState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-ConnectionState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_ConnectionState_ACN
int Decode_ACN_T_ConnectionState(asn1SccT_ConnectionState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_ConnectionState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-ConnectionState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_ConnectionState_NATIVE
int Decode_NATIVE_T_ConnectionState(asn1SccT_ConnectionState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_ConnectionState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_SwitchState_UPER
int Encode_UPER_T_SwitchState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_SwitchState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_SwitchState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-SwitchState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_SwitchState_ACN
int Encode_ACN_T_SwitchState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_SwitchState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_SwitchState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-SwitchState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_SwitchState_NATIVE
int Encode_NATIVE_T_SwitchState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_SwitchState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_SwitchState) );
    return sizeof(asn1SccT_SwitchState);
}
#endif

#ifdef __NEED_T_SwitchState_UPER
int Decode_UPER_T_SwitchState(asn1SccT_SwitchState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_SwitchState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-SwitchState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_SwitchState_ACN
int Decode_ACN_T_SwitchState(asn1SccT_SwitchState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_SwitchState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-SwitchState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_SwitchState_NATIVE
int Decode_NATIVE_T_SwitchState(asn1SccT_SwitchState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_SwitchState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_FaultState_UPER
int Encode_UPER_T_FaultState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_FaultState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_FaultState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-FaultState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_FaultState_ACN
int Encode_ACN_T_FaultState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_FaultState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_FaultState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-FaultState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_FaultState_NATIVE
int Encode_NATIVE_T_FaultState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_FaultState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_FaultState) );
    return sizeof(asn1SccT_FaultState);
}
#endif

#ifdef __NEED_T_FaultState_UPER
int Decode_UPER_T_FaultState(asn1SccT_FaultState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_FaultState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-FaultState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_FaultState_ACN
int Decode_ACN_T_FaultState(asn1SccT_FaultState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_FaultState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-FaultState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_FaultState_NATIVE
int Decode_NATIVE_T_FaultState(asn1SccT_FaultState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_FaultState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_SelectionState_UPER
int Encode_UPER_T_SelectionState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_SelectionState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_SelectionState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-SelectionState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_SelectionState_ACN
int Encode_ACN_T_SelectionState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_SelectionState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_SelectionState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-SelectionState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_SelectionState_NATIVE
int Encode_NATIVE_T_SelectionState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_SelectionState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_SelectionState) );
    return sizeof(asn1SccT_SelectionState);
}
#endif

#ifdef __NEED_T_SelectionState_UPER
int Decode_UPER_T_SelectionState(asn1SccT_SelectionState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_SelectionState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-SelectionState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_SelectionState_ACN
int Decode_ACN_T_SelectionState(asn1SccT_SelectionState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_SelectionState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-SelectionState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_SelectionState_NATIVE
int Decode_NATIVE_T_SelectionState(asn1SccT_SelectionState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_SelectionState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_PresenceState_UPER
int Encode_UPER_T_PresenceState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_PresenceState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_PresenceState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-PresenceState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_PresenceState_ACN
int Encode_ACN_T_PresenceState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_PresenceState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_PresenceState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-PresenceState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_PresenceState_NATIVE
int Encode_NATIVE_T_PresenceState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_PresenceState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_PresenceState) );
    return sizeof(asn1SccT_PresenceState);
}
#endif

#ifdef __NEED_T_PresenceState_UPER
int Decode_UPER_T_PresenceState(asn1SccT_PresenceState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_PresenceState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-PresenceState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_PresenceState_ACN
int Decode_ACN_T_PresenceState(asn1SccT_PresenceState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_PresenceState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-PresenceState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_PresenceState_NATIVE
int Decode_NATIVE_T_PresenceState(asn1SccT_PresenceState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_PresenceState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_ForwardBackwardState_UPER
int Encode_UPER_T_ForwardBackwardState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ForwardBackwardState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_ForwardBackwardState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-ForwardBackwardState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_ForwardBackwardState_ACN
int Encode_ACN_T_ForwardBackwardState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_ForwardBackwardState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_ForwardBackwardState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-ForwardBackwardState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_ForwardBackwardState_NATIVE
int Encode_NATIVE_T_ForwardBackwardState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ForwardBackwardState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_ForwardBackwardState) );
    return sizeof(asn1SccT_ForwardBackwardState);
}
#endif

#ifdef __NEED_T_ForwardBackwardState_UPER
int Decode_UPER_T_ForwardBackwardState(asn1SccT_ForwardBackwardState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_ForwardBackwardState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-ForwardBackwardState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_ForwardBackwardState_ACN
int Decode_ACN_T_ForwardBackwardState(asn1SccT_ForwardBackwardState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_ForwardBackwardState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-ForwardBackwardState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_ForwardBackwardState_NATIVE
int Decode_NATIVE_T_ForwardBackwardState(asn1SccT_ForwardBackwardState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_ForwardBackwardState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_LeftRightState_UPER
int Encode_UPER_T_LeftRightState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_LeftRightState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_LeftRightState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-LeftRightState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_LeftRightState_ACN
int Encode_ACN_T_LeftRightState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_LeftRightState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_LeftRightState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-LeftRightState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_LeftRightState_NATIVE
int Encode_NATIVE_T_LeftRightState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_LeftRightState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_LeftRightState) );
    return sizeof(asn1SccT_LeftRightState);
}
#endif

#ifdef __NEED_T_LeftRightState_UPER
int Decode_UPER_T_LeftRightState(asn1SccT_LeftRightState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_LeftRightState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-LeftRightState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_LeftRightState_ACN
int Decode_ACN_T_LeftRightState(asn1SccT_LeftRightState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_LeftRightState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-LeftRightState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_LeftRightState_NATIVE
int Decode_NATIVE_T_LeftRightState(asn1SccT_LeftRightState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_LeftRightState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_UsedUnusedState_UPER
int Encode_UPER_T_UsedUnusedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UsedUnusedState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UsedUnusedState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UsedUnusedState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UsedUnusedState_ACN
int Encode_ACN_T_UsedUnusedState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UsedUnusedState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UsedUnusedState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UsedUnusedState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UsedUnusedState_NATIVE
int Encode_NATIVE_T_UsedUnusedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UsedUnusedState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_UsedUnusedState) );
    return sizeof(asn1SccT_UsedUnusedState);
}
#endif

#ifdef __NEED_T_UsedUnusedState_UPER
int Decode_UPER_T_UsedUnusedState(asn1SccT_UsedUnusedState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UsedUnusedState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UsedUnusedState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UsedUnusedState_ACN
int Decode_ACN_T_UsedUnusedState(asn1SccT_UsedUnusedState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UsedUnusedState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UsedUnusedState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UsedUnusedState_NATIVE
int Decode_NATIVE_T_UsedUnusedState(asn1SccT_UsedUnusedState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_UsedUnusedState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_MovingStillState_UPER
int Encode_UPER_T_MovingStillState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_MovingStillState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_MovingStillState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-MovingStillState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_MovingStillState_ACN
int Encode_ACN_T_MovingStillState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_MovingStillState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_MovingStillState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-MovingStillState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_MovingStillState_NATIVE
int Encode_NATIVE_T_MovingStillState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_MovingStillState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_MovingStillState) );
    return sizeof(asn1SccT_MovingStillState);
}
#endif

#ifdef __NEED_T_MovingStillState_UPER
int Decode_UPER_T_MovingStillState(asn1SccT_MovingStillState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_MovingStillState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-MovingStillState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_MovingStillState_ACN
int Decode_ACN_T_MovingStillState(asn1SccT_MovingStillState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_MovingStillState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-MovingStillState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_MovingStillState_NATIVE
int Decode_NATIVE_T_MovingStillState(asn1SccT_MovingStillState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_MovingStillState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_PlusMinusState_UPER
int Encode_UPER_T_PlusMinusState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_PlusMinusState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_PlusMinusState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-PlusMinusState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_PlusMinusState_ACN
int Encode_ACN_T_PlusMinusState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_PlusMinusState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_PlusMinusState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-PlusMinusState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_PlusMinusState_NATIVE
int Encode_NATIVE_T_PlusMinusState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_PlusMinusState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_PlusMinusState) );
    return sizeof(asn1SccT_PlusMinusState);
}
#endif

#ifdef __NEED_T_PlusMinusState_UPER
int Decode_UPER_T_PlusMinusState(asn1SccT_PlusMinusState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_PlusMinusState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-PlusMinusState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_PlusMinusState_ACN
int Decode_ACN_T_PlusMinusState(asn1SccT_PlusMinusState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_PlusMinusState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-PlusMinusState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_PlusMinusState_NATIVE
int Decode_NATIVE_T_PlusMinusState(asn1SccT_PlusMinusState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_PlusMinusState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_ArmedDisarmedState_UPER
int Encode_UPER_T_ArmedDisarmedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ArmedDisarmedState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_ArmedDisarmedState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-ArmedDisarmedState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_ArmedDisarmedState_ACN
int Encode_ACN_T_ArmedDisarmedState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_ArmedDisarmedState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_ArmedDisarmedState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-ArmedDisarmedState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_ArmedDisarmedState_NATIVE
int Encode_NATIVE_T_ArmedDisarmedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ArmedDisarmedState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_ArmedDisarmedState) );
    return sizeof(asn1SccT_ArmedDisarmedState);
}
#endif

#ifdef __NEED_T_ArmedDisarmedState_UPER
int Decode_UPER_T_ArmedDisarmedState(asn1SccT_ArmedDisarmedState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_ArmedDisarmedState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-ArmedDisarmedState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_ArmedDisarmedState_ACN
int Decode_ACN_T_ArmedDisarmedState(asn1SccT_ArmedDisarmedState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_ArmedDisarmedState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-ArmedDisarmedState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_ArmedDisarmedState_NATIVE
int Decode_NATIVE_T_ArmedDisarmedState(asn1SccT_ArmedDisarmedState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_ArmedDisarmedState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_ActiveInactiveState_UPER
int Encode_UPER_T_ActiveInactiveState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ActiveInactiveState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_ActiveInactiveState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-ActiveInactiveState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_ActiveInactiveState_ACN
int Encode_ACN_T_ActiveInactiveState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_ActiveInactiveState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_ActiveInactiveState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-ActiveInactiveState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_ActiveInactiveState_NATIVE
int Encode_NATIVE_T_ActiveInactiveState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ActiveInactiveState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_ActiveInactiveState) );
    return sizeof(asn1SccT_ActiveInactiveState);
}
#endif

#ifdef __NEED_T_ActiveInactiveState_UPER
int Decode_UPER_T_ActiveInactiveState(asn1SccT_ActiveInactiveState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_ActiveInactiveState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-ActiveInactiveState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_ActiveInactiveState_ACN
int Decode_ACN_T_ActiveInactiveState(asn1SccT_ActiveInactiveState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_ActiveInactiveState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-ActiveInactiveState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_ActiveInactiveState_NATIVE
int Decode_NATIVE_T_ActiveInactiveState(asn1SccT_ActiveInactiveState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_ActiveInactiveState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_EnabledInhibitedState_UPER
int Encode_UPER_T_EnabledInhibitedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_EnabledInhibitedState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_EnabledInhibitedState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-EnabledInhibitedState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_EnabledInhibitedState_ACN
int Encode_ACN_T_EnabledInhibitedState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_EnabledInhibitedState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_EnabledInhibitedState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-EnabledInhibitedState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_EnabledInhibitedState_NATIVE
int Encode_NATIVE_T_EnabledInhibitedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_EnabledInhibitedState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_EnabledInhibitedState) );
    return sizeof(asn1SccT_EnabledInhibitedState);
}
#endif

#ifdef __NEED_T_EnabledInhibitedState_UPER
int Decode_UPER_T_EnabledInhibitedState(asn1SccT_EnabledInhibitedState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_EnabledInhibitedState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-EnabledInhibitedState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_EnabledInhibitedState_ACN
int Decode_ACN_T_EnabledInhibitedState(asn1SccT_EnabledInhibitedState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_EnabledInhibitedState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-EnabledInhibitedState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_EnabledInhibitedState_NATIVE
int Decode_NATIVE_T_EnabledInhibitedState(asn1SccT_EnabledInhibitedState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_EnabledInhibitedState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_FlagState_UPER
int Encode_UPER_T_FlagState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_FlagState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_FlagState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-FlagState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_FlagState_ACN
int Encode_ACN_T_FlagState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_FlagState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_FlagState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-FlagState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_FlagState_NATIVE
int Encode_NATIVE_T_FlagState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_FlagState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_FlagState) );
    return sizeof(asn1SccT_FlagState);
}
#endif

#ifdef __NEED_T_FlagState_UPER
int Decode_UPER_T_FlagState(asn1SccT_FlagState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_FlagState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-FlagState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_FlagState_ACN
int Decode_ACN_T_FlagState(asn1SccT_FlagState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_FlagState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-FlagState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_FlagState_NATIVE
int Decode_NATIVE_T_FlagState(asn1SccT_FlagState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_FlagState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_SuccessFailState_UPER
int Encode_UPER_T_SuccessFailState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_SuccessFailState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_SuccessFailState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-SuccessFailState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_SuccessFailState_ACN
int Encode_ACN_T_SuccessFailState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_SuccessFailState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_SuccessFailState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-SuccessFailState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_SuccessFailState_NATIVE
int Encode_NATIVE_T_SuccessFailState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_SuccessFailState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_SuccessFailState) );
    return sizeof(asn1SccT_SuccessFailState);
}
#endif

#ifdef __NEED_T_SuccessFailState_UPER
int Decode_UPER_T_SuccessFailState(asn1SccT_SuccessFailState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_SuccessFailState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-SuccessFailState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_SuccessFailState_ACN
int Decode_ACN_T_SuccessFailState(asn1SccT_SuccessFailState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_SuccessFailState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-SuccessFailState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_SuccessFailState_NATIVE
int Decode_NATIVE_T_SuccessFailState(asn1SccT_SuccessFailState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_SuccessFailState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_HighLowState_UPER
int Encode_UPER_T_HighLowState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_HighLowState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_HighLowState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-HighLowState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_HighLowState_ACN
int Encode_ACN_T_HighLowState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_HighLowState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_HighLowState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-HighLowState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_HighLowState_NATIVE
int Encode_NATIVE_T_HighLowState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_HighLowState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_HighLowState) );
    return sizeof(asn1SccT_HighLowState);
}
#endif

#ifdef __NEED_T_HighLowState_UPER
int Decode_UPER_T_HighLowState(asn1SccT_HighLowState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_HighLowState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-HighLowState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_HighLowState_ACN
int Decode_ACN_T_HighLowState(asn1SccT_HighLowState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_HighLowState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-HighLowState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_HighLowState_NATIVE
int Decode_NATIVE_T_HighLowState(asn1SccT_HighLowState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_HighLowState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_OverrideState_UPER
int Encode_UPER_T_OverrideState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_OverrideState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_OverrideState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-OverrideState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_OverrideState_ACN
int Encode_ACN_T_OverrideState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_OverrideState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_OverrideState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-OverrideState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_OverrideState_NATIVE
int Encode_NATIVE_T_OverrideState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_OverrideState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_OverrideState) );
    return sizeof(asn1SccT_OverrideState);
}
#endif

#ifdef __NEED_T_OverrideState_UPER
int Decode_UPER_T_OverrideState(asn1SccT_OverrideState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_OverrideState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-OverrideState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_OverrideState_ACN
int Decode_ACN_T_OverrideState(asn1SccT_OverrideState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_OverrideState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-OverrideState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_OverrideState_NATIVE
int Decode_NATIVE_T_OverrideState(asn1SccT_OverrideState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_OverrideState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_LockedUnlockedState_UPER
int Encode_UPER_T_LockedUnlockedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_LockedUnlockedState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_LockedUnlockedState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-LockedUnlockedState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_LockedUnlockedState_ACN
int Encode_ACN_T_LockedUnlockedState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_LockedUnlockedState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_LockedUnlockedState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-LockedUnlockedState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_LockedUnlockedState_NATIVE
int Encode_NATIVE_T_LockedUnlockedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_LockedUnlockedState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_LockedUnlockedState) );
    return sizeof(asn1SccT_LockedUnlockedState);
}
#endif

#ifdef __NEED_T_LockedUnlockedState_UPER
int Decode_UPER_T_LockedUnlockedState(asn1SccT_LockedUnlockedState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_LockedUnlockedState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-LockedUnlockedState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_LockedUnlockedState_ACN
int Decode_ACN_T_LockedUnlockedState(asn1SccT_LockedUnlockedState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_LockedUnlockedState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-LockedUnlockedState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_LockedUnlockedState_NATIVE
int Decode_NATIVE_T_LockedUnlockedState(asn1SccT_LockedUnlockedState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_LockedUnlockedState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_WarningState_UPER
int Encode_UPER_T_WarningState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_WarningState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_WarningState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-WarningState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_WarningState_ACN
int Encode_ACN_T_WarningState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_WarningState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_WarningState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-WarningState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_WarningState_NATIVE
int Encode_NATIVE_T_WarningState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_WarningState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_WarningState) );
    return sizeof(asn1SccT_WarningState);
}
#endif

#ifdef __NEED_T_WarningState_UPER
int Decode_UPER_T_WarningState(asn1SccT_WarningState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_WarningState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-WarningState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_WarningState_ACN
int Decode_ACN_T_WarningState(asn1SccT_WarningState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_WarningState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-WarningState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_WarningState_NATIVE
int Decode_NATIVE_T_WarningState(asn1SccT_WarningState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_WarningState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_ValidInvalidState_UPER
int Encode_UPER_T_ValidInvalidState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ValidInvalidState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_ValidInvalidState_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-ValidInvalidState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_ValidInvalidState_ACN
int Encode_ACN_T_ValidInvalidState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_ValidInvalidState *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_ValidInvalidState_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-ValidInvalidState (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_ValidInvalidState_NATIVE
int Encode_NATIVE_T_ValidInvalidState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ValidInvalidState *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_ValidInvalidState) );
    return sizeof(asn1SccT_ValidInvalidState);
}
#endif

#ifdef __NEED_T_ValidInvalidState_UPER
int Decode_UPER_T_ValidInvalidState(asn1SccT_ValidInvalidState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_ValidInvalidState_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-ValidInvalidState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_ValidInvalidState_ACN
int Decode_ACN_T_ValidInvalidState(asn1SccT_ValidInvalidState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_ValidInvalidState_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-ValidInvalidState (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_ValidInvalidState_NATIVE
int Decode_NATIVE_T_ValidInvalidState(asn1SccT_ValidInvalidState *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_ValidInvalidState *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_Header_UPER
int Encode_UPER_T_UART_DLC_Header(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_Header *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UART_DLC_Header_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UART-DLC-Header (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UART_DLC_Header_ACN
int Encode_ACN_T_UART_DLC_Header(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UART_DLC_Header *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UART_DLC_Header_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UART-DLC-Header (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UART_DLC_Header_NATIVE
int Encode_NATIVE_T_UART_DLC_Header(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_Header *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_UART_DLC_Header) );
    return sizeof(asn1SccT_UART_DLC_Header);
}
#endif

#ifdef __NEED_T_UART_DLC_Header_UPER
int Decode_UPER_T_UART_DLC_Header(asn1SccT_UART_DLC_Header *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UART_DLC_Header_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UART-DLC-Header (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_Header_ACN
int Decode_ACN_T_UART_DLC_Header(asn1SccT_UART_DLC_Header *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UART_DLC_Header_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UART-DLC-Header (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_Header_NATIVE
int Decode_NATIVE_T_UART_DLC_Header(asn1SccT_UART_DLC_Header *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_UART_DLC_Header *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_FrameCRC_UPER
int Encode_UPER_T_UART_DLC_FrameCRC(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_FrameCRC *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UART_DLC_FrameCRC_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UART-DLC-FrameCRC (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UART_DLC_FrameCRC_ACN
int Encode_ACN_T_UART_DLC_FrameCRC(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UART_DLC_FrameCRC *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UART_DLC_FrameCRC_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UART-DLC-FrameCRC (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UART_DLC_FrameCRC_NATIVE
int Encode_NATIVE_T_UART_DLC_FrameCRC(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_FrameCRC *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_UART_DLC_FrameCRC) );
    return sizeof(asn1SccT_UART_DLC_FrameCRC);
}
#endif

#ifdef __NEED_T_UART_DLC_FrameCRC_UPER
int Decode_UPER_T_UART_DLC_FrameCRC(asn1SccT_UART_DLC_FrameCRC *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UART_DLC_FrameCRC_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UART-DLC-FrameCRC (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_FrameCRC_ACN
int Decode_ACN_T_UART_DLC_FrameCRC(asn1SccT_UART_DLC_FrameCRC *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UART_DLC_FrameCRC_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UART-DLC-FrameCRC (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_FrameCRC_NATIVE
int Decode_NATIVE_T_UART_DLC_FrameCRC(asn1SccT_UART_DLC_FrameCRC *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_UART_DLC_FrameCRC *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_RMAPCRCTable_UPER
int Encode_UPER_T_RMAPCRCTable(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_RMAPCRCTable *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_RMAPCRCTable_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-RMAPCRCTable (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_RMAPCRCTable_ACN
int Encode_ACN_T_RMAPCRCTable(void *pBuffer, size_t iMaxBufferSize, asn1SccT_RMAPCRCTable *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_RMAPCRCTable_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-RMAPCRCTable (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_RMAPCRCTable_NATIVE
int Encode_NATIVE_T_RMAPCRCTable(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_RMAPCRCTable *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_RMAPCRCTable) );
    return sizeof(asn1SccT_RMAPCRCTable);
}
#endif

#ifdef __NEED_T_RMAPCRCTable_UPER
int Decode_UPER_T_RMAPCRCTable(asn1SccT_RMAPCRCTable *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_RMAPCRCTable_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-RMAPCRCTable (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_RMAPCRCTable_ACN
int Decode_ACN_T_RMAPCRCTable(asn1SccT_RMAPCRCTable *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_RMAPCRCTable_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-RMAPCRCTable (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_RMAPCRCTable_NATIVE
int Decode_NATIVE_T_RMAPCRCTable(asn1SccT_RMAPCRCTable *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_RMAPCRCTable *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_FrameBuffer_UPER
int Encode_UPER_T_UART_DLC_FrameBuffer(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_FrameBuffer *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UART_DLC_FrameBuffer_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UART-DLC-FrameBuffer (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UART_DLC_FrameBuffer_ACN
int Encode_ACN_T_UART_DLC_FrameBuffer(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UART_DLC_FrameBuffer *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UART_DLC_FrameBuffer_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UART-DLC-FrameBuffer (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UART_DLC_FrameBuffer_NATIVE
int Encode_NATIVE_T_UART_DLC_FrameBuffer(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_FrameBuffer *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_UART_DLC_FrameBuffer) );
    return sizeof(asn1SccT_UART_DLC_FrameBuffer);
}
#endif

#ifdef __NEED_T_UART_DLC_FrameBuffer_UPER
int Decode_UPER_T_UART_DLC_FrameBuffer(asn1SccT_UART_DLC_FrameBuffer *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UART_DLC_FrameBuffer_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UART-DLC-FrameBuffer (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_FrameBuffer_ACN
int Decode_ACN_T_UART_DLC_FrameBuffer(asn1SccT_UART_DLC_FrameBuffer *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UART_DLC_FrameBuffer_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UART-DLC-FrameBuffer (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_FrameBuffer_NATIVE
int Decode_NATIVE_T_UART_DLC_FrameBuffer(asn1SccT_UART_DLC_FrameBuffer *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_UART_DLC_FrameBuffer *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_SDU_UPER
int Encode_UPER_T_UART_DLC_SDU(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_SDU *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UART_DLC_SDU_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UART-DLC-SDU (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UART_DLC_SDU_ACN
int Encode_ACN_T_UART_DLC_SDU(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UART_DLC_SDU *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UART_DLC_SDU_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UART-DLC-SDU (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UART_DLC_SDU_NATIVE
int Encode_NATIVE_T_UART_DLC_SDU(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_SDU *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_UART_DLC_SDU) );
    return sizeof(asn1SccT_UART_DLC_SDU);
}
#endif

#ifdef __NEED_T_UART_DLC_SDU_UPER
int Decode_UPER_T_UART_DLC_SDU(asn1SccT_UART_DLC_SDU *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UART_DLC_SDU_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UART-DLC-SDU (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_SDU_ACN
int Decode_ACN_T_UART_DLC_SDU(asn1SccT_UART_DLC_SDU *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UART_DLC_SDU_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UART-DLC-SDU (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_SDU_NATIVE
int Decode_NATIVE_T_UART_DLC_SDU(asn1SccT_UART_DLC_SDU *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_UART_DLC_SDU *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_PDU_UPER
int Encode_UPER_T_UART_DLC_PDU(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_PDU *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UART_DLC_PDU_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UART-DLC-PDU (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UART_DLC_PDU_ACN
int Encode_ACN_T_UART_DLC_PDU(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UART_DLC_PDU *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UART_DLC_PDU_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UART-DLC-PDU (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UART_DLC_PDU_NATIVE
int Encode_NATIVE_T_UART_DLC_PDU(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_PDU *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_UART_DLC_PDU) );
    return sizeof(asn1SccT_UART_DLC_PDU);
}
#endif

#ifdef __NEED_T_UART_DLC_PDU_UPER
int Decode_UPER_T_UART_DLC_PDU(asn1SccT_UART_DLC_PDU *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UART_DLC_PDU_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UART-DLC-PDU (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_PDU_ACN
int Decode_ACN_T_UART_DLC_PDU(asn1SccT_UART_DLC_PDU *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UART_DLC_PDU_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UART-DLC-PDU (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_PDU_NATIVE
int Decode_NATIVE_T_UART_DLC_PDU(asn1SccT_UART_DLC_PDU *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_UART_DLC_PDU *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_RxErrorCode_UPER
int Encode_UPER_T_UART_DLC_RxErrorCode(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_RxErrorCode *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UART_DLC_RxErrorCode_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UART-DLC-RxErrorCode (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UART_DLC_RxErrorCode_ACN
int Encode_ACN_T_UART_DLC_RxErrorCode(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UART_DLC_RxErrorCode *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UART_DLC_RxErrorCode_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UART-DLC-RxErrorCode (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UART_DLC_RxErrorCode_NATIVE
int Encode_NATIVE_T_UART_DLC_RxErrorCode(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_RxErrorCode *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_UART_DLC_RxErrorCode) );
    return sizeof(asn1SccT_UART_DLC_RxErrorCode);
}
#endif

#ifdef __NEED_T_UART_DLC_RxErrorCode_UPER
int Decode_UPER_T_UART_DLC_RxErrorCode(asn1SccT_UART_DLC_RxErrorCode *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UART_DLC_RxErrorCode_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UART-DLC-RxErrorCode (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_RxErrorCode_ACN
int Decode_ACN_T_UART_DLC_RxErrorCode(asn1SccT_UART_DLC_RxErrorCode *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UART_DLC_RxErrorCode_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UART-DLC-RxErrorCode (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UART_DLC_RxErrorCode_NATIVE
int Decode_NATIVE_T_UART_DLC_RxErrorCode(asn1SccT_UART_DLC_RxErrorCode *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_UART_DLC_RxErrorCode *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_Int32_UPER
int Encode_UPER_T_Int32(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Int32 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_Int32_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-Int32 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_Int32_ACN
int Encode_ACN_T_Int32(void *pBuffer, size_t iMaxBufferSize, asn1SccT_Int32 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_Int32_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-Int32 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_Int32_NATIVE
int Encode_NATIVE_T_Int32(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Int32 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_Int32) );
    return sizeof(asn1SccT_Int32);
}
#endif

#ifdef __NEED_T_Int32_UPER
int Decode_UPER_T_Int32(asn1SccT_Int32 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_Int32_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-Int32 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_Int32_ACN
int Decode_ACN_T_Int32(asn1SccT_Int32 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_Int32_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-Int32 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_Int32_NATIVE
int Decode_NATIVE_T_Int32(asn1SccT_Int32 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_Int32 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_UInt32_UPER
int Encode_UPER_T_UInt32(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UInt32 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UInt32_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UInt32 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UInt32_ACN
int Encode_ACN_T_UInt32(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UInt32 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UInt32_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UInt32 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UInt32_NATIVE
int Encode_NATIVE_T_UInt32(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UInt32 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_UInt32) );
    return sizeof(asn1SccT_UInt32);
}
#endif

#ifdef __NEED_T_UInt32_UPER
int Decode_UPER_T_UInt32(asn1SccT_UInt32 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UInt32_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UInt32 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UInt32_ACN
int Decode_ACN_T_UInt32(asn1SccT_UInt32 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UInt32_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UInt32 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UInt32_NATIVE
int Decode_NATIVE_T_UInt32(asn1SccT_UInt32 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_UInt32 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_Int8_UPER
int Encode_UPER_T_Int8(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Int8 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_Int8_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-Int8 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_Int8_ACN
int Encode_ACN_T_Int8(void *pBuffer, size_t iMaxBufferSize, asn1SccT_Int8 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_Int8_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-Int8 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_Int8_NATIVE
int Encode_NATIVE_T_Int8(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Int8 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_Int8) );
    return sizeof(asn1SccT_Int8);
}
#endif

#ifdef __NEED_T_Int8_UPER
int Decode_UPER_T_Int8(asn1SccT_Int8 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_Int8_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-Int8 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_Int8_ACN
int Decode_ACN_T_Int8(asn1SccT_Int8 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_Int8_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-Int8 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_Int8_NATIVE
int Decode_NATIVE_T_Int8(asn1SccT_Int8 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_Int8 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_UInt8_UPER
int Encode_UPER_T_UInt8(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UInt8 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UInt8_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UInt8 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UInt8_ACN
int Encode_ACN_T_UInt8(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UInt8 *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_UInt8_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-UInt8 (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_UInt8_NATIVE
int Encode_NATIVE_T_UInt8(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UInt8 *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_UInt8) );
    return sizeof(asn1SccT_UInt8);
}
#endif

#ifdef __NEED_T_UInt8_UPER
int Decode_UPER_T_UInt8(asn1SccT_UInt8 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UInt8_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UInt8 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UInt8_ACN
int Decode_ACN_T_UInt8(asn1SccT_UInt8 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_UInt8_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-UInt8 (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_UInt8_NATIVE
int Decode_NATIVE_T_UInt8(asn1SccT_UInt8 *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_UInt8 *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_Boolean_UPER
int Encode_UPER_T_Boolean(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Boolean *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_Boolean_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-Boolean (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_Boolean_ACN
int Encode_ACN_T_Boolean(void *pBuffer, size_t iMaxBufferSize, asn1SccT_Boolean *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_Boolean_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-Boolean (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_Boolean_NATIVE
int Encode_NATIVE_T_Boolean(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Boolean *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_Boolean) );
    return sizeof(asn1SccT_Boolean);
}
#endif

#ifdef __NEED_T_Boolean_UPER
int Decode_UPER_T_Boolean(asn1SccT_Boolean *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_Boolean_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-Boolean (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_Boolean_ACN
int Decode_ACN_T_Boolean(asn1SccT_Boolean *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_Boolean_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-Boolean (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_Boolean_NATIVE
int Decode_NATIVE_T_Boolean(asn1SccT_Boolean *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_Boolean *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_T_Null_Record_UPER
int Encode_UPER_T_Null_Record(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Null_Record *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_Null_Record_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-Null-Record (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_Null_Record_ACN
int Encode_ACN_T_Null_Record(void *pBuffer, size_t iMaxBufferSize, asn1SccT_Null_Record *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccT_Null_Record_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode T-Null-Record (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_T_Null_Record_NATIVE
int Encode_NATIVE_T_Null_Record(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Null_Record *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccT_Null_Record) );
    return sizeof(asn1SccT_Null_Record);
}
#endif

#ifdef __NEED_T_Null_Record_UPER
int Decode_UPER_T_Null_Record(asn1SccT_Null_Record *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_Null_Record_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-Null-Record (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_Null_Record_ACN
int Decode_ACN_T_Null_Record(asn1SccT_Null_Record *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccT_Null_Record_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode T-Null-Record (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_T_Null_Record_NATIVE
int Decode_NATIVE_T_Null_Record(asn1SccT_Null_Record *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccT_Null_Record *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_MyInteger_UPER
int Encode_UPER_MyInteger(void *pBuffer, size_t iMaxBufferSize, const asn1SccMyInteger *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccMyInteger_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode MyInteger (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_MyInteger_ACN
int Encode_ACN_MyInteger(void *pBuffer, size_t iMaxBufferSize, asn1SccMyInteger *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccMyInteger_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode MyInteger (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_MyInteger_NATIVE
int Encode_NATIVE_MyInteger(void *pBuffer, size_t iMaxBufferSize, const asn1SccMyInteger *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccMyInteger) );
    return sizeof(asn1SccMyInteger);
}
#endif

#ifdef __NEED_MyInteger_UPER
int Decode_UPER_MyInteger(asn1SccMyInteger *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccMyInteger_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode MyInteger (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_MyInteger_ACN
int Decode_ACN_MyInteger(asn1SccMyInteger *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccMyInteger_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode MyInteger (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_MyInteger_NATIVE
int Decode_NATIVE_MyInteger(asn1SccMyInteger *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccMyInteger *) pBuffer;
    {
        return 0;
    }
}
#endif

#ifdef __NEED_PID_UPER
int Encode_UPER_PID(void *pBuffer, size_t iMaxBufferSize, const asn1SccPID *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccPID_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode PID (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_PID_ACN
int Encode_ACN_PID(void *pBuffer, size_t iMaxBufferSize, asn1SccPID *pSrc)
{
    (void)iMaxBufferSize;
    int errorCode;
    STATIC BitStream strm;

    BitStream_Init(&strm, pBuffer, iMaxBufferSize);
    if (asn1SccPID_ACN_Encode(pSrc, &strm, &errorCode, TRUE) == FALSE) {
#ifdef __unix__
	fprintf(stderr, "Could not encode PID (at %s, %d), errorCode was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    } else {
        return BitStream_GetLength(&strm);
    }
}
#endif

#ifdef __NEED_PID_NATIVE
int Encode_NATIVE_PID(void *pBuffer, size_t iMaxBufferSize, const asn1SccPID *pSrc)
{
    (void)iMaxBufferSize;
    memcpy(pBuffer, pSrc, sizeof(asn1SccPID) );
    return sizeof(asn1SccPID);
}
#endif

#ifdef __NEED_PID_UPER
int Decode_UPER_PID(asn1SccPID *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccPID_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode PID (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_PID_ACN
int Decode_ACN_PID(asn1SccPID *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    int errorCode;

    STATIC BitStream strm;

    BitStream_AttachBuffer(&strm, pBuffer, iBufferSize);

    if (asn1SccPID_ACN_Decode(pDst, &strm, &errorCode)) {
        /* Decoding succeeded */
        return 0;
    } else {
#ifdef __unix__
	fprintf(stderr, "Could not decode PID (at %s, %d), error code was %d\n", __FILE__, __LINE__, errorCode);
#endif
        return -1;
    }
}
#endif

#ifdef __NEED_PID_NATIVE
int Decode_NATIVE_PID(asn1SccPID *pDst, void *pBuffer, size_t iBufferSize)
{
    (void)iBufferSize;
    *pDst = *(asn1SccPID *) pBuffer;
    {
        return 0;
    }
}
#endif

