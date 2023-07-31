#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
OPENGEODE=opengeode

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR

# Translate
$SEDS_CONVERTER --from SEDS --to SDL --aux-models ASN.1,InterfaceView --skip-validation --taste --no-mangling \
  -i resources/test_seds_sdl_taste_translation.xml --out $TEST_OUTPUT_DIR/test_seds_sdl_taste_translation.xml \
  --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ --sdl-filepath-prefix $TEST_OUTPUT_DIR/

# Setup additional data
cp resources/test_seds_sdl_taste_trans.system_structure output/system_structure.pr
cp $TEST_OUTPUT_DIR/FUNCTION-1.asn $TEST_OUTPUT_DIR/dataview-uniq.asn

# Compare output against reference, generate ada code, and compile to make sure the reference is valid
# Clean (rm) only if all steps pass
cd $TEST_OUTPUT_DIR
$DIFF function_1.pr ../resources/test_seds_sdl_taste_trans.output \
  && $OPENGEODE --taste --toAda function_1.pr system_structure.pr \
  && asn1scc -Ada --type-prefix asn1Scc dataview-uniq.asn function_1_datamodel.asn \
  && gcc -c function_1.adb \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
