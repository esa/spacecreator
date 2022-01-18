/**
 * This file was generated automatically by TestGenerator
 */

#include "out1.h"

#define TEST_DATA_SIZE 5

typedef struct {
    MyInteger posX;
    MyInteger posY;
    MyInteger temp;
    MyInteger result;
} TestVector;

TestVector testData[TEST_DATA_SIZE] = { 0 };

void testdriver_startup(void)
{
    testData[0].posX = 0;
    testData[0].posY = 0;
    testData[0].temp = 0;
    testData[0].result = 0;

    testData[1].posX = 0;
    testData[1].posY = 0;
    testData[1].temp = 0;
    testData[1].result = 0;

    testData[2].posX = 0;
    testData[2].posY = 0;
    testData[2].temp = 0;
    testData[2].result = 0;

    testData[3].posX = 0;
    testData[3].posY = 0;
    testData[3].temp = 0;
    testData[3].result = 0;

    testData[4].posX = 0;
    testData[4].posY = 0;
    testData[4].temp = 0;
    testData[4].result = 0;
}

void testdriver_PI_StartTest(void)
{
    for (unsigned int i = 0; i < TEST_DATA_SIZE; i++) {
        testdriver_RI_InterfaceUnderTest(
                &(testData[i].posX, &(testData[i].posY, &(testData[i].temp), &(testData[i].result))));
    }
    notifyTestFinished();
}

void notifyTestFinished(void)
{
    volatile int a;
    while (true) {
        a = 0;
    }
}
