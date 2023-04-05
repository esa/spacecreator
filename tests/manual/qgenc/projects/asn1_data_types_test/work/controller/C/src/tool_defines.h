#pragma once

// ---------------------------------- GenerateComparisonFailureMsg ----------------------------------

#define GenerateComparisonFailureMsg(first, second) \
    TEST_CASE_NAME "\n\t" #first " != " #second

// ---------------------------------- PrintTestPassedSuccessfully ----------------------------------

#define PrintTestPassedSuccessfully() \
    printf(TEST_CASE_NAME "\n\tPassed Successfully\n\n");
