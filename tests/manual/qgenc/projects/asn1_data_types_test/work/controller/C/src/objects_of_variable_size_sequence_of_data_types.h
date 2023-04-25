#pragma once

#include "dataview-uniq.h"

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

#define VARIABLE_SIZE_DATA_LENGTH 2

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeBoolAlias asn1SccSeqOfVarSizeBoolAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        true, false
    }
};

const asn1SccSeqOfSeqOfVarSizeBoolAlias asn1SccSeqOfSeqOfVarSizeBoolAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfVarSizeBoolAliasObj, asn1SccSeqOfVarSizeBoolAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeBoolAlias asn1SccSeqOfSeqOfSeqOfVarSizeBoolAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfSeqOfVarSizeBoolAliasObj, asn1SccSeqOfSeqOfVarSizeBoolAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeIntegerAlias asn1SccSeqOfVarSizeIntegerAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        5, 10
    }
};

const asn1SccSeqOfSeqOfVarSizeIntegerAlias asn1SccSeqOfSeqOfVarSizeIntegerAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfVarSizeIntegerAliasObj, asn1SccSeqOfVarSizeIntegerAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeIntegerAlias asn1SccSeqOfSeqOfSeqOfVarSizeIntegerAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfSeqOfVarSizeIntegerAliasObj, asn1SccSeqOfSeqOfVarSizeIntegerAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeRealAlias asn1SccSeqOfVarSizeRealAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        0.00, 0.05
    }
};

const asn1SccSeqOfSeqOfVarSizeRealAlias asn1SccSeqOfSeqOfVarSizeRealAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfVarSizeRealAliasObj, asn1SccSeqOfVarSizeRealAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeRealAlias asn1SccSeqOfSeqOfSeqOfVarSizeRealAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfSeqOfVarSizeRealAliasObj, asn1SccSeqOfSeqOfVarSizeRealAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeIA5StringFixedSizeAlias asn1SccSeqOfVarSizeIA5StringFixedSizeAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        "ABC", "DEF"
    }
};

const asn1SccSeqOfSeqOfVarSizeIA5StringFixedSizeAlias asn1SccSeqOfSeqOfVarSizeIA5StringFixedSizeAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfVarSizeIA5StringFixedSizeAliasObj, asn1SccSeqOfVarSizeIA5StringFixedSizeAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeIA5StringFixedSizeAlias asn1SccSeqOfSeqOfSeqOfVarSizeIA5StringFixedSizeAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfSeqOfVarSizeIA5StringFixedSizeAliasObj, asn1SccSeqOfSeqOfVarSizeIA5StringFixedSizeAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeIA5StringVarSizeAlias asn1SccSeqOfVarSizeIA5StringVarSizeAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        "AB", "CD"
    }
};

const asn1SccSeqOfSeqOfVarSizeIA5StringVarSizeAlias asn1SccSeqOfSeqOfVarSizeIA5StringVarSizeAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfVarSizeIA5StringVarSizeAliasObj, asn1SccSeqOfVarSizeIA5StringVarSizeAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeIA5StringVarSizeAlias asn1SccSeqOfSeqOfSeqOfVarSizeIA5StringVarSizeAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfSeqOfVarSizeIA5StringVarSizeAliasObj, asn1SccSeqOfSeqOfVarSizeIA5StringVarSizeAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeOctetStringFixedSizeAlias asn1SccSeqOfVarSizeOctetStringFixedSizeAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        {{'A', 'B', 'C'}}, {{'D', 'E', 'F'}}
    }
};

const asn1SccSeqOfSeqOfVarSizeOctetStringFixedSizeAlias asn1SccSeqOfSeqOfVarSizeOctetStringFixedSizeAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfVarSizeOctetStringFixedSizeAliasObj, asn1SccSeqOfVarSizeOctetStringFixedSizeAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeOctetStringFixedSizeAlias asn1SccSeqOfSeqOfSeqOfVarSizeOctetStringFixedSizeAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfSeqOfVarSizeOctetStringFixedSizeAliasObj, asn1SccSeqOfSeqOfVarSizeOctetStringFixedSizeAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeOctetStringVarSizeAlias asn1SccSeqOfVarSizeOctetStringVarSizeAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        {VARIABLE_SIZE_DATA_LENGTH, {'A', 'B'}}, {VARIABLE_SIZE_DATA_LENGTH, {'C', 'D'}}
    }
};

const asn1SccSeqOfSeqOfVarSizeOctetStringVarSizeAlias asn1SccSeqOfSeqOfVarSizeOctetStringVarSizeAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfVarSizeOctetStringVarSizeAliasObj, asn1SccSeqOfVarSizeOctetStringVarSizeAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeOctetStringVarSizeAlias asn1SccSeqOfSeqOfSeqOfVarSizeOctetStringVarSizeAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfSeqOfVarSizeOctetStringVarSizeAliasObj, asn1SccSeqOfSeqOfVarSizeOctetStringVarSizeAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeEnumAlias asn1SccSeqOfVarSizeEnumAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        EnumAlias_zero, EnumAlias_one
    }
};

const asn1SccSeqOfSeqOfVarSizeEnumAlias asn1SccSeqOfSeqOfVarSizeEnumAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfVarSizeEnumAliasObj, asn1SccSeqOfVarSizeEnumAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeEnumAlias asn1SccSeqOfSeqOfSeqOfVarSizeEnumAliasObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfSeqOfVarSizeEnumAliasObj, asn1SccSeqOfSeqOfVarSizeEnumAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeSeqAliases asn1SccSeqOfVarSizeSeqAliasesObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        {true, 10, 0.1, "ABC", "XY",  {{'A', 'B', 'C'}}, {VARIABLE_SIZE_DATA_LENGTH, {'X', 'Y'}}, EnumAlias_zero},
        {false, 20, 0.2, "DEF", "WZ", {{'D', 'E', 'F'}}, {VARIABLE_SIZE_DATA_LENGTH, {'W', 'Z'}}, EnumAlias_one}
    }
};

const asn1SccSeqOfSeqOfVarSizeSeqAliases asn1SccSeqOfSeqOfVarSizeSeqAliasesObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfVarSizeSeqAliasesObj, asn1SccSeqOfVarSizeSeqAliasesObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeSeqAliases asn1SccSeqOfSeqOfSeqOfVarSizeSeqAliasesObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfSeqOfVarSizeSeqAliasesObj, asn1SccSeqOfSeqOfVarSizeSeqAliasesObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeSeqBuiltIn asn1SccSeqOfVarSizeSeqBuiltInObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        {false, 70, 0.7, "ABC", "XY", {{'A', 'B', 'C'}}, {VARIABLE_SIZE_DATA_LENGTH, {'X', 'Y'}}, SeqBuiltIn_enumMember_zero},
        {true, 80, 0.8, "DEF", "WZ",  {{'D', 'E', 'F'}}, {VARIABLE_SIZE_DATA_LENGTH, {'W', 'Z'}}, SeqBuiltIn_enumMember_one}
    }
};

const asn1SccSeqOfSeqOfVarSizeSeqBuiltIn asn1SccSeqOfSeqOfVarSizeSeqBuiltInObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfVarSizeSeqBuiltInObj, asn1SccSeqOfVarSizeSeqBuiltInObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeSeqBuiltIn asn1SccSeqOfSeqOfSeqOfVarSizeSeqBuiltInObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfSeqOfVarSizeSeqBuiltInObj, asn1SccSeqOfSeqOfVarSizeSeqBuiltInObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeChoiceAliases asn1SccSeqOfVarSizeChoiceAliasesObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        {ChoiceAliases_boolAliasMember_PRESENT,    {.boolAliasMember    = true}},
        {ChoiceAliases_integerAliasMember_PRESENT, {.integerAliasMember = 20}}
    }
};

const asn1SccSeqOfSeqOfVarSizeChoiceAliases asn1SccSeqOfSeqOfVarSizeChoiceAliasesObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfVarSizeChoiceAliasesObj, asn1SccSeqOfVarSizeChoiceAliasesObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeChoiceAliases asn1SccSeqOfSeqOfSeqOfVarSizeChoiceAliasesObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfSeqOfVarSizeChoiceAliasesObj, asn1SccSeqOfSeqOfVarSizeChoiceAliasesObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeChoiceBuiltIn asn1SccSeqOfVarSizeChoiceBuiltInObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        {ChoiceBuiltIn_boolMember_PRESENT,    {.boolMember    = true}},
        {ChoiceBuiltIn_integerMember_PRESENT, {.integerMember = 20}}
    }
};

const asn1SccSeqOfSeqOfVarSizeChoiceBuiltIn asn1SccSeqOfSeqOfVarSizeChoiceBuiltInObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfVarSizeChoiceBuiltInObj, asn1SccSeqOfVarSizeChoiceBuiltInObj, asn1SccSeqOfVarSizeChoiceBuiltInObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeChoiceBuiltIn asn1SccSeqOfSeqOfSeqOfVarSizeChoiceBuiltInObj =
{
    VARIABLE_SIZE_DATA_LENGTH,
    {
        asn1SccSeqOfSeqOfVarSizeChoiceBuiltInObj, asn1SccSeqOfSeqOfVarSizeChoiceBuiltInObj, asn1SccSeqOfSeqOfVarSizeChoiceBuiltInObj
    }
};
