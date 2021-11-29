#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
OPENGEODE=$HOME/.local/bin/opengeode

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to SDL --aux-models ASN.1,InterfaceView --skip-validation -i resources/test_comparator.xml \
  --out $TEST_OUTPUT_DIR/comparator.xml --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
  --sdl-filepath-prefix $TEST_OUTPUT_DIR/
# Setup additional data
cp resources/test_comparator.system_structure output/system_structure.pr
cp $TEST_OUTPUT_DIR/COMPARATOR.asn $TEST_OUTPUT_DIR/dataview-uniq.asn
# Rename the module to avoid naming conflicts
sed -i 's/COMPARATOR/SYSTEM-DATAVIEW/g' $TEST_OUTPUT_DIR/dataview-uniq.asn
cd $TEST_OUTPUT_DIR
# Compare output against reference, and compile to make sure the reference is valid
# Clean (rm) only if all steps pass
# This test uses Ada, as C backend in OpenGEODE is too buggy to handle this example
$DIFF Comparator.pr ../resources/test_comparator.output \
  && $OPENGEODE --toAda system_structure.pr Comparator.pr \
  && asn1scc -Ada --type-prefix asn1Scc dataview-uniq.asn comparator_datamodel.asn \
  && gcc -c comparator.adb \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
