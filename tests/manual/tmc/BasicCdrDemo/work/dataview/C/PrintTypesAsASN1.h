#ifndef __PRINTTYPESASASN1_H__
#define __PRINTTYPESASASN1_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "dataview-uniq.h" // Generated by ASN1SCC

void PrintASN1T_Integer(const char *paramName, const asn1SccT_Integer *pData);
void PrintASN1T_Config(const char *paramName, const asn1SccT_Config *pData);
void PrintASN1T_Report(const char *paramName, const asn1SccT_Report *pData);
void PrintASN1T_Int32(const char *paramName, const asn1SccT_Int32 *pData);
void PrintASN1T_UInt32(const char *paramName, const asn1SccT_UInt32 *pData);
void PrintASN1T_Int8(const char *paramName, const asn1SccT_Int8 *pData);
void PrintASN1T_UInt8(const char *paramName, const asn1SccT_UInt8 *pData);
void PrintASN1T_Boolean(const char *paramName, const asn1SccT_Boolean *pData);
void PrintASN1T_Null_Record(const char *paramName, const asn1SccT_Null_Record *pData);
void PrintASN1PID_Range(const char *paramName, const asn1SccPID_Range *pData);
void PrintASN1PID(const char *paramName, const asn1SccPID *pData);

#ifdef __cplusplus
}
#endif

#endif
