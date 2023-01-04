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

#ifdef __NEED_MySeq_UPER
int Encode_UPER_MySeq(void *pBuffer, size_t iMaxBufferSize, const asn1SccMySeq *pSrc);
#endif

#ifdef __NEED_MySeq_ACN
int Encode_ACN_MySeq(void *pBuffer, size_t iMaxBufferSize, asn1SccMySeq *pSrc);
#endif

#ifdef __NEED_MySeq_NATIVE
int Encode_NATIVE_MySeq(void *pBuffer, size_t iMaxBufferSize, const asn1SccMySeq *pSrc);
#endif

#ifdef __NEED_MySeq_UPER
int Decode_UPER_MySeq(asn1SccMySeq *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MySeq_ACN
int Decode_ACN_MySeq(asn1SccMySeq *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MySeq_NATIVE
int Decode_NATIVE_MySeq(asn1SccMySeq *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MyChoice_UPER
int Encode_UPER_MyChoice(void *pBuffer, size_t iMaxBufferSize, const asn1SccMyChoice *pSrc);
#endif

#ifdef __NEED_MyChoice_ACN
int Encode_ACN_MyChoice(void *pBuffer, size_t iMaxBufferSize, asn1SccMyChoice *pSrc);
#endif

#ifdef __NEED_MyChoice_NATIVE
int Encode_NATIVE_MyChoice(void *pBuffer, size_t iMaxBufferSize, const asn1SccMyChoice *pSrc);
#endif

#ifdef __NEED_MyChoice_UPER
int Decode_UPER_MyChoice(asn1SccMyChoice *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MyChoice_ACN
int Decode_ACN_MyChoice(asn1SccMyChoice *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MyChoice_NATIVE
int Decode_NATIVE_MyChoice(asn1SccMyChoice *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MyEnum_UPER
int Encode_UPER_MyEnum(void *pBuffer, size_t iMaxBufferSize, const asn1SccMyEnum *pSrc);
#endif

#ifdef __NEED_MyEnum_ACN
int Encode_ACN_MyEnum(void *pBuffer, size_t iMaxBufferSize, asn1SccMyEnum *pSrc);
#endif

#ifdef __NEED_MyEnum_NATIVE
int Encode_NATIVE_MyEnum(void *pBuffer, size_t iMaxBufferSize, const asn1SccMyEnum *pSrc);
#endif

#ifdef __NEED_MyEnum_UPER
int Decode_UPER_MyEnum(asn1SccMyEnum *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MyEnum_ACN
int Decode_ACN_MyEnum(asn1SccMyEnum *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MyEnum_NATIVE
int Decode_NATIVE_MyEnum(asn1SccMyEnum *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MySeqOf_UPER
int Encode_UPER_MySeqOf(void *pBuffer, size_t iMaxBufferSize, const asn1SccMySeqOf *pSrc);
#endif

#ifdef __NEED_MySeqOf_ACN
int Encode_ACN_MySeqOf(void *pBuffer, size_t iMaxBufferSize, asn1SccMySeqOf *pSrc);
#endif

#ifdef __NEED_MySeqOf_NATIVE
int Encode_NATIVE_MySeqOf(void *pBuffer, size_t iMaxBufferSize, const asn1SccMySeqOf *pSrc);
#endif

#ifdef __NEED_MySeqOf_UPER
int Decode_UPER_MySeqOf(asn1SccMySeqOf *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MySeqOf_ACN
int Decode_ACN_MySeqOf(asn1SccMySeqOf *pDst, void *pBuffer, size_t iBufferSize);
#endif

#ifdef __NEED_MySeqOf_NATIVE
int Decode_NATIVE_MySeqOf(asn1SccMySeqOf *pDst, void *pBuffer, size_t iBufferSize);
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
