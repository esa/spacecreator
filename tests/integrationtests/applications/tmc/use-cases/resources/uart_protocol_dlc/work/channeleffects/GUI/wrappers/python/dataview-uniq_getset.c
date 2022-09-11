#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "dataview-uniq_getset.h"

size_t GetStreamCurrentLength(BitStream *pBitStrm) {
    return pBitStrm->currentByte + ((pBitStrm->currentBit+7)/8);
}

byte *GetBitstreamBuffer(BitStream *pBitStrm) {
    return pBitStrm->buf;
}

byte GetBufferByte(byte *p, size_t off) {
    assert(p);
    return p[off];
}

void SetBufferByte(byte *p, size_t off, byte b) {
    assert(p);
    p[off] = b;
}

void ResetStream(BitStream *pStrm) {
    assert(pStrm);
    assert(pStrm->count >= 0);
    pStrm->currentByte = 0;
    pStrm->currentBit = 0;
}

BitStream *CreateStream(size_t bufferSize) {
    BitStream *pBitStrm = malloc(sizeof(BitStream));
    assert(pBitStrm);
    unsigned char* buf = malloc(bufferSize);
    assert(buf);
    memset(buf, 0x0, bufferSize);
    BitStream_Init(pBitStrm, buf, bufferSize);
    return pBitStrm;
}

void DestroyStream(BitStream *pBitStrm) {
    assert(pBitStrm);
    assert(pBitStrm->buf);
    free(pBitStrm->buf);
    free(pBitStrm);
}


/* INTEGER */
asn1SccSint UINT40__Get(UINT40* root)
{
    return (*root);
}

/* INTEGER */
void UINT40__Set(UINT40* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT32__Get(UINT32* root)
{
    return (*root);
}

/* INTEGER */
void UINT32__Set(UINT32* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT24__Get(UINT24* root)
{
    return (*root);
}

/* INTEGER */
void UINT24__Set(UINT24* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT16__Get(UINT16* root)
{
    return (*root);
}

/* INTEGER */
void UINT16__Set(UINT16* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT14__Get(UINT14* root)
{
    return (*root);
}

/* INTEGER */
void UINT14__Set(UINT14* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT12__Get(UINT12* root)
{
    return (*root);
}

/* INTEGER */
void UINT12__Set(UINT12* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT11__Get(UINT11* root)
{
    return (*root);
}

/* INTEGER */
void UINT11__Set(UINT11* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT10__Get(UINT10* root)
{
    return (*root);
}

/* INTEGER */
void UINT10__Set(UINT10* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT8__Get(UINT8* root)
{
    return (*root);
}

/* INTEGER */
void UINT8__Set(UINT8* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT7__Get(UINT7* root)
{
    return (*root);
}

/* INTEGER */
void UINT7__Set(UINT7* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT6__Get(UINT6* root)
{
    return (*root);
}

/* INTEGER */
void UINT6__Set(UINT6* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT5__Get(UINT5* root)
{
    return (*root);
}

/* INTEGER */
void UINT5__Set(UINT5* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT4__Get(UINT4* root)
{
    return (*root);
}

/* INTEGER */
void UINT4__Set(UINT4* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT3__Get(UINT3* root)
{
    return (*root);
}

/* INTEGER */
void UINT3__Set(UINT3* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT2__Get(UINT2* root)
{
    return (*root);
}

/* INTEGER */
void UINT2__Set(UINT2* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint UINT1__Get(UINT1* root)
{
    return (*root);
}

/* INTEGER */
void UINT1__Set(UINT1* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint T_Bit__Get(T_Bit* root)
{
    return (*root);
}

/* INTEGER */
void T_Bit__Set(T_Bit* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint INT16__Get(INT16* root)
{
    return (*root);
}

/* INTEGER */
void INT16__Set(INT16* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint INT32__Get(INT32* root)
{
    return (*root);
}

/* INTEGER */
void INT32__Set(INT32* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint INT24__Get(INT24* root)
{
    return (*root);
}

/* INTEGER */
void INT24__Set(INT24* root, asn1SccSint value)
{
    (*root) = value;
}

/* OCTETSTRING */
long T_OCTET__GetLength(T_OCTET* root)
{
    return 1;
}

/* OCTETSTRING */
void T_OCTET__SetLength(T_OCTET* root, long value)
{
    fprintf(stderr, "WARNING: setting length of fixed-length sequence\n");
}

/* OCTETSTRING_bytes */
byte T_OCTET__iDx_Get(T_OCTET* root, int iDx)
{
    return (*root).arr[iDx];
}

/* OCTETSTRING_bytes */
void T_OCTET__iDx_Set(T_OCTET* root, int iDx, byte value)
{
    (*root).arr[iDx] = value;
}

/* ENUMERATED */
int T_OnOffState__Get(T_OnOffState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_OnOffState__Set(T_OnOffState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_NominalRedundantState__Get(T_NominalRedundantState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_NominalRedundantState__Set(T_NominalRedundantState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_ConnectionState__Get(T_ConnectionState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_ConnectionState__Set(T_ConnectionState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_SwitchState__Get(T_SwitchState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_SwitchState__Set(T_SwitchState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_FaultState__Get(T_FaultState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_FaultState__Set(T_FaultState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_SelectionState__Get(T_SelectionState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_SelectionState__Set(T_SelectionState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_PresenceState__Get(T_PresenceState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_PresenceState__Set(T_PresenceState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_ForwardBackwardState__Get(T_ForwardBackwardState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_ForwardBackwardState__Set(T_ForwardBackwardState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_LeftRightState__Get(T_LeftRightState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_LeftRightState__Set(T_LeftRightState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_UsedUnusedState__Get(T_UsedUnusedState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_UsedUnusedState__Set(T_UsedUnusedState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_MovingStillState__Get(T_MovingStillState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_MovingStillState__Set(T_MovingStillState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_PlusMinusState__Get(T_PlusMinusState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_PlusMinusState__Set(T_PlusMinusState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_ArmedDisarmedState__Get(T_ArmedDisarmedState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_ArmedDisarmedState__Set(T_ArmedDisarmedState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_ActiveInactiveState__Get(T_ActiveInactiveState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_ActiveInactiveState__Set(T_ActiveInactiveState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_EnabledInhibitedState__Get(T_EnabledInhibitedState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_EnabledInhibitedState__Set(T_EnabledInhibitedState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_FlagState__Get(T_FlagState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_FlagState__Set(T_FlagState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_SuccessFailState__Get(T_SuccessFailState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_SuccessFailState__Set(T_SuccessFailState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_HighLowState__Get(T_HighLowState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_HighLowState__Set(T_HighLowState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_OverrideState__Get(T_OverrideState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_OverrideState__Set(T_OverrideState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_LockedUnlockedState__Get(T_LockedUnlockedState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_LockedUnlockedState__Set(T_LockedUnlockedState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_WarningState__Get(T_WarningState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_WarningState__Set(T_WarningState* root, int value)
{
    (*root) = value;
}

/* ENUMERATED */
int T_ValidInvalidState__Get(T_ValidInvalidState* root)
{
    return (*root);
}

/* ENUMERATED */
void T_ValidInvalidState__Set(T_ValidInvalidState* root, int value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint T_UART_DLC_Header__Get(T_UART_DLC_Header* root)
{
    return (*root);
}

/* INTEGER */
void T_UART_DLC_Header__Set(T_UART_DLC_Header* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint T_UART_DLC_FrameCRC__Get(T_UART_DLC_FrameCRC* root)
{
    return (*root);
}

/* INTEGER */
void T_UART_DLC_FrameCRC__Set(T_UART_DLC_FrameCRC* root, asn1SccSint value)
{
    (*root) = value;
}

/* SEQUENCEOF/SETOF */
long T_RMAPCRCTable__GetLength(T_RMAPCRCTable* root)
{
    return 256;
}

/* SEQUENCEOF/SETOF */
void T_RMAPCRCTable__SetLength(T_RMAPCRCTable* root, long value)
{
    fprintf(stderr, "WARNING: setting length of fixed-length sequence\n");
}

/* INTEGER */
asn1SccSint T_RMAPCRCTable__iDx_Get(T_RMAPCRCTable* root, int iDx)
{
    return (*root).arr[iDx];
}

/* INTEGER */
void T_RMAPCRCTable__iDx_Set(T_RMAPCRCTable* root, int iDx, asn1SccSint value)
{
    (*root).arr[iDx] = value;
}

/* OCTETSTRING */
long T_UART_DLC_FrameBuffer__GetLength(T_UART_DLC_FrameBuffer* root)
{
    return 16;
}

/* OCTETSTRING */
void T_UART_DLC_FrameBuffer__SetLength(T_UART_DLC_FrameBuffer* root, long value)
{
    fprintf(stderr, "WARNING: setting length of fixed-length sequence\n");
}

/* OCTETSTRING_bytes */
byte T_UART_DLC_FrameBuffer__iDx_Get(T_UART_DLC_FrameBuffer* root, int iDx)
{
    return (*root).arr[iDx];
}

/* OCTETSTRING_bytes */
void T_UART_DLC_FrameBuffer__iDx_Set(T_UART_DLC_FrameBuffer* root, int iDx, byte value)
{
    (*root).arr[iDx] = value;
}

/* OCTETSTRING */
long T_UART_DLC_SDU__GetLength(T_UART_DLC_SDU* root)
{
    return 14;
}

/* OCTETSTRING */
void T_UART_DLC_SDU__SetLength(T_UART_DLC_SDU* root, long value)
{
    fprintf(stderr, "WARNING: setting length of fixed-length sequence\n");
}

/* OCTETSTRING_bytes */
byte T_UART_DLC_SDU__iDx_Get(T_UART_DLC_SDU* root, int iDx)
{
    return (*root).arr[iDx];
}

/* OCTETSTRING_bytes */
void T_UART_DLC_SDU__iDx_Set(T_UART_DLC_SDU* root, int iDx, byte value)
{
    (*root).arr[iDx] = value;
}

/* INTEGER */
asn1SccSint T_UART_DLC_PDU__dlc_header_Get(T_UART_DLC_PDU* root)
{
    return (*root).dlc_header;
}

/* INTEGER */
void T_UART_DLC_PDU__dlc_header_Set(T_UART_DLC_PDU* root, asn1SccSint value)
{
    (*root).dlc_header = value;
}

/* OCTETSTRING */
long T_UART_DLC_PDU__dlc_payload_GetLength(T_UART_DLC_PDU* root)
{
    return 14;
}

/* OCTETSTRING */
void T_UART_DLC_PDU__dlc_payload_SetLength(T_UART_DLC_PDU* root, long value)
{
    fprintf(stderr, "WARNING: setting length of fixed-length sequence\n");
}

/* OCTETSTRING_bytes */
byte T_UART_DLC_PDU__dlc_payload_iDx_Get(T_UART_DLC_PDU* root, int iDx)
{
    return (*root).dlc_payload.arr[iDx];
}

/* OCTETSTRING_bytes */
void T_UART_DLC_PDU__dlc_payload_iDx_Set(T_UART_DLC_PDU* root, int iDx, byte value)
{
    (*root).dlc_payload.arr[iDx] = value;
}

/* INTEGER */
asn1SccSint T_UART_DLC_PDU__crc_Get(T_UART_DLC_PDU* root)
{
    return (*root).crc;
}

/* INTEGER */
void T_UART_DLC_PDU__crc_Set(T_UART_DLC_PDU* root, asn1SccSint value)
{
    (*root).crc = value;
}

/* ENUMERATED */
int T_UART_DLC_RxErrorCode__Get(T_UART_DLC_RxErrorCode* root)
{
    return (*root);
}

/* ENUMERATED */
void T_UART_DLC_RxErrorCode__Set(T_UART_DLC_RxErrorCode* root, int value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint T_Int32__Get(T_Int32* root)
{
    return (*root);
}

/* INTEGER */
void T_Int32__Set(T_Int32* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint T_UInt32__Get(T_UInt32* root)
{
    return (*root);
}

/* INTEGER */
void T_UInt32__Set(T_UInt32* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint T_Int8__Get(T_Int8* root)
{
    return (*root);
}

/* INTEGER */
void T_Int8__Set(T_Int8* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint T_UInt8__Get(T_UInt8* root)
{
    return (*root);
}

/* INTEGER */
void T_UInt8__Set(T_UInt8* root, asn1SccSint value)
{
    (*root) = value;
}

/* BOOLEAN */
flag T_Boolean__Get(T_Boolean* root)
{
    return (*root);
}

/* BOOLEAN */
void T_Boolean__Set(T_Boolean* root, flag value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint MyInteger__Get(MyInteger* root)
{
    return (*root);
}

/* INTEGER */
void MyInteger__Set(MyInteger* root, asn1SccSint value)
{
    (*root) = value;
}

/* ENUMERATED */
int PID__Get(PID* root)
{
    return (*root);
}

/* ENUMERATED */
void PID__Set(PID* root, int value)
{
    (*root) = value;
}

/* Helper functions for NATIVE encodings */

void SetDataFor_UINT40(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT40));
}

byte* MovePtrBySizeOf_UINT40(byte *pData)
{
    return pData + sizeof(UINT40);
}

byte* CreateInstanceOf_UINT40() {
    UINT40 *p = (UINT40*)malloc(sizeof(UINT40));
    UINT40_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT40(byte *pData) {
    free(pData);
}

void SetDataFor_UINT32(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT32));
}

byte* MovePtrBySizeOf_UINT32(byte *pData)
{
    return pData + sizeof(UINT32);
}

byte* CreateInstanceOf_UINT32() {
    UINT32 *p = (UINT32*)malloc(sizeof(UINT32));
    UINT32_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT32(byte *pData) {
    free(pData);
}

void SetDataFor_UINT24(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT24));
}

byte* MovePtrBySizeOf_UINT24(byte *pData)
{
    return pData + sizeof(UINT24);
}

byte* CreateInstanceOf_UINT24() {
    UINT24 *p = (UINT24*)malloc(sizeof(UINT24));
    UINT24_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT24(byte *pData) {
    free(pData);
}

void SetDataFor_UINT16(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT16));
}

byte* MovePtrBySizeOf_UINT16(byte *pData)
{
    return pData + sizeof(UINT16);
}

byte* CreateInstanceOf_UINT16() {
    UINT16 *p = (UINT16*)malloc(sizeof(UINT16));
    UINT16_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT16(byte *pData) {
    free(pData);
}

void SetDataFor_UINT14(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT14));
}

byte* MovePtrBySizeOf_UINT14(byte *pData)
{
    return pData + sizeof(UINT14);
}

byte* CreateInstanceOf_UINT14() {
    UINT14 *p = (UINT14*)malloc(sizeof(UINT14));
    UINT14_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT14(byte *pData) {
    free(pData);
}

void SetDataFor_UINT12(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT12));
}

byte* MovePtrBySizeOf_UINT12(byte *pData)
{
    return pData + sizeof(UINT12);
}

byte* CreateInstanceOf_UINT12() {
    UINT12 *p = (UINT12*)malloc(sizeof(UINT12));
    UINT12_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT12(byte *pData) {
    free(pData);
}

void SetDataFor_UINT11(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT11));
}

byte* MovePtrBySizeOf_UINT11(byte *pData)
{
    return pData + sizeof(UINT11);
}

byte* CreateInstanceOf_UINT11() {
    UINT11 *p = (UINT11*)malloc(sizeof(UINT11));
    UINT11_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT11(byte *pData) {
    free(pData);
}

void SetDataFor_UINT10(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT10));
}

byte* MovePtrBySizeOf_UINT10(byte *pData)
{
    return pData + sizeof(UINT10);
}

byte* CreateInstanceOf_UINT10() {
    UINT10 *p = (UINT10*)malloc(sizeof(UINT10));
    UINT10_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT10(byte *pData) {
    free(pData);
}

void SetDataFor_UINT8(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT8));
}

byte* MovePtrBySizeOf_UINT8(byte *pData)
{
    return pData + sizeof(UINT8);
}

byte* CreateInstanceOf_UINT8() {
    UINT8 *p = (UINT8*)malloc(sizeof(UINT8));
    UINT8_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT8(byte *pData) {
    free(pData);
}

void SetDataFor_UINT7(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT7));
}

byte* MovePtrBySizeOf_UINT7(byte *pData)
{
    return pData + sizeof(UINT7);
}

byte* CreateInstanceOf_UINT7() {
    UINT7 *p = (UINT7*)malloc(sizeof(UINT7));
    UINT7_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT7(byte *pData) {
    free(pData);
}

void SetDataFor_UINT6(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT6));
}

byte* MovePtrBySizeOf_UINT6(byte *pData)
{
    return pData + sizeof(UINT6);
}

byte* CreateInstanceOf_UINT6() {
    UINT6 *p = (UINT6*)malloc(sizeof(UINT6));
    UINT6_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT6(byte *pData) {
    free(pData);
}

void SetDataFor_UINT5(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT5));
}

byte* MovePtrBySizeOf_UINT5(byte *pData)
{
    return pData + sizeof(UINT5);
}

byte* CreateInstanceOf_UINT5() {
    UINT5 *p = (UINT5*)malloc(sizeof(UINT5));
    UINT5_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT5(byte *pData) {
    free(pData);
}

void SetDataFor_UINT4(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT4));
}

byte* MovePtrBySizeOf_UINT4(byte *pData)
{
    return pData + sizeof(UINT4);
}

byte* CreateInstanceOf_UINT4() {
    UINT4 *p = (UINT4*)malloc(sizeof(UINT4));
    UINT4_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT4(byte *pData) {
    free(pData);
}

void SetDataFor_UINT3(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT3));
}

byte* MovePtrBySizeOf_UINT3(byte *pData)
{
    return pData + sizeof(UINT3);
}

byte* CreateInstanceOf_UINT3() {
    UINT3 *p = (UINT3*)malloc(sizeof(UINT3));
    UINT3_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT3(byte *pData) {
    free(pData);
}

void SetDataFor_UINT2(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT2));
}

byte* MovePtrBySizeOf_UINT2(byte *pData)
{
    return pData + sizeof(UINT2);
}

byte* CreateInstanceOf_UINT2() {
    UINT2 *p = (UINT2*)malloc(sizeof(UINT2));
    UINT2_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT2(byte *pData) {
    free(pData);
}

void SetDataFor_UINT1(void *dest, void *src)
{
    memcpy(dest, src, sizeof(UINT1));
}

byte* MovePtrBySizeOf_UINT1(byte *pData)
{
    return pData + sizeof(UINT1);
}

byte* CreateInstanceOf_UINT1() {
    UINT1 *p = (UINT1*)malloc(sizeof(UINT1));
    UINT1_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_UINT1(byte *pData) {
    free(pData);
}

void SetDataFor_T_Bit(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_Bit));
}

byte* MovePtrBySizeOf_T_Bit(byte *pData)
{
    return pData + sizeof(T_Bit);
}

byte* CreateInstanceOf_T_Bit() {
    T_Bit *p = (T_Bit*)malloc(sizeof(T_Bit));
    T_Bit_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_Bit(byte *pData) {
    free(pData);
}

void SetDataFor_INT16(void *dest, void *src)
{
    memcpy(dest, src, sizeof(INT16));
}

byte* MovePtrBySizeOf_INT16(byte *pData)
{
    return pData + sizeof(INT16);
}

byte* CreateInstanceOf_INT16() {
    INT16 *p = (INT16*)malloc(sizeof(INT16));
    INT16_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_INT16(byte *pData) {
    free(pData);
}

void SetDataFor_INT32(void *dest, void *src)
{
    memcpy(dest, src, sizeof(INT32));
}

byte* MovePtrBySizeOf_INT32(byte *pData)
{
    return pData + sizeof(INT32);
}

byte* CreateInstanceOf_INT32() {
    INT32 *p = (INT32*)malloc(sizeof(INT32));
    INT32_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_INT32(byte *pData) {
    free(pData);
}

void SetDataFor_INT24(void *dest, void *src)
{
    memcpy(dest, src, sizeof(INT24));
}

byte* MovePtrBySizeOf_INT24(byte *pData)
{
    return pData + sizeof(INT24);
}

byte* CreateInstanceOf_INT24() {
    INT24 *p = (INT24*)malloc(sizeof(INT24));
    INT24_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_INT24(byte *pData) {
    free(pData);
}

void SetDataFor_T_OCTET(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_OCTET));
}

byte* MovePtrBySizeOf_T_OCTET(byte *pData)
{
    return pData + sizeof(T_OCTET);
}

byte* CreateInstanceOf_T_OCTET() {
    T_OCTET *p = (T_OCTET*)malloc(sizeof(T_OCTET));
    T_OCTET_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_OCTET(byte *pData) {
    free(pData);
}

void SetDataFor_T_OnOffState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_OnOffState));
}

byte* MovePtrBySizeOf_T_OnOffState(byte *pData)
{
    return pData + sizeof(T_OnOffState);
}

byte* CreateInstanceOf_T_OnOffState() {
    T_OnOffState *p = (T_OnOffState*)malloc(sizeof(T_OnOffState));
    T_OnOffState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_OnOffState(byte *pData) {
    free(pData);
}

void SetDataFor_T_NominalRedundantState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_NominalRedundantState));
}

byte* MovePtrBySizeOf_T_NominalRedundantState(byte *pData)
{
    return pData + sizeof(T_NominalRedundantState);
}

byte* CreateInstanceOf_T_NominalRedundantState() {
    T_NominalRedundantState *p = (T_NominalRedundantState*)malloc(sizeof(T_NominalRedundantState));
    T_NominalRedundantState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_NominalRedundantState(byte *pData) {
    free(pData);
}

void SetDataFor_T_ConnectionState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_ConnectionState));
}

byte* MovePtrBySizeOf_T_ConnectionState(byte *pData)
{
    return pData + sizeof(T_ConnectionState);
}

byte* CreateInstanceOf_T_ConnectionState() {
    T_ConnectionState *p = (T_ConnectionState*)malloc(sizeof(T_ConnectionState));
    T_ConnectionState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_ConnectionState(byte *pData) {
    free(pData);
}

void SetDataFor_T_SwitchState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_SwitchState));
}

byte* MovePtrBySizeOf_T_SwitchState(byte *pData)
{
    return pData + sizeof(T_SwitchState);
}

byte* CreateInstanceOf_T_SwitchState() {
    T_SwitchState *p = (T_SwitchState*)malloc(sizeof(T_SwitchState));
    T_SwitchState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_SwitchState(byte *pData) {
    free(pData);
}

void SetDataFor_T_FaultState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_FaultState));
}

byte* MovePtrBySizeOf_T_FaultState(byte *pData)
{
    return pData + sizeof(T_FaultState);
}

byte* CreateInstanceOf_T_FaultState() {
    T_FaultState *p = (T_FaultState*)malloc(sizeof(T_FaultState));
    T_FaultState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_FaultState(byte *pData) {
    free(pData);
}

void SetDataFor_T_SelectionState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_SelectionState));
}

byte* MovePtrBySizeOf_T_SelectionState(byte *pData)
{
    return pData + sizeof(T_SelectionState);
}

byte* CreateInstanceOf_T_SelectionState() {
    T_SelectionState *p = (T_SelectionState*)malloc(sizeof(T_SelectionState));
    T_SelectionState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_SelectionState(byte *pData) {
    free(pData);
}

void SetDataFor_T_PresenceState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_PresenceState));
}

byte* MovePtrBySizeOf_T_PresenceState(byte *pData)
{
    return pData + sizeof(T_PresenceState);
}

byte* CreateInstanceOf_T_PresenceState() {
    T_PresenceState *p = (T_PresenceState*)malloc(sizeof(T_PresenceState));
    T_PresenceState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_PresenceState(byte *pData) {
    free(pData);
}

void SetDataFor_T_ForwardBackwardState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_ForwardBackwardState));
}

byte* MovePtrBySizeOf_T_ForwardBackwardState(byte *pData)
{
    return pData + sizeof(T_ForwardBackwardState);
}

byte* CreateInstanceOf_T_ForwardBackwardState() {
    T_ForwardBackwardState *p = (T_ForwardBackwardState*)malloc(sizeof(T_ForwardBackwardState));
    T_ForwardBackwardState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_ForwardBackwardState(byte *pData) {
    free(pData);
}

void SetDataFor_T_LeftRightState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_LeftRightState));
}

byte* MovePtrBySizeOf_T_LeftRightState(byte *pData)
{
    return pData + sizeof(T_LeftRightState);
}

byte* CreateInstanceOf_T_LeftRightState() {
    T_LeftRightState *p = (T_LeftRightState*)malloc(sizeof(T_LeftRightState));
    T_LeftRightState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_LeftRightState(byte *pData) {
    free(pData);
}

void SetDataFor_T_UsedUnusedState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_UsedUnusedState));
}

byte* MovePtrBySizeOf_T_UsedUnusedState(byte *pData)
{
    return pData + sizeof(T_UsedUnusedState);
}

byte* CreateInstanceOf_T_UsedUnusedState() {
    T_UsedUnusedState *p = (T_UsedUnusedState*)malloc(sizeof(T_UsedUnusedState));
    T_UsedUnusedState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_UsedUnusedState(byte *pData) {
    free(pData);
}

void SetDataFor_T_MovingStillState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_MovingStillState));
}

byte* MovePtrBySizeOf_T_MovingStillState(byte *pData)
{
    return pData + sizeof(T_MovingStillState);
}

byte* CreateInstanceOf_T_MovingStillState() {
    T_MovingStillState *p = (T_MovingStillState*)malloc(sizeof(T_MovingStillState));
    T_MovingStillState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_MovingStillState(byte *pData) {
    free(pData);
}

void SetDataFor_T_PlusMinusState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_PlusMinusState));
}

byte* MovePtrBySizeOf_T_PlusMinusState(byte *pData)
{
    return pData + sizeof(T_PlusMinusState);
}

byte* CreateInstanceOf_T_PlusMinusState() {
    T_PlusMinusState *p = (T_PlusMinusState*)malloc(sizeof(T_PlusMinusState));
    T_PlusMinusState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_PlusMinusState(byte *pData) {
    free(pData);
}

void SetDataFor_T_ArmedDisarmedState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_ArmedDisarmedState));
}

byte* MovePtrBySizeOf_T_ArmedDisarmedState(byte *pData)
{
    return pData + sizeof(T_ArmedDisarmedState);
}

byte* CreateInstanceOf_T_ArmedDisarmedState() {
    T_ArmedDisarmedState *p = (T_ArmedDisarmedState*)malloc(sizeof(T_ArmedDisarmedState));
    T_ArmedDisarmedState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_ArmedDisarmedState(byte *pData) {
    free(pData);
}

void SetDataFor_T_ActiveInactiveState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_ActiveInactiveState));
}

byte* MovePtrBySizeOf_T_ActiveInactiveState(byte *pData)
{
    return pData + sizeof(T_ActiveInactiveState);
}

byte* CreateInstanceOf_T_ActiveInactiveState() {
    T_ActiveInactiveState *p = (T_ActiveInactiveState*)malloc(sizeof(T_ActiveInactiveState));
    T_ActiveInactiveState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_ActiveInactiveState(byte *pData) {
    free(pData);
}

void SetDataFor_T_EnabledInhibitedState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_EnabledInhibitedState));
}

byte* MovePtrBySizeOf_T_EnabledInhibitedState(byte *pData)
{
    return pData + sizeof(T_EnabledInhibitedState);
}

byte* CreateInstanceOf_T_EnabledInhibitedState() {
    T_EnabledInhibitedState *p = (T_EnabledInhibitedState*)malloc(sizeof(T_EnabledInhibitedState));
    T_EnabledInhibitedState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_EnabledInhibitedState(byte *pData) {
    free(pData);
}

void SetDataFor_T_FlagState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_FlagState));
}

byte* MovePtrBySizeOf_T_FlagState(byte *pData)
{
    return pData + sizeof(T_FlagState);
}

byte* CreateInstanceOf_T_FlagState() {
    T_FlagState *p = (T_FlagState*)malloc(sizeof(T_FlagState));
    T_FlagState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_FlagState(byte *pData) {
    free(pData);
}

void SetDataFor_T_SuccessFailState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_SuccessFailState));
}

byte* MovePtrBySizeOf_T_SuccessFailState(byte *pData)
{
    return pData + sizeof(T_SuccessFailState);
}

byte* CreateInstanceOf_T_SuccessFailState() {
    T_SuccessFailState *p = (T_SuccessFailState*)malloc(sizeof(T_SuccessFailState));
    T_SuccessFailState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_SuccessFailState(byte *pData) {
    free(pData);
}

void SetDataFor_T_HighLowState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_HighLowState));
}

byte* MovePtrBySizeOf_T_HighLowState(byte *pData)
{
    return pData + sizeof(T_HighLowState);
}

byte* CreateInstanceOf_T_HighLowState() {
    T_HighLowState *p = (T_HighLowState*)malloc(sizeof(T_HighLowState));
    T_HighLowState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_HighLowState(byte *pData) {
    free(pData);
}

void SetDataFor_T_OverrideState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_OverrideState));
}

byte* MovePtrBySizeOf_T_OverrideState(byte *pData)
{
    return pData + sizeof(T_OverrideState);
}

byte* CreateInstanceOf_T_OverrideState() {
    T_OverrideState *p = (T_OverrideState*)malloc(sizeof(T_OverrideState));
    T_OverrideState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_OverrideState(byte *pData) {
    free(pData);
}

void SetDataFor_T_LockedUnlockedState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_LockedUnlockedState));
}

byte* MovePtrBySizeOf_T_LockedUnlockedState(byte *pData)
{
    return pData + sizeof(T_LockedUnlockedState);
}

byte* CreateInstanceOf_T_LockedUnlockedState() {
    T_LockedUnlockedState *p = (T_LockedUnlockedState*)malloc(sizeof(T_LockedUnlockedState));
    T_LockedUnlockedState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_LockedUnlockedState(byte *pData) {
    free(pData);
}

void SetDataFor_T_WarningState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_WarningState));
}

byte* MovePtrBySizeOf_T_WarningState(byte *pData)
{
    return pData + sizeof(T_WarningState);
}

byte* CreateInstanceOf_T_WarningState() {
    T_WarningState *p = (T_WarningState*)malloc(sizeof(T_WarningState));
    T_WarningState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_WarningState(byte *pData) {
    free(pData);
}

void SetDataFor_T_ValidInvalidState(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_ValidInvalidState));
}

byte* MovePtrBySizeOf_T_ValidInvalidState(byte *pData)
{
    return pData + sizeof(T_ValidInvalidState);
}

byte* CreateInstanceOf_T_ValidInvalidState() {
    T_ValidInvalidState *p = (T_ValidInvalidState*)malloc(sizeof(T_ValidInvalidState));
    T_ValidInvalidState_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_ValidInvalidState(byte *pData) {
    free(pData);
}

void SetDataFor_T_UART_DLC_Header(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_UART_DLC_Header));
}

byte* MovePtrBySizeOf_T_UART_DLC_Header(byte *pData)
{
    return pData + sizeof(T_UART_DLC_Header);
}

byte* CreateInstanceOf_T_UART_DLC_Header() {
    T_UART_DLC_Header *p = (T_UART_DLC_Header*)malloc(sizeof(T_UART_DLC_Header));
    T_UART_DLC_Header_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_UART_DLC_Header(byte *pData) {
    free(pData);
}

void SetDataFor_T_UART_DLC_FrameCRC(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_UART_DLC_FrameCRC));
}

byte* MovePtrBySizeOf_T_UART_DLC_FrameCRC(byte *pData)
{
    return pData + sizeof(T_UART_DLC_FrameCRC);
}

byte* CreateInstanceOf_T_UART_DLC_FrameCRC() {
    T_UART_DLC_FrameCRC *p = (T_UART_DLC_FrameCRC*)malloc(sizeof(T_UART_DLC_FrameCRC));
    T_UART_DLC_FrameCRC_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_UART_DLC_FrameCRC(byte *pData) {
    free(pData);
}

void SetDataFor_T_RMAPCRCTable(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_RMAPCRCTable));
}

byte* MovePtrBySizeOf_T_RMAPCRCTable(byte *pData)
{
    return pData + sizeof(T_RMAPCRCTable);
}

byte* CreateInstanceOf_T_RMAPCRCTable() {
    T_RMAPCRCTable *p = (T_RMAPCRCTable*)malloc(sizeof(T_RMAPCRCTable));
    T_RMAPCRCTable_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_RMAPCRCTable(byte *pData) {
    free(pData);
}

void SetDataFor_T_UART_DLC_FrameBuffer(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_UART_DLC_FrameBuffer));
}

byte* MovePtrBySizeOf_T_UART_DLC_FrameBuffer(byte *pData)
{
    return pData + sizeof(T_UART_DLC_FrameBuffer);
}

byte* CreateInstanceOf_T_UART_DLC_FrameBuffer() {
    T_UART_DLC_FrameBuffer *p = (T_UART_DLC_FrameBuffer*)malloc(sizeof(T_UART_DLC_FrameBuffer));
    T_UART_DLC_FrameBuffer_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_UART_DLC_FrameBuffer(byte *pData) {
    free(pData);
}

void SetDataFor_T_UART_DLC_SDU(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_UART_DLC_SDU));
}

byte* MovePtrBySizeOf_T_UART_DLC_SDU(byte *pData)
{
    return pData + sizeof(T_UART_DLC_SDU);
}

byte* CreateInstanceOf_T_UART_DLC_SDU() {
    T_UART_DLC_SDU *p = (T_UART_DLC_SDU*)malloc(sizeof(T_UART_DLC_SDU));
    T_UART_DLC_SDU_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_UART_DLC_SDU(byte *pData) {
    free(pData);
}

void SetDataFor_T_UART_DLC_PDU(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_UART_DLC_PDU));
}

byte* MovePtrBySizeOf_T_UART_DLC_PDU(byte *pData)
{
    return pData + sizeof(T_UART_DLC_PDU);
}

byte* CreateInstanceOf_T_UART_DLC_PDU() {
    T_UART_DLC_PDU *p = (T_UART_DLC_PDU*)malloc(sizeof(T_UART_DLC_PDU));
    T_UART_DLC_PDU_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_UART_DLC_PDU(byte *pData) {
    free(pData);
}

void SetDataFor_T_UART_DLC_RxErrorCode(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_UART_DLC_RxErrorCode));
}

byte* MovePtrBySizeOf_T_UART_DLC_RxErrorCode(byte *pData)
{
    return pData + sizeof(T_UART_DLC_RxErrorCode);
}

byte* CreateInstanceOf_T_UART_DLC_RxErrorCode() {
    T_UART_DLC_RxErrorCode *p = (T_UART_DLC_RxErrorCode*)malloc(sizeof(T_UART_DLC_RxErrorCode));
    T_UART_DLC_RxErrorCode_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_UART_DLC_RxErrorCode(byte *pData) {
    free(pData);
}

void SetDataFor_T_Int32(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_Int32));
}

byte* MovePtrBySizeOf_T_Int32(byte *pData)
{
    return pData + sizeof(T_Int32);
}

byte* CreateInstanceOf_T_Int32() {
    T_Int32 *p = (T_Int32*)malloc(sizeof(T_Int32));
    T_Int32_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_Int32(byte *pData) {
    free(pData);
}

void SetDataFor_T_UInt32(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_UInt32));
}

byte* MovePtrBySizeOf_T_UInt32(byte *pData)
{
    return pData + sizeof(T_UInt32);
}

byte* CreateInstanceOf_T_UInt32() {
    T_UInt32 *p = (T_UInt32*)malloc(sizeof(T_UInt32));
    T_UInt32_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_UInt32(byte *pData) {
    free(pData);
}

void SetDataFor_T_Int8(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_Int8));
}

byte* MovePtrBySizeOf_T_Int8(byte *pData)
{
    return pData + sizeof(T_Int8);
}

byte* CreateInstanceOf_T_Int8() {
    T_Int8 *p = (T_Int8*)malloc(sizeof(T_Int8));
    T_Int8_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_Int8(byte *pData) {
    free(pData);
}

void SetDataFor_T_UInt8(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_UInt8));
}

byte* MovePtrBySizeOf_T_UInt8(byte *pData)
{
    return pData + sizeof(T_UInt8);
}

byte* CreateInstanceOf_T_UInt8() {
    T_UInt8 *p = (T_UInt8*)malloc(sizeof(T_UInt8));
    T_UInt8_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_UInt8(byte *pData) {
    free(pData);
}

void SetDataFor_T_Boolean(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_Boolean));
}

byte* MovePtrBySizeOf_T_Boolean(byte *pData)
{
    return pData + sizeof(T_Boolean);
}

byte* CreateInstanceOf_T_Boolean() {
    T_Boolean *p = (T_Boolean*)malloc(sizeof(T_Boolean));
    T_Boolean_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_Boolean(byte *pData) {
    free(pData);
}

void SetDataFor_T_Null_Record(void *dest, void *src)
{
    memcpy(dest, src, sizeof(T_Null_Record));
}

byte* MovePtrBySizeOf_T_Null_Record(byte *pData)
{
    return pData + sizeof(T_Null_Record);
}

byte* CreateInstanceOf_T_Null_Record() {
    T_Null_Record *p = (T_Null_Record*)malloc(sizeof(T_Null_Record));
    T_Null_Record_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_T_Null_Record(byte *pData) {
    free(pData);
}

void SetDataFor_MyInteger(void *dest, void *src)
{
    memcpy(dest, src, sizeof(MyInteger));
}

byte* MovePtrBySizeOf_MyInteger(byte *pData)
{
    return pData + sizeof(MyInteger);
}

byte* CreateInstanceOf_MyInteger() {
    MyInteger *p = (MyInteger*)malloc(sizeof(MyInteger));
    MyInteger_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_MyInteger(byte *pData) {
    free(pData);
}

void SetDataFor_PID(void *dest, void *src)
{
    memcpy(dest, src, sizeof(PID));
}

byte* MovePtrBySizeOf_PID(byte *pData)
{
    return pData + sizeof(PID);
}

byte* CreateInstanceOf_PID() {
    PID *p = (PID*)malloc(sizeof(PID));
    PID_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_PID(byte *pData) {
    free(pData);
}

void SetDataFor_int(void *dest, void *src)
{
    memcpy(dest, src, sizeof(int));
}

byte* MovePtrBySizeOf_int(byte *pData)
{
    return pData + sizeof(int);
}

byte* CreateInstanceOf_int() {
    int *p = (int*)malloc(sizeof(int));
    *p = 0;
    return (byte*)p;
}

void DestroyInstanceOf_int(byte *pData) {
    free(pData);
}

