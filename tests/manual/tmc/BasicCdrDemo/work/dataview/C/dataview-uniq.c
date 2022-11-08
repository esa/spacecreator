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




flag asn1SccTInteger_Equal(const asn1SccTInteger* pVal1, const asn1SccTInteger* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

flag asn1SccTInteger_IsConstraintValid(const asn1SccTInteger* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = ((*(pVal)) <= 1000000UL);
    *pErrCode = ret ? 0 :  ERR_TINTEGER; 

	return ret;
}

void asn1SccTInteger_Initialize(asn1SccTInteger* pVal)
{
	(void)pVal;


	(*(pVal)) = 0;
}

flag asn1SccTInteger_Encode(const asn1SccTInteger* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? asn1SccTInteger_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 1000000);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccTInteger_Decode(asn1SccTInteger* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 1000000);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_TINTEGER;

	return ret  && asn1SccTInteger_IsConstraintValid(pVal, pErrCode);
}

flag asn1SccTInteger_ACN_Encode(const asn1SccTInteger* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? asn1SccTInteger_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 1000000);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccTInteger_ACN_Decode(asn1SccTInteger* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 1000000);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_TINTEGER;

    return ret && asn1SccTInteger_IsConstraintValid(pVal, pErrCode);
}



flag asn1SccTConfig_Equal(const asn1SccTConfig* pVal1, const asn1SccTConfig* pVal2)
{
	flag ret=TRUE;

    ret = (pVal1->param_a == pVal2->param_a);

    if (ret) {
        ret = (pVal1->param_b == pVal2->param_b);

    }

	return ret;

}

flag asn1SccTConfig_IsConstraintValid(const asn1SccTConfig* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = asn1SccTInteger_IsConstraintValid((&(pVal->param_a)), pErrCode);
    if (ret) {
        ret = asn1SccTInteger_IsConstraintValid((&(pVal->param_b)), pErrCode);
    }   /*COVERAGE_IGNORE*/

	return ret;
}

void asn1SccTConfig_Initialize(asn1SccTConfig* pVal)
{
	(void)pVal;



	/*set param_a */
	asn1SccTInteger_Initialize((&(pVal->param_a)));
	/*set param_b */
	asn1SccTInteger_Initialize((&(pVal->param_b)));
}

flag asn1SccTConfig_Encode(const asn1SccTConfig* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? asn1SccTConfig_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    /*Encode param_a */
	    ret = asn1SccTInteger_Encode((&(pVal->param_a)), pBitStrm, pErrCode, FALSE);
	    if (ret) {
	        /*Encode param_b */
	        ret = asn1SccTInteger_Encode((&(pVal->param_b)), pBitStrm, pErrCode, FALSE);
	    }   /*COVERAGE_IGNORE*/
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccTConfig_Decode(asn1SccTConfig* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	/*Decode param_a */
	ret = asn1SccTInteger_Decode((&(pVal->param_a)), pBitStrm, pErrCode);
	if (ret) {
	    /*Decode param_b */
	    ret = asn1SccTInteger_Decode((&(pVal->param_b)), pBitStrm, pErrCode);
	}   /*COVERAGE_IGNORE*/

	return ret  && asn1SccTConfig_IsConstraintValid(pVal, pErrCode);
}

flag asn1SccTConfig_ACN_Encode(const asn1SccTConfig* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? asn1SccTConfig_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    /*Encode param_a */
	    ret = asn1SccTInteger_ACN_Encode((&(pVal->param_a)), pBitStrm, pErrCode, FALSE);
	    if (ret) {
	        /*Encode param_b */
	        ret = asn1SccTInteger_ACN_Encode((&(pVal->param_b)), pBitStrm, pErrCode, FALSE);
	    }   /*COVERAGE_IGNORE*/
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccTConfig_ACN_Decode(asn1SccTConfig* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	/*Decode param_a */
	ret = asn1SccTInteger_ACN_Decode((&(pVal->param_a)), pBitStrm, pErrCode);
	if (ret) {
	    /*Decode param_b */
	    ret = asn1SccTInteger_ACN_Decode((&(pVal->param_b)), pBitStrm, pErrCode);
	}   /*COVERAGE_IGNORE*/

    return ret && asn1SccTConfig_IsConstraintValid(pVal, pErrCode);
}



flag asn1SccTReport_Equal(const asn1SccTReport* pVal1, const asn1SccTReport* pVal2)
{
	flag ret=TRUE;

    ret = (pVal1->reading == pVal2->reading);

    if (ret) {
        ret = (pVal1->ctrl == pVal2->ctrl);

        if (ret) {
            ret = (pVal1->feedback == pVal2->feedback);

        }

    }

	return ret;

}

flag asn1SccTReport_IsConstraintValid(const asn1SccTReport* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = asn1SccTInteger_IsConstraintValid((&(pVal->reading)), pErrCode);
    if (ret) {
        ret = asn1SccTInteger_IsConstraintValid((&(pVal->ctrl)), pErrCode);
        if (ret) {
            ret = asn1SccTInteger_IsConstraintValid((&(pVal->feedback)), pErrCode);
        }   /*COVERAGE_IGNORE*/
    }   /*COVERAGE_IGNORE*/

	return ret;
}

void asn1SccTReport_Initialize(asn1SccTReport* pVal)
{
	(void)pVal;



	/*set reading */
	asn1SccTInteger_Initialize((&(pVal->reading)));
	/*set ctrl */
	asn1SccTInteger_Initialize((&(pVal->ctrl)));
	/*set feedback */
	asn1SccTInteger_Initialize((&(pVal->feedback)));
}

flag asn1SccTReport_Encode(const asn1SccTReport* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? asn1SccTReport_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    /*Encode reading */
	    ret = asn1SccTInteger_Encode((&(pVal->reading)), pBitStrm, pErrCode, FALSE);
	    if (ret) {
	        /*Encode ctrl */
	        ret = asn1SccTInteger_Encode((&(pVal->ctrl)), pBitStrm, pErrCode, FALSE);
	        if (ret) {
	            /*Encode feedback */
	            ret = asn1SccTInteger_Encode((&(pVal->feedback)), pBitStrm, pErrCode, FALSE);
	        }   /*COVERAGE_IGNORE*/
	    }   /*COVERAGE_IGNORE*/
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccTReport_Decode(asn1SccTReport* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	/*Decode reading */
	ret = asn1SccTInteger_Decode((&(pVal->reading)), pBitStrm, pErrCode);
	if (ret) {
	    /*Decode ctrl */
	    ret = asn1SccTInteger_Decode((&(pVal->ctrl)), pBitStrm, pErrCode);
	    if (ret) {
	        /*Decode feedback */
	        ret = asn1SccTInteger_Decode((&(pVal->feedback)), pBitStrm, pErrCode);
	    }   /*COVERAGE_IGNORE*/
	}   /*COVERAGE_IGNORE*/

	return ret  && asn1SccTReport_IsConstraintValid(pVal, pErrCode);
}

flag asn1SccTReport_ACN_Encode(const asn1SccTReport* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? asn1SccTReport_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    /*Encode reading */
	    ret = asn1SccTInteger_ACN_Encode((&(pVal->reading)), pBitStrm, pErrCode, FALSE);
	    if (ret) {
	        /*Encode ctrl */
	        ret = asn1SccTInteger_ACN_Encode((&(pVal->ctrl)), pBitStrm, pErrCode, FALSE);
	        if (ret) {
	            /*Encode feedback */
	            ret = asn1SccTInteger_ACN_Encode((&(pVal->feedback)), pBitStrm, pErrCode, FALSE);
	        }   /*COVERAGE_IGNORE*/
	    }   /*COVERAGE_IGNORE*/
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccTReport_ACN_Decode(asn1SccTReport* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	/*Decode reading */
	ret = asn1SccTInteger_ACN_Decode((&(pVal->reading)), pBitStrm, pErrCode);
	if (ret) {
	    /*Decode ctrl */
	    ret = asn1SccTInteger_ACN_Decode((&(pVal->ctrl)), pBitStrm, pErrCode);
	    if (ret) {
	        /*Decode feedback */
	        ret = asn1SccTInteger_ACN_Decode((&(pVal->feedback)), pBitStrm, pErrCode);
	    }   /*COVERAGE_IGNORE*/
	}   /*COVERAGE_IGNORE*/

    return ret && asn1SccTReport_IsConstraintValid(pVal, pErrCode);
}



flag asn1SccT_Int32_Equal(const asn1SccT_Int32* pVal1, const asn1SccT_Int32* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

flag asn1SccT_Int32_IsConstraintValid(const asn1SccT_Int32* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = ((-2147483648L <= (*(pVal))) && ((*(pVal)) <= 2147483647L));
    *pErrCode = ret ? 0 :  ERR_T_INT32; 

	return ret;
}

void asn1SccT_Int32_Initialize(asn1SccT_Int32* pVal)
{
	(void)pVal;


	(*(pVal)) = 0;
}

flag asn1SccT_Int32_Encode(const asn1SccT_Int32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? asn1SccT_Int32_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintWholeNumber(pBitStrm, (*(pVal)), -2147483648LL, 2147483647LL);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccT_Int32_Decode(asn1SccT_Int32* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, pVal, -2147483648LL, 2147483647LL);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_INT32;

	return ret  && asn1SccT_Int32_IsConstraintValid(pVal, pErrCode);
}

flag asn1SccT_Int32_ACN_Encode(const asn1SccT_Int32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? asn1SccT_Int32_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintWholeNumber(pBitStrm, (*(pVal)), -2147483648LL, 2147483647LL);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccT_Int32_ACN_Decode(asn1SccT_Int32* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, pVal, -2147483648LL, 2147483647LL);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_T_INT32;

    return ret && asn1SccT_Int32_IsConstraintValid(pVal, pErrCode);
}



flag asn1SccT_UInt32_Equal(const asn1SccT_UInt32* pVal1, const asn1SccT_UInt32* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

flag asn1SccT_UInt32_IsConstraintValid(const asn1SccT_UInt32* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = ((*(pVal)) <= 4294967295UL);
    *pErrCode = ret ? 0 :  ERR_T_UINT32; 

	return ret;
}

void asn1SccT_UInt32_Initialize(asn1SccT_UInt32* pVal)
{
	(void)pVal;


	(*(pVal)) = 0;
}

flag asn1SccT_UInt32_Encode(const asn1SccT_UInt32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? asn1SccT_UInt32_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 4294967295LL);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccT_UInt32_Decode(asn1SccT_UInt32* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 4294967295LL);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_UINT32;

	return ret  && asn1SccT_UInt32_IsConstraintValid(pVal, pErrCode);
}

flag asn1SccT_UInt32_ACN_Encode(const asn1SccT_UInt32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? asn1SccT_UInt32_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 4294967295LL);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccT_UInt32_ACN_Decode(asn1SccT_UInt32* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 4294967295LL);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_T_UINT32;

    return ret && asn1SccT_UInt32_IsConstraintValid(pVal, pErrCode);
}



flag asn1SccT_Int8_Equal(const asn1SccT_Int8* pVal1, const asn1SccT_Int8* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

flag asn1SccT_Int8_IsConstraintValid(const asn1SccT_Int8* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = ((-128L <= (*(pVal))) && ((*(pVal)) <= 127L));
    *pErrCode = ret ? 0 :  ERR_T_INT8; 

	return ret;
}

void asn1SccT_Int8_Initialize(asn1SccT_Int8* pVal)
{
	(void)pVal;


	(*(pVal)) = 0;
}

flag asn1SccT_Int8_Encode(const asn1SccT_Int8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? asn1SccT_Int8_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintWholeNumber(pBitStrm, (*(pVal)), -128, 127);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccT_Int8_Decode(asn1SccT_Int8* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, pVal, -128, 127);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_INT8;

	return ret  && asn1SccT_Int8_IsConstraintValid(pVal, pErrCode);
}

flag asn1SccT_Int8_ACN_Encode(const asn1SccT_Int8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? asn1SccT_Int8_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintWholeNumber(pBitStrm, (*(pVal)), -128, 127);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccT_Int8_ACN_Decode(asn1SccT_Int8* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, pVal, -128, 127);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_T_INT8;

    return ret && asn1SccT_Int8_IsConstraintValid(pVal, pErrCode);
}



flag asn1SccT_UInt8_Equal(const asn1SccT_UInt8* pVal1, const asn1SccT_UInt8* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

flag asn1SccT_UInt8_IsConstraintValid(const asn1SccT_UInt8* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = ((*(pVal)) <= 255UL);
    *pErrCode = ret ? 0 :  ERR_T_UINT8; 

	return ret;
}

void asn1SccT_UInt8_Initialize(asn1SccT_UInt8* pVal)
{
	(void)pVal;


	(*(pVal)) = 0;
}

flag asn1SccT_UInt8_Encode(const asn1SccT_UInt8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? asn1SccT_UInt8_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 255);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccT_UInt8_Decode(asn1SccT_UInt8* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 255);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_UINT8;

	return ret  && asn1SccT_UInt8_IsConstraintValid(pVal, pErrCode);
}

flag asn1SccT_UInt8_ACN_Encode(const asn1SccT_UInt8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? asn1SccT_UInt8_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 255);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccT_UInt8_ACN_Decode(asn1SccT_UInt8* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 255);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_T_UINT8;

    return ret && asn1SccT_UInt8_IsConstraintValid(pVal, pErrCode);
}



flag asn1SccT_Boolean_Equal(const asn1SccT_Boolean* pVal1, const asn1SccT_Boolean* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

flag asn1SccT_Boolean_IsConstraintValid(const asn1SccT_Boolean* pVal, int* pErrCode)
{
    flag ret = TRUE;
	(void)pVal;
    ret = TRUE;
    *pErrCode = 0;

	return ret;
}

void asn1SccT_Boolean_Initialize(asn1SccT_Boolean* pVal)
{
	(void)pVal;


	(*(pVal)) = FALSE;
}

flag asn1SccT_Boolean_Encode(const asn1SccT_Boolean* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? asn1SccT_Boolean_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_AppendBit(pBitStrm,(*(pVal)));
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccT_Boolean_Decode(asn1SccT_Boolean* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_ReadBit(pBitStrm, pVal);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_BOOLEAN;

	return ret  && asn1SccT_Boolean_IsConstraintValid(pVal, pErrCode);
}

flag asn1SccT_Boolean_ACN_Encode(const asn1SccT_Boolean* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? asn1SccT_Boolean_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_AppendBit(pBitStrm,(*(pVal)));
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccT_Boolean_ACN_Decode(asn1SccT_Boolean* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_ReadBit(pBitStrm, pVal);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_T_BOOLEAN;

    return ret && asn1SccT_Boolean_IsConstraintValid(pVal, pErrCode);
}



flag asn1SccT_Null_Record_Equal(const asn1SccT_Null_Record* pVal1, const asn1SccT_Null_Record* pVal2)
{
	(void)pVal1;
	(void)pVal2;
	return TRUE;

}

flag asn1SccT_Null_Record_IsConstraintValid(const asn1SccT_Null_Record* pVal, int* pErrCode)
{
    flag ret = TRUE;
	(void)pVal;
    ret = TRUE;
    *pErrCode = 0;

	return ret;
}

void asn1SccT_Null_Record_Initialize(asn1SccT_Null_Record* pVal)
{
	(void)pVal;


}

flag asn1SccT_Null_Record_Encode(const asn1SccT_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	(void)pBitStrm;


	*pErrCode = 0;
	ret = bCheckConstraints ? asn1SccT_Null_Record_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccT_Null_Record_Decode(asn1SccT_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;
	(void)pBitStrm;



	return ret  && asn1SccT_Null_Record_IsConstraintValid(pVal, pErrCode);
}

flag asn1SccT_Null_Record_ACN_Encode(const asn1SccT_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	(void)pBitStrm;

    *pErrCode = 0;
	ret = bCheckConstraints ? asn1SccT_Null_Record_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccT_Null_Record_ACN_Decode(asn1SccT_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	(void)pBitStrm;
	*pErrCode = 0;



    return ret && asn1SccT_Null_Record_IsConstraintValid(pVal, pErrCode);
}



flag asn1SccPID_Range_Equal(const asn1SccPID_Range* pVal1, const asn1SccPID_Range* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

flag asn1SccPID_Range_IsConstraintValid(const asn1SccPID_Range* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = ((*(pVal)) <= 4UL);
    *pErrCode = ret ? 0 :  ERR_PID_RANGE; 

	return ret;
}

void asn1SccPID_Range_Initialize(asn1SccPID_Range* pVal)
{
	(void)pVal;


	(*(pVal)) = 0;
}

flag asn1SccPID_Range_Encode(const asn1SccPID_Range* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? asn1SccPID_Range_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 4);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccPID_Range_Decode(asn1SccPID_Range* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 4);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_PID_RANGE;

	return ret  && asn1SccPID_Range_IsConstraintValid(pVal, pErrCode);
}

flag asn1SccPID_Range_ACN_Encode(const asn1SccPID_Range* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

    *pErrCode = 0;
	ret = bCheckConstraints ? asn1SccPID_Range_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 4);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccPID_Range_ACN_Decode(asn1SccPID_Range* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 4);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_PID_RANGE;

    return ret && asn1SccPID_Range_IsConstraintValid(pVal, pErrCode);
}



flag asn1SccPID_Equal(const asn1SccPID* pVal1, const asn1SccPID* pVal2)
{
	return (*(pVal1)) == (*(pVal2));

}

flag asn1SccPID_IsConstraintValid(const asn1SccPID* pVal, int* pErrCode)
{
    flag ret = TRUE;
    ret = ((((((((((*(pVal)) == PID_actuator)) || (((*(pVal)) == PID_egse)))) || (((*(pVal)) == PID_modemanager)))) || (((*(pVal)) == PID_sensor)))) || (((*(pVal)) == PID_env)));
    *pErrCode = ret ? 0 :  ERR_PID; 

	return ret;
}

void asn1SccPID_Initialize(asn1SccPID* pVal)
{
	(void)pVal;


	(*(pVal)) = PID_actuator;
}

flag asn1SccPID_Encode(const asn1SccPID* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;


	*pErrCode = 0;
	ret = bCheckConstraints ? asn1SccPID_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    switch((*(pVal))) 
	    {
	        case PID_actuator:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 0, 0, 4);
	        	break;
	        case PID_egse:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 1, 0, 4);
	        	break;
	        case PID_modemanager:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 2, 0, 4);
	        	break;
	        case PID_sensor:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 3, 0, 4);
	        	break;
	        case PID_env:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 4, 0, 4);
	        	break;
	        default:                    /*COVERAGE_IGNORE*/
	    	    *pErrCode = ERR_UPER_ENCODE_PID; /*COVERAGE_IGNORE*/
	    	    ret = FALSE;            /*COVERAGE_IGNORE*/
	    }
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccPID_Decode(asn1SccPID* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;


	{
	    asn1SccSint enumIndex;
	    ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, &enumIndex, 0, 4);
	    *pErrCode = ret ? 0 : ERR_UPER_DECODE_PID;
	    if (ret) {
	        switch(enumIndex) 
	        {
	            case 0: 
	                (*(pVal)) = PID_actuator;
	                break;
	            case 1: 
	                (*(pVal)) = PID_egse;
	                break;
	            case 2: 
	                (*(pVal)) = PID_modemanager;
	                break;
	            case 3: 
	                (*(pVal)) = PID_sensor;
	                break;
	            case 4: 
	                (*(pVal)) = PID_env;
	                break;
	            default:                        /*COVERAGE_IGNORE*/
		            *pErrCode = ERR_UPER_DECODE_PID;     /*COVERAGE_IGNORE*/
		            ret = FALSE;                /*COVERAGE_IGNORE*/
	        }
	    } else {
	        (*(pVal)) = PID_actuator;             /*COVERAGE_IGNORE*/
	    }
	}

	return ret  && asn1SccPID_IsConstraintValid(pVal, pErrCode);
}

flag asn1SccPID_ACN_Encode(const asn1SccPID* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;

	asn1SccUint intVal;
    *pErrCode = 0;
	ret = bCheckConstraints ? asn1SccPID_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret && *pErrCode == 0) {
	    switch((*(pVal))) { 
	        case PID_actuator:
	            intVal = 0;
	            break;
	        case PID_egse:
	            intVal = 1;
	            break;
	        case PID_modemanager:
	            intVal = 2;
	            break;
	        case PID_sensor:
	            intVal = 3;
	            break;
	        case PID_env:
	            intVal = 4;
	            break;
	        default:                                    /*COVERAGE_IGNORE*/
	            ret = FALSE;                            /*COVERAGE_IGNORE*/
	            *pErrCode = ERR_ACN_ENCODE_PID;                 /*COVERAGE_IGNORE*/
	    }
	    if (ret) {
	    	BitStream_EncodeConstraintPosWholeNumber(pBitStrm, intVal, 0, 4);
	    }
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag asn1SccPID_ACN_Decode(asn1SccPID* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;
	*pErrCode = 0;

	asn1SccUint intVal;

	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, (&(intVal)), 0, 4);
	*pErrCode = ret ? 0 : ERR_ACN_DECODE_PID;
	if (ret) {
	    switch (intVal) {
	        case 0:
	            (*(pVal)) = PID_actuator;
	            break;
	        case 1:
	            (*(pVal)) = PID_egse;
	            break;
	        case 2:
	            (*(pVal)) = PID_modemanager;
	            break;
	        case 3:
	            (*(pVal)) = PID_sensor;
	            break;
	        case 4:
	            (*(pVal)) = PID_env;
	            break;
	    default:                                    /*COVERAGE_IGNORE*/
	        ret = FALSE;                            /*COVERAGE_IGNORE*/
	        *pErrCode = ERR_ACN_DECODE_PID;                 /*COVERAGE_IGNORE*/
	    }
	} /*COVERAGE_IGNORE*/

    return ret && asn1SccPID_IsConstraintValid(pVal, pErrCode);
}

