#pragma once

#include "dataview-uniq.h"

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeBoolAlias asn1SccSeqOfVarSizeBoolAliasObj =
{
    2,
    {
        true, false
    }
};

const asn1SccSeqOfSeqOfVarSizeBoolAlias asn1SccSeqOfSeqOfVarSizeBoolAliasObj =
{
    2,
    {
        asn1SccSeqOfVarSizeBoolAliasObj, asn1SccSeqOfVarSizeBoolAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeBoolAlias asn1SccSeqOfSeqOfSeqOfVarSizeBoolAliasObj =
{
    2,
    {
        asn1SccSeqOfSeqOfVarSizeBoolAliasObj, asn1SccSeqOfSeqOfVarSizeBoolAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeIntegerAlias asn1SccSeqOfVarSizeIntegerAliasObj =
{
    2,
    {
        5, 10
    }
};

const asn1SccSeqOfSeqOfVarSizeIntegerAlias asn1SccSeqOfSeqOfVarSizeIntegerAliasObj =
{
    2,
    {
        asn1SccSeqOfVarSizeIntegerAliasObj, asn1SccSeqOfVarSizeIntegerAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeIntegerAlias asn1SccSeqOfSeqOfSeqOfVarSizeIntegerAliasObj =
{
    2,
    {
        asn1SccSeqOfSeqOfVarSizeIntegerAliasObj, asn1SccSeqOfSeqOfVarSizeIntegerAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeRealAlias asn1SccSeqOfVarSizeRealAliasObj =
{
    2,
    {
        0.00, 0.05
    }
};

const asn1SccSeqOfSeqOfVarSizeRealAlias asn1SccSeqOfSeqOfVarSizeRealAliasObj =
{
    2,
    {
        asn1SccSeqOfVarSizeRealAliasObj, asn1SccSeqOfVarSizeRealAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeRealAlias asn1SccSeqOfSeqOfSeqOfVarSizeRealAliasObj =
{
    2,
    {
        asn1SccSeqOfSeqOfVarSizeRealAliasObj, asn1SccSeqOfSeqOfVarSizeRealAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeIA5StringFixedSizeAlias asn1SccSeqOfVarSizeIA5StringFixedSizeAliasObj =
{
    2,
    {
        "ABC", "DEF"
    }
};

const asn1SccSeqOfSeqOfVarSizeIA5StringFixedSizeAlias asn1SccSeqOfSeqOfVarSizeIA5StringFixedSizeAliasObj =
{
    2,
    {
        asn1SccSeqOfVarSizeIA5StringFixedSizeAliasObj, asn1SccSeqOfVarSizeIA5StringFixedSizeAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeIA5StringFixedSizeAlias asn1SccSeqOfSeqOfSeqOfVarSizeIA5StringFixedSizeAliasObj =
{
    2,
    {
        asn1SccSeqOfSeqOfVarSizeIA5StringFixedSizeAliasObj, asn1SccSeqOfSeqOfVarSizeIA5StringFixedSizeAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeIA5StringVarSizeAlias asn1SccSeqOfVarSizeIA5StringVarSizeAliasObj =
{
    2,
    {
        "AB", "CD"
    }
};

const asn1SccSeqOfSeqOfVarSizeIA5StringVarSizeAlias asn1SccSeqOfSeqOfVarSizeIA5StringVarSizeAliasObj =
{
    2,
    {
        asn1SccSeqOfVarSizeIA5StringVarSizeAliasObj, asn1SccSeqOfVarSizeIA5StringVarSizeAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeIA5StringVarSizeAlias asn1SccSeqOfSeqOfSeqOfVarSizeIA5StringVarSizeAliasObj =
{
    2,
    {
        asn1SccSeqOfSeqOfVarSizeIA5StringVarSizeAliasObj, asn1SccSeqOfSeqOfVarSizeIA5StringVarSizeAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeOctetStringFixedSizeAlias asn1SccSeqOfVarSizeOctetStringFixedSizeAliasObj =
{
    2,
    {
        {{'A', 'B', 'C'}}, {{'D', 'E', 'F'}}
    }
};

const asn1SccSeqOfSeqOfVarSizeOctetStringFixedSizeAlias asn1SccSeqOfSeqOfVarSizeOctetStringFixedSizeAliasObj =
{
    2,
    {
        asn1SccSeqOfVarSizeOctetStringFixedSizeAliasObj, asn1SccSeqOfVarSizeOctetStringFixedSizeAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeOctetStringFixedSizeAlias asn1SccSeqOfSeqOfSeqOfVarSizeOctetStringFixedSizeAliasObj =
{
    2,
    {
        asn1SccSeqOfSeqOfVarSizeOctetStringFixedSizeAliasObj, asn1SccSeqOfSeqOfVarSizeOctetStringFixedSizeAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeOctetStringVarSizeAlias asn1SccSeqOfVarSizeOctetStringVarSizeAliasObj =
{
    2,
    {
        {2, {'A', 'B'}}, {2, {'C', 'D'}}
    }
};

const asn1SccSeqOfSeqOfVarSizeOctetStringVarSizeAlias asn1SccSeqOfSeqOfVarSizeOctetStringVarSizeAliasObj =
{
    2,
    {
        asn1SccSeqOfVarSizeOctetStringVarSizeAliasObj, asn1SccSeqOfVarSizeOctetStringVarSizeAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeOctetStringVarSizeAlias asn1SccSeqOfSeqOfSeqOfVarSizeOctetStringVarSizeAliasObj =
{
    2,
    {
        asn1SccSeqOfSeqOfVarSizeOctetStringVarSizeAliasObj, asn1SccSeqOfSeqOfVarSizeOctetStringVarSizeAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeEnumAlias asn1SccSeqOfVarSizeEnumAliasObj =
{
    2,
    {
        EnumAlias_zero, EnumAlias_one
    }
};

const asn1SccSeqOfSeqOfVarSizeEnumAlias asn1SccSeqOfSeqOfVarSizeEnumAliasObj =
{
    2,
    {
        asn1SccSeqOfVarSizeEnumAliasObj, asn1SccSeqOfVarSizeEnumAliasObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeEnumAlias asn1SccSeqOfSeqOfSeqOfVarSizeEnumAliasObj =
{
    2,
    {
        asn1SccSeqOfSeqOfVarSizeEnumAliasObj, asn1SccSeqOfSeqOfVarSizeEnumAliasObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeSeqAliases asn1SccSeqOfVarSizeSeqAliasesObj =
{
    2,
    {
        {true, 10, 0.1, "ABC", "XY",  {{'A', 'B', 'C'}}, {2, {'X', 'Y'}}, EnumAlias_zero},
        {false, 20, 0.2, "DEF", "WZ", {{'D', 'E', 'F'}}, {2, {'W', 'Z'}}, EnumAlias_one}
    }
};

const asn1SccSeqOfSeqOfVarSizeSeqAliases asn1SccSeqOfSeqOfVarSizeSeqAliasesObj =
{
    2,
    {
        asn1SccSeqOfVarSizeSeqAliasesObj, asn1SccSeqOfVarSizeSeqAliasesObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeSeqAliases asn1SccSeqOfSeqOfSeqOfVarSizeSeqAliasesObj =
{
    2,
    {
        asn1SccSeqOfSeqOfVarSizeSeqAliasesObj, asn1SccSeqOfSeqOfVarSizeSeqAliasesObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeSeqBuiltIn asn1SccSeqOfVarSizeSeqBuiltInObj =
{
    2,
    {
        {false, 70, 0.7, "ABC", "XY", {{'A', 'B', 'C'}}, {2, {'X', 'Y'}}, SeqBuiltIn_enumMember_zero},
        {true, 80, 0.8, "DEF", "WZ",  {{'D', 'E', 'F'}}, {2, {'W', 'Z'}}, SeqBuiltIn_enumMember_one}
    }
};

const asn1SccSeqOfSeqOfVarSizeSeqBuiltIn asn1SccSeqOfSeqOfVarSizeSeqBuiltInObj =
{
    2,
    {
        asn1SccSeqOfVarSizeSeqBuiltInObj, asn1SccSeqOfVarSizeSeqBuiltInObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeSeqBuiltIn asn1SccSeqOfSeqOfSeqOfVarSizeSeqBuiltInObj =
{
    2,
    {
        asn1SccSeqOfSeqOfVarSizeSeqBuiltInObj, asn1SccSeqOfSeqOfVarSizeSeqBuiltInObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeChoiceAliases asn1SccSeqOfVarSizeChoiceAliasesObj =
{
    2,
    {
        {ChoiceAliases_boolAliasMember_PRESENT,    {.boolAliasMember    = true}},
        {ChoiceAliases_integerAliasMember_PRESENT, {.integerAliasMember = 20}}
    }
};

const asn1SccSeqOfSeqOfVarSizeChoiceAliases asn1SccSeqOfSeqOfVarSizeChoiceAliasesObj =
{
    2,
    {
        asn1SccSeqOfVarSizeChoiceAliasesObj, asn1SccSeqOfVarSizeChoiceAliasesObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeChoiceAliases asn1SccSeqOfSeqOfSeqOfVarSizeChoiceAliasesObj =
{
    2,
    {
        asn1SccSeqOfSeqOfVarSizeChoiceAliasesObj, asn1SccSeqOfSeqOfVarSizeChoiceAliasesObj
    }
};

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqOfVarSizeChoiceBuiltIn asn1SccSeqOfVarSizeChoiceBuiltInObj =
{
    2,
    {
        {ChoiceBuiltIn_boolMember_PRESENT,    {.boolMember    = true}},
        {ChoiceBuiltIn_integerMember_PRESENT, {.integerMember = 20}}
    }
};

const asn1SccSeqOfSeqOfVarSizeChoiceBuiltIn asn1SccSeqOfSeqOfVarSizeChoiceBuiltInObj =
{
    2,
    {
        asn1SccSeqOfVarSizeChoiceBuiltInObj, asn1SccSeqOfVarSizeChoiceBuiltInObj, asn1SccSeqOfVarSizeChoiceBuiltInObj
    }
};

const asn1SccSeqOfSeqOfSeqOfVarSizeChoiceBuiltIn asn1SccSeqOfSeqOfSeqOfVarSizeChoiceBuiltInObj =
{
    2,
    {
        asn1SccSeqOfSeqOfVarSizeChoiceBuiltInObj, asn1SccSeqOfSeqOfVarSizeChoiceBuiltInObj, asn1SccSeqOfSeqOfVarSizeChoiceBuiltInObj
    }
};
