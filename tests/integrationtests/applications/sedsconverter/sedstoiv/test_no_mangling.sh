#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
# grep match only whole words and be quiet
GREP="grep -w -q"
PI_1="PI_1"
PI_2="PI_2"
TEST_OUTPUT_DIR=output_test_no_mangling

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR

#Translate iv_input to seds
$SEDS_CONVERTER --from InterfaceView --to SEDS --in resources/test_no_mangling_iv_input.xml --iv-config resources/config.xml \
    --out $TEST_OUTPUT_DIR/seds.xml

#Translate seds to iv with no mangling option
$SEDS_CONVERTER --from SEDS --to InterfaceView --skip-validation -i $TEST_OUTPUT_DIR/seds.xml --iv-config resources/config.xml \
    --out $TEST_OUTPUT_DIR/iv.xml --no-mangling

#test produced iv for expected outputs and clean up
$GREP $PI_1 $TEST_OUTPUT_DIR/iv.xml \
  && $GREP $PI_2 $TEST_OUTPUT_DIR/iv.xml \
  && rm -r -f $TEST_OUTPUT_DIR
