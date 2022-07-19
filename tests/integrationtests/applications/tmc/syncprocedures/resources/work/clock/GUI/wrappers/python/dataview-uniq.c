/*
Code automatically generated by asn1scc tool
*/
#include <limits.h>
#include <string.h>
#include <math.h>

#include "asn1crt_encoding.h"
#include "asn1crt_encoding_uper.h"
#include "asn1crt_encoding_acn.h"

#include "dataview-uniq.h"




flag MyInteger_Equal(const MyInteger* pVal1, const MyInteger* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

void MyInteger_Initialize(MyInteger* pVal)
{
	(void)pVal;


	(*(pVal)) = 0;
}

flag MyInteger_IsConstraintValid(const MyInteger* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = ((*(pVal)) <= 10000UL);
    *pErrCode = ret ? 0 :  ERR_MYINTEGER; 

	return ret;
}

flag MyInteger_Encode(const MyInteger* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? MyInteger_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 10000);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag MyInteger_Decode(MyInteger* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 10000);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_MYINTEGER;

	return ret  && MyInteger_IsConstraintValid(pVal, pErrCode);
}

flag MyInteger_ACN_Encode(const MyInteger* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? MyInteger_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 10000);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag MyInteger_ACN_Decode(MyInteger* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 10000);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_MYINTEGER;

    return ret && MyInteger_IsConstraintValid(pVal, pErrCode);
}



flag T_Int32_Equal(const T_Int32* pVal1, const T_Int32* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

void T_Int32_Initialize(T_Int32* pVal)
{
	(void)pVal;


	(*(pVal)) = 0;
}

flag T_Int32_IsConstraintValid(const T_Int32* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = ((-2147483648LL <= (*(pVal))) && ((*(pVal)) <= 2147483647LL));
    *pErrCode = ret ? 0 :  ERR_T_INT32; 

	return ret;
}

flag T_Int32_Encode(const T_Int32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? T_Int32_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintWholeNumber(pBitStrm, (*(pVal)), -2147483648LL, 2147483647LL);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_Int32_Decode(T_Int32* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, pVal, -2147483648LL, 2147483647LL);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_INT32;

	return ret  && T_Int32_IsConstraintValid(pVal, pErrCode);
}

flag T_Int32_ACN_Encode(const T_Int32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? T_Int32_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintWholeNumber(pBitStrm, (*(pVal)), -2147483648LL, 2147483647LL);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_Int32_ACN_Decode(T_Int32* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, pVal, -2147483648LL, 2147483647LL);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_T_INT32;

    return ret && T_Int32_IsConstraintValid(pVal, pErrCode);
}



flag T_UInt32_Equal(const T_UInt32* pVal1, const T_UInt32* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

void T_UInt32_Initialize(T_UInt32* pVal)
{
	(void)pVal;


	(*(pVal)) = 0;
}

flag T_UInt32_IsConstraintValid(const T_UInt32* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = ((*(pVal)) <= 4294967295UL);
    *pErrCode = ret ? 0 :  ERR_T_UINT32; 

	return ret;
}

flag T_UInt32_Encode(const T_UInt32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? T_UInt32_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 4294967295LL);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_UInt32_Decode(T_UInt32* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 4294967295LL);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_UINT32;

	return ret  && T_UInt32_IsConstraintValid(pVal, pErrCode);
}

flag T_UInt32_ACN_Encode(const T_UInt32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? T_UInt32_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 4294967295LL);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_UInt32_ACN_Decode(T_UInt32* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 4294967295LL);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_T_UINT32;

    return ret && T_UInt32_IsConstraintValid(pVal, pErrCode);
}



flag T_Int8_Equal(const T_Int8* pVal1, const T_Int8* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

void T_Int8_Initialize(T_Int8* pVal)
{
	(void)pVal;


	(*(pVal)) = 0;
}

flag T_Int8_IsConstraintValid(const T_Int8* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = ((-128LL <= (*(pVal))) && ((*(pVal)) <= 127LL));
    *pErrCode = ret ? 0 :  ERR_T_INT8; 

	return ret;
}

flag T_Int8_Encode(const T_Int8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? T_Int8_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintWholeNumber(pBitStrm, (*(pVal)), -128, 127);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_Int8_Decode(T_Int8* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, pVal, -128, 127);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_INT8;

	return ret  && T_Int8_IsConstraintValid(pVal, pErrCode);
}

flag T_Int8_ACN_Encode(const T_Int8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? T_Int8_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintWholeNumber(pBitStrm, (*(pVal)), -128, 127);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_Int8_ACN_Decode(T_Int8* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, pVal, -128, 127);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_T_INT8;

    return ret && T_Int8_IsConstraintValid(pVal, pErrCode);
}



flag T_UInt8_Equal(const T_UInt8* pVal1, const T_UInt8* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

void T_UInt8_Initialize(T_UInt8* pVal)
{
	(void)pVal;


	(*(pVal)) = 0;
}

flag T_UInt8_IsConstraintValid(const T_UInt8* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = ((*(pVal)) <= 255UL);
    *pErrCode = ret ? 0 :  ERR_T_UINT8; 

	return ret;
}

flag T_UInt8_Encode(const T_UInt8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? T_UInt8_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 255);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_UInt8_Decode(T_UInt8* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 255);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_UINT8;

	return ret  && T_UInt8_IsConstraintValid(pVal, pErrCode);
}

flag T_UInt8_ACN_Encode(const T_UInt8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? T_UInt8_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 255);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_UInt8_ACN_Decode(T_UInt8* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 255);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_T_UINT8;

    return ret && T_UInt8_IsConstraintValid(pVal, pErrCode);
}



flag T_Boolean_Equal(const T_Boolean* pVal1, const T_Boolean* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

void T_Boolean_Initialize(T_Boolean* pVal)
{
	(void)pVal;


	(*(pVal)) = FALSE;
}

flag T_Boolean_IsConstraintValid(const T_Boolean* pVal, int* pErrCode)
{
    flag ret = TRUE;
	(void)pVal;
    ret = TRUE;
    *pErrCode = 0;

	return ret;
}

flag T_Boolean_Encode(const T_Boolean* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? T_Boolean_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_AppendBit(pBitStrm,(*(pVal)));
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_Boolean_Decode(T_Boolean* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_ReadBit(pBitStrm, pVal);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_BOOLEAN;

	return ret  && T_Boolean_IsConstraintValid(pVal, pErrCode);
}

flag T_Boolean_ACN_Encode(const T_Boolean* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? T_Boolean_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_AppendBit(pBitStrm,(*(pVal)));
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_Boolean_ACN_Decode(T_Boolean* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_ReadBit(pBitStrm, pVal);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_T_BOOLEAN;

    return ret && T_Boolean_IsConstraintValid(pVal, pErrCode);
}



flag T_Null_Record_Equal(const T_Null_Record* pVal1, const T_Null_Record* pVal2)
{
	(void)pVal1;
	(void)pVal2;
	return TRUE;

}

void T_Null_Record_Initialize(T_Null_Record* pVal)
{
	(void)pVal;


}

flag T_Null_Record_IsConstraintValid(const T_Null_Record* pVal, int* pErrCode)
{
    flag ret = TRUE;
	(void)pVal;
    ret = TRUE;
    *pErrCode = 0;

	return ret;
}

flag T_Null_Record_Encode(const T_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	(void)pBitStrm;


	*pErrCode = 0;
	ret = bCheckConstraints ? T_Null_Record_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_Null_Record_Decode(T_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;
	(void)pBitStrm;



	return ret  && T_Null_Record_IsConstraintValid(pVal, pErrCode);
}

flag T_Null_Record_ACN_Encode(const T_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	(void)pBitStrm;

    *pErrCode = 0;
	ret = bCheckConstraints ? T_Null_Record_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_Null_Record_ACN_Decode(T_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	(void)pBitStrm;
	*pErrCode = 0;



    return ret && T_Null_Record_IsConstraintValid(pVal, pErrCode);
}

