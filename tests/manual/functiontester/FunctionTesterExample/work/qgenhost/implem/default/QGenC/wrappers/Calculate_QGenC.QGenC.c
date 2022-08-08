#ifdef __unix__
#include <stdio.h>
#include <string.h>

#include <assert.h>
#endif

#ifndef STATIC
#define STATIC
#endif

#include "Calculate_QGenC.QGenC.h"
#include "dataview-uniq.h" // Space certified compiler generated
#include "qgen_entry_calculate.h"

static qgen_entry_Calculate_comp_Input cInput;

static qgen_entry_Calculate_comp_Output cOutput;

int Convert_From_native_To_MyInteger_In_Calculate_QGenC_intparam(void *pBuffer)
{
    STATIC asn1SccMyInteger var_MyInteger;
    var_MyInteger = *(asn1SccMyInteger *) pBuffer;
    {
        cInput.intparam = var_MyInteger;
        return 0;
    }
}

int Convert_From_native_To_MyReal_In_Calculate_QGenC_realparam(void *pBuffer)
{
    STATIC asn1SccMyReal var_MyReal;
    var_MyReal = *(asn1SccMyReal *) pBuffer;
    {
        cInput.realparam = var_MyReal;
        return 0;
    }
}

int Convert_From_native_To_MyBool_In_Calculate_QGenC_boolparam(void *pBuffer)
{
    STATIC asn1SccMyBool var_MyBool;
    var_MyBool = *(asn1SccMyBool *) pBuffer;
    {
        cInput.boolparam = var_MyBool;
        return 0;
    }
}

int Convert_From_MyReal_To_native_In_Calculate_QGenC_outparam(void *pBuffer)
{
    STATIC asn1SccMyReal var_MyReal;
    var_MyReal = (double) cOutput.outparam;
    memcpy(pBuffer, &var_MyReal, sizeof(asn1SccMyReal) );
    return sizeof(asn1SccMyReal);
}

void Execute_Calculate_QGenC()
{
    qgen_entry_Calculate_comp(&cInput, &cOutput);
}

void qgenhost_PI_Calculate_startup(void)
{
    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
         qgen_entry_Calculate_init();
    }
}

void qgenhost_PI_Calculate(void *pintparam, void *prealparam, void *pboolparam, void *poutparam)
{
    Convert_From_native_To_MyInteger_In_Calculate_QGenC_intparam(pintparam);
    Convert_From_native_To_MyReal_In_Calculate_QGenC_realparam(prealparam);
    Convert_From_native_To_MyBool_In_Calculate_QGenC_boolparam(pboolparam);
    Execute_Calculate_QGenC();
    Convert_From_MyReal_To_native_In_Calculate_QGenC_outparam(poutparam);
}

