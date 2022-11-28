#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
XML_VALIDATOR=$SPACECREATOR_BUILD_DIR/bin/xmlvalidator

# diff ignoring white space and blank lines
XMLDIFF="xmldiff"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$SEDS_CONVERTER --from InterfaceView --to SEDS -i resources/test_repeated_interface_name.xml \
    --out $TEST_OUTPUT_DIR/test_repeated_interface_name.xml --iv-config resources/config.xml

cd $TEST_OUTPUT_DIR

cp ../resources/*.xsd . \
  && $XML_VALIDATOR test_repeated_interface_name.xml \
  && $XMLDIFF test_repeated_interface_name.xml ../resources/test_repeated_interface_name.output \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
