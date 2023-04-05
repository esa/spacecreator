#pragma once

#include "dataview-uniq.h"

#include "objects_of_fixed_size_sequence_of_data_types.h"
#include "objects_of_variable_size_sequence_of_data_types.h"

// ---------------------------------------------------------------------
// ---------------------------------------------------------------------

const asn1SccChoiceAliases           asn1SccChoiceAliasesObj =
{
    ChoiceAliases_integerAliasMember_PRESENT,
    {.integerAliasMember = 10}
};

const asn1SccChoiceBuiltIn           asn1SccChoiceBuiltInObj =
{
    ChoiceBuiltIn_integerMember_PRESENT,
    {.integerMember = 10}
};

const asn1SccChoiceOfFixedSizeSeqOfs asn1SccChoiceOfFixedSizeSeqOfsObj =
{
    ChoiceOfFixedSizeSeqOfs_seqOfSeqOfSeqOfFixedSizeIntegerAliasMember_PRESENT,
    {.seqOfSeqOfSeqOfFixedSizeIntegerAliasMember = asn1SccSeqOfSeqOfSeqOfFixedSizeIntegerAliasObj}
};

const asn1SccChoiceOfVarSizeSeqOfs   asn1SccChoiceOfVarSizeSeqOfsObj =
{
    ChoiceOfVarSizeSeqOfs_seqOfSeqOfSeqOfVarSizeIntegerAliasMember_PRESENT,
    {.seqOfSeqOfSeqOfVarSizeIntegerAliasMember = asn1SccSeqOfSeqOfSeqOfVarSizeIntegerAliasObj}
};
