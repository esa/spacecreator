/**
 * This file was generated automatically by TestGenerator
 */

#include "testdriver.h"
//#include <stdio.h>

#define TEST_DATA_SIZE 5

typedef struct {
    asn1SccMyBool active_in;
    asn1SccMyReal temp_in;
    asn1SccMyInteger posX_in;
    asn1SccMyInteger posY_in;
    asn1SccMyInteger result_out;
} TestVector;

TestVector testData[TEST_DATA_SIZE] = { 0 };

void notifyTestFinished(void)
{
    volatile int a;
    while (true) {
        a = 0;
    }
}

void testdriver_startup(void)
{
    // Write your initialisation code, but DO NOT CALL REQUIRED INTERFACES
    // puts ("[TestDriver] Startup");
}

void testdriver_PI_StartTest(void)
{
    // Write your code here
}
