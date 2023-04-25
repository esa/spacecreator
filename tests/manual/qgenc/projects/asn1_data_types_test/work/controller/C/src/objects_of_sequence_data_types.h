#pragma once

#include "dataview-uniq.h"

#include "objects_of_fixed_size_sequence_of_data_types.h"
#include "objects_of_variable_size_sequence_of_data_types.h"

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccSeqAliases                               asn1SccSeqAliasesObj =
{
    true,
    100,
    0.5,
    "ABC",
    "XY",
    {{'A', 'B', 'C'}},
    {VARIABLE_SIZE_DATA_LENGTH, {'X', 'Y'}},
    EnumAlias_zero
};

const asn1SccSeqBuiltIn                               asn1SccSeqBuiltInObj =
{
    false,
    50,
    0.2,
    "DEF",
    "WZ",
    {{'D', 'E', 'F'}},
    {VARIABLE_SIZE_DATA_LENGTH, {'W', 'Z'}},
    SeqBuiltIn_enumMember_zero
};

const asn1SccSeqOfFixedSizeSeqOfsProvidedInterface    asn1SccSeqOfFixedSizeSeqOfsProvidedInterfaceObj =
{
    asn1SccSeqOfSeqOfSeqOfFixedSizeBoolAliasObj,
    asn1SccSeqOfSeqOfSeqOfFixedSizeIntegerAliasObj,
    asn1SccSeqOfSeqOfSeqOfFixedSizeRealAliasObj,
    asn1SccSeqOfSeqOfSeqOfFixedSizeIA5StringFixedSizeAliasObj,
    asn1SccSeqOfSeqOfSeqOfFixedSizeIA5StringVarSizeAliasObj,
    asn1SccSeqOfSeqOfSeqOfFixedSizeOctetStringFixedSizeAliasObj,
    asn1SccSeqOfSeqOfSeqOfFixedSizeOctetStringVarSizeAliasObj,
    asn1SccSeqOfSeqOfSeqOfFixedSizeEnumAliasObj,
    asn1SccSeqOfSeqOfSeqOfFixedSizeSeqAliasesObj,
    asn1SccSeqOfSeqOfSeqOfFixedSizeSeqBuiltInObj,
    asn1SccSeqOfSeqOfSeqOfFixedSizeChoiceAliasesObj,
    asn1SccSeqOfSeqOfSeqOfFixedSizeChoiceBuiltInObj
};

const asn1SccSeqOfVarSizeSeqOfsProvidedInterface      asn1SccSeqOfVarSizeSeqOfsProvidedInterfaceObj =
{
    asn1SccSeqOfSeqOfSeqOfVarSizeBoolAliasObj,
    asn1SccSeqOfSeqOfSeqOfVarSizeIntegerAliasObj,
    asn1SccSeqOfSeqOfSeqOfVarSizeRealAliasObj,
    asn1SccSeqOfSeqOfSeqOfVarSizeIA5StringFixedSizeAliasObj,
    asn1SccSeqOfSeqOfSeqOfVarSizeIA5StringVarSizeAliasObj,
    asn1SccSeqOfSeqOfSeqOfVarSizeOctetStringFixedSizeAliasObj,
    asn1SccSeqOfSeqOfSeqOfVarSizeOctetStringVarSizeAliasObj,
    asn1SccSeqOfSeqOfSeqOfVarSizeEnumAliasObj,
    asn1SccSeqOfSeqOfSeqOfVarSizeSeqAliasesObj,
    asn1SccSeqOfSeqOfSeqOfVarSizeSeqBuiltInObj,
    asn1SccSeqOfSeqOfSeqOfVarSizeChoiceAliasesObj,
    asn1SccSeqOfSeqOfSeqOfVarSizeChoiceBuiltInObj
};
