#!/bin/bash

# This pipeline expects a command to fail, so o and e cannot be set
set -u pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
TEST_OUTPUT_DIR=output_test_validation_disabled

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to ASN.1 -i resources/validation-disabled/input-seds.xml \
  --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/ \
  --skip-validation

if [ $? -ne 0 ]
then
  echo "Failure detected when validation should be disabled"
  exit 1
else
  echo "Conversion successful as expected"
  rm -r -f $TEST_OUTPUT_DIR
  exit 0
fi
