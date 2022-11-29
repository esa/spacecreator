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
$SEDS_CONVERTER --from ASN.1 --to SEDS -i resources/test_with_data_types.asn,resources/test_with_data_types.acn \
    --out $TEST_OUTPUT_DIR/datatypes.xml
$SEDS_CONVERTER --from InterfaceView --to SEDS -i resources/test_sporadic_interface.xml \
    --out $TEST_OUTPUT_DIR/test_with_data_types.xml --iv-config resources/config.xml \
    --iv-data-types-seds-filepath datatypes.xml

cd $TEST_OUTPUT_DIR

cp ../resources/*.xsd . \
  && $XML_VALIDATOR test_with_data_types.xml \
  && $XMLDIFF test_with_data_types.xml ../resources/test_with_data_types.output \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
