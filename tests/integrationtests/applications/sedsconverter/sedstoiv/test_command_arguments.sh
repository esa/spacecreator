#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
AADL_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/aadlconverter
UPDATE_DATAVIEW="asn2aadlPlus -f output.asn DataView.aadl -aadlv2"
# diff ignoring white space and blank lines
DIFF="diff -w -B"
TEST_OUTPUT_DIR=output

echo "Running SedsConverter test: ${0##*/}'"

# Setup output dir and project
rm -r -f $TEST_OUTPUT_DIR
mkdir $TEST_OUTPUT_DIR

# Translate
$SEDS_CONVERTER --from SEDS --to InterfaceView --aux-models ASN.1 --skip-validation -i resources/test_command_arguments.xml \
  --out $TEST_OUTPUT_DIR/interfaceview.xml --iv-config resources/config.xml --asn1-filepath-prefix $TEST_OUTPUT_DIR/ --acn-filepath-prefix $TEST_OUTPUT_DIR/

cd $TEST_OUTPUT_DIR

# Execute commands in chain to make sure that the generated interface view matches
# the reference and allows to succesfully generate derived artefacts
$DIFF interfaceview.xml ../resources/test_command_arguments.output \
  && $DIFF COM-N7SPACE-COMMANDARGUMENTS.asn ../resources/test_command_arguments.asn \
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
