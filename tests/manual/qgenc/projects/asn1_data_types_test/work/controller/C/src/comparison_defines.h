#pragma once

// ---------------------------------- NCountCompare ----------------------------------

#define NCountCompare(first, second) \
    if(first.nCount != second.nCount) {  \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.nCount, second.nCount)); \
    }

// ---------------------------------- StructsCompare ----------------------------------

#define AliasesStructCompare(first, second) \
    if(first.boolAliasMember != second.boolAliasMember) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.boolAliasMember, second.boolAliasMember)); \
    } \
    if(first.integerAliasMember != second.integerAliasMember) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.integerAliasMember, second.integerAliasMember)); \
    } \
    if(fabs(first.realAliasMember - second.realAliasMember) > 0.0001) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.realAliasMember, second.realAliasMember)); \
    } \
    if(strcmp(first.ia5StringFixedSizeAliasMember, second.ia5StringFixedSizeAliasMember) != 0) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.ia5StringFixedSizeAliasMember, second.ia5StringFixedSizeAliasMember)); \
    } \
    if(strcmp(first.ia5StringVarSizeAliasMember, second.ia5StringVarSizeAliasMember) != 0) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.ia5StringVarSizeAliasMember, second.ia5StringVarSizeAliasMember)); \
    } \
    if(memcmp(&first.octetStringFixedSizeAliasMember, &second.octetStringFixedSizeAliasMember, 3) != 0) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.octetStringFixedSizeAliasMember, second.octetStringFixedSizeAliasMember)); \
    } \
    if(memcmp(&first.octetStringVarSizeAliasMember, &second.octetStringVarSizeAliasMember, 3) != 0) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.octetStringVarSizeAliasMember, second.octetStringVarSizeAliasMember)); \
    } \
    if(first.enumAliasMember != second.enumAliasMember) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.enumAliasMember, second.enumAliasMember)); \
    } \

#define BuiltInStructCompare(first, second) \
    if(first.boolMember != second.boolMember) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.boolMember, second.boolMember)); \
    } \
    if(first.integerMember != second.integerMember) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.integerMember, second.integerMember)); \
    } \
    if(fabs(first.realMember - second.realMember) > 0.0001) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.realMember, second.realMember)); \
    } \
    if(strcmp(first.ia5StringFixedSizeMember, second.ia5StringFixedSizeMember) != 0) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.ia5StringFixedSizeMember, second.ia5StringFixedSizeMember)); \
    } \
    if(strcmp(first.ia5StringVarSizeMember, second.ia5StringVarSizeMember) != 0) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.ia5StringVarSizeMember, second.ia5StringVarSizeMember)); \
    } \
    if(memcmp(&first.octetStringFixedSizeMember, &second.octetStringFixedSizeMember, 3) != 0) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.octetStringFixedSizeMember, second.octetStringFixedSizeMember)); \
    } \
    if(memcmp(&first.octetStringVarSizeMember, &second.octetStringVarSizeMember, 3) != 0) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.octetStringVarSizeMember, second.octetStringVarSizeMember)); \
    } \
    if(first.enumMember != second.enumMember) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.enumMember, second.enumMember)); \
    } \

// ---------------------------------- UnionCompare ----------------------------------

#define KindChoiceCompare(first, second) \
    if(first.kind != second.kind) { \
        reportAndExitFailure(GenerateComparisonFailureMsg(first.kind, second.kind)); \
    }

#define AliasesChoiceCompare(first, second) \
    KindChoiceCompare(first, second) \
    switch(first.kind) { \
        case ChoiceAliases_NONE: \
            break; \
        case ChoiceAliases_boolAliasMember_PRESENT: \
            if(first.u.boolAliasMember != second.u.boolAliasMember) { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.boolAliasMember, second.u.boolAliasMember)); \
            } \
            break; \
        case ChoiceAliases_integerAliasMember_PRESENT: \
            if(first.u.integerAliasMember != second.u.integerAliasMember) { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.integerAliasMember, second.u.integerAliasMember)); \
            } \
            break; \
        case ChoiceAliases_realAliasMember_PRESENT: \
            if(fabs(first.u.realAliasMember - second.u.realAliasMember) > 0.0001)  { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.realAliasMember, second.u.realAliasMember)); \
            } \
            break; \
        case ChoiceAliases_ia5StringFixedSizeAliasMember_PRESENT: \
            if(strcmp(first.u.ia5StringFixedSizeAliasMember, second.u.ia5StringFixedSizeAliasMember))  { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.ia5StringFixedSizeAliasMember, second.u.ia5StringFixedSizeAliasMember)); \
            } \
            break; \
        case ChoiceAliases_ia5StringVarSizeAliasMember_PRESENT: \
            if(strcmp(first.u.ia5StringVarSizeAliasMember, second.u.ia5StringVarSizeAliasMember))  { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.ia5StringVarSizeAliasMember, second.u.ia5StringVarSizeAliasMember)); \
            } \
            break; \
        case ChoiceAliases_octetStringFixedSizeAliasMember_PRESENT: \
            if(memcmp(&first.u.octetStringFixedSizeAliasMember, &second.u.octetStringFixedSizeAliasMember, 3))  { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.octetStringFixedSizeAliasMember, second.u.octetStringFixedSizeAliasMember)); \
            } \
            break; \
        case ChoiceAliases_octetStringVarSizeAliasMember_PRESENT: \
            if(memcmp(&first.u.octetStringVarSizeAliasMember,   &second.u.octetStringVarSizeAliasMember, 3))  { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.octetStringVarSizeAliasMember, second.u.octetStringVarSizeAliasMember)); \
            } \
            break; \
        case ChoiceAliases_enumAliasMember_PRESENT: \
            if(first.u.enumAliasMember != second.u.enumAliasMember)  { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.enumAliasMember, second.u.enumAliasMember)); \
            } \
            break; \
        default: reportAndExitFailure("Unhandled enum value in " #first); \
    }

#define BuiltInChoiceCompare(first, second) \
    KindChoiceCompare(first, second) \
    switch(first.kind) { \
        case ChoiceBuiltIn_NONE: \
            break; \
        case ChoiceBuiltIn_boolMember_PRESENT: \
            if(first.u.boolMember != second.u.boolMember) { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.boolMember, second.u.boolMember)); \
            } \
            break; \
        case ChoiceBuiltIn_integerMember_PRESENT: \
            if(first.u.integerMember != second.u.integerMember) { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.integerMember, second.u.integerMember)); \
            } \
            break; \
        case ChoiceBuiltIn_realMember_PRESENT: \
            if(fabs(first.u.realMember - second.u.realMember) > 0.0001) { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.realMember, second.u.realMember)); \
            } \
            break; \
        case ChoiceBuiltIn_ia5StringFixedSizeMember_PRESENT: \
            if(strcmp(first.u.ia5StringFixedSizeMember, second.u.ia5StringFixedSizeMember)) { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.ia5StringFixedSizeMember, second.u.ia5StringFixedSizeMember)); \
            } \
            break; \
        case ChoiceBuiltIn_ia5StringVarSizeMember_PRESENT: \
            if(strcmp(first.u.ia5StringVarSizeMember, second.u.ia5StringVarSizeMember)) { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.ia5StringVarSizeMember, second.u.ia5StringVarSizeMember)); \
            } \
            break; \
        case ChoiceBuiltIn_octetStringFixedSizeMember_PRESENT: \
            if(memcmp(&first.u.octetStringFixedSizeMember, &second.u.octetStringFixedSizeMember, 3)) { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.octetStringFixedSizeMember, second.u.octetStringFixedSizeMember)); \
            } \
            break; \
        case ChoiceBuiltIn_octetStringVarSizeMember_PRESENT: \
            if(memcmp(&first.u.octetStringVarSizeMember, &second.u.octetStringVarSizeMember, 3)) { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.octetStringVarSizeMember, second.u.octetStringVarSizeMember)); \
            } \
            break; \
        case ChoiceBuiltIn_enumMember_PRESENT: \
            if(first.u.enumMember != second.u.enumMember) { \
                reportAndExitFailure(GenerateComparisonFailureMsg(first.u.enumMember, second.u.enumMember)); \
            } \
            break; \
        default: reportAndExitFailure("Unhandled enum kind in " #first); \
    }

// ---------------------------------- OperatorCompare ----------------------------------

#define FixedSizeSeqOfOperatorCompare(num_of_elems, first, second) \
    for(int k = 0; k < num_of_elems; k++) {  \
        if(first.arr[k] != second.arr[k]) {  \
            reportAndExitFailure(GenerateComparisonFailureMsg(first.arr[k], second.arr[k])); \
        } \
    } \

#define FixedSizeSeqOfSeqOfOperatorCompare(num_of_elems, first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        FixedSizeSeqOfOperatorCompare(num_of_elems, first.arr[j], second.arr[j]) \
    } \

#define FixedSizeSeqOfSeqOfSeqOfOperatorCompare(num_of_elems, first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        FixedSizeSeqOfSeqOfOperatorCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }


#define VarSizeSeqOfOperatorCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    FixedSizeSeqOfOperatorCompare(num_of_elems, first, second)

#define VarSizeSeqOfSeqOfOperatorCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        VarSizeSeqOfOperatorCompare(num_of_elems, first.arr[j], second.arr[j]) \
    } \

#define VarSizeSeqOfSeqOfSeqOfOperatorCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        VarSizeSeqOfSeqOfOperatorCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }

// ---------------------------------- DoubleCompare ----------------------------------

#define FixedSizeSeqOfDoubleCompare(num_of_elems, first, second) \
    for(int k = 0; k < num_of_elems; k++) {  \
        if(fabs(first.arr[k] - second.arr[k]) > 0.0001) {  \
            reportAndExitFailure(GenerateComparisonFailureMsg(first.arr[k], second.arr[k])); \
        } \
    }

#define FixedSizeSeqOfSeqOfDoubleCompare(num_of_elems, first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        FixedSizeSeqOfDoubleCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define FixedSizeSeqOfSeqOfSeqOfDoubleCompare(num_of_elems, first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        FixedSizeSeqOfSeqOfDoubleCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }


#define VarSizeSeqOfDoubleCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    FixedSizeSeqOfDoubleCompare(num_of_elems, first, second)

#define VarSizeSeqOfSeqOfDoubleCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        VarSizeSeqOfDoubleCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define VarSizeSeqOfSeqOfSeqOfDoubleCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        VarSizeSeqOfSeqOfDoubleCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }

// ---------------------------------- StrCmpCompare ----------------------------------

#define FixedSizeSeqOfStrCmpCompare(num_of_elems, first, second) \
    for(int k = 0; k < num_of_elems; k++) {  \
        if(strcmp(first.arr[k], second.arr[k]) != 0) {  \
            reportAndExitFailure(GenerateComparisonFailureMsg(first.arr[k], second.arr[k])); \
        } \
    }

#define FixedSizeSeqOfSeqOfStrCmpCompare(num_of_elems, first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        FixedSizeSeqOfStrCmpCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define FixedSizeSeqOfSeqOfSeqOfStrCmpCompare(num_of_elems, first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        FixedSizeSeqOfSeqOfStrCmpCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }


#define VarSizeSeqOfStrCmpCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    FixedSizeSeqOfStrCmpCompare(num_of_elems, first, second)

#define VarSizeSeqOfSeqOfStrCmpCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        VarSizeSeqOfStrCmpCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define VarSizeSeqOfSeqOfSeqOfStrCmpCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        VarSizeSeqOfSeqOfStrCmpCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }

// ---------------------------------- MemCmpCompare ----------------------------------

#define FixedSizeSeqOfMemCmpCompare(num_of_elems, first, second) \
    for(int k = 0; k < num_of_elems; k++) {  \
        if(memcmp(&first.arr[k], &second.arr[k], num_of_elems) != 0) {  \
            reportAndExitFailure(GenerateComparisonFailureMsg(first.arr[k], second.arr[k])); \
        } \
    }

#define FixedSizeSeqOfSeqOfMemCmpCompare(num_of_elems, first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        FixedSizeSeqOfMemCmpCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define FixedSizeSeqOfSeqOfSeqOfMemCmpCompare(num_of_elems, first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        FixedSizeSeqOfSeqOfMemCmpCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }


#define VarSizeSeqOfMemCmpCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    FixedSizeSeqOfMemCmpCompare(num_of_elems, first, second)

#define VarSizeSeqOfSeqOfMemCmpCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        VarSizeSeqOfMemCmpCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define VarSizeSeqOfSeqOfSeqOfMemCmpCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        VarSizeSeqOfSeqOfMemCmpCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }

// ---------------------------------- AliasesStructCompare ----------------------------------

#define FixedSizeSeqOfAliasesStructCompare(num_of_elems, first, second) \
    for(int k = 0; k < num_of_elems; k++) {  \
        AliasesStructCompare(first.arr[k], second.arr[k]) \
    }

#define FixedSizeSeqOfSeqOfAliasesStructCompare(num_of_elems, first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        FixedSizeSeqOfAliasesStructCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define FixedSizeSeqOfSeqOfSeqOfAliasesStructCompare(num_of_elems, first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        FixedSizeSeqOfSeqOfAliasesStructCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }


#define VarSizeSeqOfAliasesStructCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    FixedSizeSeqOfAliasesStructCompare(num_of_elems, first, second)

#define VarSizeSeqOfSeqOfAliasesStructCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        VarSizeSeqOfAliasesStructCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define VarSizeSeqOfSeqOfSeqOfAliasesStructCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        VarSizeSeqOfSeqOfAliasesStructCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }

// ---------------------------------- BuiltInStructCompare ----------------------------------

#define FixedSizeSeqOfBuiltInStructCompare(num_of_elems, first, second) \
    for(int k = 0; k < num_of_elems; k++) {  \
        BuiltInStructCompare(first.arr[k], second.arr[k]) \
    }

#define FixedSizeSeqOfSeqOfBuiltInStructCompare(num_of_elems, first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        FixedSizeSeqOfBuiltInStructCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define FixedSizeSeqOfSeqOfSeqOfBuiltInStructCompare(num_of_elems, first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        FixedSizeSeqOfSeqOfBuiltInStructCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }


#define VarSizeSeqOfBuiltInStructCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    FixedSizeSeqOfBuiltInStructCompare(num_of_elems, first, second)

#define VarSizeSeqOfSeqOfBuiltInStructCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        VarSizeSeqOfBuiltInStructCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define VarSizeSeqOfSeqOfSeqOfBuiltInStructCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        VarSizeSeqOfSeqOfBuiltInStructCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }

// ---------------------------------- AliasesChoiceCompare ----------------------------------

#define FixedSizeSeqOfAliasesChoiceCompare(num_of_elems, first, second) \
    for(int k = 0; k < num_of_elems; k++) {  \
        AliasesChoiceCompare(first.arr[k], second.arr[k]) \
    }

#define FixedSizeSeqOfSeqOfAliasesChoiceCompare(num_of_elems, first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        FixedSizeSeqOfAliasesChoiceCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define FixedSizeSeqOfSeqOfSeqOfAliasesChoiceCompare(num_of_elems, first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        FixedSizeSeqOfSeqOfAliasesChoiceCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }


#define VarSizeSeqOfAliasesChoiceCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    FixedSizeSeqOfAliasesChoiceCompare(num_of_elems, first, second)

#define VarSizeSeqOfSeqOfAliasesChoiceCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        VarSizeSeqOfAliasesChoiceCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define VarSizeSeqOfSeqOfSeqOfAliasesChoiceCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        VarSizeSeqOfSeqOfAliasesChoiceCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }

// ---------------------------------- BuiltInChoiceCompare ----------------------------------

#define FixedSizeSeqOfBuiltInChoiceCompare(num_of_elems, first, second) \
    for(int k = 0; k < num_of_elems; k++) {  \
        BuiltInChoiceCompare(first.arr[k], second.arr[k]) \
    }

#define FixedSizeSeqOfSeqOfBuiltInChoiceCompare(num_of_elems, first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        FixedSizeSeqOfBuiltInChoiceCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define FixedSizeSeqOfSeqOfSeqOfBuiltInChoiceCompare(num_of_elems, first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        FixedSizeSeqOfSeqOfBuiltInChoiceCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }


#define VarSizeSeqOfBuiltInChoiceCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    FixedSizeSeqOfBuiltInChoiceCompare(num_of_elems, first, second)

#define VarSizeSeqOfSeqOfBuiltInChoiceCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int j = 0; j < num_of_elems; j++) { \
        VarSizeSeqOfBuiltInChoiceCompare(num_of_elems, first.arr[j], second.arr[j]) \
    }

#define VarSizeSeqOfSeqOfSeqOfBuiltInChoiceCompare(num_of_elems, first, second) \
    NCountCompare(first, second) \
    for(int i = 0; i < num_of_elems; i++) { \
        VarSizeSeqOfSeqOfBuiltInChoiceCompare(num_of_elems, first.arr[i], second.arr[i]) \
    }
