#!/bin/bash

# This pipeline expects a command to fail, so o and e cannot be set
set -u pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to ASN.1 -i resources/invalid/input-seds.xml \
  --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/

if [ $? -eq 0 ] 
then 
  echo "No failure detected, although it was expected - test fails" 
  exit 1
else 
  echo "Conversion failed as expected"
  rm -r -f $TEST_OUTPUT_DIR
  exit 0
fi
