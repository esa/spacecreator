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


typedef asn1SccSint asn1SccINumber;


flag asn1SccINumber_Equal(const asn1SccINumber* pVal1, const asn1SccINumber* pVal2);

#define ERR_INUMBER		1  /*(-1000 .. 1000)*/
flag asn1SccINumber_IsConstraintValid(const asn1SccINumber* pVal, int* pErrCode);

void asn1SccINumber_Initialize(asn1SccINumber* pVal);

#define ERR_UPER_ENCODE_INUMBER		2  /**/
#define asn1SccINumber_REQUIRED_BYTES_FOR_ENCODING       2 
#define asn1SccINumber_REQUIRED_BITS_FOR_ENCODING        11

flag asn1SccINumber_Encode(const asn1SccINumber* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_INUMBER		3  /**/
flag asn1SccINumber_Decode(asn1SccINumber* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_INUMBER		4  /**/
#define asn1SccINumber_REQUIRED_BYTES_FOR_ACN_ENCODING       2 
#define asn1SccINumber_REQUIRED_BITS_FOR_ACN_ENCODING        11

flag asn1SccINumber_ACN_Encode(const asn1SccINumber* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_INUMBER		5  /**/
flag asn1SccINumber_ACN_Decode(asn1SccINumber* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- asn1SccIPidConfig --------------------------------------------*/
typedef struct {
    asn1SccINumber kp;
    asn1SccINumber ki;
    asn1SccINumber kd;

} asn1SccIPidConfig;

flag asn1SccIPidConfig_Equal(const asn1SccIPidConfig* pVal1, const asn1SccIPidConfig* pVal2);

#define ERR_IPIDCONFIG		36  /**/
#define ERR_IPIDCONFIG_KP_2		11  /**/
#define ERR_IPIDCONFIG_KI_2		21  /**/
#define ERR_IPIDCONFIG_KD_2		31  /**/
flag asn1SccIPidConfig_IsConstraintValid(const asn1SccIPidConfig* pVal, int* pErrCode);

void asn1SccIPidConfig_Initialize(asn1SccIPidConfig* pVal);

#define ERR_UPER_ENCODE_IPIDCONFIG		37  /**/
#define ERR_UPER_ENCODE_IPIDCONFIG_KP_2		12  /**/
#define ERR_UPER_ENCODE_IPIDCONFIG_KI_2		22  /**/
#define ERR_UPER_ENCODE_IPIDCONFIG_KD_2		32  /**/
#define asn1SccIPidConfig_REQUIRED_BYTES_FOR_ENCODING       5 
#define asn1SccIPidConfig_REQUIRED_BITS_FOR_ENCODING        33

flag asn1SccIPidConfig_Encode(const asn1SccIPidConfig* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_IPIDCONFIG		38  /**/
#define ERR_UPER_DECODE_IPIDCONFIG_KP_2		13  /**/
#define ERR_UPER_DECODE_IPIDCONFIG_KI_2		23  /**/
#define ERR_UPER_DECODE_IPIDCONFIG_KD_2		33  /**/
flag asn1SccIPidConfig_Decode(asn1SccIPidConfig* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_IPIDCONFIG		39  /**/
#define ERR_ACN_ENCODE_IPIDCONFIG_KP_2		14  /**/
#define ERR_ACN_ENCODE_IPIDCONFIG_KI_2		24  /**/
#define ERR_ACN_ENCODE_IPIDCONFIG_KD_2		34  /**/
#define asn1SccIPidConfig_REQUIRED_BYTES_FOR_ACN_ENCODING       5 
#define asn1SccIPidConfig_REQUIRED_BITS_FOR_ACN_ENCODING        33

flag asn1SccIPidConfig_ACN_Encode(const asn1SccIPidConfig* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_IPIDCONFIG		40  /**/
#define ERR_ACN_DECODE_IPIDCONFIG_KP_2		15  /**/
#define ERR_ACN_DECODE_IPIDCONFIG_KI_2		25  /**/
#define ERR_ACN_DECODE_IPIDCONFIG_KD_2		35  /**/
flag asn1SccIPidConfig_ACN_Decode(asn1SccIPidConfig* pVal, BitStream* pBitStrm, int* pErrCode);
typedef asn1Real asn1SccNumber;


flag asn1SccNumber_Equal(const asn1SccNumber* pVal1, const asn1SccNumber* pVal2);

#define ERR_NUMBER		41  /*(-1000.0 .. 1000.0)*/
flag asn1SccNumber_IsConstraintValid(const asn1SccNumber* pVal, int* pErrCode);

void asn1SccNumber_Initialize(asn1SccNumber* pVal);

#define ERR_UPER_ENCODE_NUMBER		42  /**/
#define asn1SccNumber_REQUIRED_BYTES_FOR_ENCODING       13 
#define asn1SccNumber_REQUIRED_BITS_FOR_ENCODING        104

flag asn1SccNumber_Encode(const asn1SccNumber* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_NUMBER		43  /**/
flag asn1SccNumber_Decode(asn1SccNumber* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_NUMBER		44  /**/
#define asn1SccNumber_REQUIRED_BYTES_FOR_ACN_ENCODING       13 
#define asn1SccNumber_REQUIRED_BITS_FOR_ACN_ENCODING        104

flag asn1SccNumber_ACN_Encode(const asn1SccNumber* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_NUMBER		45  /**/
flag asn1SccNumber_ACN_Decode(asn1SccNumber* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- asn1SccPidConfig --------------------------------------------*/
typedef struct {
    asn1SccNumber kp;
    asn1SccNumber ki;
    asn1SccNumber kd;

} asn1SccPidConfig;

flag asn1SccPidConfig_Equal(const asn1SccPidConfig* pVal1, const asn1SccPidConfig* pVal2);

#define ERR_PIDCONFIG		76  /**/
#define ERR_PIDCONFIG_KP_2		51  /**/
#define ERR_PIDCONFIG_KI_2		61  /**/
#define ERR_PIDCONFIG_KD_2		71  /**/
flag asn1SccPidConfig_IsConstraintValid(const asn1SccPidConfig* pVal, int* pErrCode);

void asn1SccPidConfig_Initialize(asn1SccPidConfig* pVal);

#define ERR_UPER_ENCODE_PIDCONFIG		77  /**/
#define ERR_UPER_ENCODE_PIDCONFIG_KP_2		52  /**/
#define ERR_UPER_ENCODE_PIDCONFIG_KI_2		62  /**/
#define ERR_UPER_ENCODE_PIDCONFIG_KD_2		72  /**/
#define asn1SccPidConfig_REQUIRED_BYTES_FOR_ENCODING       39 
#define asn1SccPidConfig_REQUIRED_BITS_FOR_ENCODING        312

flag asn1SccPidConfig_Encode(const asn1SccPidConfig* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_PIDCONFIG		78  /**/
#define ERR_UPER_DECODE_PIDCONFIG_KP_2		53  /**/
#define ERR_UPER_DECODE_PIDCONFIG_KI_2		63  /**/
#define ERR_UPER_DECODE_PIDCONFIG_KD_2		73  /**/
flag asn1SccPidConfig_Decode(asn1SccPidConfig* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_PIDCONFIG		79  /**/
#define ERR_ACN_ENCODE_PIDCONFIG_KP_2		54  /**/
#define ERR_ACN_ENCODE_PIDCONFIG_KI_2		64  /**/
#define ERR_ACN_ENCODE_PIDCONFIG_KD_2		74  /**/
#define asn1SccPidConfig_REQUIRED_BYTES_FOR_ACN_ENCODING       39 
#define asn1SccPidConfig_REQUIRED_BITS_FOR_ACN_ENCODING        312

flag asn1SccPidConfig_ACN_Encode(const asn1SccPidConfig* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_PIDCONFIG		80  /**/
#define ERR_ACN_DECODE_PIDCONFIG_KP_2		55  /**/
#define ERR_ACN_DECODE_PIDCONFIG_KI_2		65  /**/
#define ERR_ACN_DECODE_PIDCONFIG_KD_2		75  /**/
flag asn1SccPidConfig_ACN_Decode(asn1SccPidConfig* pVal, BitStream* pBitStrm, int* pErrCode);
typedef asn1SccSint asn1SccT_Int32;


flag asn1SccT_Int32_Equal(const asn1SccT_Int32* pVal1, const asn1SccT_Int32* pVal2);

#define ERR_T_INT32		81  /*(-2147483648 .. 2147483647)*/
flag asn1SccT_Int32_IsConstraintValid(const asn1SccT_Int32* pVal, int* pErrCode);

void asn1SccT_Int32_Initialize(asn1SccT_Int32* pVal);

#define ERR_UPER_ENCODE_T_INT32		82  /**/
#define asn1SccT_Int32_REQUIRED_BYTES_FOR_ENCODING       4 
#define asn1SccT_Int32_REQUIRED_BITS_FOR_ENCODING        32

flag asn1SccT_Int32_Encode(const asn1SccT_Int32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_T_INT32		83  /**/
flag asn1SccT_Int32_Decode(asn1SccT_Int32* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_T_INT32		84  /**/
#define asn1SccT_Int32_REQUIRED_BYTES_FOR_ACN_ENCODING       4 
#define asn1SccT_Int32_REQUIRED_BITS_FOR_ACN_ENCODING        32

flag asn1SccT_Int32_ACN_Encode(const asn1SccT_Int32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_T_INT32		85  /**/
flag asn1SccT_Int32_ACN_Decode(asn1SccT_Int32* pVal, BitStream* pBitStrm, int* pErrCode);
typedef asn1SccUint asn1SccT_UInt32;


flag asn1SccT_UInt32_Equal(const asn1SccT_UInt32* pVal1, const asn1SccT_UInt32* pVal2);

#define ERR_T_UINT32		86  /*(0 .. 4294967295)*/
flag asn1SccT_UInt32_IsConstraintValid(const asn1SccT_UInt32* pVal, int* pErrCode);

void asn1SccT_UInt32_Initialize(asn1SccT_UInt32* pVal);

#define ERR_UPER_ENCODE_T_UINT32		87  /**/
#define asn1SccT_UInt32_REQUIRED_BYTES_FOR_ENCODING       4 
#define asn1SccT_UInt32_REQUIRED_BITS_FOR_ENCODING        32

flag asn1SccT_UInt32_Encode(const asn1SccT_UInt32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_T_UINT32		88  /**/
flag asn1SccT_UInt32_Decode(asn1SccT_UInt32* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_T_UINT32		89  /**/
#define asn1SccT_UInt32_REQUIRED_BYTES_FOR_ACN_ENCODING       4 
#define asn1SccT_UInt32_REQUIRED_BITS_FOR_ACN_ENCODING        32

flag asn1SccT_UInt32_ACN_Encode(const asn1SccT_UInt32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_T_UINT32		90  /**/
flag asn1SccT_UInt32_ACN_Decode(asn1SccT_UInt32* pVal, BitStream* pBitStrm, int* pErrCode);
typedef asn1SccSint asn1SccT_Int8;


flag asn1SccT_Int8_Equal(const asn1SccT_Int8* pVal1, const asn1SccT_Int8* pVal2);

#define ERR_T_INT8		91  /*(-128 .. 127)*/
flag asn1SccT_Int8_IsConstraintValid(const asn1SccT_Int8* pVal, int* pErrCode);

void asn1SccT_Int8_Initialize(asn1SccT_Int8* pVal);

#define ERR_UPER_ENCODE_T_INT8		92  /**/
#define asn1SccT_Int8_REQUIRED_BYTES_FOR_ENCODING       1 
#define asn1SccT_Int8_REQUIRED_BITS_FOR_ENCODING        8

flag asn1SccT_Int8_Encode(const asn1SccT_Int8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_T_INT8		93  /**/
flag asn1SccT_Int8_Decode(asn1SccT_Int8* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_T_INT8		94  /**/
#define asn1SccT_Int8_REQUIRED_BYTES_FOR_ACN_ENCODING       1 
#define asn1SccT_Int8_REQUIRED_BITS_FOR_ACN_ENCODING        8

flag asn1SccT_Int8_ACN_Encode(const asn1SccT_Int8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_T_INT8		95  /**/
flag asn1SccT_Int8_ACN_Decode(asn1SccT_Int8* pVal, BitStream* pBitStrm, int* pErrCode);
typedef asn1SccUint asn1SccT_UInt8;


flag asn1SccT_UInt8_Equal(const asn1SccT_UInt8* pVal1, const asn1SccT_UInt8* pVal2);

#define ERR_T_UINT8		96  /*(0 .. 255)*/
flag asn1SccT_UInt8_IsConstraintValid(const asn1SccT_UInt8* pVal, int* pErrCode);

void asn1SccT_UInt8_Initialize(asn1SccT_UInt8* pVal);

#define ERR_UPER_ENCODE_T_UINT8		97  /**/
#define asn1SccT_UInt8_REQUIRED_BYTES_FOR_ENCODING       1 
#define asn1SccT_UInt8_REQUIRED_BITS_FOR_ENCODING        8

flag asn1SccT_UInt8_Encode(const asn1SccT_UInt8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_T_UINT8		98  /**/
flag asn1SccT_UInt8_Decode(asn1SccT_UInt8* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_T_UINT8		99  /**/
#define asn1SccT_UInt8_REQUIRED_BYTES_FOR_ACN_ENCODING       1 
#define asn1SccT_UInt8_REQUIRED_BITS_FOR_ACN_ENCODING        8

flag asn1SccT_UInt8_ACN_Encode(const asn1SccT_UInt8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_T_UINT8		100  /**/
flag asn1SccT_UInt8_ACN_Decode(asn1SccT_UInt8* pVal, BitStream* pBitStrm, int* pErrCode);
typedef flag asn1SccT_Boolean;


flag asn1SccT_Boolean_Equal(const asn1SccT_Boolean* pVal1, const asn1SccT_Boolean* pVal2);

#define ERR_T_BOOLEAN		101  /**/
flag asn1SccT_Boolean_IsConstraintValid(const asn1SccT_Boolean* pVal, int* pErrCode);

void asn1SccT_Boolean_Initialize(asn1SccT_Boolean* pVal);

#define ERR_UPER_ENCODE_T_BOOLEAN		102  /**/
#define asn1SccT_Boolean_REQUIRED_BYTES_FOR_ENCODING       1 
#define asn1SccT_Boolean_REQUIRED_BITS_FOR_ENCODING        1

flag asn1SccT_Boolean_Encode(const asn1SccT_Boolean* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_T_BOOLEAN		103  /**/
flag asn1SccT_Boolean_Decode(asn1SccT_Boolean* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_T_BOOLEAN		104  /**/
#define asn1SccT_Boolean_REQUIRED_BYTES_FOR_ACN_ENCODING       1 
#define asn1SccT_Boolean_REQUIRED_BITS_FOR_ACN_ENCODING        1

flag asn1SccT_Boolean_ACN_Encode(const asn1SccT_Boolean* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_T_BOOLEAN		105  /**/
flag asn1SccT_Boolean_ACN_Decode(asn1SccT_Boolean* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- asn1SccT_Null_Record --------------------------------------------*/
typedef struct {

} asn1SccT_Null_Record;

flag asn1SccT_Null_Record_Equal(const asn1SccT_Null_Record* pVal1, const asn1SccT_Null_Record* pVal2);

#define ERR_T_NULL_RECORD		106  /**/
flag asn1SccT_Null_Record_IsConstraintValid(const asn1SccT_Null_Record* pVal, int* pErrCode);

void asn1SccT_Null_Record_Initialize(asn1SccT_Null_Record* pVal);

#define asn1SccT_Null_Record_REQUIRED_BYTES_FOR_ENCODING       0 
#define asn1SccT_Null_Record_REQUIRED_BITS_FOR_ENCODING        0

flag asn1SccT_Null_Record_Encode(const asn1SccT_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

flag asn1SccT_Null_Record_Decode(asn1SccT_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode);

#define asn1SccT_Null_Record_REQUIRED_BYTES_FOR_ACN_ENCODING       0 
#define asn1SccT_Null_Record_REQUIRED_BITS_FOR_ACN_ENCODING        0

flag asn1SccT_Null_Record_ACN_Encode(const asn1SccT_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

flag asn1SccT_Null_Record_ACN_Decode(asn1SccT_Null_Record* pVal, BitStream* pBitStrm, int* pErrCode);
typedef asn1SccUint asn1SccPID_Range;


flag asn1SccPID_Range_Equal(const asn1SccPID_Range* pVal1, const asn1SccPID_Range* pVal2);

#define ERR_PID_RANGE		111  /*(0..4)*/
flag asn1SccPID_Range_IsConstraintValid(const asn1SccPID_Range* pVal, int* pErrCode);

void asn1SccPID_Range_Initialize(asn1SccPID_Range* pVal);

#define ERR_UPER_ENCODE_PID_RANGE		112  /**/
#define asn1SccPID_Range_REQUIRED_BYTES_FOR_ENCODING       1 
#define asn1SccPID_Range_REQUIRED_BITS_FOR_ENCODING        3

flag asn1SccPID_Range_Encode(const asn1SccPID_Range* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_PID_RANGE		113  /**/
flag asn1SccPID_Range_Decode(asn1SccPID_Range* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_PID_RANGE		114  /**/
#define asn1SccPID_Range_REQUIRED_BYTES_FOR_ACN_ENCODING       1 
#define asn1SccPID_Range_REQUIRED_BITS_FOR_ACN_ENCODING        3

flag asn1SccPID_Range_ACN_Encode(const asn1SccPID_Range* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_PID_RANGE		115  /**/
flag asn1SccPID_Range_ACN_Decode(asn1SccPID_Range* pVal, BitStream* pBitStrm, int* pErrCode);
typedef enum {
    PID_actuator = 0,
    PID_controlsystem = 1,
    PID_host = 2,
    PID_sensor = 3,
    PID_env = 4
} asn1SccPID;

// please use the following macros to avoid breaking code.
#define asn1SccPID_actuator PID_actuator
#define asn1SccPID_controlsystem PID_controlsystem
#define asn1SccPID_host PID_host
#define asn1SccPID_sensor PID_sensor
#define asn1SccPID_env PID_env

flag asn1SccPID_Equal(const asn1SccPID* pVal1, const asn1SccPID* pVal2);

#define ERR_PID		116  /*actuator | controlsystem | host | sensor | env*/
flag asn1SccPID_IsConstraintValid(const asn1SccPID* pVal, int* pErrCode);

void asn1SccPID_Initialize(asn1SccPID* pVal);

#define ERR_UPER_ENCODE_PID		117  /**/
#define asn1SccPID_REQUIRED_BYTES_FOR_ENCODING       1 
#define asn1SccPID_REQUIRED_BITS_FOR_ENCODING        3

flag asn1SccPID_Encode(const asn1SccPID* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_PID		118  /**/
flag asn1SccPID_Decode(asn1SccPID* pVal, BitStream* pBitStrm, int* pErrCode);

#define ERR_ACN_ENCODE_PID		119  /**/
#define asn1SccPID_REQUIRED_BYTES_FOR_ACN_ENCODING       1 
#define asn1SccPID_REQUIRED_BITS_FOR_ACN_ENCODING        3

flag asn1SccPID_ACN_Encode(const asn1SccPID* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_ACN_DECODE_PID		120  /**/
flag asn1SccPID_ACN_Decode(asn1SccPID* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
