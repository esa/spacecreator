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
$SEDS_CONVERTER --from InterfaceView --to SEDS -i resources/test_protected_interface.xml \
    --out $TEST_OUTPUT_DIR/test_protected_interface.xml --iv-config resources/config.xml

cd $TEST_OUTPUT_DIR

cp ../resources/*.xsd . \
  && $XML_VALIDATOR test_protected_interface.xml \
  && $XMLDIFF test_protected_interface.xml ../resources/test_protected_interface.output \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
