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
$SEDS_CONVERTER --from SEDS --to SDL --aux-models ASN.1 --skip-validation -i resources/test_guard.xml \
  --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
  --sdl-filepath-prefix $TEST_OUTPUT_DIR/
# Setup additional data
cp resources/test_guard.system_structure output/system_structure.pr
cp $TEST_OUTPUT_DIR/GUARD.asn $TEST_OUTPUT_DIR/dataview-uniq.asn
cd $TEST_OUTPUT_DIR
# Compare and clean-up on success
$OPENGEODE --toAda system_structure.pr Component.pr \

#diff $TEST_OUTPUT_DIR/Component.pr resources/state_machine_with_bare_inputs.output

# && rm -r -f $TEST_OUTPUT_DIR
