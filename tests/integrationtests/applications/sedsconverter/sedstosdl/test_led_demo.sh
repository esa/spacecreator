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
$SEDS_CONVERTER --from SEDS --to SDL --aux-models ASN.1,InterfaceView --skip-validation -i resources/test_led_demo.xml \
  --out $TEST_OUTPUT_DIR/led_demo.xml --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
  --sdl-filepath-prefix $TEST_OUTPUT_DIR/
# Setup additional data
cp resources/test_led_demo.system_structure output/system_structure.pr
cp $TEST_OUTPUT_DIR/COM-N7SPACE-LEDDEMO.asn $TEST_OUTPUT_DIR/dataview-uniq.asn
cd $TEST_OUTPUT_DIR
# Compare output against reference, and compile to make sure the reference is valid
# Clean (rm) only if all steps pass
$DIFF LedDemo.pr ../resources/test_led_demo.output \
  && $OPENGEODE --toC system_structure.pr LedDemo.pr \
  && asn1scc -c --type-prefix asn1Scc dataview-uniq.asn \
  && gcc -c LedDemo.c \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
