/**
 * This file was generated automatically by TestGenerator
 */

#include "testdriver.h"
//#include <stdio.h>

#define TEST_DATA_SIZE 6

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
        break;
    }
}

void testdriver_startup(void)
{
    testData[0].active_in = 0;
    testData[0].temp_in = 0;
    testData[0].posX_in = 0;
    testData[0].posY_in = 0;
    testData[0].result_out = 0;

    testData[1].active_in = 0;
    testData[1].temp_in = 0;
    testData[1].posX_in = 0;
    testData[1].posY_in = 0;
    testData[1].result_out = 0;

    testData[2].active_in = 0;
    testData[2].temp_in = 0;
    testData[2].posX_in = 0;
    testData[2].posY_in = 0;
    testData[2].result_out = 0;

    testData[3].active_in = 0;
    testData[3].temp_in = 0;
    testData[3].posX_in = 0;
    testData[3].posY_in = 0;
    testData[3].result_out = 0;

    testData[4].active_in = 0;
    testData[4].temp_in = 0;
    testData[4].posX_in = 0;
    testData[4].posY_in = 0;
    testData[4].result_out = 0;
}

void testdriver_PI_StartTest(void)
{
    for (unsigned int i = 0; i < TEST_DATA_SIZE; i++) {
        // clang-format off
        testdriver_RI_InterfaceUnderTest(
                &(testData[i].active_in),
                &(testData[i].temp_in),
                &(testData[i].posX_in),
                &(testData[i].posY_in),
                &(testData[i].result_out)
        );
        // clang-format on
    }
    notifyTestFinished();
}
