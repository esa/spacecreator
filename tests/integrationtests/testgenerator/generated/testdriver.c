/**
 * This file was generated automatically by TestGenerator
 */

#include "testdriver.h"
//#include <stdio.h>

#define TEST_DATA_SIZE 2
int testDataSize = TEST_DATA_SIZE;

typedef struct {
    asn1SccMyInteger p1;
    asn1SccMyInteger p2;
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
    testData[0].p1 = 1;
    testData[0].p2 = 0;

    testData[1].p1 = 10;
    testData[1].p2 = 0;
}

void testdriver_PI_StartTest(void)
{
    for (unsigned int i = 0; i < testDataSize; i++) {
        // clang-format off
        testdriver_RI_CustomIface(
                &(testData[i].p1),
                &(testData[i].p2)
        );
        // clang-format on
    }
    notifyTestFinished();
}

