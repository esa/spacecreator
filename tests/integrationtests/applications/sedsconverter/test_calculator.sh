#!/bin/bash

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
OPENGEODE=$HOME/.local/bin/opengeode

# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to SDL --aux-models ASN.1,InterfaceView --skip-validation -i resources/test_calculator.xml \
  --out $TEST_OUTPUT_DIR/calculator.xml --iv-config config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
  --sdl-filepath-prefix $TEST_OUTPUT_DIR/
# Setup additional data
cp resources/test_calculator.system_structure output/system_structure.pr
cp $TEST_OUTPUT_DIR/CALCULATOR.asn $TEST_OUTPUT_DIR/dataview-uniq.asn
cd $TEST_OUTPUT_DIR
# Compare output against reference, and compile to make sure the reference is valid
# Clean (rm) only if all steps pass
$DIFF Calculator.pr ../resources/test_calculator.output \
  && $OPENGEODE --toC system_structure.pr Calculator.pr \
  && asn1scc -c --type-prefix asn1Scc dataview-uniq.asn \
  && gcc -c Calculator.c \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
