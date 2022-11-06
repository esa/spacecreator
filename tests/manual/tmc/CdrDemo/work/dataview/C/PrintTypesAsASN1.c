#ifdef __unix__
#include <stdio.h>
#endif

#include "PrintTypesAsASN1.h"

#ifdef __linux__
#include <pthread.h>

static pthread_mutex_t g_printing_mutex = PTHREAD_MUTEX_INITIALIZER;

#endif

void PrintASN1Dummy(const char *paramName, const asn1SccDummy *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s Dummy ::= ", paramName);
    printf("%s ", paramName);
    #if WORD_SIZE==8
    printf("%"PRId64, (*pData));
    #else
    printf("%d", (*pData));
    #endif
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1DataItem(const char *paramName, const asn1SccDataItem *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s DataItem ::= ", paramName);
    printf("%s ", paramName);
    #if WORD_SIZE==8
    printf("%"PRId64, (*pData));
    #else
    printf("%d", (*pData));
    #endif
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1ActuatorConfiguration(const char *paramName, const asn1SccActuatorConfiguration *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s ActuatorConfiguration ::= ", paramName);
    printf("%s ", paramName);
    printf("{");
    printf("threshold1 ");
    #if WORD_SIZE==8
    printf("%"PRId64, (*pData).threshold1);
    #else
    printf("%d", (*pData).threshold1);
    #endif
    printf(", ");
    printf("threshold2 ");
    #if WORD_SIZE==8
    printf("%"PRId64, (*pData).threshold2);
    #else
    printf("%d", (*pData).threshold2);
    #endif
    printf("}");
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1SystemCommand(const char *paramName, const asn1SccSystemCommand *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s SystemCommand ::= ", paramName);
    printf("%s ", paramName);
    if ((*pData).kind == SystemCommand_goToIdle_PRESENT) {
        printf("goToIdle:");
        #if WORD_SIZE==8
        printf("%"PRId64, (*pData).u.goToIdle);
        #else
        printf("%d", (*pData).u.goToIdle);
        #endif
    }
    else if ((*pData).kind == SystemCommand_goToOperational_PRESENT) {
        printf("goToOperational:");
        printf("{");
        printf("threshold1 ");
        #if WORD_SIZE==8
        printf("%"PRId64, (*pData).u.goToOperational.threshold1);
        #else
        printf("%d", (*pData).u.goToOperational.threshold1);
        #endif
        printf(", ");
        printf("threshold2 ");
        #if WORD_SIZE==8
        printf("%"PRId64, (*pData).u.goToOperational.threshold2);
        #else
        printf("%d", (*pData).u.goToOperational.threshold2);
        #endif
        printf("}");
    }
    else if ((*pData).kind == SystemCommand_doReport_PRESENT) {
        printf("doReport:");
        #if WORD_SIZE==8
        printf("%"PRId64, (*pData).u.doReport);
        #else
        printf("%d", (*pData).u.doReport);
        #endif
    }
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1ActuatorIndex(const char *paramName, const asn1SccActuatorIndex *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s ActuatorIndex ::= ", paramName);
    printf("%s ", paramName);
    switch((*pData)) {
    case 0:
        printf("actuator1");
        break;
    case 1:
        printf("actuator2");
        break;
    default:
        printf("Invalid value in ENUMERATED ((*pData))");
    }
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1ActuatorCommand(const char *paramName, const asn1SccActuatorCommand *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s ActuatorCommand ::= ", paramName);
    printf("%s ", paramName);
    switch((*pData)) {
    case 0:
        printf("actuatorArm");
        break;
    case 1:
        printf("actuatorExecute");
        break;
    case 2:
        printf("actuatorStandby");
        break;
    default:
        printf("Invalid value in ENUMERATED ((*pData))");
    }
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1ActuatorStatus(const char *paramName, const asn1SccActuatorStatus *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s ActuatorStatus ::= ", paramName);
    printf("%s ", paramName);
    switch((*pData)) {
    case 0:
        printf("actuatorArmed");
        break;
    case 1:
        printf("actuatorExecuting");
        break;
    case 2:
        printf("actuatorStandingBy");
        break;
    default:
        printf("Invalid value in ENUMERATED ((*pData))");
    }
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1SystemTelemetry(const char *paramName, const asn1SccSystemTelemetry *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s SystemTelemetry ::= ", paramName);
    printf("%s ", paramName);
    printf("{");
    printf("reading ");
    #if WORD_SIZE==8
    printf("%"PRId64, (*pData).reading);
    #else
    printf("%d", (*pData).reading);
    #endif
    printf(", ");
    printf("status1 ");
    switch((*pData).status1) {
    case 0:
        printf("actuatorArmed");
        break;
    case 1:
        printf("actuatorExecuting");
        break;
    case 2:
        printf("actuatorStandingBy");
        break;
    default:
        printf("Invalid value in ENUMERATED ((*pData).status1)");
    }
    printf(", ");
    printf("status2 ");
    switch((*pData).status2) {
    case 0:
        printf("actuatorArmed");
        break;
    case 1:
        printf("actuatorExecuting");
        break;
    case 2:
        printf("actuatorStandingBy");
        break;
    default:
        printf("Invalid value in ENUMERATED ((*pData).status2)");
    }
    printf("}");
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1T_Int32(const char *paramName, const asn1SccT_Int32 *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s T-Int32 ::= ", paramName);
    printf("%s ", paramName);
    #if WORD_SIZE==8
    printf("%"PRId64, (*pData));
    #else
    printf("%d", (*pData));
    #endif
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1T_UInt32(const char *paramName, const asn1SccT_UInt32 *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s T-UInt32 ::= ", paramName);
    printf("%s ", paramName);
    #if WORD_SIZE==8
    printf("%"PRId64, (*pData));
    #else
    printf("%d", (*pData));
    #endif
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1T_Int8(const char *paramName, const asn1SccT_Int8 *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s T-Int8 ::= ", paramName);
    printf("%s ", paramName);
    #if WORD_SIZE==8
    printf("%"PRId64, (*pData));
    #else
    printf("%d", (*pData));
    #endif
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1T_UInt8(const char *paramName, const asn1SccT_UInt8 *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s T-UInt8 ::= ", paramName);
    printf("%s ", paramName);
    #if WORD_SIZE==8
    printf("%"PRId64, (*pData));
    #else
    printf("%d", (*pData));
    #endif
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1T_Boolean(const char *paramName, const asn1SccT_Boolean *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s T-Boolean ::= ", paramName);
    printf("%s ", paramName);
    printf("%s", (int)(*pData)?"TRUE":"FALSE");
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1T_Null_Record(const char *paramName, const asn1SccT_Null_Record *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s T-Null-Record ::= ", paramName);
    printf("%s ", paramName);
    printf("{");
    printf("}");
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1PID_Range(const char *paramName, const asn1SccPID_Range *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s PID-Range ::= ", paramName);
    printf("%s ", paramName);
    #if WORD_SIZE==8
    printf("%"PRId64, (*pData));
    #else
    printf("%d", (*pData));
    #endif
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

void PrintASN1PID(const char *paramName, const asn1SccPID *pData)
{
    (void)paramName;
    (void)pData;
#ifdef __linux__
    pthread_mutex_lock(&g_printing_mutex);
#endif
#ifdef __unix__
    //printf("%s PID ::= ", paramName);
    printf("%s ", paramName);
    switch((*pData)) {
    case 0:
        printf("actuator");
        break;
    case 1:
        printf("actuator1");
        break;
    case 2:
        printf("actuator2");
        break;
    case 3:
        printf("egse");
        break;
    case 4:
        printf("modemanager");
        break;
    case 5:
        printf("sensor");
        break;
    case 6:
        printf("env");
        break;
    default:
        printf("Invalid value in ENUMERATED ((*pData))");
    }
#endif
#ifdef __linux__
    pthread_mutex_unlock(&g_printing_mutex);
#endif
}

