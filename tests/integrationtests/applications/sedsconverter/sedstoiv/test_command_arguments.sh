#!/bin/bash

set -euo pipefail

SEDS_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/sedsconverter
AADL_CONVERTER=$SPACECREATOR_BUILD_DIR/bin/aadlconverter
UPDATE_DATAVIEW="asn2aadlPlus -f output.asn DataView.aadl -aadlv2"
# diff ignoring white space and blank lines
DIFF="diff -w -B"
XMLDIFF="xmldiff --ignored-attrs $(cat ../ignored_attributes.txt)"
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
$XMLDIFF interfaceview.xml ../resources/test_command_arguments.output \
  && $DIFF COM-N7SPACE-COMMANDARGUMENTS.asn ../resources/test_command_arguments.asn \
  && $DIFF COM-N7SPACE-COMMANDARGUMENTS-COMPONENT.asn ../resources/test_command_arguments_component.asn \
  && asn1scc -c COM-N7SPACE-COMMANDARGUMENTS.asn COM-N7SPACE-COMMANDARGUMENTS.acn COM-N7SPACE-COMMANDARGUMENTS-COMPONENT.asn COM-N7SPACE-COMMANDARGUMENTS-COMPONENT.acn\
  && cd .. \
  && rm -r -f $TEST_OUTPUT_DIR
