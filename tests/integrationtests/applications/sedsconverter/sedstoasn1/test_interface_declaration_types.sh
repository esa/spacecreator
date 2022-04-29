#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR

# Translate
# gdb --args \
$SEDS_CONVERTER --from SEDS --to ASN.1 --skip-validation -i resources/test_interface_declaration_types.xml \
    --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/

# Compare and clean-up on success
cd $TEST_OUTPUT_DIR

$DIFF COM-N7SPACE-INTERFACEDECLARATIONS.asn ../resources/test_interface_declaration_types_asn.output \
    && $DIFF COM-N7SPACE-INTERFACEDECLARATIONS.acn ../resources/test_interface_declaration_types_acn.output \
    && $DIFF COM-N7SPACE-INTERFACEDECLARATIONS-COMPONENT.asn ../resources/test_interface_declaration_types_component_asn.output \
    && $DIFF COM-N7SPACE-INTERFACEDECLARATIONS-COMPONENT.acn ../resources/test_interface_declaration_types_component_acn.output \
    && asn1scc -c COM-N7SPACE-INTERFACEDECLARATIONS.asn COM-N7SPACE-INTERFACEDECLARATIONS.acn COM-N7SPACE-INTERFACEDECLARATIONS-COMPONENT.asn COM-N7SPACE-INTERFACEDECLARATIONS-COMPONENT.acn \
    && cd .. \
    && rm -r -f $TEST_OUTPUT_DIR
