#ifndef __QGENC_DATAVIEW_UNIQASN_CALCULATE_QGENC_H__
#define __QGENC_DATAVIEW_UNIQASN_CALCULATE_QGENC_H__

#include <stdlib.h> /* for size_t */

int Convert_From_native_To_MyInteger_In_Calculate_QGenC_intparam(void *pBuffer);
int Convert_From_native_To_MyReal_In_Calculate_QGenC_realparam(void *pBuffer);
int Convert_From_native_To_MyBool_In_Calculate_QGenC_boolparam(void *pBuffer);
int Convert_From_MyReal_To_native_In_Calculate_QGenC_outparam(void *pBuffer);
void Execute_Calculate_QGenC(void);
void qgenhost_PI_Calculate_startup(void);
void qgenhost_PI_Calculate(void *pintparam, void *prealparam, void *pboolparam, void *poutparam);

#endif
