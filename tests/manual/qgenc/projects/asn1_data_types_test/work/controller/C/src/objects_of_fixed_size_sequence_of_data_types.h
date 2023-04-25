#pragma once

#include "dataview-uniq.h"

#include "objects_of_variable_size_sequence_of_data_types.h"

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

#define FIXED_SIZE_DATA_LENGTH 3

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfFixedSizeBoolAlias asn1SccSeqOfFixedSizeBoolAliasObj =
{
    {
        true, false, true
    }
};

const asn1SccSeqOfSeqOfFixedSizeBoolAlias asn1SccSeqOfSeqOfFixedSizeBoolAliasObj =
{
    {
        asn1SccSeqOfFixedSizeBoolAliasObj, asn1SccSeqOfFixedSizeBoolAliasObj, asn1SccSeqOfFixedSizeBoolAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfFixedSizeBoolAlias asn1SccSeqOfSeqOfSeqOfFixedSizeBoolAliasObj =
{
    {
        asn1SccSeqOfSeqOfFixedSizeBoolAliasObj, asn1SccSeqOfSeqOfFixedSizeBoolAliasObj, asn1SccSeqOfSeqOfFixedSizeBoolAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfFixedSizeIntegerAlias asn1SccSeqOfFixedSizeIntegerAliasObj =
{
    {
        5, 10, 15
    }
};

const asn1SccSeqOfSeqOfFixedSizeIntegerAlias asn1SccSeqOfSeqOfFixedSizeIntegerAliasObj =
{
    {
        asn1SccSeqOfFixedSizeIntegerAliasObj, asn1SccSeqOfFixedSizeIntegerAliasObj, asn1SccSeqOfFixedSizeIntegerAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfFixedSizeIntegerAlias asn1SccSeqOfSeqOfSeqOfFixedSizeIntegerAliasObj =
{
    {
        asn1SccSeqOfSeqOfFixedSizeIntegerAliasObj, asn1SccSeqOfSeqOfFixedSizeIntegerAliasObj, asn1SccSeqOfSeqOfFixedSizeIntegerAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfFixedSizeRealAlias asn1SccSeqOfFixedSizeRealAliasObj =
{
    {
        0.2, 0.4, 0.6
    }
};

const asn1SccSeqOfSeqOfFixedSizeRealAlias asn1SccSeqOfSeqOfFixedSizeRealAliasObj =
{
    {
        asn1SccSeqOfFixedSizeRealAliasObj, asn1SccSeqOfFixedSizeRealAliasObj, asn1SccSeqOfFixedSizeRealAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfFixedSizeRealAlias asn1SccSeqOfSeqOfSeqOfFixedSizeRealAliasObj =
{
    {
        asn1SccSeqOfSeqOfFixedSizeRealAliasObj, asn1SccSeqOfSeqOfFixedSizeRealAliasObj, asn1SccSeqOfSeqOfFixedSizeRealAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfFixedSizeIA5StringFixedSizeAlias asn1SccSeqOfFixedSizeIA5StringFixedSizeAliasObj =
{
    {
        "ABC", "DEF", "GHI"
    }
};

const asn1SccSeqOfSeqOfFixedSizeIA5StringFixedSizeAlias asn1SccSeqOfSeqOfFixedSizeIA5StringFixedSizeAliasObj =
{
    {
        asn1SccSeqOfFixedSizeIA5StringFixedSizeAliasObj, asn1SccSeqOfFixedSizeIA5StringFixedSizeAliasObj, asn1SccSeqOfFixedSizeIA5StringFixedSizeAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfFixedSizeIA5StringFixedSizeAlias asn1SccSeqOfSeqOfSeqOfFixedSizeIA5StringFixedSizeAliasObj =
{
    {
        asn1SccSeqOfSeqOfFixedSizeIA5StringFixedSizeAliasObj, asn1SccSeqOfSeqOfFixedSizeIA5StringFixedSizeAliasObj, asn1SccSeqOfSeqOfFixedSizeIA5StringFixedSizeAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfFixedSizeIA5StringVarSizeAlias asn1SccSeqOfFixedSizeIA5StringVarSizeAliasObj =
{
    {
        "AB", "CD", "EF"
    }
};

const asn1SccSeqOfSeqOfFixedSizeIA5StringVarSizeAlias asn1SccSeqOfSeqOfFixedSizeIA5StringVarSizeAliasObj =
{
    {
        asn1SccSeqOfFixedSizeIA5StringVarSizeAliasObj, asn1SccSeqOfFixedSizeIA5StringVarSizeAliasObj, asn1SccSeqOfFixedSizeIA5StringVarSizeAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfFixedSizeIA5StringVarSizeAlias asn1SccSeqOfSeqOfSeqOfFixedSizeIA5StringVarSizeAliasObj =
{
    {
        asn1SccSeqOfSeqOfFixedSizeIA5StringVarSizeAliasObj, asn1SccSeqOfSeqOfFixedSizeIA5StringVarSizeAliasObj, asn1SccSeqOfSeqOfFixedSizeIA5StringVarSizeAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfFixedSizeOctetStringFixedSizeAlias asn1SccSeqOfFixedSizeOctetStringFixedSizeAliasObj =
{
    {
        {{'A', 'B', 'C'}}, {{'D', 'E', 'F'}}, {{'G', 'H', 'I'}}
    }
};

const asn1SccSeqOfSeqOfFixedSizeOctetStringFixedSizeAlias asn1SccSeqOfSeqOfFixedSizeOctetStringFixedSizeAliasObj =
{
    {
        asn1SccSeqOfFixedSizeOctetStringFixedSizeAliasObj, asn1SccSeqOfFixedSizeOctetStringFixedSizeAliasObj, asn1SccSeqOfFixedSizeOctetStringFixedSizeAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfFixedSizeOctetStringFixedSizeAlias asn1SccSeqOfSeqOfSeqOfFixedSizeOctetStringFixedSizeAliasObj =
{
    {
        asn1SccSeqOfSeqOfFixedSizeOctetStringFixedSizeAliasObj, asn1SccSeqOfSeqOfFixedSizeOctetStringFixedSizeAliasObj, asn1SccSeqOfSeqOfFixedSizeOctetStringFixedSizeAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfFixedSizeOctetStringVarSizeAlias asn1SccSeqOfFixedSizeOctetStringVarSizeAliasObj =
{
    {
        {VARIABLE_SIZE_DATA_LENGTH, {'A', 'B'}}, {VARIABLE_SIZE_DATA_LENGTH, {'C', 'D'}}, {VARIABLE_SIZE_DATA_LENGTH, {'E', 'F'}}
    }
};

const asn1SccSeqOfSeqOfFixedSizeOctetStringVarSizeAlias asn1SccSeqOfSeqOfFixedSizeOctetStringVarSizeAliasObj =
{
    {
        asn1SccSeqOfFixedSizeOctetStringVarSizeAliasObj, asn1SccSeqOfFixedSizeOctetStringVarSizeAliasObj, asn1SccSeqOfFixedSizeOctetStringVarSizeAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfFixedSizeOctetStringVarSizeAlias asn1SccSeqOfSeqOfSeqOfFixedSizeOctetStringVarSizeAliasObj =
{
    {
        asn1SccSeqOfSeqOfFixedSizeOctetStringVarSizeAliasObj, asn1SccSeqOfSeqOfFixedSizeOctetStringVarSizeAliasObj, asn1SccSeqOfSeqOfFixedSizeOctetStringVarSizeAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfFixedSizeEnumAlias asn1SccSeqOfFixedSizeEnumAliasObj =
{
    {
        EnumAlias_zero, EnumAlias_one, EnumAlias_two
    }
};

const asn1SccSeqOfSeqOfFixedSizeEnumAlias asn1SccSeqOfSeqOfFixedSizeEnumAliasObj =
{
    {
        asn1SccSeqOfFixedSizeEnumAliasObj, asn1SccSeqOfFixedSizeEnumAliasObj, asn1SccSeqOfFixedSizeEnumAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfFixedSizeEnumAlias asn1SccSeqOfSeqOfSeqOfFixedSizeEnumAliasObj =
{
    {
        asn1SccSeqOfSeqOfFixedSizeEnumAliasObj, asn1SccSeqOfSeqOfFixedSizeEnumAliasObj, asn1SccSeqOfSeqOfFixedSizeEnumAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfFixedSizeSeqAliases asn1SccSeqOfFixedSizeSeqAliasesObj =
{
    {
        {true, 10, 0.1,  "ABC", "XY", {{'A', 'B', 'C'}}, {VARIABLE_SIZE_DATA_LENGTH, {'A', 'B'}}, EnumAlias_zero},
        {false, 20, 0.2, "DEF", "WZ", {{'D', 'E', 'F'}}, {VARIABLE_SIZE_DATA_LENGTH, {'C', 'D'}}, EnumAlias_one},
        {true, 30, 0.3,  "GHI", "TQ", {{'G', 'H', 'I'}}, {VARIABLE_SIZE_DATA_LENGTH, {'E', 'F'}}, EnumAlias_two}
    }
};

const asn1SccSeqOfSeqOfFixedSizeSeqAliases asn1SccSeqOfSeqOfFixedSizeSeqAliasesObj =
{
    {
        asn1SccSeqOfFixedSizeSeqAliasesObj, asn1SccSeqOfFixedSizeSeqAliasesObj, asn1SccSeqOfFixedSizeSeqAliasesObj
    }
};

const asn1SccSeqOfSeqOfSeqOfFixedSizeSeqAliases asn1SccSeqOfSeqOfSeqOfFixedSizeSeqAliasesObj =
{
    {
        asn1SccSeqOfSeqOfFixedSizeSeqAliasesObj, asn1SccSeqOfSeqOfFixedSizeSeqAliasesObj, asn1SccSeqOfSeqOfFixedSizeSeqAliasesObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfFixedSizeSeqBuiltIn asn1SccSeqOfFixedSizeSeqBuiltInObj =
{
    {
        {false, 70, 0.7, "ABC", "XY", {{'A', 'B', 'C'}}, {VARIABLE_SIZE_DATA_LENGTH, {'X', 'Y'}}, SeqBuiltIn_enumMember_zero},
        {true, 80, 0.8, "DEF", "WZ",  {{'D', 'E', 'F'}}, {VARIABLE_SIZE_DATA_LENGTH, {'W', 'Z'}}, SeqBuiltIn_enumMember_one},
        {false, 90, 0.9, "GHI", "TQ", {{'G', 'H', 'I'}}, {VARIABLE_SIZE_DATA_LENGTH, {'T', 'Q'}}, SeqBuiltIn_enumMember_two}
    }
};

const asn1SccSeqOfSeqOfFixedSizeSeqBuiltIn asn1SccSeqOfSeqOfFixedSizeSeqBuiltInObj =
{
    {
        asn1SccSeqOfFixedSizeSeqBuiltInObj, asn1SccSeqOfFixedSizeSeqBuiltInObj, asn1SccSeqOfFixedSizeSeqBuiltInObj
    }
};

const asn1SccSeqOfSeqOfSeqOfFixedSizeSeqBuiltIn asn1SccSeqOfSeqOfSeqOfFixedSizeSeqBuiltInObj =
{
    {
        asn1SccSeqOfSeqOfFixedSizeSeqBuiltInObj, asn1SccSeqOfSeqOfFixedSizeSeqBuiltInObj, asn1SccSeqOfSeqOfFixedSizeSeqBuiltInObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfFixedSizeChoiceAliases asn1SccSeqOfFixedSizeChoiceAliasesObj =
{
    {
        {ChoiceAliases_boolAliasMember_PRESENT,    {.boolAliasMember    = true}},
        {ChoiceAliases_integerAliasMember_PRESENT, {.integerAliasMember = 20}},
        {ChoiceAliases_enumAliasMember_PRESENT,    {.enumAliasMember    = EnumAlias_one}}
    }
};

const asn1SccSeqOfSeqOfFixedSizeChoiceAliases asn1SccSeqOfSeqOfFixedSizeChoiceAliasesObj =
{
    {
        asn1SccSeqOfFixedSizeChoiceAliasesObj, asn1SccSeqOfFixedSizeChoiceAliasesObj, asn1SccSeqOfFixedSizeChoiceAliasesObj
    }
};

const asn1SccSeqOfSeqOfSeqOfFixedSizeChoiceAliases asn1SccSeqOfSeqOfSeqOfFixedSizeChoiceAliasesObj =
{
    {
        asn1SccSeqOfSeqOfFixedSizeChoiceAliasesObj, asn1SccSeqOfSeqOfFixedSizeChoiceAliasesObj, asn1SccSeqOfSeqOfFixedSizeChoiceAliasesObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfFixedSizeChoiceBuiltIn asn1SccSeqOfFixedSizeChoiceBuiltInObj =
{
    {
        {ChoiceBuiltIn_boolMember_PRESENT,    {.boolMember    = true}},
        {ChoiceBuiltIn_integerMember_PRESENT, {.integerMember = 20}},
        {ChoiceBuiltIn_enumMember_PRESENT,    {.enumMember    = ChoiceBuiltIn_enumMember_one}}
    }
};

const asn1SccSeqOfSeqOfFixedSizeChoiceBuiltIn asn1SccSeqOfSeqOfFixedSizeChoiceBuiltInObj =
{
    {
        asn1SccSeqOfFixedSizeChoiceBuiltInObj, asn1SccSeqOfFixedSizeChoiceBuiltInObj, asn1SccSeqOfFixedSizeChoiceBuiltInObj
    }
};

const asn1SccSeqOfSeqOfSeqOfFixedSizeChoiceBuiltIn asn1SccSeqOfSeqOfSeqOfFixedSizeChoiceBuiltInObj =
{
    {
        asn1SccSeqOfSeqOfFixedSizeChoiceBuiltInObj, asn1SccSeqOfSeqOfFixedSizeChoiceBuiltInObj, asn1SccSeqOfSeqOfFixedSizeChoiceBuiltInObj
    }
};
