#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
XML_VALIDATOR=$SPACECREATOR_BUILD_DIR/bin/xmlvalidator

# diff ignoring white space and blank lines
XMLDIFF="xmldiff --check"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$SEDS_CONVERTER --in resources/test_function_to_convert_interfaceview.xml --from InterfaceView --to SEDS --out $TEST_OUTPUT_DIR/function_1.xml \
    --iv-config resources/config.xml --function-to-convert Function_1

cd $TEST_OUTPUT_DIR

cp ../resources/*.xsd . \
  && $XML_VALIDATOR function_1.xml \
  && $XMLDIFF function_1.xml ../resources/test_function_to_convert.output \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR