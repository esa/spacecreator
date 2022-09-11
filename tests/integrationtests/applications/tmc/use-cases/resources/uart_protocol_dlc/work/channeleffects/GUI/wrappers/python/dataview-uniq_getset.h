#ifndef __GETSET_H__
#define __GETSET_H__

#include "dataview-uniq.h"

size_t GetStreamCurrentLength(BitStream *pBitStrm);
byte *GetBitstreamBuffer(BitStream *pBitStrm);
byte GetBufferByte(byte *p, size_t off);
void SetBufferByte(byte *p, size_t off, byte b);
void ResetStream(BitStream *pStrm);
BitStream *CreateStream(size_t bufferSize);
void DestroyStream(BitStream *pBitStrm);


/* INTEGER */
asn1SccSint UINT40__Get(UINT40* root);

/* INTEGER */
void UINT40__Set(UINT40* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT32__Get(UINT32* root);

/* INTEGER */
void UINT32__Set(UINT32* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT24__Get(UINT24* root);

/* INTEGER */
void UINT24__Set(UINT24* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT16__Get(UINT16* root);

/* INTEGER */
void UINT16__Set(UINT16* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT14__Get(UINT14* root);

/* INTEGER */
void UINT14__Set(UINT14* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT12__Get(UINT12* root);

/* INTEGER */
void UINT12__Set(UINT12* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT11__Get(UINT11* root);

/* INTEGER */
void UINT11__Set(UINT11* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT10__Get(UINT10* root);

/* INTEGER */
void UINT10__Set(UINT10* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT8__Get(UINT8* root);

/* INTEGER */
void UINT8__Set(UINT8* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT7__Get(UINT7* root);

/* INTEGER */
void UINT7__Set(UINT7* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT6__Get(UINT6* root);

/* INTEGER */
void UINT6__Set(UINT6* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT5__Get(UINT5* root);

/* INTEGER */
void UINT5__Set(UINT5* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT4__Get(UINT4* root);

/* INTEGER */
void UINT4__Set(UINT4* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT3__Get(UINT3* root);

/* INTEGER */
void UINT3__Set(UINT3* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT2__Get(UINT2* root);

/* INTEGER */
void UINT2__Set(UINT2* root, asn1SccSint value);

/* INTEGER */
asn1SccSint UINT1__Get(UINT1* root);

/* INTEGER */
void UINT1__Set(UINT1* root, asn1SccSint value);

/* INTEGER */
asn1SccSint T_Bit__Get(T_Bit* root);

/* INTEGER */
void T_Bit__Set(T_Bit* root, asn1SccSint value);

/* INTEGER */
asn1SccSint INT16__Get(INT16* root);

/* INTEGER */
void INT16__Set(INT16* root, asn1SccSint value);

/* INTEGER */
asn1SccSint INT32__Get(INT32* root);

/* INTEGER */
void INT32__Set(INT32* root, asn1SccSint value);

/* INTEGER */
asn1SccSint INT24__Get(INT24* root);

/* INTEGER */
void INT24__Set(INT24* root, asn1SccSint value);

/* OCTETSTRING */
long T_OCTET__GetLength(T_OCTET* root);

/* OCTETSTRING */
void T_OCTET__SetLength(T_OCTET* root, long value);

/* OCTETSTRING_bytes */
byte T_OCTET__iDx_Get(T_OCTET* root, int iDx);

/* OCTETSTRING_bytes */
void T_OCTET__iDx_Set(T_OCTET* root, int iDx, byte value);

/* ENUMERATED */
int T_OnOffState__Get(T_OnOffState* root);

/* ENUMERATED */
void T_OnOffState__Set(T_OnOffState* root, int value);

/* ENUMERATED */
int T_NominalRedundantState__Get(T_NominalRedundantState* root);

/* ENUMERATED */
void T_NominalRedundantState__Set(T_NominalRedundantState* root, int value);

/* ENUMERATED */
int T_ConnectionState__Get(T_ConnectionState* root);

/* ENUMERATED */
void T_ConnectionState__Set(T_ConnectionState* root, int value);

/* ENUMERATED */
int T_SwitchState__Get(T_SwitchState* root);

/* ENUMERATED */
void T_SwitchState__Set(T_SwitchState* root, int value);

/* ENUMERATED */
int T_FaultState__Get(T_FaultState* root);

/* ENUMERATED */
void T_FaultState__Set(T_FaultState* root, int value);

/* ENUMERATED */
int T_SelectionState__Get(T_SelectionState* root);

/* ENUMERATED */
void T_SelectionState__Set(T_SelectionState* root, int value);

/* ENUMERATED */
int T_PresenceState__Get(T_PresenceState* root);

/* ENUMERATED */
void T_PresenceState__Set(T_PresenceState* root, int value);

/* ENUMERATED */
int T_ForwardBackwardState__Get(T_ForwardBackwardState* root);

/* ENUMERATED */
void T_ForwardBackwardState__Set(T_ForwardBackwardState* root, int value);

/* ENUMERATED */
int T_LeftRightState__Get(T_LeftRightState* root);

/* ENUMERATED */
void T_LeftRightState__Set(T_LeftRightState* root, int value);

/* ENUMERATED */
int T_UsedUnusedState__Get(T_UsedUnusedState* root);

/* ENUMERATED */
void T_UsedUnusedState__Set(T_UsedUnusedState* root, int value);

/* ENUMERATED */
int T_MovingStillState__Get(T_MovingStillState* root);

/* ENUMERATED */
void T_MovingStillState__Set(T_MovingStillState* root, int value);

/* ENUMERATED */
int T_PlusMinusState__Get(T_PlusMinusState* root);

/* ENUMERATED */
void T_PlusMinusState__Set(T_PlusMinusState* root, int value);

/* ENUMERATED */
int T_ArmedDisarmedState__Get(T_ArmedDisarmedState* root);

/* ENUMERATED */
void T_ArmedDisarmedState__Set(T_ArmedDisarmedState* root, int value);

/* ENUMERATED */
int T_ActiveInactiveState__Get(T_ActiveInactiveState* root);

/* ENUMERATED */
void T_ActiveInactiveState__Set(T_ActiveInactiveState* root, int value);

/* ENUMERATED */
int T_EnabledInhibitedState__Get(T_EnabledInhibitedState* root);

/* ENUMERATED */
void T_EnabledInhibitedState__Set(T_EnabledInhibitedState* root, int value);

/* ENUMERATED */
int T_FlagState__Get(T_FlagState* root);

/* ENUMERATED */
void T_FlagState__Set(T_FlagState* root, int value);

/* ENUMERATED */
int T_SuccessFailState__Get(T_SuccessFailState* root);

/* ENUMERATED */
void T_SuccessFailState__Set(T_SuccessFailState* root, int value);

/* ENUMERATED */
int T_HighLowState__Get(T_HighLowState* root);

/* ENUMERATED */
void T_HighLowState__Set(T_HighLowState* root, int value);

/* ENUMERATED */
int T_OverrideState__Get(T_OverrideState* root);

/* ENUMERATED */
void T_OverrideState__Set(T_OverrideState* root, int value);

/* ENUMERATED */
int T_LockedUnlockedState__Get(T_LockedUnlockedState* root);

/* ENUMERATED */
void T_LockedUnlockedState__Set(T_LockedUnlockedState* root, int value);

/* ENUMERATED */
int T_WarningState__Get(T_WarningState* root);

/* ENUMERATED */
void T_WarningState__Set(T_WarningState* root, int value);

/* ENUMERATED */
int T_ValidInvalidState__Get(T_ValidInvalidState* root);

/* ENUMERATED */
void T_ValidInvalidState__Set(T_ValidInvalidState* root, int value);

/* INTEGER */
asn1SccSint T_UART_DLC_Header__Get(T_UART_DLC_Header* root);

/* INTEGER */
void T_UART_DLC_Header__Set(T_UART_DLC_Header* root, asn1SccSint value);

/* INTEGER */
asn1SccSint T_UART_DLC_FrameCRC__Get(T_UART_DLC_FrameCRC* root);

/* INTEGER */
void T_UART_DLC_FrameCRC__Set(T_UART_DLC_FrameCRC* root, asn1SccSint value);

/* SEQUENCEOF/SETOF */
long T_RMAPCRCTable__GetLength(T_RMAPCRCTable* root);

/* SEQUENCEOF/SETOF */
void T_RMAPCRCTable__SetLength(T_RMAPCRCTable* root, long value);

/* INTEGER */
asn1SccSint T_RMAPCRCTable__iDx_Get(T_RMAPCRCTable* root, int iDx);

/* INTEGER */
void T_RMAPCRCTable__iDx_Set(T_RMAPCRCTable* root, int iDx, asn1SccSint value);

/* OCTETSTRING */
long T_UART_DLC_FrameBuffer__GetLength(T_UART_DLC_FrameBuffer* root);

/* OCTETSTRING */
void T_UART_DLC_FrameBuffer__SetLength(T_UART_DLC_FrameBuffer* root, long value);

/* OCTETSTRING_bytes */
byte T_UART_DLC_FrameBuffer__iDx_Get(T_UART_DLC_FrameBuffer* root, int iDx);

/* OCTETSTRING_bytes */
void T_UART_DLC_FrameBuffer__iDx_Set(T_UART_DLC_FrameBuffer* root, int iDx, byte value);

/* OCTETSTRING */
long T_UART_DLC_SDU__GetLength(T_UART_DLC_SDU* root);

/* OCTETSTRING */
void T_UART_DLC_SDU__SetLength(T_UART_DLC_SDU* root, long value);

/* OCTETSTRING_bytes */
byte T_UART_DLC_SDU__iDx_Get(T_UART_DLC_SDU* root, int iDx);

/* OCTETSTRING_bytes */
void T_UART_DLC_SDU__iDx_Set(T_UART_DLC_SDU* root, int iDx, byte value);

/* INTEGER */
asn1SccSint T_UART_DLC_PDU__dlc_header_Get(T_UART_DLC_PDU* root);

/* INTEGER */
void T_UART_DLC_PDU__dlc_header_Set(T_UART_DLC_PDU* root, asn1SccSint value);

/* OCTETSTRING */
long T_UART_DLC_PDU__dlc_payload_GetLength(T_UART_DLC_PDU* root);

/* OCTETSTRING */
void T_UART_DLC_PDU__dlc_payload_SetLength(T_UART_DLC_PDU* root, long value);

/* OCTETSTRING_bytes */
byte T_UART_DLC_PDU__dlc_payload_iDx_Get(T_UART_DLC_PDU* root, int iDx);

/* OCTETSTRING_bytes */
void T_UART_DLC_PDU__dlc_payload_iDx_Set(T_UART_DLC_PDU* root, int iDx, byte value);

/* INTEGER */
asn1SccSint T_UART_DLC_PDU__crc_Get(T_UART_DLC_PDU* root);

/* INTEGER */
void T_UART_DLC_PDU__crc_Set(T_UART_DLC_PDU* root, asn1SccSint value);

/* ENUMERATED */
int T_UART_DLC_RxErrorCode__Get(T_UART_DLC_RxErrorCode* root);

/* ENUMERATED */
void T_UART_DLC_RxErrorCode__Set(T_UART_DLC_RxErrorCode* root, int value);

/* INTEGER */
asn1SccSint T_Int32__Get(T_Int32* root);

/* INTEGER */
void T_Int32__Set(T_Int32* root, asn1SccSint value);

/* INTEGER */
asn1SccSint T_UInt32__Get(T_UInt32* root);

/* INTEGER */
void T_UInt32__Set(T_UInt32* root, asn1SccSint value);

/* INTEGER */
asn1SccSint T_Int8__Get(T_Int8* root);

/* INTEGER */
void T_Int8__Set(T_Int8* root, asn1SccSint value);

/* INTEGER */
asn1SccSint T_UInt8__Get(T_UInt8* root);

/* INTEGER */
void T_UInt8__Set(T_UInt8* root, asn1SccSint value);

/* BOOLEAN */
flag T_Boolean__Get(T_Boolean* root);

/* BOOLEAN */
void T_Boolean__Set(T_Boolean* root, flag value);

/* INTEGER */
asn1SccSint MyInteger__Get(MyInteger* root);

/* INTEGER */
void MyInteger__Set(MyInteger* root, asn1SccSint value);

/* ENUMERATED */
int PID__Get(PID* root);

/* ENUMERATED */
void PID__Set(PID* root, int value);

/* Helper functions for NATIVE encodings */

void SetDataFor_UINT40(void *dest, void *src);
byte* MovePtrBySizeOf_UINT40(byte *pData);
byte* CreateInstanceOf_UINT40(void);
void DestroyInstanceOf_UINT40(byte *pData);

void SetDataFor_UINT32(void *dest, void *src);
byte* MovePtrBySizeOf_UINT32(byte *pData);
byte* CreateInstanceOf_UINT32(void);
void DestroyInstanceOf_UINT32(byte *pData);

void SetDataFor_UINT24(void *dest, void *src);
byte* MovePtrBySizeOf_UINT24(byte *pData);
byte* CreateInstanceOf_UINT24(void);
void DestroyInstanceOf_UINT24(byte *pData);

void SetDataFor_UINT16(void *dest, void *src);
byte* MovePtrBySizeOf_UINT16(byte *pData);
byte* CreateInstanceOf_UINT16(void);
void DestroyInstanceOf_UINT16(byte *pData);

void SetDataFor_UINT14(void *dest, void *src);
byte* MovePtrBySizeOf_UINT14(byte *pData);
byte* CreateInstanceOf_UINT14(void);
void DestroyInstanceOf_UINT14(byte *pData);

void SetDataFor_UINT12(void *dest, void *src);
byte* MovePtrBySizeOf_UINT12(byte *pData);
byte* CreateInstanceOf_UINT12(void);
void DestroyInstanceOf_UINT12(byte *pData);

void SetDataFor_UINT11(void *dest, void *src);
byte* MovePtrBySizeOf_UINT11(byte *pData);
byte* CreateInstanceOf_UINT11(void);
void DestroyInstanceOf_UINT11(byte *pData);

void SetDataFor_UINT10(void *dest, void *src);
byte* MovePtrBySizeOf_UINT10(byte *pData);
byte* CreateInstanceOf_UINT10(void);
void DestroyInstanceOf_UINT10(byte *pData);

void SetDataFor_UINT8(void *dest, void *src);
byte* MovePtrBySizeOf_UINT8(byte *pData);
byte* CreateInstanceOf_UINT8(void);
void DestroyInstanceOf_UINT8(byte *pData);

void SetDataFor_UINT7(void *dest, void *src);
byte* MovePtrBySizeOf_UINT7(byte *pData);
byte* CreateInstanceOf_UINT7(void);
void DestroyInstanceOf_UINT7(byte *pData);

void SetDataFor_UINT6(void *dest, void *src);
byte* MovePtrBySizeOf_UINT6(byte *pData);
byte* CreateInstanceOf_UINT6(void);
void DestroyInstanceOf_UINT6(byte *pData);

void SetDataFor_UINT5(void *dest, void *src);
byte* MovePtrBySizeOf_UINT5(byte *pData);
byte* CreateInstanceOf_UINT5(void);
void DestroyInstanceOf_UINT5(byte *pData);

void SetDataFor_UINT4(void *dest, void *src);
byte* MovePtrBySizeOf_UINT4(byte *pData);
byte* CreateInstanceOf_UINT4(void);
void DestroyInstanceOf_UINT4(byte *pData);

void SetDataFor_UINT3(void *dest, void *src);
byte* MovePtrBySizeOf_UINT3(byte *pData);
byte* CreateInstanceOf_UINT3(void);
void DestroyInstanceOf_UINT3(byte *pData);

void SetDataFor_UINT2(void *dest, void *src);
byte* MovePtrBySizeOf_UINT2(byte *pData);
byte* CreateInstanceOf_UINT2(void);
void DestroyInstanceOf_UINT2(byte *pData);

void SetDataFor_UINT1(void *dest, void *src);
byte* MovePtrBySizeOf_UINT1(byte *pData);
byte* CreateInstanceOf_UINT1(void);
void DestroyInstanceOf_UINT1(byte *pData);

void SetDataFor_T_Bit(void *dest, void *src);
byte* MovePtrBySizeOf_T_Bit(byte *pData);
byte* CreateInstanceOf_T_Bit(void);
void DestroyInstanceOf_T_Bit(byte *pData);

void SetDataFor_INT16(void *dest, void *src);
byte* MovePtrBySizeOf_INT16(byte *pData);
byte* CreateInstanceOf_INT16(void);
void DestroyInstanceOf_INT16(byte *pData);

void SetDataFor_INT32(void *dest, void *src);
byte* MovePtrBySizeOf_INT32(byte *pData);
byte* CreateInstanceOf_INT32(void);
void DestroyInstanceOf_INT32(byte *pData);

void SetDataFor_INT24(void *dest, void *src);
byte* MovePtrBySizeOf_INT24(byte *pData);
byte* CreateInstanceOf_INT24(void);
void DestroyInstanceOf_INT24(byte *pData);

void SetDataFor_T_OCTET(void *dest, void *src);
byte* MovePtrBySizeOf_T_OCTET(byte *pData);
byte* CreateInstanceOf_T_OCTET(void);
void DestroyInstanceOf_T_OCTET(byte *pData);

void SetDataFor_T_OnOffState(void *dest, void *src);
byte* MovePtrBySizeOf_T_OnOffState(byte *pData);
byte* CreateInstanceOf_T_OnOffState(void);
void DestroyInstanceOf_T_OnOffState(byte *pData);

void SetDataFor_T_NominalRedundantState(void *dest, void *src);
byte* MovePtrBySizeOf_T_NominalRedundantState(byte *pData);
byte* CreateInstanceOf_T_NominalRedundantState(void);
void DestroyInstanceOf_T_NominalRedundantState(byte *pData);

void SetDataFor_T_ConnectionState(void *dest, void *src);
byte* MovePtrBySizeOf_T_ConnectionState(byte *pData);
byte* CreateInstanceOf_T_ConnectionState(void);
void DestroyInstanceOf_T_ConnectionState(byte *pData);

void SetDataFor_T_SwitchState(void *dest, void *src);
byte* MovePtrBySizeOf_T_SwitchState(byte *pData);
byte* CreateInstanceOf_T_SwitchState(void);
void DestroyInstanceOf_T_SwitchState(byte *pData);

void SetDataFor_T_FaultState(void *dest, void *src);
byte* MovePtrBySizeOf_T_FaultState(byte *pData);
byte* CreateInstanceOf_T_FaultState(void);
void DestroyInstanceOf_T_FaultState(byte *pData);

void SetDataFor_T_SelectionState(void *dest, void *src);
byte* MovePtrBySizeOf_T_SelectionState(byte *pData);
byte* CreateInstanceOf_T_SelectionState(void);
void DestroyInstanceOf_T_SelectionState(byte *pData);

void SetDataFor_T_PresenceState(void *dest, void *src);
byte* MovePtrBySizeOf_T_PresenceState(byte *pData);
byte* CreateInstanceOf_T_PresenceState(void);
void DestroyInstanceOf_T_PresenceState(byte *pData);

void SetDataFor_T_ForwardBackwardState(void *dest, void *src);
byte* MovePtrBySizeOf_T_ForwardBackwardState(byte *pData);
byte* CreateInstanceOf_T_ForwardBackwardState(void);
void DestroyInstanceOf_T_ForwardBackwardState(byte *pData);

void SetDataFor_T_LeftRightState(void *dest, void *src);
byte* MovePtrBySizeOf_T_LeftRightState(byte *pData);
byte* CreateInstanceOf_T_LeftRightState(void);
void DestroyInstanceOf_T_LeftRightState(byte *pData);

void SetDataFor_T_UsedUnusedState(void *dest, void *src);
byte* MovePtrBySizeOf_T_UsedUnusedState(byte *pData);
byte* CreateInstanceOf_T_UsedUnusedState(void);
void DestroyInstanceOf_T_UsedUnusedState(byte *pData);

void SetDataFor_T_MovingStillState(void *dest, void *src);
byte* MovePtrBySizeOf_T_MovingStillState(byte *pData);
byte* CreateInstanceOf_T_MovingStillState(void);
void DestroyInstanceOf_T_MovingStillState(byte *pData);

void SetDataFor_T_PlusMinusState(void *dest, void *src);
byte* MovePtrBySizeOf_T_PlusMinusState(byte *pData);
byte* CreateInstanceOf_T_PlusMinusState(void);
void DestroyInstanceOf_T_PlusMinusState(byte *pData);

void SetDataFor_T_ArmedDisarmedState(void *dest, void *src);
byte* MovePtrBySizeOf_T_ArmedDisarmedState(byte *pData);
byte* CreateInstanceOf_T_ArmedDisarmedState(void);
void DestroyInstanceOf_T_ArmedDisarmedState(byte *pData);

void SetDataFor_T_ActiveInactiveState(void *dest, void *src);
byte* MovePtrBySizeOf_T_ActiveInactiveState(byte *pData);
byte* CreateInstanceOf_T_ActiveInactiveState(void);
void DestroyInstanceOf_T_ActiveInactiveState(byte *pData);

void SetDataFor_T_EnabledInhibitedState(void *dest, void *src);
byte* MovePtrBySizeOf_T_EnabledInhibitedState(byte *pData);
byte* CreateInstanceOf_T_EnabledInhibitedState(void);
void DestroyInstanceOf_T_EnabledInhibitedState(byte *pData);

void SetDataFor_T_FlagState(void *dest, void *src);
byte* MovePtrBySizeOf_T_FlagState(byte *pData);
byte* CreateInstanceOf_T_FlagState(void);
void DestroyInstanceOf_T_FlagState(byte *pData);

void SetDataFor_T_SuccessFailState(void *dest, void *src);
byte* MovePtrBySizeOf_T_SuccessFailState(byte *pData);
byte* CreateInstanceOf_T_SuccessFailState(void);
void DestroyInstanceOf_T_SuccessFailState(byte *pData);

void SetDataFor_T_HighLowState(void *dest, void *src);
byte* MovePtrBySizeOf_T_HighLowState(byte *pData);
byte* CreateInstanceOf_T_HighLowState(void);
void DestroyInstanceOf_T_HighLowState(byte *pData);

void SetDataFor_T_OverrideState(void *dest, void *src);
byte* MovePtrBySizeOf_T_OverrideState(byte *pData);
byte* CreateInstanceOf_T_OverrideState(void);
void DestroyInstanceOf_T_OverrideState(byte *pData);

void SetDataFor_T_LockedUnlockedState(void *dest, void *src);
byte* MovePtrBySizeOf_T_LockedUnlockedState(byte *pData);
byte* CreateInstanceOf_T_LockedUnlockedState(void);
void DestroyInstanceOf_T_LockedUnlockedState(byte *pData);

void SetDataFor_T_WarningState(void *dest, void *src);
byte* MovePtrBySizeOf_T_WarningState(byte *pData);
byte* CreateInstanceOf_T_WarningState(void);
void DestroyInstanceOf_T_WarningState(byte *pData);

void SetDataFor_T_ValidInvalidState(void *dest, void *src);
byte* MovePtrBySizeOf_T_ValidInvalidState(byte *pData);
byte* CreateInstanceOf_T_ValidInvalidState(void);
void DestroyInstanceOf_T_ValidInvalidState(byte *pData);

void SetDataFor_T_UART_DLC_Header(void *dest, void *src);
byte* MovePtrBySizeOf_T_UART_DLC_Header(byte *pData);
byte* CreateInstanceOf_T_UART_DLC_Header(void);
void DestroyInstanceOf_T_UART_DLC_Header(byte *pData);

void SetDataFor_T_UART_DLC_FrameCRC(void *dest, void *src);
byte* MovePtrBySizeOf_T_UART_DLC_FrameCRC(byte *pData);
byte* CreateInstanceOf_T_UART_DLC_FrameCRC(void);
void DestroyInstanceOf_T_UART_DLC_FrameCRC(byte *pData);

void SetDataFor_T_RMAPCRCTable(void *dest, void *src);
byte* MovePtrBySizeOf_T_RMAPCRCTable(byte *pData);
byte* CreateInstanceOf_T_RMAPCRCTable(void);
void DestroyInstanceOf_T_RMAPCRCTable(byte *pData);

void SetDataFor_T_UART_DLC_FrameBuffer(void *dest, void *src);
byte* MovePtrBySizeOf_T_UART_DLC_FrameBuffer(byte *pData);
byte* CreateInstanceOf_T_UART_DLC_FrameBuffer(void);
void DestroyInstanceOf_T_UART_DLC_FrameBuffer(byte *pData);

void SetDataFor_T_UART_DLC_SDU(void *dest, void *src);
byte* MovePtrBySizeOf_T_UART_DLC_SDU(byte *pData);
byte* CreateInstanceOf_T_UART_DLC_SDU(void);
void DestroyInstanceOf_T_UART_DLC_SDU(byte *pData);

void SetDataFor_T_UART_DLC_PDU(void *dest, void *src);
byte* MovePtrBySizeOf_T_UART_DLC_PDU(byte *pData);
byte* CreateInstanceOf_T_UART_DLC_PDU(void);
void DestroyInstanceOf_T_UART_DLC_PDU(byte *pData);

void SetDataFor_T_UART_DLC_RxErrorCode(void *dest, void *src);
byte* MovePtrBySizeOf_T_UART_DLC_RxErrorCode(byte *pData);
byte* CreateInstanceOf_T_UART_DLC_RxErrorCode(void);
void DestroyInstanceOf_T_UART_DLC_RxErrorCode(byte *pData);

void SetDataFor_T_Int32(void *dest, void *src);
byte* MovePtrBySizeOf_T_Int32(byte *pData);
byte* CreateInstanceOf_T_Int32(void);
void DestroyInstanceOf_T_Int32(byte *pData);

void SetDataFor_T_UInt32(void *dest, void *src);
byte* MovePtrBySizeOf_T_UInt32(byte *pData);
byte* CreateInstanceOf_T_UInt32(void);
void DestroyInstanceOf_T_UInt32(byte *pData);

void SetDataFor_T_Int8(void *dest, void *src);
byte* MovePtrBySizeOf_T_Int8(byte *pData);
byte* CreateInstanceOf_T_Int8(void);
void DestroyInstanceOf_T_Int8(byte *pData);

void SetDataFor_T_UInt8(void *dest, void *src);
byte* MovePtrBySizeOf_T_UInt8(byte *pData);
byte* CreateInstanceOf_T_UInt8(void);
void DestroyInstanceOf_T_UInt8(byte *pData);

void SetDataFor_T_Boolean(void *dest, void *src);
byte* MovePtrBySizeOf_T_Boolean(byte *pData);
byte* CreateInstanceOf_T_Boolean(void);
void DestroyInstanceOf_T_Boolean(byte *pData);

void SetDataFor_T_Null_Record(void *dest, void *src);
byte* MovePtrBySizeOf_T_Null_Record(byte *pData);
byte* CreateInstanceOf_T_Null_Record(void);
void DestroyInstanceOf_T_Null_Record(byte *pData);

void SetDataFor_MyInteger(void *dest, void *src);
byte* MovePtrBySizeOf_MyInteger(byte *pData);
byte* CreateInstanceOf_MyInteger(void);
void DestroyInstanceOf_MyInteger(byte *pData);

void SetDataFor_PID(void *dest, void *src);
byte* MovePtrBySizeOf_PID(byte *pData);
byte* CreateInstanceOf_PID(void);
void DestroyInstanceOf_PID(byte *pData);

void SetDataFor_int(void *dest, void *src);
byte* MovePtrBySizeOf_int(byte *pData);
byte* CreateInstanceOf_int(void);
void DestroyInstanceOf_int(byte *pData);


#endif
