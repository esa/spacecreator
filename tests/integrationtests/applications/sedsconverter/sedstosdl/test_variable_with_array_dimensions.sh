#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
OPENGEODE=$HOME/.local/bin/opengeode

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output_test_variable_with_array_dimensions

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR

# Translate
$SEDS_CONVERTER --from SEDS --to SDL --aux-models ASN.1,InterfaceView --skip-validation -i resources/test_variable_with_array_dimensions.xml \
  --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
  --seds-array-dimension-indexing-type MyIndexType \
  --sdl-filepath-prefix $TEST_OUTPUT_DIR/

# Setup additional data
cp resources/test_variable_with_array_dimensions.system_structure output/test_variable_with_array_dimensions.system_structure
cp $TEST_OUTPUT_DIR/VARIABLEWITHARRAYDIMENSIONS.asn $TEST_OUTPUT_DIR/dataview-uniq.asn

# Rename the module to avoid naming conflicts
sed -i 's/VARIABLEWITHARRAYDIMENSIONS/SYSTEM-DATAVIEW/g' $TEST_OUTPUT_DIR/dataview-uniq.asn

cd $TEST_OUTPUT_DIR

# Compare output against reference, and compile to make sure the reference is valid
# Clean (rm) only if all steps pass
# This test uses Ada, as C backend in OpenGEODE is too buggy to handle this example
$DIFF component.pr ../resources/test_variable_with_array_dimensions.output \
    && $OPENGEODE --toAda test_variable_with_array_dimensions.system_structure component.pr \
    && asn1scc -Ada --type-prefix asn1Scc -equal dataview-uniq.asn component_datamodel.asn \
    && gcc -c component.adb \
    && cd .. \
    && rm -r -f $TEST_OUTPUT_DIR
