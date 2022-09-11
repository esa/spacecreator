#ifndef __C_DATAVIEW_UNIQ_H__
#define __C_DATAVIEW_UNIQ_H__

#if defined( __unix__ ) || defined( __MSP430__ )
#include <stdlib.h> /* for size_t */
#else
typedef unsigned size_t;
#endif

#ifndef STATIC
#define STATIC
#endif


#include "dataview-uniq.h" // Space certified compiler generated

#include "../../system_config.h" // Choose ASN.1 Types to use

#ifdef __NEED_UINT40_UPER
int Encode_UPER_UINT40(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT40 *pSrc);
#endif

#ifdef __NEED_UINT40_ACN
int Encode_ACN_UINT40(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT40 *pSrc);
#endif

#ifdef __NEED_UINT40_NATIVE
int Encode_NATIVE_UINT40(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT40 *pSrc);
#endif

#ifdef __NEED_UINT40_UPER
int Decode_UPER_UINT40(asn1SccUINT40 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT40_ACN
int Decode_ACN_UINT40(asn1SccUINT40 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT40_NATIVE
int Decode_NATIVE_UINT40(asn1SccUINT40 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT32_UPER
int Encode_UPER_UINT32(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT32 *pSrc);
#endif

#ifdef __NEED_UINT32_ACN
int Encode_ACN_UINT32(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT32 *pSrc);
#endif

#ifdef __NEED_UINT32_NATIVE
int Encode_NATIVE_UINT32(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT32 *pSrc);
#endif

#ifdef __NEED_UINT32_UPER
int Decode_UPER_UINT32(asn1SccUINT32 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT32_ACN
int Decode_ACN_UINT32(asn1SccUINT32 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT32_NATIVE
int Decode_NATIVE_UINT32(asn1SccUINT32 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT24_UPER
int Encode_UPER_UINT24(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT24 *pSrc);
#endif

#ifdef __NEED_UINT24_ACN
int Encode_ACN_UINT24(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT24 *pSrc);
#endif

#ifdef __NEED_UINT24_NATIVE
int Encode_NATIVE_UINT24(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT24 *pSrc);
#endif

#ifdef __NEED_UINT24_UPER
int Decode_UPER_UINT24(asn1SccUINT24 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT24_ACN
int Decode_ACN_UINT24(asn1SccUINT24 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT24_NATIVE
int Decode_NATIVE_UINT24(asn1SccUINT24 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT16_UPER
int Encode_UPER_UINT16(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT16 *pSrc);
#endif

#ifdef __NEED_UINT16_ACN
int Encode_ACN_UINT16(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT16 *pSrc);
#endif

#ifdef __NEED_UINT16_NATIVE
int Encode_NATIVE_UINT16(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT16 *pSrc);
#endif

#ifdef __NEED_UINT16_UPER
int Decode_UPER_UINT16(asn1SccUINT16 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT16_ACN
int Decode_ACN_UINT16(asn1SccUINT16 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT16_NATIVE
int Decode_NATIVE_UINT16(asn1SccUINT16 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT14_UPER
int Encode_UPER_UINT14(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT14 *pSrc);
#endif

#ifdef __NEED_UINT14_ACN
int Encode_ACN_UINT14(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT14 *pSrc);
#endif

#ifdef __NEED_UINT14_NATIVE
int Encode_NATIVE_UINT14(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT14 *pSrc);
#endif

#ifdef __NEED_UINT14_UPER
int Decode_UPER_UINT14(asn1SccUINT14 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT14_ACN
int Decode_ACN_UINT14(asn1SccUINT14 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT14_NATIVE
int Decode_NATIVE_UINT14(asn1SccUINT14 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT12_UPER
int Encode_UPER_UINT12(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT12 *pSrc);
#endif

#ifdef __NEED_UINT12_ACN
int Encode_ACN_UINT12(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT12 *pSrc);
#endif

#ifdef __NEED_UINT12_NATIVE
int Encode_NATIVE_UINT12(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT12 *pSrc);
#endif

#ifdef __NEED_UINT12_UPER
int Decode_UPER_UINT12(asn1SccUINT12 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT12_ACN
int Decode_ACN_UINT12(asn1SccUINT12 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT12_NATIVE
int Decode_NATIVE_UINT12(asn1SccUINT12 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT11_UPER
int Encode_UPER_UINT11(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT11 *pSrc);
#endif

#ifdef __NEED_UINT11_ACN
int Encode_ACN_UINT11(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT11 *pSrc);
#endif

#ifdef __NEED_UINT11_NATIVE
int Encode_NATIVE_UINT11(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT11 *pSrc);
#endif

#ifdef __NEED_UINT11_UPER
int Decode_UPER_UINT11(asn1SccUINT11 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT11_ACN
int Decode_ACN_UINT11(asn1SccUINT11 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT11_NATIVE
int Decode_NATIVE_UINT11(asn1SccUINT11 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT10_UPER
int Encode_UPER_UINT10(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT10 *pSrc);
#endif

#ifdef __NEED_UINT10_ACN
int Encode_ACN_UINT10(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT10 *pSrc);
#endif

#ifdef __NEED_UINT10_NATIVE
int Encode_NATIVE_UINT10(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT10 *pSrc);
#endif

#ifdef __NEED_UINT10_UPER
int Decode_UPER_UINT10(asn1SccUINT10 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT10_ACN
int Decode_ACN_UINT10(asn1SccUINT10 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT10_NATIVE
int Decode_NATIVE_UINT10(asn1SccUINT10 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT8_UPER
int Encode_UPER_UINT8(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT8 *pSrc);
#endif

#ifdef __NEED_UINT8_ACN
int Encode_ACN_UINT8(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT8 *pSrc);
#endif

#ifdef __NEED_UINT8_NATIVE
int Encode_NATIVE_UINT8(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT8 *pSrc);
#endif

#ifdef __NEED_UINT8_UPER
int Decode_UPER_UINT8(asn1SccUINT8 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT8_ACN
int Decode_ACN_UINT8(asn1SccUINT8 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT8_NATIVE
int Decode_NATIVE_UINT8(asn1SccUINT8 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT7_UPER
int Encode_UPER_UINT7(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT7 *pSrc);
#endif

#ifdef __NEED_UINT7_ACN
int Encode_ACN_UINT7(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT7 *pSrc);
#endif

#ifdef __NEED_UINT7_NATIVE
int Encode_NATIVE_UINT7(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT7 *pSrc);
#endif

#ifdef __NEED_UINT7_UPER
int Decode_UPER_UINT7(asn1SccUINT7 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT7_ACN
int Decode_ACN_UINT7(asn1SccUINT7 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT7_NATIVE
int Decode_NATIVE_UINT7(asn1SccUINT7 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT6_UPER
int Encode_UPER_UINT6(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT6 *pSrc);
#endif

#ifdef __NEED_UINT6_ACN
int Encode_ACN_UINT6(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT6 *pSrc);
#endif

#ifdef __NEED_UINT6_NATIVE
int Encode_NATIVE_UINT6(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT6 *pSrc);
#endif

#ifdef __NEED_UINT6_UPER
int Decode_UPER_UINT6(asn1SccUINT6 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT6_ACN
int Decode_ACN_UINT6(asn1SccUINT6 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT6_NATIVE
int Decode_NATIVE_UINT6(asn1SccUINT6 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT5_UPER
int Encode_UPER_UINT5(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT5 *pSrc);
#endif

#ifdef __NEED_UINT5_ACN
int Encode_ACN_UINT5(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT5 *pSrc);
#endif

#ifdef __NEED_UINT5_NATIVE
int Encode_NATIVE_UINT5(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT5 *pSrc);
#endif

#ifdef __NEED_UINT5_UPER
int Decode_UPER_UINT5(asn1SccUINT5 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT5_ACN
int Decode_ACN_UINT5(asn1SccUINT5 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT5_NATIVE
int Decode_NATIVE_UINT5(asn1SccUINT5 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT4_UPER
int Encode_UPER_UINT4(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT4 *pSrc);
#endif

#ifdef __NEED_UINT4_ACN
int Encode_ACN_UINT4(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT4 *pSrc);
#endif

#ifdef __NEED_UINT4_NATIVE
int Encode_NATIVE_UINT4(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT4 *pSrc);
#endif

#ifdef __NEED_UINT4_UPER
int Decode_UPER_UINT4(asn1SccUINT4 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT4_ACN
int Decode_ACN_UINT4(asn1SccUINT4 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT4_NATIVE
int Decode_NATIVE_UINT4(asn1SccUINT4 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT3_UPER
int Encode_UPER_UINT3(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT3 *pSrc);
#endif

#ifdef __NEED_UINT3_ACN
int Encode_ACN_UINT3(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT3 *pSrc);
#endif

#ifdef __NEED_UINT3_NATIVE
int Encode_NATIVE_UINT3(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT3 *pSrc);
#endif

#ifdef __NEED_UINT3_UPER
int Decode_UPER_UINT3(asn1SccUINT3 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT3_ACN
int Decode_ACN_UINT3(asn1SccUINT3 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT3_NATIVE
int Decode_NATIVE_UINT3(asn1SccUINT3 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT2_UPER
int Encode_UPER_UINT2(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT2 *pSrc);
#endif

#ifdef __NEED_UINT2_ACN
int Encode_ACN_UINT2(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT2 *pSrc);
#endif

#ifdef __NEED_UINT2_NATIVE
int Encode_NATIVE_UINT2(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT2 *pSrc);
#endif

#ifdef __NEED_UINT2_UPER
int Decode_UPER_UINT2(asn1SccUINT2 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT2_ACN
int Decode_ACN_UINT2(asn1SccUINT2 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT2_NATIVE
int Decode_NATIVE_UINT2(asn1SccUINT2 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT1_UPER
int Encode_UPER_UINT1(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT1 *pSrc);
#endif

#ifdef __NEED_UINT1_ACN
int Encode_ACN_UINT1(void *pBuffer, size_t iMaxBufferSize, asn1SccUINT1 *pSrc);
#endif

#ifdef __NEED_UINT1_NATIVE
int Encode_NATIVE_UINT1(void *pBuffer, size_t iMaxBufferSize, const asn1SccUINT1 *pSrc);
#endif

#ifdef __NEED_UINT1_UPER
int Decode_UPER_UINT1(asn1SccUINT1 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT1_ACN
int Decode_ACN_UINT1(asn1SccUINT1 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_UINT1_NATIVE
int Decode_NATIVE_UINT1(asn1SccUINT1 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Bit_UPER
int Encode_UPER_T_Bit(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Bit *pSrc);
#endif

#ifdef __NEED_T_Bit_ACN
int Encode_ACN_T_Bit(void *pBuffer, size_t iMaxBufferSize, asn1SccT_Bit *pSrc);
#endif

#ifdef __NEED_T_Bit_NATIVE
int Encode_NATIVE_T_Bit(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Bit *pSrc);
#endif

#ifdef __NEED_T_Bit_UPER
int Decode_UPER_T_Bit(asn1SccT_Bit *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Bit_ACN
int Decode_ACN_T_Bit(asn1SccT_Bit *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Bit_NATIVE
int Decode_NATIVE_T_Bit(asn1SccT_Bit *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_INT16_UPER
int Encode_UPER_INT16(void *pBuffer, size_t iMaxBufferSize, const asn1SccINT16 *pSrc);
#endif

#ifdef __NEED_INT16_ACN
int Encode_ACN_INT16(void *pBuffer, size_t iMaxBufferSize, asn1SccINT16 *pSrc);
#endif

#ifdef __NEED_INT16_NATIVE
int Encode_NATIVE_INT16(void *pBuffer, size_t iMaxBufferSize, const asn1SccINT16 *pSrc);
#endif

#ifdef __NEED_INT16_UPER
int Decode_UPER_INT16(asn1SccINT16 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_INT16_ACN
int Decode_ACN_INT16(asn1SccINT16 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_INT16_NATIVE
int Decode_NATIVE_INT16(asn1SccINT16 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_INT32_UPER
int Encode_UPER_INT32(void *pBuffer, size_t iMaxBufferSize, const asn1SccINT32 *pSrc);
#endif

#ifdef __NEED_INT32_ACN
int Encode_ACN_INT32(void *pBuffer, size_t iMaxBufferSize, asn1SccINT32 *pSrc);
#endif

#ifdef __NEED_INT32_NATIVE
int Encode_NATIVE_INT32(void *pBuffer, size_t iMaxBufferSize, const asn1SccINT32 *pSrc);
#endif

#ifdef __NEED_INT32_UPER
int Decode_UPER_INT32(asn1SccINT32 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_INT32_ACN
int Decode_ACN_INT32(asn1SccINT32 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_INT32_NATIVE
int Decode_NATIVE_INT32(asn1SccINT32 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_INT24_UPER
int Encode_UPER_INT24(void *pBuffer, size_t iMaxBufferSize, const asn1SccINT24 *pSrc);
#endif

#ifdef __NEED_INT24_ACN
int Encode_ACN_INT24(void *pBuffer, size_t iMaxBufferSize, asn1SccINT24 *pSrc);
#endif

#ifdef __NEED_INT24_NATIVE
int Encode_NATIVE_INT24(void *pBuffer, size_t iMaxBufferSize, const asn1SccINT24 *pSrc);
#endif

#ifdef __NEED_INT24_UPER
int Decode_UPER_INT24(asn1SccINT24 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_INT24_ACN
int Decode_ACN_INT24(asn1SccINT24 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_INT24_NATIVE
int Decode_NATIVE_INT24(asn1SccINT24 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_OCTET_UPER
int Encode_UPER_T_OCTET(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_OCTET *pSrc);
#endif

#ifdef __NEED_T_OCTET_ACN
int Encode_ACN_T_OCTET(void *pBuffer, size_t iMaxBufferSize, asn1SccT_OCTET *pSrc);
#endif

#ifdef __NEED_T_OCTET_NATIVE
int Encode_NATIVE_T_OCTET(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_OCTET *pSrc);
#endif

#ifdef __NEED_T_OCTET_UPER
int Decode_UPER_T_OCTET(asn1SccT_OCTET *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_OCTET_ACN
int Decode_ACN_T_OCTET(asn1SccT_OCTET *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_OCTET_NATIVE
int Decode_NATIVE_T_OCTET(asn1SccT_OCTET *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_OnOffState_UPER
int Encode_UPER_T_OnOffState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_OnOffState *pSrc);
#endif

#ifdef __NEED_T_OnOffState_ACN
int Encode_ACN_T_OnOffState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_OnOffState *pSrc);
#endif

#ifdef __NEED_T_OnOffState_NATIVE
int Encode_NATIVE_T_OnOffState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_OnOffState *pSrc);
#endif

#ifdef __NEED_T_OnOffState_UPER
int Decode_UPER_T_OnOffState(asn1SccT_OnOffState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_OnOffState_ACN
int Decode_ACN_T_OnOffState(asn1SccT_OnOffState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_OnOffState_NATIVE
int Decode_NATIVE_T_OnOffState(asn1SccT_OnOffState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_NominalRedundantState_UPER
int Encode_UPER_T_NominalRedundantState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_NominalRedundantState *pSrc);
#endif

#ifdef __NEED_T_NominalRedundantState_ACN
int Encode_ACN_T_NominalRedundantState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_NominalRedundantState *pSrc);
#endif

#ifdef __NEED_T_NominalRedundantState_NATIVE
int Encode_NATIVE_T_NominalRedundantState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_NominalRedundantState *pSrc);
#endif

#ifdef __NEED_T_NominalRedundantState_UPER
int Decode_UPER_T_NominalRedundantState(asn1SccT_NominalRedundantState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_NominalRedundantState_ACN
int Decode_ACN_T_NominalRedundantState(asn1SccT_NominalRedundantState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_NominalRedundantState_NATIVE
int Decode_NATIVE_T_NominalRedundantState(asn1SccT_NominalRedundantState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ConnectionState_UPER
int Encode_UPER_T_ConnectionState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ConnectionState *pSrc);
#endif

#ifdef __NEED_T_ConnectionState_ACN
int Encode_ACN_T_ConnectionState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_ConnectionState *pSrc);
#endif

#ifdef __NEED_T_ConnectionState_NATIVE
int Encode_NATIVE_T_ConnectionState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ConnectionState *pSrc);
#endif

#ifdef __NEED_T_ConnectionState_UPER
int Decode_UPER_T_ConnectionState(asn1SccT_ConnectionState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ConnectionState_ACN
int Decode_ACN_T_ConnectionState(asn1SccT_ConnectionState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ConnectionState_NATIVE
int Decode_NATIVE_T_ConnectionState(asn1SccT_ConnectionState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_SwitchState_UPER
int Encode_UPER_T_SwitchState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_SwitchState *pSrc);
#endif

#ifdef __NEED_T_SwitchState_ACN
int Encode_ACN_T_SwitchState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_SwitchState *pSrc);
#endif

#ifdef __NEED_T_SwitchState_NATIVE
int Encode_NATIVE_T_SwitchState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_SwitchState *pSrc);
#endif

#ifdef __NEED_T_SwitchState_UPER
int Decode_UPER_T_SwitchState(asn1SccT_SwitchState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_SwitchState_ACN
int Decode_ACN_T_SwitchState(asn1SccT_SwitchState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_SwitchState_NATIVE
int Decode_NATIVE_T_SwitchState(asn1SccT_SwitchState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_FaultState_UPER
int Encode_UPER_T_FaultState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_FaultState *pSrc);
#endif

#ifdef __NEED_T_FaultState_ACN
int Encode_ACN_T_FaultState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_FaultState *pSrc);
#endif

#ifdef __NEED_T_FaultState_NATIVE
int Encode_NATIVE_T_FaultState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_FaultState *pSrc);
#endif

#ifdef __NEED_T_FaultState_UPER
int Decode_UPER_T_FaultState(asn1SccT_FaultState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_FaultState_ACN
int Decode_ACN_T_FaultState(asn1SccT_FaultState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_FaultState_NATIVE
int Decode_NATIVE_T_FaultState(asn1SccT_FaultState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_SelectionState_UPER
int Encode_UPER_T_SelectionState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_SelectionState *pSrc);
#endif

#ifdef __NEED_T_SelectionState_ACN
int Encode_ACN_T_SelectionState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_SelectionState *pSrc);
#endif

#ifdef __NEED_T_SelectionState_NATIVE
int Encode_NATIVE_T_SelectionState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_SelectionState *pSrc);
#endif

#ifdef __NEED_T_SelectionState_UPER
int Decode_UPER_T_SelectionState(asn1SccT_SelectionState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_SelectionState_ACN
int Decode_ACN_T_SelectionState(asn1SccT_SelectionState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_SelectionState_NATIVE
int Decode_NATIVE_T_SelectionState(asn1SccT_SelectionState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_PresenceState_UPER
int Encode_UPER_T_PresenceState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_PresenceState *pSrc);
#endif

#ifdef __NEED_T_PresenceState_ACN
int Encode_ACN_T_PresenceState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_PresenceState *pSrc);
#endif

#ifdef __NEED_T_PresenceState_NATIVE
int Encode_NATIVE_T_PresenceState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_PresenceState *pSrc);
#endif

#ifdef __NEED_T_PresenceState_UPER
int Decode_UPER_T_PresenceState(asn1SccT_PresenceState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_PresenceState_ACN
int Decode_ACN_T_PresenceState(asn1SccT_PresenceState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_PresenceState_NATIVE
int Decode_NATIVE_T_PresenceState(asn1SccT_PresenceState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ForwardBackwardState_UPER
int Encode_UPER_T_ForwardBackwardState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ForwardBackwardState *pSrc);
#endif

#ifdef __NEED_T_ForwardBackwardState_ACN
int Encode_ACN_T_ForwardBackwardState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_ForwardBackwardState *pSrc);
#endif

#ifdef __NEED_T_ForwardBackwardState_NATIVE
int Encode_NATIVE_T_ForwardBackwardState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ForwardBackwardState *pSrc);
#endif

#ifdef __NEED_T_ForwardBackwardState_UPER
int Decode_UPER_T_ForwardBackwardState(asn1SccT_ForwardBackwardState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ForwardBackwardState_ACN
int Decode_ACN_T_ForwardBackwardState(asn1SccT_ForwardBackwardState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ForwardBackwardState_NATIVE
int Decode_NATIVE_T_ForwardBackwardState(asn1SccT_ForwardBackwardState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_LeftRightState_UPER
int Encode_UPER_T_LeftRightState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_LeftRightState *pSrc);
#endif

#ifdef __NEED_T_LeftRightState_ACN
int Encode_ACN_T_LeftRightState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_LeftRightState *pSrc);
#endif

#ifdef __NEED_T_LeftRightState_NATIVE
int Encode_NATIVE_T_LeftRightState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_LeftRightState *pSrc);
#endif

#ifdef __NEED_T_LeftRightState_UPER
int Decode_UPER_T_LeftRightState(asn1SccT_LeftRightState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_LeftRightState_ACN
int Decode_ACN_T_LeftRightState(asn1SccT_LeftRightState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_LeftRightState_NATIVE
int Decode_NATIVE_T_LeftRightState(asn1SccT_LeftRightState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UsedUnusedState_UPER
int Encode_UPER_T_UsedUnusedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UsedUnusedState *pSrc);
#endif

#ifdef __NEED_T_UsedUnusedState_ACN
int Encode_ACN_T_UsedUnusedState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UsedUnusedState *pSrc);
#endif

#ifdef __NEED_T_UsedUnusedState_NATIVE
int Encode_NATIVE_T_UsedUnusedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UsedUnusedState *pSrc);
#endif

#ifdef __NEED_T_UsedUnusedState_UPER
int Decode_UPER_T_UsedUnusedState(asn1SccT_UsedUnusedState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UsedUnusedState_ACN
int Decode_ACN_T_UsedUnusedState(asn1SccT_UsedUnusedState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UsedUnusedState_NATIVE
int Decode_NATIVE_T_UsedUnusedState(asn1SccT_UsedUnusedState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_MovingStillState_UPER
int Encode_UPER_T_MovingStillState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_MovingStillState *pSrc);
#endif

#ifdef __NEED_T_MovingStillState_ACN
int Encode_ACN_T_MovingStillState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_MovingStillState *pSrc);
#endif

#ifdef __NEED_T_MovingStillState_NATIVE
int Encode_NATIVE_T_MovingStillState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_MovingStillState *pSrc);
#endif

#ifdef __NEED_T_MovingStillState_UPER
int Decode_UPER_T_MovingStillState(asn1SccT_MovingStillState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_MovingStillState_ACN
int Decode_ACN_T_MovingStillState(asn1SccT_MovingStillState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_MovingStillState_NATIVE
int Decode_NATIVE_T_MovingStillState(asn1SccT_MovingStillState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_PlusMinusState_UPER
int Encode_UPER_T_PlusMinusState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_PlusMinusState *pSrc);
#endif

#ifdef __NEED_T_PlusMinusState_ACN
int Encode_ACN_T_PlusMinusState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_PlusMinusState *pSrc);
#endif

#ifdef __NEED_T_PlusMinusState_NATIVE
int Encode_NATIVE_T_PlusMinusState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_PlusMinusState *pSrc);
#endif

#ifdef __NEED_T_PlusMinusState_UPER
int Decode_UPER_T_PlusMinusState(asn1SccT_PlusMinusState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_PlusMinusState_ACN
int Decode_ACN_T_PlusMinusState(asn1SccT_PlusMinusState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_PlusMinusState_NATIVE
int Decode_NATIVE_T_PlusMinusState(asn1SccT_PlusMinusState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ArmedDisarmedState_UPER
int Encode_UPER_T_ArmedDisarmedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ArmedDisarmedState *pSrc);
#endif

#ifdef __NEED_T_ArmedDisarmedState_ACN
int Encode_ACN_T_ArmedDisarmedState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_ArmedDisarmedState *pSrc);
#endif

#ifdef __NEED_T_ArmedDisarmedState_NATIVE
int Encode_NATIVE_T_ArmedDisarmedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ArmedDisarmedState *pSrc);
#endif

#ifdef __NEED_T_ArmedDisarmedState_UPER
int Decode_UPER_T_ArmedDisarmedState(asn1SccT_ArmedDisarmedState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ArmedDisarmedState_ACN
int Decode_ACN_T_ArmedDisarmedState(asn1SccT_ArmedDisarmedState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ArmedDisarmedState_NATIVE
int Decode_NATIVE_T_ArmedDisarmedState(asn1SccT_ArmedDisarmedState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ActiveInactiveState_UPER
int Encode_UPER_T_ActiveInactiveState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ActiveInactiveState *pSrc);
#endif

#ifdef __NEED_T_ActiveInactiveState_ACN
int Encode_ACN_T_ActiveInactiveState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_ActiveInactiveState *pSrc);
#endif

#ifdef __NEED_T_ActiveInactiveState_NATIVE
int Encode_NATIVE_T_ActiveInactiveState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ActiveInactiveState *pSrc);
#endif

#ifdef __NEED_T_ActiveInactiveState_UPER
int Decode_UPER_T_ActiveInactiveState(asn1SccT_ActiveInactiveState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ActiveInactiveState_ACN
int Decode_ACN_T_ActiveInactiveState(asn1SccT_ActiveInactiveState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ActiveInactiveState_NATIVE
int Decode_NATIVE_T_ActiveInactiveState(asn1SccT_ActiveInactiveState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_EnabledInhibitedState_UPER
int Encode_UPER_T_EnabledInhibitedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_EnabledInhibitedState *pSrc);
#endif

#ifdef __NEED_T_EnabledInhibitedState_ACN
int Encode_ACN_T_EnabledInhibitedState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_EnabledInhibitedState *pSrc);
#endif

#ifdef __NEED_T_EnabledInhibitedState_NATIVE
int Encode_NATIVE_T_EnabledInhibitedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_EnabledInhibitedState *pSrc);
#endif

#ifdef __NEED_T_EnabledInhibitedState_UPER
int Decode_UPER_T_EnabledInhibitedState(asn1SccT_EnabledInhibitedState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_EnabledInhibitedState_ACN
int Decode_ACN_T_EnabledInhibitedState(asn1SccT_EnabledInhibitedState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_EnabledInhibitedState_NATIVE
int Decode_NATIVE_T_EnabledInhibitedState(asn1SccT_EnabledInhibitedState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_FlagState_UPER
int Encode_UPER_T_FlagState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_FlagState *pSrc);
#endif

#ifdef __NEED_T_FlagState_ACN
int Encode_ACN_T_FlagState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_FlagState *pSrc);
#endif

#ifdef __NEED_T_FlagState_NATIVE
int Encode_NATIVE_T_FlagState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_FlagState *pSrc);
#endif

#ifdef __NEED_T_FlagState_UPER
int Decode_UPER_T_FlagState(asn1SccT_FlagState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_FlagState_ACN
int Decode_ACN_T_FlagState(asn1SccT_FlagState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_FlagState_NATIVE
int Decode_NATIVE_T_FlagState(asn1SccT_FlagState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_SuccessFailState_UPER
int Encode_UPER_T_SuccessFailState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_SuccessFailState *pSrc);
#endif

#ifdef __NEED_T_SuccessFailState_ACN
int Encode_ACN_T_SuccessFailState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_SuccessFailState *pSrc);
#endif

#ifdef __NEED_T_SuccessFailState_NATIVE
int Encode_NATIVE_T_SuccessFailState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_SuccessFailState *pSrc);
#endif

#ifdef __NEED_T_SuccessFailState_UPER
int Decode_UPER_T_SuccessFailState(asn1SccT_SuccessFailState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_SuccessFailState_ACN
int Decode_ACN_T_SuccessFailState(asn1SccT_SuccessFailState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_SuccessFailState_NATIVE
int Decode_NATIVE_T_SuccessFailState(asn1SccT_SuccessFailState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_HighLowState_UPER
int Encode_UPER_T_HighLowState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_HighLowState *pSrc);
#endif

#ifdef __NEED_T_HighLowState_ACN
int Encode_ACN_T_HighLowState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_HighLowState *pSrc);
#endif

#ifdef __NEED_T_HighLowState_NATIVE
int Encode_NATIVE_T_HighLowState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_HighLowState *pSrc);
#endif

#ifdef __NEED_T_HighLowState_UPER
int Decode_UPER_T_HighLowState(asn1SccT_HighLowState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_HighLowState_ACN
int Decode_ACN_T_HighLowState(asn1SccT_HighLowState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_HighLowState_NATIVE
int Decode_NATIVE_T_HighLowState(asn1SccT_HighLowState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_OverrideState_UPER
int Encode_UPER_T_OverrideState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_OverrideState *pSrc);
#endif

#ifdef __NEED_T_OverrideState_ACN
int Encode_ACN_T_OverrideState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_OverrideState *pSrc);
#endif

#ifdef __NEED_T_OverrideState_NATIVE
int Encode_NATIVE_T_OverrideState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_OverrideState *pSrc);
#endif

#ifdef __NEED_T_OverrideState_UPER
int Decode_UPER_T_OverrideState(asn1SccT_OverrideState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_OverrideState_ACN
int Decode_ACN_T_OverrideState(asn1SccT_OverrideState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_OverrideState_NATIVE
int Decode_NATIVE_T_OverrideState(asn1SccT_OverrideState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_LockedUnlockedState_UPER
int Encode_UPER_T_LockedUnlockedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_LockedUnlockedState *pSrc);
#endif

#ifdef __NEED_T_LockedUnlockedState_ACN
int Encode_ACN_T_LockedUnlockedState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_LockedUnlockedState *pSrc);
#endif

#ifdef __NEED_T_LockedUnlockedState_NATIVE
int Encode_NATIVE_T_LockedUnlockedState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_LockedUnlockedState *pSrc);
#endif

#ifdef __NEED_T_LockedUnlockedState_UPER
int Decode_UPER_T_LockedUnlockedState(asn1SccT_LockedUnlockedState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_LockedUnlockedState_ACN
int Decode_ACN_T_LockedUnlockedState(asn1SccT_LockedUnlockedState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_LockedUnlockedState_NATIVE
int Decode_NATIVE_T_LockedUnlockedState(asn1SccT_LockedUnlockedState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_WarningState_UPER
int Encode_UPER_T_WarningState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_WarningState *pSrc);
#endif

#ifdef __NEED_T_WarningState_ACN
int Encode_ACN_T_WarningState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_WarningState *pSrc);
#endif

#ifdef __NEED_T_WarningState_NATIVE
int Encode_NATIVE_T_WarningState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_WarningState *pSrc);
#endif

#ifdef __NEED_T_WarningState_UPER
int Decode_UPER_T_WarningState(asn1SccT_WarningState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_WarningState_ACN
int Decode_ACN_T_WarningState(asn1SccT_WarningState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_WarningState_NATIVE
int Decode_NATIVE_T_WarningState(asn1SccT_WarningState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ValidInvalidState_UPER
int Encode_UPER_T_ValidInvalidState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ValidInvalidState *pSrc);
#endif

#ifdef __NEED_T_ValidInvalidState_ACN
int Encode_ACN_T_ValidInvalidState(void *pBuffer, size_t iMaxBufferSize, asn1SccT_ValidInvalidState *pSrc);
#endif

#ifdef __NEED_T_ValidInvalidState_NATIVE
int Encode_NATIVE_T_ValidInvalidState(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_ValidInvalidState *pSrc);
#endif

#ifdef __NEED_T_ValidInvalidState_UPER
int Decode_UPER_T_ValidInvalidState(asn1SccT_ValidInvalidState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ValidInvalidState_ACN
int Decode_ACN_T_ValidInvalidState(asn1SccT_ValidInvalidState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_ValidInvalidState_NATIVE
int Decode_NATIVE_T_ValidInvalidState(asn1SccT_ValidInvalidState *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_Header_UPER
int Encode_UPER_T_UART_DLC_Header(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_Header *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_Header_ACN
int Encode_ACN_T_UART_DLC_Header(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UART_DLC_Header *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_Header_NATIVE
int Encode_NATIVE_T_UART_DLC_Header(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_Header *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_Header_UPER
int Decode_UPER_T_UART_DLC_Header(asn1SccT_UART_DLC_Header *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_Header_ACN
int Decode_ACN_T_UART_DLC_Header(asn1SccT_UART_DLC_Header *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_Header_NATIVE
int Decode_NATIVE_T_UART_DLC_Header(asn1SccT_UART_DLC_Header *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_FrameCRC_UPER
int Encode_UPER_T_UART_DLC_FrameCRC(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_FrameCRC *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_FrameCRC_ACN
int Encode_ACN_T_UART_DLC_FrameCRC(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UART_DLC_FrameCRC *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_FrameCRC_NATIVE
int Encode_NATIVE_T_UART_DLC_FrameCRC(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_FrameCRC *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_FrameCRC_UPER
int Decode_UPER_T_UART_DLC_FrameCRC(asn1SccT_UART_DLC_FrameCRC *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_FrameCRC_ACN
int Decode_ACN_T_UART_DLC_FrameCRC(asn1SccT_UART_DLC_FrameCRC *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_FrameCRC_NATIVE
int Decode_NATIVE_T_UART_DLC_FrameCRC(asn1SccT_UART_DLC_FrameCRC *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_RMAPCRCTable_UPER
int Encode_UPER_T_RMAPCRCTable(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_RMAPCRCTable *pSrc);
#endif

#ifdef __NEED_T_RMAPCRCTable_ACN
int Encode_ACN_T_RMAPCRCTable(void *pBuffer, size_t iMaxBufferSize, asn1SccT_RMAPCRCTable *pSrc);
#endif

#ifdef __NEED_T_RMAPCRCTable_NATIVE
int Encode_NATIVE_T_RMAPCRCTable(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_RMAPCRCTable *pSrc);
#endif

#ifdef __NEED_T_RMAPCRCTable_UPER
int Decode_UPER_T_RMAPCRCTable(asn1SccT_RMAPCRCTable *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_RMAPCRCTable_ACN
int Decode_ACN_T_RMAPCRCTable(asn1SccT_RMAPCRCTable *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_RMAPCRCTable_NATIVE
int Decode_NATIVE_T_RMAPCRCTable(asn1SccT_RMAPCRCTable *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_FrameBuffer_UPER
int Encode_UPER_T_UART_DLC_FrameBuffer(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_FrameBuffer *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_FrameBuffer_ACN
int Encode_ACN_T_UART_DLC_FrameBuffer(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UART_DLC_FrameBuffer *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_FrameBuffer_NATIVE
int Encode_NATIVE_T_UART_DLC_FrameBuffer(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_FrameBuffer *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_FrameBuffer_UPER
int Decode_UPER_T_UART_DLC_FrameBuffer(asn1SccT_UART_DLC_FrameBuffer *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_FrameBuffer_ACN
int Decode_ACN_T_UART_DLC_FrameBuffer(asn1SccT_UART_DLC_FrameBuffer *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_FrameBuffer_NATIVE
int Decode_NATIVE_T_UART_DLC_FrameBuffer(asn1SccT_UART_DLC_FrameBuffer *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_SDU_UPER
int Encode_UPER_T_UART_DLC_SDU(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_SDU *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_SDU_ACN
int Encode_ACN_T_UART_DLC_SDU(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UART_DLC_SDU *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_SDU_NATIVE
int Encode_NATIVE_T_UART_DLC_SDU(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_SDU *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_SDU_UPER
int Decode_UPER_T_UART_DLC_SDU(asn1SccT_UART_DLC_SDU *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_SDU_ACN
int Decode_ACN_T_UART_DLC_SDU(asn1SccT_UART_DLC_SDU *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_SDU_NATIVE
int Decode_NATIVE_T_UART_DLC_SDU(asn1SccT_UART_DLC_SDU *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_PDU_UPER
int Encode_UPER_T_UART_DLC_PDU(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_PDU *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_PDU_ACN
int Encode_ACN_T_UART_DLC_PDU(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UART_DLC_PDU *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_PDU_NATIVE
int Encode_NATIVE_T_UART_DLC_PDU(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_PDU *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_PDU_UPER
int Decode_UPER_T_UART_DLC_PDU(asn1SccT_UART_DLC_PDU *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_PDU_ACN
int Decode_ACN_T_UART_DLC_PDU(asn1SccT_UART_DLC_PDU *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_PDU_NATIVE
int Decode_NATIVE_T_UART_DLC_PDU(asn1SccT_UART_DLC_PDU *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_RxErrorCode_UPER
int Encode_UPER_T_UART_DLC_RxErrorCode(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_RxErrorCode *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_RxErrorCode_ACN
int Encode_ACN_T_UART_DLC_RxErrorCode(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UART_DLC_RxErrorCode *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_RxErrorCode_NATIVE
int Encode_NATIVE_T_UART_DLC_RxErrorCode(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UART_DLC_RxErrorCode *pSrc);
#endif

#ifdef __NEED_T_UART_DLC_RxErrorCode_UPER
int Decode_UPER_T_UART_DLC_RxErrorCode(asn1SccT_UART_DLC_RxErrorCode *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_RxErrorCode_ACN
int Decode_ACN_T_UART_DLC_RxErrorCode(asn1SccT_UART_DLC_RxErrorCode *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UART_DLC_RxErrorCode_NATIVE
int Decode_NATIVE_T_UART_DLC_RxErrorCode(asn1SccT_UART_DLC_RxErrorCode *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Int32_UPER
int Encode_UPER_T_Int32(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Int32 *pSrc);
#endif

#ifdef __NEED_T_Int32_ACN
int Encode_ACN_T_Int32(void *pBuffer, size_t iMaxBufferSize, asn1SccT_Int32 *pSrc);
#endif

#ifdef __NEED_T_Int32_NATIVE
int Encode_NATIVE_T_Int32(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Int32 *pSrc);
#endif

#ifdef __NEED_T_Int32_UPER
int Decode_UPER_T_Int32(asn1SccT_Int32 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Int32_ACN
int Decode_ACN_T_Int32(asn1SccT_Int32 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Int32_NATIVE
int Decode_NATIVE_T_Int32(asn1SccT_Int32 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UInt32_UPER
int Encode_UPER_T_UInt32(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UInt32 *pSrc);
#endif

#ifdef __NEED_T_UInt32_ACN
int Encode_ACN_T_UInt32(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UInt32 *pSrc);
#endif

#ifdef __NEED_T_UInt32_NATIVE
int Encode_NATIVE_T_UInt32(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UInt32 *pSrc);
#endif

#ifdef __NEED_T_UInt32_UPER
int Decode_UPER_T_UInt32(asn1SccT_UInt32 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UInt32_ACN
int Decode_ACN_T_UInt32(asn1SccT_UInt32 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UInt32_NATIVE
int Decode_NATIVE_T_UInt32(asn1SccT_UInt32 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Int8_UPER
int Encode_UPER_T_Int8(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Int8 *pSrc);
#endif

#ifdef __NEED_T_Int8_ACN
int Encode_ACN_T_Int8(void *pBuffer, size_t iMaxBufferSize, asn1SccT_Int8 *pSrc);
#endif

#ifdef __NEED_T_Int8_NATIVE
int Encode_NATIVE_T_Int8(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Int8 *pSrc);
#endif

#ifdef __NEED_T_Int8_UPER
int Decode_UPER_T_Int8(asn1SccT_Int8 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Int8_ACN
int Decode_ACN_T_Int8(asn1SccT_Int8 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Int8_NATIVE
int Decode_NATIVE_T_Int8(asn1SccT_Int8 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UInt8_UPER
int Encode_UPER_T_UInt8(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UInt8 *pSrc);
#endif

#ifdef __NEED_T_UInt8_ACN
int Encode_ACN_T_UInt8(void *pBuffer, size_t iMaxBufferSize, asn1SccT_UInt8 *pSrc);
#endif

#ifdef __NEED_T_UInt8_NATIVE
int Encode_NATIVE_T_UInt8(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_UInt8 *pSrc);
#endif

#ifdef __NEED_T_UInt8_UPER
int Decode_UPER_T_UInt8(asn1SccT_UInt8 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UInt8_ACN
int Decode_ACN_T_UInt8(asn1SccT_UInt8 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_UInt8_NATIVE
int Decode_NATIVE_T_UInt8(asn1SccT_UInt8 *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Boolean_UPER
int Encode_UPER_T_Boolean(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Boolean *pSrc);
#endif

#ifdef __NEED_T_Boolean_ACN
int Encode_ACN_T_Boolean(void *pBuffer, size_t iMaxBufferSize, asn1SccT_Boolean *pSrc);
#endif

#ifdef __NEED_T_Boolean_NATIVE
int Encode_NATIVE_T_Boolean(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Boolean *pSrc);
#endif

#ifdef __NEED_T_Boolean_UPER
int Decode_UPER_T_Boolean(asn1SccT_Boolean *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Boolean_ACN
int Decode_ACN_T_Boolean(asn1SccT_Boolean *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Boolean_NATIVE
int Decode_NATIVE_T_Boolean(asn1SccT_Boolean *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Null_Record_UPER
int Encode_UPER_T_Null_Record(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Null_Record *pSrc);
#endif

#ifdef __NEED_T_Null_Record_ACN
int Encode_ACN_T_Null_Record(void *pBuffer, size_t iMaxBufferSize, asn1SccT_Null_Record *pSrc);
#endif

#ifdef __NEED_T_Null_Record_NATIVE
int Encode_NATIVE_T_Null_Record(void *pBuffer, size_t iMaxBufferSize, const asn1SccT_Null_Record *pSrc);
#endif

#ifdef __NEED_T_Null_Record_UPER
int Decode_UPER_T_Null_Record(asn1SccT_Null_Record *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Null_Record_ACN
int Decode_ACN_T_Null_Record(asn1SccT_Null_Record *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_T_Null_Record_NATIVE
int Decode_NATIVE_T_Null_Record(asn1SccT_Null_Record *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MyInteger_UPER
int Encode_UPER_MyInteger(void *pBuffer, size_t iMaxBufferSize, const asn1SccMyInteger *pSrc);
#endif

#ifdef __NEED_MyInteger_ACN
int Encode_ACN_MyInteger(void *pBuffer, size_t iMaxBufferSize, asn1SccMyInteger *pSrc);
#endif

#ifdef __NEED_MyInteger_NATIVE
int Encode_NATIVE_MyInteger(void *pBuffer, size_t iMaxBufferSize, const asn1SccMyInteger *pSrc);
#endif

#ifdef __NEED_MyInteger_UPER
int Decode_UPER_MyInteger(asn1SccMyInteger *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MyInteger_ACN
int Decode_ACN_MyInteger(asn1SccMyInteger *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MyInteger_NATIVE
int Decode_NATIVE_MyInteger(asn1SccMyInteger *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_PID_UPER
int Encode_UPER_PID(void *pBuffer, size_t iMaxBufferSize, const asn1SccPID *pSrc);
#endif

#ifdef __NEED_PID_ACN
int Encode_ACN_PID(void *pBuffer, size_t iMaxBufferSize, asn1SccPID *pSrc);
#endif

#ifdef __NEED_PID_NATIVE
int Encode_NATIVE_PID(void *pBuffer, size_t iMaxBufferSize, const asn1SccPID *pSrc);
#endif

#ifdef __NEED_PID_UPER
int Decode_UPER_PID(asn1SccPID *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_PID_ACN
int Decode_ACN_PID(asn1SccPID *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_PID_NATIVE
int Decode_NATIVE_PID(asn1SccPID *pDst, void *pBuffer, size_t iBufferSize);
#endif


#endif
