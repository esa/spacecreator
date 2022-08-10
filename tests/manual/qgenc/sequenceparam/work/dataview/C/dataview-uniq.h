#ifndef GENERATED_ASN1SCC_dataview_uniq_H
#define GENERATED_ASN1SCC_dataview_uniq_H
/*
Code automatically generated by asn1scc tool
*/
#include "asn1crt.h"
#include "asn1crt_encoding.h"


#ifdef  __cplusplus
extern "C" {
#endif


typedef asn1SccUint asn1SccMyInteger;


flag asn1SccMyInteger_Equal(const asn1SccMyInteger* pVal1, const asn1SccMyInteger* pVal2);

void asn1SccMyInteger_Initialize(asn1SccMyInteger* pVal);

#define ERR_MYINTEGER		1  /**/
flag asn1SccMyInteger_IsConstraintValid(const asn1SccMyInteger* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_MYINTEGER		2  /**/
#define asn1SccMyInteger_REQUIRED_BYTES_FOR_ENCODING       2 
#define asn1SccMyInteger_REQUIRED_BITS_FOR_ENCODING        14

flag asn1SccMyInteger_Encode(const asn1SccMyInteger* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_MYINTEGER		3  /**/
flag asn1SccMyInteger_Decode(asn1SccMyInteger* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_MYINTEGER		4  /**/
#define asn1SccMyInteger_REQUIRED_BYTES_FOR_ACN_ENCODING       2 
#define asn1SccMyInteger_REQUIRED_BITS_FOR_ACN_ENCODING        14

flag asn1SccMyInteger_ACN_Encode(const asn1SccMyInteger* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_MYINTEGER		5  /**/
flag asn1SccMyInteger_ACN_Decode(asn1SccMyInteger* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- asn1SccMySeq --------------------------------------------*/
typedef enum {
    asn1Sccvalid = 0,
    asn1Sccinvalid = 1
} asn1SccMySeq_validity;

// please use the following macros to avoid breaking code.
#define asn1SccMySeq_validity_valid asn1Sccvalid
#define asn1SccMySeq_validity_invalid asn1Sccinvalid
typedef struct {
    asn1SccMyInteger input_data;
    asn1SccMyInteger output_data;
    asn1SccMySeq_validity validity;

} asn1SccMySeq;

flag asn1SccMySeq_validity_Equal(const asn1SccMySeq_validity* pVal1, const asn1SccMySeq_validity* pVal2);

flag asn1SccMySeq_Equal(const asn1SccMySeq* pVal1, const asn1SccMySeq* pVal2);

void asn1SccMySeq_validity_Initialize(asn1SccMySeq_validity* pVal);
void asn1SccMySeq_Initialize(asn1SccMySeq* pVal);

#define ERR_MYSEQ		31  /**/
#define ERR_MYSEQ_INPUT_DATA_2		11  /**/
#define ERR_MYSEQ_OUTPUT_DATA_2		21  /**/
#define ERR_MYSEQ_VALIDITY		26  /**/
flag asn1SccMySeq_IsConstraintValid(const asn1SccMySeq* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_MYSEQ		32  /**/
#define ERR_UPER_ENCODE_MYSEQ_INPUT_DATA_2		12  /**/
#define ERR_UPER_ENCODE_MYSEQ_OUTPUT_DATA_2		22  /**/
#define ERR_UPER_ENCODE_MYSEQ_VALIDITY		27  /**/
#define asn1SccMySeq_REQUIRED_BYTES_FOR_ENCODING       4 
#define asn1SccMySeq_REQUIRED_BITS_FOR_ENCODING        29

flag asn1SccMySeq_Encode(const asn1SccMySeq* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_MYSEQ		33  /**/
#define ERR_UPER_DECODE_MYSEQ_INPUT_DATA_2		13  /**/
#define ERR_UPER_DECODE_MYSEQ_OUTPUT_DATA_2		23  /**/
#define ERR_UPER_DECODE_MYSEQ_VALIDITY		28  /**/
flag asn1SccMySeq_Decode(asn1SccMySeq* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_MYSEQ		34  /**/
#define ERR_ACN_ENCODE_MYSEQ_INPUT_DATA_2		14  /**/
#define ERR_ACN_ENCODE_MYSEQ_OUTPUT_DATA_2		24  /**/
#define ERR_ACN_ENCODE_MYSEQ_VALIDITY		29  /**/
#define asn1SccMySeq_REQUIRED_BYTES_FOR_ACN_ENCODING       4 
#define asn1SccMySeq_REQUIRED_BITS_FOR_ACN_ENCODING        29

flag asn1SccMySeq_ACN_Encode(const asn1SccMySeq* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_MYSEQ		35  /**/
#define ERR_ACN_DECODE_MYSEQ_INPUT_DATA_2		15  /**/
#define ERR_ACN_DECODE_MYSEQ_OUTPUT_DATA_2		25  /**/
#define ERR_ACN_DECODE_MYSEQ_VALIDITY		30  /**/
flag asn1SccMySeq_ACN_Decode(asn1SccMySeq* pVal, BitStream* pBitStrm, int* pErrCode);
typedef asn1SccSint asn1SccT_Int32;


flag asn1SccT_Int32_Equal(const asn1SccT_Int32* pVal1, const asn1SccT_Int32* pVal2);

void asn1SccT_Int32_Initialize(asn1SccT_Int32* pVal);

#define ERR_T_INT32		36  /**/
flag asn1SccT_Int32_IsConstraintValid(const asn1SccT_Int32* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_T_INT32		37  /**/
#define asn1SccT_Int32_REQUIRED_BYTES_FOR_ENCODING       4 
#define asn1SccT_Int32_REQUIRED_BITS_FOR_ENCODING        32

flag asn1SccT_Int32_Encode(const asn1SccT_Int32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_T_INT32		38  /**/
flag asn1SccT_Int32_Decode(asn1SccT_Int32* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_T_INT32		39  /**/
#define asn1SccT_Int32_REQUIRED_BYTES_FOR_ACN_ENCODING       4 
#define asn1SccT_Int32_REQUIRED_BITS_FOR_ACN_ENCODING        32

flag asn1SccT_Int32_ACN_Encode(const asn1SccT_Int32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_T_INT32		40  /**/
flag asn1SccT_Int32_ACN_Decode(asn1SccT_Int32* pVal, BitStream* pBitStrm, int* pErrCode);
typedef asn1SccUint asn1SccT_UInt32;


flag asn1SccT_UInt32_Equal(const asn1SccT_UInt32* pVal1, const asn1SccT_UInt32* pVal2);

void asn1SccT_UInt32_Initialize(asn1SccT_UInt32* pVal);

#define ERR_T_UINT32		41  /**/
flag asn1SccT_UInt32_IsConstraintValid(const asn1SccT_UInt32* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_T_UINT32		42  /**/
#define asn1SccT_UInt32_REQUIRED_BYTES_FOR_ENCODING       4 
#define asn1SccT_UInt32_REQUIRED_BITS_FOR_ENCODING        32

flag asn1SccT_UInt32_Encode(const asn1SccT_UInt32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_T_UINT32		43  /**/
flag asn1SccT_UInt32_Decode(asn1SccT_UInt32* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_T_UINT32		44  /**/
#define asn1SccT_UInt32_REQUIRED_BYTES_FOR_ACN_ENCODING       4 
#define asn1SccT_UInt32_REQUIRED_BITS_FOR_ACN_ENCODING        32

flag asn1SccT_UInt32_ACN_Encode(const asn1SccT_UInt32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_T_UINT32		45  /**/
flag asn1SccT_UInt32_ACN_Decode(asn1SccT_UInt32* pVal, BitStream* pBitStrm, int* pErrCode);
typedef asn1SccSint asn1SccT_Int8;


flag asn1SccT_Int8_Equal(const asn1SccT_Int8* pVal1, const asn1SccT_Int8* pVal2);

void asn1SccT_Int8_Initialize(asn1SccT_Int8* pVal);

#define ERR_T_INT8		46  /**/
flag asn1SccT_Int8_IsConstraintValid(const asn1SccT_Int8* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_T_INT8		47  /**/
#define asn1SccT_Int8_REQUIRED_BYTES_FOR_ENCODING       1 
#define asn1SccT_Int8_REQUIRED_BITS_FOR_ENCODING        8

flag asn1SccT_Int8_Encode(const asn1SccT_Int8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_T_INT8		48  /**/
flag asn1SccT_Int8_Decode(asn1SccT_Int8* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_T_INT8		49  /**/
#define asn1SccT_Int8_REQUIRED_BYTES_FOR_ACN_ENCODING       1 
#define asn1SccT_Int8_REQUIRED_BITS_FOR_ACN_ENCODING        8

flag asn1SccT_Int8_ACN_Encode(const asn1SccT_Int8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_T_INT8		50  /**/
flag asn1SccT_Int8_ACN_Decode(asn1SccT_Int8* pVal, BitStream* pBitStrm, int* pErrCode);
typedef asn1SccUint asn1SccT_UInt8;


flag asn1SccT_UInt8_Equal(const asn1SccT_UInt8* pVal1, const asn1SccT_UInt8* pVal2);

void asn1SccT_UInt8_Initialize(asn1SccT_UInt8* pVal);

#define ERR_T_UINT8		51  /**/
flag asn1SccT_UInt8_IsConstraintValid(const asn1SccT_UInt8* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_T_UINT8		52  /**/
#define asn1SccT_UInt8_REQUIRED_BYTES_FOR_ENCODING       1 
#define asn1SccT_UInt8_REQUIRED_BITS_FOR_ENCODING        8

flag asn1SccT_UInt8_Encode(const asn1SccT_UInt8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_T_UINT8		53  /**/
flag asn1SccT_UInt8_Decode(asn1SccT_UInt8* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_T_UINT8		54  /**/
#define asn1SccT_UInt8_REQUIRED_BYTES_FOR_ACN_ENCODING       1 
#define asn1SccT_UInt8_REQUIRED_BITS_FOR_ACN_ENCODING        8

flag asn1SccT_UInt8_ACN_Encode(const asn1SccT_UInt8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_T_UINT8		55  /**/
flag asn1SccT_UInt8_ACN_Decode(asn1SccT_UInt8* pVal, BitStream* pBitStrm, int* pErrCode);
typedef flag asn1SccT_Boolean;


flag asn1SccT_Boolean_Equal(const asn1SccT_Boolean* pVal1, const asn1SccT_Boolean* pVal2);

void asn1SccT_Boolean_Initialize(asn1SccT_Boolean* pVal);

#define ERR_T_BOOLEAN		56  /**/
flag asn1SccT_Boolean_IsConstraintValid(const asn1SccT_Boolean* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_T_BOOLEAN		57  /**/
#define asn1SccT_Boolean_REQUIRED_BYTES_FOR_ENCODING       1 
#define asn1SccT_Boolean_REQUIRED_BITS_FOR_ENCODING        1

flag asn1SccT_Boolean_Encode(const asn1SccT_Boolean* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_T_BOOLEAN		58  /**/
flag asn1SccT_Boolean_Decode(asn1SccT_Boolean* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_T_BOOLEAN		59  /**/
#define asn1SccT_Boolean_REQUIRED_BYTES_FOR_ACN_ENCODING       1 
#define asn1SccT_Boolean_REQUIRED_BITS_FOR_ACN_ENCODING        1

flag asn1SccT_Boolean_ACN_Encode(const asn1SccT_Boolean* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_T_BOOLEAN		60  /**/
flag asn1SccT_Boolean_ACN_Decode(asn1SccT_Boolean* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- asn1SccT_Null_Record --------------------------------------------*/
typedef struct {

} asn1SccT_Null_Record;

flag asn1SccT_Null_Record_Equal(const asn1SccT_Null_Record* pVal1, const asn1SccT_Null_Record* pVal2);

void asn1SccT_Null_Record_Initialize(asn1SccT_Null_Record* pVal);

#define ERR_T_NULL_RECORD		61  /**/
flag asn1SccT_Null_Record_IsConstraintValid(const asn1SccT_Null_Record* pVal, int* pErrCode);

#define asn1SccT_Null_Record_REQUIRED_BYTES_FOR_ENCODING       0 
#define asn1SccT_Null_Record_REQUIRED_BITS_FOR_ENCODING        0

flag asn1SccT_Null_Record_Encode(const asn1SccT_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

flag asn1SccT_Null_Record_Decode(asn1SccT_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode);

#define asn1SccT_Null_Record_REQUIRED_BYTES_FOR_ACN_ENCODING       0 
#define asn1SccT_Null_Record_REQUIRED_BITS_FOR_ACN_ENCODING        0

flag asn1SccT_Null_Record_ACN_Encode(const asn1SccT_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

flag asn1SccT_Null_Record_ACN_Decode(asn1SccT_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
