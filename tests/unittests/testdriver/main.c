#include "C/src/testdriver.h"

#define TEST_DATA_SIZE 5

typedef struct {
    asn1SccMyInteger posX;
    asn1SccMyInteger posY;
    asn1SccMyInteger temp;
    asn1SccMyInteger result;
} TestVector;

extern TestVector testData[TEST_DATA_SIZE];

void testdriver_RI_InterfaceUnderTest(const asn1SccMyBool *active_in, const asn1SccMyReal *temp_in,
        const asn1SccMyInteger *posX_in, const asn1SccMyInteger *posY_in, asn1SccMyInteger *result_out)
{
    (void)active_in;
    (void)temp_in;
    (void)posX_in;
    (void)posY_in;
    (void)result_out;

    for (int i = 0; i < TEST_DATA_SIZE; i++) {
        testData[i].result++;
    }
}

int main(void)
{
    puts("now running test_startup:");
    testdriver_startup();

    puts("now running testdriver_PI_StartTest()");
    testdriver_PI_StartTest();

    for (int i = 0; i < TEST_DATA_SIZE; i++) {
        printf("result %d after test: %d\n", i, testData[i].result);
    }

    return 0;
}
