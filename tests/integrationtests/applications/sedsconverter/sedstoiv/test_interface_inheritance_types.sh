#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
AADL_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/aadlconverter
UPDATE_DATAVIEW="asn2aadlPlus -f dataview-uniq.asn DataView.aadl -aadlv2"
# diff ignoring white space and blank lines
DIFF="diff -w -B"
XMLDIFF="xmldiff"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR

# Translate
$SEDS_CONVERTER --from SEDS --to InterfaceView --aux-models ASN.1 --skip-validation -i resources/test_interface_inheritance_types.xml \
    --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config resources/config.xml \
    --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/

# Compare and clean-up on success
cd $TEST_OUTPUT_DIR

# Merge ASN files
sed -e '$s/$/\n/' -s COM-N7SPACE-INTERFACEINHERITANCE1.asn \
                     COM-N7SPACE-INTERFACEINHERITANCE2.asn COM-N7SPACE-INTERFACEINHERITANCE2-COMPONENT.asn> dataview-uniq.asn
sed -e '$s/$/\n/' -s COM-N7SPACE-INTERFACEINHERITANCE1.acn \
                     COM-N7SPACE-INTERFACEINHERITANCE2.acn COM-N7SPACE-INTERFACEINHERITANCE2-COMPONENT.acn> dataview-uniq.acn

$XMLDIFF interfaceview.xml ../resources/test_interface_inheritance_types.output \
    && $UPDATE_DATAVIEW \
    && $AADL_CONVERTER -o interfaceview.xml \
    -t ../resources/xml2dv/interfaceview.tmplt \
    -x DeploymentView.aadl \
    && $AADL_CONVERTER -o interfaceview.xml \
    -t ../resources/xml2iv/interfaceview.tmplt \
    -x InterfaceView.aadl \
    && cd .. \
    && rm -r -f $TEST_OUTPUT_DIR
