#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
# diff ignoring white space and blank lines
DIFF="diff -w -B"
XMLDIFF="xmldiff --check --ignored-attrs $(cat ../ignored_attributes.txt)"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir -p $TEST_OUTPUT_DIR
# Translate
$SEDS_CONVERTER --from SEDS --to InterfaceView --skip-validation -i resources/test_descriptions.xml \
  --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config resources/config.xml

cd $TEST_OUTPUT_DIR

# Execute commands in chain to make sure that the generated interface view matches
# the reference and allows to succesfully generate derived artefacts
$XMLDIFF interfaceview.xml ../resources/test_descriptions.output \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
