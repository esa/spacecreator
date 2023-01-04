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
asn1SccSint MyInteger__Get(MyInteger* root)
{
    return (*root);
}

/* INTEGER */
void MyInteger__Set(MyInteger* root, asn1SccSint value)
{
    (*root) = value;
}

/* INTEGER */
asn1SccSint MySeq__inputdata_Get(MySeq* root)
{
    return (*root).inputdata;
}

/* INTEGER */
void MySeq__inputdata_Set(MySeq* root, asn1SccSint value)
{
    (*root).inputdata = value;
}

/* INTEGER */
asn1SccSint MySeq__outputdata_Get(MySeq* root)
{
    return (*root).outputdata;
}

/* INTEGER */
void MySeq__outputdata_Set(MySeq* root, asn1SccSint value)
{
    (*root).outputdata = value;
}

/* ENUMERATED */
int MySeq__validity_Get(MySeq* root)
{
    return (*root).validity;
}

/* ENUMERATED */
void MySeq__validity_Set(MySeq* root, int value)
{
    (*root).validity = value;
}

/* CHOICE selector */
int MyChoice__kind_Get(MyChoice* root)
{
    return (*root).kind;
}

/* CHOICE selector */
void MyChoice__kind_Set(MyChoice* root, int value)
{
    (*root).kind = value;
}

/* BOOLEAN */
flag MyChoice__a_Get(MyChoice* root)
{
    return (*root).u.a;
}

/* BOOLEAN */
void MyChoice__a_Set(MyChoice* root, flag value)
{
    (*root).u.a = value;
}

/* Field b selector */
MySeq* MyChoice__b_Get(MyChoice* root)
{
    return &(*root).u.b;
}

/* INTEGER */
asn1SccSint MyChoice__b_inputdata_Get(MyChoice* root)
{
    return (*root).u.b.inputdata;
}

/* INTEGER */
void MyChoice__b_inputdata_Set(MyChoice* root, asn1SccSint value)
{
    (*root).u.b.inputdata = value;
}

/* INTEGER */
asn1SccSint MyChoice__b_outputdata_Get(MyChoice* root)
{
    return (*root).u.b.outputdata;
}

/* INTEGER */
void MyChoice__b_outputdata_Set(MyChoice* root, asn1SccSint value)
{
    (*root).u.b.outputdata = value;
}

/* ENUMERATED */
int MyChoice__b_validity_Get(MyChoice* root)
{
    return (*root).u.b.validity;
}

/* ENUMERATED */
void MyChoice__b_validity_Set(MyChoice* root, int value)
{
    (*root).u.b.validity = value;
}

/* ENUMERATED */
int MyEnum__Get(MyEnum* root)
{
    return (*root);
}

/* ENUMERATED */
void MyEnum__Set(MyEnum* root, int value)
{
    (*root) = value;
}

/* SEQUENCEOF/SETOF */
long MySeqOf__GetLength(MySeqOf* root)
{
    return (*root).nCount;
}

/* SEQUENCEOF/SETOF */
void MySeqOf__SetLength(MySeqOf* root, long value)
{
    (*root).nCount = value;
}

/* ENUMERATED */
int MySeqOf__iDx_Get(MySeqOf* root, int iDx)
{
    return (*root).arr[iDx];
}

/* ENUMERATED */
void MySeqOf__iDx_Set(MySeqOf* root, int iDx, int value)
{
    (*root).arr[iDx] = value;
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

void SetDataFor_MySeq(void *dest, void *src)
{
    memcpy(dest, src, sizeof(MySeq));
}

byte* MovePtrBySizeOf_MySeq(byte *pData)
{
    return pData + sizeof(MySeq);
}

byte* CreateInstanceOf_MySeq() {
    MySeq *p = (MySeq*)malloc(sizeof(MySeq));
    MySeq_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_MySeq(byte *pData) {
    free(pData);
}

void SetDataFor_MyChoice(void *dest, void *src)
{
    memcpy(dest, src, sizeof(MyChoice));
}

byte* MovePtrBySizeOf_MyChoice(byte *pData)
{
    return pData + sizeof(MyChoice);
}

byte* CreateInstanceOf_MyChoice() {
    MyChoice *p = (MyChoice*)malloc(sizeof(MyChoice));
    MyChoice_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_MyChoice(byte *pData) {
    free(pData);
}

void SetDataFor_MyEnum(void *dest, void *src)
{
    memcpy(dest, src, sizeof(MyEnum));
}

byte* MovePtrBySizeOf_MyEnum(byte *pData)
{
    return pData + sizeof(MyEnum);
}

byte* CreateInstanceOf_MyEnum() {
    MyEnum *p = (MyEnum*)malloc(sizeof(MyEnum));
    MyEnum_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_MyEnum(byte *pData) {
    free(pData);
}

void SetDataFor_MySeqOf(void *dest, void *src)
{
    memcpy(dest, src, sizeof(MySeqOf));
}

byte* MovePtrBySizeOf_MySeqOf(byte *pData)
{
    return pData + sizeof(MySeqOf);
}

byte* CreateInstanceOf_MySeqOf() {
    MySeqOf *p = (MySeqOf*)malloc(sizeof(MySeqOf));
    MySeqOf_Initialize(p);
    return (byte*)p;
}

void DestroyInstanceOf_MySeqOf(byte *pData) {
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

