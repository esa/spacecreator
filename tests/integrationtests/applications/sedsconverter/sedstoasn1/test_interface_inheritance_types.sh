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
$SEDS_CONVERTER --from SEDS --to ASN.1 --skip-validation -i resources/test_interface_inheritance_types.xml \
    --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/

# Compare and clean-up on success
cd $TEST_OUTPUT_DIR

$DIFF COM-N7SPACE-INTERFACEINHERITANCE1.asn ../resources/test_interface_inheritance_types1_asn.output \
    && $DIFF COM-N7SPACE-INTERFACEINHERITANCE1.acn ../resources/test_interface_inheritance_types1_acn.output \
    && $DIFF COM-N7SPACE-INTERFACEINHERITANCE2.asn ../resources/test_interface_inheritance_types2_asn.output \
    && $DIFF COM-N7SPACE-INTERFACEINHERITANCE2.acn ../resources/test_interface_inheritance_types2_acn.output \
    && $DIFF COM-N7SPACE-INTERFACEINHERITANCE2-COMPONENT.asn ../resources/test_interface_inheritance_types2_component_asn.output \
    && $DIFF COM-N7SPACE-INTERFACEINHERITANCE2-COMPONENT.acn ../resources/test_interface_inheritance_types2_component_acn.output \
    && asn1scc -c COM-N7SPACE-INTERFACEINHERITANCE1.asn COM-N7SPACE-INTERFACEINHERITANCE1.acn \
                  COM-N7SPACE-INTERFACEINHERITANCE2.asn COM-N7SPACE-INTERFACEINHERITANCE2.acn \
                  COM-N7SPACE-INTERFACEINHERITANCE2-COMPONENT.asn COM-N7SPACE-INTERFACEINHERITANCE2-COMPONENT.acn \
    && cd .. \
    && rm -r -f $TEST_OUTPUT_DIR
